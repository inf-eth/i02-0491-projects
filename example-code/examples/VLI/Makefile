# Generic Makefile.
# Input names of cpp files (without .cpp extension) against CPPFILES separated by spaces.
# Mention output binary filename against TARGET.
#
# CC = compiler name
# DEPS = any additional folders containing header apart from system headers. Default is current directory.
# SRCDIR = directory containing source files. Default is current directory.
# CPPFILES = names of cpp files to be compiled.
# TARGET = output binary file name.
# LIBS = any libraries to link with (e.g. -lpthread). Default is none.
# CFLAGS = additional compiler flags

# Number of threads to be spawned
export OMP_NUM_THREADS=2

CC=g++
DEPS=-I.
SRCDIR=.
CPPFILES=VLI VLIMain
TARGET=VLI
LIBS=-lpthread
CFLAGS=-fopenmp

include ../rules.mk
