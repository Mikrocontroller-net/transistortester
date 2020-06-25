/*
 * pin_defs.h
 * optiboot helper defining the default pin assignments (LED, SOFT_UART)
 * for the various chips that are supported.  This also has some ugly macros
 * for selecting among various UARTs and LED possibilities using command-line
 * defines like "UART=2 LED=B5"
 *
 * reorganized 2016 by K.-H. Kuebbeler
 * splitted in pin_defs.h and led_defs.h  2018 by K.-H. Kuebbeler 
 * UART_TX and UART_RX can be selected with the LED style  (D0 or B5)
 * Copyright 2013-2015 by Bill Westfield.
 * Copyright 2010 by Peter Knight.
 * This software is licensed under version 2 of the Gnu Public Licence.
 * See optiboot.c for details.
 */

#ifndef INVERSE_UART
 #define INVERSE_UART 0
#endif

#ifndef UART
 #define UART_NR 0		/* predefine 0 as default UART number */
#else
 #define UART_NR UART
#endif

#ifndef SOFT_UART
 #define SOFT_UART 0
#endif

#ifndef VIRTUAL_BOOT_PARTITION
 #define VIRTUAL_BOOT_PARTITION 0
#endif


#include <avr/io.h>
#include "short_port_codes.h"


/* First make undefined ports equal to p */
#if !defined(UART_RX)
 #warning undefined UART_RX
 #define UART_RX p
#endif
#if !defined(UART_TX)
 #warning undefined UART_TX
 #define UART_TX p
#endif

#if defined(UDR) && !defined(UDRE0)
  //Name conversion R.Wiersma
  #undef UDR0		/* probably was Bit 0 of UDR before */
  #define UDR0 		UDR
  /* and rename some Bits */
  #define UDRE0 	UDRE
  #define RXC0		RXC
  #define FE0           FE
  #define U2X0		U2X
  #define RXEN0		RXEN
  #define TXEN0		TXEN
#endif		/* defined(UDR)  ... */
#if !defined(UDR0) && defined(UDR1)
 #define UDR0 		UDR1
 #define UDRE0 	UDRE1
 #define RXC0		RXC1
 #define FE0           FE1
#endif

#if (!defined(UDR0)) && defined(LINDAT)
  // ATtiny87 and ATtiny167 has a LIN/UART controller
 #define UDR0		LINDAT

 #define UCSR0A		LINBTR
 #define UART_SCALER8	(8|(1<<LDISR))
 #define UART_SCALER16	(16|(1<<LDISR))

 #define UART_STATUS	LINSIR
 #define UDRE0		LTXOK
 #define RXC0		LRXOK

 #define UART_ERRSTAT	LINERR
 #define FE0		LFERR	/* framing error */
 #define DOR0		LOVERR	/* data overrun */
 #define UPE0		LPERR	/* parity error */

 #define UART_ENABLE_RX (1<<LENA)|(1<<LCMD2)|(1<<LCMD1)
 #define UART_ENABLE_TX (1<<LENA)|(1<<LCMD2)|(1<<LCMD0)
#else
 #if !defined(U2X0)
  /* no UART0 defined */
  #define UART_SCALER8	(1<<U2X1)
  #define UART_SCALER16	(0<<U2X1)
 
  #define UART_ENABLE_RX (1<<RXEN1)
  #define UART_ENABLE_TX (1<<TXEN1)
 #else
  #define UART_SCALER8	(1<<U2X0)
  #define UART_SCALER16	(0<<U2X0)
 
  #define UART_ENABLE_RX (1<<RXEN0)
  #define UART_ENABLE_TX (1<<TXEN0)
 #endif
#endif

#ifndef UBRR0L
 #if defined(UBRR)
  #define UBRR0L	UBRR
 #elif defined(UBRRL)
  #define UBRR0L	UBRRL
 #elif defined(LINBRRL)
  #define UBRR0L	LINBRRL
 #endif
#endif

