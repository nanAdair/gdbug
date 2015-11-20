objects = main.o file.o section.o log.o
CC = g++
CFLAGS = -c -std=c++11

main: $(objects)
	$(CC) -o main $(objects)

main.o: main.cc type.h
	$(CC) $(CFLAGS) main.cc

file.o: file.cc file.h type.h
	$(CC) $(CFLAGS) file.cc

section.o: section.cc section.h type.h
	$(CC) $(CFLAGS) section.cc

log.o: log.cc log.h
	$(CC) $(CFLAGS) log.cc

.PHONY: clean
clean:
	-rm main $(objects)
