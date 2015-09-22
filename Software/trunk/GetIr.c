
/* Get residual current in reverse direction of a diode */ 



//=================================================================
void GetIr(uint8_t hipin, uint8_t lopin) {
  unsigned int u_res;	// reverse voltage at 470k
  unsigned int u_res_old;
  unsigned int ir_nano;
  uint8_t HiADC;

#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  HiADC = pgm_read_byte(&PinRLRHADCtab[hipin-TP1]+6);	// Table of ADC pins including | TXD_VAL
  ADC_PORT = HiADC;		 	// switch ADC port to high level
  ADC_DDR = HiADC | TXD_MSK;		// switch High Pin direct to VCC
  R_PORT = 0;				// switch R-Port to GND
  LoPinR_L = pgm_read_byte(&PinRHRLADCtab[lopin-TP1]);  //R_L mask for LowPin R_L load
#else
  uint8_t LoPinR_L;
  HiADC = pgm_read_byte(&PinRLRHADCtab[hipin-TP1]+3);	// Table of ADC pins including | TXD_VAL
  ADC_PORT = HiADC;		 	// switch ADC port to high level
  ADC_DDR = HiADC | TXD_MSK;		// switch High Pin direct to VCC
  R_PORT = 0;				// switch R-Port to GND
  LoPinR_L = pgm_read_byte(&PinRLRHADCtab[lopin-TP1]);  //R_L mask for LowPin R_L load
  // R_H Pin must always be one pin number higher
#endif
  R_DDR = LoPinR_L;		// switch R_L port for LowPin to output (GND)
  u_res = U_VCC;
  // first load the parallel capacity with 680 Ohm resistor, then measure current with 470k
  do {
     u_res_old = u_res;
     u_res = W20msReadADC(lopin);		// read voltage
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
     R_DDR = pgm_read_byte(&PinRHRLADCtab[lopin-TP1+3]);  //R_H mask for LowPin R_H load
#else
     R_DDR = LoPinR_L + LoPinR_L;		// switch R_H port for LowPin to output (GND)
#endif
  } while (u_res < u_res_old);
  if (u_res == 0) return;		// no Output, if no current in reverse direction
#if (LCD_LINES > 3)
 #define IR_DIGITS 3
  lcd_line4();				// use Line 4 for Ir output
  lcd_MEM_string(Ir_str);		// output text "Ir="
#else
 #define IR_DIGITS 2
  lcd_MEM_string(Ir_str);		// output text "  Ir="
#endif
#ifdef WITH_IRMICRO
  unsigned int ir_micro;
  if (u_res < 2500) {
#endif
     /* R_H_VAL has units of 10 Ohm, u_res has units of mV, ir_nano has units of nA */
     ir_nano = (unsigned long)(u_res * 100000UL) / R_H_VAL;
     DisplayValue16(ir_nano,-9,'A',2);	// output two digits of current with nA units
#ifdef WITH_IRMICRO
  } else {
     R_DDR = LoPinR_L;			// switch R_L port for LowPin to output (GND)
     u_res = W5msReadADC(lopin);	// read voltage
     ir_nano = 0xffff;			// set to max
     /* RR680MI has units of 0.1 Ohm, u_res has units of mV, ir_micro has units of uA */
     ir_micro = (unsigned long)(u_res * 10000UL) / RR680MI;
     DisplayValue16(ir_micro,-6,'A',IR_DIGITS);	// output 2 or 3 digits of current in uA units
  }
#endif
  ADC_DDR = TXD_MSK;			// switch off
  ADC_PORT = TXD_VAL;			// switch off
  R_DDR = 0;				// switch off current

  return ;
}
