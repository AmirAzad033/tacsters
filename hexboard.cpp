#include "hexboard.h"
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <cmath>

HexCell::HexCell(QWidget *parent) : QWidget(parent), m_value(0), m_hovered(false) {
    setAttribute(Qt::WA_Hover);
}

void HexCell::setValue(int value) {
    m_value = value;
    update();
}

void HexCell::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = this->rect().adjusted(2, 2, -2, -2);
    QPainterPath path;

    for (int i = 0; i < 6; i++) {
        double angle = 2 * M_PI / 6 * (i + 0.5);
        QPointF point(rect.center() + QPointF(rect.width()/2 * cos(angle),
                                              rect.height()/2 * sin(angle)));
        if (i == 0) path.moveTo(point);
        else path.lineTo(point);
    }
    path.closeSubpath();

    QColor baseColor=m_baseColor;
    QColor outlineColor = baseColor.darker(125);
    int outlineWidth = qMax(2, static_cast<int>(rect.height() * 0.08));

    if (m_hovered) {
        painter.setPen(QPen(outlineColor, outlineWidth + 1));
        painter.setBrush(baseColor.lighter(120));
    } else {
        painter.setPen(QPen(outlineColor, outlineWidth));
        painter.setBrush(baseColor);
    }

    painter.drawPath(path);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(m_value));
}

void HexCell::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

void HexCell::enterEvent(QEnterEvent *) {
    m_hovered = true;
    update();
}

void HexCell::leaveEvent(QEvent *) {
    m_hovered = false;
    update();
}

HexBoard::HexBoard(QWidget *parent) : QWidget(parent), m_hexSize(40.0) {
    setMouseTracking(true);
    MapReader mapReader;
    if (mapReader.loadRandomMap(":/maps/maps")) {
        this->initializeBoardFromMap(mapReader.getCellPositions());
    } else {
        this->initializeBoard(9, 5, 4);
    }
}
void HexBoard::initializeBoardFromMap(const QVector<CellPosition>& positions) {
    initializeBoard(9, 5, 4);

    for (const auto& pos : positions) {
        int hexX = pos.col;
        int hexY = pos.row;

        if (hexX % 2 == 1) {
            hexY--;
        }

        if (hexX >= 0 && hexX < m_cells.size() &&
            hexY >= 0 && hexY < m_cells[hexX].size()) {

            HexCell* cell = m_cells[hexX][hexY];
            QColor color;

            switch (pos.type) {
            case CellPosition::Player1: color = Qt::red; break;
            case CellPosition::Player2: color = Qt::blue; break;
            case CellPosition::Stone: color = Qt::darkGray; break;
            case CellPosition::Water: color = Qt::cyan; break;
            default: color = Qt::green;
            }

            cell->setColor(color);
        }
    }
    update();
}
void HexBoard::initializeBoard(int columns, int bigRows, int smallRows) {
    qDeleteAll(children());
    m_cells.clear();

    for (int x = 0; x < columns; x++) {
        QVector<HexCell*> column;
        int rows = (x % 2) ? smallRows : bigRows;

        for (int y = 0; y < rows; y++) {
            HexCell *cell = new HexCell(this);
            cell->setValue(x + y);
            column.append(cell);
        }
        m_cells.append(column);
    }

    updateGeometry();
}

QPointF HexBoard::hexToPixel(int x, int y) const {
    double hexWidth = m_hexSize * 2;
    double hexHeight = m_hexSize * std::sqrt(3);
    double horizSpacing = hexWidth * 0.75;

    double xPos = x * horizSpacing;
    double yPos = y * hexHeight;
    if (x % 2) yPos += hexHeight * 0.5;

    double boardPixelWidth = (m_cells.size() - 1) * horizSpacing + hexWidth;

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    for (int col = 0; col < m_cells.size(); ++col) {
        double colTop = (col % 2) ? hexHeight * 0.5 : 0;
        double colBottom = colTop + m_cells[col].size() * hexHeight;
        minY = std::min(minY, colTop);
        maxY = std::max(maxY, colBottom);
    }
    double boardPixelHeight = maxY - minY;

    double offsetX = (width() - boardPixelWidth) / 2.0;
    double offsetY = (height() - boardPixelHeight) / 2.0;

    return QPointF(xPos + offsetX, yPos + offsetY);
}

void HexBoard::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRadialGradient grad(rect().center(), qMax(width(), height())/2);
    grad.setColorAt(0, QColor(255, 215, 0, 30));
    grad.setColorAt(1, Qt::transparent);
    painter.fillRect(rect(), grad);
}

void HexBoard::resizeEvent(QResizeEvent *) {
    m_hexSize = qMin(width() / 13.0, height() / 11.0);

    for (int x = 0; x < m_cells.size(); x++) {
        for (int y = 0; y < m_cells[x].size(); y++) {
            QPointF center = hexToPixel(x, y);
            m_cells[x][y]->setGeometry(
                center.x() - m_hexSize,
                center.y() - m_hexSize,
                m_hexSize * 2,
                m_hexSize * 2
                );
        }
    }
    update();
}
