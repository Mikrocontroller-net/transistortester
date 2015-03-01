// table of the bits for the 8x8 character set
// each byte hold 8 bits of a row, the upper bit is 1, the bit for the 8th line is 128
 #ifdef LCD_CYRILLIC                                      
  #define CHAR_COUNT8 (Cyr_ja + 16)
 #else
  #define CHAR_COUNT8 128
 #endif
const unsigned char PROGMEM font[CHAR_COUNT8][8]={
{0x08,0x3E,0x22,0x22,0x22,0x22,0x3E,0x08},	// 0x00   //00/ --> Resistor3 // {0x00,0x3E,0x45,0x51,0x45,0x3E},	// 0x00
{0x08,0x7F,0x3E,0x1C,0x08,0x7F,0x7F,0x08},	// 0x01   //01/ --> Diode1
{0x08,0x7F,0x7F,0x08,0x1C,0x3E,0x7F,0x08},	// 0x02   //02/ --> Diode2   // {0x00,0x3E,0x6B,0x6F,0x6B,0x3E},	// 0x02
{0x08,0x7F,0x7F,0x00,0x00,0x7F,0x7F,0x08},	// 0x03   //03/ --> Capacitance //{0x00,0x1C,0x3E,0x7C,0x3E,0x1C},	// 0x03
{0x4C,0x5E,0x72,0x02,0x72,0x5E,0x4C,0x00},	// 0x04   //04/ --> Omega    // {0x00,0x18,0x3C,0x7E,0x3C,0x18},	// 0x04
{0xC0,0x7E,0x3E,0x20,0x1E,0x3E,0x20,0x00},	// 0x05   //05/ --> mu        // {0x00,0x30,0x36,0x7F,0x36,0x30},	// 0x05
{0x41,0x41,0x41,0x41,0x41,0x41,0x7F,0x08},	// 0x06   //06/ --> Resistor1 // {0x00,0x18,0x5C,0x7E,0x5C,0x18},	// 0x06
{0x08,0x7F,0x41,0x41,0x41,0x41,0x41,0x41},	// 0x07   //07/ --> Resistor2 // {0x00,0x00,0x00,0x00,0x00,0x00},	// 0x07
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x08   //08/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x09   //09/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0A   //10/     
{0x70,0xF8,0x88,0x88,0xFD,0x7F,0x07,0x0F},	// 0x0B   //11/     
{0x00,0x4E,0x5F,0xF1,0xF1,0x5F,0x4E,0x00},	// 0x0C   //12/     
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0D   //13/     
{0xC0,0xFF,0x7F,0x05,0x05,0x65,0x7F,0x3F},	// 0x0E   //14/     
{0x15,0x0A,0x1F,0x1F,0x0A,0x15,0x00,0x00},	// 0x0F   //15/     
#ifdef LCD_CYRYLLIC
{0x38,0x7C,0x46,0x47,0x45,0x7C,0x38,0x00},	// 0xE0  ,Cyr_D
{0xFC,0xFE,0x2A,0x2A,0x3E,0x14,0x00,0x00},	// 0xE1  ,Cyr_C
{0x3A,0x7D,0x45,0x45,0x45,0x7D,0x3A,0x00},	// 0xE2  ,Cyr_Schtsch
{0x38,0x7C,0x45,0x47,0x46,0x7C,0x38,0x00},	// 0xE3  ,Cyr_d
{0x32,0x7B,0x49,0x4B,0x7A,0x33,0x01,0x00},	// 0xE4  ,Cyr_f
{0x3A,0x7F,0x45,0x47,0x46,0x7F,0x39,0x00},	// 0xE5  ,Cyr_c
{0x80,0xFE,0x7E,0x20,0x20,0x3E,0x1E,0x00},	// 0xE6  ,Cyr_schtsch
#else
{0x00,0x00,0x7F,0x3E,0x1C,0x08,0x00,0x00},	// 0x10   //16/     
{0x00,0x00,0x08,0x1C,0x3E,0x7F,0x00,0x00},	// 0x11   //17/     
{0x00,0x24,0x66,0xFF,0xFF,0x66,0x24,0x00},	// 0x12   //18/     
{0x00,0x5F,0x5F,0x00,0x00,0x5F,0x5F,0x00},	// 0x13   //19/     
{0x06,0x0F,0x09,0x7F,0x7F,0x01,0x7F,0x7F},	// 0x14   //20/     
{0xDA,0xBF,0xA5,0xA5,0xFD,0x59,0x03,0x02},	// 0x15   //21/     
{0x00,0x70,0x70,0x70,0x70,0x70,0x70,0x00},	// 0x16   //22/     
#endif
{0x80,0x94,0xB6,0xFF,0xFF,0xB6,0x94,0x80},	// 0x17   //23/     
{0x00,0x04,0x06,0x7F,0x7F,0x06,0x04,0x00},	// 0x18   //24/     
{0x00,0x10,0x30,0x7F,0x7F,0x30,0x10,0x00},	// 0x19   //25/     
{0x08,0x08,0x08,0x2A,0x3E,0x1C,0x08,0x00},	// 0x1A   //26/     
{0x08,0x1C,0x3E,0x2A,0x08,0x08,0x08,0x00},	// 0x1B   //27/     
{0x3C,0x3C,0x20,0x20,0x20,0x20,0x20,0x00},	// 0x1C   //28/     
{0x08,0x1C,0x3E,0x08,0x08,0x3E,0x1C,0x08},	// 0x1D   //29/     
{0x30,0x38,0x3C,0x3E,0x3E,0x3C,0x38,0x30},	// 0x1E   //30/     
{0x06,0x0E,0x1E,0x3E,0x3E,0x1E,0x0E,0x06},	// 0x1F   //31/     
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20   //32/     
{0x00,0x06,0x6F,0x6F,0x06,0x00,0x00,0x00},	// 0x21   //33/     
{0x00,0x0B,0x07,0x00,0x00,0x0B,0x07,0x00},	// 0x22   //34/     
{0x36,0x7F,0x7F,0x36,0x7F,0x7F,0x36,0x00},	// 0x23   //35/     
{0x04,0x2E,0x2A,0x6B,0x2A,0x3A,0x10,0x00},	// 0x24   //36/     
{0x63,0x73,0x38,0x1C,0x0E,0x67,0x63,0x00},	// 0x25   //37/     
{0x36,0x7F,0x49,0x79,0x3B,0x6A,0x48,0x00},	// 0x26   //38/     
{0x00,0x00,0x07,0x0B,0x00,0x00,0x00,0x00},	// 0x27   //39/     
{0x00,0x00,0x1C,0x3E,0x63,0x41,0x00,0x00},	// 0x28   //40/     
{0x00,0x00,0x41,0x63,0x3E,0x1C,0x00,0x00},	// 0x29   //41/     
{0x00,0x15,0x0A,0x1F,0x1F,0x0A,0x15,0x00},	// 0x2A   //42/     
{0x00,0x18,0x18,0x7E,0x7E,0x18,0x18,0x00},	// 0x2B   //43/     
{0x00,0x00,0xB0,0x70,0x00,0x00,0x00,0x00},	// 0x2C   //44/     
{0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08},	// 0x2D   //45/     
{0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00},	// 0x2E   //46/     
{0x60,0x70,0x38,0x1C,0x0E,0x07,0x03,0x00},	// 0x2F   //47/     
{0x1C,0x3E,0x63,0x41,0x63,0x3E,0x1C,0x00},	// 0x30   //48/     
{0x00,0x42,0x7F,0x7F,0x40,0x00,0x00,0x00},	// 0x31   //49/     
{0x62,0x73,0x51,0x59,0x49,0x4F,0x46,0x00},	// 0x32   //50/     
{0x22,0x63,0x41,0x49,0x49,0x77,0x36,0x00},	// 0x33   //51/     
{0x18,0x1C,0x16,0x13,0x7F,0x7F,0x10,0x00},	// 0x34   //52/     
{0x27,0x67,0x45,0x45,0x45,0x7D,0x38,0x00},	// 0x35   //53/     
{0x3C,0x7E,0x4B,0x49,0x49,0x79,0x30,0x00},	// 0x36   //54/     
{0x03,0x03,0x71,0x79,0x0D,0x07,0x03,0x00},	// 0x37   //55/     
{0x36,0x7F,0x49,0x49,0x49,0x7F,0x36,0x00},	// 0x38   //56/     
{0x06,0x4F,0x49,0x49,0x69,0x3F,0x1E,0x00},	// 0x39   //57/     
{0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00},	// 0x3A   //58/     
{0x00,0x00,0x00,0xA6,0x66,0x00,0x00,0x00},	// 0x3B   //59/     
{0x00,0x08,0x1C,0x36,0x63,0x00,0x00,0x00},	// 0x3C   //60/     
{0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x00},	// 0x3D   //61/     
{0x00,0x41,0x63,0x36,0x1C,0x08,0x00,0x00},	// 0x3E   //62/     
{0x02,0x03,0x69,0x6D,0x07,0x06,0x00,0x00},	// 0x3F   //63/     
{0x3E,0x63,0x5D,0x55,0x5D,0x6B,0x2E,0x00},	// 0x40   //64/     
{0x7C,0x7E,0x13,0x11,0x13,0x7E,0x7C,0x00},	// 0x41   //65/ A   
{0x7F,0x7F,0x49,0x49,0x49,0x7F,0x36,0x00},	// 0x42   //66/ B   
{0x1C,0x3E,0x63,0x41,0x41,0x63,0x22,0x00},	// 0x43   //67/ C   
{0x7F,0x7F,0x41,0x41,0x63,0x3E,0x1C,0x00},	// 0x44   //68/ D   
{0x7F,0x7F,0x49,0x49,0x49,0x49,0x41,0x00},	// 0x45   //69/ E   
{0x7F,0x7F,0x09,0x09,0x09,0x09,0x01,0x00},	// 0x46   //70/ F   
{0x1C,0x3E,0x63,0x41,0x49,0x79,0x79,0x00},	// 0x47   //71/ G   
{0x7F,0x7F,0x08,0x08,0x08,0x7F,0x7F,0x00},	// 0x48   //72/ H   
{0x00,0x00,0x41,0x7F,0x7F,0x41,0x00,0x00},	// 0x49   //73/ I   
{0x30,0x70,0x40,0x40,0x40,0x7F,0x3F,0x00},	// 0x4A   //74/ J   
{0x7F,0x7F,0x18,0x3C,0x36,0x63,0x41,0x00},	// 0x4B   //75/ K   
{0x7F,0x7F,0x40,0x40,0x40,0x40,0x40,0x00},	// 0x4C   //76/ L   
{0x7F,0x7F,0x0E,0x1C,0x0E,0x7F,0x7F,0x00},	// 0x4D   //77/ M   
{0x7F,0x7F,0x0E,0x1C,0x38,0x7F,0x7F,0x00},	// 0x4E   //78/ N   
{0x1C,0x3E,0x63,0x41,0x63,0x3E,0x1C,0x00},	// 0x4F   //79/ O   
{0x7F,0x7F,0x11,0x11,0x11,0x1F,0x0E,0x00},	// 0x50   //80/ P   
{0x3E,0x7F,0x41,0x51,0x71,0x3F,0x5E,0x00},	// 0x51   //81/ Q   
{0x7F,0x7F,0x09,0x19,0x39,0x6F,0x46,0x00},	// 0x52   //82/ R   
{0x26,0x6F,0x49,0x49,0x49,0x7B,0x32,0x00},	// 0x53   //83/ S   
{0x00,0x01,0x01,0x7F,0x7F,0x01,0x01,0x00},	// 0x54   //84/ T   
{0x3F,0x7F,0x40,0x40,0x40,0x7F,0x3F,0x00},	// 0x55   //85/ U   
{0x0F,0x1F,0x38,0x70,0x38,0x1F,0x0F,0x00},	// 0x56   //86/ V   
{0x3F,0x7F,0x40,0x38,0x40,0x7F,0x3F,0x00},	// 0x57   //87/ W   
{0x63,0x77,0x3E,0x1C,0x3E,0x77,0x63,0x00},	// 0x58   //88/ X   
{0x00,0x07,0x0F,0x78,0x78,0x0F,0x07,0x00},	// 0x59   //89/ Y   
{0x61,0x71,0x79,0x5D,0x4F,0x47,0x43,0x00},	// 0x5A   //90/ Z   
{0x00,0x7F,0x7F,0x41,0x41,0x00,0x00,0x00},	// 0x5B   //91/     
{0x03,0x07,0x0E,0x1C,0x38,0x70,0x60,0x00},	// 0x5C   //92/     
{0x00,0x41,0x41,0x7F,0x7F,0x00,0x00,0x00},	// 0x5D   //93/     
{0x08,0x0C,0x06,0x03,0x06,0x0C,0x08,0x00},	// 0x5E   //94/     
{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},	// 0x5F   //95/     
{0x00,0x00,0x07,0x0B,0x00,0x00,0x00,0x00},	// 0x60   //96/     
{0x30,0x7A,0x4A,0x4A,0x7E,0x3C,0x40,0x00},	// 0x61   //97/ a   
{0x7F,0x7F,0x44,0x42,0x62,0x7E,0x3C,0x00},	// 0x62   //98/ b   
{0x18,0x3C,0x66,0x42,0x42,0x66,0x24,0x00},	// 0x63   //99/ c   
{0x3C,0x7E,0x42,0x42,0x44,0x7F,0x7F,0x00},	// 0x64   //100/ d  
{0x3C,0x7E,0x4A,0x4A,0x4A,0x6E,0x2C,0x00},	// 0x65   //101/ e  
{0x00,0x08,0x7E,0x7F,0x09,0x0A,0x00,0x00},	// 0x66   //102/ f  
{0x1C,0x3E,0xA2,0xA2,0x92,0xFE,0x7C,0x00},	// 0x67   //103/ g  
{0x7F,0x7F,0x04,0x02,0x02,0x7E,0x7C,0x00},	// 0x68   //104/ h  
{0x00,0x00,0x48,0x7B,0x7B,0x40,0x00,0x00},	// 0x69   //105/ i  
{0x00,0x40,0xC0,0x88,0xFB,0x7B,0x00,0x00},	// 0x6A   //106/ j  
{0x00,0x7F,0x7F,0x08,0x34,0x66,0x42,0x00},	// 0x6B   //107/ k  
{0x00,0x00,0x01,0x7F,0x7F,0x00,0x00,0x00},	// 0x6C   //108/ l  
{0x7E,0x7E,0x02,0x7E,0x04,0x7E,0x7C,0x00},	// 0x6D   //109/ m  
{0x7E,0x7E,0x04,0x02,0x06,0x7C,0x78,0x00},	// 0x6E   //110/ n  
{0x18,0x3C,0x66,0x42,0x66,0x3C,0x18,0x00},	// 0x6F   //111/ o  
{0xFE,0xFE,0x22,0x42,0x42,0x7E,0x3C,0x00},	// 0x70   //112/ p  
{0x00,0x3C,0x7E,0x42,0x42,0x22,0xFE,0xFE},	// 0x71   //113/ q  
{0x7E,0x7E,0x04,0x02,0x02,0x0E,0x0C,0x00},	// 0x72   //114/ r  
{0x24,0x6E,0x4A,0x4A,0x4A,0x7A,0x30,0x00},	// 0x73   //115/ s  
{0x04,0x04,0x3F,0x7F,0x44,0x64,0x20,0x00},	// 0x74   //116/ t  
{0x3E,0x7E,0x40,0x40,0x20,0x7E,0x7E,0x00},	// 0x75   //117/ u  
{0x0E,0x1E,0x30,0x60,0x30,0x1E,0x0E,0x00},	// 0x76   //118/ v  
{0x3E,0x7E,0x40,0x30,0x40,0x7E,0x3E,0x00},	// 0x77   //119/ w  
{0x62,0x76,0x1C,0x08,0x1C,0x76,0x62,0x00},	// 0x78   //120/ x  
{0x1E,0x3E,0xA0,0xA0,0x90,0xFE,0x7E,0x00},	// 0x79   //121/ y  
{0x42,0x62,0x72,0x5A,0x4E,0x46,0x00,0x00},	// 0x7A   //122/ z  
{0x08,0x3E,0x77,0x41,0x41,0x00,0x00,0x00},	// 0x7B   //123/    
{0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00},	// 0x7C   //124/    
{0x00,0x00,0x41,0x41,0x77,0x3E,0x08,0x00},	// 0x7D   //125/    
{0x1C,0x1C,0x1C,0x7F,0x3E,0x1C,0x08,0x00},	// 0x7E   //126/    
{0x08,0x1C,0x3E,0x7F,0x1C,0x1C,0x1C,0x00},	// 0x7F   //127/    
 #if 0
{0x1E,0xBF,0xE1,0x61,0x33,0x12,0x00,0x00},	// 0x80   //128/    
{0x3A,0x7A,0x40,0x40,0x7A,0x7A,0x40,0x00},	// 0x81   //129/    
{0x38,0x7C,0x56,0x57,0x5D,0x18,0x00,0x00},	// 0x82   //130/    
{0x02,0x23,0x75,0x55,0x55,0x7D,0x7B,0x42},	// 0x83   //131/    
{0x21,0x75,0x54,0x54,0x7D,0x79,0x40,0x00},	// 0x84   //132/    
{0x20,0x75,0x57,0x56,0x7C,0x78,0x40,0x00},	// 0x85   //133/    
{0x00,0x22,0x77,0x55,0x55,0x7F,0x7A,0x40},	// 0x86   //134/    
{0x1C,0xBE,0xE2,0x62,0x36,0x14,0x00,0x00},	// 0x87   //135/    
{0x02,0x3B,0x7D,0x55,0x55,0x5D,0x1B,0x02},	// 0x88   //136/    
{0x39,0x7D,0x54,0x54,0x5D,0x19,0x00,0x00},	// 0x89   //137/    
{0x38,0x7D,0x57,0x56,0x5C,0x18,0x00,0x00},	// 0x8A   //138/    
{0x01,0x45,0x7C,0x7C,0x41,0x01,0x00,0x00},	// 0x8B   //139/    
{0x02,0x03,0x45,0x7D,0x7D,0x43,0x02,0x00},	// 0x8C   //140/    
{0x00,0x45,0x7F,0x7E,0x40,0x00,0x00,0x00},	// 0x8D   //141/    
{0x79,0x7D,0x26,0x26,0x7D,0x79,0x00,0x00},	// 0x8E   //142/    
{0x70,0x7A,0x2D,0x2D,0x7A,0x70,0x00,0x00},	// 0x8F   //143/    
{0x44,0x7C,0x7E,0x57,0x55,0x44,0x00,0x00},	// 0x90   //144/    
{0x20,0x74,0x54,0x54,0x7C,0x7C,0x54,0x54},	// 0x91   //145/    
{0x7C,0x7E,0x0B,0x09,0x7F,0x7F,0x49,0x00},	// 0x92   //146/    
{0x32,0x7B,0x49,0x49,0x7B,0x32,0x00,0x00},	// 0x93   //147/    
{0x32,0x7A,0x48,0x48,0x7A,0x32,0x00,0x00},	// 0x94   //148/    
{0x30,0x79,0x4B,0x4A,0x78,0x30,0x00,0x00},	// 0x95   //149/    
{0x3A,0x7B,0x41,0x41,0x7B,0x7A,0x40,0x00},	// 0x96   //150/    
{0x38,0x79,0x43,0x42,0x78,0x78,0x40,0x00},	// 0x97   //151/    
{0xBA,0xBA,0xA0,0xA0,0xFA,0x7A,0x00,0x00},	// 0x98   //152/    
{0x39,0x7D,0x44,0x44,0x44,0x7D,0x39,0x00},	// 0x99   //153/    
{0x3D,0x7D,0x40,0x40,0x7D,0x3D,0x00,0x00},	// 0x9A   //154/    
{0x38,0x7C,0x64,0x54,0x4C,0x7C,0x38,0x00},	// 0x9B   //155/    
{0x68,0x7E,0x7F,0x49,0x43,0x66,0x20,0x00},	// 0x9C   //156/    
{0x5C,0x3E,0x73,0x49,0x67,0x3E,0x1D,0x00},	// 0x9D   //157/    
{0x44,0x6C,0x38,0x38,0x6C,0x44,0x00,0x00},	// 0x9E   //158/    
{0x40,0xC8,0x88,0xFE,0x7F,0x09,0x0B,0x02},	// 0x9F   //159/    
 #endif
#ifndef LCD_CYRILLIC                                                
{0x20,0x74,0x56,0x57,0x7D,0x78,0x40,0x00},	// 0xA0  ,Cyr_B
{0x00,0x44,0x7E,0x7F,0x41,0x00,0x00,0x00},	// 0xA1  ,Cyr_G
{0x30,0x78,0x48,0x4A,0x7B,0x31,0x00,0x00},	// 0xA2  ,Cyr_Jo
{0x38,0x78,0x40,0x42,0x7B,0x79,0x40,0x00},	// 0xA3  ,Cyr_Zsch
{0x7A,0x7B,0x09,0x0B,0x7A,0x73,0x01,0x00},	// 0xA4  ,Cyr_Z
{0x7A,0x7B,0x19,0x33,0x7A,0x7B,0x01,0x00},	// 0xA5  ,Cyr_I
{0x00,0x26,0x2F,0x29,0x2F,0x2F,0x28,0x00},	// 0xA6  ,Cyr_J
{0x00,0x26,0x2F,0x29,0x29,0x2F,0x26,0x00},	// 0xA7  ,Cyr_L
{0x30,0x78,0x4D,0x45,0x60,0x20,0x00,0x00},	// 0xA8  ,Cyr_P
{0x1C,0x22,0x7D,0x4B,0x5B,0x65,0x22,0x1C},	// 0xA9  ,Cyr_U
{0x08,0x08,0x08,0x08,0x38,0x38,0x00,0x00},	// 0xAA  ,Cyr_F
{0x61,0x3F,0x1F,0xCC,0xEE,0xAB,0xB9,0x90},	// 0xAB  ,Cyr_Tsch
{0x61,0x3F,0x1F,0x4C,0x66,0x73,0xD9,0xF8},	// 0xAC  ,Cyr_Sch
{0x00,0x00,0x60,0xFA,0xFA,0x60,0x00,0x00},	// 0xAD  ,Cyr_HH
{0x08,0x1C,0x36,0x22,0x08,0x1C,0x36,0x22},	// 0xAE  ,Cyr_Y
{0x22,0x36,0x1C,0x08,0x22,0x36,0x1C,0x08},	// 0xAF  ,Cyr_E
{0xAA,0x00,0x55,0x00,0xAA,0x00,0x55,0x00},	// 0xB0  ,Cyr_Ju
{0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55},	// 0xB1  ,Cyr_Ja
{0x55,0xFF,0xAA,0xFF,0x55,0xFF,0xAA,0xFF},	// 0xB2  ,Cyr_b
{0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00},	// 0xB3  ,Cyr_v
{0x10,0x10,0x10,0xFF,0xFF,0x00,0x00,0x00},	// 0xB4  ,Cyr_g
{0x70,0x78,0x2C,0x2E,0x7B,0x71,0x00,0x00},	// 0xB5  ,Cyr_jo
{0x72,0x79,0x2D,0x2D,0x79,0x72,0x00,0x00},	// 0xB6  ,Cyr_zsch
{0x71,0x7B,0x2E,0x2C,0x78,0x70,0x00,0x00},	// 0xB7  ,Cyr_z
{0x1C,0x22,0x5D,0x55,0x55,0x41,0x22,0x1C},	// 0xB8  ,Cyr_i
{0x14,0x14,0xF7,0xF7,0x00,0xFF,0xFF,0x00},	// 0xB9  ,Cyr_j
{0x00,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00},	// 0xBA  ,Cyr_k
{0x14,0x14,0xF4,0xF4,0x04,0xFC,0xFC,0x00},	// 0xBB  ,Cyr_l
{0x14,0x14,0x17,0x17,0x10,0x1F,0x1F,0x00},	// 0xBC  ,Cyr_m
{0x18,0x3C,0x24,0xE7,0xE7,0x24,0x24,0x00},	// 0xBD  ,Cyr_n
{0x2B,0x2F,0xFC,0xFC,0x2F,0x2B,0x00,0x00},	// 0xBE  ,Cyr_p
{0x10,0x10,0x10,0xF0,0xF0,0x00,0x00,0x00},	// 0xBF  ,Cyr_t
{0x00,0x00,0x00,0x1F,0x1F,0x10,0x10,0x10},	// 0xC0  ,Cyr_tsch
{0x10,0x10,0x10,0x1F,0x1F,0x10,0x10,0x10},	// 0xC1  ,Cyr_sch
{0x10,0x10,0x10,0xF0,0xF0,0x10,0x10,0x10},	// 0xC2  ,Cyr_hh
{0x00,0x00,0x00,0xFF,0xFF,0x10,0x10,0x10},	// 0xC3  ,Cyr_y
{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},	// 0xC4  ,Cyr_ww
{0x10,0x10,0x10,0xFF,0xFF,0x10,0x10,0x10},	// 0xC5  ,Cyr_e
{0x22,0x77,0x55,0x57,0x7E,0x7B,0x41,0x00},	// 0xC6  ,Cyr_ju
{0x72,0x7B,0x2D,0x2F,0x7A,0x73,0x01,0x00},	// 0xC7  ,Cyr_ja
 #if 0
{0x00,0x00,0x1F,0x1F,0x10,0x17,0x17,0x14},	// 0xC8             
{0x00,0x00,0xFC,0xFC,0x04,0xF4,0xF4,0x14},	// 0xC9             
{0x14,0x14,0x17,0x17,0x10,0x17,0x17,0x14},	// 0xCA             
{0x14,0x14,0xF4,0xF4,0x04,0xF4,0xF4,0x14},	// 0xCB             
{0x00,0x00,0xFF,0xFF,0x00,0xF7,0xF7,0x14},	// 0xCC             
{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},	// 0xCD             
{0x14,0x14,0xF7,0xF7,0x00,0xF7,0xF7,0x14},	// 0xCE             
{0x66,0x3C,0x3C,0x24,0x3C,0x3C,0x66,0x00},	// 0xCF             
{0x05,0x27,0x72,0x57,0x7D,0x38,0x00,0x00},	// 0xD0             
{0x49,0x7F,0x7F,0x49,0x63,0x7F,0x1C,0x00},	// 0xD1             
{0x46,0x7D,0x7D,0x55,0x55,0x46,0x00,0x00},	// 0xD2             
{0x45,0x7D,0x7C,0x54,0x55,0x45,0x00,0x00},	// 0xD3             
{0x44,0x7D,0x7F,0x56,0x54,0x44,0x00,0x00},	// 0xD4             
{0x0A,0x0E,0x08,0x00,0x00,0x00,0x00,0x00},	// 0xD5             
{0x00,0x44,0x7E,0x7F,0x45,0x00,0x00,0x00},	// 0xD6             
{0x02,0x45,0x7D,0x7D,0x45,0x02,0x00,0x00},	// 0xD7             
{0x01,0x45,0x7C,0x7C,0x45,0x01,0x00,0x00},	// 0xD8             
{0x10,0x10,0x10,0x1F,0x1F,0x00,0x00,0x00},	// 0xD9             
{0x00,0x00,0x00,0xF0,0xF0,0x10,0x10,0x10},	// 0xDA             
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	// 0xDB             
{0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},	// 0xDC             
{0x00,0x00,0x00,0x77,0x77,0x00,0x00,0x00},	// 0xDD             
{0x00,0x45,0x7F,0x7E,0x44,0x00,0x00,0x00},	// 0xDE             
{0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},	// 0xDF             
{0x38,0x7C,0x46,0x47,0x45,0x7C,0x38,0x00},	// 0xE0  ,Cyr_D
{0xFC,0xFE,0x2A,0x2A,0x3E,0x14,0x00,0x00},	// 0xE1  ,Cyr_C
{0x3A,0x7D,0x45,0x45,0x45,0x7D,0x3A,0x00},	// 0xE2  ,Cyr_Schtsch
{0x38,0x7C,0x45,0x47,0x46,0x7C,0x38,0x00},	// 0xE3  ,Cyr_d
{0x32,0x7B,0x49,0x4B,0x7A,0x33,0x01,0x00},	// 0xE4  ,Cyr_f
{0x3A,0x7F,0x45,0x47,0x46,0x7F,0x39,0x00},	// 0xE5  ,Cyr_c
{0x80,0xFE,0x7E,0x20,0x20,0x3E,0x1E,0x00},	// 0xE6  ,Cyr_schtsch
{0x42,0x7E,0x7E,0x54,0x1C,0x08,0x00,0x00},	// 0xE7  
{0x41,0x7F,0x7F,0x55,0x14,0x1C,0x08,0x00},	// 0xE8             
{0x3C,0x7C,0x42,0x43,0x7D,0x3C,0x00,0x00},	// 0xE9             
{0x3A,0x79,0x41,0x41,0x79,0x3A,0x00,0x00},	// 0xEA             
{0x3C,0x7D,0x43,0x42,0x7C,0x3C,0x00,0x00},	// 0xEB             
{0xB8,0xB8,0xA2,0xA3,0xF9,0x78,0x00,0x00},	// 0xEC             
{0x0C,0x5C,0x72,0x73,0x5D,0x0C,0x00,0x00},	// 0xED             
{0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00},	// 0xEE             
{0x00,0x00,0x02,0x03,0x01,0x00,0x00,0x00},	// 0xEF             
{0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},	// 0xF0             
{0x44,0x44,0x5F,0x5F,0x44,0x44,0x00,0x00},	// 0xF1             
{0x28,0x28,0x28,0x28,0x28,0x28,0x00,0x00},	// 0xF2             
{0x71,0x35,0x1F,0x4C,0x66,0x73,0xD9,0xF8},	// 0xF3             
{0x06,0x0F,0x09,0x7F,0x7F,0x01,0x7F,0x7F},	// 0xF4             
{0xDA,0xBF,0xA5,0xA5,0xFD,0x59,0x03,0x02},	// 0xF5             
{0x08,0x08,0x6B,0x6B,0x08,0x08,0x00,0x00},	// 0xF6             
{0x00,0x80,0xC0,0x40,0x00,0x00,0x00,0x00},	// 0xF7             
{0x00,0x06,0x0F,0x09,0x0F,0x06,0x00,0x00},	// 0xF8             
{0x02,0x02,0x00,0x00,0x02,0x02,0x00,0x00},	// 0xF9             
{0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00},	// 0xFA             
{0x00,0x12,0x13,0x1F,0x1F,0x10,0x10,0x00},	// 0xFB             
{0x00,0x11,0x15,0x15,0x1F,0x1F,0x0A,0x00},	// 0xFC             
{0x00,0x19,0x1D,0x15,0x17,0x12,0x00,0x00},	// 0xFD             
{0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00},	// 0xFE             
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} 	// 0xFF             
 #endif
