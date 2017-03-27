
// original June-Sept 2015, pa3fwm@amsat.org

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

#ifdef DEB_SAM
void report_buf(uint16_t uu[], uint8_t ll) {
         uint16_t ii;
         for (ii=0;ii<255;ii+=8) {
            lcd_set_cursor(ll,0);
            uart_newline();
            DisplayValue16(uu[ii],0,' ',5);
            DisplayValue16(uu[ii+1],0,' ',5);
            DisplayValue16(uu[ii+2],0,' ',5);
            DisplayValue16(uu[ii+3],0,' ',5);
            DisplayValue16(uu[ii+4],0,' ',5);
            DisplayValue16(uu[ii+5],0,' ',5);
            DisplayValue16(uu[ii+6],0,' ',5);
            DisplayValue16(uu[ii+7],0,' ',5);
            lcd_clear_line();
            lcd_refresh();
            wait_about100ms();
         } /* end for ii */
}
#endif


// Maxpk is maximum number of peaks to take into account
#define Maxpk 20

// define Special_dist1

static unsigned int peaksearch(unsigned int uu[], unsigned int *qptr)
// searches uu[256] for peaks, using averaging over dist samples 
// writes measured Q *10 into *qptr if non-NULL
// returns measured period, with 6 bits of fraction, or 256*64 if no resonance found
{  
   unsigned int xx;
   int delta;			// difference of aa and bb sum
   int prevdelta;
   unsigned int aa;  // moving average of previous dist points
   unsigned int bb;  // moving average of next dist points
   unsigned char ii;        // index in uu[]
   unsigned char ipk;    // peak counter
   unsigned int firstpeak_sum;      // height of first peak
   unsigned int prevpeak_sum;       // height of previous peak
   unsigned int sumpeak;        // sum of peaks (without initial peak)
   unsigned int firstpeak_x;     // time of first peak, with 6 bits of fraction
   unsigned int prevpeak_x;      // time of previous peak, with 6 bits of fraction
   unsigned char sawzero;                // flag: did we already encounter a zero?
   unsigned char dist;		// dist should be set automatically to about 1/4 of the detected period
   unsigned int mean_per;		// average period; the (ipk>>2) provides for rounding
   unsigned int last_per;		// length of last period with 6 bits of fraction
   unsigned int sum_ab;
   unsigned int rr;
   
   dist = 1;		// begin peak search with dist=1

repeat:
   aa = 0;
   bb = 0;
   ipk=0;    // peak counter
   firstpeak_sum=0;      // height of first peak
   prevpeak_sum=0;       // height of previous peak
   sumpeak=0;        // sum of peaks
   firstpeak_x=0;     // time of first peak, with 6 bits of fraction
   prevpeak_x=0;      // time of previous peak, with 6 bits of fraction
   sawzero=1;                // flag: did we already encounter a zero?
   prevdelta=1;
   mean_per = 256<<6;	// set period illegal
   for (ii=0;ii<255-dist;ii++) 
   {
      bb += uu[ii+dist];
      aa += uu[ii];
      if (ii < dist) continue;
      aa -= uu[ii-dist];
      bb -= uu[ii];
#ifdef Special_dist1
      if (dist > 1) {
#endif
        sum_ab = aa + bb;
        delta = aa - bb;
      // the detection of zero is replaced by  detection of rising  (kubi)
        if (((int)(sum_ab/32)+delta) < 0) {
           sawzero=1;
        }

      // note: uu[] can be assumed < 600 or so, since we're not interested in peaks above some 600 mV
      // with dist<=32 (because at least 2 periods must fit in 256 samples)
      // this means a and b cannot exceed about +20000, and cannot be negative
      // delta is between -20000 and 20000; near zero crossing delta can't exceed one uu[] value, i.e. +/- 600
      // delta can therefore safely be shifted <<6, but not <<8
      // hence the 6 bits of fraction in the peak location
      // on my atmega328p, in some cases most measured peak intervals differ by less than about 0.05, so 6 bits of fraction is just enough to not lose precision

//        if ((ipk < 2) && (ii > 160)) break;	// no chance to find a period
#ifdef Special_dist1
      } else {
// this method gives better interpolation of the peak location (fraction of ii)
// but find too many peaks in case of slow voltage change with noise
        sum_ab = aa + bb + uu[ii-dist];  // bb is uu[ii+dist], aa is uu[ii]
        delta =  uu[ii-dist] - bb;
        sawzero = (aa > uu[ii-dist]);
      }
#endif

      if ((bb < aa) && (sawzero == 1)) {
         // found (local) maximum
         xx = (ii<<6);
//         xx -=  ((delta<<6)+(1<<5))/(delta-prevdelta);
       // (delta - prevdelta) is same as ((2*uu[ii]) - uu[ii-dist] - uu[ii+dist])
       // For a correct size of delta the uu[ii-dist] and uu[ii+dist] are low against 2*uu[ii].
       // By dividing with 2*uu[ii] alone, the interpolation can be used with dist=1.
         uint16_t kdiv;
//         kdiv = 2 * uu[ii];
         kdiv = delta - prevdelta;
         if (kdiv >= delta) xx -= (((long)delta * 64)+ 32) / kdiv;

         if (ipk != 0) {
            if (sum_ab < (3*dist)) break;  // stop if peak not significantly high
            if (ipk==1) {
               firstpeak_sum = sum_ab;		// amplitude sum of first peak
               firstpeak_x = xx;		// position of first peak
            } else {  // ipk > 1
               last_per = xx - prevpeak_x;	// length of last period
               if ((ipk > 3) && (last_per > (mean_per + mean_per/2 + 32))) break;  // gap between peaks
            // sanity check: distance between peaks is expected to be 4*dist samples
               unsigned char smp_per;
               smp_per = last_per>>8;	// period with 4 sample units
               if (smp_per > dist) {		// dist is lower than 1/4 period without rounding
                  dist = smp_per+1;		// set dist to the found period / 4, rounded up
                  goto repeat;
               }
            }  /* ipk > 1 */
            sumpeak += sum_ab;		// build sum of amplitude
#if (DEB_SAM == 5)
            if (qptr) {	 /* report only the final peaks */
               lcd_set_cursor(6,0);
	       uart_newline();
               lcd_data('i');
               lcd_data('=');
               DisplayValue(ii,0,' ',3);
               DisplayValue(sumpeak,0,'=',5);
               DisplayValue(aa,0,'+',5);
               DisplayValue(bb,0,' ',5);
               lcd_set_cursor(7,0);
	       uart_newline();
               wait_about500ms();
            }
#endif
         }  /* ipk != 0 */
         sawzero=0;
         prevpeak_sum = sum_ab;		// save amplitude of last peak
         prevpeak_x = xx;		// save position of last peak
         ipk++;			// one more peak found
         if (ipk > 2) {
            mean_per = (prevpeak_x - firstpeak_x + ((ipk-2)>>1)) / (ipk-2);  // average period with rounding
         }
         if (ipk > Maxpk) break;	// count of requested peaks is found
      }  /* end found maximum */
      prevdelta = delta;
   } /* end for ii */

   /* total data are analysed, ipk is now the count of peaks + 1 */
//   if (firstzero>(per>>6)) return 0;     // sanity check: first zero should be within first period
   if (qptr) {
      // calculate r = ratio of amplitude between two consecutive peaks (*1000 for scaling)
      // but we use all peaks seen in this calculation for better accuracy
      // r is weighted average of peak2/peak1, peak3/peak2 and so on, weighed by respectively peak1, peak2 and so on
      // this can be calculated as r = (peak2+peak3+...+peak_m)/(peak1+peak2+peak_{m-1})
      rr = 0;
      if (ipk > 2) {
         unsigned int sumdiff = sumpeak - prevpeak_sum;
         rr = ((unsigned long)(sumpeak-firstpeak_sum)*1000)/sumdiff;
         // rr = exp(-pi/Q)  so Q = -pi/(ln(rr))
         // for r almost 1 (i.e., high Q), this is approx. pi/(1-rr)
         rr = 31416u/get_log(1000-rr);
         // note scaling: the get_log is *1000, and we now compute Q in multiples of .1, hence the numerator being 10000*pi
      }
      *qptr = (unsigned int)rr;
   }
#if DEB_SAM == 11
   if (qptr) lcd_set_cursor(7,0);
   else      lcd_set_cursor(6,0);
   uart_newline();
   lcd_data('p');
   lcd_data('=');
   DisplayValue(ipk,0,' ',3);
   DisplayValue(((unsigned long)mean_per*25)/16,-2,' ',6);
   DisplayValue(dist,0,' ',5);
#endif
   return mean_per;
}  /* end of peaksearch */




