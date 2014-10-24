#include "Puzzle.h"
#include "Timer.h"

//Puzzle
Puzzle::Puzzle(int nRows, int nColumns, int minVal, int maxVal)
{
	m_Rows = nRows;
	m_Cols = nColumns;
	m_min = minVal;
	m_max = maxVal;
	m_bestSol = NULL;
}

Puzzle::~Puzzle()
{
}
//done
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
	for (std::list<direction>::const_iterator itr = m_bestSol->m_puzzle[m_Rows - 1][m_Cols - 1].m_path.begin(), end = m_bestSol->m_puzzle[m_Rows - 1][m_Cols - 1].m_path.end(); itr != end; ++itr)
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

	printf("Solution: %s\n", m_bestSol->m_solution ? "true" : "false");
	printf("Unique: %s\n", m_bestSol->m_unique ? "true" : "false");
	printf("Solution Length: %d\n", m_bestSol->m_path.size());
	printf("# of black holes: %d\n", m_bestSol->m_blackHoles);
	printf("# of white holes: %d\n", m_bestSol->m_whiteHoles);
	printf("# of forced forwards: %d\n", m_bestSol->m_forwardForced);
	printf("# of forced backwards: %d\n", m_bestSol->m_backwardForced);
	printf("Puzzle Score: %d\n\n", m_bestSol->m_score);
}
//done
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
//done (no hillclimbing yet)
void Puzzle::checkPuzzle()
{
	PuzSolution* sol = new PuzSolution(m_Rows, m_Cols);

	if (m_bestSol == NULL || m_bestSol->m_solution == false) //we want to try and generate a puzzle with a solution first. then hill climb
	{
		sol->m_puzzle = generatePuzzle();
	}
	else//perform hillclimbing here
	{
		for (int i = 0; i < m_Rows; i++)
		{
			for (int j = 0; j < m_Cols; j++)
			{
				sol->m_puzzle[i][j] = m_bestSol->m_puzzle[i][j];
			}
		}
		hillClimb(sol);
	}
	
	dijkstra(sol);
	sol->CalculateScore();
	if (m_bestSol == NULL)
	{
		m_bestSol = sol;
	}
	else if (m_bestSol->m_score < sol->m_score)
	{
		//printPuzzle();

		m_bestSol = sol;
	}
	else
	{
		//random chance of keeping it in the future
		sol->~PuzSolution();
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
			sol->m_puzzle[i][j].m_explored = false;
			sol->m_puzzle[i][j].m_backwardsExplore = false;
			sol->m_puzzle[i][j].m_fCost = INT_MAX;
			sol->m_puzzle[i][j].m_backCost = INT_MAX;
			sol->m_puzzle[i][j].m_path.clear();
			sol->m_puzzle[i][j].m_Parent = NULL;
			sol->m_puzzle[i][j].m_pathesTo = 0;
			sol->m_puzzle[i][j].m_backwardspathesTo = 0;
			sol->m_puzzle[i][j].m_reachedBy.empty();
			calculateConnectedCells(&sol->m_puzzle[i][j], sol);
			nodes.push_back(&sol->m_puzzle[i][j]);
		}
	}
	sol->m_puzzle[0][0].m_fCost = 0;
	sol->m_puzzle[0][0].m_pathesTo = 1;
	sol->m_puzzle[m_Rows - 1][m_Cols - 1].m_backCost = 0;
	sol->m_puzzle[m_Rows - 1][m_Cols - 1].m_backwardspathesTo = 1;
	unexplored = nodes.size();
	while (unexplored>0)
	{
		//pick min distance vertex of those not visited
		Cell* l_cell = lowestCostCell(nodes);

		//update value of all childrenNodes
		//set fcost and parentNode and steps/direction to get there
		for (std::list<Cell*>::const_iterator itr = l_cell->m_connectedCells.begin(), end = l_cell->m_connectedCells.end(); itr != end; ++itr)
		{
			Cell* child = *itr;
			int val = child->m_fCost;
			child->m_pathesTo += l_cell->m_pathesTo;//Check this
			child->m_reachedBy.push_back(l_cell);//used for backwards search
			if (l_cell->m_fCost != INT_MAX)
			{
				if ((l_cell->m_fCost + 1) < val)
				{
					if (child->m_row == m_Rows - 1 & child->m_col == m_Cols - 1)
					{
						sol->m_unique = true;
					}
					child->m_Parent = l_cell;
					child->m_fCost = l_cell->m_fCost + 1;
					child->m_path.empty();
					child->m_path = l_cell->m_path;
					//update Path to
					if (child->m_row < l_cell->m_row)
					{
						//up
						child->m_path.push_back(UP);
					}
					else if (child->m_row > l_cell->m_row)
					{
						//down
						child->m_path.push_back(DOWN);
					}
					else if (child->m_col < l_cell->m_col)
					{
						//left
						child->m_path.push_back(LEFT);
					}
					else if (child->m_col > l_cell->m_col)
					{
						//right
						child->m_path.push_back(RIGHT);
					}
				}
				else if ((l_cell->m_fCost + 1) == val)
				{
					if (child->m_row == m_Rows - 1 & child->m_col == m_Cols - 1)
					{
						sol->m_unique = false;
					}
				}
			}
		}
	}

	//backwards explore here
	backwardsExplore(nodes);

	//find stats here
	findStats(nodes, sol);

}

