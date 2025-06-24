#include "splash.h"
#include "ui_splash.h"
#include "fantasybutton.h"
#include "fantasylabel.h"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>

splash::splash(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::splash) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    QIcon *q = new QIcon(":/images/images/splashbackground.png");
    splash::setWindowIcon(*q);

    QLabel *logo = new QLabel();
    QPixmap logoPix(":/images/images/logo.png");
    logo->setPixmap(logoPix.scaled(400, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setStyleSheet("background: transparent;");
    logo->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(logo, 0, Qt::AlignHCenter | Qt::AlignTop);

    FantasyButton *button = new FantasyButton("Begin your journey!", this);

    QPoint position = ui->labell2->pos();
    QString text = ui->labell2->text();
    Qt::Alignment alignment = ui->labell2->alignment();
    ui->labell2->hide();
    ui->labell2->setParent(nullptr);
    delete ui->labell2;
    fantasylabel = new FantasyLabel(text, this);
    fantasylabel->setObjectName("labell2");
    fantasylabel->move(position);
    fantasylabel->setAlignment(alignment);
    fantasylabel->setAttribute(Qt::WA_TranslucentBackground);
    fantasylabel->setStyleSheet("background: transparent;");

    ui->labell2 = fantasylabel;
    layout->addStretch();
    layout->addWidget(ui->labell2, 0, Qt::AlignCenter);
    layout->addSpacing(85);
    layout->addWidget(button, 0, Qt::AlignHCenter);
    layout->addStretch();
    connect(button, &QPushButton::clicked, this, &splash::openMainWindow);
    mainWindow = new MainWindow();
    setWindowTitle("Tacsters");
    this->setStyleSheet("background-image: url(:/images/images/back1.jpg); background-position: center;");
    resize(800, 600);
}

splash::~splash()
{
    delete ui;
}

void splash::openMainWindow()
{
    mainWindow->showNormal();
    this->hide();
}
