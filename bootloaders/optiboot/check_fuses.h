/* Check the LFUSE and HFUSE data for known processors */
#ifndef __check_fuses_h_
#define __check_fuses_h_ 1
#include "fuse2bin.h"

#define BHFUSE HFUSE
#define BLFUSE LFUSE

/* ================================================================== */
#ifdef __AVR_ATmega163__
/* CKSEL is in LFUSE 3:0 */
 #define MAX_FREQ 8000000

 #if ((BLFUSE & 0x0e) == 0x02)
  #if (F_CPU < 900000) || (F_CPU > 1100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega163)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x06) || ((BLFUSE & 0x0f) == 0x05)
  #if (F_CPU < 100000) || (F_CPU > 4100000)
   #error "Ext RC mode, wrong Lfuse setting for this frequency (ATmega163)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (F_CPU < 32000) || (F_CPU > 33000)
   #error "LF Crystal mode, wrong Lfuse setting for this frequency (ATmega163)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a) || ((BLFUSE & 0x0f) == 0x0c)
  #if (F_CPU < 400000) || (F_CPU > (MAX_FREQ + 100000))
   #error "Ext Crystal mode, wrong Lfuse setting for this frequency (ATmega163)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e) || ((BLFUSE & 0x0f) == 0x0d)
  #if (F_CPU < 400000) || (F_CPU > 900000)
   #error "Ceramic resonator mode, wrong Lfuse setting for this frequency (ATmega163)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x00)
  #if (F_CPU > MAX_FREQ)
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

#endif		/* ATmega163 */

/* ================================================================== */
#if defined(__AVR_ATmega8U2__) || defined(AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__)
/* CKDIV8 is in LFUSE 7, CKSEL 3:0 is in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1)
  #error "CKSEL 1 is illegal for the ATmega32U2 family!"
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #error "CKSEL 3 is illegal for the ATmega32U2 family!"
 #endif

 #if ((BLFUSE & 0x0e) == 4)
  #error "CKSEL 4 and 5 is illegal for the ATmega32U2 family!"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 7300000) || (CK_FREQ > 8100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x06)
  #if (CK_FREQ < 400000) || (CK_FREQ > MAX_FREQ)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "LP Ceramic resonator , wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > 17000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U2 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > MAX_FREQ)
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif
#endif  /* __AVR_ATmega8U2__ || __AVR_ATmega16U2__ || __AVR_ATmega32U2__ */

/* ================================================================== */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
/* CKDIV8 is in LFUSE 7, CKSEL 3:0 is in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1)
  #error "CKSEL 1 is illegal for the ATmega32U4 family!"
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #error "CKSEL 3 is illegal for the ATmega32U4 family!"
 #endif

 #if ((BLFUSE & 0x0e) == 6)
  #error "CKSEL 6 and 7 is illegal for the ATmega32U4 family!"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 7300000) || (CK_FREQ > 8100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x04)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "LP Ceramic resonator , wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > 17000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega32U4 family)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > MAX_FREQ)
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif
#endif  /* __AVR_ATmega16U4__ || __AVR_ATmega32U4__ */

/* ================================================================== */
#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) || \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || \
    defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || \
    defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
/* RSTDISBL is in HFUSE 7  */
/* CKDIV8 is in LFUSE 7, CKSEL 3:0 is in LFUSE 3:0 */
 #define MAX_FREQ 20000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1)
  #error "CKSEL 1 is illegal for the ATmega328 family!"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 7200000) || (CK_FREQ > 8800000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if BAUD_RATE >= 100
   #error "CKSEL 3 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x04)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x06)
  #if (CK_FREQ < 400000) || (CK_FREQ > MAX_FREQ)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "LP Ceramic resonator , wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > 17000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...8)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > MAX_FREQ)
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATmega48/88/168/328 */

/* ================================================================== */
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__)
/* CKSEL is in LFUSE 3:0, CKOPT in HFUSE 4 */
 #define MAX_FREQ 16000000

 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0a)
  /* range 0.4 - 0.9 MHz */
  #if (F_CPU < 400000) || (F_CPU > 900000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0c)
  #if (F_CPU < 900000) || (F_CPU > 3000000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0e)
  #if (F_CPU < 3000000) || (F_CPU > 8000000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BHFUSE & 0x10) == 0) && ((BLFUSE & 0x0e) == 0x0e)
  #if (F_CPU < 1000000) || (F_CPU > (MAX_FREQ + 100000))
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x05)
  #if (F_CPU < 100000) || (F_CPU > 900000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x06)
  #if (F_CPU < 900000) || (F_CPU > 3000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x07)
  #if (F_CPU < 3000000) || (F_CPU > 8000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x08)
  #if (F_CPU < 3000000) || (F_CPU > 8000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x01)
  #if (F_CPU < 800000) || (F_CPU > 1200000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x02)
  #if (F_CPU < 1600000) || (F_CPU > 2400000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x03)
  #if (F_CPU < 3200000) || (F_CPU > 4800000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x04)
  #if (F_CPU < 6400000) || (F_CPU > 9600000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATmega8 */

