unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set style fill  empty border
set style rectangle back fc lt -3 fillstyle   solid 1.00 border lt -1
set style circle radius graph 0.02, first 0, 0 
set format y "% g"
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 2
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
#set yrange [-2 to 10]
set key inside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/Mega168all.pdf"
plot "../GNU/capmess168-all.dat" u 3:($4-$3)/$3*100 title "168-1" ls 1,"../GNU/capmess168-all.dat" u 3:($5-$3)/$3*100 title "168-2" ls 2,"../GNU/capmess168-all.dat" u 3:($6-$3)/$3*100 title "168-3" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "погрешность / \%"
set output "../GNU/Mega168allCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168allRU.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega168Aall.pdf"
plot "../GNU/capmess168-all.dat" u 3:($7-$3)/$3*100 title "168A-4" ls 1,"../GNU/capmess168-all.dat" u 3:($8-$3)/$3*100 title "168A-5" ls 2,"../GNU/capmess168-all.dat" u 3:($9-$3)/$3*100 title "168A-6" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168AallCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168AallRU.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega168PAall.pdf"
plot "../GNU/capmess168-all.dat" u 3:($10-$3)/$3*100 title "168PA-7" ls 1,"../GNU/capmess168-all.dat" u 3:($11-$3)/$3*100 title "168PA-8" ls 2,"../GNU/capmess168-all.dat" u 3:($12-$3)/$3*100 title "168PA-9" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168PAallCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168PAallRU.pdf" ; replot ; set output
#
# calibrated
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega168cal.pdf"
plot "../GNU/capmess168-autocal.dat" u 3:($4-$3)/$3*100 title "168-1" ls 1,"../GNU/capmess168-autocal.dat" u 3:($5-$3)/$3*100 title "168-2" ls 2,"../GNU/capmess168-autocal.dat" u 3:($6-$3)/$3*100 title "168-3" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168calCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168calRU.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega168Acal.pdf"
plot "../GNU/capmess168-autocal.dat" u 3:($7-$3)/$3*100 title "168A-4" ls 1,"../GNU/capmess168-autocal.dat" u 3:($8-$3)/$3*100 title "168A-5" ls 2,"../GNU/capmess168-autocal.dat" u 3:($9-$3)/$3*100 title "168A-6" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168AcalCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168AcalRU.pdf" ; replot ; set output
#
set key inside center top vertical Right noreverse enhanced autotitles nobox
set xlabel "Capacity value / F"
set ylabel "Error / \%"
set output "../GNU/Mega168PAcal.pdf"
plot "../GNU/capmess168-autocal.dat" u 3:($10-$3)/$3*100 title "168PA-7" ls 1,"../GNU/capmess168-autocal.dat" u 3:($11-$3)/$3*100 title "168PA-8" ls 2,"../GNU/capmess168-autocal.dat" u 3:($12-$3)/$3*100 title "168PA-9" ls 3
set output
set xlabel "hodnota kapacity / F"
set ylabel "chyba / \%"
set output "../GNU/Mega168PAcalCZ.pdf" ; replot ; set output
#
set xlabel "Значение емкости / F" 
set ylabel "погрешность / \%"
set output "../GNU/Mega168PAcalRU.pdf" ; replot ; set output
#
#    EOF
