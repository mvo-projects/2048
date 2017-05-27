CC=g++
OPT=-std=c++11 -Wall -O1
EXEC=2048

.PHONY: all re clean

all: 2048

2048:
	$(CC) $(OPT) -Iincludes/ game.cpp ai.cpp gui.cpp -o $(EXEC) -lGL -lglut -fopenmp

clean:
	rm -rf *.o *.exe $(EXEC)  2>/dev/null

re:
	make clean ; make all

