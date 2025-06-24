#include "crystalgridview.h"
#include <QPainter>
#include <QPolygonF>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPixmap>
#include <cmath>
#include <QDebug>
#include <QDataStream>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "agent.h"
#include "cell.h"
#include "gamemanager.h"
#include "gamephases.h"
#include <QQueue>

CrystalGridView::CrystalGridView(GameManager* gameManager, QWidget *parent)
    : QWidget(parent), m_gameCore(gameManager), m_phaseCtrl(nullptr),
    m_activeCell(nullptr), m_animUnit(nullptr), m_inMotion(false)
{
    setAcceptDrops(true);
    setMinimumSize(927, 600);
    //setAttribute(Qt::WA_OpaquePaintEvent);
}

void CrystalGridView::assignPhase(GamePhases* gamePhase) {
    m_phaseCtrl = gamePhase;
}

void CrystalGridView::refreshGrid() {
    update();
}

Agent* CrystalGridView::spawnAgent(const QString& agentTypeName, int ownerId) {
    if (agentTypeName == "Billy") return new Water_Walking("Billy", QPixmap(":/agents/agents/Billy.png"), 320, 3, 90, 1, ownerId);
    if (agentTypeName == "Reketon") return new Water_Walking("Reketon", QPixmap(":/agents/agents/Reketon.png"), 320, 2, 80, 2, ownerId);
    if (agentTypeName == "Angus") return new Water_Walking("Angus", QPixmap(":/agents/agents/Angus.png"), 400, 2, 100, 1, ownerId);
    if (agentTypeName == "Duraham") return new Water_Walking("Duraham", QPixmap(":/agents/agents/Duraham.png"), 320, 2, 100, 2, ownerId);
    if (agentTypeName == "Colonel_Baba") return new Water_Walking("Colonel_Baba", QPixmap(":/agents/agents/Colonel_Baba.png"), 400, 2, 100, 1, ownerId);
    if (agentTypeName == "Medusa") return new Water_Walking("Medusa", QPixmap(":/agents/agents/Medusa.png"), 320, 2, 90, 2, ownerId);
    if (agentTypeName == "Bunka") return new Water_Walking("Bunka", QPixmap(":/agents/agents/Bunka.png"), 320, 3, 100, 1, ownerId);
    if (agentTypeName == "Sanka") return new Water_Walking("Sanka", QPixmap(":/agents/agents/Sanka.png"), 320, 3, 100, 1, ownerId);

    if (agentTypeName == "Sir_Lamorak") return new Grounded("Sir_Lamorak", QPixmap(":/agents/agents/Sir_Lamorak.png"), 320, 3, 110, 1, ownerId);
    if (agentTypeName == "Kabu") return new Grounded("Kabu", QPixmap(":/agents/agents/Kabu.png"), 400, 2, 120, 1, ownerId);
    if (agentTypeName == "Rajakal") return new Grounded("Rajakal", QPixmap(":/agents/agents/Rajakal.png"), 320, 2, 130, 1, ownerId);
    if (agentTypeName == "Salih") return new Grounded("Salih", QPixmap(":/agents/agents/Salih.png"), 400, 2, 80, 1, ownerId);
    if (agentTypeName == "Khan") return new Grounded("Khan", QPixmap(":/agents/agents/Khan.png"), 320, 2, 90, 1, ownerId);
    if (agentTypeName == "Boi") return new Grounded("Boi", QPixmap(":/agents/agents/Boi.png"), 400, 2, 100, 1, ownerId);
    if (agentTypeName == "Eloi") return new Grounded("Eloi", QPixmap(":/agents/agents/Eloi.png"), 240, 2, 100, 2, ownerId);
    if (agentTypeName == "Kanar") return new Grounded("Kanar", QPixmap(":/agents/agents/Kanar.png"), 160, 2, 100, 2, ownerId);
    if (agentTypeName == "Elsa") return new Grounded("Elsa", QPixmap(":/agents/agents/Elsa.png"), 320, 2, 140, 2, ownerId);
    if (agentTypeName == "Karissa") return new Grounded("Karissa", QPixmap(":/agents/agents/Karissa.png"), 280, 2, 80, 2, ownerId);
    if (agentTypeName == "Sir_Philip") return new Grounded("Sir_Philip", QPixmap(":/agents/agents/Sir_Philip.png"), 400, 2, 100, 1, ownerId);
    if (agentTypeName == "Frost") return new Grounded("Frost", QPixmap(":/agents/agents/Frost.png"), 260, 2, 80, 2, ownerId);
    if (agentTypeName == "Tusk") return new Grounded("Tusk", QPixmap(":/agents/agents/Tusk.png"), 400, 2, 100, 1, ownerId);

    if (agentTypeName == "Rambu") return new Flying("Rambu", QPixmap(":/agents/agents/Rambu.png"), 320, 3, 120, 1, ownerId);

    if (agentTypeName == "Sabrina") return new Floating("Sabrina", QPixmap(":/agents/agents/Sabrina.png"), 320, 3, 100, 1, ownerId);
    if (agentTypeName == "Death") return new Floating("Death", QPixmap(":/agents/agents/Death.png"), 240, 3, 120, 2, ownerId);

    return nullptr;
}

