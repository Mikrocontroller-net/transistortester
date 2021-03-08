unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set dummy x,y
set format x "% g"
set format y "% g"
set key title ""
unset logscale
set offsets 0, 0, 0, 0
set pointsize 2
set samples 100, 100
set isosamples 10, 10
set mapping cartesian
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
set xrange [ 0.00000 : 10.0000 ] noreverse nowriteback
set ylabel "" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ * : * ] noreverse nowriteback  # (currently [-20.0000:70.0000] )
GNUTERM = "wxt"
set terminal pdf color 
set output "../GNU/ComparatorAdjust.pdf"
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set style line 3  linetype 3 linewidth 2.000 pointsize 1
set style line 4  linetype 4 linewidth 2.000 pointsize 1
set xlabel "Number of ATmega168"
set ylabel "Error / per mill"
set y2label "Difference to reference / mV"
plot "../GNU/comparator_cap.dat" u 1:2 title "diff2ref" ls 4, "../GNU/comparator_cap.dat" u 1:3 title "CapErr" ls 2, "../GNU/comparator_cap.dat" u 1:($3-$2) title "diff" ls 3
set output
set xlabel "číslo procesoru ATmega168"
set ylabel "Chyba / per mill"
set y2label "diference k referenci / mV"
set output "../GNU/ComparatorAdjustCZ.pdf" ; replot ; set output
#
set xlabel "Number of ATmega168"
set ylabel "Error / per mill"
set y2label "Difference to reference / mV"
set output "../GNU/ComparatorAdjustRU.pdf" ; replot ; set output
#
set xlabel "Nummer des ATmega168"
set ylabel "Fehler / Promille"
set y2label "Differenz zur Referenz / mV"
set output "../GNU/ComparatorAdjustGE.pdf" ; replot ; set output
#
#    EOF
