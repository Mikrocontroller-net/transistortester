# ************************************************************************
#  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
#  
#  File:       finisch.mk
#  Funktion:   late common tasks for the Makefile(s)
# 
# History:     Date        Sign    Kommentar   
#              2021-01-10  Karl    LastChar
#              2021-01-10  Karl    add the function in comment
#
# ************************************************************************ */
########### Compile only Assembler source available
$(OBJDIR)/lcd_hw_4_bit.o: ../lcd_hw_4_bit.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/wait1000ms.o: ../wait1000ms.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/swuart.o: ../swuart.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

########### Compile  Assembler source only, is time critical
$(OBJDIR)/GetESR.o: ../GetESR.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

########### Compile C source only 
$(OBJDIR)/lcd-routines.o: ../lcd-routines.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/lcd-draw.o: ../lcd-draw.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/DisplayValue.o: ../DisplayValue.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/function_menu.o: ../function_menu.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/message_key_released.o: ../message_key_released.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/ReadCapacity.o: ../ReadCapacity.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/CalibrationCap.o: ../CalibrationCap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/ReadBigCap.o: ../ReadBigCap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/ReadInductance.o: ../ReadInductance.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/Calibrate_UR.o: ../Calibrate_UR.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/CheckPins.o: ../CheckPins.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/GetResistance.o: ../GetResistance.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/GetVloss.o: ../GetVloss.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/GetFrequency.o: ../GetFrequency.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/CheckRotaryEncoder.o: ../CheckRotaryEncoder.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/Battery_check.o: ../Battery_check.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/main.o: ../main.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/ChargePin10ms.o: ../ChargePin10ms.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/EntladePins.o: ../EntladePins.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/EE_check_init.o: ../EE_check_init.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/ShowData.o: ../ShowData.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/show_Resis_Cap.o: ../show_Resis_Cap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/sampling_cap.o: ../sampling_cap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/sampling_lc.o: ../sampling_lc.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/sampling_xtal.o: ../sampling_xtal.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/CheckUJT.o: ../CheckUJT.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

########### Compile assembler source only 

$(OBJDIR)/samplingADC.o: ../samplingADC.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<
	
########### directories
deleteobj:
	rm -rf $(OBJDIR)
# create directories
objdirect: deleteobj
	mkdir -p $(OBJDIR)
directories: objdirect
	mkdir -p $(DEPDIR)

OBJFILES = $(addprefix $(OBJDIR)/, $(OBJECTS))


##Link
$(TARGET): directories $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	$(AVR_TOOL/PATH)avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@echo $(OP_MHZ) MHz operation configured.
	@$(AVR_TOOL/PATH)avr-size -C --mcu=${MCU} ${TARGET}
	@rm -rf $(OBJDIR)

## Clean target
.PHONY: directories size clean steril fuses fuses-crystal fuses-crystal-lp erase upload program flash eeprom eeread read verify upload_orig
clean:
	-rm -f $(OBJECTS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map
	@rm -rf dep *.o  # old path and files
	@rm -rf $(OBJDIR)

steril:
	-rm -f $(PROJECT).hex $(PROJECT).eep
	$(MAKE)
	-rm -f $(PROJECT).elf $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)


# device programming
# make fuses  call if you don't have installed a crystal
SlowBitClock = 200
fuses:
	avrdude -c $(PROGRAMMER) -B $(SlowBitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT)  $(FUSES_INT)

# fuses-crytal can only used if a 8MHz Crytal is installed (between Pin 9 and 10)
fuses-crystal:
	avrdude -c $(PROGRAMMER) -B $(SlowBitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY)
fuses-crystal-lp:
	avrdude -c $(PROGRAMMER) -B $(SlowBitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY_L)

# erases the FLASH ROM, EEPROM and clears all lock bits 
erase:
	avrdude -c $(PROGRAMMER) -B $(SlowBitClock) -p $(PARTNO) -P $(PORT) -e

# make upload  additionally calls make to compile the software for selected target
upload:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
program:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
flash:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a 
eeprom:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) \
	-U eeprom:w:./$(PROJECT).eep:a
eeread:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) \
	-U eeprom:r:./TTactual.eep:i
read:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) \
	-U flash:r:./TTactual.hex:i
verify:
	avrdude -D -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) -U flash:v:./$(PROJECT).hex:a \
	-U eeprom:v:./$(PROJECT).eep:a -v
upload_orig:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) -p $(PARTNO) -P $(PORT) -U flash:w:./TransistorTestorig.hex:a \
	-U eeprom:w:./TransistorTestorig.eep:a
if:
	ls -l /dev/serial/by-id/* | cut -c 40-
## ****************************** EOF ***************************************** ##
