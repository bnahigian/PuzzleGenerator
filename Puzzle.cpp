#include "Puzzle.h"
#include "Timer.h"

//Puzzle
Puzzle::Puzzle(int nRows, int nColumns, int minVal, int maxVal)
{
	m_Rows = nRows;
	m_Cols = nColumns;
	m_min = minVal;
	m_max = maxVal;
	m_puzzle = generatePuzzle();
	m_bestSol = NULL;
}

Puzzle::~Puzzle()
{
	for (int i = 0; i < m_Rows; i++)
	{
		delete(m_puzzle[i]);
	}
}

void Puzzle::printPuzzle()
{
	for (int i = 0; i < m_Rows; i++)
	{
		for (int j = 0; j < m_Cols; j++)
		{
			printf("%d ", m_bestSol->m_puzzle[i][j].m_value);
		}

		printf("\n");
	}
	
	//now print shortest path
	for (std::list<direction>::const_iterator itr = m_bestSol->m_puzzle[m_Rows - 1][m_Cols - 1].m_pathTo.begin(), end = m_bestSol->m_puzzle[m_Rows - 1][m_Cols - 1].m_pathTo.end(); itr != end; ++itr)
	{
		if ((*itr) == UP)
		{
			printf("Up, ");
		}
		else if ((*itr) == DOWN)
		{
			printf("Down, ");
		}
		else if ((*itr) == LEFT)
		{
			printf("Left, ");
		}
		else
		{
			printf("Right, ");
		}
	}
	printf("\n");
}

Cell** Puzzle::generatePuzzle()
{
	srand(time(NULL));
	Cell** newPuz = new Cell*[m_Rows];

	for (int i = 0; i < m_Rows; i++)
	{
		newPuz[i] = new Cell[m_Cols];

		for (int j = 0; j < m_Cols; j++)
		{
			int val = rand() % (m_max - m_min + 1) + m_min;
			newPuz[i][j].m_value = val;
			newPuz[i][j].m_row = i;
			newPuz[i][j].m_col = j;
		}
	}
	newPuz[0][0].m_fCost = 0;//set first cost to 0
	newPuz[m_Rows - 1][m_Cols - 1].m_value = 0;

	return newPuz;
}

void Puzzle::checkPuzzle()
{
	PuzSolution* sol = new PuzSolution();
	dijkstra(sol);
	sol->CalculateScore();
	if (m_bestSol == NULL)
	{
		m_bestSol = sol;
	}
	else if (m_bestSol->m_score < sol->m_score)
	{
		m_bestSol = sol;
	}
}

void Puzzle::dijkstra(PuzSolution* sol)
{
	//set of unexploredNodes
	std::list<Cell*> nodes;
	for (int i = 0; i < m_Rows; i++)
	{
		for (int j = 0; j < m_Cols; j++)
		{
			m_puzzle[i][j].m_explored = false;
			if (i == 0 && j ==0)
			{
				m_puzzle[i][j].m_fCost = 0;
			}
			else
			{
				m_puzzle[i][j].m_fCost = INT_MAX;
			}
			m_puzzle[i][j].m_pathTo.clear();
			m_puzzle[i][j].m_Parent = NULL;
			calculateConnectedCells(&m_puzzle[i][j]);
			nodes.push_back(&m_puzzle[i][j]);
		}
	}

	printf("Starting dijkstra's\n");
	for (int i = 0; i < m_Rows*m_Cols-1; i++)
	{
		//pick min distance vertex of those not visited
		Cell* l_cell = lowestCostCell(nodes);

		//update value of all childrenNodes
		//set fcost and parentNode and steps/direction to get there

		for (std::list<Cell*>::const_iterator itr = l_cell->m_connectedCells.begin(), end = l_cell->m_connectedCells.end(); itr != end; ++itr)
		{
			int val = (*itr)->m_fCost;
			if ((l_cell->m_fCost + 1) < val)
			{
				Cell* child = *itr;
				child->m_Parent = l_cell;
				child->m_fCost = l_cell->m_fCost + 1;
				child->m_pathTo.empty();
				child->m_pathTo = l_cell->m_pathTo;
				//update Path to
				if (child->m_row < l_cell->m_row)
				{
					//up
					child->m_pathTo.push_back(UP);
				}
				else if (child->m_row > l_cell->m_row)
				{
					//down
					child->m_pathTo.push_back(DOWN);
				}
				else if (child->m_col < l_cell->m_col)
				{
					//left
					child->m_pathTo.push_back(LEFT);
				}
				else if (child->m_col > l_cell->m_col)
				{
					//right
					child->m_pathTo.push_back(RIGHT);
				}
			}
		}
	}

	sol->m_puzzle = m_puzzle;
}

void Puzzle::calculateConnectedCells(Cell* in_Cell)
{
	int l_val = in_Cell->m_value;
	int l_row = in_Cell->m_row;
	int l_col = in_Cell->m_col;
	if (l_row + l_val < m_Rows)
	{
		in_Cell->m_connectedCells.push_back(&m_puzzle[l_row + l_val][l_col]);
	}
	if (l_row - l_val >= 0)
	{
		in_Cell->m_connectedCells.push_back(&m_puzzle[l_row - l_val][l_col]);
	}
	if (l_col + l_val < m_Cols)
	{
		in_Cell->m_connectedCells.push_back(&m_puzzle[l_row][l_col+l_val]);
	}
	if (l_col - l_val >= 0)
	{
		in_Cell->m_connectedCells.push_back(&m_puzzle[l_row][l_col-l_val]);
	}
}

//returns the lowest cost cell in the unexplored set
Cell* Puzzle::lowestCostCell(std::list<Cell*> in_current)
{
	Cell* l_cell = in_current.back();

	for (std::list<Cell*>::const_iterator itr = in_current.begin(), end = in_current.end(); itr != end; ++itr)
	{
		if ((*itr)->m_explored == false)
		{
			if (l_cell->m_fCost > (*itr)->m_fCost)
			{
				l_cell = *itr;
			}
			else if (l_cell->m_fCost == (*itr)->m_fCost)//if tied choose lower row + col
			{
				int val1 = l_cell->m_col + l_cell->m_row;
				int val2 = (*itr)->m_col + (*itr)->m_row;
				if (val2 < val1)
				{
					l_cell = *itr;
				}
			}
		}
	}

	l_cell->m_explored = true;

	return l_cell;
}

//PuzSolution
void PuzSolution::CalculateScore()
{
	m_score = m_path.size() * 5;
	if (m_unique)
	{
		m_score += (m_rows*m_cols);
	}

	m_score -= (2 * (m_blackHoles+m_whiteHoles+m_backwardForced + m_forwardForced));
}