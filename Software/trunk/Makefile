
ALL_PROJECTS := \
       default \
       mega8 \
       mega168_1.9V \
       mega328_1.9V \
       mega328 \
       mega168_3.3V \
       mega328_3.3V \
       mega328_dogm \
       mega168_strip_grid \
       mega328_strip_grid \
       mega328_strip_grid_dogm \
       mega328_2X16_menu \
       mega328_st7565 \
       mega328_st7565_kit \
       mega328_color_kit \
       mega328_st7920 \
       mega328_st7108 \
       mega328_GM328 \
       mega328_fish8840 \
       mega328_fish8840_OC \
       mega328_wei_st7565 \
       mega328_MK-328 \
       mega328_T3_T4_st7565 \
       mega328_T4_v2_st7565 \
       mega328_T5_st7565 \
       mega328_ssd1306SPI \
       mega328_ssd1306I2C \
       mega328_ssd1327SPI \
       mega328_ssd1327I2C \
       mega328_PCF8812 \
       mega328_PCF8814 \
       arduino_m2560 \
       arduino_uno \
       mega644_hiland_m644 \
       mega644_LCD2004 \
       mega644_ssd1306I2C \
       ST7735 \
       ILI9163 \


.PHONY: all steril clean $(ALL_PROJECTS)

all: $(ALL_PROJECTS)
	
$(ALL_PROJECTS):
	$(MAKE) -C $@

clean:
	for d in $(ALL_PROJECTS);        \
	do                               \
	  $(MAKE) --directory=$$d clean; \
	done

steril:
	for d in $(ALL_PROJECTS);         \
	do                                \
	  $(MAKE) --directory=$$d steril; \
	done



