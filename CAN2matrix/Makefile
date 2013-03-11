SHELL := cmd.exe
RM := rm -rf

# set correct MCU
MCU := atmega8
# set frequency of MCU
MCU_SPEED := 4000000UL

USER_OBJS :=

LIBS :=
PROJ :=

O_SRCS :=
C_SRCS :=
S_SRCS :=
S_UPPER_SRCS :=
OBJ_SRCS :=
ASM_SRCS :=
PREPROCESSING_SRCS :=
OBJS :=
OBJS_AS_ARGS :=
C_DEPS :=
C_DEPS_AS_ARGS :=
EXECUTABLES :=
LIB_AS_ARGS :=
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
HEX_FLASH_FILE_PATH :=
HEX_FLASH_FILE_PATH_AS_ARGS :=
HEX_EEPROM_FILE_PATH :=
HEX_EEPROM_FILE_PATH_AS_ARGS :=
LSS_FILE_PATH :=
LSS_FILE_PATH_AS_ARGS :=
MAP_FILE_PATH :=
MAP_FILE_PATH_AS_ARGS :=
AVR_APP_PATH := $(AVR_APP_PATH)
QUOTE := "
SLASH := \\
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
DOC_EXE := doxygen
DOC_CONF := doxygen.conf

# Every subdirectory with source files must be described here
SUBDIRS :=  \
./modules/can \
./comm \
./modules/leds \
./modules/bar \
./modules/adc \
./modules/util \
./modules/timer \
./modules/uart \
./modules/spi


# Add inputs and outputs from these tool invocations to the build variables
C_SRCS +=  \
./modules/adc/adc.c \
./modules/bar/bar.c \
./CAN2matrix.c \
./modules/config/can_config_mcp2515.c \
./modules/can/can_error_mcp2515.c \
./modules/can/can_filter_mcp2515.c \
./modules/can/can_init_mcp2515.c \
./modules/can/can_receive_mcp2515.c \
./modules/can/can_sleep_mcp2515.c \
./modules/can/can_send_mcp2515.c \
./comm/comm_matrix.c \
./comm/ic_comm.c \
./modules/leds/leds.c \
./modules/spi/spi.c \
./modules/timer/timer.c \
./modules/uart/uart.c


PREPROCESSING_SRCS +=


ASM_SRCS +=


OBJS +=  \
modules/adc/adc.o \
modules/bar/bar.o \
CAN2matrix.o \
modules/config/can_config_mcp2515.o \
modules/can/can_error_mcp2515.o \
modules/can/can_filter_mcp2515.o \
modules/can/can_init_mcp2515.o \
modules/can/can_receive_mcp2515.o \
modules/can/can_sleep_mcp2515.o \
modules/can/can_send_mcp2515.o \
comm/comm_matrix.o \
comm/ic_comm.o \
modules/leds/leds.o \
modules/spi/spi.o \
modules/timer/timer.o \
modules/uart/uart.o


OBJS_AS_ARGS +=  \
modules/adc/adc.o \
modules/bar/bar.o \
CAN2matrix.o \
modules/config/can_config_mcp2515.o \
modules/can/can_error_mcp2515.o \
modules/can/can_filter_mcp2515.o \
modules/can/can_init_mcp2515.o \
modules/can/can_receive_mcp2515.o \
modules/can/can_sleep_mcp2515.o \
modules/can/can_send_mcp2515.o \
comm/comm_matrix.o \
comm/ic_comm.o \
modules/leds/leds.o \
modules/spi/spi.o \
modules/timer/timer.o \
modules/uart/uart.o


C_DEPS +=  \
modules/adc/adc.d \
modules/bar/bar.d \
CAN2matrix.d \
modules/config/can_config_mcp2515.d \
modules/can/can_error_mcp2515.d \
modules/can/can_filter_mcp2515.d \
modules/can/can_init_mcp2515.d \
modules/can/can_receive_mcp2515.d \
modules/can/can_sleep_mcp2515.d \
modules/can/can_send_mcp2515.d \
comm/comm_matrix.d \
comm/ic_comm.d \
modules/leds/leds.d \
modules/spi/spi.d \
modules/timer/timer.d \
modules/uart/uart.d


C_DEPS_AS_ARGS +=  \
modules/adc/adc.d \
modules/bar/bar.d \
CAN2matrix.d \
modules/config/can_config_mcp2515.d \
modules/can/can_error_mcp2515.d \
modules/can/can_filter_mcp2515.d \
modules/can/can_init_mcp2515.d \
modules/can/can_receive_mcp2515.d \
modules/can/can_sleep_mcp2515.d \
modules/can/can_send_mcp2515.d \
comm/comm_matrix.d \
comm/ic_comm.d \
modules/leds/leds.d \
modules/spi/spi.d \
modules/timer/timer.d \
modules/uart/uart.d

