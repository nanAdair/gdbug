objects = main.o log.o file.o section.o symbol.o relocation.o version.o instruction.o upm.o
CC = g++
CFLAGS = -c -std=c++11
LIBNAME = capstone

main: $(objects)
	$(CC) -o main $(objects) -l$(LIBNAME)

main.o: main.cc type.h
	$(CC) $(CFLAGS) main.cc

file.o: file.cc file.h type.h
	$(CC) $(CFLAGS) file.cc

section.o: section.cc section.h type.h sectionGD.h
	$(CC) $(CFLAGS) section.cc

symbol.o: symbol.cc symbol.h type.h
	$(CC) $(CFLAGS) symbol.cc

relocation.o: relocation.cc relocation.h type.h 
	$(CC) $(CFLAGS) relocation.cc

version.o: version.cc version.h type.h
	$(CC) $(CFLAGS) version.cc

instruction.o: instruction.cc instruction.h utilsGD.h
	$(CC) $(CFLAGS) instruction.cc

upm.o: upm.cc upm.h
	$(CC) $(CFLAGS) upm.cc

log.o: log.cc log.h
	$(CC) $(CFLAGS) log.cc

.PHONY: clean
clean:
	-rm main $(objects)
