#include "battlefield.h"
#include "ui_battlefield.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QIcon>
battlefield::battlefield(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::battlefield) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    setObjectName("battlefield");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    QIcon *q = new QIcon(":/images/images/splashbackground.png");
    battlefield::setWindowIcon(*q);

    QHBoxLayout *playerLayout = new QHBoxLayout();
    playerLayout->setContentsMargins(20, 0, 20, 0);
    playerLayout->setSpacing(0);

    player1Label = new FantasyLabel("", this);
    player1Label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    player1Label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    player2Label = new FantasyLabel("", this);
    player2Label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    player2Label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    playerLayout->addWidget(player1Label, 0, Qt::AlignLeft);
    playerLayout->addStretch();
    playerLayout->addWidget(player2Label, 0, Qt::AlignRight);

    QHBoxLayout *boardLayout = new QHBoxLayout();
    gameBoard = new HexBoard(this);
    gameBoard->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    gameBoard->setMinimumSize(1450, 750);
    boardLayout->addStretch();
    boardLayout->addWidget(gameBoard, 0, Qt::AlignCenter);
    boardLayout->addStretch();

    mainLayout->addLayout(playerLayout);
    mainLayout->addLayout(boardLayout, 1);

    setStyleSheet(
        "QWidget#battlefield { "
        "   background-image: url(:/images/images/playback.jpg); "
        "   background-position: center; "
        "}"
        );
    setWindowTitle("Tacsters");
}

void battlefield::setPlayerNames(const QString &name1, const QString &name2) {
    player1Label->setText(name1);
    player2Label->setText(name2);
}
