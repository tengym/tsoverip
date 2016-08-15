#===============================================================================
# Add for Hisilion project
#===============================================================================
include ../config.mk

#===============================================================================
# local variable
#===============================================================================

image = hiapp

# add source_dir here.
VPATH += ./src/main

OBJ_DIR := ./obj

SOURCES := $(notdir $(shell find $(VPATH) -maxdepth 1 -name "*.c"))
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

CFLAGS += -Wall -O2 -g $(CFG_CFLAGS)
CFG_INC = $(INCFLAGS)

LIBPATH = -L $(LIBMOD_DIR) -L $(LIBLOD_DIR) -L $(LIBHIS_DIR) -L $(LIBTHI_DIR) -L $(LIBTSOCKET_DIR)

LIB = -Wl,--start-group -ljthread -ljrtp -lrtp_unicast -lrtp_multicast -ludp_unicast -ludp_multicast \
		-lpthread -lrt -ldl -lm -lhi_common -lhi_msp -lporting -lmidware -Wl,--end-group

EXTRA_CFLAGS += -c -O2 -Wall $(CFG_INC) $(CFG_DEBUG) $(USER_CFLAGS) -fPIC -DUSE_AEC
EXTRA_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=1024
EXTRA_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

COMPILE=$(CC) $(EXTRA_CFLAGS) -o "$(OBJ_DIR)/$(*F).o" "$<"

CC+= -D_GNU_SOURCE $(CFLAGS) $(CFG_CFLAGS)
CXX = arm-hisiv200-linux-g++
CXX+= -D_GNU_SOURCE $(CFLAGS) $(CFG_CFLAGS)

.c.o:
	@echo  CC   $< ...
	@$(CC) $(EXTRA_CFLAGS) $< -o $@

.cc.o: 
	@echo  CC  $< ...
	@$(CXX) $(EXTRA_CFLAGS) $< -o $@

.cpp.o:
	@echo  CPP  $< ...
	@$(CXX) $(EXTRA_CFLAGS) $< -o $@

.cxx.o: 
	@echo  CXX  $< ...
	@$(CXX) $(EXTRA_CFLAGS) $< -o $@

.PHONY: all clean


all: $(image)
hiapp: $(OBJECTS)
	@echo $(OBJECTS)
	@$(CXX) -o $@.elf $^ $(CFG_INC) $(LIBPATH) $(LIB)
	@$(STRIP) $@.elf
	@$(MV) $@.elf $(OUT_DIR)

$(OBJECTS):  $(HEADFILES) $(MODULE_HEAD) 

clean:
	@echo "good luck!"
	@echo $(OBJECTS)
	-@rm -f $(OBJECTS)
	-@rm -f $(OUT_DIR)/$(image).elf

$(OBJ_DIR)/%.o:%.c
	@echo CC $< ...
	@$(CC) $(EXTRA_CFLAGS) $< -o $@