/* ================================================================== */
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny24__) || \
    defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny24A__)
/* RSTDISBL in HFUSE 7 */
/* CKSEL is in LFUSE 3:0, CKDIV in LFUSE 8 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x0f) == 0x01) || ((BLFUSE & 0x0f) == 0x03) || \
     ((BLFUSE & 0x0f) == 0x05) || ((BLFUSE & 0x0f) == 0x07)
  #error "Wrong LFuse setting for this ATtiny.4!"
 #endif

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 4)
  #if BAUD_RATE >= 100
   #error "CKSEL 4 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 6)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny84/44/24 */

/* ================================================================== */
#if defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__)
/* ULPOSCSEL 2:1 in EFUSE 7:5 */
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7 */
/* CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x0f) == 0x01) || ((BLFUSE & 0x0f) == 0x03) || \
     ((BLFUSE & 0x0f) == 0x05) || ((BLFUSE & 0x0f) == 0x07)
   #error "Wrong LFuse setting for this ATtiny.41!"
 #endif

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 4)
  #error "The internal ULP oszillator is not usable for the bootloader!"
 #endif

 #if ((BLFUSE & 0x0f) == 6)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny.41)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny841/441 */

/* ================================================================== */
#if defined(__AVR_ATtiny88__) || defined(__AVR_ATtiny48__)
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7, CKSEL 1:0 in LFUSE 1:0 */
 #define MAX_FREQ 12000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x03) == 0x01) 
   #error "Wrong LFuse setting for this ATtiny88/48!"
 #endif

 #if ((BLFUSE & 0x03) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny88/48)!"
  #endif
 #endif

 #if ((BLFUSE & 0x03) == 3)
  #if BAUD_RATE >= 100
   #error "CKSEL 3 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x03) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny88/48 */

/* ================================================================== */
#if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 0x01) 
   #error "Wrong LFuse setting for this ATtiny87/167!"
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if BAUD_RATE >= 100
   #error "CKSEL 3 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0c) == 4)	/* 4-7 */
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny87/167)!"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny87/167 */

/* ================================================================== */
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 20000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 5) || ((BLFUSE & 0x0f) == 7)
  #error "Wrong CKSEL in LFUSE for ATtiny85/45/25 !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if (F_CPU < 1400000) || (F_CPU > 1800000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 4)
  #if BAUD_RATE >= 100
   #error "CKSEL 4 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 6)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny85/45/25)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny25/45/85 */

/* ================================================================== */
#if defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny261__) || \
    defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny261A__) 
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 20000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1)
  #if (F_CPU < 14000000) || (F_CPU > 18000000)
   #error "Int PLL mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
  #define PLL_MODE 1
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if BAUD_RATE >= 100
   #error "CKSEL 3 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0c) == 0x04)		/* 4-7 */
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny861/461/261 */

/* ================================================================== */
#if defined(__AVR_ATtiny4313__) || defined(__AVR_ATtiny2313__) || \
    defined(__AVR_ATtiny4313A__) || defined(__AVR_ATtiny2313A__)
/* RSTDISBL in HFUSE 0 */ 
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 20000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1) || ((BLFUSE & 0x0f) == 3) || \
     ((BLFUSE & 0x0f) == 5) || ((BLFUSE & 0x0f) == 7) 
  #error "Wrong CKSEL in LFUSE for ATtiny4313/2313 !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 5000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny4313/2313)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny4313/2313)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 6)
  #if BAUD_RATE >= 100
   #error "CKSEL 6 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny861/461/261)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif		/* ATtiny4313/2313 */

/* ================================================================== */
#if defined(__AVR_ATtiny1634__)
/* RSTDISBL in HFUSE 7 */ 
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 12000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1) || ((BLFUSE & 0x0f) == 3) || \
     ((BLFUSE & 0x0f) == 5) || ((BLFUSE & 0x0f) == 7) 
  #error "Wrong CKSEL in LFUSE for ATtiny1634 !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x04)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 100000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATtiny1634)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif	/* ATtiny1634 */

/* ================================================================== */
#if defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || \
    defined(__AVR_ATmega164PA__) || \
    defined(__AVR_ATmega324__) || defined(__AVR_ATmega324P__) || \
    defined(__AVR_ATmega324PA__) || \
    defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || \
    defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || \
    defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || \
    defined(__AVR_ATmega1284A__) || \
    defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
    defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
