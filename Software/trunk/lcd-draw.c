#include "Transistortester.h"

#ifdef WITH_GRAPHICS
static const struct tbitmap * const bmp_number[3] PROGMEM =
{
   &g_bmp_one,
   &g_bmp_two,
   &g_bmp_three,
};

static const struct tbitmap * const bmp_mosfet[4] PROGMEM =
{
   &g_bmp_n_e_mos,
   &g_bmp_p_e_mos,
   &g_bmp_n_d_mos,
   &g_bmp_p_d_mos,
};

static const struct tbitmap * const bmp_igbt[4] PROGMEM =
{
   &g_bmp_n_e_igbt,
   &g_bmp_p_e_igbt,
   &g_bmp_n_d_igbt,
   &g_bmp_p_d_igbt,
};


void lcd_draw_pin(unsigned char index, unsigned char x, unsigned char y)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}

// only the position of the base pin is selectable
void lcd_draw_trans_pins( unsigned char xb, unsigned char yb)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->e]);
   lcd_pgm_bitmap(pbmp, 122, 56, 0);
   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->b]);
   lcd_pgm_bitmap(pbmp, xb, yb, 0);
   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->c]);
   lcd_pgm_bitmap(pbmp, 122, 32, 0);
}


void lcd_draw_mosfet(unsigned char index)
{
#define X_POS 90	/* left side x-Position of bitmap */ 
#define Y_POS 32	/* upper side y-position of the bitmap, must match to multiply of 8 */
   const struct tbitmap *pbmp;

   lcd_pgm_bitmap(&g_bmp_n_e_mos, X_POS, Y_POS, 0);
   if (index != 0) {
      // update the n_e_mos bitmap at relativ location 9,8
      pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_mosfet[index]);
      lcd_pgm_bitmap(pbmp, X_POS+9, Y_POS+8, 0);
   }
}

void lcd_draw_igbt(unsigned char index)
{
   const struct tbitmap *pbmp;
   lcd_pgm_bitmap(&g_bmp_n_e_igbt, X_POS, Y_POS, 0);

   if (index != 0) {
      // update the n_e_igbt bitmap at relativ location 12, 0
      pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_igbt[index]);
      lcd_pgm_bitmap(pbmp, X_POS+12, Y_POS, 0);
   }
}
#endif

