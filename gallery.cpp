#include "gallery.h"
#include "Agent.h"
#include "ui_gallery.h"
#include "fantasybutton.h"
#include "fantasylabel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Gallery::Gallery(const std::vector<Agent> &agents, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Gallery),
    m_agents(agents),
    m_currentIndex(0)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/images/splashbackground.png"));
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(
        "Gallery { "
        "   background-image: url(:/images/images/splashback.jpg); "
        "   background-position: center; "
        "   background-repeat: no-repeat; "
        "}"
        );
    setWindowTitle("Tacsters");

    FantasyButton *backButton = new FantasyButton("← Back", this);
    backButton->setFixedSize(200, 60);
    backButton->move(20, 20);
    connect(backButton, &QPushButton::clicked, this, [this]() { close(); });

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 100, 50, 50);
    mainLayout->setSpacing(20);


    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->setSpacing(20);

    FantasyButton *prevButton = new FantasyButton("←");
    FantasyButton *nextButton = new FantasyButton("→");
    prevButton->setFixedSize(80, 80);
    nextButton->setFixedSize(80, 80);
    prevButton->setStyleSheet("font-size: 32px; color: white;");
    nextButton->setStyleSheet("font-size: 32px; color: white;");

    connect(prevButton, &QPushButton::clicked, this, &Gallery::showPreviousAgent);
    connect(nextButton, &QPushButton::clicked, this, &Gallery::showNextAgent);

    m_imageLabel = new QLabel();
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setFixedSize(400, 400);
    m_imageLabel->setStyleSheet(
        "background-image: url(:/images/images/splashback.jpg);"
        "background-position: center;"
        "background-repeat: no-repeat;"
        );
    centerLayout->addWidget(prevButton);
    centerLayout->addWidget(m_imageLabel, 1);
    centerLayout->addWidget(nextButton);

    mainLayout->addLayout(centerLayout);

    m_nameLabel = new FantasyLabel("");
    m_nameLabel->setFontSize(28);

    m_typeLabel = new FantasyLabel("");
    m_typeLabel->setFontSize(20);

    mainLayout->addWidget(m_nameLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_typeLabel, 0, Qt::AlignCenter);

    QGridLayout *statsLayout = new QGridLayout();
    statsLayout->setHorizontalSpacing(40);
    statsLayout->setVerticalSpacing(10);

    m_hpLabel = new FantasyLabel("");
    m_moveLabel = new FantasyLabel("");
    m_dmgLabel = new FantasyLabel("");
    m_rngLabel = new FantasyLabel("");

    m_hpLabel->setFontSize(18);
    m_moveLabel->setFontSize(18);
    m_dmgLabel->setFontSize(18);
    m_rngLabel->setFontSize(18);

    statsLayout->addWidget(m_hpLabel, 0, 0);
    statsLayout->addWidget(m_moveLabel, 0, 1);
    statsLayout->addWidget(m_dmgLabel, 1, 0);
    statsLayout->addWidget(m_rngLabel, 1, 1);

    QWidget *statsWidget = new QWidget();
    statsWidget->setLayout(statsLayout);
    mainLayout->addWidget(statsWidget, 0, Qt::AlignCenter);

    updateAgentView();
}

void Gallery::showPreviousAgent()
{
    if (m_agents.empty()) return;
    m_currentIndex = (m_currentIndex - 1 + m_agents.size()) % m_agents.size();
    updateAgentView();
}

void Gallery::showNextAgent()
{
    if (m_agents.empty()) return;
    m_currentIndex = (m_currentIndex + 1) % m_agents.size();
    updateAgentView();
}

void Gallery::updateAgentView()
{
    if (m_agents.empty()) return;

    const Agent &agent = m_agents[m_currentIndex];

    m_imageLabel->setPixmap(agent.icon().scaled(
        m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation
        ));

    m_nameLabel->setText(agent.name().replace('_', ' '));
    m_typeLabel->setText(agent.type().replace('_', ' '));

    m_hpLabel->setText(QString("HP: %1").arg(agent.getHp()));
    m_moveLabel->setText(QString("Move: %1").arg(agent.getMobility()));
    m_dmgLabel->setText(QString("Damage: %1").arg(agent.getDamage()));
    m_rngLabel->setText(QString("Range: %1").arg(agent.getAttackRange()));
}

Gallery::~Gallery()
{
    delete ui;
}