QPointF CrystalGridView::animPoint() const {
    return m_animPoint;
}

void CrystalGridView::setAnimPoint(const QPointF& pos) {
    m_animPoint = pos;
    update();
}

void CrystalGridView::slideUnit(Agent* unit, const QPointF& from, const QPointF& to, std::function<void()> onDone) {
    if (!unit) {
        if (onDone) onDone();
        return;
    }

    m_animUnit = unit;
    m_animPoint = from;
    m_inMotion = true;

    QPropertyAnimation* anim = new QPropertyAnimation(this, "animPoint");
    anim->setDuration(450);
    anim->setStartValue(from);
    anim->setEndValue(to);
    anim->setEasingCurve(QEasingCurve::OutBounce);

    connect(anim, &QPropertyAnimation::finished, this, [=]() {
        m_inMotion = false;
        m_animUnit = nullptr;
        if (onDone) onDone();
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CrystalGridView::strikeUnit(Agent* attacker, const QPointF& from, const QPointF& target, std::function<void()> onDone) {
    if (!attacker) {
        if (onDone) onDone();
        return;
    }

    m_animUnit = attacker;
    m_animPoint = from;
    m_inMotion = true;

    QPointF approach = from + (target - from) * 0.6;

    QSequentialAnimationGroup* seq = new QSequentialAnimationGroup(this);

    QPropertyAnimation* forward = new QPropertyAnimation(this, "animPoint");
    forward->setDuration(160);
    forward->setStartValue(from);
    forward->setEndValue(approach);
    forward->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation* impact = new QPropertyAnimation(this, "animPoint");
    impact->setDuration(140);
    impact->setStartValue(approach);
    impact->setEndValue(target);
    impact->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* recoil = new QPropertyAnimation(this, "animPoint");
    recoil->setDuration(140);
    recoil->setStartValue(target);
    recoil->setEndValue(from);
    recoil->setEasingCurve(QEasingCurve::InCubic);

    seq->addAnimation(forward);
    seq->addAnimation(impact);
    seq->addPause(80);
    seq->addAnimation(recoil);

    connect(seq, &QSequentialAnimationGroup::finished, this, [=]() {
        m_inMotion = false;
        m_animUnit = nullptr;
        QTimer::singleShot(80, this, [=]() {
            if (onDone) onDone();
        });
    });

    seq->start(QAbstractAnimation::DeleteWhenStopped);
}

void CrystalGridView::paintEvent(QPaintEvent *) {
    if (!m_gameCore) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double hexSize = 48.0;
    double hexWidth = 2 * hexSize;
    double hexHeight = std::sqrt(3.0) * hexSize;
    double horizSpacing = hexWidth * 1.5;
    double vertSpacing = hexHeight * 0.5;

    double totalRows = m_gameCore->getRows();
    double totalCols = 5;
    double mapWidth = (totalCols - 1) * horizSpacing + hexWidth;
    double mapHeight = (totalRows - 1) * vertSpacing + hexHeight;

    double offsetX = (width() - mapWidth) / 2.0;
    double offsetY = (height() - mapHeight) / 2.0;

    for (int r = 0; r < m_gameCore->getRows(); ++r) {
        for (int c = 0; c < m_gameCore->getCols(r); ++c) {
            Cell* cell = m_gameCore->accessCell(r, c);
            if (!cell) continue;

            double x = c * horizSpacing + offsetX;
            if (r % 2 == 1) x += horizSpacing / 2.0;
            double y = r * vertSpacing + offsetY;

            QPolygonF hex;
            for (int i = 0; i < 6; ++i) {
                double angle = 2 * M_PI / 6 * (i + 0.5);
                hex << QPointF(x + hexSize * cos(angle), y + hexSize * sin(angle));
            }

            QColor terrainColor;
            if (cell->owner == 1) terrainColor = QColor("#F44336");
            else if (cell->owner == 2) terrainColor = QColor("#2196F3");
            else if (cell->terrain == TerrainType::Water) terrainColor = QColor("#4FC3F7");
            else if (cell->terrain == TerrainType::Rock) terrainColor = QColor("#546E7A");
            else terrainColor = QColor("#9C27B0");

            QLinearGradient grad(QPointF(x - hexSize, y - hexSize), QPointF(x + hexSize, y + hexSize));
            grad.setColorAt(0.0, terrainColor.lighter(120));
            grad.setColorAt(1.0, terrainColor.darker(110));
            painter.setBrush(grad);

            QColor borderColor = terrainColor.darker(160);
            int borderWidth = std::max(1.0, hexSize * 0.06);

            QColor fillColor = terrainColor;

            if (cell == m_activeCell) {
                fillColor = QColor(255, 105, 180, 180);
                borderColor = QColor(255, 105, 180);
            }
            else if (cell->occupiedAgent &&
                     std::find(m_attackOptions.begin(), m_attackOptions.end(), cell) != m_attackOptions.end() &&
                     cell->occupiedAgent->getOwner() != m_phaseCtrl->getCurrentPlayer())
            {
                fillColor = QColor(255, 0, 0, 100);
                borderColor = QColor(255, 0, 0);
            }
            else if (std::find(m_moveOptions.begin(), m_moveOptions.end(), cell) != m_moveOptions.end()) {
                fillColor = QColor(255, 182, 193, 140);
                borderColor = QColor(255, 182, 193);
            }

            painter.setBrush(fillColor);
            painter.setPen(QPen(borderColor, borderWidth));
            painter.drawPolygon(hex);

            if (cell->occupiedAgent) {
                if (!(m_inMotion && cell->occupiedAgent == m_animUnit)) {
                    QPixmap icon = cell->occupiedAgent->icon();
                    if (!icon.isNull()) {
                        QRectF drawRect(x - hexSize * 0.8, y - hexSize * 0.8, hexSize * 1.6, hexSize * 1.6);
                        painter.drawPixmap(drawRect, icon, icon.rect());
                    }

                    double maxHp = cell->occupiedAgent->getOriginalHP();
                    double hpRatio = double(cell->occupiedAgent->getHp()) / double(maxHp);
                    int barW = 56;
                    int barH = 6;
                    int bx = int(x - barW / 2);
                    int by = int(y - hexSize - 10);

                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(0,0,0,200));
                    painter.drawRoundedRect(bx - 1, by - 1, barW + 2, barH + 2, 2, 2);

                    QColor hpColor = QColor::fromRgb(
                        255,
                        int(182 * hpRatio),
                        int(193 * hpRatio)
                        );
                    painter.setBrush(hpColor);
                    painter.drawRect(bx, by, int(barW * hpRatio), barH);
                }
            }
        }
    }

    if (m_inMotion && m_animUnit) {
        double hexSize = 48.0;
        QPixmap icon = m_animUnit->icon();
        if (!icon.isNull()) {
            QRectF drawRect(m_animPoint.x() - hexSize, m_animPoint.y() - hexSize, hexSize * 2.0, hexSize * 2.0);
            painter.setOpacity(1.0);
            painter.drawPixmap(drawRect, icon, icon.rect());
        }

        double maxHp = m_animUnit->getOriginalHP();
        double hpRatio = double(m_animUnit->getHp()) / double(maxHp);
        int barW = 56;
        int barH = 6;
        int bx = int(m_animPoint.x() - barW / 2);
        int by = int(m_animPoint.y() - hexSize - 10);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0,0,0,200));
        painter.drawRoundedRect(bx - 1, by - 1, barW + 2, barH + 2, 2, 2);

        QColor hpColor = QColor::fromRgb(
            255,
            int(182 * hpRatio),
            int(193 * hpRatio)
            );
        painter.setBrush(hpColor);
        painter.drawRect(bx, by, int(barW * hpRatio), barH);
    }
}


