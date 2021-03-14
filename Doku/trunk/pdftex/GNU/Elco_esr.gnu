#!/usr/bin/gnuplot -persist
set key title ""
unset logscale
set pointsize 1
set samples 100, 100
set size ratio 0 1,1
set origin 0,0
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set ytics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set xlabel "" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel "" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set terminal pdf color 
set style line 1  linetype 1 linecolor rgb "green" linewidth 2.000  pointtype 4 pointsize 0.8
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 2 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 0.7
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 1 pointsize 0.7
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 7 pointsize 0.5
set style line 7  linetype 1 linecolor rgb "black" linewidth 2.000  pointtype 4 pointsize 0.6
set grid
set logscale y
set yrange [ 0.01 : 100 ] 
set xrange [ 0 : 18 ]
#set xtics 1,1,14
set xtics rotate
set xtics ("0.47µ/100V" 1 0,"1µ/100V" 2 0,"1µ/50V" 3 0,"2.2µ/100V" 4 0,"2.2µ/50V" 5 0,"3.3µ/100V" 6 0,"4.7µ/63V" 7 0,"4.7µ/50V" 8 0,"10µ/50V" 9 0,"22µ/10V" 10 0,"22µ/63V" 11 0,"33µ/63V" 12 0,"47µ/63V" 13 0,"100µ/63V" 14 0,"220µ/63V" 15 0,"470µ/35V" 16 0,"500µ/3V" 17 0)
#set ytics 0.1,5,50
GNUTERM = "wxt"
set ylabel "ESR / Ω"
#set title "ESR values for different capacitors"
set output "../GNU/Elco_esrEN.pdf"
plot "../GNU/Elco_esr.dat" u 1:4 title "LCR-100Hz" ls 1 with points,"../GNU/Elco_esr.dat" u 1:8 title "LCR-1kHz" ls 4 with points, "../GNU/Elco_esr.dat" u 1:10 title "LCR-10kHz" ls 3 with points, "../GNU/Elco_esr.dat" u 1:12 title "LCR-100kHz" ls 5 with points, "../GNU/Elco_esr.dat" u 1:17 title "TTester" ls 7 with points
set output
set ylabel "ESR / Ω"
set output "../GNU/Elco_esrCZ.pdf" ; replot ; set output
#
set ylabel "ESR / Ω"
set output "../GNU/Elco_esrRU.pdf" ; replot ; set output
#
set ylabel "ESR / Ω"
set output "../GNU/Elco_esrGE.pdf" ; replot ; set output
#
#set y2range [ 0.01 : 100 ] 
#set y2label "Vloss / \%"
# "../GNU/Elco_esr.dat" u 1:16 title "Vloss" ls 6 with points
#    EOF
