
//definitions of parts  (values for global variable PartFound)
#define PART_NONE 0
#define PART_RESISTOR 1
#define PART_UJT 2
#define PART_DIODE 3
#define PART_TRANSISTOR 4
#define PART_FET 5
#define PART_THYRISTOR 6
#define PART_TRIAC 7
#define PART_CAPACITOR 8
#define PART_CELL 9
#define PART_PUT 10
#define PART_CERAMICRESONATOR 11
#define PART_XTAL 12

//End (parts)

//special definition for different parts   (values for global variable PartMode)
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
