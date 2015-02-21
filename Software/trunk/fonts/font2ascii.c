#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char *argv[])
{
 int width;
 int height;
 int anz;
 int hex[128];
 int ii, kk;
 int hh;
 int step;
 int hbit;
 int lev;
 int line_cnt[20];
 char linebuf[256];
 char filename[256];
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
    if (height > 8) anz *= 2;
    line_cnt[lev] = 0;
    while (fgets(linebuf,256,stdin) != NULL)
    {
      for(ii=0;ii<256;ii++) {
        if (linebuf[ii] == '\n') linebuf[ii] = (char)0;
        if (linebuf[ii] == '\r') linebuf[ii] = (char)0;
      }
      if (linebuf[0] == '{') { 
         fprintf(outfile,"/* 0x%02x ",line_cnt[lev]);
         if ((line_cnt[lev] >= (int)' ') && (line_cnt[lev] < 0x7f)) {
           fprintf(outfile,"'%c'",(char)line_cnt[lev]);
         }
         if (line_cnt[lev] == 0) fprintf(outfile,"Resistor3 ");
         if (line_cnt[lev] == 1) fprintf(outfile,"Diode1 ");
         if (line_cnt[lev] == 2) fprintf(outfile,"Diode2 ");
         if (line_cnt[lev] == 3) fprintf(outfile,"Capacitor ");
         if (line_cnt[lev] == 4) fprintf(outfile,"Omega ");
         if (line_cnt[lev] == 5) fprintf(outfile,"mu ");
         if (line_cnt[lev] == 6) fprintf(outfile,"Resistor1 ");
         if (line_cnt[lev] == 7) fprintf(outfile,"Resistor2 ");
         fprintf(outfile," */\r\n");
         tx = &linebuf[1];
         for (ii=0; ii<anz; ii++) {
            sscanf( tx,"%x,", &hex[ii]);
            tx += 5;
         }
         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
            fprintf(outfile,"-");
         }
         fprintf(outfile," \r\n");

         for (kk=0; kk<height; kk++) {
//         for (kk=2; kk<(height+2); kk++) { /* for shifted font data, move 2 lines up */
            // Zeile aufbereiten
            fprintf(outfile,"|");
            hbit = (kk % 8); 
            step = 1;
            ii = 0;
            if (height > 8) {
               step = 2;		// 2 Byte for row
               if (kk >= 8) {
                  ii = 1;			// begin mit 2. Byte
               }
            }
            for (; ii<anz; ii+=step) {
               if ((hex[ii] & (1<<hbit)) != 0) {
                  fprintf(outfile,"*");
               } else {
                  fprintf(outfile," ");
               }
            }
            fprintf(outfile,"|\r\n");
         }

         fprintf(outfile," ");
         for (kk=0; kk<width; kk++) {
           fprintf(outfile,"-");
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

