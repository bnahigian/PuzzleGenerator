#ifndef CELL_H
#define CELL_H
#include <list>
enum direction { UP, DOWN, LEFT, RIGHT };
class Cell{
	public:
	Cell(int in_val, int in_row, int in_col)
	{
		m_value = in_val;
		m_row = in_row;
		m_col = in_col;
		m_fCost = INT_MAX;//infinity
		m_explored = false;
	}

	Cell()
	{
		
	}

	int m_value;
	int m_row;
	int m_col;
	int m_fCost;
	bool m_explored;
	std::list<Cell*> m_connectedCells;
	std::list<direction>  m_pathTo;
	Cell* m_Parent;//if there is a shortest path, 
};
#endif