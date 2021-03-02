LISTE1 = ../GNU/RLvtot.eps ../GNU/RLvlow.eps ../GNU/RLvhigh.eps ../GNU/RHv.eps ../GNU/Mega8res.eps
LISTE1 += ../GNU/Mega8cap.eps ../GNU/Mega168cap.eps ../GNU/Mega168cap_opt.eps ../GNU/Mega168all.eps 
LISTE1 += ../GNU/ComparatorAdjust.pdf ../GNU/Mega328all.eps ../GNU/Mega1284.eps ../GNU/SelfTref.eps ../GNU/MegaAuto.eps
LISTE1 += ../GNU/m168res_all.eps ../GNU/induct328p.eps ../GNU/Cesr.pdf ../GNU/Cesr2.pdf ../GNU/res_esr.eps
LISTE1 += ../GNU/Cesr_22n.pdf ../GNU/Cesr_22n_low.pdf
LISTE1 += ../GNU/Elco_esr.pdf ../GNU/frequency-ppm.eps
LISTE1 += ../GNU/Cap_FC_data.pdf ../GNU/Cap_FR_data.pdf ../GNU/Cap_FC_FR_data.pdf
LISTE1 += ../GNU/W12compare.eps
LISTE1 += ../FIG/port.pdf ../FIG/ResistormessH1.pdf ../FIG/ResistormessH2.pdf ../FIG/ResistormessL1.pdf
LISTE1 += ../FIG/ResistormessL2.pdf ../FIG/terminal.pdf
LISTE1 += ../FIG/Comparat.pdf ../FIG/Test2.pdf ../FIG/Test7.pdf ../FIG/Test8.pdf ../FIG/Bigcap.pdf ../FIG/Cap_residV.pdf
LISTE1 += ../FIG/PNPcc.pdf ../FIG/PNPce.pdf ../FIG/NPNce.pdf ../FIG/ttester.pdf ../FIG/t644tester.pdf ../FIG/relay_addon.pdf
LISTE1 += ../FIG/relay_um_addon.pdf ../FIG/diode_addon.pdf ../FIG/zener_exp.pdf ../FIG/Frequency_addon.pdf ../FIG/t1280tester.pdf
LISTE1 += ../FIG/Inductance.pdf ../FIG/JFETcd.pdf ../FIG/Cap_equiv.pdf ../FIG/Cap_esr.pdf ../FIG/Cap_esr2.pdf ../FIG/Cap_esr2_timing.pdf
LISTE1 += ../FIG/CheckSemi1.pdf ../FIG/CheckSemi2.pdf ../FIG/CheckSemi3.pdf
LISTE1 += ../FIG/rotary_extension.pdf ../FIG/rotary_encoder.pdf	../FIG/rotary_bouncing.pdf
LISTE1 += ../FIG/Fish8840.pdf ../FIG/ST7565lcd.pdf ../FIG/ST7920interface.pdf ../FIG/ST7108serial164.pdf
LISTE1 += ../FIG/ST7108serial595.pdf ../FIG/ST7108serial164_644.pdf ../FIG/ST7108serial595_644.pdf ../FIG/SSD1306_I2C.pdf
LISTE1 += ../FIG/sampling.pdf 

LISTE2 = ../FIG/BJT_AD161.pdf ../FIG/BJT_AD162.pdf ../FIG/BJT_BC639.pdf ../FIG/BJT_BC640.pdf ../FIG/BJT_BU508D.pdf
LISTE2 += ../FIG/BJT_BC516.pdf ../FIG/BJT_BC517.pdf ../FIG/BJT_BDV64.pdf ../FIG/BJT_BDV65.pdf
LISTE2 += ../FIG/JFET_BF245.pdf ../FIG/JFET_J175.pdf ../FIG/MOS_BSS169.pdf ../FIG/MOS_BUZ71.pdf ../FIG/MOS_IRF5305.pdf
LISTE2 += ../FIG/GPS_UP501.pdf ../FIG/GPS_GNS701.pdf

all: ttester.pdf ttinfo.pdf
	
../GNU/RLvtot.eps: ../GNU/RLvtot.gnu
	gnuplot $<
../GNU/RLvlow.eps: ../GNU/RLvlow.gnu
	gnuplot $<
