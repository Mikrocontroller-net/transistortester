set key title ""
unset logscale
#set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set ytics autofreq  norangelimit
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : * ] noreverse nowriteback  # (currently [0.00000:8.00000] )
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set terminal epslatex color solid
set grid
set style data linespoints
set style line 1  linetype 1 linecolor rgb "red" linewidth 3.000  pointtype 4
set style line 2  linetype 1 linecolor rgb "green" linewidth 3.000 pointtype 4
set style line 3  linetype 1 linecolor rgb "blue" linewidth 3.000  pointtype 4
set style line 4  linetype 1 linecolor rgb "red" linewidth 3.000  pointtype 3
set style line 5  linetype 1 linecolor rgb "green" linewidth 3.000 pointtype 3
set style line 6  linetype 1 linecolor rgb "blue" linewidth 3.000  pointtype 3

set xlabel "Resistor value / Ohm"
#set format x "%.0s%c"
set format x "% g"
set format y "% g"
set xrange [ 0 : 15]
set yrange [ -0.20000 : 0.20000 ] noreverse nowriteback
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "difference / Ohm"
set output "../GNU/res_esr.eps"
plot "../GNU/rmess-esr.dat" u 1:($2-$1) title "m168" ls 1 with points,\
     "../GNU/rmess-esr.dat" u 1:($3-$1) notitle ls 1 with points,\
     "../GNU/rmess-esr.dat" u 1:($4-$1) notitle ls 1 with points,\
     "../GNU/rmess-esr.dat" u 1:($5-$1) title "m168a" ls 2 with points,\
     "../GNU/rmess-esr.dat" u 1:($6-$1) notitle ls 2 with points,\
     "../GNU/rmess-esr.dat" u 1:($7-$1) notitle ls 2 with points,\
     "../GNU/rmess-esr.dat" u 1:($8-$1) title "m168p" ls 3 with points,\
     "../GNU/rmess-esr.dat" u 1:($9-$1) notitle ls 3 with points,\
     "../GNU/rmess-esr.dat" u 1:($10-$1) notitle ls 3 with points,\
     "../GNU/rmess-esr.dat" u 1:($11-$1) title "m328" ls 4 with points,\
     "../GNU/rmess-esr.dat" u 1:($12-$1) notitle ls 4 with points,\
     "../GNU/rmess-esr.dat" u 1:($13-$1) notitle ls 4 with points,\
     "../GNU/rmess-esr.dat" u 1:($14-$1) title "m328p" ls 5 with points,\
     "../GNU/rmess-esr.dat" u 1:($15-$1) notitle ls 5 with points,\
     "../GNU/rmess-esr.dat" u 1:($16-$1) notitle ls 5 with points
set output
#    EOF