#ifndef UBRR0H
 #ifdef UBRRHI
  #define UBRR0H	UBRRHI
 #elif defined(UBRRH)
  #define UBRR0H	UBRRH
 #elif defined(LINBRRH)
  #define UBRR0H	LINBRRH
 #endif
#endif

#ifndef WDTCSR
  #define WDTCSR	WDTCR
#endif

#if (!defined(UCSR0A)) && defined(UCSRA)
  // Make the only UART to the UART0
  #define UCSR0A	UCSRA
  #define UCSR0B	UCSRB
#endif
#if (!defined(UCSR0B)) && defined(LINCR)
  #define UCSR0B	LINCR
#endif

#if (!defined(UCSR0C)) && defined(UCSRC)
  #define UCSR0C	UCSRC
  #define UCSZ01	UCSZ1
  #define UCSZ00	UCSZ0
#endif
#if (!defined(UCSR0C)) && defined(UCSR1C)
  #define UCSR0C	UCSRC
  #define UCSZ01	UCSZ11
  #define UCSZ00	UCSZ10
#endif
#if (!defined(URSEL0)) && defined(URSEL)
  #define URSEL0	URSEL
#endif
#if (!defined(URSZ00)) && defined(URSZ0)
  #define URSZ00	URSZ0
  #define URSZ01	URSZ1
#endif

#if (!defined(EEPE)) && defined(EEWE) 
  // bits in EECR has other names
 #define EEPE		EEWE
 #define EEMPE		EEMWE
#endif

#if (!defined(TIFR1)) && defined(TIFR)
 #define TIFR1 		TIFR
#endif


/*------------------------------------------------------------------------ */
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || \
    defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || \
    defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) || \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) 
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATmega8/48/88/168/328 */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D0 as RX"
   #endif
  #endif
  #define UART_RXX      pD0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D1 as TX"
   #endif
  #endif
  #define UART_TXX      pD1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATmega168__  ... */

/* Luminet support */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny24__) || \
    defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny24A__)
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny84 */
 /* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin A3 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pA3
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin A2 as TX"
   #endif
  #endif
  #define UART_TXX	pA2
 #else
  #define UART_TXX      UART_TX
 #endif
#endif	/* __AVR_ATtiny44__ || __AVR_ATtiny84__ */

/*------------------------------------------------------------------------ */
/* support for attiny841 and attiny441 */
#if defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__) 
/*------------------------------------------------------------------------ */
 /* usually all AVR devices without a bootpage region has set
    BOOT_PAGE_LEN and SPM_PAGESIZE to the same value,
    but tiny441 and tiny841 not */
  #undef NRWWSTART
  #define NRWWSTART 0

 /* Default "SOFT" UART Ports for ATtiny441/841 */
 #if UART_RX == p
  #if UART_NR == 0
    /* UART0 RX can be remapped to B2 with Bit U0MAP in the REMAP register */
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin A2 as RX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pA2
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin A4 as RX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pA4
  #endif
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    /* UART0 TX can be remapped to A7 with Bit U0MAP in the REMAP register */
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin A1 as TX for UART 0"
    #endif
   #endif
   #define UART_TXX	pA1
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin A5 as TX for UART 1"
    #endif
   #endif
   #define UART_TXX	pA5
  #endif
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATiny841__  .. */

/*------------------------------------------------------------------------ */

/* Support for ATtiny48 and ATtiny88 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny88__) || defined(__AVR_ATtiny48__)
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny84 */
 /* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
 #if SOFT_UART > 0
  #if UART_RX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D0 as RX"
   #endif
   #define UART_RXX	pD0
  #else
   #define UART_RXX      UART_RX
  #endif
  #if UART_TX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D1 as TX"
   #endif
   #define UART_TXX	pD1
  #else
   #define UART_TXX      UART_TX
  #endif
 #else
  #error "Attiny48 and ATtiny88 has no Hardware UART"
 #endif		/* SOFT_UART > 0 */
