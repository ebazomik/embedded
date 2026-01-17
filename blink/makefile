MCU=atmega328p

F_CPU=16000000UL

TARGET=main

PROGRAMMER=arduino
BAUDRATE=115200

PORT=/dev/ttyACM0

CC=avr-gcc
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@
	avr-size --mcu=$(MCU) --format=avr $(TARGET).elf

clean:
	rm -f *.elf *.hex

flash: $(TARGET).hex
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUDRATE) -U flash:w:$<:i

.PHONY: all clean flash