#else                                                               
 #if 0
{0x7F,0x7F,0x49,0x49,0x49,0x79,0x30,0x00},	// 0xA0   //160/ �  
{0x7F,0x7F,0x01,0x01,0x01,0x01,0x01,0x00},	// 0xA1   //161/ �  
{0x7C,0x7D,0x55,0x54,0x55,0x55,0x44,0x00},	// 0xA2   //162/ �  
{0x63,0x36,0x08,0x7F,0x08,0x36,0x63,0x00},	// 0xA3   //163/ �  
{0x22,0x63,0x41,0x49,0x49,0x77,0x36,0x00},	// 0xA4   //164/ �  
{0x7F,0x7F,0x30,0x18,0x0C,0x7F,0x7F,0x00},	// 0xA5   //165/ �  
{0x7C,0x7C,0x31,0x19,0x0D,0x7C,0x7C,0x00},	// 0xA6   //166/ �  
{0x60,0x3C,0x02,0x01,0x01,0x7F,0x7F,0x00},	// 0xA7   //167/ �  
{0x7F,0x7F,0x01,0x01,0x01,0x7F,0x7F,0x00},	// 0xA8   //168/ �  
{0x07,0x0F,0x48,0x48,0x48,0x7F,0x3F,0x00},	// 0xA9   //169/ �  
{0x1C,0x3E,0x22,0x7F,0x22,0x3E,0x1C,0x00},	// 0xAA   //170/ �  
{0x07,0x0F,0x08,0x08,0x08,0x7F,0x7F,0x00},	// 0xAB   //171/ �  
{0x7F,0x7F,0x40,0x7F,0x40,0x7F,0x7F,0x00},	// 0xAC   //172/ �  
{0x03,0x7F,0x7F,0x48,0x48,0x78,0x30,0x00},	// 0xAD   //173/ �  
{0x7F,0x7F,0x48,0x78,0x30,0x7F,0x7F,0x00},	// 0xAE   //174/ �  
{0x22,0x63,0x41,0x49,0x49,0x7F,0x3E,0x00},	// 0xAF   //175/ �  
{0x7F,0x7F,0x08,0x3E,0x41,0x7F,0x3E,0x00},	// 0xB0   //176/ �  
{0x06,0x4F,0x69,0x39,0x19,0x7F,0x7F,0x00},	// 0xB1   //177/ �  
{0x3C,0x7E,0x4B,0x49,0x49,0x78,0x30,0x00},	// 0xB2   //178/ �  
{0x7E,0x7E,0x4A,0x4A,0x4A,0x7E,0x34,0x00},	// 0xB3   //179/ �  
{0x7E,0x7E,0x02,0x02,0x02,0x06,0x06,0x00},	// 0xB4   //180/ �  
{0x38,0x7D,0x55,0x54,0x55,0x55,0x18,0x00},	// 0xB5   //181/ �  
{0x62,0x34,0x08,0x7E,0x08,0x34,0x62,0x00},	// 0xB6   //182/ �  
{0x24,0x66,0x42,0x4A,0x4A,0x7E,0x34,0x00},	// 0xB7   //183/ �  
{0x7E,0x7E,0x30,0x18,0x0C,0x7E,0x7E,0x00},	// 0xB8   //184/ �  
{0x7C,0x7C,0x31,0x19,0x0D,0x7C,0x7C,0x00},	// 0xB9   //185/ �  
{0x7E,0x7E,0x08,0x1C,0x36,0x62,0x40,0x00},	// 0xBA   //186/ �  
{0x60,0x3C,0x02,0x02,0x02,0x7E,0x7E,0x00},	// 0xBB   //187/ �  
{0x7E,0x7E,0x0C,0x18,0x0C,0x7E,0x7E,0x00},	// 0xBC   //188/ �  
{0x7E,0x7E,0x08,0x08,0x08,0x7E,0x7E,0x00},	// 0xBD   //189/ �  
{0x7E,0x7E,0x02,0x02,0x02,0x7E,0x7E,0x00},	// 0xBE   //190/ �  
{0x02,0x02,0x7E,0x7E,0x02,0x02,0x00,0x00},	// 0xBF   //191/ �  
{0x0E,0x1E,0x10,0x10,0x10,0x7E,0x7E,0x00},	// 0xC0   //192/ �  
{0x7E,0x7E,0x40,0x7E,0x40,0x7E,0x7E,0x00},	// 0xC1   //193/ �  
{0x06,0x7E,0x7E,0x48,0x48,0x78,0x30,0x00},	// 0xC2   //194/ �  
{0x7E,0x7E,0x48,0x78,0x30,0x7E,0x7E,0x00},	// 0xC3   //195/ �  
{0x7E,0x7E,0x48,0x48,0x48,0x78,0x30,0x00},	// 0xC4   //196/ �  
{0x24,0x66,0x42,0x4A,0x7E,0x3C,0x00,0x00},	// 0xC5   //197/ �  
{0x7E,0x7E,0x08,0x3C,0x42,0x7E,0x3C,0x00},	// 0xC6   //198/ �  
{0x0C,0x5E,0x72,0x32,0x12,0x7E,0x7E,0x00},	// 0xC7   //199/ �  
{0x00,0x00,0x1F,0x1F,0x10,0x17,0x17,0x14},	// 0xC8   //200/    
{0x00,0x00,0xFC,0xFC,0x04,0xF4,0xF4,0x14},	// 0xC9   //201/    
{0x14,0x14,0x17,0x17,0x10,0x17,0x17,0x14},	// 0xCA   //202/    
{0x14,0x14,0xF4,0xF4,0x04,0xF4,0xF4,0x14},	// 0xCB   //203/    
{0x00,0x00,0xFF,0xFF,0x00,0xF7,0xF7,0x14},	// 0xCC   //204/    
{0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14},	// 0xCD   //205/    
{0x14,0x14,0xF7,0xF7,0x00,0xF7,0xF7,0x14},	// 0xCE   //206/    
{0x66,0x3C,0x3C,0x24,0x3C,0x3C,0x66,0x00},	// 0xCF   //207/    
{0x05,0x27,0x72,0x57,0x7D,0x38,0x00,0x00},	// 0xD0   //208/    
{0x49,0x7F,0x7F,0x49,0x63,0x7F,0x1C,0x00},	// 0xD1   //209/    
{0x46,0x7D,0x7D,0x55,0x55,0x46,0x00,0x00},	// 0xD2   //210/    
{0x45,0x7D,0x7C,0x54,0x55,0x45,0x00,0x00},	// 0xD3   //211/    
{0x44,0x7D,0x7F,0x56,0x54,0x44,0x00,0x00},	// 0xD4   //212/    
{0x0A,0x0E,0x08,0x00,0x00,0x00,0x00,0x00},	// 0xD5   //213/    
{0x00,0x44,0x7E,0x7F,0x45,0x00,0x00,0x00},	// 0xD6   //214/    
{0x02,0x45,0x7D,0x7D,0x45,0x02,0x00,0x00},	// 0xD7   //215/    
{0x01,0x45,0x7C,0x7C,0x45,0x01,0x00,0x00},	// 0xD8   //216/    
{0x10,0x10,0x10,0x1F,0x1F,0x00,0x00,0x00},	// 0xD9   //217/    
{0x00,0x00,0x00,0xF0,0xF0,0x10,0x10,0x10},	// 0xDA   //218/    
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	// 0xDB   //219/    
{0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0},	// 0xDC   //220/    
{0x00,0x00,0x00,0x77,0x77,0x00,0x00,0x00},	// 0xDD   //221/    
{0x00,0x45,0x7F,0x7E,0x44,0x00,0x00,0x00},	// 0xDE   //222/    
{0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F},	// 0xDF   //223/    
{0xC0,0x70,0x4E,0x41,0x7F,0xFF,0xC0,0x00},	// 0xE0   //224/ �  
{0x7F,0x7F,0x40,0x40,0x7F,0xFF,0xC0,0x00},	// 0xE1   //225/ �  
{0x7F,0x7F,0x40,0x7F,0x40,0xFF,0xFF,0x00},	// 0xE2   //226/ �  
{0xC0,0x70,0x4C,0x42,0x7E,0xFE,0xC0,0x00},	// 0xE3   //227/ �  
{0x38,0x7C,0x44,0xFE,0x44,0x7C,0x38,0x00},	// 0xE4   //228/ �  
{0x7E,0x7E,0x40,0x40,0x7E,0xFE,0xC0,0x00},	// 0xE5   //229/ �  
{0x7E,0x7E,0x40,0x7E,0x40,0xFE,0xFE,0x00},	// 0xE6   //230/ �  
{0x42,0x7E,0x7E,0x54,0x1C,0x08,0x00,0x00},	// 0xE7   //231/    
{0x41,0x7F,0x7F,0x55,0x14,0x1C,0x08,0x00},	// 0xE8   //232/    
{0x3C,0x7C,0x42,0x43,0x7D,0x3C,0x00,0x00},	// 0xE9   //233/    
{0x3A,0x79,0x41,0x41,0x79,0x3A,0x00,0x00},	// 0xEA   //234/    
{0x3C,0x7D,0x43,0x42,0x7C,0x3C,0x00,0x00},	// 0xEB   //235/    
{0xB8,0xB8,0xA2,0xA3,0xF9,0x78,0x00,0x00},	// 0xEC   //236/    
{0x0C,0x5C,0x72,0x73,0x5D,0x0C,0x00,0x00},	// 0xED   //237/    
{0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00},	// 0xEE   //238/    
{0x00,0x00,0x02,0x03,0x01,0x00,0x00,0x00},	// 0xEF   //239/    
{0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},	// 0xF0   //240/    
{0x44,0x44,0x5F,0x5F,0x44,0x44,0x00,0x00},	// 0xF1   //241/    
{0x28,0x28,0x28,0x28,0x28,0x28,0x00,0x00},	// 0xF2   //242/    
{0x71,0x35,0x1F,0x4C,0x66,0x73,0xD9,0xF8},	// 0xF3   //243/    
{0x06,0x0F,0x09,0x7F,0x7F,0x01,0x7F,0x7F},	// 0xF4   //244/    
{0xDA,0xBF,0xA5,0xA5,0xFD,0x59,0x03,0x02},	// 0xF5   //245/    
{0x08,0x08,0x6B,0x6B,0x08,0x08,0x00,0x00},	// 0xF6   //246/    
{0x00,0x80,0xC0,0x40,0x00,0x00,0x00,0x00},	// 0xF7   //247/    
{0x00,0x06,0x0F,0x09,0x0F,0x06,0x00,0x00},	// 0xF8   //248/    
{0x02,0x02,0x00,0x00,0x02,0x02,0x00,0x00},	// 0xF9   //249/    
{0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00},	// 0xFA   //250/    
{0x00,0x12,0x13,0x1F,0x1F,0x10,0x10,0x00},	// 0xFB   //251/    
{0x00,0x11,0x15,0x15,0x1F,0x1F,0x0A,0x00},	// 0xFC   //252/    
{0x00,0x19,0x1D,0x15,0x17,0x12,0x00,0x00},	// 0xFD   //253/    
{0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00},	// 0xFE   //254/    
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFF   //255/    
 #endif
#endif                                          
};
                                                
