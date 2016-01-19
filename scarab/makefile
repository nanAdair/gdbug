objects = main.o log.o file.o section.o symbol.o relocation.o version.o instruction.o disasm.o dis-table.o operand.o upm.o block.o function.o edge.o utility.o asm.o asm-table.o
CC = g++
CFLAGS = -c -std=c++11

main: $(objects)
	$(CC) -o main $(objects)

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
disasm.o: disasm.cpp disasm.h dis-table.h type.h dis.h utility.h
	g++ -c -std=c++11 disasm.cpp
dis-table.o: dis-table.cpp dis-table.h
	g++ -c -std=c++11 dis-table.cpp
operand.o: type.h dis-table.h
	g++ -c -std=c++11 operand.cc
utility.o: utility.h utility.cpp
	g++ -c -std=c++11 utility.cpp
asm-table.o: asm-table.cpp asm-table.h asm.h asm.cpp
	g++ -c -std=c++11 asm-table.cpp
asm.o: asm.cpp asm.h utility.h utility.cpp dis.h type.h
	g++ -c -std=c++11 asm.cpp

block.o: type.h block.h
	$(CC) $(CFLAGS) block.cc

function.o: type.h function.h function.cc
	$(CC) $(CFLAGS) function.cc

edge.o: type.h edge.h edge.cc
	$(CC) $(CFLAGS) edge.cc

upm.o: upm.cc upm.h
	$(CC) $(CFLAGS) upm.cc

log.o: log.cc log.h
	$(CC) $(CFLAGS) log.cc

.PHONY: clean
clean:
	-rm -rf main $(objects)