void CrystalGridView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-yourgame-agent-type")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void CrystalGridView::dropEvent(QDropEvent *event) {
    if (!m_gameCore || !m_phaseCtrl) {
        event->ignore();
        return;
    }

    if (!event->mimeData()->hasFormat("application/x-yourgame-agent-type")) {
        event->ignore();
        return;
    }

    QPointF dropPos = event->position();
    auto [row, col] = mapPointToCell(dropPos);
    Cell* target = m_gameCore->accessCell(row, col);

    if (!target) {
        event->ignore();
        return;
    }

    QByteArray agentData = event->mimeData()->data("application/x-yourgame-agent-type");
    QDataStream stream(&agentData, QIODevice::ReadOnly);
    QString agentType;
    int ownerId = 0;
    stream >> agentType >> ownerId;

    if (m_phaseCtrl->getCurrentPhase() != GamePhase::Deploy) {
        event->ignore();
        return;
    }

    if (m_phaseCtrl->getCurrentPlayer() != ownerId) {
        event->ignore();
        return;
    }

    Agent* newUnit = spawnAgent(agentType, ownerId);
    if (!newUnit) {
        event->ignore();
        return;
    }

    bool valid = false;
    if (!target->occupiedAgent) {
        if (ownerId == 1 && target->owner == 1) valid = true;
        if (ownerId == 2 && target->owner == 2) valid = true;
    }

    if (valid) {
        m_gameCore->addAgentToCell(newUnit, row, col);
        refreshGrid();
        event->acceptProposedAction();
        emit unitDeployed(agentType, ownerId);
    } else {
        delete newUnit;
        event->ignore();
    }
}

