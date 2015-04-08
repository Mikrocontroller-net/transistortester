/* tt_globals.h defines global variables in RAM ans EEprom */


#if defined (MAIN_C)
 #define COMMON

#ifdef AUTO_CAL
  const int8_t RefDiff EEMEM = REF_R_KORR;	// correction of internal Reference Voltage

//  const uint16_t cap_null EEMEM = C_NULL;	// Zero offset of capacity measurement 
   const int16_t ref_offset EEMEM = REF_C_KORR;	// default correction of internal reference voltage for capacity measurement
   // the zero offset for capacity measurement for all pin combinations
  // LoPin:HiPin                        2:1    3:1    1:2                    marker  3:2                   1:3    2:3
   const uint8_t c_zero_tab[] EEMEM = { C_NULL,C_NULL,C_NULL+TP2_CAP_OFFSET,C_NULL+2,C_NULL+TP2_CAP_OFFSET,C_NULL,C_NULL }; //table of zero offsets
   // if the marker position of c_zero_tab is not equal the first position, the calibration has not run before
#endif

  const uint8_t EE_ESR_ZEROtab[] EEMEM = {ESR_ZERO, ESR_ZERO, ESR_ZERO, ESR_ZERO};	// zero offset of ESR measurement
#ifdef WITH_ROTARY_SWITCH
//  const uint8_t EE_RotarySwitch EEMEM = 0;	// rotation switch is not detected
#endif
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
  const uint8_t EE_Volume_Value EEMEM = VOLUME_VALUE;	// Volume Value for ST7565 controller 
#endif

//End of EEPROM-Strings
#else
 // no MAIN_C
 #define COMMON extern
 #ifdef AUTO_CAL
//  extern uint16_t R680pl;
//  extern uint16_t R680mi;
  extern int8_t RefDiff;
  extern uint16_t ref_offset;
  extern uint8_t c_zero_tab[];
 #endif
  extern const uint8_t EE_ESR_ZEROtab[] EEMEM;	// zero offset of ESR measurement

 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
  extern const uint8_t EE_Volume_Value EEMEM;	// Volume Value for ST7565 controller 
 #endif
#endif /* if defined (MAIN_C) */


struct Diode_t {
  uint8_t Anode[6];
  uint8_t Cathode[6];
  unsigned int Voltage[6];
};
COMMON struct Diode_t diodes;

struct Switch_t {

  union {
  unsigned long Pw;	// combined Mask
  uint8_t R[4];	// mask to switch a Pin with R_L,  mask to switch a Pin with R_H
  } Pin;

};

COMMON uint8_t NumOfDiodes;
COMMON uint8_t diode_sequence;

typedef struct {
  unsigned long hfe;		//current amplification factor 
  unsigned int uBE;		//B-E-voltage of the Transistor or RDS for E-MOS
  unsigned int current;		// current of Drain in 0.01mA
  				// for bipolar current is ICE0
  unsigned int ice0;		// for BJT ICEO
  unsigned int gthvoltage;	//Gate-threshold voltage 
  				// for bipolar gthvoltage is ICEs in 0.01mA
 #define ices gthvoltage
  uint8_t b,c,e;		//pins of the Transistor
  uint8_t count;
}trans_t;

COMMON trans_t ptrans;		// parameters of P type transistor
COMMON trans_t ntrans;		// parameters of N type transistor
COMMON trans_t *_trans;		// pointer to trans_t structure

COMMON uint8_t tmpval, tmpval2;
COMMON unsigned int ref_mv;            //Reference-voltage  in mV units (as read with ADC)
COMMON unsigned int ref_mv_offs;       //Reference-voltage  in mV units with eeprom offset for C
COMMON unsigned int adc_internal_reference;  //internal reference voltage of ADC in mV units
COMMON  unsigned int RHmultip;	// Multiplier for capacity measurement with R_H (470KOhm)

