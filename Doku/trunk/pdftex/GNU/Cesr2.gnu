unset logscale
set logscale x 10
set pointsize 2
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
GNUTERM = "wxt"
set terminal pdf color 
set grid
set logscale x
set logscale y
set pointsize 2
set xrange [ 0.1e-6 : 0.001 ]
set style data points
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1.5
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 1.5
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 2 pointsize 0.5
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 7 pointsize 1
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 7 pointsize 1
set style line 7  linetype 1 linecolor rgb "black" linewidth 2.000  pointtype 4 pointsize 1.8

set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "ESR / Ω"
set key inside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Cesr2.pdf"
plot "../GNU/Cesr2_all.dat" u 2:16 title "328p" ls 1, "../GNU/Cesr2_all.dat" u 2:17 notitle ls 1 , "../GNU/Cesr2_all.dat" u 2:18 notitle ls 1,\
     "../GNU/Cesr2_all.dat" u 2:13 title "328" ls 4, "../GNU/Cesr2_all.dat" u 2:14 notitle ls 4 , "../GNU/Cesr2_all.dat" u 2:15 notitle ls 4,\
     "../GNU/Cesr2_all.dat" u 2:10 title "168p" ls 2, "../GNU/Cesr2_all.dat" u 2:11 notitle ls 2 , "../GNU/Cesr2_all.dat" u 2:12 notitle ls 2,\
     "../GNU/Cesr2_all.dat" u 2:7 title "168a" ls 5, "../GNU/Cesr2_all.dat" u 2:8 notitle ls 5 , "../GNU/Cesr2_all.dat" u 2:9 notitle ls 5,\
     "../GNU/Cesr2_all.dat" u 2:4 title "168" ls 6, "../GNU/Cesr2_all.dat" u 2:5 notitle ls 6 , "../GNU/Cesr2_all.dat" u 2:6 notitle ls 6,\
     "../GNU/Cesr2_all.dat" u 2:3 title "LCR" ls 7
set output
#
set xlabel "hodnota kapacity / F"
set ylabel "ESR / Ω"
set output "../GNU/Cesr2CZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F"
set ylabel "ESR / Ω"
set output "../GNU/Cesr2RU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "ESR / Ω"
set output "../GNU/Cesr2GE.pdf" ; replot ; set output
#
#    EOF
