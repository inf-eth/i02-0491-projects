# Automated Worm Fingerprinting Makefile.
# Copyright (c) 2012 Attique Dawood, Awais Aslam
#
# This file is part of Automated Worm Fingerprinting.
#
# Automated Worm Fingerprinting is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Automated Worm Fingerprinting is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################
#
# Input names of cpp files (without .cpp extension) against CPPFILES separated by spaces.
# Mention output binary filename against TARGET.
#
# CC = compiler name
# DEPS = any additional folders containing header apart from system headers. Default is current directory.
# SRCDIR = directory containing source files. Default is current directory.
# CPPFILES = names of cpp files to be compiled.
# TARGET= output binary file name.
# LIBS= any libraries to link with (e.g. -lpthread). Default is none.

CC=g++
DEPS=-I./include
SRCDIR=./src
CPPFILES=AutomatedWormFingerprinting PacketManip
TARGET=AutomatedWormFingerprinting
LIBS=-lpcap

# debug or release configuration. Default is release.
# 'make dbg=1' will use debug configuration.
ifdef dbg
CFLAGS:=-g3
BINDIR:=bin/debug
ODIR:=obj/debug
else
CFLAGS:=-O3
BINDIR:=bin/release
ODIR:=obj/release
endif

OBJ=$(patsubst %,$(ODIR)/%.obj,$(CPPFILES))

$(BINDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.obj: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(DEPS) $(CFLAGS)

# Object and binary files must be placed in obj and bin directories.
# Create bin and obj directories if they do not exist before compiling.
$(OBJ): | $(ODIR) $(BINDIR)

$(ODIR):
	mkdir -p $(ODIR)
$(BINDIR):
	mkdir -p $(BINDIR)

# Default run.
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET) $(ARG)

all: $(BINDIR)/$(TARGET)

.PHONY: clean

clean:
	rm -rf bin obj