/* RSTDISBL in HFUSE 7 */ 
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
     defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
  #define MAX_FREQ 16000000
 #else
  #define MAX_FREQ 20000000
 #endif

 #if ((BLFUSE & 0x0f) == 0x01)
  #error "CKSEL 1 is not allowed for this ATmega !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 6000000) || (CK_FREQ > 10000000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 3)
  #if BAUD_RATE >= 100
   #error "CKSEL 3 (128k) is not usable for the bootloader with fix baud rate!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x04)
  #if (CK_FREQ < 32000) || (CK_FREQ > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x06)
  #if (CK_FREQ < 400000) || (CK_FREQ > MAX_FREQ)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "LP Ceramic resonator , wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > 17000000)
   #error "LP Crystal mode, wrong Lfuse setting for this frequency (ATmega...4)!"
  #endif
 #endif


 #if ((BLFUSE & 0x0f) == 0x00)
  #if CK_FREQ > (MAX_FREQ + 1000000)
   #error "Wrong Frequency selection for external Clock!"
  #else
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

 #if !defined(__AVR_ATmega640__) && !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega1281__) && \
     !defined(__AVR_ATmega2560__) && !defined(__AVR_ATmega2561__)
  #if ((BHFUSE & 0x80) == 0)
   #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
   #if !defined(FORCE_RSTDISBL)
    #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
   #endif
  #endif
 #endif

#endif	/* ATmega164/324..  ATmega640/1280..   */

/* ================================================================== */
#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || \
    defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__) || \
    defined(__AVR_ATmega64__) || defined(__AVR_ATmega64A__) || \
    defined(__AVR_ATmega128__)
/* CKOPT in HFUSE 4 */
/* CKSEL 3:0 is in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x0f) == 0x01)
  #if (F_CPU < 800000) || (F_CPU > 1200000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x02)
  #if (F_CPU < 1600000) || (F_CPU > 2400000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x03)
  #if (F_CPU < 3200000) || (F_CPU > 4800000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x04)
  #if (F_CPU < 6400000) || (F_CPU > 9600000)
   #error "Int RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x05)
  #if (F_CPU < 100000) || (F_CPU > 900000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x06)
  #if (F_CPU < 900000) || (F_CPU > 3000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x07)
  #if (F_CPU < 3000000) || (F_CPU > 12000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x08)
  #if (F_CPU < 3000000) || (F_CPU > 8000000)
   #error "Ext RC mode, wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x09)
  #if (F_CPU < 32000) || (F_CPU > 34000)
   #error "Low Freq. oscillator , wrong Lfuse setting for this frequency (ATmega16/32)!"
  #endif
 #endif

 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0a)
  /* range 0.4 - 0.9 MHz */
  #if (F_CPU < 400000) || (F_CPU > 900000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif
 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0c)
  #if (F_CPU < 900000) || (F_CPU > 3000000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif
 #if ((BHFUSE & 0x10) == 1) && ((BLFUSE & 0x0e) == 0x0e)
  #if (F_CPU < 3000000) || (F_CPU > 8000000)
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BHFUSE & 0x10) == 0) && ((BLFUSE & 0x0e) == 0x0e)
  #if (F_CPU < 1000000) || (F_CPU > (MAX_FREQ + 100000))
   #error "wrong Fuse setting for this Frequency!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x00)
  #if CK_FREQ > (MAX_FREQ + 1000000)
   #error "Wrong Frequency selection for external Clock!"
  #else
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

#endif	/* ATmega16/32 ... */



/* ================================================================== */
#if defined(__AVR_ATmega165__) || defined(__AVR_ATmega165P__) || \
    defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165PA__) || \
    defined(__AVR_ATmega325__) || defined(__AVR_ATmega325P__) || \
    defined(__AVR_ATmega325A__) || defined(__AVR_ATmega325PA__) || \
    defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250P__) || \
    defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250PA__) || \
    defined(__AVR_ATmega645__) || defined(__AVR_ATmega645P__) || \
    defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645PA__) || \
    defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6450P__) || \
    defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450PA__) || \
    defined(__AVR_ATmega169__) || defined(__AVR_ATmega169P__) || \
    defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || \
    defined(__AVR_ATmega329__) || defined(__AVR_ATmega329P__) || \
    defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290P__) || \
    defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega649__) || defined(__AVR_ATmega649P__) || \
    defined(__AVR_ATmega649A__) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490P__) || \
    defined(__AVR_ATmega6490A__) 
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
/* RSTDISBL in EFUSE 0 for ATmegaxx9, can not be checked! */

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if defined(__AVR_ATmega165__) || defined(__AVR_ATmega165P__) || \
     defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165PA__) || \
     defined(__AVR_ATmega645__) || defined(__AVR_ATmega645P__) || \
     defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645PA__) || \
     defined(__AVR_ATmega169__) || defined(__AVR_ATmega169P__) || \
     defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || \
     defined(__AVR_ATmega329__) || defined(__AVR_ATmega329P__) || \
     defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
     defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290P__) || \
     defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
     defined(__AVR_ATmega649__) || defined(__AVR_ATmega649P__) || \
     defined(__AVR_ATmega649A__) || \
     defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490P__) || \
     defined(__AVR_ATmega6490A__) 
  #define MAX_FREQ 16000000
 #else
  #define MAX_FREQ 20000000
 #endif

 #if ((BLFUSE & 0x0f) == 0x01) || ((BLFUSE & 0x0f) == 0x03) || \
     ((BLFUSE & 0x0f) == 0x04) || ((BLFUSE & 0x0f) == 0x05) 
  #error "illegal CKSEL setting in LFUSE for ATmega...5 !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 7300000) || (CK_FREQ > 8100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x06)
  #if (CK_FREQ < 32000) || (CK_FREQ > 33000)
   #error "LF Crystal mode, wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 1000000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (ATmega...5)!"
  #endif
 #endif


 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > (MAX_FREQ + 100000))
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