std::pair<int,int> CrystalGridView::mapPointToCell(const QPointF &pt) const {
    double hexSize = 48.0;
    double hexWidth = 2 * hexSize;
    double hexHeight = std::sqrt(3.0) * hexSize;
    double horizSpacing = hexWidth * 1.5;
    double vertSpacing = hexHeight * 0.5;

    double totalRows = m_gameCore->getRows();
    double totalCols = 5;
    double mapWidth = (totalCols - 1) * horizSpacing + hexWidth;
    double mapHeight = (totalRows - 1) * vertSpacing + hexHeight;

    double offsetX = (width() - mapWidth) / 2.0;
    double offsetY = (height() - mapHeight) / 2.0;

    for (int r = 0; r < m_gameCore->getRows(); ++r) {
        for (int c = 0; c < m_gameCore->getCols(r); ++c) {
            Cell* cell = m_gameCore->accessCell(r, c);
            if (!cell) continue;

            double x = c * horizSpacing + offsetX;
            if (r % 2 == 1) x += horizSpacing / 2.0;
            double y = r * vertSpacing + offsetY;

            double dx = pt.x() - x;
            double dy = pt.y() - y;

            if (std::sqrt(dx*dx + dy*dy) <= hexSize) return {r, c};
        }
    }

    return {-1, -1};
}

