#ifdef AUTO_CAL
// Check is direct included in the main source of the TransistorTester
// a function with a call from main will use additional 38 bytes of flash
  // define additional variables , ii is already defined in main
  unsigned int jj;
  char zeich;
  uint8_t space_pos;
  uint8_t line_nr;
  uint8_t ww;
  jj = 0;
  if (UnCalibrated) {
    // Output the help text for calibration.
    // The text is formatted for two 16 character display lines.
    zeich = ' ';		// initial value for while loop
    line_nr = 0;		// begin with the first LCD line
    while (zeich != (char)0) {	// zero is end of text
       space_pos = 16;		// if no space is found
       for (ii=0;ii<17;ii++) {	// look for the last space character
         zeich = pgm_read_byte(&HelpCalibration_str[ii+jj]);
         if (zeich == ' ')  space_pos = ii; // save the position
       }
       if (line_nr == 0) {
          // it is the first LCD line, wait for showing the last message
          for (ww=0;ww<50;ww++) {
             wait_about100ms();
             if(!(ON_PIN_REG & (1<<RST_PIN))) break;	// key pressed
          }
          lcd_clear();
          if (ww != 50) break;		// is interrupted, leave the loop
       }
       if (line_nr == 1) lcd_line2(); // write to the second LCD line
#ifdef FOUR_LINE_LCD
       if (line_nr == 2) lcd_line3(); // write to the third LCD line
       if (line_nr == 3) lcd_line4(); // write to the fourth LCD line
       line_nr = (line_nr + 1) & 3;
 #define TIME_TO_READ 100
#else
       line_nr = (line_nr + 1) & 1;
 #define TIME_TO_READ 50
#endif
       for (ii=0;ii<space_pos;ii++) {
         zeich = pgm_read_byte(&HelpCalibration_str[ii+jj]);
         if (zeich == (char)0) break;	// end of text found
         // the character can have a offset of 0x80 to hide the space character
         lcd_data(zeich&0x7f);		// display the character without offset
       }
       if (zeich == (char)0) break;	// end of text found
       jj += space_pos;		// start position of line 2
       if((pgm_read_byte(&HelpCalibration_str[jj])) == ' ') jj++; // no space at begin of line
    }  /* end while */
    for (ww=0;ww<TIME_TO_READ;ww++) {
       wait_about100ms();
       if(!(ON_PIN_REG & (1<<RST_PIN))) break;	// key pressed
    }
  }
#endif
