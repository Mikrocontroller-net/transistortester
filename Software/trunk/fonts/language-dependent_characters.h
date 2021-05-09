#ifndef LCD_CYRILLIC
 #if defined LANG_FRANCAIS
/* ---F---- ISO8859-15---------------- */
      PIX_a_grave            /* 0x80 a_grave  e0 */
      PIX_a_circ             /* 0x81 a_circ   e2 */
      PIX_a_elig             /* 0x82 a_elig   e6 */
      PIX_c_cedil            /* 0x83 c_cedil  e7 */
      PIX_e_acute            /* 0x84 e_acute  e9 */
      PIX_e_circ             /* 0x85 e_circ   ea */
      PIX_e_grave            /* 0x86 e_grave  e8 */
      PIX_e_uml              /* 0x87 e_uml    eb */
      PIX_i_circ             /* 0x88 i_circ   ee */
      PIX_i_uml              /* 0x89 i_uml    ef */
      PIX_o_circ             /* 0x8a o_circ   f4 */
      PIX_o_elig             /* 0x8b o_elig   bd */
      PIX_u_grave            /* 0x8c u_grave  f9 */
      PIX_u_circ             /* 0x8d u_circ   fb */
      PIX_u_uml              /* 0x8e u_uml    fc */
      PIX_y_uml              /* 0x8f y_uml    ff */
  #ifdef WITH_CAPITAL_SPECIALS
      PIX_A_grave            /* 0x90 A_grave  c0 */
      PIX_A_circ             /* 0x91 A_circ   c2 */
      PIX_A_elig             /* 0x92 A_elig   c6 */
      PIX_C_cedil            /* 0x93 C_cedil  c7 */
      PIX_E_acute            /* 0x94 E_acute  c9 */
      PIX_E_circ             /* 0x95 E_circ   ca */
      PIX_E_grave            /* 0x96 E_grave  c8 */
      PIX_E_uml              /* 0x97 E_uml    cb */
      PIX_I_circ             /* 0x98 I_circ   ce */
      PIX_I_uml              /* 0x99 I_uml    cf */
      PIX_O_circ             /* 0x9a O_circ   d4 */
      PIX_O_elig             /* 0x9b O_elig   bc */
      PIX_U_grave            /* 0x9c U_grave  d9 */
      PIX_U_circ             /* 0x9d U_circ   db */
      PIX_U_uml              /* 0x9e U_uml    dc */
      PIX_Y_uml              /* 0x9f Y_uml    be */
  #endif
 #elif defined LANG_CZECH 
/* ---CZ------ ISO8859-2----------- */
      PIX_a_acute            /* 0x80 a_acute  e1 */
      PIX_c_caron            /* 0x81 c_caron  e8 */
      PIX_d_caron            /* 0x82 d_caron  ef */
      PIX_e_acute            /* 0x83 e_acute  e9 */
      PIX_e_caron            /* 0x84 e_caron  ec */
      PIX_i_acute            /* 0x85 i_acute  ed */
      PIX_n_caron            /* 0x86 n_caron  f2 */
      PIX_o_acute            /* 0x87 o_acute  f3 */
      PIX_r_caron            /* 0x88 r_caron  f8 */
      PIX_s_caron            /* 0x89 s_caron  b9 */
      PIX_t_caron            /* 0x8a t_caron  bb */
      PIX_u_acute            /* 0x8b u_acute  fa */
      PIX_u_ring             /* 0x8c u_ring   f9 */
      PIX_y_acute            /* 0x8d y_acute  fd */
      PIX_z_caron            /* 0x8e z_caron  b9 */
  #ifdef WITH_CAPITAL_SPECIALS
      PIX_A_acute            /* 0x8f A_acute  c1 */
      PIX_C_caron            /* 0x90 C_caron  c8 */
      PIX_D_caron            /* 0x91 D_caron  cf */
      PIX_E_acute            /* 0x92 E_acute  c9 */
      PIX_E_caron            /* 0x93 E_caron  cc */
      PIX_I_acute            /* 0x94 I_acute  cd */
      PIX_N_caron            /* 0x95 N_caron  d2 */
      PIX_O_acute            /* 0x96 O_acute  d3 */
      PIX_R_caron            /* 0x97 R_caron  d8 */
      PIX_S_caron            /* 0x98 S_caron  a9 */
      PIX_T_caron            /* 0x99 T_caron  ab */
      PIX_U_acute            /* 0x9a U_acute  da */
      PIX_U_ring             /* 0x9b U_ring      */
      PIX_Y_acute            /* 0x9c Y_acute  dd */
      PIX_Z_caron            /* 0x9d Z_caron  ae */
  #endif
 #elif defined LANG_SLOVAK