#endif	/* __AVR_ATtiny88__ || __AVR_ATtiny48__ */

/* Support for the ATtiny87 family */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__) 
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny87 ATtiny167 */
 /* Ports for soft UART - left port only for now. TX/RX on PA1/PA0 */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin A0 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pA0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin A1 as TX"
   #endif
  #endif
  #define UART_TXX	pA1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif	/* __AVR_ATtiny87__ || __AVR_ATtiny167__ */

/* Support for ATtiny85 family*/
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) 
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny85 */
 /* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
 #if SOFT_UART > 0
  #if UART_RX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin B0 as RX"
   #endif
   #define UART_RXX	pB0
  #else
   #define UART_RXX      UART_RX
  #endif
  #if UART_TX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin B1 as TX"
   #endif
   #define UART_TXX	pB1
  #else
   #define UART_TXX      UART_TX
  #endif
 #else
  #error "ATtiny85 family has no hardware UART"
 #endif		/* SOFT_UART > 0 */
#endif	/* __AVR_ATtiny85__  */

/* Support for ATtiny 26x family*/
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny261__) || \
    defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny261A__) 
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny26x */
 /* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
 #if SOFT_UART > 0
  #if UART_RX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin B0 as RX"
   #endif
   #define UART_RXX	pB0
  #else
   #define UART_RXX      UART_RX
  #endif
  #if UART_TX == p
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin B1 as TX"
   #endif
   #define UART_TXX	pB1
  #else
   #define UART_TXX      UART_TX
  #endif
 #endif		/* SOFT_UART > 0 */
#endif	/* __AVR_ATtiny86x__  */

/* Support for ATtiny4313 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny4313__) || defined(__AVR_ATtiny2313__) || \
    defined(__AVR_ATtiny4313A__) || defined(__AVR_ATtiny2313A__)
/*------------------------------------------------------------------------ */
 /* Default "SOFT" UART Ports for ATtiny4313 */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D0 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pD0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D1 as TX"
   #endif
  #endif
  #define UART_TXX	pD1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif	/* __AVR_ATtiny4313__ */

/* Support for ATtiny1634 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny1634__)
/*------------------------------------------------------------------------ */
 /* usually all AVR devices without a bootpage region has set
    BOOT_PAGE_LEN and SPM_PAGESIZE to the same value,
    but tiny1634 not */
  #undef NRWWSTART
  #define NRWWSTART 0

 /* Default "SOFT" UART Ports for ATtiny1634 */
 /* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin A7 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pA7
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin B0 as TX"
   #endif
  #endif
  #define UART_TXX	pB0
 #else
  #define UART_TXX      UART_TX
 #endif
#endif	/* __AVR_ATtiny1634__ */

/*------------------------------------------------------------------------ */
/* Sanguino support (and other 40pin DIP cpus) */
#if defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || \
    defined(__AVR_ATmega164PA__) || \
    defined(__AVR_ATmega324__) || defined(__AVR_ATmega324A__) || \
    defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || \
    defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || \
    defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || \
    defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || \
    defined(__AVR_ATmega1284A__) || \
    defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || \
    defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
/*------------------------------------------------------------------------ */
 #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || \
     defined(__AVR_ATmega16A__) || defined(__AVR_ATmega32A__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
  /* ATmega16/32 support only one UART */
  #undef UART_NR
  #define UART_NR 0
 #endif

 /* Default "SOFT" UART Ports for ATmega644/1284/32 */
 #if UART_RX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D0 as RX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pD0
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D2 as RX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pD2
  #endif
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D1 as TX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pD1
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D3 as TX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pD3
  #endif
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATmega644P__  .. */

/*------------------------------------------------------------------------ */
/* Support for ATmaga16U4, ATmega32U4, ATmega8U2, ATmega16U2, ATmega32U2 */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)  || \
    defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__)
