#include "namesdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <qlineedit.h>
#include "fantasybutton.h"

NamesDialog::NamesDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Enter Player Names");
    setFixedSize(600, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(20);

    username1LineEdit = new QLineEdit(this);
    username1LineEdit->setPlaceholderText("Player 1 name...");
    username1LineEdit->setStyleSheet(
        "QLineEdit {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0096FF, stop:1 #FF69B4);"  // Changed to pink (#FF69B4)
        "   color: white;"
        "   border-radius: 40px;"
        "   padding: 15px 25px;"
        "   border: none;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   selection-background-color: #FF69B4;"
        "   selection-color: white;"
        "   min-height: 50px;"
        "   min-width: 220px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid rgba(255, 255, 255, 0.7);"
        "}"
        );

    username2LineEdit = new QLineEdit(this);
    username2LineEdit->setPlaceholderText("Player 2 name...");
    username2LineEdit->setStyleSheet(
        "QLineEdit {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0096FF, stop:1 #FF69B4);"  // Same blue-to-pink
        "   color: white;"
        "   border-radius: 40px;"
        "   padding: 15px 25px;"
        "   border: none;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   selection-background-color: #FF69B4;"
        "   selection-color: white;"
        "   min-height: 50px;"
        "   min-width: 220px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid rgba(255, 255, 255, 0.7);"
        "}"
        );

    inputLayout->addStretch();
    inputLayout->addWidget(username1LineEdit);
    inputLayout->addWidget(username2LineEdit);
    inputLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addLayout(inputLayout);
    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    FantasyButton *submitButton = new FantasyButton("Start Game", this);
    submitButton->setFixedSize(300, 60);

    buttonLayout->addStretch();
    buttonLayout->addWidget(submitButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    connect(submitButton, &QPushButton::clicked, this, [this]() {
        if (username1LineEdit->text().isEmpty() || username2LineEdit->text().isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Both usernames must be filled!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet(
                "QMessageBox { background-image: url(:/images/images/splashbg.png); }"
                "QMessageBox QLabel { color: white; }"
                "QMessageBox QPushButton { background: rgba(100, 100, 100, 150); color: white; }"
                );
            msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
            msgBox.exec();
        } else {
            emit namesSubmitted(username1LineEdit->text(), username2LineEdit->text());
            accept();
        }
    });

    setStyleSheet(
        "QDialog { "
        "   background-image: url(:/images/images/splashback.jpg); "
        "   background-position: center; "
        "}"
        );
}

QString NamesDialog::getUsername1() const {
    return username1LineEdit->text();
}

QString NamesDialog::getUsername2() const {
    return username2LineEdit->text();
}
