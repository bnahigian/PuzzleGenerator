#ifndef PUZZLE_H
#define PUZZLE_H
#include <stdio.h>
#include <cstdlib>
#include <climits>
#include "Cell.h"

class PuzSolution
{
public:
	PuzSolution(int in_rows, int in_cols) :m_solution(false), m_unique(false), m_rows(in_rows), m_cols(in_cols), m_backwardForced(0), m_forwardForced(0), m_blackHoles(0), m_whiteHoles(0), m_score(0)
	{
		m_puzzle = new Cell*[in_rows];

		for (int i = 0; i < in_rows; i++)
		{
			m_puzzle[i] = new Cell[in_cols];

			for (int j = 0; j < in_cols; j++)
			{
				m_puzzle[i][j].m_row = i;
				m_puzzle[i][j].m_col = j;
			}
		}
	};
	~PuzSolution()
	{
		m_path.clear();

		delete m_puzzle;
	};

	void CalculateScore();

	Cell** m_puzzle; //The puzzle for the best solution
	bool m_solution; //if there is a solution or not
	bool m_unique; //if there is a unique solution or not
	std::list<direction> m_path; //the shortest path from the start to the goal cell
	int m_rows; //number of rows
	int m_cols; //number of cols
	int m_blackHoles; //number of cells reachable from the beginning but don't reach end
	int m_whiteHoles; //number of cells that are not reachable from the beginning but can reach the end
	int m_forwardForced; //number of cells that only have 1 connected Cell and is reachable (can get to from start)
	int m_backwardForced; //number of cells that are only reached by 1 Cell and is a reaching cell (gets to end)
	int m_score; //the total score of the puzzle
};

class Puzzle
{
public:
	Puzzle(int nRows, int nColumns, int minVal, int maxVal);
	~Puzzle();

	void printPuzzle(); //prints the puzzle
	Cell** generatePuzzle(); //generates the puzzle using the values given
	void checkPuzzle();
	void dijkstra(PuzSolution* sol);
	void backwardsExplore(std::list<Cell*> nodes);
	void findStats(std::list<Cell*> nodes, PuzSolution* sol);
	void hillClimb(PuzSolution* sol);
	Cell* lowestCostCell(std::list<Cell*>* in_current);
	Cell* lowestCostCellBackwards(std::list<Cell*>* in_current);

	void calculateConnectedCells(Cell* in_Cell, PuzSolution* sol);

	int m_Rows; //the number of rows entered
	int m_Cols; //the number of cols entered
	int m_min; // the min value in the range entered
	int m_max; //the max value in the range entered
	PuzSolution* m_bestSol; //the best puzzle solution so far
};
#endif