/*------------------------------------------------------------------------ */
 #undef UART_NR
 #define UART_NR 1	/* support only UART No 1 */

 /* Default "SOFT" UART Ports for ATmega32U4... */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D2 as RX for UART 1"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pD2
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D3 as TX for UART 1"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pD3
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATmega16U4__ ||  __AVR_ATmega32U4__ ... */

/*------------------------------------------------------------------------ */
/* Support for AT90CAN32_64_128 */
#if defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__) 
/*------------------------------------------------------------------------ */

 /* Default "SOFT" UART Ports for AT90CAN... */
 #if UART_RX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin E0 as RX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pE0
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D2 as RX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pD2
  #endif
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin E1 as TX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pE1
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D3 as TX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pD3
  #endif
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_AT90CAN... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_AT90PWM2__) || defined(__AVR_AT90PWM2B__)
/*------------------------------------------------------------------------ */

 /* Default "SOFT" UART Ports for AT90PWM */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D4 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX      pD4
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D3 as TX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX      pD3
 #else
  #define UART_TXX      UART_TX
 #endif
 #if defined(__AVR_AT90PWM2__) && !defined(SIGNATURE_2)
   /* add missing Signature bytes */
   #define SIGNATURE_0	0x1e
   #define SIGNATURE_1	0x93
   #define SIGNATURE_2	0x81
 #endif
#endif		/* __AVR_AT90PWM... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_AT90PWM3__) || defined(__AVR_AT90PWM3B__)
/*------------------------------------------------------------------------ */

 /* Default "SOFT" UART Ports for AT90PWM */
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D4 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX      pD4
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D3 as TX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX      pD3
 #else
  #define UART_TXX      UART_TX
 #endif
 #if defined(__AVR_AT90PWM3__) && !defined(SIGNATURE_2)
   /* add missing Signature bytes */
   #define SIGNATURE_0	0x1e
   #define SIGNATURE_1	0x93
   #define SIGNATURE_2	0x81
 #endif
#endif		/* __AVR_AT90PWM... */

/*------------------------------------------------------------------------ */
/* Support for ATmega64 */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega64A__) || \
    defined(__AVR_ATmega128__)
/*------------------------------------------------------------------------ */

 /* Default "SOFT" UART Ports for ATmega644/1284/32 */
 #if UART_RX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin E0 as RX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pE0
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D2 as RX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pD2
  #endif
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin E1 as TX for UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pE1
  #else
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "SOFT_UART use Pin D3 as TX for UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pD3
  #endif
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATmega64__  .. */

#if defined(__AVR_ATmega162__) || defined(__AVR_ATmega163__) || defined(__AVR_ATmega323__)
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D0 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pD0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin D1 as TX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pD1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif

#if defined(__AVR_ATmega169__) || defined(__AVR_ATmega169P__) || \
    defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__)
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin E0 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pE0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin E1 as TX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pE1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif


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
    defined(__AVR_ATmega329__) || defined(__AVR_ATmega329P__) || \
    defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290P__) || \
    defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega649__) || defined(__AVR_ATmega649P__) || \
    defined(__AVR_ATmega649A__) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490P__) || \
    defined(__AVR_ATmega6490A__) 
 
 #if UART_RX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin E0 as RX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_RXX	pE0
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if SOFT_UART > 0
   #ifndef NO_WARNING
    #warning "SOFT_UART use Pin E1 as TX"
   #endif
  #endif		/* SOFT_UART > 0 */
  #define UART_TXX	pE1
 #else
  #define UART_TXX      UART_TX
 #endif
#endif

/*------------------------------------------------------------------------ */
/* Mega support */
#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
    defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
