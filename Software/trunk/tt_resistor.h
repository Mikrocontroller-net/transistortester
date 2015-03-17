#if defined(MAIN_C)
unsigned long ResistorVal[3];	// Values of resistor 0:1, 0:2, 1:2
uint8_t ResistorList[3];	// list of the found resistor Numbers
uint8_t ResistorChecked[3];	// 2, if resistor is checked in both directions
uint8_t ResistorsFound;	//Number of found resistors
const uint8_t RPinTab[] MEM_TEXT = {TP1, TP1, TP2, TP3, TP3};

 #if FLASHEND > 0x1fff
   unsigned long inductor_lx;	// inductance 10uH or 100uH
   int8_t inductor_lpre;	// prefix for inductance
 #endif

#else  /* no main */

extern unsigned long ResistorVal[3];	// Values of resistor 0:1, 0:2, 1:2
extern uint8_t ResistorList[3];	// list of the found resistor Numbers
extern uint8_t ResistorChecked[3];	// 2, if resistor is checked in both directions
extern  uint8_t ResistorsFound;	//Number of found resistors
extern const uint8_t RPinTab[] MEM_TEXT;

 #if FLASHEND > 0x1fff
   unsigned long inductor_lx;	// inductance 10uH or 100uH
   int8_t inductor_lpre;	// prefix for inductance
 #endif
#endif