void CrystalGridView::mousePressEvent(QMouseEvent *event) {
    if (m_inMotion) return;

    if (event->button() == Qt::LeftButton && m_phaseCtrl && m_phaseCtrl->getCurrentPhase() == GamePhase::Fight) {
        auto [row, col] = mapPointToCell(event->position());
        Cell* clicked = m_gameCore->accessCell(row, col);

        if (!clicked) {
            m_activeCell = nullptr;
            m_moveOptions.clear();
            m_attackOptions.clear();
            update();
            return;
        }

        if (m_activeCell) {
            if (std::find(m_moveOptions.begin(), m_moveOptions.end(), clicked) != m_moveOptions.end()) {
                processMove(m_activeCell, clicked);
            } else if (std::find(m_attackOptions.begin(), m_attackOptions.end(), clicked) != m_attackOptions.end()) {
                resolveCombat(m_activeCell, clicked);
            } else {
                m_activeCell = nullptr;
                m_moveOptions.clear();
                m_attackOptions.clear();
                update();
            }
        } else if (clicked->occupiedAgent && clicked->occupiedAgent->getOwner() == m_phaseCtrl->getCurrentPlayer()) {
            m_activeCell = clicked;
            m_moveOptions = m_gameCore->computeReachableCells(m_activeCell, m_activeCell->occupiedAgent->getMobility(), m_phaseCtrl->getCurrentPlayer());
            m_attackOptions = collectTargets(m_activeCell);
            update();
        } else {
            m_activeCell = nullptr;
            m_moveOptions.clear();
            m_attackOptions.clear();
            update();
        }
    }
}

void CrystalGridView::processMove(Cell* fromCell, Cell* toCell) {
    if (!fromCell || !toCell || !fromCell->occupiedAgent) return;

    QPointF start = centerOf(fromCell);
    QPointF end = centerOf(toCell);
    Agent* mover = fromCell->occupiedAgent;

    slideUnit(mover, start, end, [=]() {
        m_gameCore->transferAgentToCell(mover, fromCell->row, fromCell->col, toCell->row, toCell->col);

        m_activeCell = nullptr;
        m_moveOptions.clear();
        m_attackOptions.clear();
        refreshGrid();

        emit phaseEnded();
    });
}

std::vector<Cell*> CrystalGridView::collectTargets(Cell* fromCell) {
    if (!fromCell || !fromCell->occupiedAgent) return {};

    std::vector<Cell*> targets;
    QQueue<Cell*> q;
    m_gameCore->resetTraversal();

    fromCell->distance = 0;
    fromCell->visited = true;
    q.enqueue(fromCell);

    while (!q.isEmpty()) {
        Cell* cur = q.dequeue();

        if (cur->distance >= fromCell->occupiedAgent->getAttackRange()) continue;

        for (Cell* n : cur->neighbors) {
            if (!n->visited) {
                n->visited = true;
                n->distance = cur->distance + 1;
                n->parent = cur;
                q.enqueue(n);
            }
        }
    }

    for (size_t r = 0; r < m_gameCore->getRows(); ++r) {
        for (size_t c = 0; c < m_gameCore->getCols(r); ++c) {
            Cell* cell = m_gameCore->accessCell(r, c);
            if (cell && cell->occupiedAgent && cell->occupiedAgent->getOwner() != fromCell->occupiedAgent->getOwner()) {
                if (cell->distance != -1 && cell->distance <= fromCell->occupiedAgent->getAttackRange()) {
                    targets.push_back(cell);
                }
            }
        }
    }

    return targets;
}

