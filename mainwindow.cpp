#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include "battlefield.h"
#include "fantasybutton.h"
#include "namesdialog.h"

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

    battlefield = new Battlefield();
    setWindowTitle("Tacsters");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gallery;
    delete battlefield;
}

void MainWindow::openGallery()
{
    std::vector<Agent> agents = {
        Agent("Billy", "Water_Walking", QPixmap(":/agents/agents/Billy.png", 0), 320, 3, 90, 1, 0),
        Agent("Reketon", "Water_Walking", QPixmap(":/agents/agents/Reketon.png", 0), 320, 2, 80, 2, 0),
        Agent("Angus", "Water_Walking", QPixmap(":/agents/agents/Angus.png", 0), 400, 2, 100, 1, 0),
        Agent("Duraham", "Water_Walking", QPixmap(":/agents/agents/Duraham.png", 0), 320, 2, 100, 2, 0),
        Agent("Colonel_Baba", "Water_Walking", QPixmap(":/agents/agents/Colonel_Baba.png", 0), 400, 2, 100, 1, 0),
        Agent("Medusa", "Water_Walking", QPixmap(":/agents/agents/Medusa.png", 0), 320, 2, 90, 2, 0),
        Agent("Bunka", "Water_Walking", QPixmap(":/agents/agents/Bunka.png", 0), 320, 3, 100, 1, 0),
        Agent("Sanka", "Water_Walking", QPixmap(":/agents/agents/Sanka.png", 0), 320, 3, 100, 1, 0),
        Agent("Sir_Lamorak", "Grounded", QPixmap(":/agents/agents/Sir_Lamorak.png", 0), 320, 3, 110, 1, 0),
        Agent("Kabu", "Grounded", QPixmap(":/agents/agents/Kabu.png", 0), 400, 2, 120, 1, 0),
        Agent("Rajakal", "Grounded", QPixmap(":/agents/agents/Rajakal.png", 0), 320, 2, 130, 1, 0),
        Agent("Salih", "Grounded", QPixmap(":/agents/agents/Salih.png", 0), 400, 2, 80, 1, 0),
        Agent("Khan", "Grounded", QPixmap(":/agents/agents/Khan.png", 0), 320, 2, 90, 1, 0),
        Agent("Boi", "Grounded", QPixmap(":/agents/agents/Boi.png", 0), 400, 2, 100, 1, 0),
        Agent("Eloi", "Grounded", QPixmap(":/agents/agents/Eloi.png", 0), 240, 2, 100, 2, 0),
        Agent("Kanar", "Grounded", QPixmap(":/agents/agents/Kanar.png", 0), 160, 2, 100, 2, 0),
        Agent("Elsa", "Grounded", QPixmap(":/agents/agents/Elsa.png", 0), 320, 2, 140, 2, 0),
        Agent("Karissa", "Grounded", QPixmap(":/agents/agents/Karissa.png", 0), 280, 2, 80, 2, 0),
        Agent("Sir_Philip", "Grounded", QPixmap(":/agents/agents/Sir_Philip.png", 0), 400, 2, 100, 1, 0),
        Agent("Frost", "Grounded", QPixmap(":/agents/agents/Frost.png", 0), 260, 2, 80, 2, 0),
        Agent("Tusk", "Grounded", QPixmap(":/agents/agents/Tusk.png", 0), 400, 2, 100, 1, 0),
        Agent("Rambu", "Flying", QPixmap(":/agents/agents/Rambu.png", 0), 320, 3, 120, 1, 0),
        Agent("Sabrina", "Floating", QPixmap(":/agents/agents/Sabrina.png", 0), 320, 3, 100, 1, 0),
        Agent("Death", "Floating", QPixmap(":/agents/agents/Death.png", 0), 240, 3, 120, 2, 0)
    };


    Gallery *gallery = new Gallery(agents);
    gallery->show();
}

void MainWindow::on_startGameButton_clicked()
{
    NamesDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Battlefield *battle = new Battlefield(nullptr,
                                              dialog.getUsername1(),
                                              dialog.getUsername2());
        battle->showMaximized();
        this->close();
    }
}

