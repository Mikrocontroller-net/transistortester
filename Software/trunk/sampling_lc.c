
// June-Sept 2015, pa3fwm@amsat.org

#ifdef SamplingADC

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

typedef uint8_t byte;

uint32_t lc_fx;      // measured resonant frequency (in Hz)
uint16_t lc_qx;      // measured resonance Q (in units of 0.1)
uint32_t lc_lx;      // measured inductance using the samplingADC method (in nH)
uint16_t lc_cpartmp; 
#ifndef CxL 
 #define CxL 18040			//default value for parallel capacitor for inductance measurement
#endif
const uint16_t lc_cpar_ee EEMEM = CxL;         // place for lc_cpar as calibration constant in eeprom




#define Maxpk 20

static unsigned int peaksearch(unsigned int uu[], unsigned int *qptr, byte d, byte maxpk)
// searches uu[256] for peaks, using averaging over d samples (d should be about 1/4 of the expected period)
// writes measured Q *10 into *qptr if non-NULL
// maxpk is maximum number of peaks to take into account
// if maxpk<=2, indicates preliminary search mode, where d may not yet be approximately correct; less stringent filtering then
// returns measured period, with 6 bits of fraction, or 0 if no resonance found
{  
   unsigned int a=0;  // moving average of previous d points
   unsigned int b=0;  // moving average of next d points
   byte i;        // index in uu[]
   byte ipk=0;    // peak count
   int prevdelta=1;
   unsigned int firstpeak=0;      // height of first peak
   unsigned int prevpeak=0;       // height of previous peak
   unsigned int sumpeak=0;        // sum of peaks
   unsigned int firstpeakx=0;     // time of first peak, with 6 bits of fraction
   unsigned int prevpeakx=0;      // time of previous peak, with 6 bits of fraction
   byte sawzero=1;                // flag: did we already encounter a zero?
//         uart_putc('d'); uart_int(d); uart_newline();

   for (i=2;i<255-d;i++) 
   {
      b+=uu[i+d];
      a+=uu[i];
      if (i<2+d) continue;
      a-=uu[i-d];
      b-=uu[i];
      int delta=a-b;
      if (a==0) sawzero=1;
      if (d<=2 && uu[i]==0) sawzero=1;
//         if (maxpk>2-2) { wdt_reset(); uart_putc('b'); uart_int(a); uart_int(b); uart_newline();}

      // note: uu[] can be assumed < 600 or so, since we're not interested in peaks above some 600 mV
      // with d<=32 (because at least 2 periods must fit in 256 samples)
      // this means a and b cannot exceed about +20000, and cannot be negative
      // delta is between -20000 and 20000; near zero crossing delta can't exceed one uu[] value, i.e. +/- 600
      // delta can therefore safely be shifted <<6, but not <<8
      // hence the 6 bits of fraction in the peak location
      // on my atmega328p, in some cases most measured peak intervals differ by less than about 0.05, so 6 bits of fraction is just enough to not lose precision
      if (delta>0 && prevdelta<=0 && i>2*d) {
         // found (local) maximum
//         uart_putc('p'); uart_int(i); uart_int(a+b); uart_int(d); uart_newline();
         if (a+b < 3*d) break;  // stop if peak not significantly high
         unsigned x = (i<<6) - ((delta<<6)+(1<<5))/(delta-prevdelta);
//         if (maxpk>2-2) { uart_putc('E'); uart_int(x); uart_int(ipk); uart_int(sawzero); uart_int(d); uart_newline(); }

         sumpeak+=a+b;
         if (ipk==0) {
            firstpeak=a+b;
            firstpeakx=x;
         }
         if (!sawzero) {           // stop if the signal has not been zero since previous peak
            if (maxpk>2) break;
            else goto skip;        // if we're still in preliminary search mode: just skip this peak hoping we'll find a real one later
         }
         sawzero=0;
         prevpeak=a+b;
//         if (maxpk>2-2) { uart_putc('e'); uart_int(x-prevpeakx); uart_int(a+b); uart_newline(); }
         prevpeakx=x;
         ipk++;
         if (ipk==maxpk) break;
      }
skip:
      prevdelta=delta;
   } /* end for i */

   if (qptr) {
      // calculate r = ratio of amplitude between two consecutive peaks (*1000 for scaling)
      // but we use all peaks seen in this calculation for better accuracy
      // r is weighted average of peak2/peak1, peak3/peak2 and so on, weighed by respectively peak1, peak2 and so on
      // this can be calculated as r = (peak2+peak3+...+peak_m)/(peak1+peak2+peak_{m-1})
      unsigned long int r=((sumpeak-firstpeak)*1000ul)/(sumpeak-prevpeak);
      // r = exp(-pi/Q)  so Q = -pi/(ln(r))
      // for r almost 1 (i.e., high Q), this is approx. pi/(1-r)
//      uart_newline(); uart_int((unsigned int)r);
      r = 31416u/get_log(1000-r);
      // note scaling: the get_log is *1000, and we now compute Q in multiples of .1, hence the numerator being 10000*pi
      *qptr=(unsigned int)r;
//      uart_int((unsigned int)r); uart_newline();
   }
   if (ipk<2) return 0;		// only one or no peak found
   ipk--;
//         if (d>=1) { wdt_reset(); uart_newline();uart_putc('i'); uart_int(ipk); uart_int(prevpeakx); uart_int(firstpeakx); uart_newline();}
   return (prevpeakx-firstpeakx+(ipk>>1))/ipk;
}  /* end of peaksearch */




