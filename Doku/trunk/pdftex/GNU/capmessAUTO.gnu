unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set pointintervalbox 1
set samples 100, 100
set isosamples 10, 10
set clabel '%8.3g'
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
set xrange [ * : * ] noreverse nowriteback  # (currently [-11.0000:-1.00000] )
set ylabel "" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ * : * ] noreverse nowriteback  # (currently [-8.00000:8.00000] )
GNUTERM = "wxt"
set terminal pdf color 
set grid
set logscale x
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
set key outside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/MegaAuto.pdf"
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
