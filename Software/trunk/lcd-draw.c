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

static const struct tbitmap * const bmp_jfet[2] PROGMEM =
{
   &g_bmp_n_jfet,
   &g_bmp_p_jfet,
};

void lcd_draw_pin(unsigned char index, unsigned char x, unsigned char y)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}

void lcd_draw_trans_pins(unsigned char xe, unsigned char ye,
                         unsigned char xb, unsigned char yb,
                         unsigned char xc, unsigned char yc)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->e]);
   lcd_pgm_bitmap(pbmp, xe, ye, 0);
   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->b]);
   lcd_pgm_bitmap(pbmp, xb, yb, 0);
   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_number[_trans->c]);
   lcd_pgm_bitmap(pbmp, xc, yc, 0);
}

void lcd_draw_jfet(unsigned char index, unsigned char x, unsigned char y)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_jfet[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}

void lcd_draw_mosfet(unsigned char index, unsigned char x, unsigned char y)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_mosfet[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}

void lcd_draw_igbt(unsigned char index, unsigned char x, unsigned char y)
{
   const struct tbitmap *pbmp;

   pbmp = (const struct tbitmap * const)pgm_read_word(&bmp_igbt[index]);
   lcd_pgm_bitmap(pbmp, x, y, 0);
}
#endif

