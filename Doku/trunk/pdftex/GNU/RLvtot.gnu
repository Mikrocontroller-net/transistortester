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
set key at 1,2800 left center Right nobox 
set grid
set xrange [ 0.1 : 100000 ] noreverse nowriteback
set format x "%.0s%c"
set xlabel "resistor Rx / Ohm" 
set yrange [0: 5000 ] noreverse nowriteback 
set ylabel "voltage / mV" 
set terminal epslatex color solid
set output "../GNU/RLvtot.eps"
#set terminal pdfcairo color linewidth 0.5
#set output "../GNU/RLvtot.pdf"
set style line 1  linetype 1 linewidth 3.000 
set style line 2  linetype 2 linewidth 3.000 
plot (5000*(x+19)/(721+x)) title 'PC2, type 1' ls 1, (5000*699/(721+x)) title 'PC0, type 2' ls 2
set output
#    EOF
