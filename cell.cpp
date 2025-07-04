#include "cell.h"

Cell::Cell()
    : row(0), col(0), terrain(TerrainType::Free), owner(0)
{}

Cell::Cell(int r, int c, TerrainType t, int o)
    : row(r), col(c), terrain(t), owner(o)
{}