void sampling_lc(byte LowPin, byte HighPin)
{
   uint16_t lc_cpar;    // value of parallel capacitor used for calculating inductance, in pF
   uint16_t period;
   lc_cpar=eeprom_read_word((uint16_t *)&lc_cpar_ee);


//###################################################################################################
 // new version of the code, with pulses via the ADC port, i.e., without 680 ohm series resistor

   byte HiPinR_L, LoADC;
   HiPinR_L = pinmaskRL(HighPin);
   LoADC = pinmaskADC(LowPin);

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

   unsigned int uu[255];


   ADC_PORT = TXD_VAL;
   ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
   wait100us();

   // first, acquire data at maximum speed:
   ADMUX=HighPin|ADref1V1;   // use built-in reference, about 1.1 V;
                             // that's enough, because peaks more than about 0.6 V are not of interest
                             // (because the negative peak would be chopped by the protection diodes)
   wait_aref_stabilize();                               

   // run a first measurement, using the narrow full-current impulse
   samplingADC((1<<smplADC_span)|(1<<smplADC_direct), uu, 255, HiPinR_L, 0, 0, HiPinR_L);

   // also measure some 20 samples at the "cold side" of the coil, and subtract
   // at highest frequencies, this is useful because the on-chip ADC has an RC lowpass which gives an exponentially decaying "DC" offset
   unsigned int uu0[20];
   ADMUX=LowPin|ADref1V1;   // switch to "cold" side for reference measurement
   samplingADC((1<<smplADC_span)|(1<<smplADC_direct), uu0, 20, HiPinR_L, 0, 0, HiPinR_L);
   for (i=0;i<20;i++)
   {
      if (uu[i]>=uu0[i]) uu[i]-=uu0[i];
      else uu[i]=0;
   }
   ADMUX=HighPin|ADref1V1;   // back to "hot" side

//   uart_newline(); for (i=0;i<255;i++) { myuart_putc('A'); myuart_putc(' '); uart_int(uu[i]); uart_int(uu0[i]); uart_newline(); wdt_reset(); }

   byte dist0;         // estimate of duration of 1/4 of a period, used to set averaging interval in peaksearch()
   unsigned shift=0;   // by how many bit positions measured period needs to be shifted due to measuring with span>1

   // check how long until signal reaches 0: that gives us a first guess of 1/4 of the resonance period (because we apply an impulse, so we start at the maximum of the sinewave)
   period = peaksearch(uu,NULL);
   dist0 = 1+(period>>8);    // >>6 because of fraction bits, plus >>2 because dist0 should be about a quarter period, plus +1 to round up
 #if (DEB_SAM == 10) 
   lcd_set_cursor(5,0);
   uart_newline();
   lcd_data('p');
   DisplayValue(((unsigned long)dist0*10)/64,-1,' ',3);
 #endif

   uint16_t par = (1<<smplADC_span) | (1<<smplADC_direct); // default: one pulse 
 #if (DEB_SAM == 6)
         lcd_set_cursor(5,0);
         uart_newline();
         lcd_data('d');
	 DisplayValue16(dist0,0,' ',3);
         lcd_data('p');
	 DisplayValue(((unsigned long)period*10)/64,-1,' ',5);
         report_buf(uu,6);
 #endif
	
#define samplingADC_direct (1<<smplADC_direct)
     par = (1<<smplADC_span) | samplingADC_direct;
     if (dist0>16) {
      // rather slow resonance: then re-sample with 4 or 16 times larger interval; shift variable serves to take this into account in later calculations
        if (dist0<64) {
           shift = 2;
           par = (4<<smplADC_span) | samplingADC_direct;
        } else {
           shift = 4;
           par = (16<<smplADC_span) | samplingADC_direct;
        }
     }

   // we take the average of 8 measurements, to increase S/N, except when using span>1, since then the sampling takes annoyingly long and S/N usually is better anyway at these lower frequencies
     for (i=0;i<8;i++) {
        wdt_reset();
        samplingADC(par, uu, 255, HiPinR_L, 0, 0, HiPinR_L);
//        if (par > (2<<smplADC_span)) goto noavg;
        if (shift > 0) goto noavg;
        par |= samplingADC_cumul;
     }
   if ((par >> smplADC_span) < 4) {
    // case of high frequency: subtract reference measurement, and do no scaling down of accumulated amplitudes since they are small in this case
      ADMUX=LowPin|ADref1V1;   // switch to "cold" side for reference measurement
      par &= ~samplingADC_cumul;
      for (i=0;i<8;i++) {
         samplingADC(par, uu0, 20, HiPinR_L, 0, 0, HiPinR_L);
         par |= samplingADC_cumul;
      }
      i=20; while (i--)         // equivalent to for (i=0;i<20;i++) but saves 48 bytes of flash????
      {
         if (uu[i]>=uu0[i]) uu[i]-=uu0[i];
         else uu[i]=0;
      }
//   } else {
    }
    // low frequency: no reference measurement needed, but do scale amplitude by number of accumulated samples
    i=255;
    while (i--) uu[i]>>=3;   // divide all samples by 8
//   }


noavg:;
   period = peaksearch(uu,&lc_qx);
   if (period > (255<<6)) period = 0;		// set implausible period to zero
 #if (DEB_SAM == 5)
         lcd_set_cursor(6,0);
         uart_newline();
         lcd_data('#');
         lcd_data('2');
	 lcd_space();
	 lcd_data('>');
	 lcd_data('>');
	 DisplayValue16(shift,0,' ',3);
         DisplayValue(((unsigned long)period*10)/64,-1,' ',5);
         report_buf(uu,8);
 #endif

//###################################################################################################

   // calculation of the results:
   // d= distance between peaks (in units of CPU clock tics)
   //
   // L = 1/C/(2*pi*fclock/d)**2
   //   = 1/C/(2*pi*fclock)**2 * (d**2)
   //   = 1/(2*pi*fclock)**2 / C * (d**2)

 #if (DEB_SAM == 10)
         lcd_set_cursor(6,0);
         uart_newline();
         lcd_data('#');
	 lcd_space();
	 lcd_data('>');
	 lcd_data('>');
	 DisplayValue16(shift,0,' ',3);
	 lcd_data('p');
         DisplayValue16((period*10)/64,-1,' ',4);
 #endif

   unsigned long vv;
   vv = (unsigned long)period;         // measured period with 6 fraction bits, before applying shift, is < 256*64 = 2^14
   vv = vv*vv;                            // vv < 2^28   ; this is (except for shift)  d<<12
#if F_CPU==16000000UL
//   vv=(vv>>10)*12368;		// vv < 2^32   ; is (d<<2)/(2*pi*fclock)^2 * 1e21 >>3
				// that 12368 is calculated as 1/(2*pi*16e6)**2*1e21 /8, for 16 MHz CPU clock
				// 1e21 / (2*pi*16e6)**2 / (8 * 1024)      = 12.07842632, which can be computed
				// with a divide by 51 and a mul with 616 (= 12.07843137) .
				// whith better accuracy than   12368/1024 = 12.07812500
   vv = (vv/51)*616;
#elif F_CPU==8000000UL
//   v=(v>>12)*49473;		// for 8 MHz CPU clock, it's 49473, but we need to right-shift further to fit in 32 bit
				// 1e21 / (2*pi*16e6)**2 / (8 * 1024)      = 12.07842632, which can be computed
				// with a divide by 51 and a mul with 616 (= 12.07843137) .
				// whith better accuracy than   49473/4096 = 12.07836914
   vv = (vv/51)*616;
   shift++;                          // change shift variable to compensate for that later on
#else
   #error "CPU clocks other than 8 and 16 MHz not yet supported for SamplingADC"
   vv = 0;
#endif
//   vv/=lc_cpar;                          //          ; is (d<<2)/(2*pi*fclock)^2/c * 1e9 >>3
   vv /= (lc_cpar>>1);                          //          ; is (d<<2)/(2*pi*fclock)^2/c * 1e9 >>2
	// shift for 16 MHz 0, 2,  4; for 8 MHz 1, 3,  5
        // resulting factor 2,32,512            8,128,2048 
//   vv<<=1+2*shift;                      //          ; is L in 1e-9 H  
   vv <<= 2*shift;                      //          ; is L in 1e-9 H  
                        // not nice: the nH number will always be even; then again, do we really measure that precisely?
   lc_lx = vv;


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
#if (DEB_SAM == 3)
   uint16_t ii;
   lcd_set_cursor(5,0);
   uart_newline();
   lcd_data('#');
   lcd_space();
   lcd_data('>');
   lcd_data('>');
   DisplayValue16(shift,0,' ',3);
   DisplayValue16(dist0,0,' ',4);
   report_buf(ii,6);
   lcd_refresh();
#endif
      return;
   }
