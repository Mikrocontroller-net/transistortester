#if defined(MAIN_C)
unsigned long ResistorVal[3];	// Values of resistor 0:1, 0:2, 1:2
uint8_t ResistorList[3];	// list of the found resistor Numbers
uint8_t ResistorChecked[3];	// 2, if resistor is checked in both directions
uint8_t ResistorsFound;	//Number of found resistors

 #if FLASHEND > 0x1fff
   unsigned long inductor_lx;	// inductance 10uH or 100uH
   int8_t inductor_lpre;	// prefix for inductance
 #endif

#else  /* no main */

extern unsigned long ResistorVal[3];	// Values of resistor 0:1, 0:2, 1:2
extern uint8_t ResistorList[3];	// list of the found resistor Numbers
extern uint8_t ResistorChecked[3];	// 2, if resistor is checked in both directions
extern  uint8_t ResistorsFound;	//Number of found resistors

 #if FLASHEND > 0x1fff
   unsigned long inductor_lx;	// inductance 10uH or 100uH
   int8_t inductor_lpre;	// prefix for inductance
 #endif
#endif

#ifdef SamplingADC
// these belong to the LC resonant frequency measurement 
extern uint32_t  lc_fx;      // measured resonant frequency
#define          lc_fpre 0          // ... and its (fixed) prefix
extern uint16_t  lc_qx;     // measured resonance Q
#define          lc_qpre -1         // ... fixed prefix for Q: units of 10^{-1}
extern uint32_t  lc_lx;      // measured inductance using the samplingADC method
#define          lc_lpre -9         // ... and fixed prefix
extern uint16_t  lc_cpar;    // value of parallel capacitor used for calculating inductance, in pF
extern uint16_t  lc_cpartmp;    // used to "exfiltrate" the raw capacitance value from ReadCapacitance()
extern const uint16_t lc_cpar_ee EEMEM;         // place for lc_cpar as calibration constant in eeprom
#endif

