#include "Transistortester.h"


// show the Pin Layout of the device 
void PinLayout(char pin1, char pin2, char pin3) {
// pin1-3 is EBC or SGD or CGA
#ifndef EBC_STYLE
   // Layout with 123= style
   uint8_t ipp;
   lcd_fix_string(N123_str);		//" 123="
   for (ipp=0;ipp<3;ipp++) {
       if (ipp == _trans->e)  lcd_data(pin1);	// Output Character in right order
       if (ipp == _trans->b)  lcd_data(pin2);
       if (ipp == _trans->c)  lcd_data(pin3);
   }
#else
 #if EBC_STYLE == 321
   // Layout with 321= style
   uint8_t ipp;
   lcd_fix_string(N321_str);		//" 321="
   ipp = 3;
   while (ipp != 0) {
       ipp--;
       if (ipp == _trans->e)  lcd_data(pin1);	// Output Character in right order
       if (ipp == _trans->b)  lcd_data(pin2);
       if (ipp == _trans->c)  lcd_data(pin3);
   }
 #else 
   // Layout with EBC= style
   lcd_space();
   lcd_data(pin1);
   lcd_data(pin2);
   lcd_data(pin3);
   lcd_data('=');
   lcd_testpin(_trans->e);
   lcd_testpin(_trans->b);
   lcd_testpin(_trans->c);
 #endif
#endif
}
