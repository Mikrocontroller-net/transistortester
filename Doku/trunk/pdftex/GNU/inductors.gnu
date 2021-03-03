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
set style data points
set style function lines
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
set pointsize 3
set style data points
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 2 pointsize 1
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 3 pointsize 1
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 7 pointsize 1

set xlabel "Inductance value / H"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / Percent"
set key inside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/induct328p.pdf"
plot "../GNU/inductors.dat" u 1:($28-$1)/$1*100 title "328p" ls 1, "../GNU/inductors.dat" u 1:($30-$1)/$1*100 notitle ls 1 , "../GNU/inductors.dat" u 1:($32-$1)/$1*100 notitle ls 1,\
     "../GNU/inductors.dat" u 1:($22-$1)/$1*100 title "328" ls 4, "../GNU/inductors.dat" u 1:($24-$1)/$1*100 notitle ls 4 , "../GNU/inductors.dat" u 1:($26-$1)/$1*100 notitle ls 4,\
     "../GNU/inductors.dat" u 1:($16-$1)/$1*100 title "168p" ls 2, "../GNU/inductors.dat" u 1:($18-$1)/$1*100 notitle ls 2 , "../GNU/inductors.dat" u 1:($20-$1)/$1*100 notitle ls 2,\
     "../GNU/inductors.dat" u 1:($10-$1)/$1*100 title "168a" ls 5, "../GNU/inductors.dat" u 1:($12-$1)/$1*100 notitle ls 5 , "../GNU/inductors.dat" u 1:($14-$1)/$1*100 notitle ls 5,\
     "../GNU/inductors.dat" u 1:($4-$1)/$1*100 title "168" ls 6, "../GNU/inductors.dat" u 1:($6-$1)/$1*100 notitle ls 6 , "../GNU/inductors.dat" u 1:($8-$1)/$1*100 notitle ls 6
set output
#    EOF
