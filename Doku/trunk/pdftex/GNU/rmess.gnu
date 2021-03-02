set key title ""
unset logscale
set logscale x 10
set pointsize 2
set size ratio 0 1,1
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
#set ytics autofreq  norangelimit
set ytics 1  norangelimit
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : * ] noreverse nowriteback  # (currently [0.00000:8.00000] )
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ -5.00000 : 5.00000 ] noreverse nowriteback
GNUTERM = "wxt"
set terminal epslatex color solid
set output "../GNU/Mega8res.eps"
set grid
set logscale x
set style data linespoints
set style line 1  linetype 1 linewidth 3.000 
set style line 2  linetype 2 linewidth 3.000 
set style line 3  linetype 3 linewidth 3.000 
set xlabel "Resistor value / Ohm"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / Percent"
plot "../GNU/rmess.dat" u 1:(($2-$1)/$1*100) title "Mega8-1" ls 1, "../GNU/rmess.dat" u 1:(($3-$1)/$1*100) title "Mega8-2" ls 2, "../GNU/rmess.dat" u 1:(($4-$1)/$1*100) title "Mega8-3" ls 3,\
"../GNU/ormess.dat" u 1:(($2-$1)/$1*100) title "Mega8orig" ls 4
set output
set output "../GNU/Mega8Ares.eps"
plot "../GNU/rmess.dat" u 1:(($5-$1)/$1*100) title "Mega8A-4" ls 1, "../GNU/rmess.dat" u 1:(($6-$1)/$1*100) title "Mega8A-5" ls 2, "../GNU/rmess.dat" u 1:(($7-$1)/$1*100) title "Mega8A-6" ls 3
set output
set output "../GNU/Mega8Lres.eps"
plot "../GNU/rmess.dat" u 1:(($8-$1)/$1*100) title "Mega8L-7" ls 1, "../GNU/rmess.dat" u 1:(($9-$1)/$1*100) title "Mega8L-8" ls 2, "../GNU/rmess.dat" u 1:(($10-$1)/$1*100) title "Mega8L-9" ls 3
set output
set output "../GNU/Mega168res.eps"
plot "../GNU/rmess.dat" u 1:(($11-$1)/$1*100) title "Mega168" ls 1, "../GNU/rmess.dat" u 1:(($12-$1)/$1*100) title "Mega168as" ls 2
set output
#    EOF
