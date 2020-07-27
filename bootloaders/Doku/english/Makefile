LISTE3 = ../FIG/boot_pages.eps 	../FIG/avr_block.eps ../FIG/port.eps ../FIG/ablauf.eps
LISTE3 += ../FIG/VectorMove.eps ../FIG/ISP.eps ../FIG/UM232.eps
LISTE3 += ../FIG/uart_putch_timing.eps ../FIG/uart_getch_timing.eps
LISTE3 += ../FIG/OneWire.eps ../FIG/OneWire2.eps ../FIG/OneWire_simple.eps ../FIG/AutoBaud.eps

all: bootloader.pdf
	
#../GNU/example.eps: ../GNU/example.gnu ../GNU/example.dat
#	gnuplot ../GNU/example.gnu

../FIG/boot_pages.eps: ../FIG/boot_pages.fig
	fig2eps $<
../FIG/port.eps: ../FIG/port.fig
	fig2eps $<
../FIG/avr_block.eps: ../FIG/avr_block.fig
	fig2eps $<
../FIG/ablauf.eps: ../FIG/ablauf.fig
	fig2eps $<
../FIG/ISP.eps: ../FIG/ISP.fig
	fig2eps $<
../FIG/UM232.eps: ../FIG/UM232.fig
	fig2eps $<
../FIG/VectorMove.eps: ../FIG/VectorMove.fig
	fig2eps $<
../FIG/uart_getch_timing.eps: ../FIG/uart_getch_timing.fig
	fig2eps $<
../FIG/uart_putch_timing.eps: ../FIG/uart_putch_timing.fig
	fig2eps $<
../FIG/OneWire.eps: ../FIG/OneWire.fig
	fig2eps $<
../FIG/OneWire2.eps: ../FIG/OneWire2.fig
	fig2eps $<
../FIG/OneWire_simple.eps: ../FIG/OneWire_simple.fig
	fig2eps $<
../FIG/AutoBaud.eps: ../FIG/AutoBaud.fig
	fig2eps $<


TEXLIST3 = preface.tex principle.tex AVRhardware.tex optiboot.tex AVRdata.tex USBserial.tex

bootloader.pdf: bootloader.tex $(LISTE3) $(TEXLIST3)
	pdflatex --shell-escape bootloader.tex
	pdflatex --shell-escape bootloader.tex
clean:
	rm $(LISTE3)
