/*
 * baudcheck.S
 * Mar, 2013 by Bill Westfield (WestfW@yahoo.com)
 * Exercises in executing arithmetic code on a system that we can't count
 * on having the usual languages or tools installed.
 *
 * This little "C program" is run through the C preprocessor using the same
 * arguments as our "real" target (which should assure that it gets the
 * same values for clock speed and desired baud rate), and produces as
 * output a shell script that can be run through bash, and THAT in turn
 * writes the desired output...
 *
 * Note that the C-style comments are stripped by the C preprocessor.
 * Added support for special SOFT_UART by K.-H. Kuebbeler (kh_kuebbeler@web.de)
 */

/*
 * First strip any trailing "L" from the defined constants.  To do this
 * we need to make the constants into shell variables first.
 */

// determine the count of loop tics

#include <avr/io.h>
#ifndef __ASSEMBLER__
 #define __ASSEMBLER__ 
#endif
#define NO_WARNING
#include "pin_defs.h"

#define SOFT_UART_PORT UART_RX_PORT
#include "set_loop_tics.h"

#ifndef BAUD_RATE
 #error "in check_baudrate.c is BAUD_RATE not set"
#endif

bps=`echo BAUD_RATE | tr -d LU`

#ifndef F_CPU
 #error "in check_baudrate.c is F_CPU not set"
#endif
fcpu=`echo F_CPU | tr -d LU`

# #echo "f_cpu = ${fcpu}, baud = ${bps}"
/*
 * Compute the divisor
 */
echo --------------------------------------------------------------------------------
#if BAUD_RATE < 100

 #if (BAUD_RATE-((BAUD_RATE/20)*20)) < 10 
  #define Bits2Measure 2
 #else
  #define Bits2Measure 4
 #endif

 #if (BAUD_RATE >= 80) || (BAUD_RATE < 20)
  #undef Bits2Measure
  #define Bits2Measure 9
 #endif

 #if (BAUD_RATE-((BAUD_RATE/10)*10)) < 5 
  #define CounterClockDiv 8
 #else
  #define CounterClockDiv 1
 #endif

 #if BAUD_RATE >= 60
  #define Method "Complex"
 #else
  #define Method "Simple"
 #endif

 B2M=Bits2Measure
 CCD=CounterClockDiv
 MTD=Method

 #if BAUD_RATE >= 40 && (Baud_RATE < 60) 
  echo "${MTD} Baudrate measurement with time limit implemented in optiboot! (${B2M}-bit, Clk/${CCD})"
 #else
  echo "${MTD} Baudrate measurement implemented in optiboot! (${B2M}-bit, Clk/${CCD})"
 #endif
 
 #if SOFT_UART > 0
  #define Remainder5 (BAUD_RATE - ((BAUD_RATE/5)*5))
  #if ((BAUD_RATE - ((BAUD_RATE/2)*2)) != 0)
   // 15-bit delay 
   // With 15 bit delay loop you can get a max delay of 32767*2*5 tics for 1 bit,
   // so the time measure counter gives allways the max periode for 2-9 bits,
   // if it runs at full speed or clk/8 speed (65535*8)<(32767*10*2).
   #if (Remainder5 > 2)
    #define TICS_PER_LOOP_PASS 8
   #else
    #define TICS_PER_LOOP_PASS 5
   #endif
   #if BAUD_RATE < 60
     UartDelay=$(( (65535* CounterClockDiv / Bits2Measure ) ))
   #else
    // With full check (9 bits)
    UartDelay=$(( (65535* CounterClockDiv / 9) ))
   #endif

TicsN=$(( TICS_PER_LOOP_PASS * 2 ))
UartMinDelay=$(( ((25*TICS_PER_LOOP_PASS)+(RCALL_TICS+1)*2 + LOOP_TICS) ))
echo "SoftUART Minimum `echo "scale=0;(${fcpu} / ${UartDelay})" | bc ` Baud, Difference surely less than 4% up to `echo "scale=1;(${fcpu} / $UartMinDelay / 1000)" | bc ` kBaud, Slow 15-Bit loop with N*${TicsN} cycles"

  #else		/* 8-bit delay */
   #if ((Bits2Measure == 9) && (CounterClockDiv == 8)) || (Remainder5 > 2)
    #define TICS_PER_LOOP_PASS 4         /* special case 9 Bit measurement and clk/8 Counter */
   #else
    #define TICS_PER_LOOP_PASS 3
   #endif

