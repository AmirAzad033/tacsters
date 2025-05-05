#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QWidget>
#include <QVector>
#include "mapreader.h"

class HexCell : public QWidget {
    Q_OBJECT
public:
    explicit HexCell(QWidget *parent = nullptr);
    void setValue(int value);
    void setColor(const QColor& color) {
        m_baseColor = color;
        update();
    }
    void setBlocked(bool blocked) { m_blocked = blocked; }
    bool isBlocked() const { return m_blocked; }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int m_value;
    bool m_hovered;
    QColor m_baseColor = QColor(140, 140, 255);
    bool m_blocked = false;
};

class HexBoard : public QWidget {
    Q_OBJECT
public:
    explicit HexBoard(QWidget *parent = nullptr);
    void initializeBoard(int columns = 9, int bigRows = 5, int smallRows = 4);
    void initializeBoardFromMap(const QVector<CellPosition>& cellPositions);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPointF hexToPixel(int x, int y) const;
    QVector<QVector<HexCell*>> m_cells;
    double m_hexSize;
    QVector<CellPosition> m_cellPositions;
};

#endif // HEXBOARD_H
