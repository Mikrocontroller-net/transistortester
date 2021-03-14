set key title ""
set pointsize 3
set pointintervalbox 3
set encoding default
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
set ylabel "" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
GNUTERM = "wxt"
set terminal pdf color
set grid
set style data points
set style line 1  linetype 1 linecolor rgb "red" linewidth 3.000  pointtype 4 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 3.000 pointtype 2 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 3.000  pointtype 6 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 3.000  pointtype 4 pointsize 1
set xrange [ 0.00000 : 16.0000 ] noreverse nowriteback
set xlabel "Processor number"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics 1  norangelimit
set ylabel "reference voltage / mV"
set key inside right top vertical Right noreverse enhanced autotitles nobox
set output "../GNU/SelfTrefEN.pdf"
set yrange [ 1050 : 1110 ] noreverse nowriteback 
plot "../GNU/selftest.dat" u 1:2 ls 4 title "Reference"
set output
set xlabel "číslo procesoru"
set ylabel "reference napětí / mV"
set output "../GNU/SelfTrefCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "Опорное напряжение / mV" 
set output "../GNU/SelfTrefRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Referenzspannung / mV"
set output "../GNU/SelfTrefGE.pdf" ; replot ; set output
#
set xlabel "Processor number"
set ylabel "voltage / mV"
set output "../GNU/SelfTMitLEN.pdf"
set yrange [ -20 : 5 ] noreverse nowriteback 
plot "../GNU/selftest.dat" u 1:3 ls 4 title "RLmiddle12", "../GNU/selftest.dat" u 1:4 ls 2 title "RLmiddle13", "../GNU/selftest.dat" u 1:5 ls 3 title "RLmiddle23"
set output
set xlabel "číslo procesoru"
set ylabel "napětí / mV"
set output "../GNU/SelfTMitLCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "напряжений / mV" 
set output "../GNU/SelfTMitLRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Spannung / mV"
set output "../GNU/SelfTMitLGE.pdf" ; replot ; set output
#
#
set xlabel "Processor number"
set ylabel "voltage / mV"
set output "../GNU/SelfTMitHEN.pdf"
set yrange [ -20 : 5 ] noreverse nowriteback 
plot "../GNU/selftest.dat" u 1:6 ls 4 title "RHmiddle12", "../GNU/selftest.dat" u 1:7 ls 2 title "RHmiddle13", "../GNU/selftest.dat" u 1:8 ls 3 title "RHmiddle23"
set output
set xlabel "číslo procesoru"
set ylabel "napětí / mV"
set output "../GNU/SelfTMitHCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "напряжений / mV" 
set output "../GNU/SelfTMitHRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Spannung / mV"
set output "../GNU/SelfTMitHGE.pdf" ; replot ; set output
#
set xlabel "Processor number"
set ylabel "voltage / mV"
set output "../GNU/SelfTbottomHEN.pdf"
set yrange [ 0 : 5 ] noreverse nowriteback 
plot "../GNU/selftest.dat" u 1:9 ls 4 title "RHbottom1", "../GNU/selftest.dat" u 1:10 ls 2 title "RHbottom2", "../GNU/selftest.dat" u 1:11 ls 3 title "RHbottom3"
set output
set xlabel "číslo procesoru"
set ylabel "napětí / mV"
set output "../GNU/SelfTbottomHCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "напряжений / mV" 
set output "../GNU/SelfTbottomHRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Spannung / mV"
set output "../GNU/SelfTbottomHGE.pdf" ; replot ; set output
#
set autoscale y
set xlabel "Processor number"
set ylabel "(voltage - VCC) / mV"
set output "../GNU/SelfTtopHEN.pdf"
plot "../GNU/selftest.dat" u 1:12 ls 4 title "RHtop1", "../GNU/selftest.dat" u 1:13 ls 2 title "RHtop2", "../GNU/selftest.dat" u 1:14 ls 3 title "RHtop3"
set output
set xlabel "číslo procesoru"
set ylabel "(napětí - VCC) / mV"
set output "../GNU/SelfTtopHCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "(напряжений - VCC) / mV" 
set output "../GNU/SelfTtopHRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "(Spannung - VCC)/ mV"
set output "../GNU/SelfTtopHGE.pdf" ; replot ; set output
#
set yrange [19 to 24]
set xlabel "Processor number"
set ylabel "resistance / Ω"
set output "../GNU/SelfTRiLoEN.pdf"
plot "../GNU/selftest.dat" u 1:(($15*680)/(5001 - $15 - $18)) ls 4 title "RiLo1", \
     "../GNU/selftest.dat" u 1:(($16*680)/(5001 - $16 - $19)) ls 2 title "RiLo2", \
     "../GNU/selftest.dat" u 1:(($17*680)/(5001 - $17 - $20)) ls 3 title "RiLo3"