UartDelay=$(( ((255*TICS_PER_LOOP_PASS*2) + (RCALL_TICS*2)  + LOOP_TICS) ))
UartMinDelay=$(( ((25*TICS_PER_LOOP_PASS)+(RCALL_TICS*2) + LOOP_TICS) ))
TicsN=$(( TICS_PER_LOOP_PASS * 2 ))
echo "SoftUART Minimum `echo "scale=0;(${fcpu} / ${UartDelay})" | bc ` Baud, Difference surely less than 4% up to `echo "scale=1;(${fcpu} / $UartMinDelay / 1000)" | bc ` kBaud, Fast 8-Bit loop with N*${TicsN} cycles"
  #endif

 #else		/* HW UART */
  // UART run with clk/8 and max period is 4096 tics.
  // Max total clk tics for 9 bits 4096*8*9=294912,
  // max total clk tics for 4 bits 4096*8*4=131072,
  // which is more than a 16 bit counter can take.
  // So the time measurement with the 16 Bit counter limits
  // the lower baud, when it runs at full speed.
  #if BAUD_RATE < 60
   // simple mode
   #if (Bits2Measure > 2) && (CounterClockDiv == 1)
    UartDelay=$(( (65535/ Bits2Measure ) ))
   #else
    UartDelay=$(( (8*4096) ))
   #endif
  #else
   // full 10-bit check
   #if CounterClockDiv == 1
    // always the time of 9 bits is analysed
    UartDelay=$(( (65535/ 9 ) ))
   #else
    UartDelay=$(( (8*4096) ))
   #endif
  #endif

echo "UART Minimum `echo "scale=0;(${fcpu} / ${UartDelay})" | bc ` Baud, Difference surely less than 4% up to `echo "scale=1;(${fcpu} / (8*12500))" | bc ` kBaud"

 #endif		/* SW HW UART */

#else 	/* BAUD_RATE >= 100) */

#if SOFT_UART > 0
 #define DelayMul 1
 #define CLOCKS_PER_BIT ((F_CPU-((((2*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2))/BAUD_RATE)
 #if CLOCKS_PER_BIT < 0
  #undef RCALL_TICS
  #undef CLOCKS_PER_BIT
  #define RCALL_TICS 0
  #define CLOCKS_PER_BIT ((F_CPU-((((2*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2))/BAUD_RATE)
 #else
  #define UART_B_VALUE ((CLOCKS_PER_BIT)/6)
  #if UART_B_VALUE > 255
   // try with double rcall uartDelay_single
   #undef CLOCKS_PER_BIT
   #undef UART_B_VALUE
   #undef DelayMul
   #define DelayMul 2
   #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
   #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
   #if UART_B_VALUE > 255
    // compute with 4x rcall uartDelay_single
    #undef CLOCKS_PER_BIT
    #undef UART_B_VALUE
    #undef DelayMul
    #define DelayMul 4
    #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
    #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
    #if UART_B_VALUE > 255
     // compute with 8x rcall uartDelay_single
     #undef CLOCKS_PER_BIT
     #undef UART_B_VALUE
     #undef DelayMul
     #define DelayMul 8
     #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
     #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
     #if UART_B_VALUE > 255
      // compute with 16 rcall uartDelay_single
      #undef CLOCKS_PER_BIT
      #undef UART_B_VALUE
      #undef DelayMul
      #define DelayMul 16
      #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
      #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
      #if UART_B_VALUE > 255
       // compute with 32 rcall uartDelay_single
       #undef CLOCKS_PER_BIT
       #undef UART_B_VALUE
       #undef DelayMul
       #define DelayMul 32
       #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
       #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
       #if UART_B_VALUE > 255
        // compute with 64 rcall uartDelay_single
        #undef CLOCKS_PER_BIT
        #undef UART_B_VALUE
        #undef DelayMul
        #define DelayMul 64
        #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
        #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
       #endif        /* DelayMul 64*/
      #endif        /* DelayMul 32 */
     #endif        /* DelayMul 16 */
    #endif        /* DelayMul 8 */
   #endif        /* DelayMul 4 */
  #endif         /* DelayMul 2 */
 #endif 	/* CLOCKS_PER_BIT < 0 */

