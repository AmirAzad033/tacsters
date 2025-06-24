#include "battlefield.h"
#include "fantasybutton.h"
#include "fantasylabel.h"
#include "mapreader.h"
#include "agentdet.h"
#include "crystalgridview.h"
#include "gamemanager.h"
#include "gamephases.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QFrame>
#include <QLayoutItem>
#include <QDataStream>
#include <QMessageBox>
#include <QGraphicsBlurEffect>
#include <cstdlib>
#include <ctime>
#include <qpainter.h>
#include <qpushbutton.h>
#include "agent.h"

// ------------------------------------------------------------
// CONSTRUCTOR
// ------------------------------------------------------------
Battlefield::Battlefield(QWidget *parent, const QString &player1, const QString &player2)
    : QWidget(parent), m_player1(player1), m_player2(player2),
    m_gamemanager(nullptr), m_boardWidget(nullptr), m_gamePhase(nullptr),
    m_currentPageLeft(0), m_currentPageRight(0)
{
    setWindowTitle("Tacsters");
    QIcon *q = new QIcon(":/images/images/splashbackground.png");
    Battlefield::setWindowIcon(*q);

    setStyleSheet("Battlefield {"
                  "background-image: url(:/images/images/playback.jpg);"
                  "background-repeat: no-repeat;"
                  "background-position: center;"
                  "background-attachment: fixed;"
                  "background-size: cover;"
                  "}");

    resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.8);
    setMinimumSize(1280, 720);

    setupUI();
}

// ------------------------------------------------------------
// DESTRUCTOR
// ------------------------------------------------------------
Battlefield::~Battlefield()
{
    delete m_boardWidget;
    delete m_gamemanager;
    delete m_gamePhase;

    for (Agent *agentPrototype : m_player1Agents) {
        delete agentPrototype;
    }
    for (Agent *agentPrototype : m_player2Agents) {
        delete agentPrototype;
    }
    m_player1Agents.clear();
    m_player2Agents.clear();
    m_player1AgentDets.clear();
    m_player2AgentDets.clear();
}

