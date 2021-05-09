#if defined LCD_CYRILLIC
  #define Cyr_B         0x80
  #define Cyr_G         0x81
  #define Cyr_Jo        0x82
  #define Cyr_Zsch      0x83
  #define Cyr_Z         0x84
  #define Cyr_I         0x85
  #define Cyr_J         0x86
  #define Cyr_L         0x87
  #define Cyr_P         0x88
  #define Cyr_U         0x89
  #define Cyr_F         0x8a
  #define Cyr_Tsch      0x8b
  #define Cyr_Sch       0x8c
  #define Cyr_HH        0x8d
  #define Cyr_Y         0x8e
  #define Cyr_E         0x8f
  #define Cyr_Ju        0x90
  #define Cyr_Ja        0x91
  #define Cyr_b         0x92
  #define Cyr_v         0x93
  #define Cyr_g         0x94
  #define Cyr_jo        0x95
  #define Cyr_zsch      0x96
  #define Cyr_z         0x97
  #define Cyr_i         0x98
  #define Cyr_j         0x99
  #define Cyr_k         0x9a
  #define Cyr_l         0x9b
  #define Cyr_m         0x9c
  #define Cyr_n         0x9d
  #define Cyr_p         0x9e
  #define Cyr_t         0x9f

  #define Cyr_tsch      0xa0
  #define Cyr_sch       0xa1
  #define Cyr_hh        0xa2
  #define Cyr_y         0xa3
  #define Cyr_ww        0xa4
  #define Cyr_e         0xa5
  #define Cyr_ju        0xa6
  #define Cyr_ja        0xa7
  #define Cyr_D         0xa8
  #define Cyr_C         0xa9
  #define Cyr_Schtsch   0xaa
  #define Cyr_d         0xab
  #define Cyr_f         0xac
  #define Cyr_c         0xad
  #define Cyr_schtsch   0xae

  #define CHAR_COUNT 0xaf