delay_mul=$(( DelayMul ))
clocks=$(( (CLOCKS_PER_BIT / DelayMul) ))
UartDelay=$(( (((RCALL_TICS*2) + (CLOCKS_PER_BIT/DelayMul)) * DelayMul) + LOOP_TICS ))
percent_msg=", Delay: ${clocks}*${delay_mul}, Difference=`echo "scale=2;(100*((${fcpu} / ${UartDelay})- ${bps}) ) / ${bps}"| bc `%"
 #if BAUD_RATE > 10000
echo "BAUD RATE CHECK: Desired: ${bps},  SoftUART_Real: `echo "scale=0;(${fcpu} / ${UartDelay})" | bc `${percent_msg}"
 #elif BAUD_RATE > 1000
echo "BAUD RATE CHECK: Desired: ${bps},  SoftUART_Real: `echo "scale=1;(${fcpu} / ${UartDelay})" | bc `${percent_msg}"
 #else
echo "BAUD RATE CHECK: Desired: ${bps},  SoftUART_Real: `echo "scale=2;(${fcpu} / ${UartDelay})" | bc `${percent_msg}"
 #endif
#else
// for hardware UART
 #define BAUD_DIV  ( ( ( F_CPU / 4L / BAUD_RATE )  - 1) / 2) 
 #if (BAUD_DIV > 255) && (BAUD_DIV < 2046)
BAUD_SETTING=$((  ( ( (${fcpu} / 8 / ${bps}) - 1) / 2) ))
UartDelay=$(( (16 * ((BAUD_SETTING) +1)) ))
/*
 * Based on the computer divisor, calculate the actual bitrate,
 * And the error.  Since we're all integers, we have to calculate
 * the tenths part of the error separately.
 */
// BAUD_ACTUAL=$(( (${fcpu}/(16 * (($BAUD_SETTING)+1))) ))

 #else
BAUD_SETTING=$(( ( ( (${fcpu} / 4 / ${bps} ) - 1) / 2) ))
UartDelay=$(( (8 * ((BAUD_SETTING) +1)) ))
/*
 * Based on the computer divisor, calculate the actual bitrate,
 * And the error.  Since we're all integers, we have to calculate
 * the tenths part of the error separately.
 */
//BAUD_ACTUAL=$(( (${fcpu}/(8 * (($BAUD_SETTING)+1))) ))

 #endif
//echo baud setting = $BAUD_SETTING

/*
 * Print a nice message containing the info we've calculated
 */
BR_ERR=`echo "scale=2;(100*((${fcpu} / ${UartDelay}) - ${bps}) ) / ${bps}"| bc `
if (( `echo "${BR_ERR} > 2"|bc` )) || (( `echo "${BR_ERR} < -2"|bc` )) ; then
 BR_MSG=`echo ", ${Vfett}Difference=${BR_ERR}%${Vnormal}"`
else
 BR_MSG=`echo ", Difference=${BR_ERR}%"`
fi
BRC="${Vgelb}BAUD RATE CHECK:${Vnormal}"
 #if BAUD_RATE > 10000
echo "${BRC} Desired: ${bps},  Real: `echo "scale=0;(${fcpu} / ${UartDelay})" | bc `, UBRR = $BAUD_SETTING${BR_MSG}"
 #elif BAUD_RATE > 1000
echo "${BRC} Desired: ${bps},  Real: `echo "scale=1;(${fcpu} / ${UartDelay})" | bc `, UBRR = $BAUD_SETTING${BR_MSG}"
 #else
echo "${BRC} Desired: ${bps},  Real: `echo "scale=2;(${fcpu} / ${UartDelay})" | bc `, UBRR = $BAUD_SETTING${BR_MSG}"
 #endif
#endif	/* SOFT_UART */
#endif	/* BAUD_RATE < 100 */
echo --------------------------------------------------------------------------------


