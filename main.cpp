/*
NAME: 
STUDENT ID: 
*/

#include <cstdlib>
#include <iostream>
//#include <sys/time.h>
#include <stdio.h>
#include "Timer.h"
#include "Puzzle.h"

using namespace std;

void GeneratePuzzle(int nRows, int nColumns, int minVal, int maxVal, Timer* t)
{
	Puzzle* puz = new Puzzle(nRows, nColumns, minVal, maxVal);
	puz->checkPuzzle();
	int time = t->GetElapsedTime();
	while ( time< 55)
	{
		puz->generatePuzzle();
		puz->checkPuzzle();
		time = t->GetElapsedTime();
	}

	puz->printPuzzle();
}

int main(int argc, char **argv)
{
	// Process the arguments
	if (argc != 5)
	{
		printf("Please specify the number of rows and columns and the minimum and maximum values for each cell (requires 4 parameters)\n");
		exit(0);
	}
	int nRows = atoi(argv[1]);
	int nColumns = atoi(argv[2]);
	int minVal = atoi(argv[3]);
	int maxVal = atoi(argv[4]);

	// Start the timer
	Timer t;
	t.StartTimer();
  	
	// Generate the puzzle
	printf("Generating a %dx%d puzzle with values in range [%d-%d]\n", nRows, nColumns, minVal, maxVal);
	
	GeneratePuzzle(nRows, nColumns, minVal, maxVal, &t);
		
	// Print the elapsed time
    printf("Total time: %.6lf seconds\n", t.GetElapsedTime());

	int x;
	cin >> x;

	return 0;
}
