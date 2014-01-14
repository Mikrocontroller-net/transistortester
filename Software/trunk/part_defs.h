
//definitions of parts
#define PART_NONE 0
#define PART_RESISTOR 1
#define PART_DIODE 2
#define PART_TRANSISTOR 3
#define PART_FET 4
#define PART_THYRISTOR 5
#define PART_TRIAC 6
#define PART_CAPACITOR 7
#define PART_CELL 8

//End (parts)
//special definition for different parts 
//FETs , even numbers are N-type
#define PART_MODE_NONE 0
#define PART_MODE_MOS 2
#define PART_MODE_IGBT 3
#define PART_MODE_JFET 4
#define P_CHANNEL 0x10
#define N_CHANNEL 0x00
#define D_MODE 0x20
#define E_MODE 0x00

//Bipolar
#define PART_MODE_NPN 12
#define PART_MODE_PNP 13
