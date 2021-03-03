set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
unset grid
set key title ""
unset logscale
#set logscale x 10
set offsets 0, 0, 0, 0
set pointsize 1
set pointintervalbox 1
set encoding default
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set mxtics default
set mytics default
set mcbtics default
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set ytics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set timestamp bottom 
set timestamp "" 
set timestamp  offset character 0, 0, 0 font "" norotate
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set grid
set xtics 1
set ytics 5
set terminal pdf color 
set output "../GNU/RLvhigh.pdf"
set style line 1  linetype 1 linewidth 2.000 pointsize 1
set style line 2  linetype 2 linewidth 2.000 pointsize 1
set xrange [ 0 : 10 ] noreverse nowriteback
set xlabel "resistor Rx / Ohm" 
set yrange [4780: 4850 ] noreverse nowriteback 
set ylabel "voltage / mV" 
#plot (5000*699/(721+x)) ,(5000*(x+19)/(721+x))
plot (5000*(x+19)/(721+x)) notitle ls 1, (5000*699/(721+x)) title 'PC0, type 2' ls 2
set output
#    EOF
#    EOF