../GNU/RLvhigh.eps: ../GNU/RLvhigh.gnu
	gnuplot $<
../GNU/RHv.eps: ../GNU/RHv.gnu
	gnuplot $<
../GNU/Mega8res.eps: ../GNU/rmess.gnu ../GNU/rmess.dat
	gnuplot ../GNU/rmess.gnu
../GNU/Mega8cap.eps: ../GNU/capmess8.gnu ../GNU/capmess8.dat ../GNU/capmessorig.dat
	gnuplot ../GNU/capmess8.gnu
../GNU/Mega168cap.eps: ../GNU/capmess168.gnu ../GNU/capmess168.dat
	gnuplot ../GNU/capmess168.gnu
../GNU/Mega168cap_opt.eps: ../GNU/capmessopt168.gnu ../GNU/capmess168-3.dat
	gnuplot ../GNU/capmessopt168.gnu
../GNU/Mega168all.eps: ../GNU/capmess168all.gnu ../GNU/capmess168-all.dat ../GNU/capmess168-autocal.dat
	gnuplot ../GNU/capmess168all.gnu
../GNU/Mega328all.eps: ../GNU/capmess328all.gnu ../GNU/capmess328-all.dat ../GNU/capmess328-autocal.dat
	gnuplot ../GNU/capmess328all.gnu
../GNU/Mega1284.eps: ../GNU/capmess1284.gnu ../GNU/capmess1284.dat
	gnuplot ../GNU/capmess1284.gnu
../GNU/ComparatorAdjust.pdf: ../GNU/comparator_diff.gnu ../GNU/comparator_cap.dat
	gnuplot ../GNU/comparator_diff.gnu
../GNU/SelfTref.eps: ../GNU/selftest.gnu ../GNU/selftest.dat
	gnuplot ../GNU/selftest.gnu
../GNU/MegaAuto.eps: ../GNU/capmessAUTO.gnu ../GNU/capmessAUTO.dat
	gnuplot ../GNU/capmessAUTO.gnu
../GNU/m168res_all.eps: ../GNU/rmess_all.gnu ../GNU/rmess-all.dat ../GNU/rmess-autocal.dat
	gnuplot ../GNU/rmess_all.gnu
../GNU/induct328p.eps: ../GNU/inductors.gnu ../GNU/inductors.dat
	gnuplot ../GNU/inductors.gnu
../GNU/Cesr.pdf: ../GNU/Cesr.gnu ../GNU/Cesr_all.dat
	gnuplot ../GNU/Cesr.gnu
../GNU/Cesr2.pdf: ../GNU/Cesr2.gnu ../GNU/Cesr2_all.dat
	gnuplot ../GNU/Cesr2.gnu
../GNU/Cesr_22n.pdf: ../GNU/Cesr_22n.gnu ../GNU/Cesr_22n.dat
	gnuplot ../GNU/Cesr_22n.gnu
../GNU/Cesr_22n_low.pdf: ../GNU/Cesr_22n_low.gnu ../GNU/Cesr_22n_low.dat
	gnuplot ../GNU/Cesr_22n_low.gnu
../GNU/res_esr.eps: ../GNU/rmess_esr.gnu ../GNU/rmess-esr.dat
	gnuplot ../GNU/rmess_esr.gnu
../GNU/Elco_esr.pdf: ../GNU/Elco_esr.gnu ../GNU/Elco_esr.dat
	gnuplot ../GNU/Elco_esr.gnu
../GNU/frequency-ppm.eps: ../GNU/frequency-ppm.gnu ../GNU/frequency-ppm.dat
	gnuplot ../GNU/frequency-ppm.gnu
../GNU/Cap_FC_data.pdf: ../GNU/Cap_FC_data.gnu ../GNU/Cap6_FC_ESR.dat ../GNU/Cap16_FC_ESR.dat ../GNU/Cap35_FC_ESR.dat ../GNU/Cap63_FC_ESR.dat
	gnuplot ../GNU/Cap_FC_data.gnu
../GNU/Cap_FR_data.pdf: ../GNU/Cap_FR_data.gnu ../GNU/Cap6_FR_ESR.dat ../GNU/Cap16_FR_ESR.dat ../GNU/Cap35_FR_ESR.dat ../GNU/Cap63_FR_ESR.dat
	gnuplot ../GNU/Cap_FR_data.gnu
