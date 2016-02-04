
// original Dec 2015 - Jan 2016, pa3fwm@amsat.org

#ifdef WITH_XTAL

#if MHZ_CPU!=16
   #error "CPU clocks other than 16 MHz not yet supported for crystal measurement"
#endif

#ifdef SamplingADC_CNT
   #error WITH_XTAL not compatible with SamplingADC_CNT
#endif

#ifndef SamplingADC
   #error WITH_XTAL needs SamplingADC
#endif


#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

typedef uint8_t byte;






int16_t sumi;
int16_t sumq;
static void minifourier(unsigned int uu[], int16_t freq, byte nuu, byte phase0)
{
   // computes approximation of one term of Fourier transform of uu[0...nuu-1] at frequency freq with initial phase phase0
   // result is returned in global variables sumi and sumq
   // freq is 16-bit fractional frequency, i.e., phase increments by freq/65536*2*pi per element of uu[]
   // phase0 is 8-bit fractional
   uint16_t avg=uu[0]<<3;
   byte i;                     // index in uu[]
   int16_t phi16=phase0<<8;    // 16-bit phase of the "oscillator"
   int16_t sumi1,sumq1;
   sumi1=sumq1=0;
   for (i=0;i<nuu;i++) 
   {
      int8_t u=uu[i]-(avg>>3);               // more or less remove DC component
      avg+=u;
      byte phi=phi16>>8;
#if 0
      int8_t u2=u/2;
      switch (phi>>5) {
         // approximate sine by a stepped triangle:
         case 0: sumi+=u;            break;
         case 1: sumi+=u2; sumq+=u2; break;
         case 2:           sumq+=u;  break;
         case 3: sumi-=u2; sumq+=u2; break;
         case 4: sumi-=u;            break;
         case 5: sumi-=u2; sumq-=u2; break;
         case 6:           sumq-=u;  break;
         case 7: sumi+=u2; sumq-=u2; break;
      }
#else
      // equivalent to the above, but saves 194 bytes of flash  (don't delete the above code from the source file since it's more readable)
      int8_t di,dq;
      if (phi&0x80) u=-u;                                  // 180 degrees
      if (phi&0x20) { di=dq=u>>2; } else { di=u; dq=0; }   //  45 degrees
      if (phi&0x40) { int8_t tmp=dq; dq=di; di=-tmp; }     //  90 degrees
      sumi1+=di; sumq1+=dq;
#endif
      phi16+=freq;
      //if (what==1) {      myuart_putc('f'); myuart_putc(' '); uart_int(i); uart_int(1000+sumi); uart_int(1000+sumq); uart_int(1000+u); uart_int(1000+sumi-prevsumi); uart_int(uu[i]); uart_newline(); wdt_reset();}
   }
   sumi=sumi1; sumq=sumq1;
}


static byte findphase(unsigned int uu[], int16_t freq, byte nuu)
{
   // find phase at which minifourier().sumi is maximal for given frequency
   // (poor man's arctangent)
   byte phase=0;
   byte bit=0x40;
   bit=0x80;
   // binary search for the upward zero-crossing of sumq, to find maximum of sumi; works because this signal is pretty "well behaved"
   while (bit) {
      minifourier(uu,freq,nuu,phase+bit); 
      if (sumq<0) phase+=bit;
      bit>>=1;
   }
   return phase;
}


static void show_progress(void)
{
/*
   static uint8_t a=0;
   lcd_save_position();
   static uint8_t syms[4]={'|','/','-','\\'};
   a=(a+1)&3;
   lcd_data(syms[a]);
   lcd_restore_position();
*/
   lcd_data('.');
}


