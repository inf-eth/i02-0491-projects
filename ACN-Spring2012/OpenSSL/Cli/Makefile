#Makefile for OpenSSL Commandline wrapper in C++
CC=g++
CFLAGS=-I.
DEPS=OpenSSLCli.h
ODIR=./obj
BINDIR=./bin
_OBJ=CliMain.o OpenSSLCli.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(BINDIR)/OpenSSLCli: $(OBJ)
	$(CC) -o $@ $^ -I.

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Object and binary files must be placed in obj and bin directories.
# Create bin and obj directories if they do not exist before compiling.

$(OBJ): | $(ODIR) $(BINDIR)

$(ODIR):
	mkdir -p $(ODIR)
$(BINDIR):
	mkdir -p $(BINDIR)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BINDIR)/OpenSSLCli
