#===============================================================================
# Add for Hisilion project
#===============================================================================
include ../config.mk
TARGET := porting
#===============================================================================
# local variable
#===============================================================================

# add source_dir here.
VPATH += ./src/tsparse
VPATH += ./src/threadpool
VPATH += ./src/usb
VPATH += ./src/web
VPATH += ./src/linuxos
VPATH += ./src/crc

LIBS := lib$(TARGET)
OBJ_DIR := ./obj
LIB_DIR := ./lib

SOURCES := $(notdir $(shell find $(VPATH) -maxdepth 1 -name "*.c"))
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

CFLAGS  = -c -Wall -O2 -g -fPIC  $(CFG_CFLAGS)
#CFLAGS += -Wall -O2 -g $(CFG_CFLAGS)
CFG_INC = $(INCFLAGS)

EXTRA_CFLAGS += -c -O2 -Wall $(CFG_INC) $(CFG_DEBUG) $(USER_CFLAGS) -fPIC -DUSE_AEC

CC+= -D_GNU_SOURCE $(CFLAGS) $(CFG_CFLAGS)
CXX = $(CROSS)-g++
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

clean:
	-@rm -f $(LIB_DIR)/$(LIBS).a
	-@rm -f $(LIBMOD_DIR)/$(LIBS).a
	-@rm -f $(OBJECTS)
	
$(LIBS).a: $(OBJECTS)
	$(AR) -r -o $@ $^	
	$(CP) $(LIBS).a $(LIBMOD_DIR)
	$(MV) $(LIBS).a $(LIB_DIR)
	@echo "---->porting prepared!"

$(OBJ_DIR)/%.o:%.c
	@echo CC $< ...
	@$(CC) $(EXTRA_CFLAGS) $< -o $@
