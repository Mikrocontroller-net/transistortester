#include "Transistortester.h"


// show the Pin Layout of the device 
void PinLayout(char pin1, char pin2, char pin3) {
// pin1-3 is EBC or SGD or CGA
#ifndef EBC_STYLE
   // Layout with 123= style
   uint8_t ipp;
   lcd_MEM_string(N123_str);		//" 123="
   for (ipp=0;ipp<3;ipp++) {
       if (ipp == _trans->e)  lcd_data(pin1);	// Output Character in right order
       if (ipp == _trans->b)  lcd_data(pin2);
       if (ipp == _trans->c)  lcd_data(pin3);
   }
#else	/* EBC_STYLE is defined */
 #if EBC_STYLE == 321
   // Layout with 321= style
   uint8_t ipp;
   lcd_MEM_string(N321_str);		//" 321="
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
   lcd_equal();		// lcd_data('=');
   lcd_testpin(_trans->e);
   lcd_testpin(_trans->b);
   lcd_testpin(_trans->c);
 #endif
#endif
}

#ifdef WITH_GRAPHICS
void PinLayoutLine(char pin1, char pin2, char pin3) {
   lcd_next_line_wait(0);
 #ifdef NO_LONG_PINLAYOUT
   lcd_space();
   lcd_MEM_string(Pin_str);		//"Pin "
   PinLayout(pin1, pin2, pin3);		// short form of Layout
 #else
   lcd_MEM_string(Pin_str);		//"Pin "
// pin1-3 is EBC or SGD or CGA
  #ifndef EBC_STYLE
   // Layout with 1=  2=  3=  style
   uint8_t ipp;
   for (ipp=0;ipp<3;ipp++) {
      lcd_testpin(ipp);
      lcd_equal();		// lcd_data('=');
      if (ipp == _trans->e)  lcd_data(pin1);	// Output Character in right order
      if (ipp == _trans->b)  lcd_data(pin2);
      if (ipp == _trans->c)  lcd_data(pin3);
      lcd_space();
   }
  #else	/* EBC_STYLE is defined */
   #if EBC_STYLE == 321
   // Layout with 3=  2=  1=  style
   uint8_t ipp;
   ipp = 3;
   while (ipp != 0) {
       ipp--;
       lcd_testpin(ipp);
       lcd_equal();		// lcd_data('=');
       if (ipp == _trans->e)  lcd_data(pin1);	// Output Character in right order
       if (ipp == _trans->b)  lcd_data(pin2);
       if (ipp == _trans->c)  lcd_data(pin3);
       lcd_space();
   }
   #else 
   // Layout with E=  B=  C=  style
   lcd_data(pin1);
   lcd_equal();		// lcd_data('=');
   lcd_testpin(_trans->e);
   lcd_space();
   lcd_data(pin2);
   lcd_equal();		// lcd_data('=');
   lcd_testpin(_trans->b);
   lcd_space();
   lcd_data(pin3);
   lcd_equal();		// lcd_data('=');
   lcd_testpin(_trans->c);
   #endif
  #endif
 #endif  /* NO_LONG_PINLAYOUT */
}
#endif  /* WITH_GRAPHICS */

/* ********************************************************** */
/* Rnum2pins computes two pin numbers for resistor number num */
uint16_t Rnum2pins(uint8_t num) {
uint8_t x,y;

  x = TP1;
  y = TP3;
  if (num == 0) y = TP2;
  if (num == 2) x = TP2;
  return ((uint16_t)(y<<8) | x);
}
