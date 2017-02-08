#if defined(AUTO_CAL) && (FLASHEND > 0x3fff)
// Check is direct included in the main source of the TransistorTester
// a function with a call from main will use additional 38 bytes of flash
  // define additional variables , ii is already defined in main
 #define TIME_TO_READ 10000
  if (UnCalibrated) {
#ifndef SHORT_UNCAL_MSG
  unsigned int jj;
  char zeich;
  uint8_t space_pos;
  uint8_t line_nr;
  uint8_t sub_line;
    // Output the help text for calibration.
    // The text is formatted for two 16 character display lines.
    jj = 0;
    zeich = ' ';		// initial value for while loop
    line_nr = LCD_LINES;		// begin with the first LCD line, but don't wait
    while (zeich != (char)0) {	// zero is end of text
       space_pos = LCD_LINE_LENGTH;		// if no space is found
       for (ii=0;ii<(LCD_LINE_LENGTH+1);ii++) {	// look for the last space character
         zeich = pgm_read_byte(&HelpCalibration_str[ii+jj]);
         if (zeich == ' ')  space_pos = ii; // save the position
       }
       if (line_nr == 0) {
          // it is the first LCD line, wait for showing the last message
          if ((wait_for_key_ms(TIME_TO_READ)) != 0) break;	// key pressed
       }
       sub_line = line_nr % LCD_LINES;
       if (sub_line == 0) lcd_clear();  // clear display, line_nr is 0 or 4
       lcd_set_cursor(sub_line*PAGES_PER_LINE ,0);
       uart_newline();
       line_nr = (line_nr + 1) % LCD_LINES;
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
#else
    lcd_clear();
    lcd_pgm_string(HelpCalibration_str);	// only short message!
	#ifdef WITH_UART		//Mauro 
		uart_newline();		//Mauro 
	#endif					//Mauro 
#endif
    wait_for_key_ms(TIME_TO_READ);	// key pressed
  }
#endif  /* AUTO_CAL && (FLASHEND > 0x3fff) */