void CrystalGridView::resolveCombat(Cell* attackerCell, Cell* defenderCell) {
    if (!attackerCell || !defenderCell || !attackerCell->occupiedAgent || !defenderCell->occupiedAgent) return;

    Agent* attacker = attackerCell->occupiedAgent;
    Agent* defender = defenderCell->occupiedAgent;

    int attRow = attackerCell->row;
    int attCol = attackerCell->col;
    int defRow = defenderCell->row;
    int defCol = defenderCell->col;

    QPointF start = centerOf(attackerCell);
    QPointF target = centerOf(defenderCell);

    strikeUnit(attacker, start, target, [=]() {
        if (!attacker || !defender) return;

        defender->setHP(defender->getHp() - attacker->getDamage());
        attacker->setHP(attacker->getHp() - (defender->getDamage() / 2));

        bool attackerDied = attacker->getHp() <= 0;
        bool defenderDied = defender->getHp() <= 0;

        if (defenderDied) {
            m_gameCore->removeAgentFromCell(defRow, defCol);
        }

        if (attackerDied) {
            m_gameCore->removeAgentFromCell(attRow, attCol);
        }

        if (!attackerDied) {
            Cell* currentAttackerCell = m_gameCore->accessCell(attRow, attCol);
            if (currentAttackerCell && currentAttackerCell->occupiedAgent) {
                std::vector<Cell*> neighbors = currentAttackerCell->neighbors;
                std::vector<Cell*> valid;
                for (Cell* n : neighbors) {
                    if (!n->occupiedAgent) {
                        bool can = false;
                        if (attacker->type() == "Grounded" && n->terrain == TerrainType::Free) can = true;
                        else if (attacker->type() == "Water_Walking" && (n->terrain == TerrainType::Free || n->terrain == TerrainType::Water)) can = true;
                        else if (attacker->type() == "Flying" && n->terrain == TerrainType::Free) can = true;
                        else if (attacker->type() == "Floating" && (n->terrain == TerrainType::Free || n->terrain == TerrainType::Water || n->terrain == TerrainType::Rock)) can = true;

                        if (can) valid.push_back(n);
                    }
                }

                if (!valid.empty()) {
                    std::srand(static_cast<unsigned int>(std::time(nullptr)));
                    int idx = std::rand() % valid.size();
                    Cell* dest = valid[idx];

                    QPointF s = centerOf(currentAttackerCell);
                    QPointF e = centerOf(dest);

                    slideUnit(attacker, s, e, [=]() {
                        m_gameCore->transferAgentToCell(attacker, attRow, attCol, dest->row, dest->col);
                        refreshGrid();
                    });
                }
            }
        }

        m_activeCell = nullptr;
        m_moveOptions.clear();
        m_attackOptions.clear();
        refreshGrid();

        emit phaseEnded();
    });
}

QPointF CrystalGridView::centerOf(Cell* cell) const {
    if (!m_gameCore || !cell) return QPointF(-1, -1);

    double hexSize = 48.0;
    double hexWidth = 2 * hexSize;
    double hexHeight = std::sqrt(3.0) * hexSize;
    double horizSpacing = hexWidth * 1.5;
    double vertSpacing = hexHeight * 0.5;

    double totalRows = m_gameCore->getRows();
    double totalCols = 5;
    double mapWidth = (totalCols - 1) * horizSpacing + hexWidth;
    double mapHeight = (totalRows - 1) * vertSpacing + hexHeight;

    double offsetX = (width() - mapWidth) / 2.0;
    double offsetY = (height() - mapHeight) / 2.0;

    double x = cell->col * horizSpacing + offsetX;
    if (cell->row % 2 == 1) x += horizSpacing / 2.0;
    double y = cell->row * vertSpacing + offsetY;

    return QPointF(x, y);
}
