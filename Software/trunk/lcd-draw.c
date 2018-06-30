// file lcd-draw.c

#include "Transistortester.h"

#ifdef WITH_GRAPHICS
//#include "bitmaps.h"
extern const unsigned char bmp_one_data[] PROGMEM;
extern const unsigned char bmp_two_data[] PROGMEM;
extern const unsigned char bmp_three_data[] PROGMEM;
extern uint8_t icon_xx;
extern uint8_t icon_yy;
static const unsigned char * const bmp_number[3] PROGMEM =
{
   bmp_one_data,
   bmp_two_data,
   bmp_three_data
};


//*****************************************************************************
// draw the pin number index to the relative position of the last icon location

void lcd_draw_pin(unsigned char index, char dx, char dy)
{
   const unsigned char *pbmp;

   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[index]);
   lcd_pgm_bitmap(pbmp, icon_xx + dx, icon_yy + dy, 0);
}

//*****************************************************************************
// Draw the transistor pin numbers of transistor _trans to the last icon location
// only the position of the base pin is selectable
void lcd_draw_trans_pins( char dxb, char dyb)
{
//   const unsigned char *pbmp;

   lcd_draw_pin(_trans->e, 30, 24);	// relative to icon position
   lcd_draw_pin(_trans->b, dxb, dyb);	// relative to icon position
   lcd_draw_pin(_trans->c, 30, 0);	// relative to icon position
}

//*****************************************************************************
 #ifdef SHOW_ICONS
 // not enough Flash space for ShowIcons at ATmega328