static uint16_t findfreqmax(unsigned int uu[], byte nuu)
{
   // find frequency for which minifourier() is largest

   // first a coarse search
   show_progress();
   uint8_t i;
   byte imax=0;
   int16_t maxi=0;
//   int16_t mini=32767;
//   uint16_t sumiq,sumiq1=0,sumiq2=0;
   for (i=6;i<=129;i+=1) {        // start at i=6, corresponding to 16 MHz / (256/6) = 375 kHz, to prevent DC component from being selected
#if 1
      // simpler code, saves flash, would be a bit slower, which we compensate by considering only half the data in this course search
      findphase(uu,i<<8,nuu/2);
      if (sumi>maxi) { imax=i; maxi=sumi; }
#else
      // somewhat faster code because of simpler calculations, but takes some 100 bytes more flash
      minifourier(uu,i<<8,nuu,0);
      if (sumi<0) sumi=-sumi;
      if (sumq<0) sumq=-sumq;
      sumiq=sumi+sumq;
      if (sumiq<mini) mini=sumiq;
      sumiq-=mini;
      // if(what==2) { myuart_putc('r'); myuart_putc(' '); uart_int(i); uart_int(sumiq); uart_int(sumiq+sumiq1+sumiq2); uart_newline(); wdt_reset(); }
      wdt_reset();
      if (sumiq+sumiq1+sumiq2>maxi) { maxi=sumiq+sumiq1+sumiq2; imax=i-1; }
      sumiq2=sumiq1;
      sumiq1=sumiq;
#endif
//      {            myuart_putc('r'); myuart_putc(' '); uart_int(i); uart_int(10000+sumq); uart_int(10000+sumi); uart_int(imax); uart_newline(); wdt_reset(); }
   }
   show_progress();

   // then a more refined search
   uint16_t imaxx;
   imaxx=imax;
   uint16_t imax2=0;
   int16_t ii;
   maxi=0;
   for (ii=(imaxx-2)<<8;ii<=(imaxx+2)<<8;ii+=16)  
//   for (ii=18000; ii<=23000; ii+=16)
   {
      findphase(uu,ii,nuu);
      // find maximum
      if (sumi>maxi) { imax2=ii; maxi=sumi; }
      // if (what==2)  {            myuart_putc('w'); myuart_putc(' '); uart_int(ii); uart_int(10000+sumq); uart_int(10000+sumi); uart_newline(); wdt_reset(); }
//      {            myuart_putc('w'); myuart_putc(' '); uart_int(ii); uart_int(10000+sumq); uart_int(10000+sumi); uart_newline(); wdt_reset(); }
     wdt_reset();
   }
   show_progress();
   // now try to find maximum with more precision, by searching for equally high points on both skirts
   // binary search; may not be the best idea, some smoothing/integration (like in peaksearch()) might be better
   {
      i=128;
      do {
         imax2+=i;
         findphase(uu,imax2+0,nuu);
         int16_t a=sumi;
         findphase(uu,imax2-256,nuu);
         if (a<sumi) imax2-=i;
         i>>=1;
      } while (i);
      imax2-=128;
      wdt_reset();
   }
   show_progress();
   return imax2;
}


static uint16_t sumabs(unsigned int uu[], byte nuu)
// computes sum of absolute values of uu[] after subtraction of DC level
// this is a measure for "how much" oscillation there is
{
   uint16_t avg=uu[1]<<3;
   uint16_t sumd2=0;
   byte i;
   for (i=0;i<nuu;i++)
   {
      int8_t u=((signed)(uu[i]-((avg)>>3)));
      avg+=u;
      if (u<0) u=-u;
      sumd2+=u;
   }
   return sumd2;
}



// variables which store information between sampling_test_xtal() and sampling_measure_xtal(), about how the DUT can be excited
static byte wht;       // bits to be passed as 'what' parameter to samplingADC()
#define isXtal (wht&samplingADC_freq)
static uint16_t dmax=0;    // optimal interpulse distance for samplingADC() call