#endif	/* ATmega165/325  ATmega169/329 */


/* ================================================================== */
#if defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 1) || ((BLFUSE & 0x0f) == 3) 
  #error "Wrong CKSEL in LFUSE for AT90CAN.. !"
 #endif

 #if ((BLFUSE & 0x0f) == 2)
  #if (CK_FREQ < 7300000) || (CK_FREQ > 8100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x04) || ((BLFUSE & 0x0f) == 0x07)
  #if (CK_FREQ < 32000) || (CK_FREQ > 33000)
   #error "LF Crystal mode, wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 8)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 1000000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90CAN...)!"
  #endif
 #endif


 #if ((BLFUSE & 0x0f) == 0x00)
  #if (CK_FREQ > MAX_FREQ)
   #error "Frequency setting is illegal for external clock!"
  #else 
   #warning ">>>>> External clock is selectet! <<<<<"
  #endif
 #endif

#endif	/* AT90CAN... */

/* ================================================================== */
#if defined(__AVR_AT90PWM2__) || defined(__AVR_AT90PWM3__) || \
    defined(__AVR_AT90PWM2B__) || defined(__AVR_AT90PWM3B__)
/* RSTDISBL in HFUSE 7 */
/* CKDIV8 in LFUSE 7, CKSEL 3:0 in LFUSE 3:0 */
 #define MAX_FREQ 16000000

 #if ((BLFUSE & 0x80) == 0x00)
  #define CK_FREQ  (F_CPU*8)
 #else
  #define CK_FREQ  F_CPU
 #endif

 #if ((BLFUSE & 0x0f) == 0x01) || ((BLFUSE & 0x0f) == 0x04) || \
     ((BLFUSE & 0x0f) == 0x05) || ((BLFUSE & 0x0f) == 0x06) || \
     ((BLFUSE & 0x0f) == 0x07) 
  #error "CKSEL in LFUSE is set illegal for AT90PWM... !"
 #endif

 #if ((BLFUSE & 0x0f) == 0x02)
  #if (CK_FREQ < 7300000) || (CK_FREQ > 8100000)
   #error "Int RC mode, wrong Lfuse setting for this frequency (AT90PWM..)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0f) == 0x03)
  #if (CK_FREQ < 14600000) || (CK_FREQ > 16200000)
   #error "Int PLL mode, wrong Lfuse setting for this frequency (AT90PWM..)!"
  #endif
  #define PLL_MODE 1
 #endif

 #if ((BLFUSE & 0x0e) == 0x08)
  #if (CK_FREQ < 400000) || (CK_FREQ > 900000)
   #error "Ceramic resonator , wrong Lfuse setting for this frequency (AT90PWM...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0a)
  #if (CK_FREQ < 900000) || (CK_FREQ > 3000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90PWM...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0c)
  #if (CK_FREQ < 3000000) || (CK_FREQ > 8000000)
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90PWM...)!"
  #endif
 #endif

 #if ((BLFUSE & 0x0e) == 0x0e)
  #if (CK_FREQ < 8000000) || (CK_FREQ > (MAX_FREQ + 1000000))
   #error "Crystal mode, wrong Lfuse setting for this frequency (AT90PWM...)!"
  #endif
 #endif

 #if ((BHFUSE & 0x80) == 0)
  #warning "RSTDISBL is set in HFUSE, you can NOT use ISP again."
  #if !defined(FORCE_RSTDISBL)
   #error "Setting of Fuse RSTDISBL not allowed without Option FORCE_RSTDISBL"
  #endif
 #endif

#endif	/* AT90PWM... */
#endif
