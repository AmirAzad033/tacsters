#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "crystalgridview.h"
#include "fantasybutton.h"
#include <QWidget>
#include <QString>
#include <QList>
#include <qframe.h>
#include <qgridlayout.h>
#include <vector>

class GameManager;
class HexBoardWidget;
class GamePhases;
class Agent;
class AgentDet;
class QLabel;
class QVBoxLayout;
struct AgentPanel {
    QGridLayout* grid = nullptr;
    FantasyButton* prevButton = nullptr;
    FantasyButton* nextButton = nullptr;
    std::vector<Agent*> agents;
    int currentPage = 0;
    int agentsPerPage = 6;
};
//class
class Battlefield : public QWidget
{
    Q_OBJECT

public:
    Battlefield(QWidget *parent = nullptr, const QString &player1 = "Player 1", const QString &player2 = "Player 2");
    ~Battlefield();
    void paintEvent(QPaintEvent q);
    void updateAgentPanel(AgentPanel &panel, const std::vector<Agent*> &agents, int playerNumber);
    QFrame* setupAgentPanel(AgentPanel &panel, const QString &playerName,
                            const std::vector<Agent*> &agents, int playerNumber,
                            const QString &bgColor);
    void paintEvent(QPaintEvent *);
private slots:
    void agentPlaced(const QString& agentTypeName, int playerOwner);
    void turnFinished();

private:
    void setupUI();
    void updatePlayerLabels();
    void updateBoardDisplay();

    QString m_player1;
    QString m_player2;
    GameManager* m_gamemanager;
    CrystalGridView* m_boardWidget;
    GamePhases* m_gamePhase;

    QLabel* m_player1Label;
    QLabel* m_player2Label;
    QGridLayout* m_player1AgentsLayout;
    QGridLayout* m_player2AgentsLayout;

    std::vector<Agent*> m_player1Agents;
    std::vector<Agent*> m_player2Agents;
    QList<AgentDet*> m_player1AgentDets;
    QList<AgentDet*> m_player2AgentDets;
    int m_currentPageLeft;
    int m_currentPageRight;

    AgentPanel player1Panel;
    AgentPanel player2Panel;
};

#endif
