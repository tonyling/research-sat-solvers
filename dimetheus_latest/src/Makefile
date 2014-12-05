SHELL = /bin/sh
BINTYPE = STANDALONEBINARY
SRCPATH = .
BINPATH = ../bin
CC = gcc
LDFLAGS = -lm
AR = ar
ARFLAGS = rc
RANLIB = ranlib
BINNAME = dimetheus

$(info Dimetheus SAT solver Makefile.)
$(info   Author : Oliver Gableske. All rights reserved.)
$(info   Website: https://www.gableske.net/dimetheus)
$(info   )
GCCVERSIONGTEQ45 := $(shell expr `gcc -dumpversion | cut -f1-3 -d.` \>= 4.5)
GCCVERSIONGTEQ44 := $(shell expr `gcc -dumpversion | cut -f1-3 -d.` \>= 4.4)

ifeq "$(GCCVERSIONGTEQ44)" "0"
$(error Compiling requires GCC in at least version 4.4. Better is at least version 4.5.)
else ifeq "$(GCCVERSIONGTEQ45)" "1"
$(info GCC is available in version at least 4.5.)
else ifeq "$(GCCVERSIONGTEQ44)" "1"
$(info GCC is available in version at least 4.4.)
endif

MAKEFLAGS += --quiet

ifeq ($(BINTYPE),STANDALONEBINARY)
$(info Stand-alone binary mode.)
ifeq "$(GCCVERSIONGTEQ45)" "1"
CFLAGS = -std=c99 -O3 -static -fexpensive-optimizations -flto -fwhole-program -march=native -Wall -pedantic
else
CFLAGS = -std=c99 -O3 -static -fexpensive-optimizations -march=native -Wall -pedantic
endif
all: STANDALONEBINARY
endif

ifeq ($(BINTYPE),STATICLIB)
$(info Static library mode.)
STATICLIBNAME = lib$(BINNAME).a
ifeq "$(GCCVERSIONGTEQ45)" "1"
CFLAGS = -std=c99 -O3 -static -fexpensive-optimizations -flto -fwhole-program -march=native -Wall -pedantic -DCOMPILEASLIBRARY
else
CFLAGS = -std=c99 -O3 -static -fexpensive-optimizations -march=native -Wall -pedantic -DCOMPILEASLIBRARY
endif
all: STATICLIBRARY
endif

ifeq ($(BINTYPE),DYNAMICLIB)
$(info Dynamic library mode.)
ifeq ($(OS),Windows_NT)
DYNAMICLIBNAME = lib$(BINNAME).dll
else
DYNAMICLIBNAME = lib$(BINNAME).so
endif
ifeq "$(GCCVERSIONGTEQ45)" "1"
CFLAGS = -std=c99 -O3 -fexpensive-optimizations -flto -fwhole-program -march=native -Wall -pedantic -fPIC -DCOMPILEASLIBRARY
else
CFLAGS = -std=c99 -O3 -fexpensive-optimizations -march=native -Wall -pedantic -fPIC -DCOMPILEASLIBRARY
endif
CLIBFLAGS = -shared -Wl,-soname,$(DYNAMICLIBNAME)
all: DYNAMICLIBRARY
endif

-include $(SRCPATH)/generic.mk