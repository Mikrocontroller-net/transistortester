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
void AutoCheck(void);			// check if self-test should be done 
unsigned int getRLmultip(unsigned int cvolt);  // get C-Multiplikator for voltage cvolt
void Scale_C_with_vcc(void);		// scale capacity value for different VCC Voltages
void scale_intref_adc(void);		// get scale factors for ReadADC with internal reference
//uint8_t value_out(unsigned long vval,uint8_t pp);    // output 4 digits with (pp-1) digits after point
void DisplayValue(unsigned long vval,int8_t Expo,unsigned char Unit, unsigned char Digits); //output Digits characters with exponent and unit
unsigned int compute_hfe(unsigned int lpx, unsigned int tpy);
void sleep_5ms(uint16_t xxx);		// set processor to sleep state for xxx times 5ms
void PinLayout(char pin1, char pin2, char pin3); // show pin layout with EBC= or 123=
void Calibrate_UR(void);		// get reference voltages and port resistance
uint16_t get_log(uint16_t permil);	// get -log(1-(permil/1000))
uint8_t ShortedProbes(uint8_t p1, uint8_t p2);	// check if two probes are shorted
uint8_t AllProbesShorted();		// check if all probes are shorted, return 3 if true
