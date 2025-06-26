#include "gamemanager.h"
#include "cell.h"
#include <QDebug>
#include <QQueue>

GameManager::GameManager(const Board& boardData)
    : m_board(boardData)
{
    configureNeighbors();
}

void GameManager::configureNeighbors() {
    for (size_t rowIdx = 0; rowIdx < m_board.size(); ++rowIdx) {
        for (size_t colIdx = 0; colIdx < m_board[rowIdx].size(); ++colIdx) {
            Cell* current = &m_board[rowIdx][colIdx];
            current->neighbors.clear();

            if (accessCell(rowIdx - 2, colIdx)) current->neighbors.push_back(accessCell(rowIdx - 2, colIdx));
            if (accessCell(rowIdx + 2, colIdx)) current->neighbors.push_back(accessCell(rowIdx + 2, colIdx));

            if (rowIdx % 2 == 0) {
                if (accessCell(rowIdx - 1, colIdx)) current->neighbors.push_back(accessCell(rowIdx - 1, colIdx));
                if (accessCell(rowIdx - 1, colIdx - 1)) current->neighbors.push_back(accessCell(rowIdx - 1, colIdx - 1));
                if (accessCell(rowIdx + 1, colIdx)) current->neighbors.push_back(accessCell(rowIdx + 1, colIdx));
                if (accessCell(rowIdx + 1, colIdx - 1)) current->neighbors.push_back(accessCell(rowIdx + 1, colIdx - 1));
            } else {
                if (accessCell(rowIdx - 1, colIdx + 1)) current->neighbors.push_back(accessCell(rowIdx - 1, colIdx + 1));
                if (accessCell(rowIdx - 1, colIdx)) current->neighbors.push_back(accessCell(rowIdx - 1, colIdx));
                if (accessCell(rowIdx + 1, colIdx + 1)) current->neighbors.push_back(accessCell(rowIdx + 1, colIdx + 1));
                if (accessCell(rowIdx + 1, colIdx)) current->neighbors.push_back(accessCell(rowIdx + 1, colIdx));
            }
        }
    }
}

Cell* GameManager::accessCell(int row, int col) {
    if (row >= 0 && row < m_board.size() && col >= 0 && col < m_board[row].size()) {
        return &m_board[row][col];
    }
    return nullptr;
}

void GameManager::addAgentToCell(Agent* agent, int row, int col) {
    Cell* target = accessCell(row, col);
    if (!target) return;
    if (target->occupiedAgent) return;
    target->occupiedAgent = agent;
}

void GameManager::removeAgentFromCell(int row, int col) {
    Cell* target = accessCell(row, col);
    if (!target) return;
    if (!target->occupiedAgent) return;
    target->occupiedAgent = nullptr;
}

void GameManager::transferAgentToCell(Agent* agent, int startRow, int startCol, int endRow, int endCol) {
    Cell* startCell = accessCell(startRow, startCol);
    Cell* endCell = accessCell(endRow, endCol);

    if (!startCell || !endCell) return;
    if (startCell->occupiedAgent != agent) return;
    if (endCell->occupiedAgent) return;

    endCell->occupiedAgent = agent;
    startCell->occupiedAgent = nullptr;
}

void GameManager::resetStartZones() {
    for (size_t r = 0; r < m_board.size(); ++r) {
        for (size_t c = 0; c < m_board[r].size(); ++c) {
            m_board[r][c].owner = 0;
        }
    }
}

void GameManager::resetTraversal() {
    for (size_t r = 0; r < m_board.size(); ++r) {
        for (size_t c = 0; c < m_board[r].size(); ++c) {
            m_board[r][c].visited = false;
            m_board[r][c].parent = nullptr;
            m_board[r][c].distance = -1;
        }
    }
}

std::vector<Cell*> GameManager::computeReachableCells(Cell* origin, int movePoints, int controllingPlayer) {
    resetTraversal();

    std::vector<Cell*> reachable;
    QQueue<Cell*> queue;

    origin->distance = 0;
    origin->visited = true;
    queue.enqueue(origin);

    while (!queue.isEmpty()) {
        Cell* current = queue.dequeue();

        if (current->distance < movePoints) {
            for (Cell* neighbor : current->neighbors) {
                if (!neighbor->visited && !neighbor->occupiedAgent && neighbor->owner != controllingPlayer) {
                    bool allowed = false;

                    if (origin->occupiedAgent->type() == "Grounded" && neighbor->terrain == TerrainType::Free) allowed = true;
                    else if (origin->occupiedAgent->type() == "Water_Walking" &&
                             (neighbor->terrain == TerrainType::Free || neighbor->terrain == TerrainType::Water)) allowed = true;
                    else if (origin->occupiedAgent->type() == "Flying" &&
                             (neighbor->terrain == TerrainType::Free || neighbor->terrain == TerrainType::Water || neighbor->terrain == TerrainType::Rock)) allowed = true;
                    else if (origin->occupiedAgent->type() == "Floating" &&
                             (neighbor->terrain == TerrainType::Free || neighbor->terrain == TerrainType::Water || neighbor->terrain == TerrainType::Rock)) allowed = true;

                    if (allowed) {
                        neighbor->visited = true;
                        neighbor->distance = current->distance + 1;
                        neighbor->parent = current;
                        queue.enqueue(neighbor);
                        reachable.push_back(neighbor);
                    }
                }
            }
        }
    }

    return reachable;
}

int GameManager::countPlayerAgents(int playerId) const {
    int total = 0;
    for (const auto& row : m_board) {
        for (const auto& cell : row) {
            if (cell.occupiedAgent && cell.occupiedAgent->getOwner() == playerId) {
                total++;
            }
        }
    }
    return total;
}
