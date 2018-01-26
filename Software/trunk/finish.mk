
########### Compile C source only 
lcd-routines.o: ../lcd-routines.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd-draw.o: ../lcd-draw.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DisplayValue.o: ../DisplayValue.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

function_menu.o: ../function_menu.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

message_key_released.o: ../message_key_released.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadCapacity.o: ../ReadCapacity.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CalibrationCap.o: ../CalibrationCap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadBigCap.o: ../ReadBigCap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadInductance.o: ../ReadInductance.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Calibrate_UR.o: ../Calibrate_UR.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckPins.o: ../CheckPins.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetResistance.o: ../GetResistance.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetVloss.o: ../GetVloss.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetFrequency.o: ../GetFrequency.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckRotaryEncoder.o: ../CheckRotaryEncoder.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Battery_check.o: ../Battery_check.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -D$(UI_LANGUAGE) -c  $<

ChargePin10ms.o: ../ChargePin10ms.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EntladePins.o: ../EntladePins.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EE_check_init.o: ../EE_check_init.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ShowData.o: ../ShowData.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

show_Resis_Cap.o: ../show_Resis_Cap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sampling_cap.o: ../sampling_cap.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sampling_lc.o: ../sampling_lc.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sampling_xtal.o: ../sampling_xtal.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckUJT.o: ../CheckUJT.c $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

########### Compile assembler source only 

samplingADC.o: ../samplingADC.S $(MKFILES)
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

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

## Clean target
.PHONY: size clean steril fuses fuses-crystal fuses-crystal-lp erase upload program flash eeprom eeread read verify upload_orig
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map
steril:
	$(MAKE)
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


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
