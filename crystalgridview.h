#ifndef CRYSTALGRIDVIEW_H
#define CRYSTALGRIDVIEW_H

#include <QWidget>
#include <QPointF>
#include <vector>
#include <functional>

class GameManager;
class GamePhases;
class Cell;
class Agent;

class CrystalGridView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF animPoint READ animPoint WRITE setAnimPoint)

public:
    explicit CrystalGridView(GameManager* gameManager, QWidget *parent = nullptr);
    void assignPhase(GamePhases* gamePhase);
    void refreshGrid();
    Agent* spawnAgent(const QString& agentTypeName, int ownerId);

signals:
    void unitDeployed(const QString& agentTypeName, int ownerId);
    void phaseEnded();

protected:
    void paintEvent(QPaintEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    GameManager* m_gameCore;
    GamePhases* m_phaseCtrl;

    Cell* m_activeCell = nullptr;
    std::vector<Cell*> m_moveOptions;
    std::vector<Cell*> m_attackOptions;

    QPointF m_animPoint;
    bool m_inMotion = false;
    Agent* m_animUnit = nullptr;

    QPointF animPoint() const;
    void setAnimPoint(const QPointF& pos);

    void slideUnit(Agent* unit, const QPointF& from, const QPointF& to, std::function<void()> onDone = {});
    void strikeUnit(Agent* attacker, const QPointF& from, const QPointF& target, std::function<void()> onDone = {});

    std::pair<int,int> mapPointToCell(const QPointF &pt) const;
    void processMove(Cell* fromCell, Cell* toCell);
    std::vector<Cell*> collectTargets(Cell* fromCell);
    void resolveCombat(Cell* attackerCell, Cell* defenderCell);

    QPointF centerOf(Cell* cell) const;
};

#endif
