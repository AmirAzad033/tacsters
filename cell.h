#ifndef CELL_H
#define CELL_H

#include <vector>
#include "Agent.h"

enum class TerrainType {
    Free,
    Water,
    Rock
};
//implementing the cell
class Cell {
public:
    int row;
    int col;
    TerrainType terrain;
    int owner;

    std::vector<Cell*> neighbors;
    bool visited = false;
    Cell* parent = nullptr;
    int distance = -1;
    Agent* occupiedAgent = nullptr;

    Cell();
    Cell(int r, int c, TerrainType t, int o = 0);
};

#endif