#ifdef WITH_MENU
COMMON union t_frq{
  unsigned long dw;
  uint16_t w[2];
  uint8_t b[4];
} ext_freq;	// external frequency
//COMMON unsigned long ext_period; 
COMMON unsigned int pinchange_count;
COMMON unsigned int pinchange_max;
#endif

COMMON struct cap_t {
  unsigned long cval;		// capacitor value 
  unsigned long cval_max;	//capacitor with maximum value
  union t_combi{
  unsigned long dw;	// capacity value without corrections
  uint16_t w[2];
  } cval_uncorrected;

#if FLASHEND > 0x1fff
  unsigned int esr;		// serial resistance of C in 0.01 Ohm
  unsigned int v_loss;		// voltage loss 0.1%
#endif

  uint8_t ca, cb;		//pins of capacitor
  int8_t cpre;			//Prefix for capacitor value  -12=p, -9=n, -6=µ, -3=m
  int8_t cpre_max;		//Prefix of the biggest capacitor
} cap;
  unsigned int cell_mv[3];	//remaining load voltages after discharge cycle

#ifndef INHIBIT_SLEEP_MODE
 /* with sleep mode we need a global ovcnt16 */
COMMON volatile uint16_t ovcnt16;
COMMON volatile uint8_t unfinished;
#endif

COMMON int16_t load_diff;		// difference voltage of loaded capacitor and internal reference

COMMON uint8_t WithReference;		// Marker for found precision voltage reference = 1
COMMON uint8_t PartFound;	 	// type of the found part 
COMMON uint8_t PartMode;		// description of the found part
COMMON char outval[10];			// String for ASCII-output i2lcd, u2lcd
COMMON char OutBuffer[10];		// String for ASCII-output DisplayValue
COMMON uint8_t empty_count;		// counter for max count of empty measurements
COMMON uint8_t mess_count;		// counter for max count of nonempty measurements

COMMON struct ADCconfig_t {
  uint8_t Samples;		// number of ADC samples to take
  uint8_t RefFlag;		// save Reference type VCC of IntRef
  uint16_t U_Bandgap;		// Reference Voltage in mV
  uint16_t U_AVCC;		// Voltage of AVCC
} ADCconfig;

#ifdef AUTO_CAL
COMMON uint8_t pin_combination;		// coded Pin-combination  2:1,3:1,1:2,x:x,3:2,1:3,2:3
COMMON uint16_t resis680pl;	// port output resistance + 680
COMMON uint16_t resis680mi;	// port output resistance + 680
COMMON uint16_t pin_rmi;	// port output resistance to GND side, 0.1 Ohm units
COMMON uint16_t pin_rpl;	// port output resistance to VCC side, 0.1 Ohm units
COMMON uint8_t UnCalibrated;	// 0, if the tester is calibrated
#endif

#ifdef WITH_ROTARY_SWITCH
 #define ROT_MSK 0x03		/* must be power of two - 1: 3,7,15 */
struct Rotary_t {
 uint8_t state[(ROT_MSK+1)];	// coded state history of the rotatry switch, bit 0 == state of A-switch, bit 1 = state of B-switch
 uint8_t ind;		// index to the last entry of the state history (rotary switch)
 int8_t count;		// count of right steps, negative if left steps
 uint8_t incre;		// absolute value of step count
 #if WITH_ROTARY_SWITCH == 4
 // no rotary switch connected, UP and DOWN  key is present
 uint8_t a_state;	// history of switch A state for single UP switch
 uint8_t b_state;	// history of switch B state for single DOWN switch
 #endif
};
COMMON struct Rotary_t rotary;
COMMON uint8_t rotary_switch_present;	// is set to 1, if rotary switch movement is detected
// COMMON  const uint8_t EE_RotarySwitch; 	// rotation switch is detected
#endif

COMMON uint8_t lcd_text_line;
COMMON uint8_t last_line_used;
//#if POWER_OFF+0 > 1
COMMON unsigned int display_time;	// display time of measurement in ms units
//#endif
#if (LCD_ST_TYPE == 7920)
COMMON uint8_t lcd_bit_mem[64][16];
#endif
