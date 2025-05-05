#include "gallery.h"
#include "ui_gallery.h"
#include "fantasybutton.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Gallery::Gallery(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Gallery)
{
    ui->setupUi(this);
    QIcon *q = new QIcon(":/images/images/splashbackground.png");
    Gallery::setWindowIcon(*q);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(
        "Gallery { "
        "   background-image: url(:/images/images/splashback.jpg); "
        "   background-position: center; "
        "   background-repeat: no-repeat; "
        "}"
        );
    setWindowTitle("Tacsters");
    resize(800, 600);
    FantasyButton *backButton = new FantasyButton("← Back", this);
    backButton->setFixedSize(200, 60);
    backButton->move(20, 20);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        this->close();
    });
}


Gallery::~Gallery()
{
    delete ui;
}