#else
 #if defined LANG_FRANCAIS
  #define a_grave       0x80 /* e0 */
  #define a_circ        0x81 /* e2 */
  #define a_elig        0x82 /* e6 */
  #define c_cedil       0x83 /* e7 */
  #define e_acute       0x84 /* e9 */
  #define e_circ        0x85 /* ea */
  #define e_grave       0x86 /* e8 */
  #define e_uml         0x87 /* eb */
  #define i_circ        0x88 /* ee */
  #define i_uml         0x89 /* ef */
  #define o_circ        0x8a /* f4 */
  #define o_elig        0x8b /* bd */
  #define u_grave       0x8c /* f9 */
  #define u_circ        0x8d /* fb */
  #define u_uml         0x8e /* fc */
  #define y_uml         0x8f /* ff */
  #ifdef WITH_CAPITAL_SPECIALS
   #define A_grave       0x90 /* e0 */
   #define A_circ        0x91 /* e2 */
   #define A_elig        0x92 /* e6 */
   #define C_cedil       0x93 /* e7 */
   #define E_acute       0x94 /* e9 */
   #define E_circ        0x95 /* ea */
   #define E_grave       0x96 /* e8 */
   #define E_uml         0x97 /* eb */
   #define I_circ        0x98 /* ee */
   #define I_uml         0x99 /* ef */
   #define O_circ        0x9a /* f4 */
   #define O_elig        0x9b /* bd */
   #define U_grave       0x9c /* f9 */
   #define U_circ        0x9d /* fb */
   #define U_uml         0x9e /* fc */
   #define Y_uml         0x9f /* ff */
   #define CHAR_COUNT    0xa0
  #else
   #define A_grave   a_grave
   #define A_circ    a_circ
   #define A_elig    a_elig
   #define C_cedil   c_cedil
   #define E_acute   e_acute
   #define E_circ    e_circ
   #define E_grave   e_grave
   #define E_uml     e_uml
   #define I_circ    i_circ
   #define I_uml     i_uml
   #define O_circ    o_circ
   #define O_elig    o_elig
   #define U_grave   u_grave
   #define U_circ    u_circ
   #define U_uml     u_uml
   #define Y_uml     y_uml
   #define CHAR_COUNT    0x90
  #endif
 #elif defined LANG_CZECH
  #define a_acute       0x80 /* e1 */
  #define c_caron       0x81 /* e8 */
  #define d_caron       0x82 /* ef */
  #define e_acute       0x83 /* e9 */
  #define e_caron       0x84 /* ec */
  #define i_acute       0x85 /* ed */
  #define n_caron       0x86 /* f2 */
  #define o_acute       0x87 /* f3 */
  #define r_caron       0x88 /* f8 */
  #define s_caron       0x89 /* b9 */
  #define t_caron       0x8a /* bb */
  #define u_acute       0x8b /* fa */
  #define u_ring        0x8c /* f9 */
  #define y_acute       0x8d /* fd */
  #define z_caron       0x8e /* b9 */
  #ifdef WITH_CAPITAL_SPECIALS
   #define A_acute       0x8f /* c1 */
   #define C_caron       0x90 /* c8 */
   #define D_caron       0x91 /* cf */
   #define E_acute       0x92 /* c9 */
   #define E_caron       0x93 /* cc */
   #define I_acute       0x94 /* cd */
   #define N_caron       0x95 /* d2 */
   #define O_acute       0x96 /* d3 */
   #define R_caron       0x97 /* d8 */
   #define S_caron       0x98 /* a9 */
   #define T_caron       0x99 /* ab */
   #define U_acute       0x9a /* da */
   #define U_ring        0x9b /*    */
   #define Y_acute       0x9c /* dd */
   #define Z_caron       0x9d /* ae */
   #define CHAR_COUNT 0x9e
  #else
   #define A_acute       a_acute
   #define C_caron       c_caron
   #define D_caron       d_caron
   #define E_acute       e_acute
   #define E_caron       e_caron
   #define I_acute       i_acute
   #define N_caron       n_caron
   #define O_acute       o_acute
   #define R_caron       r_caron
   #define S_caron       s_caron
   #define T_caron       t_caron
   #define U_acute       u_acute
   #define U_ring        u_uml
   #define Y_acute       y_acute
   #define Z_caron       z_caron
   #define CHAR_COUNT 0x8f
  #endif
 #elif defined LANG_SLOVAK
  #define a_acute       0x80 /* e1 */
  #define a_uml         0x81 /* e4 */
  #define c_caron       0x82 /* e8 */
  #define d_caron       0x83 /* ef */
  #define e_acute       0x84 /* e9 */
  #define i_acute       0x85 /* ed */
  #define l_acute       0x86 /* e5 */
  #define l_caron       0x87 /* b5 */
  #define n_caron       0x88 /* f2 */
  #define o_acute       0x89 /* f3 */
  #define o_circ        0x8a /* f4 */
  #define r_acute       0x8b /* e0 */
  #define s_caron       0x8c /* b9 */
  #define t_caron       0x8d /* bb */
  #define u_acute       0x8e /* fa */
  #define y_acute       0x8f /* fd */
  #define z_caron       0x90 /* b9 */
  #ifdef WITH_CAPITAL_SPECIALS
   #define A_acute       0x91 /* c1 */
   #define A_uml         0x92 /* c4 */
   #define C_caron       0x93 /* c8 */
   #define D_caron       0x94 /* cf */
   #define E_acute       0x95 /* c9 */
   #define I_acute       0x96 /* cd */
   #define L_acute       0x97 /* c5 */
   #define L_caron       0x98 /* a5 */
   #define N_caron       0x99 /* d2 */
   #define O_acute       0x9a /* d3 */
   #define O_circ        0x9b /* d4 */
   #define R_acute       0x9c /* c0 */
   #define S_caron       0x9d /* a9 */
   #define T_caron       0x9e /* ab */
   #define U_acute       0x9f /* da */
   #define Y_acute       0xa0 /* dd */
   #define Z_caron       0xa1 /* ae */
   #define CHAR_COUNT 0xa2
  #else
   #define A_acute       a_acute
   #define A_uml         a_uml
   #define C_caron       c_caron
   #define D_caron       d_caron
   #define E_acute       e_acute
   #define I_acute       i_acute
   #define L_acute       l_acute
   #define L_caron       l_caron
   #define N_caron       n_caron
   #define O_acute       o_acute
   #define O_circ        o_circ
   #define R_acute       r_acute
   #define S_caron       s_caron
   #define T_caron       t_caron
   #define U_acute       u_acute
   #define Y_acute       y_acute
   #define Z_caron       z_caron
   #define CHAR_COUNT 0x91
  #endif
 #elif defined LANG_SERBIAN || LANG_CROATIAN || LANG_BOSNIAN
  #define c_acute	0x80 /* e6 */
  #define c_caron	0x81 /* e8 */
  #define d_eth		0x82 /* f0 */
  #define s_caron	0x83 /* b9 */
  #define z_caron	0x84 /* be */
  #define C_acute	0x85 /* c6 */
  #define C_caron	0x86 /* c8 */
  #define D_eth		0x87 /* d0 */
  #define S_caron	0x88 /* a9 */
  #define Z_caron	0x89 /* ae */
  #define CHAR_COUNT  0x8a
 #elif defined LANG_SLOWENIAN
  #define c_caron	0x80 /* e8 */
  #define s_caron	0x81 /* b9 */
  #define z_caron	0x82 /* be */
  #define C_caron	0x83 /* c8 */
  #define S_caron	0x84 /* a9 */
  #define Z_caron	0x85 /* ae */
  #define CHAR_COUNT  0x86
 #elif defined LANG_POLISH
  #define a_ogon        0x80 /* b1 */
  #define c_acute       0x81 /* e6 */
  #define e_ogon        0x82 /* ea */
  #define l_stroke      0x83 /* b3 */
  #define n_acute       0x84 /* f1 */
  #define o_acute       0x85 /* f3 */
  #define s_acute       0x86 /* b6 */
  #define z_acute       0x87 /* bc */
  #define z_dot         0x88 /* bf */
  #define A_ogon        0x89 /* a1 */
  #define C_acute       0x8a /* c6 */
  #define E_ogon        0x8b /* ca */
  #define L_stroke      0x8c /* a3 */
  #define N_acute       0x8d /* d1 */
  #define O_acute       0x8e /* d3 */
  #define S_acute       0x8f /* a6 */
  #define Z_acute       0x90 /* ac */
  #define Z_dot         0x91 /* ae */
 #define CHAR_COUNT  0x92
 #elif defined LANG_GERMAN
  #define a_uml      	0x80 /* e4 */
  #define o_uml      	0x81 /* f6 */
  #define u_uml      	0x82 /* fc */
  #define A_uml      	0x83 /* c4 */
  #define O_uml      	0x84 /* d6 */
  #define U_uml      	0x85 /* dc */
  #define s_sharp       0x86 /* df */
  #define CHAR_COUNT  0x87
 #elif defined LANG_ALBANIAN
  #define c_cedil       0x80 /* e7 */
  #define e_uml      	0x81 /* eb */
  #define C_cedil       0x82 /* c7 */
  #define E_uml      	0x83 /* d6 */
  #define CHAR_COUNT  0x84
 #elif defined LANG_ITALIAN
  #define a_grave       0x80 /* e0 */
  #define e_acute       0x81 /* e9 */
  #define e_grave       0x82 /* e8 */
  #define i_acute       0x83 /* ed */
  #define i_circ        0x84 /* ee */
  #define i_grave       0x85 /*    */
  #define o_acute       0x86 /* f3 */
  #define o_grave       0x87 /*    */
  #define u_acute       0x88 /* fa */
  #define u_grave       0x89 /* f9 */

  #define A_grave       0x8a /* c0 */
  #define E_acute       0x8b /* c9 */
  #define E_grave       0x8c /* c8 */
  #define I_acute       0x8d /* cd */
  #define I_circ        0x8e /* ce */
  #define I_grave       0x8f /*    */
  #define O_acute       0x90 /* d3 */
  #define O_grave       0x91 /*    */
  #define U_acute       0x92 /* da */
  #define U_grave       0x93 /* d9 */
  #define CHAR_COUNT 0x94
  #elif defined LANG_SPANISH
  #define a_acute       0x80
  #define c_cedil       0x81
  #define e_acute       0x82
  #define i_acute       0x83
  #define o_acute       0x84
  #define u_acute       0x85
  #define u_uml         0x86
  #define A_acute       0x87
  #define E_acute       0x88
  #define I_acute       0x89
  #define O_acute       0x8a
  #define U_acute       0x8b
  #define CHAR_COUNT 0x8c
  #define a_super       'a'
  #define o_super       'o'
  #define n_tilde       'n'
  #define N_tilde       'N'
  #define updown_exclam '!'
  #define updown_question '?'
  /* #define n_tilde       0x8e */
  /* #define N_tilde       0x8f */
  /* #define updown_exclam 0x90 */
  /* #define updown_question 0x91 */
  #elif defined LANG_HUNGARIAN
  #define a_acute       0x80 /* e1 */
  #define e_acute       0x81 /* e9 */
  #define i_acute       0x82 /* ed */
  #define o_acute       0x83 /* f3 */
  #define o_dblac       0x84 /* f5 */
  #define o_uml         0x85 /* f6 */
  #define u_acute       0x86 /* fa */
  #define u_dblac       0x87 /* fb */
  #define u_uml         0x88 /* fc */
  #define A_acute       0x89 /* c1 */
  #define E_acute       0x8a /* c9 */
  #define I_acute       0x8b /* cd */
  #define O_acute       0x8c /* d3 */
  #define O_dblac       0x8d /* d5 */
  #define O_uml         0x8e /* d6 */
  #define U_acute       0x8f /* da */
  #define U_dblac       0x90 /* db */
  #define U_uml         0x91 /* dc */
  #define CHAR_COUNT  0x92
  #elif defined LANG_ROMANIAN
  #define a_circ        0x80 /* e2 */
  #define a_breve       0x81 /* e3 */
  #define i_circ        0x82 /* ee */
  #define s_cedil       0x83 /* ba */
  #define t_cedil       0x84 /* fe */
  #define A_circ        0x85 /* c2 */
  #define A_breve       0x86 /* c3 */
  #define I_circ        0x87 /* ce */
  #define S_cedil       0x88 /* aa */
  #define T_cedil       0x89 /* de */
  #define CHAR_COUNT  0x8a
  #elif defined LANG_DANISH || defined LANG_NORWEGIAN || defined LANG_SWEDISH
  #define a_ring        0x80 /* e5 */
  #define a_elig        0x81 /* e6 */
  #define o_slash       0x82 /* f8 */
  #define A_ring        0x83 /* c5 */
  #define A_elig        0x84 /* c6 */
  #define O_slash       0x85 /* d8 */
  #define CHAR_COUNT  0x86
  #elif defined LANG_FINNISH
  #define a_ring        0x80 a_ring   /* c5 */
  #define a_uml         0x81 a_uml    /* e4 */
  #define o_uml         0x82 o_uml    /* f6 */
  #define s_caron       0x83 s_caron  /* b9 */
  #define z_caron       0x84 z_caron  /* b9 */
  #define A_ring        0x85 A_ring   /*    */
  #define A_uml         0x86 A_uml    /* c4 */
  #define O_uml         0x87 O_uml    /* d6 */
  #define S_caron       0x88 S_caron  /* a9 */
  #define Z_caron       0x89 Z_caron  /* ae */
  #define CHAR_COUNT  0x8a
 #else
  #define CHAR_COUNT 128
 #endif
#endif

#define LastChar (CHAR_COUNT - 1)
