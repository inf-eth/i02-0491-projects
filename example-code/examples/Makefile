DEPTH:=..
SUBDIR:= 	alarm\
		bitset\
		FileHandling\
		HexPrinting\
		Integration\
		LALab12\
		LinuxVsWindowsThreadTest\
		MallocFree\
		ProgramExecutionTime\
		StringStream\
		LinuxFileSize\
		ArrayShift\
		VLI\
		VLI64bit\
		HighPrecisionTimer\
		Dynamic2DArray\
		SymmetricEncryption\
		LZ77\
		RadixTree\
		MatrixMultiplication

all:
	for dir in $(SUBDIR);\
		do\
			$(MAKE) --directory=$$dir;\
		done
clean:
	for dir in $(SUBDIR);\
		do\
			$(MAKE) --directory=$$dir clean;\
		done
