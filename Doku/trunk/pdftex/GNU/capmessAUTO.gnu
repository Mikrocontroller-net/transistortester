set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set offsets 0, 0, 0, 0
set pointsize 2
set pointintervalbox 1
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
GNUTERM = "wxt"
set terminal pdf color 
set grid
set logscale x 10
set style data linespoints
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 2 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 3 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 2 pointsize 1
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 3 pointsize 1
set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / \%"
#set yrange [-2 to 10]
set xrange [10.e-12 : 200.e-6 ]
set key outside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/MegaAutoEN.pdf"
plot "../GNU/capmessAUTO.dat" u 3:($4-$3)/$3*100 title "168-3unc" ls 1 with points,   "../GNU/capmessAUTO.dat" u 3:($5-$3)/$3*100 title "168-3cal" ls 4 with lines,\
     "../GNU/capmessAUTO.dat" u 3:($6-$3)/$3*100 title "168PA-7unc" ls 2 with points, "../GNU/capmessAUTO.dat" u 3:($7-$3)/$3*100 title "168PA-7cal" ls 5 with lines,\
     "../GNU/capmessAUTO.dat" u 3:($8-$3)/$3*100 title "328P-14unc" ls 3 with points, "../GNU/capmessAUTO.dat" u 3:($9-$3)/$3*100 title "328P-14cal" ls 6 with lines
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/MegaAutoCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/MegaAutoRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/MegaAutoGE.pdf" ; replot ; set output
#
