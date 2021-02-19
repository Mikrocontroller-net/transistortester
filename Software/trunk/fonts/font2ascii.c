#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define BitIsSET '*'
//#define BitIsSET '#'
#define BitIsSET '@'
//#define BitIsSET ' '     /* special invers Darstellung */

int main(int argc, char *argv[])
{
 int width,new_width;
 int height;
 int anz;
 int hexwert;
 int ii, kk;
 int blanks;
 int hh;
 int line, row;
 int lines;
 char origbuf[256];
 char *linebuf;
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
 /* Test-Ausgabe */
//  for (kk=0x20; kk<255; kk++) {
//    if ((kk%16) == 0) fprintf(stderr,"\r\n");
//    fprintf(stderr,"%c",kk);
//  }
//  fprintf(stderr,"\r\n");
 
    anz = width;
    anz *= ((height + 7) / 8);
    lines = 0;
    while (fgets(origbuf,256,stdin) != NULL)
    {
      lines++;
      for(ii=0;ii<256;ii++) {
        if (origbuf[ii] == '\n') origbuf[ii] = (char)0;
        if (origbuf[ii] == '\r') origbuf[ii] = (char)0;
      }
      /* remove leading Blanks */
      for (blanks=0;blanks<20;blanks++) {
	if (origbuf[blanks] != ' ') break;
      }
      linebuf = &origbuf[blanks];
      if (linebuf[0] == 0) {
           linebuf[1] = 0;
           linebuf[20] = 0;
           linebuf[22] = 0;
         }
      if ( ((linebuf[0]  == '{') && (linebuf[1]  == '0')) ||
           ((linebuf[1]  == ' ') && (linebuf[2]  == '{') && (linebuf[3]  == '0')) ||
	   ((strncmp(&linebuf[0],"#define PIX_",12) == 0) && (linebuf[21] == '{') && (linebuf[22] == '0')) ||
	   ((strncmp(&linebuf[2],"#define PIX_",12) == 0) && (linebuf[23] == '{') && (linebuf[24] == '0')) ) { 
         if ( (linebuf[0]  == '{') ) tx = &linebuf[1];
         if ( (linebuf[1]  == ' ') && (linebuf[2]  == '{') ) tx = &linebuf[3]; /* probably diff output "< {" "> {" */
	 if ( (linebuf[20] == ' ') && (linebuf[21] == '{') ) {
	    /* probably #define style */
            /* #define PIX_a_stroke {0x.... */
            /* 012345678901234567890123     */
            tx = &linebuf[22];
	 }
	 if ( (linebuf[22] == ' ') && (linebuf[23] == '{') ) {
	    /* probably #define style */
            /* < #define PIX_a_stroke {0x.... */
            /* 01234567890123456789012345   */
            tx = &linebuf[24];
	 }
	 if ( (strncmp(linebuf,"#define PIX_",12) == 0) ||
              (strncmp(&linebuf[2],"#define PIX_",12) == 0) ) {			 
            fprintf(outfile,"%s\r\n",&linebuf[0]);
         }
	 for (ii=0;ii<235;ii++) {
            if (tx[ii] == '}') {
	       /* end of byte list found */
	       anz = ((ii +2) / 5);
               new_width = anz / ((height + 7) / 8);
	       if (new_width != width) {
//                fprintf(outfile,"// **** new width = %d\r\n",new_width);
	          width = new_width;
	       }
	       break;
	    }
	 }  /* end for ii */
         // init the table
         for (ii=0;ii<100;ii++) {
            for (kk=0;kk<height;kk++) asc_table[ii][kk] = '.';
            asc_table[ii][width] = (char)0;
         }
         BitSetChar = BitIsSET;
         BitClearChar = '.';
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
            } else {  /* no "0x" */
              if ((tx[0] == '/') && (tx[1] == '*')) {
                BitClearChar = ' ';
                BitSetChar = 'O';
              }
              if ((tx[0] == '*') && (tx[1] == '/')) {
                BitSetChar = BitIsSET;
                BitClearChar = '.';
              }
              tx++;
              if (tx[0] == '}') {
                 fprintf(stderr,"Warning } is found at line %d!\r\n",lines);
                 break;
              } 
              if (tx >= end_line) {
              // read new line
                if (fgets(origbuf,256,stdin) != NULL) {
		   lines++;
                   for(kk=0;kk<256;kk++) {
                     if (origbuf[kk] == '\n') origbuf[kk] = (char)0;
                     if (origbuf[kk] == '\r') origbuf[kk] = (char)0;
                   }
                   /* remove leading Blanks */
                   for (blanks=0;blanks<20;blanks++) {
	             if (origbuf[blanks] != ' ') break;
                   }
                   linebuf = &origbuf[blanks];
                   end_line = &linebuf[strlen(linebuf)];
                   tx = &linebuf[0];
                }
              }
            }  /* no "0x" */
         } /* end while(ii<anz) */
	 if ( tx[2] != 0 ) fprintf(outfile,"%s\r\n",&tx[2]);
         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
            if ( ((kk % 8) == 0) && (kk != 0) ) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"-");
            }
         } /* end for kk */
         fprintf(outfile," \r\n");

         for (kk=0; kk<height; kk++) {
            // Zeile aufbereiten
            if ((kk % 8) == 0) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"|");
            }
//              fprintf(outfile,"%s",&asc_table[kk][0]);
            int ll;
            ll = 0;
	    while (asc_table[kk][ll] != 0) {
 #if BitIsSET == ' '
	     if (asc_table[kk][ll] == BitIsSET ) {
	       fprintf(outfile,"\033[0;7m%c\033[0m",asc_table[kk][ll]);
	     } else {
	       fprintf(outfile,"%c",asc_table[kk][ll]);
	     }
 #else
	     fprintf(outfile,"%c",asc_table[kk][ll]);
 #endif
	     ll++;
	    } /* end while */
            if ((kk % 8) == 0) {
              fprintf(outfile,"+\r\n");
            } else {
              fprintf(outfile,"|\r\n");
            }
         } /* end for kk */

         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
            if ( ((kk % 8) == 0) && (kk != 0) ) {
              fprintf(outfile,"+");
            } else {
              fprintf(outfile,"-");
            }
          }  /* end for kk */
//         fprintf(outfile," l=%d\r\n",lines);
         fprintf(outfile," \r\n");
      } else {  /* linebuf != "{0" */
        fprintf(outfile,"%s\r\n", origbuf);
      }
    }
 fclose(outfile);
 return(0);
}

