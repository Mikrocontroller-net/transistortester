
########### Compile C source only 
font.o: ../font.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd-draw.o: ../lcd-draw.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DisplayValue.o: ../DisplayValue.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

function_menu.o: ../function_menu.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

message_key_released.o: ../message_key_released.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadCapacity.o: ../ReadCapacity.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadBigCap.o: ../ReadBigCap.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadInductance.o: ../ReadInductance.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Calibrate_UR.o: ../Calibrate_UR.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckPins.o: ../CheckPins.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetVloss.o: ../GetVloss.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetFrequency.o: ../GetFrequency.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckRotaryEncoder.o: ../CheckRotaryEncoder.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -D$(UI_LANGUAGE) -c  $<

ChargePin10ms.o: ../ChargePin10ms.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EntladePins.o: ../EntladePins.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EE_check_init.o: ../EE_check_init.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_one.o: bmp_one.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_two.o: bmp_two.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_three.o: bmp_three.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_npn.o: bmp_npn.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_pnp.o: bmp_pnp.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_e_mos.o: bmp_p_e_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_d_mos.o: bmp_p_d_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_e_mos.o: bmp_n_e_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_d_mos.o: bmp_n_d_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_e_igbt.o: bmp_p_e_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_d_igbt.o: bmp_p_d_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_e_igbt.o: bmp_n_e_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_d_igbt.o: bmp_n_d_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_vakdiode.o: bmp_vakdiode.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_jfet.o: bmp_n_jfet.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_jfet.o: bmp_p_jfet.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_triac.o: bmp_triac.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_thyristor.o: bmp_thyristor.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_inductor.o: bmp_inductor.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@echo $(OP_MHZ) MHz operation configured.
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map
steril:
	$(MAKE)
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


# device programming
# make fuses  call if you don't have installed a crystal
fuses:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_INT)

# fuses-crytal can only used if a 8MHz Crytal is installed (between Pin 9 and 10)
fuses-crystal:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY)
fuses-crystal-lp:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY_L)

# erases the FLASH ROM, EEPROM and clears all lock bits 
erase:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -e

# make upload  additionally calls make to compile the software for selected target
upload:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
program:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
eeprom:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) \
	-U eeprom:w:./$(PROJECT).eep:a
eeread:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) \
	-U eeprom:r:./TTactual.eep:i
verify:
	avrdude -D -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:v:./$(PROJECT).hex:a \
	-U eeprom:v:./$(PROJECT).eep:a -v
upload_orig:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./TransistorTestorig.hex:a \
	-U eeprom:w:./TransistorTestorig.eep:a