/* ---SL----- ISO8859-2----------- */
      PIX_a_acute            /* 0x80 a_acute  e1 */
      PIX_a_uml              /* 0x81 a_uml    e4 */
      PIX_c_caron            /* 0x82 c_caron  e8 */
      PIX_d_caron            /* 0x83 d_caron  ef */
      PIX_e_acute            /* 0x84 e_acute  e9 */
      PIX_i_acute            /* 0x85 i_acute  ed */
      PIX_l_acute            /* 0x86 l_acute  e5 */
      PIX_l_caron            /* 0x87 l_caron  b5 */
      PIX_n_caron            /* 0x88 n_caron  f2 */
      PIX_o_acute            /* 0x89 o_acute  f3 */
      PIX_o_circ             /* 0x8a o_circ   f4 */
      PIX_r_acute            /* 0x8b r_acute  e0 */
      PIX_s_caron            /* 0x8c s_caron  b9 */
      PIX_t_caron            /* 0x8d t_caron  bb */
      PIX_u_acute            /* 0x8e u_acute  fa */
      PIX_y_acute            /* 0x8f y_acute  fd */
      PIX_z_caron            /* 0x90 z_caron  b9 */
  #ifdef WITH_CAPITAL_SPECIALS
      PIX_A_acute            /* 0x91 A_acute  c1 */
      PIX_A_uml              /* 0x92 A_uml    c4 */
      PIX_C_caron            /* 0x93 C_caron  c8 */
      PIX_D_caron            /* 0x94 D_caron  cf */
      PIX_E_acute            /* 0x95 E_acute  c9 */
      PIX_I_acute            /* 0x96 I_acute  cd */
      PIX_L_acute            /* 0x97 L_acute  c5 */
      PIX_L_caron            /* 0x98 L_caron  a5 */
      PIX_N_caron            /* 0x99 N_caron  d2 */
      PIX_O_acute            /* 0x9a O_acute  d3 */
      PIX_O_circ             /* 0x9b O_circ   d4 */
      PIX_R_acute            /* 0x9c R_acute  c0 */
      PIX_S_caron            /* 0x9d S_caron  a9 */
      PIX_T_caron            /* 0x9e T_caron  ab */
      PIX_U_acute            /* 0x9f U_acute  da */
      PIX_Y_acute            /* 0xa0 Y_acute  dd */
      PIX_Z_caron            /* 0xa1 Z_caron  ae */
  #endif
 
 
 #elif defined LANG_SERBIAN || LANG_CROATIAN || LANG_BOSNIAN
/* -SRB-HR--BIH--ISO8859-2-------- */
      PIX_c_acute            /* 0x80 c_acute  e6 */
      PIX_c_caron            /* 0x81 c_caron  e8 */
      PIX_d_eth              /* 0x82 d_eth    f0 */
      PIX_s_caron            /* 0x83 s_caron  b9 */
      PIX_z_caron            /* 0x84 z_caron  b9 */
      PIX_C_acute            /* 0x85 C_acute  c6 */
      PIX_C_caron            /* 0x86 C_caron  c8 */
      PIX_D_eth              /* 0x87 D_eth    d0 */
      PIX_S_caron            /* 0x88 S_caron  a9 */
      PIX_Z_caron            /* 0x89 Z_caron  ae */
 #elif defined LANG_SLOWENIAN
/* ---SLO---ISO8859-2-------- */
      PIX_c_caron            /* 0x80 c_caron  e8 */
      PIX_s_caron            /* 0x81 s_caron  b9 */
      PIX_z_caron            /* 0x82 z_caron  b9 */
      PIX_C_caron            /* 0x83 C_caron  c8 */
      PIX_S_caron            /* 0x84 S_caron  a9 */
      PIX_Z_caron            /* 0x85 Z_caron  ae */
 #elif defined LANG_POLISH
