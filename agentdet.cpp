#include "AgentDet.h"
#include "fantasylabel.h"
#include <QDebug>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "fantasylabel.h"

AgentDet::AgentDet(Agent* agentPrototype, int playerName, QWidget *parent)
    : QLabel(parent), m_agentPrototype(agentPrototype), m_playerName(playerName) {
    if (m_agentPrototype) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        mainLayout->setSpacing(5);

        QLabel *imgLabel = new QLabel;
        imgLabel->setPixmap(m_agentPrototype->icon().scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imgLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(imgLabel);

        FantasyLabel *nameLabel = new FantasyLabel(m_agentPrototype->name());
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-top: 5px; color: black;");
        mainLayout->addWidget(nameLabel);

        QHBoxLayout *statsLayout1 = new QHBoxLayout;
        statsLayout1->setContentsMargins(0,0,0,0);
        statsLayout1->setSpacing(8);
        FantasyLabel *hpLabel = new FantasyLabel(QString("H: %1").arg(m_agentPrototype->getHp()));
        hpLabel->setStyleSheet("font-size: 10px; color: black;");
        FantasyLabel *mobilityLabel = new FantasyLabel(QString("M: %1").arg(m_agentPrototype->getMobility()));
        mobilityLabel->setStyleSheet("font-size: 10px; color: black;");
        statsLayout1->addWidget(hpLabel);
        statsLayout1->addStretch();
        statsLayout1->addWidget(mobilityLabel);
        mainLayout->addLayout(statsLayout1);

        QHBoxLayout *statsLayout2 = new QHBoxLayout;
        statsLayout2->setContentsMargins(0,0,0,0);
        statsLayout2->setSpacing(8);
        FantasyLabel *damageLabel = new FantasyLabel(QString("D: %1").arg(m_agentPrototype->getDamage()));
        damageLabel->setStyleSheet("font-size: 10px; color: black;");

        QString attackRangeText;
        if (m_agentPrototype->name() == "Eloi") {
            attackRangeText = "R: 2-3";
        } else {
            attackRangeText = QString("R: %1").arg(m_agentPrototype->getAttackRange());
        }
        FantasyLabel *attackRangeLabel = new FantasyLabel(attackRangeText);
        attackRangeLabel->setStyleSheet("font-size: 10px; color: black;");

        statsLayout2->addWidget(damageLabel);
        statsLayout2->addStretch();
        statsLayout2->addWidget(attackRangeLabel);
        mainLayout->addLayout(statsLayout2);

        setFixedSize(160, 260);
        setCursor(Qt::OpenHandCursor);
    }
}

Agent* AgentDet::getAgentPrototype() const {
    return m_agentPrototype;
}

int AgentDet::getPlayerName() const {
    return m_playerName;
}

void AgentDet::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_agentPrototype) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        QByteArray agentData;
        QDataStream stream(&agentData, QIODevice::WriteOnly);
        stream << m_agentPrototype->name() << m_playerName;

        mimeData->setData("application/x-yourgame-agent-type", agentData);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap());
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}
