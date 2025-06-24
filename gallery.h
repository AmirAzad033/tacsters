#ifndef GALLERY_H
#define GALLERY_H

#include <QWidget>
#include <vector>
#include "Agent.h"

namespace Ui {
class Gallery;
}

class QLabel;
class FantasyLabel;

class Gallery : public QWidget
{
    Q_OBJECT

public:
    explicit Gallery(const std::vector<Agent> &agents, QWidget *parent = nullptr);
    ~Gallery();

private slots:
    void showPreviousAgent();
    void showNextAgent();

private:
    void updateAgentView();

    Ui::Gallery *ui;
    std::vector<Agent> m_agents;
    int m_currentIndex;

    QLabel *m_imageLabel;
    FantasyLabel *m_nameLabel;
    FantasyLabel *m_typeLabel;
    FantasyLabel *m_hpLabel;
    FantasyLabel *m_moveLabel;
    FantasyLabel *m_dmgLabel;
    FantasyLabel *m_rngLabel;
};

#endif // GALLERY_H
