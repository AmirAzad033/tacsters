#include "gamephases.h"
#include <QDebug>

GamePhases::GamePhases(QObject *parent)
    : QObject(parent)
{
    m_placedAgents.resize(2, 0);
}

GamePhase GamePhases::getCurrentPhase() const
{
    return m_currentPhase;
}

int GamePhases::getCurrentPlayer() const
{
    return m_currentPlayer;
}

int GamePhases::getPlayerPlacedAgents(int player) const
{
    if (player == 1) return m_placedAgents[0];
    if (player == 2) return m_placedAgents[1];
    return 0;
}

void GamePhases::nextTurn()
{
    m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
}

void GamePhases::agentPlaced(int player)
{
    if (player == 1) {
        m_placedAgents[0]++;
    } else if (player == 2) {
        m_placedAgents[1]++;
    }

    if (m_placedAgents[0] >= 6 && m_placedAgents[1] >= 6) {
        startGame();
    } else {
        nextTurn();
    }
}

void GamePhases::startGame()
{
    m_currentPhase = GamePhase::Fight;
}
