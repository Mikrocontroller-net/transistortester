unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set pointintervalbox 1
set encoding default
set samples 100, 100
set isosamples 10, 10
set surface
unset contour
set mapping cartesian
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
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set style line 3  linetype 3 linewidth 2.000 pointsize 1
set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / \%"
set output "../GNU/Mega168cap_optEN.pdf"
plot "../GNU/capmess168-3.dat" u 3:($4-$3)/$3*100 title "168-3-A" ls 1,"../GNU/capmess168-3.dat" u 3:($5-$3)/$3*100 title "168-3-B" ls 2,"../GNU/capmess168-3.dat" u 3:($6-$3)/$3*100 title "168-3-C" ls 3
set output
set xlabel "Capacity value / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168cap_optCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168cap_optRU.pdf" ; replot ; set output
#
set xlabel "Kapazitätswert / F"
set ylabel "Messfehler / \%"
set output "../GNU/Mega168cap_optGE.pdf" ; replot ; set output
#
#    EOF
