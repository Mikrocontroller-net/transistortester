set border 31 front linetype -1 linewidth 1.000
set key maxcolumns 0 maxrows 0
unset logscale
set pointsize 2
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
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
set ylabel "Error / \%"
#set yrange [-2 to 10]
set xrange [10.e-12 : 10.e-3 ]
set key inside center top vertical Right noreverse enhanced autotitles nobox
#
set output "../GNU/Mega328allEN.pdf"
plot "../GNU/capmess328-all.dat" u 3:($4-$3)/$3*100 title "328-10" ls 1,"../GNU/capmess328-all.dat" u 3:($5-$3)/$3*100 title "328-11" ls 2,"../GNU/capmess328-all.dat" u 3:($6-$3)/$3*100 title "168-12" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega328allCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega328allRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega328allGE.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega328PallEN.pdf"
plot "../GNU/capmess328-all.dat" u 3:($7-$3)/$3*100 title "328P-13" ls 1,"../GNU/capmess328-all.dat" u 3:($8-$3)/$3*100 title "328P-14" ls 2,"../GNU/capmess328-all.dat" u 3:($9-$3)/$3*100 title "328P-15" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega328PallCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega328PallRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega328PallGE.pdf" ; replot ; set output
#
# calibrated
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega328calEN.pdf"
plot "../GNU/capmess328-autocal.dat" u 3:($4-$3)/$3*100 title "328-10" ls 1,"../GNU/capmess328-autocal.dat" u 3:($5-$3)/$3*100 title "328-11" ls 2,"../GNU/capmess328-autocal.dat" u 3:($6-$3)/$3*100 title "328-12" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega328calCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega328calRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega328calGE.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega328PcalEN.pdf"
plot "../GNU/capmess328-autocal.dat" u 3:($7-$3)/$3*100 title "328P-13" ls 1,"../GNU/capmess328-autocal.dat" u 3:($8-$3)/$3*100 title "328P-14" ls 2,"../GNU/capmess328-autocal.dat" u 3:($9-$3)/$3*100 title "328P-15" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega328PcalCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega328PcalRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega328PcalGE.pdf" ; replot ; set output
#
#    EOF