void sampling_test_xtal()
// check whether there is a ceramic resonator or crystal connected
// do not yet do precise measurement since that takes quite a bit of time
{
   byte LowPin=TP1;
   byte HighPin=TP3;
   byte HiPinR_L, LoADC;
   HiPinR_L = pinmaskRL(HighPin);
   byte HiPinR_H = pinmaskRH(HighPin);
   LoADC = pinmaskADC(LowPin);

   unsigned int uu[255];

   ADC_PORT = TXD_VAL;
   ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
   wait100us();

   ADMUX=HighPin|ADref1V1;   // use built-in reference, about 1.1 V
   wait_aref_stabilize();                               

   wht=samplingADC_mux;      // we let the ADC MUX switch back and forth between both sides of the crystal, thus ensuring that the "high" end of the crystal will have roughly the same DC potential as the low end
                             // note that the use of TP1 (MUX=0) for low side is hardcoded!

   // run a first measurement using a single impulse
   // this allows us to detect ceramic resonators
   samplingADC((1<<smplADC_span), uu, 255, HiPinR_L, HiPinR_H, 0, HiPinR_L);
//   { byte i;for (i=0;i<255;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); } }
   uint16_t sumd=sumabs(uu+1,254);
   if (sumd>=3072) {
      PartFound=PART_CERAMICRESONATOR;
      return;
   }

   // next try a bunch of pulses, at different intervals
   // first try all possible intervals in somewhat coarse steps for a short while
   // next repeat, but with finer steps in a limited range for a longer while (i.e., about 4 times as many pulses)
   uint16_t d;
   uint16_t maxsumd0;
   uint16_t maxsumd=0;
   uint16_t d0,d1,ds;   // minimum, maximum interval, and stepsize
   d0=0;
   d1=8*256+64;
   ds=16/1;
   wht|=samplingADC_freq;
   for (;;) { 
      for (d=d0;d<d1;d+=ds) {
         // we acquire data for a short time (only 40 samples), because we need to try so many different intervals, otherwise it would take too long
         samplingADC_freqgen((1<<smplADC_span)|wht, uu, 40, HiPinR_L, HiPinR_H, 0, HiPinR_L, d);
//   { byte i;for (i=0;i<200;i++) { myuart_putc('B'); myuart_putc(' '); uart_int(uu[i]); uart_int(d); uart_int(i); uart_newline(); wdt_reset(); } }

         R_DDR=0;   // switch off low-side current after measurement
         uint16_t sumd=0;
         sumd=sumabs(uu+1,39);
         if (sumd>maxsumd) { maxsumd=sumd; dmax=d; }
         wdt_reset();
//         myuart_putc('b'); myuart_putc(' '); uart_int(d); uart_int(sumd); uart_newline(); 
      }
//         myuart_putc('S'); myuart_putc(' '); uart_int(dmax); uart_int(maxsumd); uart_newline(); 
      if (ds==1) break;  // if we were already in the second (fine-grained) phase, break
      maxsumd0=maxsumd;
      ds=1;              // otherwise, set variables up for the second phase
      d0=dmax-48;
      if (d0&0x8000) d0=0;
      d1=dmax+48;
      wht|=samplingADC_many;
//      d0=0; d1=8*256+64;
   }
   // now check: in the second phase, we should have a significantly larger oscillation amplitude because of the longer stimulus
   // if not, then there's apparently no crystal
//         myuart_putc('S'); myuart_putc(' '); uart_int(maxsumd0); uart_int(maxsumd); uart_newline(); 
   if (maxsumd>maxsumd0+(maxsumd0>>1))
   {
      PartFound=PART_XTAL;
   }
}




