
######################################
# target
######################################
TARGET = Helix-OBC-Firmware


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# source path
SOURCES_DIR =  \
src

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
#C_SOURCES =  \
#src/canParseDefault.cpp \
#src/event.cpp \
#src/eventTimer.cpp \
#src/leakCheck.cpp \
#src/main.cpp \
#src/testThread.cpp

C_SOURCES := $(wildcard src/*.cpp)

# ASM sources
ASM_SOURCES =  \


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# binaries
#######################################
#BINPATH = /opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/
BINPATH = 
#PREFIX = arm-none-eabi-
PREFIX = 
CC = $(BINPATH)$(PREFIX)g++
AS = $(BINPATH)$(PREFIX)g++ -x assembler-with-cpp
CP = $(BINPATH)$(PREFIX)objcopy
DP = $(BINPATH)$(PREFIX)objdump
AR = $(BINPATH)$(PREFIX)ar
SZ = $(BINPATH)$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
ASM = $(DP) $(DPFLAGS)

#######################################
# CFLAGS
#######################################
# cpu
#CPU = -mcpu=cortex-m4


# fpu
#FPU = -mfpu=fpv4-sp-d16


# float-abi
#FLOAT-ABI = -mfloat-abi=hard


# mcu
#MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)


# macros for gcc
# AS defines

FPU =
FLOAT-ABI = 
MCU = 
CPU =
AS_DEFS = 

# C defines
C_DEFS =  \
#-DUSE_HAL_DRIVER \
#-DSTM32L476xx \
#-DUSE_FULL_LL_DRIVER


# AS includes
AS_INCLUDES = 


# C includes
C_INCLUDES =  \
-Iinc \
-Ihelix-software/headers \
-I/usr/include/boost

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -DBOOST_LOG_DYN_LINK -Wall -fdata-sections -ffunction-sections

# compile objdump flags
DPFLAGS = --disassemble -M intel

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

#######################################
# LDFLAGS
#######################################
# link script
#LDSCRIPT = STM32L476RGTx_FLASH.ld

# libraries
#LIBS = -lc -lm -lnosys -lboost_system -lboost_thread -lpthread
LIBS = -lboost_system -lboost_thread -lpthread -lboost_log_setup -lboost_log
LIBDIR = 
#LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS = $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections


# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

#######################################
# Dependencies
#######################################
DEPS := $(OBJECTS:.o=.d)

-include $(DEPS)

#######################################
# build the application
#######################################
.PHONY: documentation

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).asm

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) -MMD -MF $(patsubst %.o,%.d,$@) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR)/%.asm: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(ASM) $< > $@

#$(BUILD_DIR)/$(TARGET).asm: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
#	$(DP) $(DPFLAGS) $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).asm

$(BUILD_DIR):
	mkdir $@	

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
	-rm -fR documentation/

documentation:
	doxygen

# Function used to check variables. Use on the command line:
# make print-VARNAME
# Useful for debugging and adding features
print-%: ; @echo $*=$($*)
