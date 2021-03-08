set key title ""
set key noinvert samplen 4 spacing 1 width 0 height 0 
set key maxcolumns 0 maxrows 0
set style histogram clustered gap 2 title  offset character 0, 0, 0
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set title "" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set key inside center center vertical Right noreverse enhanced autotitles nobox
set grid
set xrange [ 0.1 : 100000 ] noreverse nowriteback
set format x "%.0s%c"
set xlabel "resistor Rx / Ω" 
set yrange [0: 5000 ] noreverse nowriteback 
set ylabel "voltage / mV" 
set terminal pdf color solid
#set terminal pdfcairo color linewidth 0.5
set output "../GNU/RLvtot.pdf"
set style line 1  linetype 1 linewidth 2.000 
set style line 2  linetype 2 linewidth 2.000 
plot (5000*(x+19)/(721+x)) title 'PC2, type 1' ls 1, (5000*699/(721+x)) title 'PC0, type 2' ls 2
set output
set xlabel "odpor Rx / Ω" 
set ylabel "napětí / mV" 
set output "../GNU/RLvtotCZ.pdf" ; replot ; set output
#
set xlabel "резистор Rx / Ω"
set ylabel "Напряжение / mV" 
set output "../GNU/RLvtotRU.pdf" ; replot ; set output
#
set xlabel "Widerstand Rx / Ω" 
set ylabel "Spannung / mV" 
set output "../GNU/RLvtotGE.pdf" ; replot ; set output
#
#    EOF