/*------------------------------------------------------------------------ */

 /* Default "SOFT" UART Ports for ATmega1280 */
 #if UART_RX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin E0 as RX for soft UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pE0
  #endif
  #if UART_NR == 1
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin D2 as RX for soft UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pD2
  #endif
  #if UART_NR == 2
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin H0 as RX for soft UART 2"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pH0
  #endif
  #if UART_NR == 3
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin J0 as RX for soft UART 3"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_RXX	pJ0
  #endif
 #else
  #define UART_RXX      UART_RX
 #endif
 #if UART_TX == p
  #if UART_NR == 0
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin E1 as TX for soft UART 0"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pE1
  #endif
  #if UART_NR == 1
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin D3 as TX for soft UART 1"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pD3
  #endif
  #if UART_NR == 2
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin H1 as TX for soft UART 2"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pH1
  #endif
  #if UART_NR == 3
   #if SOFT_UART > 0
    #ifndef NO_WARNING
     #warning "ATmega use Pin J1 as TX for soft UART 3"
    #endif
   #endif		/* SOFT_UART > 0 */
   #define UART_TXX	pJ1
  #endif
 #else
  #define UART_TXX      UART_TX
 #endif
#endif		/* __AVR_ATmega640__ || __AVR_ATmega1280__ ... */

/*------------------------------------------------------------------------ */
/* Settings for all processors */
/*------------------------------------------------------------------------ */



/* Watchdog settings */
#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#if defined(WDP3)
 #define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
 #define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#else
 #define WATCHDOG_4S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE)) /* 2 seconds are max */
 #define WATCHDOG_8S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE)) /* 2 seconds are max */
#endif


// setup for UART_RX
//#if SOFT_UART > 0
 #if ((UART_RXX & 0xff) > 7) || ((UART_RXX >> 8) > 12) || ((UART_RXX & 0xff00) == 0)
  #error "Unrecognized UART_RX name.  Should be like 'B5'"
  #error "You can only select a existing port for the target processor!"
 #endif
 // setup for UART_TX
 #if ((UART_TXX & 0xff) > 7) || ((UART_TXX >> 8) > 12) || ((UART_TXX & 0xff00) == 0)
  #error "Unrecognized UART_TX name.  Should be like 'B5'"
  #error "You can only select a existing port for the target processor!"
 #endif
 // UART_RX_BIT must be set to the bit number of the selected Port.
 // This is allways given by the lower Byte of the previous definition
 #define UART_RX_BIT (UART_RXX & 0x07)
 // the same must be set for the TX_BIT
 #define UART_TX_BIT (UART_TXX & 0x07)
//#else		/* Hardware UART */
#if SOFT_UART == 0
 /*
  * Handle devices with up to 4 uarts (eg m1280.)  Rather inelegantly.
  * Note that mega8/m32 still needs special handling, because ubrr is handled
  * differently.
  */
 #if UART_NR == 0
  #if !defined(UDR0)
   #error "UART == 0, but no UART0 on device"
  #endif
  #define UART_SRA UCSR0A
  #define UART_SRB UCSR0B
  #ifdef UCSR0C
   #define UART_SRC UCSR0C
  #endif
  #ifdef URSEL0
   #define UART_SEL URSEL0
  #endif
  #define UART_SRRL UBRR0L
  #define UART_SRRH UBRR0H
  #define UART_UDR UDR0
 #elif UART_NR == 1
  #if !defined(UDR1)
   #error "UART == 1, but no UART1 on device"
  #endif
  #define UART_SRA UCSR1A
  #define UART_SRB UCSR1B
  #define UART_SRC UCSR1C
  #ifdef URSEL1
   #define UART_SEL URSEL1
  #endif
  #define UART_SRRL UBRR1L
  #define UART_SRRH UBRR1H
  #define UART_UDR UDR1
 #elif UART_NR == 2
  #if !defined(UDR2)
   #error "UART == 2, but no UART2 on device"
  #endif
  #define UART_SRA UCSR2A
  #define UART_SRB UCSR2B
  #define UART_SRC UCSR2C
  #ifdef URSEL2
   #define UART_SEL URSEL2
  #endif
  #define UART_SRRL UBRR2L
  #define UART_SRRH UBRR2H
  #define UART_UDR UDR2
 #elif UART_NR == 3
  #if !defined(UDR3)
   #error "UART == 3, but no UART3 on device"
  #endif
  #define UART_SRA UCSR3A
  #define UART_SRB UCSR3B
  #define UART_SRC UCSR3C
  #ifdef URSEL3
   #define UART_SEL URSEL3
  #endif
  #define UART_SRRL UBRR3L
  #define UART_SRRH UBRR3H
  #define UART_UDR UDR3
 #else
  #error "Unknown UART number, only 0 to 3 !!!"
 #endif
 
 #ifndef UART_ERRSTAT
  #define UART_ERRSTAT UART_SRA
 #endif
 #ifndef UART_STATUS
  #define UART_STATUS UART_SRA
 #endif
 #ifndef UART_CNTL
  #define UART_CNTL UART_SRA
 #endif

 // use a uniform version for the 2 Stop Bit transmission option bit
 #if !defined(USBS0) && defined(USBS)
  #define USBS0 USBS
 #else
  #if !defined(USBS0) && defined(USBS1)
   #define USBS0 USBS1
  #endif
 #endif

