#ifndef AGENTDET_H
#define AGENTDET_H

#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QDataStream>
#include "Agent.h"

class AgentDet : public QLabel {
    Q_OBJECT

public:
    explicit AgentDet(Agent* agentPrototype, int playerName, QWidget *parent = nullptr);

    Agent* getAgentPrototype() const;
    int getPlayerName() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Agent* m_agentPrototype;
    int m_playerName;
};

#endif