../GNU/Cap_FC_FR_data.pdf: ../GNU/Cap_FC_FR_data.gnu ../GNU/Cap25_FC_ESR.dat ../GNU/Cap25_FR_ESR.dat
	gnuplot ../GNU/Cap_FC_FR_data.gnu
../GNU/W12compare.eps: ../GNU/W12compare.gnu ../GNU/W12_18nF.dat ../GNU/W12_20nF.dat
	gnuplot ../GNU/W12compare.gnu

../FIG/port.pdf: ../FIG/port.fig
	fig2pdf --nogv $<
../FIG/terminal.pdf: ../FIG/terminal.fig
	fig2pdf --nogv $<
../FIG/ResistormessH1.pdf: ../FIG/ResistormessH1.fig
	fig2pdf --nogv $<
../FIG/ResistormessH2.pdf: ../FIG/ResistormessH2.fig
	fig2pdf --nogv $<
../FIG/ResistormessL1.pdf: ../FIG/ResistormessL1.fig
	fig2pdf --nogv $<
../FIG/ResistormessL2.pdf: ../FIG/ResistormessL2.fig
	fig2pdf --nogv $<
../FIG/Bigcap.pdf: ../FIG/Bigcap.fig
	fig2pdf --nogv $<
../FIG/Cap_residV.pdf: ../FIG/Cap_residV.fig
	fig2pdf --nogv $<
../FIG/Comparat.pdf: ../FIG/Comparat.fig
	fig2pdf --nogv $<
../FIG/Test2.pdf: ../FIG/Test2.fig
	fig2pdf --nogv $<
../FIG/Test7.pdf: ../FIG/Test7.fig
	fig2pdf --nogv $<
../FIG/Test8.pdf: ../FIG/Test8.fig
	fig2pdf --nogv $<
../FIG/PNPcc.pdf: ../FIG/PNPcc.fig
	fig2pdf --nogv $<
../FIG/PNPce.pdf: ../FIG/PNPce.fig
	fig2pdf --nogv $<
../FIG/NPNce.pdf: ../FIG/NPNce.fig
	fig2pdf --nogv $<
../FIG/JFETcd.pdf: ../FIG/JFETcd.fig
	fig2pdf --nogv $<
../FIG/ttester.pdf: ../FIG/ttester.fig
	fig2pdf --nogv $<
../FIG/t644tester.pdf: ../FIG/t644tester.fig
	fig2pdf --nogv $<
../FIG/t1280tester.pdf: ../FIG/t1280tester.fig
	fig2pdf --nogv $<
../FIG/relay_addon.pdf: ../FIG/relay_addon.fig
	fig2pdf --nogv $<
../FIG/relay_um_addon.pdf: ../FIG/relay_um_addon.fig
	fig2pdf --nogv $<
../FIG/diode_addon.pdf: ../FIG/diode_addon.fig
	fig2pdf --nogv $<
../FIG/Frequency_addon.pdf: ../FIG/Frequency_addon.fig
	fig2pdf --nogv $<
../FIG/zener_exp.pdf: ../FIG/zener_exp.fig
	fig2pdf --nogv $<
../FIG/Inductance.pdf: ../FIG/Inductance.fig
	fig2pdf --nogv $<
../FIG/Cap_equiv.pdf: ../FIG/Cap_equiv.fig
	fig2pdf --nogv $<
../FIG/Cap_esr.pdf: ../FIG/Cap_esr.fig
	fig2pdf --nogv $<
../FIG/Cap_esr2.pdf: ../FIG/Cap_esr2.fig
	fig2pdf --nogv $<
../FIG/Cap_esr2_timing.pdf: ../FIG/Cap_esr2_timing.fig
	fig2pdf --nogv $<
../FIG/CheckSemi1.pdf: ../FIG/CheckSemi1.fig
	fig2pdf --nogv $<
../FIG/CheckSemi2.pdf: ../FIG/CheckSemi2.fig
	fig2pdf --nogv $<
../FIG/CheckSemi3.pdf: ../FIG/CheckSemi3.fig
	fig2pdf --nogv $<

../FIG/BJT_AD161.pdf: ../FIG/BJT_AD161.fig
	fig2pdf --nogv $<
