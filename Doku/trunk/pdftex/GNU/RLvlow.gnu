unset logscale
#set logscale x 10
set offsets 0, 0, 0, 0
set samples 100, 100
set size ratio 0 1,1
set origin 0,0
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set mxtics default
set mytics default
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set ytics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set grid
set xtics 1
set ytics 5
set key inside left top vertical Right noreverse enhanced autotitles nobox
set terminal pdf color 
set output "../GNU/RLvlow.pdf"
set style line 1  linetype 1 linewidth 2.000  pointsize 1
set style line 2  linetype 2 linewidth 2.000  pointsize 1
set xrange [ 0 : 10 ] noreverse nowriteback
set xlabel "resistor Rx / Ω" 
set yrange [130: 200 ] noreverse nowriteback 
set ylabel "voltage / mV" 
#plot (5000*699/(721+x)) ,(5000*(x+19)/(721+x))
plot (5000*(x+19)/(721+x)) title 'PC2, type 1' ls 1
set output
set xlabel "odpor Rx / Ω" 
set ylabel "napětí / mV" 
set output "../GNU/RLvlowCZ.pdf" ; replot ; set output
#
set xlabel "резистор Rx / Ω"
set ylabel "напряжений / mV"
set output "../GNU/RLvlowRU.pdf" ; replot ; set output
#
set xlabel "Widerstand Rx / Ω" 
set ylabel "Spannung / mV" 
set output "../GNU/RLvlowGE.pdf" ; replot ; set output
#
#    EOF
