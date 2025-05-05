#ifndef MAPREADER_H
#define MAPREADER_H

#include <QString>
#include <QVector>

struct CellPosition {
    int row;
    int col;
    enum Type { Empty, Player1, Player2, Stone, Water } type;
};

class MapReader
{
public:
    MapReader();
    bool loadRandomMap(const QString& mapsFolder);
    QVector<CellPosition> getCellPositions() const { return cellPositions; }
    int getMapId() const { return mapId; }

private:
    QVector<CellPosition> cellPositions;
    int mapId;

    CellPosition::Type charToCellType(QChar c) const;
};

#endif // MAPREADER_H
