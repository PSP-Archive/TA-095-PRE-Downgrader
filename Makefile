TARGET = TA095v2_PRE_Downgrader
OBJS = main.o idsprx/IdStorage.o

INCDIR =
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1

LIBDIR =  ./
LIBS = -lpspkubridge
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = TA-095v2 PRE-Downgrader

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
