unset clip points
set clip one
unset clip two
set border 31 front linetype -1 linewidth 1.000
set key maxcolumns 0 maxrows 0
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set pointintervalbox 1
set encoding default
set view 60, 30, 1, 1
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
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : * ] noreverse nowriteback  # (currently [-11.0000:-1.00000] )
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set key inside left top vertical Right noreverse enhanced autotitles nobox
set grid
set logscale x
set style data linespoints
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set style line 3  linetype 3 linewidth 2.000 pointsize 1
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set yrange [-2 to 10]
set terminal pdf color 
set output "../GNU/Mega8cap.pdf"
set xlabel "Capacity value / F"
set ylabel "Error / \%"
plot "../GNU/capmess8.dat" u 3:($4-$3)/$3*100 title "Mega8-1" ls 1,\
     "../GNU/capmess8.dat" u 3:($5-$3)/$3*100 title "Mega8-2" ls 2,\
     "../GNU/capmess8.dat" u 3:($6-$3)/$3*100 title "Mega8-3" ls 3,\
     "../GNU/capmessorig.dat" u 3:($4*0.88-$3)/$3*100 title "orig" ls 4
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega8capCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega8capRU.pdf" ; replot ; set output
#
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega8Acap.pdf"
plot "../GNU/capmess8.dat" u 3:($7-$3)/$3*100 title "Mega8A-4" ls 1,\
     "../GNU/capmess8.dat" u 3:($8-$3)/$3*100 title "Mega8A-5" ls 2,\
     "../GNU/capmess8.dat" u 3:($9-$3)/$3*100 title "Mega8A-6" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega8AcapCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega8AcapRU.pdf" ; replot ; set output
#
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega8Lcap.pdf"
plot "../GNU/capmess8.dat" u 3:($10-$3)/$3*100 title "Mega8L-7" ls 1,\
     "../GNU/capmess8.dat" u 3:($11-$3)/$3*100 title "Mega8L-8" ls 2,\
     "../GNU/capmess8.dat" u 3:($12-$3)/$3*100 title "Mega8L-9" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega8LcapCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega8LcapRU.pdf" ; replot ; set output
#
#    EOF
