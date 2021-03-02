#!/usr/bin/gnuplot -persist
#
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 1
set view 60, 30, 1, 1
set samples 100, 100
set isosamples 10, 10
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set zzeroaxis linetype -2 linewidth 1.000
set x2zeroaxis linetype -2 linewidth 1.000
set y2zeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0 autojustify
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0 autojustify
set ytics autofreq  norangelimit
set title  offset character 0, 0, 0 font "" norotate
set xlabel "" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : * ] noreverse nowriteback
set ylabel "" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ * : * ] noreverse nowriteback
set psdir
# GNUTERM = "wxt"
set terminal epslatex color solid
set output "../GNU/frequency-ppm.eps"
set style line 1  linetype 1 linewidth 3.000 pointtype 4
set style line 2  linetype 2 linewidth 3.000 pointtype 2
set xlabel "frequency / Hz"
set ylabel "Error / ppm"
plot "../GNU/frequency-ppm.dat" u 1:($2-$1)/$1*1000000 title '2 Gen' ls 1, "../GNU/frequency-ppm.dat" u 1:($3-$1)/$1*1000000 title '1 Gen' ls 2
set output

#    EOF
