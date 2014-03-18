CC = g++
CPPFLAGS = -std=c++0x -Wall -O3 -g

OBJ = Main.o LSDRadix.o MSDRadix.o MSDRadix_countSort.o CountingSort.o MultiCoreRadix.o QuickSort.o

all: Main

Main: $(OBJ) -lrt -lpthread

clean:
	rm Main *.o
