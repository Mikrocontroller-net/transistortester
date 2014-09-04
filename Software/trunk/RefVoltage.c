#include <avr/io.h>
#include <avr/eeprom.h>
#include "Transistortester.h"

// computes RHmultip, adc_internal_reference (ADCconfig.U_Bandgap) and ref_mv_offs

void RefVoltage(void) {
// RefVoltage interpolates table RHtab corresponding to voltage ref_mv_offs .
// RHtab contain the factors to get capacity from load time with 470k for
// different Band gab reference voltages.
// for remember:
//resistor     470000 Ohm      1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
//uint16_t RHTAB[] MEM_TEXT = { 954, 903, 856, 814, 775, 740, 707, 676, 648};

#define Ref_Tab_Abstand 50		// displacement of table is 50mV
#define Ref_Tab_Beginn 1000		// begin of table is 1000mV

  unsigned int referenz;
  unsigned int y1, y2;
  uint8_t tabind;
  uint8_t tabres;

#ifdef AUTO_CAL
  referenz = ref_mv + (int16_t)eeprom_read_word((uint16_t *)(&ref_offset));
#else
  referenz = ref_mv + REF_C_KORR;
#endif
  ref_mv_offs = referenz;
#ifdef AUTO_RH
  if (referenz >= Ref_Tab_Beginn) {
     referenz -= Ref_Tab_Beginn;
  } else  {
     referenz = 0;		// limit to begin of table
  }
  tabind = referenz / Ref_Tab_Abstand;
  tabres = referenz % Ref_Tab_Abstand;
  tabres = Ref_Tab_Abstand-tabres;
  if (tabind > 7) {
     tabind = 7;		// limit to end of table
  }
  // interpolate the table of factors
  y1 = pgm_read_word(&RHtab[tabind]);
  y2 = pgm_read_word(&RHtab[tabind+1]);
  // RHmultip is the interpolated factor to compute capacity from load time with 470k
  RHmultip = ((y1 - y2) * tabres + (Ref_Tab_Abstand/2)) / Ref_Tab_Abstand + y2;
//########################################
#else
  RHmultip = DEFAULT_RH_FAKT;	// default Multiplier for capacity measurement with R_H (470KOhm)
#endif

#if PROCESSOR_TYP == 8
  referenz = 2560;		// use defined internal ADC Voltage
#else
  referenz = ref_mv;		// use the read out internal Reference voltage
#endif
#ifdef AUTO_CAL
  ADCconfig.U_Bandgap = (referenz + (int8_t)eeprom_read_byte((uint8_t *)&RefDiff));
#else
  ADCconfig.U_Bandgap = (referenz + REF_R_KORR);
#endif
  adc_internal_reference = ADCconfig.U_Bandgap;
 }

