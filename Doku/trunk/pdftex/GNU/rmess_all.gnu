unset logscale
set logscale x 10
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
#set ytics autofreq  norangelimit
set ytics 1  norangelimit
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ -5.00000 : 5.00000 ] noreverse nowriteback
set xrange [ 1 : 100000000 ]
GNUTERM = "wxt"
set terminal pdf color 
set grid
set logscale x
set style data linespoints
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 2 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 3 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 2 pointsize 1
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 3 pointsize 1

set xlabel "Resistor value / Ω"
set format x "%.0s%c"
set format y "% g"
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ylabel "Error / \%"
set output "../GNU/m168res_allEN.pdf"
plot "../GNU/rmess-all.dat" u 1:(($2-$1)/$1*100) title "m168-1" ls 1 with points,\
     "../GNU/rmess-all.dat" u 1:(($3-$1)/$1*100) title "m168-2" ls 2 with points,\
     "../GNU/rmess-all.dat" u 1:(($4-$1)/$1*100) title "m168-3" ls 3 with points,\
     "../GNU/rmess-autocal.dat" u 1:(($2-$1)/$1*100) title "m168-1" ls 4 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($3-$1)/$1*100) title "m168-2" ls 5 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($4-$1)/$1*100) title "m168-3" ls 6 with lines
set output
set xlabel "hodnota odporu / Ω"
set ylabel "chyba / \%"
set output "../GNU/m168res_allCZ.pdf" ; replot ; set output
#
set xlabel "Значение сопротивления / Ω"
set ylabel "ошибка / \%"
set output "../GNU/m168res_allRU.pdf" ; replot ; set output
#
set xlabel "Widerstandswert / Ω"
set ylabel "Messfehler / \%"
set output "../GNU/m168res_allGE.pdf" ; replot ; set output
#
set xlabel "Resistor value / Ω"
set ylabel "Error / \%"
set output "../GNU/m168ares_allEN.pdf"
plot "../GNU/rmess-all.dat" u 1:(($5-$1)/$1*100) title "m168a-4" ls 1 with points,\
     "../GNU/rmess-all.dat" u 1:(($6-$1)/$1*100) title "m168a-5" ls 2 with points,\
     "../GNU/rmess-all.dat" u 1:(($7-$1)/$1*100) title "m168a-6" ls 3 with points,\
     "../GNU/rmess-autocal.dat" u 1:(($5-$1)/$1*100) title "m168a-4" ls 4 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($6-$1)/$1*100) title "m168a-5" ls 5 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($7-$1)/$1*100) title "m168a-6" ls 6 with lines
set output
set xlabel "hodnota odporu / Ω"
set ylabel "chyba / \%"
set output "../GNU/m168ares_allCZ.pdf" ; replot ; set output
#
set xlabel "Значение сопротивления / Ω"
set ylabel "ошибка / \%"
set output "../GNU/m168ares_allRU.pdf" ; replot ; set output
#
set xlabel "Widerstandswert / Ω"
set ylabel "Messfehler / \%"
set output "../GNU/m168ares_allGE.pdf" ; replot ; set output
#
set xlabel "Resistor value / Ω"
set ylabel "Error / \%"
set output "../GNU/m168pres_allEN.pdf"
plot "../GNU/rmess-all.dat" u 1:(($8-$1)/$1*100) title "m168p-7" ls 1 with points,\
     "../GNU/rmess-all.dat" u 1:(($9-$1)/$1*100) title "m168p-8" ls 2 with points,\
     "../GNU/rmess-all.dat" u 1:(($10-$1)/$1*100) title "m168p-9" ls 3 with points,\
     "../GNU/rmess-autocal.dat" u 1:(($8-$1)/$1*100) title "m168p-7" ls 4 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($9-$1)/$1*100) title "m168p-8" ls 5 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($10-$1)/$1*100) title "m168p-9" ls 6 with lines
set output
set xlabel "hodnota odporu / Ω"
set ylabel "chyba / \%"
set output "../GNU/m168pres_allCZ.pdf" ; replot ; set output
#
set xlabel "Значение сопротивления / Ω"
set ylabel "ошибка / \%"
set output "../GNU/m168pres_allRU.pdf" ; replot ; set output
#
set xlabel "Widerstandswert / Ω"
set ylabel "Messfehler / \%"
set output "../GNU/m168pres_allGE.pdf" ; replot ; set output
#
set xlabel "Resistor value / Ω"
set ylabel "Error / \%"
set output "../GNU/m328res_allEN.pdf"
plot "../GNU/rmess-all.dat" u 1:(($11-$1)/$1*100) title "m328-10" ls 1 with points,\
     "../GNU/rmess-all.dat" u 1:(($12-$1)/$1*100) title "m328-11" ls 2 with points,\
     "../GNU/rmess-all.dat" u 1:(($13-$1)/$1*100) title "m328-12" ls 3 with points,\
     "../GNU/rmess-autocal.dat" u 1:(($11-$1)/$1*100) title "m328-10" ls 4 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($12-$1)/$1*100) title "m328-11" ls 5 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($13-$1)/$1*100) title "m328-12" ls 6 with lines
