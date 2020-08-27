# File:   Makefile
# AuthorM. Lewis Thorp, Lydia Looi
# Date:   18 Oct 19
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../drivers/avr -I../../fonts -I../../drivers -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
bullet.o: bullet.c ../../drivers/avr/ir_uart.h ../../drivers/avr/system.h ../../drivers/display.h ../../utils/boing.h ../../utils/font.h ../../utils/tinygl.h bullet.h
	$(CC) -c $(CFLAGS) $< -o $@

game_data.o: game_data.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/display.h ../../utils/boing.h ../../utils/font.h ../../utils/tinygl.h bullet.h game_data.h ship.h
	$(CC) -c $(CFLAGS) $< -o $@

ship.o: ship.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h ship.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/delay.h ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

boing.o: ../../utils/boing.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/boing.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

game.o: game.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../drivers/display.h ../../drivers/navswitch.h ../../fonts/font5x5_1.h ../../utils/boing.h ../../utils/font.h ../../utils/pacer.h ../../utils/task.h ../../utils/tinygl.h bullet.h game_data.h ship.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
game.out: bullet.o game_data.o ship.o ir_uart.o pio.o prescale.o system.o timer.o timer0.o usart1.o display.o ledmat.o navswitch.o boing.o font.o pacer.o task.o tinygl.o game.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
game.hex: game.out
	$(OBJCOPY) -O ihex game.out game.hex


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


