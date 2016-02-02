
// June-Sept 2015, pa3fwm@amsat.org

#ifdef SamplingADC

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


typedef uint8_t byte;

//const uint16_t c_zero_tab2_lo[] EEMEM = { 0,0,0,0,0,0,0 };   // zero offsets for SamplingADC capacitance measurement, in 0.01 pF, lo voltage
//const uint16_t c_zero_tab2_hi[] EEMEM = { 0,0,0,0,0,0,0 };   // same, hi voltage




// calculate -32768*ln(1-y/32768)
static unsigned int mylog(unsigned int y)
{
   unsigned long int a,b;
   b=y;

// optimized to give more accurate results for small y
// reason is that in the end, we determine the slope (using least-squares) roughly between y=0 and some larger y (larger for smaller caps)
// mylog(32768*(1-exp(-x/20000))) / x  does not deviate by more than 1 % for x=0...17600
// 17600/20000 = 0.88, so good for times up to 88% of the RC timeconstant
// we measure up to 230 clockcycles, so at 16 MHz, that 88% of RC, with 470 kOhm, is 35 pF, which is the typicaly parasitic capacitance we see
// TODO: at 8 MHz, we'll need a mylog() function which is good also for larger y, or use fewer of the samples; latter may be best, to restrict the voltage range on the cap (important for varicaps)

/*
   a = (b<<16);
   a += b* ((y>>1) + (y>>2) + (y>>4));
   b = b*y;
   a += y * ((b>>15) + (b>>16));
   return a>>16;
*/

/*
   // version which uses more 16-bit rather than 32-bit arithmetic; produces larger code though :(
   unsigned short int bb;
   b=y;
   a = (b<<16);
   a += b* ((y>>1) + (y>>2) + (y>>4));
   bb = (y>>6)*(y>>7);
   a += y * ((bb>>2) + (bb>>3));
   return a>>16;
*/

   // newer version: uses one more term for better accuracy (0.2% instead of 1% in the test mentioned above)
   // only 30 bytes longer than the 1% version
   unsigned short int bb;
   b=y;
   a = (b<<16);
   a += b* (y+(y>>4)+(y>>8));
   b = b*y;
   bb = b>>16;
   bb>>=2;                                     // not integrated in previous line because then compiler does an 18-position shift of a 32-bit variable :-(
   unsigned short bb2=bb>>2;                   // without this intermediate variable, compiler does some of >> multiple times
   a += y * (unsigned long int) (bb - bb2);
   bb = (bb2>>1)*(y>>8);            
   a += y * (unsigned long int)(bb - (bb>>3));
   return a>>16;
} /* end mylog */