../FIG/BJT_AD162.pdf: ../FIG/BJT_AD162.fig
	fig2pdf --nogv $<
../FIG/BJT_BC639.pdf: ../FIG/BJT_BC639.fig
	fig2pdf --nogv $<
../FIG/BJT_BC640.pdf: ../FIG/BJT_BC640.fig
	fig2pdf --nogv $<
../FIG/BJT_BU508D.pdf: ../FIG/BJT_BU508D.fig
	fig2pdf --nogv $<
../FIG/BJT_BC516.pdf: ../FIG/BJT_BC516.fig
	fig2pdf --nogv $<
../FIG/BJT_BC517.pdf: ../FIG/BJT_BC517.fig
	fig2pdf --nogv $<
../FIG/BJT_BDV64.pdf: ../FIG/BJT_BDV64.fig
	fig2pdf --nogv $<
../FIG/BJT_BDV65.pdf: ../FIG/BJT_BDV65.fig
	fig2pdf --nogv $<
../FIG/JFET_BF245.pdf: ../FIG/JFET_BF245.fig
	fig2pdf --nogv $<
../FIG/JFET_J175.pdf: ../FIG/JFET_J175.fig
	fig2pdf --nogv $<
../FIG/MOS_BSS169.pdf: ../FIG/MOS_BSS169.fig
	fig2pdf --nogv $<
../FIG/MOS_BUZ71.pdf: ../FIG/MOS_BUZ71.fig
	fig2pdf --nogv $<
../FIG/MOS_IRF5305.pdf: ../FIG/MOS_IRF5305.fig
	fig2pdf --nogv $<
../FIG/GPS_UP501.pdf: ../FIG/GPS_UP501.fig
	fig2pdf --nogv $<
../FIG/GPS_GNS701.pdf: ../FIG/GPS_GNS701.fig
	fig2pdf --nogv $<
../FIG/rotary_encoder.pdf: ../FIG/rotary_encoder.fig
	fig2pdf --nogv $<
../FIG/rotary_extension.pdf: ../FIG/rotary_extension.fig
	fig2pdf --nogv $<
../FIG/rotary_bouncing.pdf: ../FIG/rotary_bouncing.fig
	fig2pdf --nogv $<
../FIG/Fish8840.pdf: ../FIG/Fish8840.fig
	fig2pdf --nogv $<
../FIG/ST7565lcd.pdf: ../FIG/ST7565lcd.fig
	fig2pdf --nogv $<
../FIG/ST7920interface.pdf: ../FIG/ST7920interface.fig
	fig2pdf --nogv $<
../FIG/ST7108serial164.pdf: ../FIG/ST7108serial164.fig
	fig2pdf --nogv $<
../FIG/ST7108serial595.pdf: ../FIG/ST7108serial595.fig
	fig2pdf --nogv $<
../FIG/ST7108serial164_644.pdf: ../FIG/ST7108serial164_644.fig
	fig2pdf --nogv $<
../FIG/ST7108serial595_644.pdf: ../FIG/ST7108serial595_644.fig
	fig2pdf --nogv $<
../FIG/SSD1306_I2C.pdf: ../FIG/SSD1306_I2C.fig
	fig2pdf --nogv $<
../FIG/sampling.pdf: ../FIG/sampling.fig
	fig2pdf --nogv $<

TEXLIST1 =  00-preface.tex 10-features.tex 20-hardware.tex 30-manual.tex 40-configuring.tex
TEXLIST1 += 50-measurement.tex 51-semicon.tex 52-resistors.tex 53-capacitors.tex 54-inductors.tex 55-selftest.tex 56-frequency.tex
TEXLIST1 += 60-generators.tex 70-errors.tex 80-special.tex 90-todo.tex

TEXLIST2 = BJT-transistors.tex MOS-transistors.tex

ttester.pdf: ttester.tex $(LISTE1) $(TEXLIST1)
	pdflatex --shell-escape ttester.tex
	pdflatex --shell-escape ttester.tex

ttinfo.pdf: ttinfo.tex $(LISTE2) $(TEXLIST2)
	pdflatex --shell-escape ttinfo.tex
	pdflatex --shell-escape ttinfo.tex
clean:
	rm $(LISTE1) $(LISTE2)
