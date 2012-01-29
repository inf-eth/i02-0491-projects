# Makefile for LinuxVsWindowsThreadTest. Portable on windows or linux.
CC=g++
DEPS=-I./
SRCDIR=./
TARGET=LinuxVsWindowsThreadTest

# debug or release configuration. Default is release.
# 'make dbg=1' will use debug configuration.
ifdef dbg
CFLAGS:=-g3
BINDIR:=../../bin/debug
ODIR:=./obj/debug
else
CFLAGS:=-O3
BINDIR:=../../bin/release
ODIR:=./obj/release
endif

_OBJ=LinuxVsWindowsThreadTest.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(BINDIR)/LinuxVsWindowsThreadTest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(DEPS) $(CFLAGS)

# Object and binary files must be placed in obj and bin directories.
# Create bin and obj directories if they do not exist before compiling.

$(OBJ): | $(ODIR) $(BINDIR)

$(ODIR):
	mkdir -p $(ODIR)
$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: clean

clean:
	rm -f ./obj/debug/*.o ./obj/release/*.o ../../bin/debug/LinuxVsWindowsThreadTest ../../bin/release/LinuxVsWindowsThreadTest
