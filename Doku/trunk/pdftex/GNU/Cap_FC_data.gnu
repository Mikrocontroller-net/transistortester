#!/usr/bin/gnuplot -persist
unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set dummy x,y
set format y "% g"
set format x "%.0s%c"
set angles radians
set grid nopolar
set grid xtics nomxtics ytics nomytics noztics nomztics \
 nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 1.000
set raxis
set key title ""
set key inside right top vertical Right noreverse enhanced autotitles nobox
set key noinvert samplen 4 spacing 1 width 0 height 0 
set key maxcolumns 0 maxrows 0
set key noopaque
unset label
unset arrow
set style increment default
unset style line
unset style arrow
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set logscale y 10
set offsets 0, 0, 0, 0
set pointsize 1
set pointintervalbox 1
set encoding default
set view 60, 30, 1, 1
set samples 100, 100
set isosamples 10, 10
set surface
unset contour
set datafile separator whitespace
set size ratio 0 1,1
set origin 0,0
set style data linespoints
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0 autojustify
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0 autojustify
set ytics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set timestamp bottom 
set timestamp "" 
set timestamp  offset character 0, 0, 0 font "" norotate
set xlabel "Capacity / F" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ 0.000001 : 0.1 ] noreverse nowriteback
set ylabel "ESR / Ω" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ * : * ] noreverse nowriteback
GNUTERM = "wxt"
#plot "../GNU/Cap6_FC_ESR.dat" title "6V","../GNU/Cap10_FC_ESR.dat" title "10V",\
#"../GNU/Cap16_FC_ESR.dat" title "16V","../GNU/Cap25_FC_ESR.dat" title "25V",\
#"../GNU/Cap35_FC_ESR.dat" title "35V","../GNU/Cap50_FC_ESR.dat" title "50V",\
#"../GNU/Cap63_FC_ESR.dat" title "63V","../GNU/Cap100_FC_ESR.dat" title "100V"
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 7 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 2 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 1
# set terminal epslatex color solid
set terminal pdf color
set output "../GNU/Cap_FC_dataEN.pdf"
plot "../GNU/Cap6_FC_ESR.dat" using ($1*0.000001):2 title "6V" ls 1,\
"../GNU/Cap16_FC_ESR.dat" using ($1*0.000001):2 title "16V" ls 2,\
"../GNU/Cap35_FC_ESR.dat" using ($1*0.000001):2 title "35V" ls 3,\
"../GNU/Cap63_FC_ESR.dat" using ($1*0.000001):2 title "63V" ls 4
set output
#
set xlabel "kapacita / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FC_dataCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FC_dataRU.pdf" ; replot ; set output
#
set xlabel "Kapazität / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FC_dataGE.pdf" ; replot ; set output
#    EOF
