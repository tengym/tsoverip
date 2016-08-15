# Config.mk 
# defined some generic variables

SHELL = /bin/bash

CFG_SDK_VERSION=60
ifeq ($(CFG_SDK_VERSION),20)
CFG_CFLAGS += -DSDK_VER_20
else
CFG_CFLAGS += -DSDK_VER_60
endif

OUT_DIR = $(PROJECT_DIR)
MW_DIR = $(PROJECT_DIR)/midware
PORT_DIR = $(PROJECT_DIR)/porting

LIBMOD_DIR = $(PROJECT_DIR)/lib/modules
LIBTHI_DIR = $(PROJECT_DIR)/lib/thirdparty
LIBTSOCKET_DIR = $(PROJECT_DIR)/lib/thirdparty/mediadistribution

PROG_INC_DIR = $(PROJECT_DIR)/include
PORT_INC_DIR = $(PORT_DIR)/include
MW_INC_DIR = $(MW_DIR)/include

# compile and bin utilies
CC		= arm-hisiv200-linux-gcc 
AR		= arm-hisiv200-linux-ar   
LD		= arm-hisiv200-linux-ld
RUN		= arm-hisiv200-linux-run  
DB		= arm-hisiv200-linux-gdb  
LINK    = arm-hisiv200-linux-gcc  
CPP     = arm-hisiv200-linux-cpp
CXX     = arm-hisiv200-linux-g++
RANLIB  = arm-hisiv200-linux-ranlib
AS      = arm-hisiv200-linux-as
STRIP   = arm-hisiv200-linux-strip

# shell commands
CP 		= cp
MV		= mv


INCFLAGS	=	-I $(PROG_INC_DIR)/thirdparty -I $(PROG_INC_DIR)/thirdparty/mediadistribution -I $(PROG_INC_DIR)/hisilicon
				
INCFLAGS	+= -I $(PORT_DIR)/include
INCFLAGS	+= -I $(PORT_INC_DIR)/tsparse
INCFLAGS	+= -I $(PORT_INC_DIR)/threadpool
INCFLAGS	+= -I $(PORT_INC_DIR)/usb
INCFLAGS	+= -I $(PORT_INC_DIR)/web
INCFLAGS	+= -I $(PORT_INC_DIR)/linuxos
INCFLAGS	+= -I $(PORT_INC_DIR)/crc

INCFLAGS	+= -I $(MW_DIR)/include
INCFLAGS	+= -I $(MW_INC_DIR)/gos_sys

CFG_CFLAGS	+= -Werror
SHARED_FLAGS   	= -fPIC -shared
STATIC_FLAGS  	= crs