#endif		/* SOFT_UART > 0 */


//#if SOFT_UART > 0
 // setup the Port definitions for UART_RX
 #if (UART_RXX & 0xff00) == pB0
  #define UART_RX_DDR     DDRB
  #define UART_RX_PORT    PORTB
  #define UART_RX_PIN     PINB
 #elif (UART_RXX & 0xff00) == pC0
  #define UART_RX_DDR     DDRC
  #define UART_RX_PORT    PORTC
  #define UART_RX_PIN     PINC
 #elif (UART_RXX & 0xff00) == pD0
  #define UART_RX_DDR     DDRD
  #define UART_RX_PORT    PORTD
  #define UART_RX_PIN     PIND
 #elif (UART_RXX & 0xff00) == pE0
  #define UART_RX_DDR     DDRE
  #define UART_RX_PORT    PORTE
  #define UART_RX_PIN     PINE
 #elif (UART_RXX & 0xff00) == pF0
  #define UART_RX_DDR     DDRF
  #define UART_RX_PORT    PORTF
  #define UART_RX_PIN     PINF
 #elif (UART_RXX & 0xff00) == pG0
  #define UART_RX_DDR     DDRG
  #define UART_RX_PORT    PORTG
  #define UART_RX_PIN     PING
 #elif (UART_RXX & 0xff00) == pH0
  #define UART_RX_DDR     DDRH
  #define UART_RX_PORT    PORTH
  #define UART_RX_PIN     PINH
 #elif (UART_RXX & 0xff00) == pJ0
  #define UART_RX_DDR     DDRJ
  #define UART_RX_PORT    PORTJ
  #define UART_RX_PIN     PINJ
 #elif (UART_RXX & 0xff00) == pK0
  #define UART_RX_DDR     DDRK
  #define UART_RX_PORT    PORTK
  #define UART_RX_PIN     PINK
 #elif (UART_RXX & 0xff00) == pL0
  #define UART_RX_DDR     DDRL
  #define UART_RX_PORT    PORTL
  #define UART_RX_PIN     PINL
 #elif (UART_RXX & 0xff00) == pA0
  #define UART_RX_DDR     DDRA
  #define UART_RX_PORT    PORTA
  #define UART_RX_PIN     PINA
 
 #else
  #error -------------------------------------------
  #error Unrecognized UART_RX name.  Should be like "B5"
  #error -------------------------------------------
 #endif
//#endif 		/* SOFT_UART > 0 */