// ------------------------------------------------------------
// SETUP UI
// ------------------------------------------------------------
void Battlefield::setupUI()
{
    const QStringList mapFiles = {
        ":/maps/maps/map1.txt", ":/maps/maps/map2.txt", ":/maps/maps/map3.txt",
        ":/maps/maps/map4.txt", ":/maps/maps/map5.txt", ":/maps/maps/map6.txt",
        ":/maps/maps/map7.txt", ":/maps/maps/map8.txt"
    };
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    QString chosenMap = mapFiles[std::rand() % mapFiles.size()];

    Board board = MapReader::parseBoard(chosenMap);
    m_gamemanager = new GameManager(board);
    m_gamePhase = new GamePhases(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // ------------------------------------------------------------
    // AGENT PROTOTYPES
    // ------------------------------------------------------------
    std::vector<Agent *> allAgentPrototypes;
    allAgentPrototypes.push_back(new Water_Walking("Billy", QPixmap(":/agents/agents/Billy.png"), 320, 3, 90, 1, 0));
    allAgentPrototypes.push_back(new Water_Walking("Reketon", QPixmap(":/agents/agents/Reketon.png"), 320, 2, 80, 2, 0));
    allAgentPrototypes.push_back(new Water_Walking("Angus", QPixmap(":/agents/agents/Angus.png"), 400, 2, 100, 1, 0));
    allAgentPrototypes.push_back(new Water_Walking("Duraham", QPixmap(":/agents/agents/Duraham.png"), 320, 2, 100, 2, 0));
    allAgentPrototypes.push_back(new Water_Walking("Colonel_Baba", QPixmap(":/agents/agents/Colonel_Baba.png"), 400, 2, 100, 1, 0));
    allAgentPrototypes.push_back(new Water_Walking("Medusa", QPixmap(":/agents/agents/Medusa.png"), 320, 2, 90, 2, 0));
    allAgentPrototypes.push_back(new Water_Walking("Bunka", QPixmap(":/agents/agents/Bunka.png"), 320, 3, 100, 1, 0));
    allAgentPrototypes.push_back(new Water_Walking("Sanka", QPixmap(":/agents/agents/Sanka.png"), 320, 3, 100, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Sir_Lamorak", QPixmap(":/agents/agents/Sir_Lamorak.png"), 320, 3, 110, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Kabu", QPixmap(":/agents/agents/Kabu.png"), 400, 2, 120, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Rajakal", QPixmap(":/agents/agents/Rajakal.png"), 320, 2, 130, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Salih", QPixmap(":/agents/agents/Salih.png"), 400, 2, 80, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Khan", QPixmap(":/agents/agents/Khan.png"), 320, 2, 90, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Boi", QPixmap(":/agents/agents/Boi.png"), 400, 2, 100, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Eloi", QPixmap(":/agents/agents/Eloi.png"), 240, 2, 100, 2, 0));
    allAgentPrototypes.push_back(new Grounded("Kanar", QPixmap(":/agents/agents/Kanar.png"), 160, 2, 100, 2, 0));
    allAgentPrototypes.push_back(new Grounded("Elsa", QPixmap(":/agents/agents/Elsa.png"), 320, 2, 140, 2, 0));
    allAgentPrototypes.push_back(new Grounded("Karissa", QPixmap(":/agents/agents/Karissa.png"), 280, 2, 80, 2, 0));
    allAgentPrototypes.push_back(new Grounded("Sir_Philip", QPixmap(":/agents/agents/Sir_Philip.png"), 400, 2, 100, 1, 0));
    allAgentPrototypes.push_back(new Grounded("Frost", QPixmap(":/agents/agents/Frost.png"), 260, 2, 80, 2, 0));
    allAgentPrototypes.push_back(new Grounded("Tusk", QPixmap(":/agents/agents/Tusk.png"), 400, 2, 100, 1, 0));
    allAgentPrototypes.push_back(new Flying("Rambu", QPixmap(":/agents/agents/Rambu.png"), 320, 3, 120, 1, 0));
    allAgentPrototypes.push_back(new Floating("Sabrina", QPixmap(":/agents/agents/Sabrina.png"), 320, 3, 100, 1, 0));
    allAgentPrototypes.push_back(new Floating("Death", QPixmap(":/agents/agents/Death.png"), 240, 3, 120, 2, 0));

    m_player1Agents = allAgentPrototypes;
    m_player2Agents = allAgentPrototypes;

    // ------------------------------------------------------------
    // LEFT PANEL
    // ------------------------------------------------------------
    QVBoxLayout *leftPanelLayout = new QVBoxLayout;
    leftPanelLayout->setSpacing(5);
    leftPanelLayout->setContentsMargins(10, 10, 10, 10);

    m_player1Label = new QLabel(m_player1);
    m_player1Label->setAlignment(Qt::AlignCenter);
    m_player1Label->setStyleSheet("color: white; font-weight: bold; font-size: 14px; padding: 5px 8px; border-radius: 6px; background-color: rgba(244,67,54,200);");
    leftPanelLayout->addWidget(m_player1Label);

    m_player1AgentsLayout = new QGridLayout;
    m_player1AgentsLayout->setSpacing(5);
    m_player1AgentsLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *agentsPageWidgetLeft = new QWidget;
    agentsPageWidgetLeft->setLayout(m_player1AgentsLayout);
    leftPanelLayout->addWidget(agentsPageWidgetLeft, 1);

    QHBoxLayout *paginationLeft = new QHBoxLayout;
    FantasyButton *prevBtnLeft = new FantasyButton("◀");
    FantasyButton *nextBtnLeft = new FantasyButton("▶");
    prevBtnLeft->setFixedSize(100, 40);
    nextBtnLeft->setFixedSize(100,40);
    paginationLeft->addWidget(prevBtnLeft);
    paginationLeft->addStretch();
    paginationLeft->addWidget(nextBtnLeft);
    leftPanelLayout->addLayout(paginationLeft);

    QFrame *leftPanel = new QFrame;
    leftPanel->setLayout(leftPanelLayout);
    leftPanel->setFixedWidth(250);
    leftPanel->setStyleSheet("background-color: rgba(255, 255, 255, 80); border-radius: 12px;");

    // ------------------------------------------------------------
    // RIGHT PANEL
    // ------------------------------------------------------------
    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->setSpacing(5);
    rightPanelLayout->setContentsMargins(10, 10, 10, 10);

    m_player2Label = new QLabel(m_player2);
    m_player2Label->setAlignment(Qt::AlignCenter);
    m_player2Label->setStyleSheet("color: white; font-weight: bold; font-size: 14px; padding: 5px 8px; border-radius: 6px; background-color: rgba(33,150,243,200);");
    rightPanelLayout->addWidget(m_player2Label);

    m_player2AgentsLayout = new QGridLayout;
    m_player2AgentsLayout->setSpacing(5);
    m_player2AgentsLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *agentsPageWidgetRight = new QWidget;
    agentsPageWidgetRight->setLayout(m_player2AgentsLayout);
    rightPanelLayout->addWidget(agentsPageWidgetRight, 1);

    QHBoxLayout *paginationRight = new QHBoxLayout;
    FantasyButton *prevBtnRight = new FantasyButton("◀");
    FantasyButton *nextBtnRight = new FantasyButton("▶");
    prevBtnRight->setFixedSize(100, 40);
    nextBtnRight->setFixedSize(100,40);
    paginationRight->addWidget(prevBtnRight);
    paginationRight->addStretch();
    paginationRight->addWidget(nextBtnRight);
    rightPanelLayout->addLayout(paginationRight);

    QFrame *rightPanel = new QFrame;
    rightPanel->setLayout(rightPanelLayout);
    rightPanel->setFixedWidth(250);
    rightPanel->setStyleSheet("background-color: rgba(255, 255, 255, 80); border-radius: 12px;");

    // ------------------------------------------------------------
    // CENTER BOARD
    // ------------------------------------------------------------
    m_boardWidget = new CrystalGridView(m_gamemanager, this);
    m_boardWidget->setAcceptDrops(true);
    connect(m_boardWidget, &CrystalGridView::unitDeployed, this, &Battlefield::agentPlaced);
    connect(m_boardWidget, &CrystalGridView::phaseEnded, this, &Battlefield::turnFinished);
    m_boardWidget->assignPhase(m_gamePhase);

    mainLayout->addWidget(leftPanel, 3);
    mainLayout->addWidget(m_boardWidget, 4);
    mainLayout->addWidget(rightPanel, 3);

    setLayout(mainLayout);

    // ------------------------------------------------------------
    // PAGINATION
    // ------------------------------------------------------------
    const int cardsPerPage = 6;

    auto renderLeftPage = [=]() mutable {
        QLayoutItem *child;
        while ((child = m_player1AgentsLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        int start = m_currentPageLeft * cardsPerPage;
        int end = std::min(start + cardsPerPage, (int)m_player1Agents.size());
        int row = 0, col = 0;

        for (int i = start; i < end; i++) {
            AgentDet *card = new AgentDet(m_player1Agents[i], 1);
            card->setFixedSize(100, 180);
            m_player1AgentsLayout->addWidget(card, row, col);
            m_player1AgentDets.append(card);
            col++;
            if (col >= 2) { col = 0; row++; }
        }
    };

    auto renderRightPage = [=]() mutable {
        QLayoutItem *child;
        while ((child = m_player2AgentsLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        int start = m_currentPageRight * cardsPerPage;
        int end = std::min(start + cardsPerPage, (int)m_player2Agents.size());
        int row = 0, col = 0;

        for (int i = start; i < end; i++) {
            AgentDet *card = new AgentDet(m_player2Agents[i], 2);
            card->setFixedSize(100, 180);
            m_player2AgentsLayout->addWidget(card, row, col);
            m_player2AgentDets.append(card);
            col++;
            if (col >= 2) { col = 0; row++; }
        }
    };

    connect(prevBtnLeft, &QPushButton::clicked, [=]() mutable {
        if (m_currentPageLeft > 0) { m_currentPageLeft--; renderLeftPage(); }
    });
    connect(nextBtnLeft, &QPushButton::clicked, [=]() mutable {
        if ((m_currentPageLeft + 1) * cardsPerPage < m_player1Agents.size()) {
            m_currentPageLeft++; renderLeftPage();
        }
    });
    connect(prevBtnRight, &QPushButton::clicked, [=]() mutable {
        if (m_currentPageRight > 0) { m_currentPageRight--; renderRightPage(); }
    });
    connect(nextBtnRight, &QPushButton::clicked, [=]() mutable {
        if ((m_currentPageRight + 1) * cardsPerPage < m_player2Agents.size()) {
            m_currentPageRight++; renderRightPage();
        }
    });

    renderLeftPage();
    renderRightPage();
    updatePlayerLabels();
}

// ------------------------------------------------------------
// AGENT PLACEMENT HANDLING
// ------------------------------------------------------------
void Battlefield::agentPlaced(const QString &agentTypeName, int playerOwner)
{
    if (m_gamePhase->getCurrentPhase() != GamePhase::Deploy) return;
    if (m_gamePhase->getCurrentPlayer() != playerOwner) return;

    QList<AgentDet *> *targetList = nullptr;
    QGridLayout *targetLayout = nullptr;

    if (playerOwner == 1) {
        targetList = &m_player1AgentDets;
        targetLayout = m_player1AgentsLayout;
    } else if (playerOwner == 2) {
        targetList = &m_player2AgentDets;
        targetLayout = m_player2AgentsLayout;
    }

    if (targetList && targetLayout) {
        AgentDet *cardToRemove = nullptr;
        for (AgentDet *card : *targetList) {
            if (card->getAgentPrototype()->name() == agentTypeName) {
                cardToRemove = card; break;
            }
        }

        if (cardToRemove) {
            int index = targetLayout->indexOf(cardToRemove);
            if (index != -1) {
                targetLayout->removeWidget(cardToRemove);
            }
            cardToRemove->deleteLater();
            targetList->removeOne(cardToRemove);

            m_gamePhase->agentPlaced(playerOwner);
            updatePlayerLabels();

            if (m_gamePhase->getCurrentPhase() == GamePhase::Fight) {
                m_gamemanager->resetStartZones();
                m_gamePhase->forceCurrentPlayer(1);
                updateBoardDisplay();
            }
        }
    }
}

// ------------------------------------------------------------
// UPDATE LABELS
// ------------------------------------------------------------
void Battlefield::updatePlayerLabels()
{
    m_player1Label->setStyleSheet(
        m_gamePhase->getCurrentPlayer() == 1 ?
            "border: 3px solid white; border-radius: 8px; background-color: rgba(244,67,54,200); color: white; font-weight: bold; font-size: 18px; padding: 10px;" :
            "border: 3px solid transparent; border-radius: 8px; background-color: rgba(244,67,54,200); color: white; font-weight: bold; font-size: 18px; padding: 10px;"
        );

    m_player2Label->setStyleSheet(
        m_gamePhase->getCurrentPlayer() == 2 ?
            "border: 3px solid white; border-radius: 8px; background-color: rgba(33,150,243,200); color: white; font-weight: bold; font-size: 18px; padding: 10px;" :
            "border: 3px solid transparent; border-radius: 8px; background-color: rgba(33,150,243,200); color: white; font-weight: bold; font-size: 18px; padding: 10px;"
        );
}

// ------------------------------------------------------------
// UPDATE BOARD DISPLAY
// ------------------------------------------------------------
void Battlefield::updateBoardDisplay()
{
    if (m_boardWidget) {
        m_boardWidget->refreshGrid();
    }
}

// ------------------------------------------------------------
// TURN FINISHED
// ------------------------------------------------------------
void Battlefield::turnFinished()
{
    m_gamePhase->nextTurn();
    updatePlayerLabels();
    updateBoardDisplay();

    int player1Agents = m_gamemanager->countPlayerAgents(1);
    int player2Agents = m_gamemanager->countPlayerAgents(2);

    QString message;
    QColor bgColor;
    QColor textColor;
    QColor buttonColor;
    QColor buttonHover;

    if (player1Agents == 0 && player2Agents == 0) {
        message = "DRAW!";
        bgColor = QColor("#2E2E2E");
        textColor = QColor("white");
        buttonColor = QColor("#00BCD4");
        buttonHover = QColor("#0097A7");
    } else if (player1Agents == 0) {
        message = m_player2 + " WINS!";
        bgColor = QColor("#1E3A8A");
        textColor = QColor("#BBDEFB");
        buttonColor = QColor("#2196F3");
        buttonHover = QColor("#1976D2");
    } else if (player2Agents == 0) {
        message = m_player1 + " WINS!";
        bgColor = QColor("#7B1FA2");
        textColor = QColor("#E1BEE7");
        buttonColor = QColor("#9C27B0");
        buttonHover = QColor("#7B1FA2");
    } else {
        return;
    }

    QDialog* dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dialog->setAttribute(Qt::WA_TranslucentBackground);
    dialog->setModal(true);

    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size();
    dialog->resize(screenSize.width() / 2, screenSize.height() / 2);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(30);

    FantasyLabel* label = new FantasyLabel(message, dialog);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(QString(
                             "FantasyLabel { "
                             "color: %1; "
                             "font-size: 64px; "
                             "font-weight: bold; "
                             "text-shadow: 2px 2px 6px black; "
                             "}")
                             .arg(textColor.name()));
    layout->addWidget(label, 1, Qt::AlignCenter);

    FantasyButton* okBtn = new FantasyButton("Well Done!", dialog);
    okBtn->setCursor(Qt::PointingHandCursor);
    okBtn->setStyleSheet(QString(
                             "FantasyButton { background-color: %1; color: white; padding: 20px 40px; border-radius: 12px; font-size: 32px; font-weight: bold; } "
                             "FantasyButton:hover { background-color: %2; }"
                             ).arg(buttonColor.name(), buttonHover.name()));
    layout->addWidget(okBtn, 0, Qt::AlignCenter);

    connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(okBtn, &QPushButton::clicked, this, &QWidget::close);

    dialog->setStyleSheet(QString("QDialog { background-color: %1; border-radius: 20px; }").arg(bgColor.name()));
    dialog->exec();
}

void Battlefield::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap bg(":/images/images/playback.jpg");
    if (!bg.isNull()) {

        painter.drawPixmap(rect(), bg);
    }
}

