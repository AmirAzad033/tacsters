#ifndef SPLASH_H
#define SPLASH_H

#include <QWidget>
#include <QLabel>
#include "mainwindow.h"
#include "fantasylabel.h"

namespace Ui {
class splash;
}

class splash : public QWidget
{
    Q_OBJECT

public:
    explicit splash(QWidget *parent = nullptr);
    ~splash();

private:
    Ui::splash *ui;
private slots:
    void openMainWindow();
private:
    MainWindow *mainWindow;
    FantasyLabel *fantasylabel;

};

#endif // SPLASH_H
