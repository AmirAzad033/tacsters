#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include "fantasybutton.h"
#include "namesdialog.h"
#include "battlefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    statusBar()->hide();
    QIcon *q = new QIcon(":/images/images/splashbackground.png");
    MainWindow::setWindowIcon(*q);

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-image: url(:/images/images/splashback.jpg); background-position: center;");
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    FantasyButton *button1 = new FantasyButton("Start an Async PVP Game", this);
    FantasyButton *button2 = new FantasyButton("Gallery", this);

    layout->addStretch();
    layout->addWidget(button1, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(button2, 0, Qt::AlignHCenter);
    layout->addStretch();
    button1->setFixedWidth(500);

    connect(button2, &QPushButton::clicked, this, &MainWindow::openGallery);
    connect(button1, &QPushButton::clicked, this, &MainWindow::on_startGameButton_clicked);

    gallery = new Gallery();
    battleField = new battlefield();
    setWindowTitle("Tacsters");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gallery;
    delete battleField;
}

void MainWindow::openGallery(){
    gallery->showNormal();
}

void MainWindow::on_startGameButton_clicked() {
    NamesDialog dialog(this);
    connect(&dialog, &NamesDialog::namesSubmitted, battleField, &battlefield::setPlayerNames);

    if (dialog.exec() == QDialog::Accepted) {
        battleField->showMaximized();
        this->hide();
    }
}