/* ---PL---  ISO8859-2  ------------- */
      PIX_a_ogon             /* 0x80 a_ogon   b1 */
      PIX_c_acute            /* 0x81 c_acute  e6 */
      PIX_e_ogon             /* 0x82 e_ogon   ea */
      PIX_l_stroke           /* 0x83 l_stroke b3 */
      PIX_n_acute            /* 0x84 n_acute  f1 */
      PIX_o_acute            /* 0x85 o_acute  f3 */
      PIX_s_acute            /* 0x86 s_acute  b6 */
      PIX_z_acute            /* 0x87 z_acute  bc */
      PIX_z_dot              /* 0x88 z_dot    bf */
      PIX_A_ogon             /* 0x89 A_ogon   a1 */
      PIX_C_acute            /* 0x8a C_acute  c6 */
      PIX_E_ogon             /* 0x8b E_ogon   ca */
      PIX_L_stroke           /* 0x8c L_stroke a3 */
      PIX_N_acute            /* 0x8d N_acute  d1 */
      PIX_O_acute            /* 0x8e O_acute  d3 */
      PIX_S_acute            /* 0x8f S_acute  a6 */
      PIX_Z_acute            /* 0x90 Z_acute  ac */
      PIX_Z_dot              /* 0x91 Z_dot    af */
 #elif defined LANG_GERMAN
/*---D---ISO8859-2--------- */
      PIX_a_uml              /* 0x80 a_uml    e4 */
      PIX_o_uml              /* 0x81 o_uml    f6 */
      PIX_u_uml              /* 0x82 u_uml    fc */
      PIX_A_uml              /* 0x83 A_uml    c4 */
      PIX_O_uml              /* 0x84 O_uml    d6 */
      PIX_U_uml              /* 0x85 U_uml    dc */
      PIX_s_sharp            /* 0x86 s_sharp  df */
 #elif defined LANG_ALBANIAN
/*---AL---ISO8859-2--------- */
      PIX_c_cedil            /* 0x80 c_cedil  e7 */
      PIX_e_uml              /* 0x81 e_uml    eb */
      PIX_C_cedil            /* 0x82 C_cedil  c7 */
      PIX_E_uml              /* 0x83 E_uml    cb */
 #elif defined LANG_ITALIAN
      PIX_a_grave            /* 0x80 a_grave  e0 */
      PIX_e_acute            /* 0x81 e_acute  e9 */
      PIX_e_grave            /* 0x82 e_grave  e8 */
      PIX_i_acute            /* 0x83 i_acute     */
      PIX_i_circ             /* 0x84 i_circ   ee */
      PIX_i_grave            /* 0x85 i_grave     */
      PIX_o_acute            /* 0x86 o_acute  f3 */
      PIX_o_grave            /* 0x87 o_grave     */
      PIX_u_acute            /* 0x88 u_acute  fa */
      PIX_u_grave            /* 0x89 u_grave  f9 */

      PIX_A_grave            /* 0x9a A_grave  c0 */
      PIX_E_acute            /* 0x8b E_acute  c9 */
      PIX_E_grave            /* 0x9c E_grave  c8 */
      PIX_I_acute            /* 0x8d I_acute     */
      PIX_I_circ             /* 0x8e I_circ   ce */
      PIX_I_grave            /* 0x8f I_grave     */
      PIX_O_acute            /* 0x90 O_acute  d3 */
      PIX_O_grave            /* 0x91 O_grave     */
      PIX_U_acute            /* 0x92 U_acute  da */
      PIX_U_grave            /* 0x93 U_grave  d9 */
  #elif defined LANG_SPANISH
      PIX_a_acute            /* 0x80 a_acute  e1 */
      PIX_c_cedil            /* 0x81 c_cedil  e7 */
      PIX_e_acute            /* 0x82 e_acute  e9 */
      PIX_i_acute            /* 0x83 i_acute  ed */
      PIX_o_acute            /* 0x84 o_acute  f3 */
      PIX_u_acute            /* 0x85 u_acute  fa */
      PIX_u_uml              /* 0x86 u_uml    fc */
      PIX_A_acute            /* 0x87 A_acute  c1 */
      PIX_E_acute            /* 0x88 E_acute  c9 */
      PIX_I_acute            /* 0x89 I_acute  cd */
      PIX_O_acute            /* 0x8a O_acute  d3 */
      PIX_U_acute            /* 0x8b U_acute  da */
      /* PIX_a_super */           /* 0x8c a_super     */
      /* PIX_o_super */           /* 0x8d o_super     */
      /* PIX_n_tilde */           /* 0x8e n_tilde     */
      /* PIX_N_tilde */           /* 0x8f N_tilde     */
      /* PIX_updown_exclam */     /* 0x90 updown_exclam  */
      /* PIX_updown_question */   /* 0x91 updown_question */
 #elif defined LANG_HUNGARIAN
