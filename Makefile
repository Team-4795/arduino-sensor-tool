# Arduino 0015 Makefile
# Arduino adaptation by mellis, eighthave, oli.keller
# Modified by Johannes Hoff <johannes@johanneshoff.com> to
# work on linux command line and sort out dependencies.
# further modified by Steve Tell, for standalone use
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# Detailed instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch. There should be a
#     file with the same name as the folder and with the extension .pde
#     (e.g. foo.pde in the foo/ folder).
#
#  2. Modify the line containg "INSTALL_DIR" to point to the directory that
#     contains the Arduino installation (for example, under Mac OS X, this
#     might be /Applications/arduino-0015).
#
#  3. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.usb*).
#
#  4. Set the line containing "MCU" to match your board's processor.
#     Older one's are atmega8 based, newer ones like Arduino Mini, Bluetooth
#     or Diecimila have the atmega168.  If you're using a LilyPad Arduino,
#     change F_CPU to 8000000.
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make download", reset your Arduino board, and press enter to
#     download your program to the Arduino board.

TARGET = sensormenu
INSTALL_DIR = /usr/share/arduino
PORT = /dev/ttyUSB4
#DOWNLOAD_RATE = 115200
DOWNLOAD_RATE = 57600
#DOWNLOAD_RATE = 19200
AVRDUDE_PROGRAMMER = arduino
MCU = atmega328p
F_CPU = 16000000

EXTRA_OBJS=gndkeys.o lcdmenu.o

############################################################################
# Below here nothing should be changed...

ARDUINO = $(INSTALL_DIR)/hardware/arduino/avr/cores/arduino
# TODO generalize this
ARDUINO2 = $(INSTALL_DIR)/hardware/arduino/avr/libraries/SoftwareSerial/src

AVR_TOOLS_PATH = /usr/bin
AVRDUDE_PATH = $(INSTALL_DIR)/hardware/tools
LIBCSRC =   $(ARDUINO)/wiring.c \
	$(ARDUINO)/wiring_analog.c $(ARDUINO)/wiring_digital.c \
	$(ARDUINO)/wiring_pulse.c \
	$(ARDUINO)/wiring_shift.c $(ARDUINO)/WInterrupts.c \
	$(ARDUINO)/hooks.c 

ARDLIB=$(INSTALL_DIR)/libraries

LIBCXXSRC = $(ARDUINO)/HardwareSerial.cpp \
	$(ARDUINO)/HardwareSerial0.cpp \
	$(ARDUINO)/WMath.cpp \
	$(ARDUINO)/Print.cpp 

LIBCXXSRC2=$(ARDUINO2)/SoftwareSerial.cpp

FORMAT = ihex
VPATH=$(ARDUINO) $(ARDUINO2)


# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

# Place -I options here
CINCS = -I$(ARDUINO) -I$(ARDUINO2) -I$(ARDUINO)/../../variants/standard
CXXINCS = 

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(CINCS) $(CXXINCS) -O$(OPT)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -lm


# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_FLAGS = -V -F -C $(AVRDUDE_PATH)/avrdude.conf \
-p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
# -b $(DOWNLOAD_RATE)

AVRDUDE_FLAGS = -v -V -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
 -b $(DOWNLOAD_RATE)


# Program settings
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
LIBOBJ = $(addprefix lib/, $(notdir $(LIBCSRC:.c=.o) $(LIBCXXSRC:.cpp=.o) $(LIBCXXSRC2:.cpp=.o) $(LIBASRC:.S=.o)))

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: build sizeafter

build: elf hex

# Here is the "preprocessing".
# It creates a .cpp file based with the same name as the .pde file.
# On top of the new .cpp file comes the WProgram.h header.
# At the end there is a generic main() function attached.
# Then the .cpp file will be compiled. Errors during compile will
# refer to this new, automatically generated, file.
# Not the original .pde file you actually edit...
#applet/$(TARGET).cpp: $(TARGET).pde
#	test -d applet || mkdir applet
#	echo '#include "WProgram.h"' > applet/$(TARGET).cpp
#	cat $(TARGET).pde >> applet/$(TARGET).cpp
#	cat $(ARDUINO)/main.cpp >> applet/$(TARGET).cpp

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

# Program the device.
download: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE)  $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

# Link: create ELF output file from library.
$(TARGET).elf: $(TARGET).cpp lib/libcore.a $(EXTRA_OBJS)
	$(CXX) $(ALL_CXXFLAGS) -o $@ $(TARGET).cpp $(EXTRA_OBJS) -L. lib/libcore.a $(LDFLAGS)

lib/libcore.a: $(LIBOBJ)
	mkdir -p lib
	@for i in $(LIBOBJ); do echo $(AR) rcs $@ $$i; $(AR) rcs $@ $$i; done



# Compile: create object files from C++ source files.
#.cpp.o:

lib/%.o: %.cpp
	@mkdir -p lib
	@echo  cpp.o rule
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
#.c.o:

lib/%.o: %.c
	@mkdir -p lib
	@echo  c.o rule
	$(CC) -c $(ALL_CFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss lib/libcore.a \
	$(LIBOBJ) $(LST) $(LIBCSRC:.c=.s) $(LIBCSRC:.c=.d) $(LIBCXXSRC:.cpp=.s) $(LIBCXXSRC:.cpp=.d)

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(LIBSRC) $(ASRC) >> $(MAKEFILE)

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend sizebefore sizeafter

testvars:
	@echo LIBOBJ=$(LIBOBJ)
	@echo LIBLST=$(LIBLST)
