#include "mapreader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

CellPositions MapReader::parseBoard(const QString &filePath) {
    QFile file(filePath);
    CellPositions cells(9);

    for (int i = 0; i < 9; ++i)
        cells[i].reserve(5);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return cells;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.trimmed().isEmpty())
            lines.append(line);
    }
    file.close();
    for (int i = 1; i < 10; i++){
        int slash_num = 0;
        for (int j = 0; j < lines[i].length(); ++j) {
            if (lines[i][j] == "/"){
                if (j + 1 >= lines[i].length()) {
                    break;
                }
                TerrainType terrain = TerrainType::Free;
                int owner = 0;
                QChar ch = lines[i][j + 1];
                if (ch == '#') terrain = TerrainType::Rock;
                else if (ch == '~') terrain = TerrainType::Water;
                else if (ch == '1') owner = 1;
                else if (ch == '2') owner = 2;
                cells[i - 1].emplace_back(i - 1, slash_num, terrain, owner);

                slash_num++;

            }
        }
    }


    return cells;
}

