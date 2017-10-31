#include "Transistortester.h"
#ifdef WITH_UJT
void CheckUJT(void) {
 #ifdef SamplingADC
             uint8_t B1pin, B2pin, Epin;
             Epin = diodes.Anode[0];
             B1pin = diodes.Cathode[0];
             B2pin = diodes.Cathode[1];
ujt_exch_b:;
             uint8_t port0, port1, ddr0, ddr1;
             // B1 to ground via 0 ohm
             // B2 to plus via 680 ohm
             // discharge: E to ground via 680 ohm
             // charge: E to plus via 470 k

             port0 = pinmaskRL(B2pin);
             ddr0 = port0 | pinmaskRL(Epin);
             port0 |= pinmaskRH(Epin);
             port1 = port0;
             ddr1 = port1;
              ADMUX = Epin|(1<<REFS0);	// switch Multiplexer to emitter and use 5V reference voltage
              ADC_DDR = pinmaskADC(B1pin);
              ADC_PORT = TXD_VAL;
              R_PORT = 0;
              R_DDR = 0;			
              wait_aref_stabilize();
              unsigned int uu[64];
              samplingADC((16<<smplADC_span), uu, 64, port1, ddr1, port0, ddr0);
//              myuart_putc('s'); myuart_putc(' '); uart_int(port1); uart_int(ddr1); uart_int(port0); uart_int(ddr0); uart_newline();
              uint8_t i;
              uint8_t max=0;
              uint8_t maxm;
              for (i=10;i<64;i++) {
                 uint8_t v=uu[i]>>2;  // 8-bit arithmetic is enough for this
                 if (v>max) max=v;
                 else break;
              }
              // found first maximum
              // now check whether after this, the voltage drops significantly, if so, we've detected oscillation
              maxm = max - (max>>3);
              for (;i<64;i++) {
                 uint8_t v=uu[i]>>2;  // 8-bit arithmetic is enough for this
                 if (v<maxm) {
                    // significant voltage drop, oscillation found: must be an UJT
                    PartFound=PART_UJT;
                    ntrans.e=B1pin;
                    ntrans.b=Epin;
                    ntrans.c=B2pin;
                    // calculate eta
                    ResistorChecked[ntrans.e - TP_MIN + ntrans.c - TP_MIN - 1] = 0;	// forget last resistance measurement
                    GetResistance(ntrans.c, ntrans.e);	// resistor value is in ResistorVal[resnum]
                    uint32_t v;
                    uint32_t r=ResistorVal[ntrans.e - TP_MIN + ntrans.c - TP_MIN - 1];
                    v=(r<<8)/(r+R_L_VAL);      // Vbb in units of supplyvoltage/256 ; perhaps should also take into account the about 20 ohm internal resistance?
                    max-=35;               // subtract about 0.7 volts (1 diode voltage drop) from the maximum emitter voltage seen; 35 is 0.7/5*256.
                    ntrans.gthvoltage=100ul*max/v;     // this is eta in percent, cf. http://www.allaboutcircuits.com/textbook/semiconductors/chpt-7/unijunction-transistor-ujt/
                    goto ujtdone;
                 }
              } /* end for i */
              // didn't find oscillation yet
              if (B1pin == diodes.Cathode[0]) {
                 // try exchanging the two base connections
                 B1pin=B2pin;
                 B2pin=diodes.Cathode[0];
                 goto ujt_exch_b;
              }
ujtdone:;

//   uart_newline(); for (i=0;i<64;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i<<2]); uart_newline(); wdt_reset(); }
//   uart_newline(); for (i=0;i<64;i++) { myuart_putc('a'); myuart_putc(' '); uart_int(uu[i]); uart_newline(); wdt_reset(); }
//              myuart_putc('v'); myuart_putc(' '); uart_int(min); uart_int(max); uart_newline();
 #else		/* ! SamplingADC */
             uint16_t v0,v1,v2;
             ntrans.b = diodes.Anode[0];			// common anode must be emitter of UJT
             ADC_DDR = pinmaskADC(diodes.Cathode[0]);
             ADC_PORT =  TXD_VAL;   // UJT B1 to GND
             R_PORT = pinmaskRL(ntrans.b) | pinmaskRL(diodes.Cathode[1]);	// UJT emitter and B2 with RL to VCC
             R_DDR = R_PORT;
             v1 = vcc_diff(W5msReadADC(diodes.Cathode[1]));			// read voltage at B2
             R_PORT = pinmaskRL(diodes.Cathode[1]);	// UJT B2 with RL to VCC, emitter open
             R_DDR = R_PORT;
             v0 = vcc_diff(W5msReadADC(diodes.Cathode[1]));			// read voltage at B2
             if (v0 < v1) v1 -= v0;

             R_PORT = pinmaskRL(ntrans.b) | pinmaskRL(diodes.Cathode[0]);	// UJT emitter and B2 with RL to VCC
             R_DDR = R_PORT;
             ADC_DDR = pinmaskADC(diodes.Cathode[1]);
             v2 = vcc_diff(W5msReadADC(diodes.Cathode[0]));			// read voltage at B2
             R_PORT = pinmaskRL(diodes.Cathode[0]);	// UJT B2 with RL to VCC, emitter open
             R_DDR = R_PORT;
             v0 = vcc_diff(W5msReadADC(diodes.Cathode[0]));			// read voltage at B2
             if (v0 < v2) v2 -= v0;
             if (v1 > v2) {
                ntrans.uBE = v1;
                ntrans.e = diodes.Cathode[0];
                ntrans.c = diodes.Cathode[1];
             } else {
                ntrans.uBE = v2;
                ntrans.e = diodes.Cathode[1];
                ntrans.c = diodes.Cathode[0];
             }
             if (ntrans.uBE > 1500) {
                ResistorChecked[ntrans.e - TP_MIN + ntrans.c - TP_MIN - 1] = 0;	// forget last resistance measurement
                GetResistance(ntrans.c, ntrans.e);	// resistor value is in ResistorVal[resnum]
                PartFound = PART_UJT;
             }
           
 #endif		/* defined SamplingADC */
}
#endif
