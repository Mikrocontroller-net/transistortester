
#include <avr/io.h>

#include "Transistortester.h"


//******************************************************************

void ChargePin10ms(uint8_t PinToCharge, uint8_t ChargeDirection) {
   //Load the specified pin to the specified direction with 680 Ohm for 10ms.
   //Will be used by discharge of MOSFET Gates or to load big capacities.
   //Parameters:
   //PinToCharge: specifies the pin as mask for R-Port
   //ChargeDirection: 0 = switch to GND (N-Kanal-FET), 1= switch to VCC(P-Kanal-FET)

   if(ChargeDirection&1) {
      R_PORT |= PinToCharge;	//R_PORT to 1 (VCC) 
   } else {
      R_PORT &= ~PinToCharge; // or 0 (GND)
   }
   R_DDR |= PinToCharge;		//switch Pin to output, across R to GND or VCC
   wait_about10ms();			// wait about 10ms
   // switch back Input, no current
   R_DDR &= ~PinToCharge;	// switch back to input
   R_PORT &= ~PinToCharge;	// no Pull up
}