int32_t sampling_cap(byte HighPin, byte LowPin, byte opts)
{
  EntladePins();

  uint8_t HiPinR_L, HiPinR_H;
  uint8_t LoADC;
  uint16_t samp_opt;
  HiPinR_L = pgm_read_byte(&PinRLRHADCtab[HighPin]);	//R_L mask for HighPin R_L load
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  HiPinR_H = pgm_read_byte((&PinRLRHADCtab[3])+HighPin);	//R_H mask for HighPin R_H load
  LoADC = pgm_read_byte((&PinRLRHADCtab[6])+LowPin) | TXD_MSK;
#else
  HiPinR_H = HiPinR_L + HiPinR_L;	//double for HighPin R_H load
  LoADC = pgm_read_byte((&PinRLRHADCtab[3])+LowPin) | TXD_MSK;
#endif

   byte i=0;

   unsigned int uu[256];
   ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
   R_PORT = 0;			//R_L to 0 (GND) 
   R_DDR = HiPinR_L;			
//   wait200us();			// let the last ADC cycle finish

   ADMUX = HighPin|(1<<REFS0);	// switch Multiplexer to Highpin and use 5V reference voltage
#ifdef NO_AREF_CAP
    wait100us(); /* time for voltage stabilization */
#else
    wait_about10ms(); /* time for voltage stabilization */
#endif

//   lcd_line1();
//   lcd_string(itoa(ADCSRA, outval, 16));
//   wait_about500ms();

//   memset(uu,0,sizeof(uu));

   // we'll do the least-squares calculation over samples N1..N2 (inclusive), excluding N3..(N4-1)
#if MHZ_CPU==16
   #define N1 70     // skip the initial part, which seems to behave a bit differently
   #define N2 230    // skip the last part where the sampling switch starts to affect the results
   #define N3 182    // skip the part just before sample 192 where the ADC mux switch slightly affects results
   #define N4 193
#elif MHZ_CPU==8
   // at 8 MHz, we only use half the possible range of samples, because otherwise the voltage on the cap changes by too much during the measurement if cap is small, like the 35 pF or so if nothing connected
   #define N1 35     // skip the initial part, which seems to behave a bit differently
   #define N2 115    // skip the last part where the sampling switch starts to affect the results
   #define N3 200
   #define N4 200
#else
   #error Clock frequency not supported!
#endif

   #define N (N2-N1+1-(N4-N3))
   #define sumx (unsigned long)((N2*(N2+1l)/2)-(N4*(N4-1l)/2)+(N3*(N3-1l)/2)-(N1*(N1-1l)/2))
   unsigned long sumxx=(unsigned long)((N2*(N2+1l)*(2*N2+1l)/6)-(N4*(N4-1l)*(2*N4-1l)/6)+(N3*(N3-1l)*(2*N3-1l)/6)-(N1*(N1-1l)*(2*N1-1l)/6));
   samp_opt = 1;		// sample distance 1
   byte d=( (opts&1) ? HiPinR_L : HiPinR_H );

   for (i=0;i<32;i++) {
      samplingADC(samp_opt, uu, N2+1, d, HiPinR_H, d, HiPinR_L);
      samp_opt |= samplingADC_cumul;
   }
//   uart_newline(); for (i=0;i<N2;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

   R_DDR = 0;			
#if (DEB_SAM == 1)
//------------------------------------------------------------------
 #warning "sampling_cap with test output enabled!"
   uint16_t kk;
   int16_t udiff;
   uint16_t old_uu;
// #define Step3 ((N2-N1)/15)
// define NN1 N1
#define Step3 1
#define NN1 0

   old_uu = uu[NN1];
   for (kk=(NN1+Step3); kk<(NN1+(Step3*16)); kk+=Step3) {
     udiff = uu[kk] - old_uu;
     if (udiff < 0) udiff = -udiff;	// absolute value
     old_uu = uu[kk];
     if ( ((kk-NN1-Step3) % (3*Step3)) == 0) lcd_next_line_wait(0);
     DisplayValue16((uint16_t)(udiff)/32,0,' ',4);
   }
   lcd_next_line_wait(0);
   DisplayValue16(uu[0]/32,0,'a',5);
   DisplayValue16(uu[1]/32,0,'b',5);
   lcd_clear_line();
   lcd_refresh();
   wait_about5s();
//------------------------------------------------------------------
#endif

   // we use the least-squares algorithm to find the slope
   // cf. e.g. https://en.wikipedia.org/wiki/Ordinary_least_squares
   unsigned long sumy, sumxy;
   sumy=sumxy=0;
   for (i=N1;i<=N2;i++) {	/* 70-230 @16MHz, 35-115 @8MHz */
      if (i==N3) i=N4;		// skip from N3 to (N4-1), 182-192 @16MHz, 200-200 @8MHz
      unsigned int z;
      if (opts&1) z=mylog(32768-uu[i]);
      else z=mylog(uu[i]);
      sumy+=z;
      sumxy+=(unsigned long int)i*z;
   }
   // maximum magnitudes at 16 MHz clock:
   // N ~ 2^8
   // sumx ~ 2^15
   // sumxx ~ 2^23  (in practice with our N1..4 choice: <2^22)
   // sumy ~ 2^22
   // sumxy ~ 2^30
   // at 8 MHz, we sum roughly half as many sample samples, and x will get only half as big, so sumxy will be about 1/4, and sumxx 1/8
   sumxx -= sumx*sumx/N;
   // now sumxx ~ <2^19 , as it turns out; note that sumxx is a constant, depending only on N and N1..4; it was a variable previously, hence this code
   sumxy -= sumx*(sumy/N);
   // now sumxy/sumxx is a least-squares estimate for the slope, in units of 32768 per-clockcycle
   // this slope is 1/RC, where R is the (nominally) 470 kOhm test resistor
   // thus C = 1/(slope*R) = sumxx/(sumxy*f*R/32768) = sumxx/sumxy * 32768/(f*R)
   // we scale sumxx and sumxy for least errors in the coming calculation
   // at 16 MHz, and aiming for units of 10^{-14} Farad, the constant becomes 32768e14/16e6/R = 2048e8/R
   // for R=470000, this is 435744.68
   // if 8 MHz, we correct for the extra factor 2 in the bitshift of sumxx, so as to not lose bits
#if MHZ_CPU==16
   sumxx>>=3;   // bitshift such that sumxx fits in 16 bits
   sumxy>>=6;   // we shift sumxy by 3 bits more than sumxx because a few lines down we have an extra /8 to make things fit 
#elif MHZ_CPU==8
   sumxx>>=0;
   sumxy>>=4;
#endif
   unsigned long int c3;
   c3 = sumxx*((uint32_t)(((20480000000./R_H_VAL)+4)/8))/sumxy;  // units of 0.01 pF
   // the /8 is to make the (...) factor (about 435745) fit in 16 bits
   // note that the /8 is compensated for by the different bitshifts of sumxx and sumxy, and the +4 rounds this number properly
   if (!(opts&2)) 
   {
      byte k = ((HighPin - TP1)*3) + LowPin - TP1 -1;
      if (opts&1) c3-= eeprom_read_word(c_zero_tab2_hi+k);
      else c3-= eeprom_read_word(c_zero_tab2_lo+k);
   }
   return c3;
}  /* end sampling_cap */





void sampling_cap_calibrate()
{
   // measure capacitance of all combinations without anything connected
   byte i,j;
   for (i=0;i<=2;i++)
      for (j=0;j<=2;j++)
         if (i!=j) {
            unsigned int c;
            unsigned int d;
            c=sampling_cap(i,j,2);
            d=sampling_cap(i,j,3);
            lcd_clear();
            lcd_MEM2_string(C0samp_str);			//output "C0samp "
            lcd_space();				// lcd_data(' ');
            lcd_testpin(i);
            lcd_data(':');
            lcd_testpin(j);
            lcd_space();				//lcd_data(' ');
            lcd_line2();
            DisplayValue16(c,-2,' ',4);
            DisplayValue16(d,-14,'F',4);
            byte k=3*i+j-1;
            eeprom_write_word((void*)(c_zero_tab2_lo+k),c);
            eeprom_write_word((void*)(c_zero_tab2_hi+k),d);
            lcd_clear_line();
            lcd_refresh();
         }
   lcd_line1();
   lcd_MEM_string(OK_str);	// "OK"
   lcd_clear_line();
   lcd_refresh();
   wait_about200ms();
} /* end sampling_cap_calibrate */



#endif
