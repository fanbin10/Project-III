CPP = g++
CFLAGS = -Wall -ansi -g -c

all: evo

evo: main.o evolution.o 
	$(CPP)  main.o evolution.o  -o evo

main.o: main.cpp
	$(CPP) $(CFLAGS) main.cpp -o main.o

evolution.o: evolution.cpp
	$(CPP) $(CFLAGS) evolution.cpp -o evolution.o

check-syntax:
	$(CPP) -o nul -S ${CHK_SOURCES}
run:
	./evo

clean:

	rm -rf *.o *.txt shoot