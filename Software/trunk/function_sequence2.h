/*   file  function_sequence2.h  */
/*   defines the sequence of the menu items for the transistortester function menu */
/*   every constant with MODE_...   is computed from previous USE_... settings. */ 
/*   The USE_... settings are defined in autoconf.h dependig of Makefile settings. */
/*   If you change the sequence of menu items , you must change the order of the USE_... sum in the MODE_... defines. */
/*   An alternative sequence example is finished in the function_sequence1.h file */

#if USE_ESR == 1
 #define MODE_ESR 1
#else
 #define MODE_ESR 99  /* dummy number */
#endif

#if USE_RESIS == 1
 #define MODE_RESIS (USE_ESR + 1)
#else
 #define MODE_RESIS 99  /* dummy number */
#endif

#if USE_CAP13 == 1
 #define MODE_CAP13 (USE_ESR + USE_RESIS + 1)
#else
 #define MODE_RESIS 99  /* dummy number */
#endif

#if USE_FSCALER == 1
 #define MODE_FSCALER (USE_ESR + USE_RESIS + USE_CAP13 + 1)
#else
 #define MODE_FSCALER 99  /* dummy number */
#endif
#if USE_FREQ == 1
 #define MODE_FREQ (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + 1)
#else
 #define MODE_FREQ 99  /* dummy number */
#endif
#if USE_HFREQ == 1
 #define MODE_HFREQ (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + 1)
#else
 #define MODE_HFREQ 99  /* dummy number */
#endif
#if USE_H_CRYSTAL == 1
 #define MODE_H_CRYSTAL (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + 1)
#else
 #define MODE_H_CRYSTAL 99  /* dummy number */
#endif
#if USE_L_CRYSTAL == 1
 #define MODE_L_CRYSTAL (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + 1)
#else
 #define MODE_L_CRYSTAL 99  /* dummy number */
#endif
#if USE_FGEN == 1
 #define MODE_FGEN (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + 1)
#else
 #define MODE_FGEN 99  /* dummy number */
#endif
#if USE_PWM == 1
 #define MODE_PWM (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + 1)
#else
 #define MODE_PWM 99  /* dummy number */
#endif
#if USE_ROTARY == 1
 #define MODE_ROTARY (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + 1)
#else
 #define MODE_ROTARY 99  /* dummy number */
#endif
#if USE_BIG_CAP_CORR == 1
 #define MODE_BIG_CAP_CORR (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + 1)
#else
 #define MODE_BIG_CAP_CORR 99  /* dummy number */
#endif
#if USE_VEXT == 1
 #define MODE_VEXT (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + 1)
#else
 #define MODE_VEXT 99  /* dummy number */
#endif
#if USE_SELFTEST == 1
 #define MODE_SELFTEST (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + 1)
#else
 #define MODE_SELFTEST 99  /* dummy number */
#endif
#if USE_CONTRAST == 1
 #define MODE_CONTRAST (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + 1)
#else
 #define MODE_CONTRAST 99  /* dummy number */
#endif
#if USE_SELECT_FG == 1
 #define MODE_SELECT_FG (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + USE_CONTRAST + 1)
#else
 #define MODE_SELECT_FG 99  /* dummy number */
#endif
#if USE_SELECT_BG == 1
 #define MODE_SELECT_BG (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + USE_CONTRAST + USE_SELECT_FG + 1)
#else
 #define MODE_SELECT_BG 99  /* dummy number */
#endif
#if USE_SHOW == 1
 #define MODE_SHOW (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + USE_CONTRAST + USE_SELECT_FG + USE_SELECT_BG + 1)
#else
 #define MODE_SHOW 99  /* dummy number */
#endif
#ifdef USE_OFF
 #define MODE_OFF (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + USE_CONTRAST + USE_SELECT_FG + USE_SELECT_BG + USE_SHOW + 1)
#else
 #define MODE_OFF 99  /* dummy number */
#endif
#define MODE_LAST (USE_ESR + USE_RESIS + USE_CAP13 + USE_FSCALER + USE_FREQ + USE_HFREQ + USE_H_CRYSTAL + USE_L_CRYSTAL + USE_FGEN + USE_PWM + USE_ROTARY + USE_BIG_CAP_CORR + USE_VEXT + USE_SELFTEST + USE_CONTRAST + USE_SELECT_FG + USE_SELECT_BG + USE_SHOW + USE_OFF)
