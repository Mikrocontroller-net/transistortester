#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "Transistortester.h"


// get_log interpolate a table with the function -1000*log(1 - (permil/1000))
uint16_t get_log(uint16_t permil) {
// for remember:
// uint16_t LogTab[] PROGMEM = {0, 20, 41, 62, 83, 105, 128, 151, 174, 198, 223, 248, 274, 301, 329, 357, 386, 416, 446, 478, 511, 545, 580, 616, 654, 693, 734, 777, 821, 868, 916, 968, 1022, 1079, 1139, 1204, 1273, 1347, 1427, 1514, 1609, 1715, 1833, 1966, 2120, 2303, 2526 };


#define Log_Tab_Distance 20              // displacement of table is 20 mil

  uint16_t y1, y2;			// table values
  uint16_t result;			// result of interpolation
  uint8_t tabind;			// index to table value
  uint8_t tabres;			// distance to lower table value, fraction of Log_Tab_Distance

  tabind = permil / Log_Tab_Distance;	// index to table
  tabres = permil % Log_Tab_Distance;	// fraction of table distance
  // interpolate the table of factors
  y1 = pgm_read_word(&LogTab[tabind]);	// get the lower table value
  y2 = pgm_read_word(&LogTab[tabind+1]); // get the higher table value
  result = ((y2 - y1) * tabres ) / Log_Tab_Distance + y1; // interpolate
  return(result);
 }
