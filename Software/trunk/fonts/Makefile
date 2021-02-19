
## General Flags
CC = gcc

CPP = g++

CFLAGS = -Wall -g



## Build
all:  font2ascii ascii2font ascii2font2 5x8_vertikal_LSB_1.asc 6x8_vertikal_LSB_1.asc \
      7x8_vertikal_LSB_1.asc 8x8_vertikal_LSB_1.asc \
      7x12_vertikal_LSB_1.asc 8x12_vertikal_LSB_1.asc \
      8x12_vertikal_LSB_1thin.asc 8x14_vertikal_LSB_1.asc \
      8x16_vertikal_LSB_1.asc 8x16_vertikal_LSB_1thin.asc \
      10x16_vertikal_LSB_1.asc \
      24x32_vertical_LSB.asc

# Buils ascii tables from font tables for visual check
5x8_vertikal_LSB_1.asc: 5x8_vertikal_LSB_1.h 
	./font2ascii 5x8_vertikal_LSB_1.h
6x8_vertikal_LSB_1.asc: 6x8_vertikal_LSB_1.h 
	./font2ascii 6x8_vertikal_LSB_1.h
7x8_vertikal_LSB_1.asc: 7x8_vertikal_LSB_1.h 
	./font2ascii 7x8_vertikal_LSB_1.h
8x8_vertikal_LSB_1.asc: 8x8_vertikal_LSB_1.h 
	./font2ascii 8x8_vertikal_LSB_1.h
7x12_vertikal_LSB_1.asc: 7x12_vertikal_LSB_1.h 
	./font2ascii 7x12_vertikal_LSB_1.h
8x12_vertikal_LSB_1.asc: 8x12_vertikal_LSB_1.h 
	./font2ascii 8x12_vertikal_LSB_1.h
8x12_vertikal_LSB_1thin.asc: 8x12_vertikal_LSB_1thin.h 
	./font2ascii 8x12_vertikal_LSB_1thin.h
8x14_vertikal_LSB_1.asc: 8x14_vertikal_LSB_1.h 
	./font2ascii 8x14_vertikal_LSB_1.h
8x16_vertikal_LSB_1.asc: 8x16_vertikal_LSB_1.h 
	./font2ascii 8x16_vertikal_LSB_1.h
8x16_vertikal_LSB_1thin.asc: 8x16_vertikal_LSB_1thin.h 
	./font2ascii 8x16_vertikal_LSB_1thin.h
10x16_vertikal_LSB_1.asc: 10x16_vertikal_LSB_1.h 
	./font2ascii 10x16_vertikal_LSB_1.h
24x32_vertical_LSB.asc: 24x32_vertical_LSB.h 
	./font2ascii 24x32_vertical_LSB.h

##Link
font2ascii: font2ascii.c
	$(CC)  $(CFLAGS) -o font2ascii font2ascii.c
ascii2font: ascii2font.c
	$(CC)  $(CFLAGS) -g -o ascii2font ascii2font.c
ascii2font2: ascii2font2.c
	$(CC)  $(CFLAGS) -o ascii2font2 ascii2font2.c

clean:
	rm *.asc
