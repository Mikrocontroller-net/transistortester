unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set grid xtics nomxtics ytics nomytics noztics nomztics \
 nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 1.000
set key inside left top vertical Right noreverse enhanced autotitles nobox
set key noinvert samplen 4 spacing 1 width 0 height 0 
set key maxcolumns 0 maxrows 0
unset label
unset arrow
set style increment default
unset style line
unset style arrow
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set pointintervalbox 1
set encoding default
set view 60, 30, 1, 1
set samples 100, 100
set isosamples 10, 10
unset contour
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
set zero 1e-08
set palette positive nops_allcF maxcolors 0 gamma 1.5 color model RGB 
set palette rgbformulae 7, 5, 15
set colorbox default
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
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
set ylabel "Error / Percent"
set yrange [-2 to 10]
set output "../GNU/Mega168cap.pdf"
#plot "../GNU/capmess168.dat" u 3:($4-$3)/$3*100 title "Mega168" ls 1,"../GNU/capmess168.dat" u 3:(($5-$3)-12e-12+(4000e-24/($5+259e-12)))/$3*100 title "Mega168as8" ls 2
plot "../GNU/capmess168.dat" u 3:($4-$3)/$3*100 title "Mega168" ls 1,"../GNU/capmess168.dat" u 3:($5-$3)/$3*100 title "Mega168as8" ls 2
set output
set output "../GNU/capcompare.pdf"
set autoscale y
#plot "../GNU/capmess168.dat" u 1:($2-$1)/$1*100 title "Multimeter" ls 1,"../GNU/capmess168.dat" u 1:($3-$1)/$1*100 title "LCR" ls 2,"../GNU/capmess168.dat" u 1:($5-$1)/$1*100 title "Mega168as" ls 3
plot "../GNU/capmess168.dat" u 1:($2-$6)/$6*100 title "Multimeter" ls 1,"../GNU/capmess168.dat" u 1:($3-$6)/$6*100 title "LCR" ls 2,"../GNU/capmess168.dat" u 1:($5-$6)/$6*100 title "Mega168as" ls 3
set output
#    EOF