set output
set xlabel "číslo procesoru"
set ylabel "odpor / Ω"
set output "../GNU/SelfTRiLoCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "сопротивления / Ω"
set output "../GNU/SelfTRiLoRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Widerstand / Ω"
set output "../GNU/SelfTRiLoGE.pdf" ; replot ; set output
#
set xlabel "Processor number"
set ylabel "resistance / Ω"
set output "../GNU/SelfTRiHiEN.pdf"
plot "../GNU/selftest.dat" u 1:(($18*680)/(5001 - $15 - $18)) ls 4 title "RiHi1", \
     "../GNU/selftest.dat" u 1:(($19*680)/(5001 - $16 - $19)) ls 2 title "RiHi2", \
     "../GNU/selftest.dat" u 1:(($20*680)/(5001 - $17 - $20)) ls 3 title "RiHi3"
set output
set xlabel "číslo procesoru"
set ylabel "odpor / Ω"
set output "../GNU/SelfTRiHiCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "сопротивления / Ω"
set output "../GNU/SelfTRiHiRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Widerstand / Ω"
set output "../GNU/SelfTRiHiGE.pdf" ; replot ; set output
#
set autoscale y
set xlabel "Processor number"
set ylabel "Capacity / pF"
set output "../GNU/SelfTcap0EN.pdf"
plot "../GNU/selftest.dat" u 1:21 ls 4 title "CNULL1", "../GNU/selftest.dat" u 1:22 ls 2 title "CNULL2", "../GNU/selftest.dat" u 1:23 ls 3 title "CNULL3"
set output
set xlabel "číslo procesoru"
set ylabel "kapacita / pF"
set output "../GNU/SelfTcap0CZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "значение ёмкости / pF"
set output "../GNU/SelfTcap0RU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Kapazität / pF"
set output "../GNU/SelfTcap0GE.pdf" ; replot ; set output
#
set xlabel "Processor number"
set output "../GNU/SelfTrefKorrEN.pdf"
set ylabel "Voltage correction / mV"
plot "../GNU/selftest.dat" u 1:24 ls 4 title "REF\\_C\\_KORR", "../GNU/selftest.dat" u 1:25 ls 2 title "REF\\_R\\_KORR"
set output
set xlabel "číslo procesoru"
set ylabel "korekce napětí / mV"
set output "../GNU/SelfTrefKorrCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "Коррекция напряжений / mV" 
set output "../GNU/SelfTrefKorrRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Spannungskorrektur / mV"
set output "../GNU/SelfTrefKorrGE.pdf" ; replot ; set output
#
set xlabel "Processor number"
set ylabel "Voltage difference / mV"
set output "../GNU/SelfTrefDiffEN.pdf"
set yrange [ -40 : 40 ] 
plot "../GNU/selftest.dat" u 1:($26-$2) ls 4 title "AREF - REF", "../GNU/selftest.dat" u 1:25 ls 2 title "REF\\_R\\_KORR"
set output
set xlabel "číslo procesoru"
set ylabel "diference napětí / mV"
set output "../GNU/SelfTrefDiffCZ.pdf" ; replot ; set output
#
set xlabel "Номер процессора"
set ylabel "Разность напряжений / mV" 
set output "../GNU/SelfTrefDiffRU.pdf" ; replot ; set output
#
set xlabel "Prozessornummer"
set ylabel "Spannungsdifferenz / mV"
set output "../GNU/SelfTrefDiffGE.pdf" ; replot ; set output
#
#    EOF
