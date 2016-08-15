#===============================================================================
# Add for Hisilion project
#===============================================================================
include ../config.mk
TARGET := midware
#===============================================================================
# local variable
#===============================================================================

# add source_dir here.
VPATH += ./src/gos_sys
#VPATH += ./src/gos_socket

LIBS := lib$(TARGET)
OBJ_DIR := ./obj
LIB_DIR := ./lib

SOURCES := $(notdir $(shell find $(VPATH) -maxdepth 1 -name "*.c"))
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

CFLAGS += -Wall -O2 -g $(CFG_CFLAGS)
CFG_INC = $(INCFLAGS)

EXTRA_CFLAGS += -c -O2 -Wall $(CFG_INC) $(CFG_DEBUG) $(USER_CFLAGS) -fPIC -DUSE_AEC
EXTRA_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600 
EXTRA_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

CC+= -D_GNU_SOURCE $(CFLAGS) $(CFG_CFLAGS)
#CXX = $(CROSS)-g++
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

all:$(LIBS).a
	-@mkdir $(OBJECTS) -p
	-@mkdir $(LIB_DIR) -p

clean:
	-@rm -f $(LIB_DIR)/$(LIBS).a
	-@rm -f $(LIBMOD_DIR)/$(LIBS).a
	-@rm -f $(OBJECTS)
	
$(LIBS).a: $(OBJECTS)
	$(AR) -r -o $@ $^	
	$(CP) $(LIBS).a $(LIBMOD_DIR)
	$(MV) $(LIBS).a $(LIB_DIR)
	@echo "---->midware prepared!"

$(OBJ_DIR)/%.o:%.c
	@echo CC $< ...
	@$(CC) $(EXTRA_CFLAGS) $< -o $@