TEMPS += \
adc.i \
bar.i \
CAN2matrix.i \
can_config_mcp2515.i \
can_error_mcp2515.i \
can_filter_mcp2515.i \
can_init_mcp2515.i \
can_receive_mcp2515.i \
can_sleep_mcp2515.i \
can_send_mcp2515.i \
comm_matrix.i \
ic_comm.i \
leds.i \
spi.i \
timer.i \
uart.i

S_SRCS += \
adc.s \
bar.s \
CAN2matrix.s \
can_config_mcp2515.s \
can_error_mcp2515.s \
can_filter_mcp2515.s \
can_init_mcp2515.s \
can_receive_mcp2515.s \
can_sleep_mcp2515.s \
can_send_mcp2515.s \
comm_matrix.s \
ic_comm.s \
leds.s \
spi.s \
timer.s \
uart.s


OUTPUT_FILE_PATH +=CAN2matrix.elf

OUTPUT_FILE_PATH_AS_ARGS +=CAN2matrix.elf

HEX_FLASH_FILE_PATH +=CAN2matrix.hex

HEX_FLASH_FILE_PATH_AS_ARGS +=CAN2matrix.hex

HEX_EEPROM_FILE_PATH +=CAN2matrix.eep

HEX_EEPROM_FILE_PATH_AS_ARGS +=CAN2matrix.eep

LSS_FILE_PATH +=CAN2matrix.lss

LSS_FILE_PATH_AS_ARGS +=CAN2matrix.lss

MAP_FILE_PATH =CAN2matrix.map

MAP_FILE_PATH_AS_ARGS =CAN2matrix.map

LIB_AS_ARGS +=libCAN2matrix.a

ADDITIONAL_DEPENDENCIES:= $(HEX_FLASH_FILE_PATH) $(LSS_FILE_PATH) $(HEX_EEPROM_FILE_PATH) size

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR/GNU C Compiler

























modules/adc/%.o: ./modules/adc/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99 -save-temps  -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/bar/%.o: ./modules/bar/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

./%.o: ./%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/can/%.o: ./modules/can/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

comm/%.o: ./comm/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/leds/%.o: ./modules/leds/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/spi/%.o: ./modules/spi/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/timer/%.o: ./modules/timer/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

modules/uart/%.o: ./modules/uart/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=$(MCU_SPEED) -Os -ffunction-sections -fpack-struct -fshort-enums -Wall -Werror -pedantic -pedantic-errors -c -std=gnu99  -save-temps -mmcu=$(MCU)   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<



# AVR/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

# AVR/GNU C/C++ Linker
$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU C/C++ Linker
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-gcc.exe$(QUOTE) -Wl,--gc-sections -mrelax  -mmcu=$(MCU)  -Wl,-Map=$(MAP_FILE_PATH_AS_ARGS) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@



$(HEX_FLASH_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-objcopy.exe$(QUOTE) -O ihex -R .eeprom -R .fuse -R .lock -R .signature  $(QUOTE)$<$(QUOTE) $(QUOTE)$@$(QUOTE)

$(HEX_EEPROM_FILE_PATH): $(OUTPUT_FILE_PATH)
	-$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-objcopy.exe$(QUOTE) -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex $(QUOTE)$<$(QUOTE) $(QUOTE)$@$(QUOTE) || exit 0

$(LSS_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-objdump.exe$(QUOTE) -h -S $(QUOTE)$<$(QUOTE) > $(QUOTE)$@$(QUOTE)

size: $(OUTPUT_FILE_PATH)
	@$(QUOTE)$(AVR_APP_PATH)$(SLASH)avr-size.exe$(QUOTE) -C --mcu=$(MCU) $(OUTPUT_FILE_PATH_AS_ARGS)

# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) $(LIB_AS_ARGS) $(HEX_FLASH_FILE_PATH_AS_ARGS) $(HEX_EEPROM_FILE_PATH_AS_ARGS) $(LSS_FILE_PATH_AS_ARGS) $(MAP_FILE_PATH_AS_ARGS) $(OUTPUT_FILE_PATH_AS_ARGS) $(TEMPS) $(S_SRCS)

docu:
	$(DOC_EXE) $(DOC_CONF)

