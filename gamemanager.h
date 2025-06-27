#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "cell.h"
#include "agent.h"
#include <vector>
#include <QPoint>

using Board = std::vector<std::vector<Cell>>;

class GameManager {
public:
    GameManager(const Board& boardData);

    Cell* accessCell(int row, int col);

    void addAgentToCell(Agent* agent, int row, int col);
    void removeAgentFromCell(int row, int col);
    void transferAgentToCell(Agent* agent, int startRow, int startCol, int endRow, int endCol);
    void resetStartZones();

    std::vector<Cell*> computeReachableCells(Cell* origin, int movePoints, int controllingPlayer);

    int countPlayerAgents(int playerId) const;

    void resetTraversal();
    int getRows() const { return m_board.size(); }
    int getCols(int row) const { return m_board[row].size(); }

private:
    Board m_board;
protected:
    void configureNeighbors();
};

#endif
