#!/usr/bin/gnuplot -persist
unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set style fill  empty border
set style rectangle back fc lt -3 fillstyle   solid 1.00 border lt -1
set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 1.000
set raxis
set key title ""
set style increment default
unset logscale
set logscale x 10
set logscale y 10
set offsets 0, 0, 0, 0
set pointsize 1
set pointintervalbox 1
set encoding default
set samples 100, 100
set isosamples 10, 10
set size ratio 0 1,1
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
set format x "%.0s%c"
set xlabel "Capacity / F" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ 0.00001 : 0.01 ] noreverse nowriteback
set ylabel "ESR / Ω" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ * : * ] noreverse nowriteback
set fit noerrorvariables
GNUTERM = "wxt"
#plot "../GNU/Cap6_FR_ESR.dat" title "6V","../GNU/Cap10_FR_ESR.dat" title "10V",\
#"../GNU/Cap16_FR_ESR.dat" title "16V","../GNU/Cap25_FR_ESR.dat" title "25V",\
#"../GNU/Cap35_FR_ESR.dat" title "35V","../GNU/Cap50_FR_ESR.dat" title "50V"\
#,"../GNU/Cap63_FR_ESR.dat" title "63V"
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 7 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 2 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 1
set terminal pdf color 
set output "../GNU/Cap_FR_data.pdf"
plot "../GNU/Cap6_FR_ESR.dat" using ($1*0.000001):2 title "6V" ls 1,\
"../GNU/Cap16_FR_ESR.dat" using ($1*0.000001):2 title "16V" ls 2,\
"../GNU/Cap35_FR_ESR.dat" using ($1*0.000001):2 title "35V" ls 3,\
"../GNU/Cap63_FR_ESR.dat" using ($1*0.000001):2 title "63V" ls 4
set output
set xlabel "kapacita / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FR_dataCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FR_dataRU.pdf" ; replot ; set output
#
set xlabel "Kapazität / F" 
set ylabel "ESR / Ω" 
set output "../GNU/Cap_FR_dataGE.pdf" ; replot ; set output
#
#    EOF
