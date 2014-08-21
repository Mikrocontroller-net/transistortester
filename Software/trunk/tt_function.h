void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin);
void ChargePin10ms(uint8_t PinToCharge, uint8_t ChargeDirection);
unsigned int ReadADC(uint8_t mux);		// read Routine for ADC
unsigned int W5msReadADC(uint8_t mux);		// wait 5ms and read than ADC
unsigned int W10msReadADC(uint8_t mux);		// wait 10ms and read than ADC
unsigned int W20msReadADC(uint8_t mux);		// wait 20ms and read then ADC
void lcd_show_format_cap(void);
void ReadCapacity(uint8_t HighPin, uint8_t LowPin);	//capacity measurement
void ReadInductance(void);		//inductance measurement
uint16_t GetESR(uint8_t hipin, uint8_t lopin);	//get ESR of capacitor
void GetVloss(void);			//get Voltage loss of capacitor
void GetIr(uint8_t HighPin, uint8_t LowPin);	// get residual current of diode
void SerienDiodenAusgabe();		// output diodes as ->|-->|- with voltages
void UfAusgabe(uint8_t bcdchar);	// Output of the threshold voltage(s) Uf
void mVAusgabe(uint8_t nn);		// Output of the theshold voltage for Diode nn 
void RvalOut(uint8_t nrr);		// Output of the resistor value(s)
void ShowResistor(void);		// show one or two Resistors
void EntladePins(void);			// discharge capacitors
void RefVoltage(void);			// compensate the reference voltage for comparator 
void AutoCheck(uint8_t test_mode);	// check if self-test should be done 
unsigned int GetRLmultip(unsigned int cvolt);  // get C-Multiplikator for voltage cvolt
void Scale_C_with_vcc(void);		// scale capacity value for different VCC Voltages
void scale_intref_adc(void);		// get scale factors for ReadADC with internal reference
//uint8_t value_out(unsigned long vval,uint8_t pp);    // output 4 digits with (pp-1) digits after point
void DisplayValue(unsigned long vval,int8_t Expo,unsigned char Unit, unsigned char Digits); //output Digits characters with exponent and unit
unsigned int compute_hfe(unsigned int lpx, unsigned int tpy);
void sleep_5ms(uint8_t xxx);		// set processor to sleep state for xxx times 5ms, xxx>200  (xxx-200)*100  
void PinLayout(char pin1, char pin2, char pin3); // show pin layout with EBC= or 123=
// Graphics display only: Some drawing functions for graphics LCD
#ifdef WITH_GRAPHICS
void lcd_draw_pin(unsigned char index, unsigned char x, unsigned char y);
void lcd_draw_trans_pins( unsigned char xb, unsigned char yb);
void lcd_draw_mosfet(unsigned char index);
void lcd_draw_igbt(unsigned char index) ;
#endif
void Calibrate_UR(void);		// get reference voltages and port resistance
uint16_t get_log(uint16_t permil);	// get -log(1-(permil/1000))
uint8_t ShortedProbes(uint8_t p1, uint8_t p2);	// check if two probes are shorted
uint8_t AllProbesShorted();		// check if all probes are shorted, return 3 if true
void mark_as_uncalibrated();		// mark in EEprom as uncalibrated device
uint8_t wait_for_key_ms(int max_time); // wait max_time ms units for key is pressed
void wait_for_key_5s_line2(void);	// wait 5 seconds and clear line 2
void i2lcd(int iw);			// output integer value to LCD
void u2lcd(uint16_t iw);		// output unsigned integer value to LCD
void EE_check_init(void);		// check calibration values and init the values if unset
void GetFrequency(uint8_t range);	// measure Frequency and display 
void function_menu();			// menu for selecting extra function
void show_vext();			// show external voltage (zener) 
void message_key_released(const unsigned char XX_str[]); // clear display, show message XX_str and wait for released key
void make_frequency(void);		// start counter 1 to make frequency at middle pin
void do_10bit_PWM(void);		// start counter 1 to make fixed frequency with PWM
void show_C_ESR();			// show big capacity values and ESR 
void ReadBigCap(uint8_t HighPin, uint8_t LowPin);	//capacity measurement only for big capacitors
void CheckRotaryEncoder(void);				// check rotary puls generator
void check_rotary(void);			// check state of rotary switch	for menu function
void message2line(uint8_t number);	// write menue message number to lcd line
void switch_frequency(uint8_t freq_num);	// switch frequency and write actual frequency to LCD
void CalibrationCap(void);		// read voltage offsets with calibration capacitor
void set_contrast(void);		// set the contrast value of ST7565 controller
