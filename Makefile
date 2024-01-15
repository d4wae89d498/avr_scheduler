export CC = avr-gcc

export MCU = atmega328p
export TARGET_ARCH = -mmcu=$(MCU)

export CFLAGS =  -Wall -I. -DF_CPU=16000000 -Os #-g
export LDFLAGS = -g $(TARGET_ARCH) -lm -Wl,--gc-sections #	-Os


UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Darwin) # If you're a darwin user, you should have arduino installed cause we point to it :)
ARDUINO_HW_ROOT = /Applications/Arduino.app/Contents/Java/hardware/arduino/avr
TERM = /dev/tty.usbmodem*
STTY_OPT = -f
AVRDUDE_PATH = /opt/local/bin/
AVRDUDECONF_PATH = /opt/local/etc/
else # only linux is supported, haven't checked for varieties here
ARDUINO_HW_ROOT = /usr/share/arduino/hardware/arduino/
TERM = /dev/ttyACM0
STTY_OPT = -F
AVRDUDE_PATH = /usr/bin/
AVRDUDECONF_PATH = /usr/share/arduino/hardware/tools/
endif

TARGET = bin/scheduler
CPPFLAGS = -mmcu=$(MCU)
PGMER = -c stk500v1 -b 57600 -P $(TERM)
PGMERISP = -c stk500v1 -b 115200 -P $(TERM)
ARVDUDECONF= -C /usr/share/arduino/hardware/tools/avrdude.conf
export DUDE = avrdude -F -v -p $(MCU) $(AVRDUDECONF)

C_SRC = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,bin/%.o,$(C_SRC))

all: $(TARGET).hex


clean:
	rm -f bin/*.o *~ *.elf *.hex

bin/%.o: src/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@


$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET).hex: $(TARGET).elf
	mkdir -p bin
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(TARGET).elf bin/eeprom.hex

upload: $(TARGET).hex
	stty $(STTY_OPT) $(TERM) hupcl
# reset
#	stty -f $(TERM) hupcl # reset
#	$(DUDE) $(PGMER) -U flash:w:$(TARGET).hex
	$(DUDE) $(PGMERISP) -U flash:w:$(TARGET).hex

size: $(TARGET).elf
	avr-size --format=avr --mcu=$(MCU) $(TARGET).elf

re: clean all