// void do_sampling_resonator(byte LowPin, byte HighPin)
void sampling_measure_xtal()
{
   byte LowPin=TP1;
   byte HighPin=TP3;
   byte HiPinR_L, LoADC;
   byte LoPinR_L;
   HiPinR_L = pinmaskRL(HighPin);
   LoPinR_L = pinmaskRL(LowPin);
   byte HiPinR_H = pinmaskRH(HighPin);
   LoADC = pinmaskADC(LowPin);

   unsigned int uu[255];
   int i;

   ADC_PORT = TXD_VAL;
   ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
   wait100us();

   ADMUX=HighPin|ADref1V1;   // use built-in reference, about 1.1 V;
                             // that's enough, because peaks more than about 0.6 V are not of interest
                             // (because the negative peak would be chopped by the protection diodes)
   wait_aref_stabilize();                               

   lcd_next_line(0);         // do this here already because we'll be putting out progress markings in the course of the measurement


   // initial sampling, still with span=1
   samplingADC_freqgen((1<<smplADC_span)|wht, uu, 255, HiPinR_L, HiPinR_H, 0, HiPinR_L, dmax);
//   for (i=0;i<255;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

   uint16_t imax1=findfreqmax(uu,255);   // obtain a coarse estimate of the resonant frequency

   // next, calculate span (here called ff) to be used for final measurement
   // for ceramic resonators, we choose ff about 16
   // for crystals, we first try about 64, check the amplitude at the end and go to about either 32 or 128 accordingly
   uint16_t imax2;
   byte ff=64;
   byte oldff=1;   // flag that we're still at the first ff attempt in case of crystal
   if (!isXtal) { oldff=0; ff=16; }

newff:;
   byte u;
   // I say "about" in the above, because we should avoid ff values which would result in a sampling rate near a subharmonic of the signal
   // if the signal would be close to a subharmonic, the informvation from the span=1 measurement above will not suffice to disambiguate the aliasing
   do {
      ff--;
      imax2=imax1*ff;        // estimate of what the frequency will be with span=ff; note that truncation to 16 bits of this multiplication mimics the aliasing due to subsampling
      u=imax2>>8;
      if (ff==1) break;      // one hopes we'll not get such a low ff; this happens with crystals very near half the clockfrequency; for those, we can't properly resolve the aliasing
   } while ((u&0x60)==0 || (u&0x60)==0x60);

   // sampling during longer time (with larger span), to make frequency estimate more precise
   samplingADC_freqgen((ff<<smplADC_span)|wht, uu, 255, HiPinR_L|LoPinR_L, HiPinR_H|LoPinR_L, 0|LoPinR_L,         HiPinR_L|LoPinR_L, dmax);
   // from here on, we use the test pins as follows:
   // low side is connected via de ADC-pin to ground (20 ohm or so) and via the low resistor (680 ohm) to +5V, thus establishing about 0.14 V on this pin
   // high side idles with 680 ohm to ground
   // high side gets pulses with 680 ohm to +5V
   // high side has 470 k to ground during measurement
   // additionally, the ADC input is switched to the low side when taking samples that are discarded, thus for every measurement "transfering" the 0.14 V to the high-side, thus establishing its DC level at said 0.14 V.
   R_DDR=0;   // switch off low-side current after measurement

   show_progress();

   if (oldff) {
      // choose a more appropriate ff depending on the acquired data
      // smaller ff if at the end of the data the amplitude is small
      // bigger ff if at the end of the data the amplitude is still very large
      oldff=0;
      byte umin=255,umax=0;
      for (i=254;i>220;i--) {
         if (uu[i]==0) { ff=128; goto newff; }
         byte u=uu[i];
         if (u<umin) umin=u;
         if (u>umax) umax=u;
      }
//      myuart_putc('O'); myuart_putc(' '); uart_int(umax); uart_int(umin); uart_int(ff); uart_newline();
      if (umax-umin<10) { ff=32; goto newff; }
   }

   imax2=findfreqmax(uu,255);   // find the frequency of the new measurement
   if (u&0x80) imax2=-imax2;    // make it negative if that's what we expect based on the coarse measurement
//   myuart_putc('o'); myuart_putc(' '); uart_int(imax1); uart_int(imax2); uart_int(ff); uart_newline();


//   if (imax2<19300 || imax2>19400) 
//   for (i=0;i<255;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }

   // next step, but only for crystals, is to short-circuit the crystal for a while and see how that influences the phase
   // from that we can compute the series resonance frequency (which is the frequency in the short-circuited state)
   // and also disambiguate the aliasing due to our raw 16 MHz sample rate (i.e, distinguish between e.g. 7 and 9 MHz),
   // since we know the series resonance must be at a lower frequency

   uint8_t ssd=0;    // how long the crystal is short-circuited, in units of (about) 256 clockcycles; after the measurement, it will hold how many of such units the crystal was short-circuited for
   uint8_t ph1l=0;   // fractional part of total phase shift, unwrapped
   uint8_t ph1h=0;   // integer part of same
   if (isXtal) {
      uint8_t probingstepsize=0;
      uint8_t ssdstep=1;
      uint8_t ph0;         // starting phase, at ssd=5
      uint8_t rphase;
      // we start at ssd=5, because this is the minimum meaningful value because of the duration of the excitation pulses
      for (ssd=5;ssd<ff/2;ssd+=ssdstep) {
         samplingADC_freqgen_sck((ff<<smplADC_span)|wht|samplingADC_sck, uu, 255, HiPinR_L|LoPinR_L, HiPinR_H|LoPinR_L, 0|LoPinR_L, HiPinR_L|LoPinR_L, dmax, ssd);
//   for (i=0;i<255;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_int(ssd); uart_newline(); wdt_reset(); }
         R_DDR=0;   // switch off low-side bias current between measurements
         rphase=findphase(uu+128,imax2,127);
         if (ssd==5) { ph0=rphase; rphase=0; }
         else {
            rphase-=ph0;
            if ((ph1l^rphase) & 0x80) {
               // most significant bit changed
//               if (!(((ph1l<<1)^ph1l) & 0x80)) 
               if (( (rphase-ph1l)^ph1l ) &0x80)
               {
                  // and we were near 0 or 255
                  // then change MSB accordingly, to "unwrap" the phase
                  if (ph1l&0x80) ph1h++; else ph1h--;
               }
            }
         }
         ph1l=rphase;
         if (ssd==7) probingstepsize=1;
         if (probingstepsize) {
            // if phase turns out to grow slowly, make stepsize larger
            int8_t r=rphase;
            if (r<48 && r>-48) ssdstep<<=1;
            else probingstepsize=0;
         }

//         myuart_putc('y'); myuart_putc(' '); uart_int(ssd); uart_int(ph1l);  uart_int(ph1l+(((uint16_t)ph1h)<<8)); uart_int(ff); uart_int(dmax); uart_int(imax1); uart_int(imax2); uart_newline(); 
         wdt_reset();
         show_progress();
      }
      ssd-=5+ssdstep;
      // now ph1h:ph1l is the phaseshift (in 360/256 degrees) during ssd*256 clockcycles of shortcircuiting the crystal
      // this implies a frequency difference of ph1/256 / (ssd*256/fclock) = ph1/256/ssd/256*fclock = ph1 * fclock / ssd / 65536
      // assuming fclk = 16.000000000 MHz: ph1 * 244.140 / ssd

   } else ph1h=0x80;

   uint32_t f;
   // samplerate is fclk/ff
   // so frequency is fclk/ff * imax2/65536 = imax2 * (fclk/65536/ff)
   // but that's after subsampling by a factor ff
   // so an integer number of times fclk/ff needs to be added
   // the real frequency must be "near" imax1/65536*fclk = fclk/65536/ff * imax1*ff
   f=imax2;
//   while (f+32768 < (uint32_t)imax1*ff) f+=65536ul;   // older version, takes more flash
   while (((f+32768)>>8) < (imax1>>8)*ff) f+=65536ul; 
   // assuming fclk = 16.000000000 MHz:
   //      f = f*16000000/65536/ff;
   //      f = f*15625/64/ff;
   f = f*625/64*25/ff;
   uint16_t ph1 = (((uint16_t)ph1h)<<8)|ph1l;
   if (!(ph1h&0x80)) f=F_CPU-f;         else ph1=-ph1;

//#define F_CPU 16016000UL
//#define F_CPU 16032000UL
//#define F_CPU 15968000UL
#if F_CPU!=16000000UL        // but... unfortunately the makefile system doesn't yet support non-round CPU clocks
   // correction for the CPU clock deviating from 16 MHz
   #if (F_CPU>16000000UL)
      uint16_t df=(F_CPU-16000000UL)*256/244;
      df = ( (f>>16) * df )>>8;
      f += df;
   #else
      uint16_t df=(16000000UL-F_CPU)*256/244;
      df = ( (f>>16) * df )>>8;
      f -= df;
   #endif
#endif


   if (isXtal && ff!=1) {

      // in case of crystal, find motional capacitance and series resonance
      //
      // note: in principle we could find even more parameters of the crystal's equivalent circuit:
      // - motional inductance, calculated from mot.cap. and ser.res. freq
      // - parallel capacitance: simply to standard cap. measurement
      // - series resistance: could in theory be computed from Q, found from decay rate of oscillation, but it turns out the tester circuit damps the crystal more.
      // I (pa3fwm) didn't implement these, because it saves program memory space (the ATmega328 is getting full),
      //    the quantities are rarely needed, and mot.ind. can be computed by user, and par.cap. can be measured in [C] mode.

//      uint16_t df = ph1 * (244 / ssd);   // frequency shift due to short-circuiting
      uint16_t df = ( (ph1 * 62) / ssd )<<2;   // frequency shift due to short-circuiting
      f -= df;                           // compute series resonant frequency
      uint16_t parcap;
      parcap=sampling_cap(HighPin, LowPin, 2);  // capacitance including parasitics, in 0.01pF, needed to compute Cm
      parcap-=1400;    // subtract 14 pF of capacitance in the on-chip S&H, because this is behind >=12k of resistance, so invisible at higher frequencies; below about 3 MHz, we should perhaps subtract only 10 pF, or rather, do a gradual transition...
        // motional capacitance in fF = parcap (in cpF) * df/f * 20
//      uint16_t cm = ((uint32_t)parcap * (df / 10))/(uint16_t)(f/200);   // motional capacitance in fF
      uint16_t cm = ((parcap/25) * (df>>5) )/(uint16_t)(f>>14);    // try to keep it in a 16-bit division, at the expense of accuracy; this measurement isn't very precise anyway
      // 25 should precisely be 25.6; rounded down to at least slightly compensate for rounding down due to truncation

      lcd_set_cursor(0,9);
      static const unsigned char str_Cm[] MEM_TEXT = "Cm="; lcd_MEM_string(str_Cm);
      DisplayValue(cm,-15,'F',2);

      lcd_next_line(0);
      static const unsigned char str_ser[] MEM_TEXT = "ser "; lcd_MEM_string(str_ser);
      DisplayValue(f,0,'H',6); lcd_data('z');

      f+=df;   // back to parallel resonant frequency

      lcd_next_line(0);
      static const unsigned char str_par[] MEM_TEXT = "par "; lcd_MEM_string(str_par);

      DisplayValue(f,0,'H',6); lcd_data('z');
   } else {
      // no crystal, then just ceramic resonator, only show a single resonant frequency
      // also use this code for crystals if ff=1, practically speaking, if we have an 8 MHz crystal, since then precision measurements are not possible
      lcd_clear_line2();
      DisplayValue(f,0,'H',4); lcd_data('z');
   }

}


#endif   // WITH_XTAL
