#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QWidget>
#include "fantasylabel.h"
#include "hexboard.h"

namespace Ui {
    class battlefield;
}
class battlefield : public QWidget
{
    Q_OBJECT

public:
    explicit battlefield(QWidget *parent = nullptr);
    void setPlayerNames(const QString& name1, const QString& name2);
    HexBoard* getGameBoard() { return gameBoard; }

private:
    Ui::battlefield *ui;
    FantasyLabel *player1Label;
    FantasyLabel *player2Label;
    HexBoard *gameBoard;
};

#endif // THIRDWINDOW_H
