
#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


unsigned int GetRLmultip(unsigned int cvolt) {

// interpolate table RLtab corresponding to voltage cvolt
// Widerstand 680 Ohm          300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
//uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};

#define RL_Tab_Abstand 25       // displacement of table 25mV
#define RL_Tab_Beginn 300       // begin of table ist 300mV
#define RL_Tab_Length 1100      // length of table is 1400-300

  unsigned int uvolt;
  unsigned int y1, y2;
  uint8_t tabind;
  uint8_t tabres;
  if (cvolt >= RL_Tab_Beginn) {
     uvolt = cvolt - RL_Tab_Beginn;
  } else {
     uvolt = 0;			// limit to begin of table
  }
  tabind = uvolt / RL_Tab_Abstand;
  tabres = uvolt % RL_Tab_Abstand;
  tabres = RL_Tab_Abstand - tabres;
  if (tabind > ((RL_Tab_Length/RL_Tab_Abstand)-1)) {
     tabind = (RL_Tab_Length/RL_Tab_Abstand)-1;	// limit to end of table
     tabres = 0;
  }
  y1 = MEM_read_word(&RLtab[tabind]);
  y2 = MEM_read_word(&RLtab[tabind+1]);
  return ( ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand + y2); // interpolate table
}

