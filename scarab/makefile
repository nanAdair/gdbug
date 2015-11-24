objects = main.o log.o file.o section.o symbol.o relocation.o version.o
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

relocation.o: relocation.cc relocation.h type.h
	$(CC) $(CFLAGS) relocation.cc

version.o: version.cc version.h type.h
	$(CC) $(CFLAGS) version.cc

log.o: log.cc log.h
	$(CC) $(CFLAGS) log.cc

.PHONY: clean
clean:
	-rm main $(objects)
