#include <stdio.h>
#include <math.h>
#include <string.h>


int main(int argc, char *argv[])
{
 int width;
 int kk;	// loop counter
 int line;	// number of read ascii character bitmap
 int ll;	
 int height;
 int beg_end;
 int comment_out;
 int kk_min, kk_max;
 int ll_min, ll_max;
 char comment[256];
 char linebuf[256];	// buffer for reading a line from stdin
 int bittab1[16][80];	// bits for every line of bitmap
 int bittab2[16][80];	// bits for every line of bitmap
 
    
 line = 0;
 height = 0;
 while (fgets(linebuf,256,stdin) != NULL) {
    for (ll=0;ll<256;ll++) {
      if (linebuf[ll] == '\r') linebuf[ll] = (char)0;
      if (linebuf[ll] == '\n') linebuf[ll] = (char)0;
    }
//      fprintf(stdout,"%s", linebuf);
    beg_end = 0;
    if ((linebuf[0] == '|') || (linebuf[0] == '+')) {
       if (height == 0) {
          for (kk=1; kk<256; kk++) {
            if ((linebuf[kk] == '|') || (linebuf[kk] == '+')) break; 
          }
          width = kk-1;
       }
       for (kk=1; kk<=width; kk++) {
          if ((height % 8) == 0) {
             bittab1[height/8][kk-1] = 0;  // clear all bits
             bittab2[height/8][kk-1] = 0;  // clear all bits
          }
          if ((linebuf[kk] == '*') || (linebuf[kk] == ':') || (linebuf[kk] == 'O')) bittab1[height/8][kk-1] |= (1<<(height % 8));
          if ((linebuf[kk] == '*') || (linebuf[kk] == ' ')) bittab2[height/8][kk-1] |= (1<<(height % 8));
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
           if ((linebuf[0] != '}') || ((linebuf[1] != ',') && (linebuf[1] != ';'))) {
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
       fprintf(stdout,"{ ");
       comment_out = 0;
       for (ll=0;ll<height;ll+=8) {
         for (kk=0;kk<width;kk++) {
            if (((kk%8) == 0) && (kk != 0)) fprintf(stdout,"\r\n  ");
            if ((bittab1[ll/8][kk] != bittab2[ll/8][kk]) && (comment_out == 0)) {
               comment_out = 1;
               fprintf(stdout,"/* ");
            }
            if ((bittab1[ll/8][kk] == bittab2[ll/8][kk]) && (comment_out != 0)) {
               comment_out = 0;
               fprintf(stdout,"*/ ");
            }
            if (comment_out == 0) {
              if (ll < ll_min) ll_min = ll;
              if (ll > ll_max) ll_max = ll;
              if (kk < kk_min) kk_min = kk;
              if (kk > kk_max) kk_max = kk;
            }
            fprintf(stdout,"0x%02X",bittab1[ll/8][kk]);
            if (((ll+8) < height) || (kk != (width-1))) fprintf(stdout,",");
         }
         fprintf(stdout,"\r\n  ");
       }
       if (comment_out != 0) {
          comment_out = 0;
          fprintf(stdout,"*/ ");
       }
       fprintf(stdout,"}");
       sscanf(&comment[3],"%x",&line);
       if (line != 255) {
          fprintf(stdout,",");
       } else {
          fprintf(stdout," ");
       }
       fprintf(stdout,"  /* x=%d, y=%d, w=%d, h=%d */\r\n", kk_min, ll_min, kk_max-kk_min+1, ll_max-ll_min+8);
       fprintf(stdout,"\t%s\r\n",comment);
       height = 0;
    } /* end if ((height > 5) && (beg_end != 0)) */
 } /* end while */
fprintf(stdout,"};\r\n");
 return(0);
}