#if F_CPU==16000000UL
   vv = (unsigned long)period<<1;
#else
   vv = (unsigned long)period;
#endif
   lc_fx = ((F_CPU<<(7-shift))/vv);

   if (inductor_lpre >= 0) inductor_lpre = -1; 	/* no ESR measurement! */
   return;
} /* end of sampling_lc() */




void sampling_lc_calibrate(byte ww)
{
 #define C_DIFF_LIMIT 200   /* Tolerance of capacity to rewrite eeprom */
//   lcd_clear();
   lcd_line1();
   lcd_MEM2_string(cap_for_l_meas_str);		// "Cap for L meas?"
   lcd_clear_line();
   byte i = 0;		// no legal capacity value found
   do { 
      lc_cpartmp = 0;
      wait_about100ms();
      ReadCapacity(TP1,TP3);
      lcd_line2();
      DisplayValue16(lc_cpartmp,-12,'F',4);
      i++;
//      if ((lc_cpartmp<9500) || (lc_cpartmp>33000)) i=0;		// unstable or not connected
      if ((lc_cpartmp<3900) || (lc_cpartmp>34000)) i=0;		// unstable or not connected
 #ifdef AUTO_LC_CAP
      if ((lc_cpartmp == 0) && (ww > 249)) break;	// a coil is connected in the AUTO_LC_CAP mode
 #endif
      if (i > 4) {	// Cx measurement was stable
 #ifdef AUTO_LC_CAP
         uint16_t lc_cpar;    // value of parallel capacitor used for calculating inductance, in pF
         lc_cpar = eeprom_read_word((uint16_t *)&lc_cpar_ee);	// read old value
         if ((lc_cpartmp < (lc_cpar - C_DIFF_LIMIT)) || (lc_cpartmp > (lc_cpar + C_DIFF_LIMIT)))
           /* new cap value is out of tolerance */
 #else
        if (1)
 #endif
         {  /* rewrite new capacity value to eeprom and report OK */
            eeprom_write_word((uint16_t *)&lc_cpar_ee,lc_cpartmp);
	    lcd_space();
            lcd_MEM_string(OK_str);	// Output "OK"
            lcd_refresh();
            wait_about500ms();
         }
         break;
      }  /* end (i > 4) */
      lcd_clear_line();
      lcd_refresh();
   } while (++ww != 0);
   lcd_line1();
}

#endif   // SamplingADC