/* ---H----  ISO8859-2  ------------- */
      PIX_a_acute            /* 0x80 a_acute  e1 */
      PIX_e_acute            /* 0x81 e_acute  e9 */
      PIX_i_acute            /* 0x82 i_acute  ed */
      PIX_o_acute            /* 0x83 o_acute  f3 */
      PIX_o_dblac            /* 0x84 o_dblac  f5 */
      PIX_o_uml              /* 0x85 o_uml    f6 */
      PIX_u_acute            /* 0x86 u_acute  fa */
      PIX_u_dblac            /* 0x87 u_dblac  fb */
      PIX_u_uml              /* 0x88 u_uml    fc */
      PIX_A_acute            /* 0x89 A_acute  c1 */
      PIX_E_acute            /* 0x8a E_acute  c9 */
      PIX_I_acute            /* 0x8b I_acute  cd */
      PIX_O_acute            /* 0x8c O_acute  d3 */
      PIX_O_dblac            /* 0x8d O_dblac  d5 */
      PIX_O_uml              /* 0x8e O_uml    d6 */
      PIX_U_acute            /* 0x8f U_acute  da */
      PIX_U_dblac            /* 0x90 U_dblac  db */
      PIX_U_uml              /* 0x91 U_uml    dc */
 #elif defined LANG_ROMANIAN
/* ----RO--ISO-8859-2--------------- */
      PIX_a_circ             /* 0x80 a_circ   e2 */
      PIX_a_breve            /* 0x81 a_breve  e3 */
      PIX_i_circ             /* 0x82 i_circ   ee */
      PIX_s_cedil            /* 0x83 s_cedil  ba */
      PIX_t_cedil            /* 0x84 t_cedil  fe */
      PIX_A_circ             /* 0x85 A_circ   c2 */
      PIX_A_breve            /* 0x86 A_breve  c3 */
      PIX_I_circ             /* 0x87 I_circ   ce */
      PIX_S_cedil            /* 0x88 S_cedil  aa */
      PIX_T_cedil            /* 0x89 T_cedil  de */
 #elif defined LANG_DANISH || defined LANG_NORWEGIAN || defined LANG_SWEDISH
 /* ----DK---ISO-8859-15------- */
      PIX_a_ring             /* 0x80 a_ring   e5 */
      PIX_a_elig             /* 0x81 a_elig   e6 */
      PIX_o_slash            /* 0x82 o_slash  f8 */
      PIX_a_ring             /* 0x83 a_ring   c5 */
      PIX_A_elig             /* 0x84 A_elig   c6 */
      PIX_O_slash            /* 0x85 O_slash  d8 */
 #elif defined LANG_FINNISH
      PIX_a_ring             /* 0x80 a_ring   c5 */
      PIX_a_uml              /* 0x81 a_uml    e4 */
      PIX_o_uml              /* 0x82 o_uml    f6 */
      PIX_s_caron            /* 0x83 s_caron  b9 */
      PIX_z_caron            /* 0x84 z_caron  b9 */
      PIX_A_ring             /* 0x85 A_ring      */
      PIX_A_uml              /* 0x86 A_uml    c4 */
      PIX_O_uml              /* 0x87 O_uml    d6 */
      PIX_S_caron            /* 0x88 S_caron  a9 */
      PIX_Z_caron            /* 0x89 Z_caron  ae */
 #endif
#endif
