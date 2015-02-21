#include <stdio.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[])
{
 int width;
 int ii;	// loop counter
 int kk;	// loop counter
 int line;	// number of read ascii character bitmap
 int ll;	
 int height;
 int beg_end;
 char comment[256];
 char linebuf[256];	// buffer for reading a line from stdin
 int bbit;
 int bbit2;
 long bittab[80];	// bits for every line of bitmap
 
    
 line = 0;
 height = 0;
 while (fgets(linebuf,256,stdin) != NULL) {
    for (ll=0;ll<256;ll++) {
      if (linebuf[ll] == '\r') linebuf[ll] = (char)0;
      if (linebuf[ll] == '\n') linebuf[ll] = (char)0;
    }
//      fprintf(stdout,"%s", linebuf);
    beg_end = 0;
    if (linebuf[0] == '|') {
       if (height == 0) {
          for (kk=1; kk<256; kk++) {
            if (linebuf[kk] == '|') break; 
          }
          width = kk-1;
       }
       bittab[height] = 0;
       for (kk=1; kk<=width; kk++) {
          if (linebuf[kk] != ' ') bittab[height] |= (1<<(kk-1));
       }
       height++;
    } else {
      if ((strncmp(linebuf, "/* ",3)) == 0) {
         // save comment 
         strcpy(comment,linebuf);
      } else {
        if ((linebuf[0] == ' ') && (linebuf[1] == '-')) {
           // begin or end of block
           beg_end = 1;
         } else {
           // copy the line
           fprintf(stdout,"%s\r\n",linebuf);
         }
      }
    }
    if ((height > 5) && (beg_end != 0)) {
       // last line of character is found
       fprintf(stdout,"{");
       for (kk=0; kk<width; kk++) {
          bbit = 0;
          bbit2 = 0;
          for (ii=0; ii<height; ii++) {
             if (height <= 8) {
                if ((bittab[ii] & (1<<kk)) != 0) bbit |= (1<<ii);
             } else {
                if (ii >= 8) {
                   if ((bittab[ii] & (1<<kk)) != 0) bbit2 |= (1<<(ii-8));
                } else {
                   if ((bittab[ii] & (1<<kk)) != 0) bbit |= (1<<ii);
                }
             }
          } /* end for ii */
          fprintf(stdout,"0x%02X",bbit);
          if (height > 8) fprintf(stdout,",0x%02X",bbit2);
          if (kk != (width-1)) fprintf(stdout,",");
       } /* end for kk */
       fprintf(stdout,"}");
       sscanf(&comment[3],"%x",&line);
       if (line != 255) {
          fprintf(stdout,",");
       } else {
          fprintf(stdout," ");
       }
       fprintf(stdout,"\t%s\r\n",comment);
       height = 0;
    } /* end if ((height > 5) && (beg_end != 0)) */
 } /* end while */
 return(0);
}

