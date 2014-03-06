CC = g++
CPPFLAGS = -std=c++0x -Wall -O3

OBJ = Main.o TileMulti.o matrix.o RecMulti.o NaiveMatrixMulti.o

all: Main

Main: $(OBJ) -lrt

clean:
	$(RM) Main $(OBJ)
