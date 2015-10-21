#!/bin/sh

PROJECT  = lma

C_SRCS   = $(PROJECT).c 
#OBJS    = $(PROJECT).o crt1.o
OBJS     = $(PROJECT).o battery.o buzzer.o morse.o switch.o
LIBS     = 
LIBDIRS  = 
INCLUDES = 
LSS          = $(PROJECT).lss
FLASH_IMAGE  = $(PROJECT).hex
EEPROM_IMAGE = $(PROJECT).eep
SIZEDUMMY = sizedummy

MCU      = attiny25
CFLAGS   = -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -ffunction-sections
LDFLAGS  = $(LIBDIRS) $(LIBS) -lm -Wl,-gc-sections

TARGET=$(MCU)
#AVR Dragon
PORT=usb
PROGRAMMER=dragon_isp
#Pololu programmer
#PORT=/dev/pololu_avrisp
#PROGRAMMER=avrispv2
#JTAG ICE MkII
#PORT=usb
#PROGRAMMER=jtag2isp
AVRDUDE_FLAGS=-B20

#LFUSE=0xfa
#HFUSE=0xdf
LFUSE=0xe2
HFUSE=0xd7
EFUSE=0xff

# Add inputs and outputs from these tool invocations to the build variables 

RM := rm -rf

# All Target
all: clean $(PROJECT).hex

# This is included for AVR Studio 4 compatibility
$(PROJECT): all

# Tool invocations
$(PROJECT).elf: $(OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,$(PROJECT).map $(LIBDIRS) -mmcu=$(MCU) -o "$(PROJECT).elf" $(OBJS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

$(PROJECT).lss: $(PROJECT).elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S $(PROJECT).elf  >"$(PROJECT).lss"
	@echo 'Finished building: $@'
	@echo ' '

$(PROJECT).hex: $(PROJECT).elf $(SIZEDUMMY)
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex $(PROJECT).elf  "$(PROJECT).hex"
	@echo 'Finished building: $@'
	@echo ' '

$(PROJECT).eep: $(PROJECT).elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $(PROJECT).elf  "$(PROJECT).eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: $(PROJECT).elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t $(PROJECT).elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(EEPROM_IMAGE) $(FLASH_IMAGE) $(ELFS) $(LSS) $(SIZEDUMMY) $(PROJECT).elf

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

install: $(PROJECT).hex
	avrdude $(AVRDUDE_FLAGS) -p $(TARGET) -c $(PROGRAMMER) -P $(PORT) -v -U flash:w:$(PROJECT).hex

eeprom: $(PROJECT).eep
	avrdude $(AVRDUDE_FLAGS) -p $(TARGET) -c $(PROGRAMMER) -P $(PORT) -v -U eeprom:w:$(PROJECT).eep

fuses:
	avrdude $(AVRDUDE_FLAGS) -p $(TARGET) -c $(PROGRAMMER) -P $(PORT) -v -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

calibrate:
	avrdude $(AVRDUDE_FLAGS) -p $(TARGET) -c $(PROGRAMMER) -P $(PORT) -v -O

%.o: %.c
	@echo 'Building file: $<'
	avr-gcc $(INCLUDES) $(CFLAGS) -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
../A2D/libA2D.a:
	-cd ../A2D && $(MAKE) all

../USI_I2C/libUSI_I2C.a: ../A2D/libA2D.a
	-cd ../USI_I2C && $(MAKE) all
		
.PHONY: all clean dependents
.SECONDARY:
