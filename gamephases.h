#ifndef GAMEPHASES_H
#define GAMEPHASES_H

#include <QObject>
#include <QVector>
#include <QString>

class GameManager;
class Agent;

enum class GamePhase {
    Fight,
    Deploy
};

class GamePhases : public QObject
{
    Q_OBJECT

public:
    explicit GamePhases(QObject *parent = nullptr);

    GamePhase getCurrentPhase() const;
    int getCurrentPlayer() const;
    int getPlayerPlacedAgents(int player) const;
    void forceCurrentPlayer(int player) { m_currentPlayer = player; }

public slots:
    void nextTurn();
    void agentPlaced(int player);
    void startGame();

private:
    GamePhase m_currentPhase = GamePhase::Deploy;
    int m_currentPlayer = 1;
    QVector<int> m_placedAgents;
};

#endif
