#include "wait1000ms.h"
#include "lcd-routines.h"

void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin);
void GetResistance(uint8_t HighPin, uint8_t LowPin);
void ChargePin10ms(uint8_t PinToCharge, uint8_t ChargeDirection);
unsigned int ReadADC(uint8_t mux);		// read Routine for ADC
unsigned int W5msReadADC(uint8_t mux);		// wait 5ms and read than ADC
unsigned int W10msReadADC(uint8_t mux);		// wait 10ms and read than ADC
unsigned int W20msReadADC(uint8_t mux);		// wait 20ms and read then ADC
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
void DisplayValue(signed long vval,int8_t Expo,unsigned char Unit, uint8_t Digits); //output Digits characters with exponent and unit
void DisplayValue16(uint16_t vval,int8_t Expo,unsigned char Unit, uint8_t Digits); //output Digits characters with exponent and unit
void Display_mV(uint16_t val, uint8_t dd);		// show mV with dd digits
void Display_Hz(unsigned long val, uint8_t dd);		// show Hz with dd digits
unsigned int compute_hfe(unsigned int lpx, unsigned int tpy);
void sleep_5ms(uint8_t xxx);		// set processor to sleep state for xxx times 5ms, xxx>200  (xxx-200)*100  
void PinLayout(char pin1, char pin2, char pin3); // show pin layout with EBC= or 123=
void PinLayoutLine(char pin1, char pin2, char pin3); // show pin layout with Pin 1=...
void Calibrate_UR(void);		// get reference voltages and port resistance
uint16_t get_log(uint16_t permil);	// get -1000*log(1-(permil/1000))
uint8_t ShortedProbes(uint8_t p1, uint8_t p2);	// check if two probes are shorted
uint8_t AllProbesShorted();		// check if all probes are shorted, return 3 if true
void mark_as_uncalibrated();		// mark in EEprom as uncalibrated device
uint8_t wait_for_key_ms(int max_time); // wait max_time ms units for key is pressed
void wait_for_key_5s_line2(void);	// wait 5 seconds and clear line 2
void i2lcd(int iw);			// output integer value to LCD
void u2lcd(uint16_t iw);		// output unsigned integer value to LCD
void i2lcd_space(int iw);		// output integer value to LCD and ' '
void u2lcd_space(uint16_t iw);		// output unsigned integer value to LCD and ' '
void EE_check_init(void);		// check calibration values and init the values if unset
void GetFrequency(uint8_t range);	// measure Frequency and display 
uint8_t function_menu();			// menu for selecting extra function
void show_vext();			// show external voltage (zener) 
void set_big_cap_corr();		// set the correction value for big capacitor measurement
void message_key_released(const unsigned char XX_str[]); // clear display, show message XX_str and wait for released key
void make_frequency(void);		// start counter 1 to make frequency at middle pin
void do_10bit_PWM(void);		// start counter 1 to make fixed frequency with PWM
void show_C_ESR();			// show big capacity values and ESR 
void show_Resis13();			// show Resistor value at TP1:TP3 , measuring continuously
void show_Cap13(void);		        // show Capacitor value at TP1:TP3, measuring continuously
void show_resis(uint8_t pin1, uint8_t pin2, uint8_t how);   // show resistor (and inductance) between pin1 and pin2
void show_cap(uint8_t how);		// show capacity and measure additional values
void show_cap_simple(void);		// show capacity and measure additional values without [C]
void ReadBigCap(uint8_t HighPin, uint8_t LowPin);	//capacity measurement only for big capacitors
void CheckRotaryEncoder(void);				// check rotary puls generator
void check_rotary(void);			// check state of rotary switch	for menu function
void message2line(uint8_t number);	// write menue message number to lcd line
void switch_frequency(uint8_t freq_num);	// switch frequency and write actual frequency to LCD
void CalibrationCap(void);		// read voltage offsets with calibration capacitor
void set_contrast(void);		// set the contrast value of ST7565 controller
void ShowData(void);			// show software revision and calibration parameters
void Battery_check(void);		// check and report the battery state
uint8_t Pwr_mode_check(uint8_t ll);	// give next counter state depending on DC_Pwr_mode
void Bat_update(uint8_t tt);		// Update Battery voltage
void init_parts(void);			// initialize all parts to nothing found
void i2c_init(void);			// init the I2C interface
uint16_t Rnum2pins(uint8_t num);	// compute Pin-Numbers of the resistor number
void myuart_putc(uint8_t bb);
void uart_int(uint16_t num);
void CheckUJT(void);		// measure UJT
void select_color(uint8_t xc);		// select color function 
void switch_tester_off(void);		// switch power off

#ifdef SamplingADC
uint16_t samplingADC(   // code is in sampling_ADC.S
   uint16_t what, 	// what to measure? see smplADC_... defs in config.h, R24:R25
   void *ptr,           // output pointer (note: ptr[0] will contain incorrect data; ptr[1]...ptr[n-1] will be valid) R22:R23
   uint8_t n,           // number of samples (n=0 for 256), R20
   uint8_t Rport_1,	
   uint8_t Rddr_1,	
   uint8_t Rport_0,	
   uint8_t Rddr_0      
   );
// ptr can point to an array of either 8 or 16 bit data (with or without smplADC_8bit, respectively)
// and either signed or unsigned (with or without smplADC_hpf, respectively)
//
// you can find the defines for bits in "what" in config.h
//
// The meaning of Rport_1, Rddr_1, Rport_0 and Rddr_0 are now as follows.
// The measurement cycle consist of three phases, henceforth called idle, pulse, and active, in that order.
// Measurements are performed duing 'active' period.
// Rport_0 and Rddr_0 apply during idle.
// Rport_0 and Rddr_1 apply during active.
// If the samplingADC_direct option is NOT used, during pulse Rddr_0 and Rport_1 apply (duration of pulse is between 1 and 16 CPU cycles, controlled by lower bits of 'what' parameter).
//  (note though that currently the non-direct mode is only used for step response measurements, with Rport_1=Port_0, so effectively no pulse is applied)
// If the samplingADC_direct option is used, a 1 CPU-cycle pulse is applied via de ADC pin corresponding to a 1 bit in Rddr_0.




int32_t sampling_cap(uint8_t HighPin, uint8_t LowPin, uint8_t opts);   
// returns measured capacitance in 0.01 pF units
// opts&1 is flag demands measurement at 5 V rather than at 0 V
// opts&2 is flag to not subtract the parasitic capacitance
void sampling_lc(uint8_t LowPin, uint8_t HighPin);
#define sampling_cap_pre -14
void sampling_cap_calibrate(void);	// calibrate the sampling cap method
void sampling_lc_calibrate(void);	// calibrate the sampling  LC method

#ifdef WITH_XTAL
void sampling_test_xtal(void);          // test presence of resonator or crystal between pin 1 and 3 (hardcoded)
uint8_t sampling_measure_xtal(void);    // measure resonator or crystal between pin 1 and 3 (hardcoded),
                                        //   uses data already gathered in previous sampling_test_xtal() call
                                        //   prints output to LCD too
                                        //   returns nonzero if user interrupts this (time-consuming) measurement

// version to be called when using samlingADC_freq option, to pass the desired frequency:
uint16_t samplingADC_freqgen(uint16_t, void*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t freq);
// version to be called when using samlingADC_sck option, to also pass the desired short circuit duration:
uint16_t samplingADC_freqgen_sck(uint16_t, void*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t freq, uint8_t shortcircuitduration);
#endif
#endif
