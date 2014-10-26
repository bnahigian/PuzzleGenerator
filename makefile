CC = g++
CFLAGS = -O3
EXEC = PuzzleGenerator
	
all:
	$(CC) $(CFLAGS) -o $(EXEC) Puzzle.cpp main.cpp
	
clean:
	rm -f $(EXEC)

test:
	make clean		
	make all
	./$(EXEC) 10 10 1 9
	