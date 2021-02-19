#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[])
{
 int width;
 int kk;	// loop counter
// int line;	// number of read ascii character bitmap
 int ll;	
 int hh;
 int height;
 int beg_end;
 int lines;
 int comment_out;
 int kk_min, kk_max;
 int ll_min, ll_max;
 char comment_buf[256];
 char *start_txt;
 char linebuf[256];	// buffer for reading a line from stdin
 int bittab1[16][80];	// bits for every line of bitmap
 int bittab2[16][80];	// bits for every line of bitmap

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
 
#define vertical_offset 0
 if (vertical_offset != 0) {
   fprintf(stderr,"Vertikaler Offset %d\r\n",vertical_offset);
 }
    
// line = 0;
 height = 0;
 lines = 0;
 comment_buf[0] = ' ';
 comment_buf[1] = 0;  /* Leerer Kommentar */
 while (fgets(linebuf,256,stdin) != NULL) {
    lines++;
    for (ll=0;ll<256;ll++) {
      if (linebuf[ll] == '\r') linebuf[ll] = (char)0;
      if (linebuf[ll] == '\n') linebuf[ll] = (char)0;
    }
    beg_end = 0;
    if ((linebuf[0] == '|') || (linebuf[0] == '+')) {
       // moeglicherweise codiertes ASCII Zeichen 
       if (height == 0) {
          for (kk=1; kk<256; kk++) {
            if ((linebuf[kk] == '|') || (linebuf[kk] == '+')) break; 
          }
          width = kk-1;
       }
       for (kk=1; kk<=width; kk++) {
          hh = height + vertical_offset;
          if ((height == 0) || ((hh % 8) == 0)) {
             bittab1[hh/8][kk-1] = 0;  // clear all bits
             bittab2[hh/8][kk-1] = 0;  // clear all bits
          }
          if ((linebuf[kk] == '@') || (linebuf[kk] == '*') || (linebuf[kk] == ':') || (linebuf[kk] == 'O')) bittab1[hh/8][kk-1] |= (1<<(hh % 8));
          if ((linebuf[kk] == '@') || (linebuf[kk] == '*') || (linebuf[kk] == ' ')) bittab2[hh/8][kk-1] |= (1<<(hh % 8));
       }  /* end for kk */
       height++;
    } else {  /* Zeile beginnt nicht mit | oder + */
      start_txt = &linebuf[0];
      for (kk=0;kk<20;kk++) {
	if ((linebuf[kk] == ' ') || (linebuf[kk] == '\t')) start_txt = &linebuf[kk+1];
	else  break;
      }
      if ( ((strncmp(start_txt, "/* 0x",5)) == 0) ||
           ((strncmp(start_txt, "#define PIX_",12)) == 0) ) {
         // save comment 
         strcpy(comment_buf,start_txt);
	 /* prepare for output */
	 /* #define PIX_a_acute  { */
	 /* 0123456789012345678901 */
         if ( (strncmp(start_txt, "#define PIX_",12)) == 0) comment_buf[22] = 0;
      } else {
        if ((linebuf[0] == ' ') && (linebuf[1] == '-')) {
           // begin or end of block
           beg_end = 1;
         } else {
//           if ((linebuf[0] != '}') || ((linebuf[1] != ',') && (linebuf[1] != ';')))
	   {
             // copy the line
             fprintf(stdout,"%s\r\n",linebuf);
           }
         }
      }
    }
    if ((height > 5) && (beg_end != 0)) {
       // last line of character is found
       ll_min = height;
       ll_max = -1;
       kk_min = width;
       kk_max = -1;
       if ((strncmp(comment_buf, "#define PIX_",12)) == 0) fprintf(stdout,"%s",comment_buf);
       else                                                fprintf(stdout,"{");
       comment_out = 0;
       for (ll=0;ll<height;ll+=8) {
         for (kk=0;kk<width;kk++) {
 //           if (((kk%8) == 0) && (kk != 0)) fprintf(stdout,"\r\n  ");
            if ((bittab1[ll/8][kk] != bittab2[ll/8][kk]) && (comment_out == 0)) {
               comment_out = 1;
               fprintf(stdout,"/* ");		// Beginn Kommentar
            }
            if ((bittab1[ll/8][kk] == bittab2[ll/8][kk]) && (comment_out != 0)) {
               comment_out = 0;
               fprintf(stdout,"*/ ");		// Ende Kommentar
            }
            if (comment_out == 0) { // suche Max und Min des unkommentierten Feldes
              if (ll < ll_min) ll_min = ll;
              if (ll > ll_max) ll_max = ll;
              if (kk < kk_min) kk_min = kk;
              if (kk > kk_max) kk_max = kk;
            }
            fprintf(stdout,"0x%02X",bittab1[ll/8][kk]);		// Ausgabe des Hex-Wertes
            if (((ll+8) < height) || (kk != (width-1))) fprintf(stdout,",");
         } /* end for kk */
	 if (ll/8 == (height-1)/8 ) {
            if ((strncmp(comment_buf, "#define PIX_",12)) == 0) fprintf(stdout,"}, \r\n");
	    else                                                fprintf(stdout,"},  %s\r\n",comment_buf);
	 } else {
//          fprintf(stdout,"\r\n ");
            fprintf(stdout," ");
	 }
       } /* end for ll */
       if (comment_out != 0) {
          comment_out = 0;
          fprintf(stdout,"*/ ");
       }
#if 0
       int line;
       fprintf(stdout,"}");
       sscanf(&comment[3],"%x",&line);
       if (line != 255) {
          fprintf(stdout,",");
       } else {
          fprintf(stdout," ");
       }
       fprintf(stdout,"  /* x=%d, y=%d, w=%d, h=%d */\r\n", kk_min, ll_min, kk_max-kk_min+1, ll_max-ll_min+8);
       fprintf(stdout,"\t%s\r\n",comment);
#endif
       height = 0;
    } /* end if ((height > 5) && (beg_end != 0)) */
 } /* end while */
 return(0);
}

