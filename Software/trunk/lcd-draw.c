#include "Transistortester.h"
#include "bitmaps.h"


#ifdef WITH_GRAPHICS
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

//   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->e]);
//   lcd_pgm_bitmap(pbmp, 121, 56, 0);
   lcd_draw_pin(_trans->e, 33, 24);	// relative to icon position
//   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->b]);
//   lcd_pgm_bitmap(pbmp, xb, yb, 0);
   lcd_draw_pin(_trans->b, dxb, dyb);	// relative to icon position
//   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->c]);
//   lcd_pgm_bitmap(pbmp, 121, 32, 0);
   lcd_draw_pin(_trans->c, 33, 0);	// relative to icon position
}

//*****************************************************************************
// Show all Icons on the screen, up to four at one screen
void ShowIcons(void) {
#define ShowTime 10000  /* 10 seconds wait time, or key press, or rotary encoder movement */
 lcd_clear();
 lcd_big_icon(BJT_NPN|LCD_UPPER_LEFT);
 lcd_update_icon_opt(bmp_vakdiode, OPT_VREVERSE);
 lcd_big_icon(BJT_NPN|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_pnp);	// update for PNP
 lcd_set_cursor(5,2);
 lcd_MEM_string(NPN_str);
 lcd_set_cursor(5,10);
 lcd_MEM_string(PNP_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_JFET|LCD_LOWER_LEFT);
 lcd_big_icon(N_JFET|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_jfet);	// update to P_JFET
 lcd_set_cursor(0,0);
 lcd_data('N');
 lcd_data('-');
 lcd_MEM_string(jfet_str);
 lcd_set_cursor(0,8);
 lcd_data('P');
 lcd_data('-');
 lcd_MEM_string(jfet_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_IGBT|LCD_UPPER_LEFT);
 lcd_big_icon(N_E_IGBT|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_p_e_igbt);	// update to P-E-IGBT
 lcd_set_cursor(5,0);
 lcd_data('N');
 lcd_data('-');
 lcd_data('E');
 lcd_MEM_string(igbt_str);
 lcd_set_cursor(6,8);
 lcd_data('P');
 lcd_data('-');
 lcd_data('E');
 lcd_MEM_string(igbt_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_IGBT|LCD_LOWER_LEFT);
 lcd_update_icon(bmp_n_d_igbt);	// update to N-D-IGBT
 lcd_big_icon(N_E_IGBT|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_d_igbt);	// update to P-D-IGBT
 lcd_set_cursor(0,0);
 lcd_data('N');
 lcd_data('-');
 lcd_data('D');
 lcd_MEM_string(igbt_str);
 lcd_set_cursor(1,8);
 lcd_data('P');
 lcd_data('-');
 lcd_data('D');
 lcd_MEM_string(igbt_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_MOS|LCD_UPPER_LEFT);
 lcd_big_icon(N_E_MOS|LCD_UPPER_RIGHT);
 lcd_update_icon(bmp_p_e_mos);  // update to P-E-MOS
 lcd_set_cursor(5,0);
 lcd_data('N');
 lcd_data('-');
 lcd_data('E');
 lcd_MEM_string(mosfet_str);
 lcd_set_cursor(6,8);
 lcd_data('P');
 lcd_data('-');
 lcd_data('E');
 lcd_MEM_string(mosfet_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(N_E_MOS|LCD_LOWER_LEFT);
 lcd_update_icon(bmp_n_d_mos);  // update to N-D-MOS
 lcd_big_icon(N_E_MOS|LCD_LOWER_RIGHT);
 lcd_update_icon(bmp_p_d_mos);  // update to P-D-MOS
 lcd_set_cursor(0,0);
 lcd_data('N');
 lcd_data('-');
 lcd_data('D');
 lcd_MEM_string(mosfet_str);
 lcd_set_cursor(1,8);
 lcd_data('P');
 lcd_data('-');
 lcd_data('D');
 lcd_MEM_string(mosfet_str);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(TRIAC|LCD_UPPER_LEFT);
 lcd_big_icon(THYRISTOR|LCD_UPPER_RIGHT);
 lcd_big_icon(INDUCTOR|LCD_LOWER_LEFT);
 lcd_big_icon(CAPACITOR|LCD_LOWER_RIGHT);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(DIODE_C_A|LCD_UPPER_LEFT);
 lcd_big_icon(DIODES_C_A_C_A|LCD_UPPER_RIGHT);
 lcd_big_icon(DIODES_A_C_C_A|LCD_LOWER_LEFT);
 lcd_big_icon(DIODES_C_A_A_C|LCD_LOWER_RIGHT);
 wait_for_key_ms(ShowTime);

 lcd_clear();
 lcd_big_icon(RESISTOR|LCD_UPPER_LEFT);
 lcd_big_icon(RESISTORS|LCD_UPPER_RIGHT);
 wait_for_key_ms(ShowTime);
}

#endif

