set pointsize 2
set pointintervalbox 1
set samples 100, 100
set isosamples 10, 10
set size ratio 0 1,1
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
GNUTERM = "wxt"
set terminal pdf color
set grid
set logscale x 10
set xtics 10
set yrange [-2 to 10]
set xrange [10.e-12 : 100.e-3]
set key inside center top vertical Right noreverse enhanced autotitles nobox
set style data linespoints
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set style line 3  linetype 3 linewidth 2.000 pointsize 1
set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set ylabel "Error / \%"
set output "../GNU/Mega168cap.pdf"
#plot "../GNU/capmess168.dat" u 3:($4-$3)/$3*100 title "Mega168" ls 1,"../GNU/capmess168.dat" u 3:(($5-$3)-12e-12+(4000e-24/($5+259e-12)))/$3*100 title "Mega168as8" ls 2
plot "../GNU/capmess168.dat" u 3:($4-$3)/$3*100 title "Mega168" ls 1,"../GNU/capmess168.dat" u 3:($5-$3)/$3*100 title "Mega168as8" ls 2
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168capCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168capRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega168capGE.pdf" ; replot ; set output
#
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/capcompare.pdf"
set autoscale y
#plot "../GNU/capmess168.dat" u 1:($2-$1)/$1*100 title "Multimeter" ls 1,"../GNU/capmess168.dat" u 1:($3-$1)/$1*100 title "LCR" ls 2,"../GNU/capmess168.dat" u 1:($5-$1)/$1*100 title "Mega168as" ls 3
plot "../GNU/capmess168.dat" u 1:($2-$6)/$6*100 title "Multimeter" ls 1,"../GNU/capmess168.dat" u 1:($3-$6)/$6*100 title "LCR" ls 2,"../GNU/capmess168.dat" u 1:($5-$6)/$6*100 title "Mega168as" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/capcompareCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/capcompareRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/capcompareGE.pdf" ; replot ; set output
#
#    EOF