set output
set xlabel "hodnota odporu / Ω"
set ylabel "chyba / \%"
set output "../GNU/m328res_allCZ.pdf" ; replot ; set output
#
set xlabel "Значение сопротивления / Ω"
set ylabel "ошибка / \%"
set output "../GNU/m328res_allRU.pdf" ; replot ; set output
#
set xlabel "Widerstandswert / Ω"
set ylabel "Messfehler / \%"
set output "../GNU/m328res_allGE.pdf" ; replot ; set output
#
set xlabel "Resistor value / Ω"
set ylabel "Error / \%"
set output "../GNU/m328pres_allEN.pdf"
plot "../GNU/rmess-all.dat" u 1:(($14-$1)/$1*100) title "m328p-13" ls 1 with points,\
     "../GNU/rmess-all.dat" u 1:(($15-$1)/$1*100) title "m328p-14" ls 2 with points,\
     "../GNU/rmess-all.dat" u 1:(($16-$1)/$1*100) title "m328p-15" ls 3 with points,\
     "../GNU/rmess-autocal.dat" u 1:(($14-$1)/$1*100) title "m328p-13" ls 4 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($15-$1)/$1*100) title "m328p-14" ls 5 with lines,\
     "../GNU/rmess-autocal.dat" u 1:(($16-$1)/$1*100) title "m328p-15" ls 6 with lines
set output
set xlabel "hodnota odporu / Ω"
set ylabel "chyba / \%"
set output "../GNU/m328pres_allCZ.pdf" ; replot ; set output
#
set xlabel "Значение сопротивления / Ω"
set ylabel "ошибка / \%"
set output "../GNU/m328pres_allRU.pdf" ; replot ; set output
#
set xlabel "Widerstandswert / Ω"
set ylabel "Messfehler / \%"
set output "../GNU/m328pres_allGE.pdf" ; replot ; set output
#
##################################################################################
#set output "../GNU/m168res_korrEN.eps"
#plot "../GNU/rmess-korr.dat" u 1:(($2-$1)/$1*100) title "m168-1" ls 1 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($3-$1)/$1*100) title "m168-2" ls 2 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($4-$1)/$1*100) title "m168-3" ls 3 with points,\
#     "../GNU/rmess-autocal.dat" u 1:(($2-$1)/$1*100) title "m168-1" ls 4 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($3-$1)/$1*100) title "m168-2" ls 5 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($4-$1)/$1*100) title "m168-3" ls 6 with lines
#set output
#set output "../GNU/m168ares_korrEN.eps"
#plot "../GNU/rmess-korr.dat" u 1:(($5-$1)/$1*100) title "m168a-4" ls 1 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($6-$1)/$1*100) title "m168a-5" ls 2 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($7-$1)/$1*100) title "m168a-6" ls 3 with points,\
#     "../GNU/rmess-autocal.dat" u 1:(($5-$1)/$1*100) title "m168a-4" ls 4 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($6-$1)/$1*100) title "m168a-5" ls 5 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($7-$1)/$1*100) title "m168a-6" ls 6 with lines
#set output
#set output "../GNU/m168pres_korrEN.eps"
#plot "../GNU/rmess-korr.dat" u 1:(($8-$1)/$1*100) title "m168p-7" ls 1 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($9-$1)/$1*100) title "m168p-8" ls 2 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($10-$1)/$1*100) title "m168p-9" ls 3 with points,\
#     "../GNU/rmess-autocal.dat" u 1:(($8-$1)/$1*100) title "m168p-7" ls 4 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($9-$1)/$1*100) title "m168p-8" ls 5 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($10-$1)/$1*100) title "m168p-9" ls 6 with lines
#set output
#set output "../GNU/m328res_korrEN.eps"
#plot "../GNU/rmess-korr.dat" u 1:(($11-$1)/$1*100) title "m328-10" ls 1 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($12-$1)/$1*100) title "m328-11" ls 2 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($13-$1)/$1*100) title "m328-12" ls 3 with points,\
#     "../GNU/rmess-autocal.dat" u 1:(($11-$1)/$1*100) title "m328-10" ls 4 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($12-$1)/$1*100) title "m328-11" ls 5 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($13-$1)/$1*100) title "m328-12" ls 6 with lines
#set output
#set output "../GNU/m328pres_korrEN.eps"
#plot "../GNU/rmess-korr.dat" u 1:(($14-$1)/$1*100) title "m328p-13" ls 1 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($15-$1)/$1*100) title "m328p-14" ls 2 with points,\
#     "../GNU/rmess-korr.dat" u 1:(($16-$1)/$1*100) title "m328p-15" ls 3 with points,\
#     "../GNU/rmess-autocal.dat" u 1:(($14-$1)/$1*100) title "m328p-13" ls 4 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($15-$1)/$1*100) title "m328p-14" ls 5 with lines,\
#     "../GNU/rmess-autocal.dat" u 1:(($16-$1)/$1*100) title "m328p-15" ls 6 with lines
#set output
#    EOF
