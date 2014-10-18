#ifndef PUZZLE_H
#define PUZZLE_H
#include <stdio.h>
#include <cstdlib>
#include "Cell.h"

class PuzSolution
{
public:
	PuzSolution() :m_solution(false), m_unique(false), m_rows(0), m_cols(0), m_backwardForced(0), m_forwardForced(0), m_blackHoles(0), m_whiteHoles(0), m_score(0)
	{};
	~PuzSolution();

	void CalculateScore();

	Cell** m_puzzle;
	bool m_solution;
	bool m_unique;
	std::list<direction> m_path;
	int m_rows;
	int m_cols;
	int m_blackHoles;
	int m_whiteHoles;
	int m_forwardForced;
	int m_backwardForced;
	int m_score;
};

class Puzzle
{
public:
	Puzzle(int nRows, int nColumns, int minVal, int maxVal);
	~Puzzle();

	void printPuzzle();
	Cell** generatePuzzle();
	void checkPuzzle();
	void dijkstra(PuzSolution* sol);
	void backwardsExplore(std::list<Cell*> nodes);
	void findStats(std::list<Cell*> nodes, PuzSolution* sol);
	Cell* lowestCostCell(std::list<Cell*> in_current);
	Cell* lowestCostCellBackwards(std::list<Cell*> in_current);

	void calculateConnectedCells(Cell* in_cell);

	Cell** m_puzzle;
	int m_Rows;
	int m_Cols;
	int m_min;
	int m_max;
	PuzSolution* m_bestSol;
	int unexplored;
};
#endif
