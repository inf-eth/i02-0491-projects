DEPTH:=../..
SUBDIR:= 	DES\
		RC4

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