//#if SOFT_UART > 0
 // setup the Port definitions for UART_TX
 #if (UART_TXX & 0xff00) == pB0
  #define UART_TX_DDR     DDRB
  #define UART_TX_PORT    PORTB
  #define UART_TX_PIN     PINB
 #elif (UART_TXX & 0xff00) == pC0
  #define UART_TX_DDR     DDRC
  #define UART_TX_PORT    PORTC
  #define UART_TX_PIN     PINC
 #elif (UART_TXX & 0xff00) == pD0
  #define UART_TX_DDR     DDRD
  #define UART_TX_PORT    PORTD
  #define UART_TX_PIN     PIND
 #elif (UART_TXX & 0xff00) == pE0
  #define UART_TX_DDR     DDRE
  #define UART_TX_PORT    PORTE
  #define UART_TX_PIN     PINE
 #elif (UART_TXX & 0xff00) == pF0
  #define UART_TX_DDR     DDRF
  #define UART_TX_PORT    PORTF
  #define UART_TX_PIN     PINF
 #elif (UART_TXX & 0xff00) == pG0
  #define UART_TX_DDR     DDRG
  #define UART_TX_PORT    PORTG
  #define UART_TX_PIN     PING
 #elif (UART_TXX & 0xff00) == pH0
  #define UART_TX_DDR     DDRH
  #define UART_TX_PORT    PORTH
  #define UART_TX_PIN     PINH
 #elif (UART_TXX & 0xff00) == pJ0
  #define UART_TX_DDR     DDRJ
  #define UART_TX_PORT    PORTJ
  #define UART_TX_PIN     PINJ
 #elif (UART_TXX & 0xff00) == pK0
  #define UART_TX_DDR     DDRK
  #define UART_TX_PORT    PORTK
  #define UART_TX_PIN     PINK
 #elif (UART_TXX & 0xff00) == pL0
  #define UART_TX_DDR     DDRL
  #define UART_TX_PORT    PORTL
  #define UART_TX_PIN     PINL
 #elif (UART_TXX & 0xff00) == pA0
  #define UART_TX_DDR     DDRA
  #define UART_TX_PORT    PORTA
  #define UART_TX_PIN     PINA
 
 #else
  #error -------------------------------------------
  #error Unrecognized UART_TX name.  Should be like "B5"
  #error -------------------------------------------
 #endif
//#endif 		/* SOFT_UART > 0 */

#if (UART_TXX == UART_RXX)
 #define UART_ONE_WIRE 
 #if INVERSE_UART > 0
  #warning UART RX and TX to the same Port Bit not possible with INVERSE_UART! Option is canceled!
  #undef INVERSE_UART
  #define INVERSE_UART 0
 #endif
 #if SOFT_UART == 0
  #ifndef NO_WARNING
   #warning "for using One-Wire mode with hardware UART, please connect TX and RX pin!"
  #endif
 #endif
#endif

/* Virtual boot partition support */
#if VIRTUAL_BOOT_PARTITION > 0
 #define rstVect0_sav (*(uint8_t*)(RAMSTART+SPM_PAGESIZE+4))
 #define rstVect1_sav (*(uint8_t*)(RAMSTART+SPM_PAGESIZE+5))
 #define saveVect0_sav (*(uint8_t*)(RAMSTART+SPM_PAGESIZE+6))
 #define saveVect1_sav (*(uint8_t*)(RAMSTART+SPM_PAGESIZE+7))
