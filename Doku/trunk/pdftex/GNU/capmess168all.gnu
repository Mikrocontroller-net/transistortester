unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set xdata
set ydata
set zdata
set boxwidth
set style fill  empty border
set style rectangle back fc lt -3 fillstyle   solid 1.00 border lt -1
set style circle radius graph 0.02, first 0, 0 
set dummy x,y
set format y "% g"
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
set view 60, 30, 1, 1
set samples 100, 100
set isosamples 10, 10
set surface
unset contour
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
set fit noerrorvariables
GNUTERM = "wxt"
set terminal epslatex color solid
set grid
set logscale x
set style data linespoints
set style line 1  linetype 1 linewidth 3.000 
set style line 2  linetype 2 linewidth 3.000 
set style line 3  linetype 3 linewidth 3.000 
set xlabel "Capacity value / F"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / Percent"
#set yrange [-2 to 10]
set key inside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega168all.eps"
plot "../GNU/capmess168-all.dat" u 3:($4-$3)/$3*100 title "168-1" ls 1,"../GNU/capmess168-all.dat" u 3:($5-$3)/$3*100 title "168-2" ls 2,"../GNU/capmess168-all.dat" u 3:($6-$3)/$3*100 title "168-3" ls 3
set output
set key inside at 1.e-6,5 vertical Right nobox
set output "../GNU/Mega168Aall.eps"
plot "../GNU/capmess168-all.dat" u 3:($7-$3)/$3*100 title "168A-4" ls 1,"../GNU/capmess168-all.dat" u 3:($8-$3)/$3*100 title "168A-5" ls 2,"../GNU/capmess168-all.dat" u 3:($9-$3)/$3*100 title "168A-6" ls 3
set output
set key inside at 1.e-6,9 vertical Right nobox
set output "../GNU/Mega168PAall.eps"
plot "../GNU/capmess168-all.dat" u 3:($10-$3)/$3*100 title "168PA-7" ls 1,"../GNU/capmess168-all.dat" u 3:($11-$3)/$3*100 title "168PA-8" ls 2,"../GNU/capmess168-all.dat" u 3:($12-$3)/$3*100 title "168PA-9" ls 3
set output
# calibrated
set key inside left top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega168cal.eps"
plot "../GNU/capmess168-autocal.dat" u 3:($4-$3)/$3*100 title "168-1" ls 1,"../GNU/capmess168-autocal.dat" u 3:($5-$3)/$3*100 title "168-2" ls 2,"../GNU/capmess168-autocal.dat" u 3:($6-$3)/$3*100 title "168-3" ls 3
set output
set key inside left top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega168Acal.eps"
plot "../GNU/capmess168-autocal.dat" u 3:($7-$3)/$3*100 title "168A-4" ls 1,"../GNU/capmess168-autocal.dat" u 3:($8-$3)/$3*100 title "168A-5" ls 2,"../GNU/capmess168-autocal.dat" u 3:($9-$3)/$3*100 title "168A-6" ls 3
set output
set key inside left top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega168PAcal.eps"
plot "../GNU/capmess168-autocal.dat" u 3:($10-$3)/$3*100 title "168PA-7" ls 1,"../GNU/capmess168-autocal.dat" u 3:($11-$3)/$3*100 title "168PA-8" ls 2,"../GNU/capmess168-autocal.dat" u 3:($12-$3)/$3*100 title "168PA-9" ls 3
set output
#    EOF
