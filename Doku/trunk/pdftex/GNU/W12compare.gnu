GNUTERM = "wxt"
set terminal pdf color 
set grid
set style data lines
set style line 1  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 4 pointsize 1
set style line 4  linetype 1 linecolor rgb "red" linewidth 2.000  pointtype 6 pointsize 1
set style line 2  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 4 pointsize 1
set style line 3  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 2 pointsize 0.5
set style line 5  linetype 1 linecolor rgb "green" linewidth 2.000 pointtype 7 pointsize 1
set style line 6  linetype 1 linecolor rgb "blue" linewidth 2.000  pointtype 7 pointsize 1
set style line 7  linetype 1 linecolor rgb "black" linewidth 2.000  pointtype 4 pointsize 2

set format x "% g"
set format y "% g"
set key inside right top vertical Right noreverse enhanced autotitles nobox
set xlabel "ADC sample"
set output "../GNU/W12compare.pdf"
set xrange [0 to 250]
set ylabel "ADC value"
plot "../GNU/W12_18nF.dat" u 1 title "18.2nF", "../GNU/W12_20nF.dat" u 1 title "20.3nF"
set output
set xlabel "vzorek ADC"
set ylabel "hodnota ADC"
set output "../GNU/W12compareCZ.pdf" ; replot ; set output
#
set xlabel "ADC sample"
set ylabel "ADC value"
set output "../GNU/W12compareRU.pdf" ; replot ; set output
#
set xlabel "ADC Folgenummer"
set ylabel "ADC Wert"
set output "../GNU/W12compareGE.pdf" ; replot ; set output
#
#    EOF