// Vector to save original reset jump:
//   SPM Ready is least probably used, so it's default
//   if not, use old way WDT_vect_num,
//   or simply set custom save_vect_num in Makefile using vector name
//   or even raw number.
 #if !defined(SPM_RDY_vect_num) && defined(SPM_READY_vect_num)
  // rename _READY_ to _RDY_
  #define SPM_RDY_vect_num (SPM_READY_vect_num)
 #endif

 #if !defined(EE_RDY_vect_num) && defined(EE_READY_vect_num)
  // rename _READY_ to _RDY_
  #define EE_RDY_vect_num (EE_READY_vect_num)
 #elif !defined(EE_RDY_vect_num) && defined(EEPROM_Ready_vect_num)
  // rename EEPROM_Ready_ to EE_RDY
  #define EE_RDY_vect_num (EEPROM_Ready_vect_num)
 #endif

 #if !defined(WDT_vect_num) && defined(WDT_OVERFLOW_vect_num)
  // rename _OVERFLOW_ to _
  #define WDT_vect_num (WDT_OVERFLOW_vect_num)
 #endif

 #if !defined(UART_TX_vect_num) && defined(USART_TX_vect_num)
  // rename USART_TX_ to UART_TX_
  #define UART_TX_vect_num USART_TX_vect_num
 #endif

 #if FLASHEND > 8192
  #define Vect2Byte 4
 #else
  #define Vect2Byte 2
 #endif

 #if !defined (save_vect_num)
  #if (defined (SPM_RDY_vect_num)) && ((SPM_RDY_vect_num * Vect2Byte) < SPM_PAGESIZE)
   #if !(defined(NO_WARNING)) && (VerboseLev > 3)
    #warning "SPM_RDY_vect_num is selected as save_vect_num for virtual boot partition!"
   #endif
   #define save_vect_num (SPM_RDY_vect_num)
  #elif (defined (EE_RDY_vect_num)) && ((EE_RDY_vect_num * Vect2Byte) < SPM_PAGESIZE)
   #if !(defined(NO_WARNING)) && (VerboseLev > 3)
    #warning "EE_RDY_vect_num is selected as save_vect_num for virtual boot partition!"
   #endif
   #define save_vect_num (EE_RDY_vect_num)
  #elif (defined (WDT_vect_num)) && ((WDT_vect_num * Vect2Byte) < SPM_PAGESIZE)
   #if !(defined(NO_WARNING)) && (VerboseLev > 3)
    #warning "WDT_vect_num is selected as save_vect_num for virtual boot partition!"
   #endif
   #define save_vect_num (WDT_vect_num)
  #elif (defined (UART_TX_vect_num)) && ((UART_TX_vect_num * Vect2Byte) < SPM_PAGESIZE)
   #if !(defined(NO_WARNING)) && (VerboseLev > 3)
    #warning "UART_TX_vect_num is selected as save_vect_num for virtual boot partition!"
   #endif
   #define save_vect_num (UART_TX_vect_num)
  #else
   #error "Cant find SPM, EE, WDT or UART_TX interrupt vector for this CPU to support virtual boot partition"
  #endif
 #endif //save_vect_num

 // check if it's on the same page (code assumes that)
 #if (SPM_PAGESIZE <= (save_vect_num * Vect2Byte))
  #error "Save vector not in the same page as reset vector!"
 #endif

 #if FLASHEND > 8192
  // AVRs with more than 8k of flash have 4-byte vectors, and use jmp.
  //  We save only 16 bits of address, so devices with more than 128KB
  //  may behave wrong for upper part of address space.
  #define rstVect0 2
  #define rstVect1 3
  #define save_vect_addr (save_vect_num*4)
  #define saveVect0 (save_vect_num*4+2)
  #define saveVect1 (save_vect_num*4+3)
  #define appstart_vec (save_vect_num*2)
 #else
  // AVRs with up to 8k of flash have 2-byte vectors, and use rjmp.
  #define rstVect0 0
  #define rstVect1 1
  #define save_vect_addr (save_vect_num*2)
  #define saveVect0 (save_vect_num*2)
  #define saveVect1 (save_vect_num*2+1)
  #define appstart_vec (save_vect_num)
 #endif
#else
 #define appstart_vec (0)
#endif // VIRTUAL_BOOT_PARTITION

// AVR processors with more than 128kbyte flash have slower rcall and ret commands
#undef RCALL_TICS               /* can be redefined as constant value, depends only on processor type */
#if FLASHEND > 0x1ffff
 #define RCALL_TICS 9           /* sum of tics for rcall and ret if more than 128kbyte flash */
#else
 #define RCALL_TICS 7           /* sum of tics for rcall and ret  if less than 128kbyte flash */
#endif

