#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char *argv[])
{
 int width;
 int height;
 int anz;
 int hexwert;
 int ii, kk;
 int hh;
 int lev;
 int line, row;
 int line_cnt[20];
 char linebuf[256];
 char filename[256];
 char asc_table[100][130];
 char *end_line;
 char BitSetChar;
 char BitClearChar;
 char *tx;
 FILE *outfile;
 if (argc != 2) {
    fprintf(stderr,"Count of arguments: %d\r\n",argc-1);
    fprintf(stderr,"Argument 1: file to be converted\r\n");
    exit(0);
 }
 stdin = fopen(argv[1],"r");
 if (stdin == (FILE *)NULL) {
    fprintf(stderr,"File %s can not be opened!\r\n",argv[1]);
    exit(-1);
 }
 width = 0;
 sscanf(argv[1],"%d",&width);
 if (width < 4) {
    fprintf(stderr,"Width can not be located from file name %s !\r\n",argv[1]);
    exit(-1);
 }
 hh = 0;
 if ((argv[1][1] == 'x') || (argv[1][1] == 'X')) hh = 2;
 if ((argv[1][2] == 'x') || (argv[1][2] == 'X')) hh = 3;
 sscanf(&argv[1][hh],"%d",&height);
 if ((hh == 0)|| (height < 5)) {
    fprintf(stderr,"Height can not be located from file name %s !\r\n",argv[1]);
    exit(-1);
 }
 fprintf(stderr,"Width = %d\r\n", width);
 fprintf(stderr,"Height = %d\r\n", height);
 hh = strlen(argv[1]);
 while (hh > 1) {
   hh--;
   if (argv[1][hh] == '.') argv[1][hh] = (char)0;
 }
 sprintf(filename,"%s.asc",argv[1]);
 fprintf(stderr,"Create file: %s\r\n", filename);
 outfile = fopen(filename, "w+");
 if (outfile == (FILE *)NULL) {
    fprintf(stderr,"File %s can not be created!\r\n",filename);
    exit(-1);
 }
 
 lev = 0;    
    anz = width;
    anz *= ((height + 7) / 8);
    line_cnt[lev] = 0;
    while (fgets(linebuf,256,stdin) != NULL)
    {
      for(ii=0;ii<256;ii++) {
        if (linebuf[ii] == '\n') linebuf[ii] = (char)0;
        if (linebuf[ii] == '\r') linebuf[ii] = (char)0;
      }
      if (linebuf[0] == '{') { 
         fprintf(outfile," /* 0x%02x ",line_cnt[lev]);
         if ((line_cnt[lev] >= (int)' ') && (line_cnt[lev] < 0x7f)) {
           fprintf(outfile,"'%c'",(char)line_cnt[lev]);
         }
         if (height <= 16) {
           if (line_cnt[lev] == 0) fprintf(outfile,"Resistor3 ");
           if (line_cnt[lev] == 1) fprintf(outfile,"Diode1 ");
           if (line_cnt[lev] == 2) fprintf(outfile,"Diode2 ");
           if (line_cnt[lev] == 3) fprintf(outfile,"Capacitor ");
           if (line_cnt[lev] == 4) fprintf(outfile,"Omega ");
           if (line_cnt[lev] == 5) fprintf(outfile,"mu ");
           if (line_cnt[lev] == 6) fprintf(outfile,"Resistor1 ");
           if (line_cnt[lev] == 7) fprintf(outfile,"Resistor2 ");
           if (line_cnt[lev] == 0xa0) fprintf(outfile,"Cyr_B ");
           if (line_cnt[lev] == 0xa1) fprintf(outfile,"Cyr_G ");
           if (line_cnt[lev] == 0xa2) fprintf(outfile,"Cyr_Jo ");
           if (line_cnt[lev] == 0xa3) fprintf(outfile,"Cyr_Tsch ");
           if (line_cnt[lev] == 0xa4) fprintf(outfile,"Cyr_Z ");
           if (line_cnt[lev] == 0xa5) fprintf(outfile,"Cyr_I ");
           if (line_cnt[lev] == 0xa6) fprintf(outfile,"Cyr_J ");
           if (line_cnt[lev] == 0xa7) fprintf(outfile,"Cyr_L ");
           if (line_cnt[lev] == 0xa8) fprintf(outfile,"Cyr_P ");
           if (line_cnt[lev] == 0xa9) fprintf(outfile,"Cyr_U ");
           if (line_cnt[lev] == 0xaa) fprintf(outfile,"Cyr_F");
           if (line_cnt[lev] == 0xab) fprintf(outfile,"Cyr_Tsch");
           if (line_cnt[lev] == 0xac) fprintf(outfile,"Cyr_Sch");
           if (line_cnt[lev] == 0xad) fprintf(outfile,"Cyr_HH");
           if (line_cnt[lev] == 0xae) fprintf(outfile,"Cyr_Y");
           if (line_cnt[lev] == 0xaf) fprintf(outfile,"Cyr_E");
           if (line_cnt[lev] == 0xb0) fprintf(outfile,"Cyr_Ju");
           if (line_cnt[lev] == 0xb1) fprintf(outfile,"Cyr_Ja");
           if (line_cnt[lev] == 0xb2) fprintf(outfile,"Cyr_b");
           if (line_cnt[lev] == 0xb3) fprintf(outfile,"Cyr_v");
           if (line_cnt[lev] == 0xb4) fprintf(outfile,"Cyr_g");
           if (line_cnt[lev] == 0xb5) fprintf(outfile,"Cyr_jo");
           if (line_cnt[lev] == 0xb6) fprintf(outfile,"Cyr_zsch");
           if (line_cnt[lev] == 0xb7) fprintf(outfile,"Cyr_z");
           if (line_cnt[lev] == 0xb8) fprintf(outfile,"Cyr_i");
           if (line_cnt[lev] == 0xb9) fprintf(outfile,"Cyr_j");
           if (line_cnt[lev] == 0xba) fprintf(outfile,"Cyr_k");
           if (line_cnt[lev] == 0xbb) fprintf(outfile,"Cyr_l");
           if (line_cnt[lev] == 0xbc) fprintf(outfile,"Cyr_m");
           if (line_cnt[lev] == 0xbd) fprintf(outfile,"Cyr_n");
           if (line_cnt[lev] == 0xbe) fprintf(outfile,"Cyr_p");
           if (line_cnt[lev] == 0xbf) fprintf(outfile,"Cyr_t");
           if (line_cnt[lev] == 0xc0) fprintf(outfile,"Cyr_tsch");
           if (line_cnt[lev] == 0xc1) fprintf(outfile,"Cyr_sch");
           if (line_cnt[lev] == 0xc2) fprintf(outfile,"Cyr_hh");
           if (line_cnt[lev] == 0xc3) fprintf(outfile,"Cyr_y");
           if (line_cnt[lev] == 0xc4) fprintf(outfile,"Cyr_ww");
           if (line_cnt[lev] == 0xc5) fprintf(outfile,"Cyr_e");
           if (line_cnt[lev] == 0xc6) fprintf(outfile,"Cyr_ju");
           if (line_cnt[lev] == 0xc7) fprintf(outfile,"Cyr_ja");
           if (line_cnt[lev] == 0x10) fprintf(outfile,"(Cyr_D)");
           if (line_cnt[lev] == 0x11) fprintf(outfile,"(Cyr_C)");
           if (line_cnt[lev] == 0x12) fprintf(outfile,"(Cyr_Schtsch)");
           if (line_cnt[lev] == 0x13) fprintf(outfile,"(Cyr_d)");
           if (line_cnt[lev] == 0x14) fprintf(outfile,"(Cyr_f)");
           if (line_cnt[lev] == 0x15) fprintf(outfile,"(Cyr_c)");
           if (line_cnt[lev] == 0x16) fprintf(outfile,"(Cyr_schtsch)");
           if (line_cnt[lev] == 0xe0) fprintf(outfile,"(Cyr_D)");
           if (line_cnt[lev] == 0xe1) fprintf(outfile,"(Cyr_C)");
           if (line_cnt[lev] == 0xe2) fprintf(outfile,"(Cyr_Schtsch)");
           if (line_cnt[lev] == 0xe3) fprintf(outfile,"(Cyr_d)");
           if (line_cnt[lev] == 0xe4) fprintf(outfile,"(Cyr_f)");
           if (line_cnt[lev] == 0xe5) fprintf(outfile,"(Cyr_c)");
           if (line_cnt[lev] == 0xe6) fprintf(outfile,"(Cyr_schtsch)");
         }
         fprintf(outfile," */\r\n");
         // init the table
         for (ii=0;ii<100;ii++) {
            for (kk=0;kk<height;kk++) asc_table[ii][kk] = '.';
            asc_table[ii][width] = (char)0;
         }
         BitSetChar = '*';
         BitClearChar = '.';
         tx = &linebuf[1];
         ii = 0;
         end_line = &linebuf[strlen(linebuf)];
         while (ii<anz ) {
            line = (ii / width) * 8;
            if ((tx[0] == '0') && ((tx[1] == 'x') || (tx[1] == 'X'))) {
              sscanf( tx,"%x,", &hexwert);
              row = (ii % width);
              if ((hexwert & 0x01) != 0) {
                asc_table[line][row] = BitSetChar;
              } else {
                asc_table[line][row] = BitClearChar;
              }
              if ((hexwert & 0x02) != 0) {
                asc_table[line+1][row] = BitSetChar;
              } else {
                asc_table[line+1][row] = BitClearChar;
              }
              if ((hexwert & 0x04) != 0) {
                asc_table[line+2][row] = BitSetChar;
              } else {
                asc_table[line+2][row] = BitClearChar;
              }
              if ((hexwert & 0x08) != 0) {
                asc_table[line+3][row] = BitSetChar;
              } else {
                asc_table[line+3][row] = BitClearChar;
              }
              if ((hexwert & 0x10) != 0) {
                asc_table[line+4][row] = BitSetChar;
              } else {
                asc_table[line+4][row] = BitClearChar;
              }
              if ((hexwert & 0x20) != 0) {
                asc_table[line+5][row] = BitSetChar;
              } else {
                asc_table[line+5][row] = BitClearChar;
              }
              if ((hexwert & 0x40) != 0) {
                asc_table[line+6][row] = BitSetChar;
              } else {
                asc_table[line+6][row] = BitClearChar;
              }
              if ((hexwert & 0x80) != 0) {
                asc_table[line+7][row] = BitSetChar;
              } else {
                asc_table[line+7][row] = BitClearChar;
              }
              tx += 5;
              ii++;
            } else {
              if ((tx[0] == '/') && (tx[1] == '*')) {
                BitClearChar = ' ';
                BitSetChar = 'O';
              }
              if ((tx[0] == '*') && (tx[1] == '/')) {
                BitSetChar = '*';
                BitClearChar = '.';
              }
              tx++;
              if (tx[0] == '}') {
                 fprintf(stderr,"Warning } is found!\r\n");
                 break;
              } 
              if (tx >= end_line) {
              // read new line
                if (fgets(linebuf,256,stdin) != NULL) {
                   for(kk=0;kk<256;kk++) {
                     if (linebuf[kk] == '\n') linebuf[kk] = (char)0;
                     if (linebuf[kk] == '\r') linebuf[kk] = (char)0;
                   }
                   end_line = &linebuf[strlen(linebuf)];
                   tx = &linebuf[0];
                }
              }
            }
         }
         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
            if ( ((kk % 8) == 0) && (kk != 0) ) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"-");
            }
         }
         fprintf(outfile," \r\n");

         for (kk=0; kk<height; kk++) {
            // Zeile aufbereiten
            if ((kk % 8) == 0) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"|");
            }
              fprintf(outfile,"%s",&asc_table[kk][0]);
            if ((kk % 8) == 0) {
              fprintf(outfile,"+\r\n");
            } else {
              fprintf(outfile,"|\r\n");
            }
         }

         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
            if ( ((kk % 8) == 0) && (kk != 0) ) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"-");
            }
          }
         fprintf(outfile," \r\n");
         line_cnt[lev]++;
      } else {
        fprintf(outfile,"%s\r\n", linebuf);
        if ((strncmp(linebuf,"#ifdef",6)) == 0) {
          lev++;
          line_cnt[lev] = line_cnt[lev-1];
        }
        if ((strncmp(linebuf,"#ifndef",7)) == 0) {
          lev++;
          line_cnt[lev] = line_cnt[lev-1];
        }
        if (((strncmp(linebuf,"#else",5)) == 0) && (lev > 0)) {
	  line_cnt[lev] = line_cnt[lev-1];
        }
        if (((strncmp(linebuf,"#endif",6)) == 0) && (lev > 0)) {
          line_cnt[lev-1] = line_cnt[lev];
          lev--;
        }
      }
    }
 fclose(outfile);
 return(0);
}