void Puzzle::calculateConnectedCells(Cell* in_Cell, PuzSolution* sol)
{
	int l_val = in_Cell->m_value;
	int l_row = in_Cell->m_row;
	int l_col = in_Cell->m_col;

	in_Cell->m_connectedCells.empty();//make sure to clear this list

	if (l_row + l_val < m_Rows) //Down
	{
		in_Cell->m_connectedCells.push_back(&sol->m_puzzle[l_row + l_val][l_col]);
	}
	if (l_row - l_val >= 0) //Up
	{
		in_Cell->m_connectedCells.push_back(&sol->m_puzzle[l_row - l_val][l_col]);
	}
	if (l_col + l_val < m_Cols) //Right
	{
		in_Cell->m_connectedCells.push_back(&sol->m_puzzle[l_row][l_col + l_val]);
	}
	if (l_col - l_val >= 0) //Left
	{
		in_Cell->m_connectedCells.push_back(&sol->m_puzzle[l_row][l_col - l_val]);
	}
}

void Puzzle::backwardsExplore(std::list<Cell*> nodes)
{
	unexplored = nodes.size();

	while (unexplored>0)
	{
		//pick min distance vertex of those not visited
		Cell* l_cell = lowestCostCellBackwards(nodes);

		//update value of all childrenNodes
		//set fcost and parentNode and steps/direction to get there
		for (std::list<Cell*>::const_iterator itr = l_cell->m_reachedBy.begin(), end = l_cell->m_reachedBy.end(); itr != end; ++itr)
		{
			int val = (*itr)->m_fCost;
			Cell* child = *itr;
			child->m_backwardspathesTo += l_cell->m_backwardspathesTo;
			if ((l_cell->m_fCost + 1) < val)
			{
				child->m_fCost = l_cell->m_fCost + 1;
			}
		}
	}
}

void Puzzle::findStats(std::list<Cell*> nodes, PuzSolution* sol)
{
	Cell goal = sol->m_puzzle[m_Rows - 1][m_Cols - 1];//goal cell
	if (goal.m_path.size() >= 2)//min path length
	{
		sol->m_solution = true;

		for (std::list<Cell*>::const_iterator itr = nodes.begin(), end = nodes.end(); itr != end; ++itr)
		{
			Cell* temp = (*itr);
			if (temp->m_explored == false)
			{
				sol->m_whiteHoles++; //can't be reached from beginning so not reachable
			}
			else
			{
				if (temp->m_connectedCells.size() == 1)//only one action leaves reachable cell
				{
					sol->m_forwardForced++;
				}
			}
			if (temp->m_backwardsExplore == false) //can't be reached from end so not reaching
			{
				sol->m_blackHoles++;
			}
			else
			{
				if (temp->m_pathesTo == 1)//only one cell enters this one
				{
					sol->m_backwardForced++;
				}
			}
		}
		sol->m_path = sol->m_puzzle[m_Rows - 1][m_Cols - 1].m_path;
	}

}

void Puzzle::hillClimb(PuzSolution* sol)
{
	Cell* l_cell = &sol->m_puzzle[m_Rows - 1][m_Cols - 1];
	for (std::list<Cell*>::const_iterator itr = l_cell->m_reachedBy.begin(), end = l_cell->m_reachedBy.end(); itr != end; ++itr)
	{
		int val = (*itr)->m_value;

		if (val <= m_min)
		{
			(*itr)->m_value = m_max;
		}
		else
		{
			(*itr)->m_value-=1;
		}
	}//*/
}

//returns the lowest cost cell in the unexplored set
//Done
Cell* Puzzle::lowestCostCell(std::list<Cell*> in_current)
{
	Cell* l_cell = NULL;

	for (std::list<Cell*>::const_iterator itr = in_current.begin(), end = in_current.end(); itr != end; ++itr)
	{
		if ((*itr)->m_explored == false)
		{
			if (l_cell == NULL)
			{
				l_cell = *itr;
			}
			else
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
	}
	if (l_cell->m_explored == false)
	{
		unexplored -= 1;
	}
	l_cell->m_explored = true;

	return l_cell;
}
//Done
Cell* Puzzle::lowestCostCellBackwards(std::list<Cell*> in_current)
{
	Cell* l_cell = NULL;

	for (std::list<Cell*>::const_iterator itr = in_current.begin(), end = in_current.end(); itr != end; ++itr)
	{
		if ((*itr)->m_backwardsExplore == false)
		{
			if (l_cell == NULL)
			{
				l_cell = (*itr);
			}
			else
			{
				if (l_cell->m_fCost > (*itr)->m_fCost)
				{
					l_cell = *itr;
				}
				else if (l_cell->m_fCost == (*itr)->m_fCost)//if tied choose higher row + col
				{
					int val1 = l_cell->m_col + l_cell->m_row;
					int val2 = (*itr)->m_col + (*itr)->m_row;
					if (val2 > val1)
					{
						l_cell = *itr;
					}
				}
			}
		}
	}

	if (l_cell->m_backwardsExplore == false)
	{
		unexplored -= 1;
	}
	l_cell->m_backwardsExplore = true;

	return l_cell;
}

//PuzSolution
//Done
void PuzSolution::CalculateScore()
{
	if (m_solution)
	{
		m_score = m_path.size() * 5;
		if (m_unique)
		{
			m_score += (m_rows*m_cols);
		}
	}
	else
	{
		m_unique = false;
		m_score = -400;//penalty for no solution
	}
	m_score -= (2 * (m_blackHoles+m_whiteHoles+m_backwardForced + m_forwardForced));
}