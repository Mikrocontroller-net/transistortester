#include "Transistortester.h"


#ifdef WITH_GRAPHICS
extern const unsigned char bmp_one_data[] PROGMEM;
extern const unsigned char bmp_two_data[] PROGMEM;
extern const unsigned char bmp_three_data[] PROGMEM;
static const unsigned char * const bmp_number[3] PROGMEM =
{
   bmp_one_data,
   bmp_two_data,
   bmp_three_data
};

extern const unsigned char bmp_n_e_mos_data[] PROGMEM;
extern const unsigned char bmp_p_e_mos_data[] PROGMEM;
extern const unsigned char bmp_n_d_mos_data[] PROGMEM;
extern const unsigned char bmp_p_d_mos_data[] PROGMEM;
static const unsigned char * const bmp_mosfet[4] PROGMEM =
{
   &bmp_n_e_mos_data[0],
   &bmp_p_e_mos_data[0],
   &bmp_n_d_mos_data[0],
   &bmp_p_d_mos_data[0]
};

extern const unsigned char bmp_n_e_igbt_data[] PROGMEM;
extern const unsigned char bmp_p_e_igbt_data[] PROGMEM;
extern const unsigned char bmp_n_d_igbt_data[] PROGMEM;
extern const unsigned char bmp_p_d_igbt_data[] PROGMEM;
static const unsigned char * const bmp_igbt[4] PROGMEM =
{
   &bmp_n_e_igbt_data[0],
   &bmp_p_e_igbt_data[0],
   &bmp_n_d_igbt_data[0],
   &bmp_p_d_igbt_data[0]
};


void lcd_draw_pin(unsigned char index, unsigned char x, unsigned char y)
{
   const unsigned char *pbmp;

   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}

// only the position of the base pin is selectable
void lcd_draw_trans_pins( unsigned char xb, unsigned char yb)
{
   const unsigned char *pbmp;

   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->e]);
   lcd_pgm_bitmap(pbmp, 120, 56, 0);
   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->b]);
   lcd_pgm_bitmap(pbmp, xb, yb, 0);
   pbmp = (const unsigned char *)pgm_read_word(&bmp_number[_trans->c]);
   lcd_pgm_bitmap(pbmp, 120, 32, 0);
}


void lcd_draw_mosfet(unsigned char index)
{
#define X_POS 88	/* left side x-Position of bitmap */ 
#define Y_POS 32	/* upper side y-position of the bitmap, must match to multiply of 8 */
   const unsigned char *pbmp;

   lcd_pgm_bitmap(bmp_n_e_mos_data, X_POS, Y_POS, 0);
   if (index != 0) {
      // update the n_e_mos bitmap at relativ location 9,8
      pbmp = (const unsigned char *)pgm_read_word(&bmp_mosfet[index]);
      lcd_pgm_bitmap(pbmp, X_POS+9, Y_POS+8, 0);
   }
}

void lcd_draw_igbt(unsigned char index)
{
   const unsigned char *pbmp;
   lcd_pgm_bitmap(bmp_n_e_igbt_data, X_POS, Y_POS, 0);

   if (index != 0) {
      // update the n_e_igbt bitmap at relativ location 12, 0
      pbmp = (const unsigned char *)pgm_read_word(&bmp_igbt[index]);
      lcd_pgm_bitmap(pbmp, X_POS+12, Y_POS, 0);
   }
}
#endif

