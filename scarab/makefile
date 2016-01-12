objects = main.o log.o file.o section.o symbol.o relocation.o version.o instruction.o disasm.o dis-table.o operand.o upm.o
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

relocation.o: relocation.cc relocation.h type.h upm.h
	$(CC) $(CFLAGS) relocation.cc

version.o: version.cc version.h type.h
	$(CC) $(CFLAGS) version.cc

instruction.o: instruction.cc instruction.h utilsGD.h type.h
	$(CC) $(CFLAGS) instruction.cc

disasm.o: disasm.cpp disasm.h dis-table.h type.h
	g++ -c -std=c++11 disasm.cpp
dis-table.o: dis-table.cpp dis-table.h instruction.h
	g++ -c -std=c++11 dis-table.cpp
operand.o: type.h dis-table.h
	g++ -c -std=c++11 operand.cc

upm.o: upm.cc upm.h
	$(CC) $(CFLAGS) upm.cc

log.o: log.cc log.h
	$(CC) $(CFLAGS) log.cc

.PHONY: clean
clean:
	-rm main $(objects)
