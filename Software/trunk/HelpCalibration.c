#ifdef AUTO_CAL
// Check is direct included in the main source of the TransistorTester
// a function with a call from main will use additional 38 bytes of flash
  // define additional variables , ii is already defined in main
  unsigned int jj;
  char zeich;
  uint8_t space_pos;
  uint8_t line_nr;
  jj = 0;
  if (UnCalibrated) {
    // Output the help text for calibration.
    // The text is formatted for two 16 character display lines.
    zeich = ' ';		// initial value for while loop
    line_nr = 4;		// begin with the first LCD line, but don't wait
    while (zeich != (char)0) {	// zero is end of text
       space_pos = 16;		// if no space is found
       for (ii=0;ii<17;ii++) {	// look for the last space character
         zeich = pgm_read_byte(&HelpCalibration_str[ii+jj]);
         if (zeich == ' ')  space_pos = ii; // save the position
       }
       if (line_nr == 0) {
          // it is the first LCD line, wait for showing the last message
          if ((wait_for_key_ms(5000)) != 0) break;	// key pressed
       }
       if ((line_nr & 3) == 0) lcd_clear();  // clear display, line_nr is 0 or 4
       if (line_nr == 1) lcd_line2(); // write to the second LCD line
#if (LCD_LINES > 3)
       if (line_nr == 2) lcd_line3(); // write to the third LCD line
       if (line_nr == 3) lcd_line4(); // write to the fourth LCD line
       line_nr = (line_nr + 1) & 3;
 #define TIME_TO_READ 10000
#else
       line_nr = (line_nr + 1) & 1;
 #define TIME_TO_READ 5000
#endif
       for (ii=0;ii<space_pos;ii++) {
         zeich = pgm_read_byte(&HelpCalibration_str[ii+jj]);
         if (zeich == (char)0) break;	// end of text found
         if (zeich == LCD_CHAR_INSEP) {
            lcd_space();	// replace with space
         } else {
            lcd_data(zeich);		// display the character without offset
         }
       }
       if (zeich == (char)0) break;	// end of text found
       jj += space_pos;		// start position of line 2
       if((pgm_read_byte(&HelpCalibration_str[jj])) == ' ') jj++; // no space at begin of line
    }  /* end while */
    wait_for_key_ms(TIME_TO_READ);	// key pressed
  }
#endif
