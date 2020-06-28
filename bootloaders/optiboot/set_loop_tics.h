// set_loop_tics.h
// loop requires 9 or 10 tics depending on IO address
// you must define the UART_PORT makro to a valid IO port

#undef LOOP_TICS	/* new LOOP_TICS will be determined with UART_PORT address and INVERS_UART */
#if	(UART_PORT < (0x40 + __SFR_OFFSET))
 // for this Port we can use IN and OUT commands
  #define LOOP_TICS 9
#else
 // (UART_PORT >= (0x40 + __SFR_OFFSET))
 // for this Port we must use LDS and STS commands
  #define LOOP_TICS 10
#endif

