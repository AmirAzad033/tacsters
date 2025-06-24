#ifndef MAPREADER_H
#define MAPREADER_H

#include "cell.h"
#include <vector>
#include <QString>

using CellPositions = std::vector<std::vector<Cell>>;

class MapReader {
public:
    static CellPositions parseBoard(const QString &path);
};


#endif