void sampling_lc(byte LowPin, byte HighPin)
{
uint16_t lc_cpar;    // value of parallel capacitor used for calculating inductance, in pF
   lc_cpar=eeprom_read_word((uint16_t *)&lc_cpar_ee);
//   byte LowPin=pb[0];
//   byte HighPin=pb[1];
   byte HiPinR_L, LoADC;
   HiPinR_L = pgm_read_byte(&PinRLRHADCtab[HighPin]);	//R_L mask for HighPin R_L load
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
   LoADC = pgm_read_byte((&PinRLRHADCtab[6])+LowPin) | TXD_MSK;
#else
   LoADC = pgm_read_byte((&PinRLRHADCtab[3])+LowPin) | TXD_MSK;
#endif

   lc_fx=0;
   lc_qx=0;
   lc_lx=0;
   if ((PartFound != PART_RESISTOR) || (inductor_lpre > 0)) {
      // can happen if we're invoke when there's both a diode and a resistor;
      // don't try to measure inductance then
      // the other reason is a too big resistance, 2100 Ohm is found by ReadInductance
      return;
   }

   byte i=0;

   unsigned int uu[256];
   ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
   wait100us();


   // first, acquire data at maximum speed:
#if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
   ADMUX=HighPin|(0<<REFS0)|(1<<REFS1);   // use built-in reference, about 1.1 V;
#else
   ADMUX=HighPin|(1<<REFS0)|(1<<REFS1);   // use built-in reference, about 1.1 V;
   // that's enough, because peaks more than about 0.6 V are not of interest
   // (because the negative peak would be chopped by the protection diodes)
#endif
#ifdef NO_AREF_CAP
    wait100us(); /* time for voltage stabilization */
#else
    wait_about10ms(); /* time for voltage stabilization */
#endif
   samplingADC(0, uu, 0, HiPinR_L, 0, HiPinR_L, HiPinR_L);     // floats the HiPin during measurement, thus not loading the tuned circuit
//   uart_newline(); for (i=0;i<255;i++) { uart_putc('A'); uart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

   byte d;
   unsigned shift=0;
   // check how long until signal reaches 0: that gives us a first guess of 1/4 of the resonance period (because we apply an impulse, so we start at the maximum of the sinewave)
   for (d=2;d<250;d++) if (uu[d]==0) break;
   if (d==250) {
      // no periodicity seen, so no valid results
//      lc_lx=0;
//      lc_fx=0;
//      lc_qx=0;
      return;
   }

   d--;                 // improves estimate slightly (experimentally)
   uint16_t par = samplingADC_twopulses | (4<<smplADC_inter_pulse_width); // default: two pulses at minimal distance
   if (d<=6*3) {
      // in case of rather small d, need to measure d more precisely, by simply invoking the peaksearch function
      unsigned int per;
retry:
      per=peaksearch(uu,NULL,d,2);
//         uart_putc('p'); uart_int(per); uart_newline();
      if (per==0) {   // not at least 2 peaks found: try with smaller d, or give up
                      // reason for trying with small d is that with very small inductors, zero is often not reached before the first peak, making the zeroth peak look much too wide, causing overestimate of d
                      // perhaps should no longer use that for estimating d...
         if (d>2) { d=2; goto retry; }
         return;  
      }
      d= 1+(per>>8);    // >>6 because of fraction bits, plus >>2 because d should be about a quarter period, plus +1 since 2 turns out to work better than 1 even on very fast signals (2 MHz or so)
      per>>=6;
      if (per<=15) {
         // for high frequencies, we can send 2 pulses at the appropriate interval
         // (for lower frequencies, we just keep them at their default minimum distance)
         if (per<4) per=4;
         par = samplingADC_twopulses | (((byte)per)<<smplADC_inter_pulse_width);
      }
   }
//   uart_newline(); for (i=0;i<255;i++) { uart_putc('a'); uart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

   if (d>16) {
      // rather slow resonance: then re-sample with 4 or 16 times larger interval; shift variable serves to take this into account in later calculations
      if (d<64) {
         shift=2;
         par = samplingADC_slow4 | samplingADC_twopulses | (4<<smplADC_inter_pulse_width);
      } else {
         shift=4;
         par = samplingADC_slow16 | samplingADC_twopulses | (4<<smplADC_inter_pulse_width);
      }
      d>>=shift;
   }
#if (DEB_SAM == 2)
   lcd_line2();
   DisplayValue16(shift,0,'s',4);
   wait_about500ms();
#endif
   // we take the average of 8 measurements, to increase S/N, except when using slow16 mode, since then the sampling would take annoyingly long (and S/N usually is better anyway at these lower frequencies)
   for (i=0;i<8;i++) {
      samplingADC(par, uu, 0, HiPinR_L, 0, HiPinR_L, HiPinR_L);
      if (par & samplingADC_slow16) goto noavg;
      par |= samplingADC_cumul;
   }
   for (i=0;i<255;i++) uu[i]>>=3;   // divide all samples by 8
   goto xyz;

noavg:;
//***************************************************************************************************
#if (DEB_SAM == 2)
   uint16_t ii;
   for (ii=0;ii<256;ii+=4) {
      if ((ii%32) == 0) {
         lcd_clear();
	 DisplayValue16(ii,0,'-',4);
	 DisplayValue16(ii+31,0,' ',4);
	 lcd_next_line_wait(0);
      } else{	 	lcd_next_line_wait(0); }
      DisplayValue16(uu[ii],0,' ',5);
      DisplayValue16(uu[ii+1],0,' ',5);
      DisplayValue16(uu[ii+2],0,' ',5);
      DisplayValue16(uu[ii+3],0,' ',5);
      if ((ii%32) == 24) {
	 lcd_clear_line();
	 lcd_refresh();
         wait_about5s();
      }
   }
#endif
xyz: ;
//***************************************************************************************************

//   uart_newline(); uart_putc('s'); uart_int(shift); uart_newline();  for (i=0;i<255;i++) { uart_putc('a'); uart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

//         uart_putc('d'); uart_int(d); uart_newline();

   // calculation of the results:
   // d= distance between peaks (in units of CPU clock tics)
   //
   // L = 1/C/(2*pi*fclock/d)**2
   //   = 1/C/(2*pi*fclock)**2 * (d**2)
   //   = 1/(2*pi*fclock)**2 / C * (d**2)

   unsigned period=peaksearch(uu,&lc_qx,d,Maxpk);

   unsigned long v;
   v= (unsigned long)period;         // measured period with 6 fraction bits, before applying shift, is < 256*64 = 2^14
   v=v*v;                            // v < 2^28   ; this is (except for shift)  d<<12
#if F_CPU==16000000UL
//   v=(v>>10)*12368;		// v < 2^32   ; is (d<<2)/(2*pi*fclock)^2 * 1e21 >>3
				// that 12368 is calculated as 1/(2*pi*16e6)**2*1e21 /8, for 16 MHz CPU clock
				// 1e21 / (2*pi*16e6)**2 / (8 * 1024)      = 12.07842632, which can be computed
				// with a divide by 51 and a mul with 616 (= 12.07843137) .
				// whith better accuracy than   12368/1024 = 12.07812500
   v = (v/51)*616;
#elif F_CPU==8000000UL
//   v=(v>>12)*49473;		// for 8 MHz CPU clock, it's 49473, but we need to right-shift further to fit in 32 bit
				// 1e21 / (2*pi*16e6)**2 / (8 * 1024)      = 12.07842632, which can be computed
				// with a divide by 51 and a mul with 616 (= 12.07843137) .
				// whith better accuracy than   49473/4096 = 12.07836914
   v = (v/51)*616;
   shift++;                          // change shift variable to compensate for that later on
#else
   #error "CPU clocks other than 8 and 16 MHz not yet supported for SamplingADC"
   v=0;
#endif
   v/=lc_cpar;                          //          ; is (d<<2)/(2*pi*fclock)^2/c * 1e9 >>3
	// shift for 16 MHz 0, 2,  4; for 8 MHz 1, 3,  5
        // resulting factor 2,32,512            8,128,2048 
   v<<=1+2*shift;                      //          ; is L in 1e-9 H  
                        // not nice: the nH number will always be even; then again, do we really measure that precisely?
   lc_lx=v;


   // check whether this estimate is plausible
   // probably inductor_lpre should be checked:
   // inductor_lpre = 0, no Inductor found 
   // unductor_lpre = 1,  rx is above 2100 Ohm
   // inductor_lpre = -5, Inductance searched without 680 Ohm, rx is below 24 Ohm
   // inductor_lpre = -4,  Inductance is searched with 680i Ohm, 24 < rx < 2100
   // probably search of 
   if (inductor_lx>2) {
      // if traditional measurement gave some meaningful-looking value ( > 20 uH, but that's rather arbitrary)
      // discard the new one, it's probably self-resonance
      // note that if a sizeable cap is in parallel, the normal measurement doesn't come up with an answer
      lc_lx=0;
   }

   // freq/Hz = F_CPU/d
   if (period==0) {
      lc_qx = 0;
      lc_lx = 0;
      return;
   }
   v= (unsigned long)period<<1;
   lc_fx=((F_CPU<<(7-shift))/v);

   if (inductor_lpre >= 0) inductor_lpre = -1; 	/* no ESR measurement! */
   return;
} /* end of sampling_lc() */




void sampling_lc_calibrate()
{
   lcd_clear();
   lcd_MEM2_string(str_cap_for_l_meas);		// "Cap for L meas?"
   byte i=0;
   byte ww=0;
   do { 
      lc_cpartmp=0;
      wait_about100ms();
      ReadCapacity(TP1,TP3);
      lcd_line2();
      DisplayValue16(lc_cpartmp,-12,'F',4);
      i++;
      if ((lc_cpartmp<9500) || (lc_cpartmp>33000)) i=0;		// unstable or not connected
      if (i > 4) {	// Cx measurement was stable
         eeprom_write_word((uint16_t *)&lc_cpar_ee,lc_cpartmp);
	 lcd_space();
         lcd_MEM_string(OK_str);	// Output "OK"
         lcd_refresh();
         break;
      }
      lcd_clear_line();
      lcd_refresh();
   } while (++ww != 0);
//   if (i <= 4) {
//      lcd_line2();
//      lcd_MEM2_string(NotFound_str);		// "Not found!"
//      lcd_clear_line();
//      wait_about1s();			// time to read the Not found message
//   }
//   wait_for_key_5s_line2();
}


#endif   // SamplingADC
