set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 1.000
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set ytics autofreq  norangelimit
GNUTERM = "wxt"
set terminal pdf color 
set grid
set logscale x 10
set style data linespoints
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set style line 3  linetype 3 linewidth 2.000 pointsize 1
set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set xtics 10
set ylabel "Error / \%"
#set yrange [-2 to 10]
set xrange [10.e-12 : 20.e-3 ]
# calibrated
set key inside center top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega1284.pdf"
plot "../GNU/capmess1284.dat" u 3:($4-$3)/$3*100 title "328-10" ls 1,"../GNU/capmess1284.dat" u 3:($5-$3)/$3*100 title "1284-int" ls 2,"../GNU/capmess1284.dat" u 3:($6-$3)/$3*100 title "1284-ext" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega1284CZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega1284RU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega1284GE.pdf" ; replot ; set output
#
#    EOF
