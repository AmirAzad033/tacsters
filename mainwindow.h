#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "battlefield.h"
#include "gallery.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openGallery();
private:
    void on_startGameButton_clicked();

private:
    Ui::MainWindow *ui;
    Gallery *gallery;
    Battlefield *battlefield;
};
#endif // MAINWINDOW_H
