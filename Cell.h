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
		m_backCost = INT_MAX;
		m_explored = false;
		m_backwardsExplore = false;
		m_parents = 0;
		m_backwardsparents = 0;
	}

	Cell()
	{
		m_value = 0;
		m_row = 0;
		m_col = 0;
		m_fCost = INT_MAX;//infinity
		m_backCost = INT_MAX;
		m_explored = false;
		m_backwardsExplore = false;
		m_parents = 0;
		m_backwardsparents = 0;
	}

	int m_value; //value stored in cell
	int m_row; //row for cell
	int m_col; //column for cell
	int m_fCost; //cost of getting to cell from start
	int m_backCost; //cost of getting to cell from end
	bool m_explored; //if this cell has been explored
	bool m_backwardsExplore; //if this cell has been explored by backwards check
	int m_parents; //number of ways to get to this node
	int m_backwardsparents; //number of ways to get to this node backwards
	std::list<Cell*> m_connectedCells; //Cells you can reach from this one
	std::list<direction>  m_pathTo; //Path to get to this cell
	std::list<Cell*> m_reachedBy; //Cells that can reach this Cell
	Cell* m_Parent;//the Cell that reaches this one by the shortest path
};
#endif