#include "mapreader.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

MapReader::MapReader() : mapId(-1) {}

CellPosition::Type MapReader::charToCellType(QChar c) const
{
    switch (c.unicode()) {
    case '1': return CellPosition::Player1;
    case '2': return CellPosition::Player2;
    case '~': return CellPosition::Water;
    case '#': return CellPosition::Stone;
    default: return CellPosition::Empty;
    }
}


bool MapReader::loadRandomMap(const QString& mapsFolder)
{
    cellPositions.clear();

    QDir directory(mapsFolder);
    QStringList maps = directory.entryList(QStringList() << "*.txt", QDir::Files);

    if (maps.isEmpty()) {
        qDebug() << "No map files found in" << mapsFolder;
        return false;
    }

    int randomIndex = QRandomGenerator::global()->bounded(maps.size());
    QString mapFile = directory.filePath(maps[randomIndex]);
    QString baseName = QFileInfo(mapFile).baseName();
    mapId = baseName.mid(3).toInt();

    QFile file(mapFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open map file:" << mapFile;
        return false;
    }

    QTextStream in(&file);
    QVector<QString> lines;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.trimmed().isEmpty()) {
            lines.append(line);
        }
    }
    file.close();
    for (int row = 0; row < lines.size(); ++row) {
        const QString& line = lines[row];

        if (line.trimmed().isEmpty()) continue;

        bool isInterRow = (row % 2 == 1);
        int logicalRow = row / 2;

        for (int col = 0; col < line.length(); ++col) {
            QChar c = line[col];
            if (c == '/' ||  c == '\\' ||  c == '_' || c == ' ') continue;

            CellPosition::Type type = charToCellType(c);
            if (type != CellPosition::Empty) {
                int logicalCol = col / 3;

                CellPosition pos;
                pos.row = logicalRow;
                pos.col = logicalCol;
                pos.type = type;

                cellPositions.append(pos);

            }
        }
    }

    return !cellPositions.isEmpty();
}