// Show all Icons on the screen, up to four at one screen
void ShowIcons(void) {
#define ShowTime 15000  /* 15 seconds wait time, or key press, or rotary encoder movement */
 uint8_t cc;
 lcd_clear();
 lcd_big_icon(BJT_NPN|LCD_UPPER_LEFT);
 lcd_update_icon_opt(bmp_vakdiode, OPT_VREVERSE);
 lcd_big_icon(BJT_NPN|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_pnp);	// update for PNP
 lcd_set_cursor(5,0);
 lcd_MEM_string(NPN_str);
 lcd_set_cursor(5,(LCD_LINE_LENGTH / 2));
 lcd_MEM_string(PNP_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_JFET|LCD_LOWER_LEFT);
 lcd_big_icon(N_JFET|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_jfet);	// update to P_JFET
 lcd_set_cursor(2,1);
 lcd_data('N');		// N-Type
 lcd_data('-');
 lcd_MEM_string(jfet_str);
 lcd_set_cursor(2,1+(LCD_LINE_LENGTH / 2));
 lcd_data('P');		// P-Type
 lcd_data('-');
 lcd_MEM_string(jfet_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_IGBT|LCD_UPPER_LEFT);
 lcd_big_icon(N_E_IGBT|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_p_e_igbt);	// update to P-E-IGBT
 lcd_set_cursor(5,0);
 lcd_data('N');		// N-Type
 lcd_data('-');
 lcd_data('E');		// Enhancement Type
 lcd_MEM_string(igbt_str);
 lcd_set_cursor(6,(LCD_LINE_LENGTH / 2));
 lcd_data('P');		// P-Type
 lcd_data('-');
 lcd_data('E');		// Enhancement Type
 lcd_MEM_string(igbt_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_IGBT|LCD_LOWER_LEFT);
 lcd_update_icon(bmp_n_d_igbt);	// update to N-D-IGBT
 lcd_big_icon(N_E_IGBT|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_d_igbt);	// update to P-D-IGBT
 lcd_set_cursor(1,0);
 lcd_data('N');		// N-Type
 lcd_data('-');
 lcd_data('D');		// Depletion Type
 lcd_MEM_string(igbt_str);
 lcd_set_cursor(2,(LCD_LINE_LENGTH / 2));
 lcd_data('P');		// P-Type
 lcd_data('-');
 lcd_data('D');		// Depletion Type
 lcd_MEM_string(igbt_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_MOS|LCD_UPPER_LEFT);
 lcd_big_icon(N_E_MOS|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_p_e_mos);  // update to P-E-MOS
 lcd_set_cursor(5,0);
 lcd_data('N');		// N-Type
 lcd_data('-');
 lcd_data('E');		// Enhancement Type
 lcd_MEM_string(mosfet_str);
 lcd_set_cursor(6,(LCD_LINE_LENGTH / 2));
 lcd_data('P');		// P-Type
 lcd_data('-');
 lcd_data('E');		// Enhancement Type
 lcd_MEM_string(mosfet_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_MOS|LCD_LOWER_LEFT);
 lcd_update_icon(bmp_n_d_mos);  // update to N-D-MOS
 lcd_big_icon(N_E_MOS|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_d_mos);  // update to P-D-MOS
 lcd_set_cursor(1,1);
 lcd_data('N');		// N-Type
 lcd_data('-');
 lcd_data('D');		// Depletion Type
 lcd_MEM_string(mosfet_str);
 lcd_set_cursor(2,1+(LCD_LINE_LENGTH / 2));
 lcd_data('P');		// P-Type
 lcd_data('-');
 lcd_data('D');		// Depletion Type
 lcd_MEM_string(mosfet_str);
 wait_for_key_ms(ShowTime);
 #if (ICON_ELEMENTS == 14)
 lcd_clear();
 lcd_big_icon(TRIAC|LCD_UPPER_LEFT);
 lcd_big_icon(THYRISTOR|LCD_UPPER_RIGHT);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(INDUCTOR|LCD_LOWER_LEFT);
 lcd_big_icon(CAPACITOR|LCD_LOWER_RIGHT);
 lcd_big_icon(RESISTOR|LCD_UPPER_LEFT);
 lcd_big_icon(RESISTORS|LCD_UPPER_RIGHT);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(DIODE_C_A|LCD_UPPER_LEFT);
 lcd_big_icon(DIODES_C_A_C_A|LCD_UPPER_RIGHT);
 lcd_big_icon(DIODES_A_C_C_A|LCD_LOWER_LEFT);
 lcd_big_icon(DIODES_C_A_A_C|LCD_LOWER_RIGHT);
 wait_for_key_ms(ShowTime);
 #endif

 
 lcd_clear();
 lcd_line1();
 lcd_MEM_string(Resistor_str);    // -[=]-
 lcd_line2();
 lcd_MEM_string(Inductor_str);         // -ww-
 lcd_line3();
 lcd_MEM_string(CapZeich);          // capacitor sign
 lcd_line4();
 lcd_MEM_string(AnKat_str);       //"->|-"
 lcd_space();
 lcd_MEM_string(KatAn_str);       //"-|<-"


// show character set
 for (cc=0;cc<(0x7f-0x20);cc++) {
   if ((cc%LCD_LINE_LENGTH) == 0) {
     // begin new line
     if(((cc/LCD_LINE_LENGTH) % LCD_LINES) == 0) {
       wait_for_key_ms(ShowTime);
       lcd_clear();
     }
     lcd_set_cursor(((cc/LCD_LINE_LENGTH)%LCD_LINES)*PAGES_PER_LINE,0);
   }
  lcd_data(cc+0x20);
 } /* end for cc */
 wait_for_key_ms(ShowTime);

 lcd_clear();
  #ifdef LCD_CYRILLIC
 for (cc=0;cc<((Cyr_ja+1-Cyr_B)+(Cyr_schtsch+1-Cyr_D));cc++) {
   if ((cc%LCD_LINE_LENGTH) == 0) {
     // begin new line
     lcd_set_cursor(((cc/LCD_LINE_LENGTH)%LCD_LINES)*PAGES_PER_LINE,0);
   }
   if (cc <= (Cyr_ja-Cyr_B))
   {
     lcd_data(cc + Cyr_B);
   } else {
     lcd_data(cc + Cyr_D - (Cyr_ja + 1 - Cyr_B));
   }
 } /* end for cc */
 wait_for_key_ms(ShowTime);

 lcd_clear();
  #endif

}
 #endif  /* defined(SamplingADC) || (FLASHEND > 0x7fff)  */
#else
 #warning "no WITH_GRAPHICS" 
#endif  /* WITH_GRAPHICS */

