#ifndef AGENT_H
#define AGENT_H

#include <QString>
#include <QPixmap>

class Agent {
public:
    Agent(const QString &name, const QString &type, const QPixmap &icon, int h, int m, int d, int a, int o);

    QString name() const;
    QString type() const;
    QPixmap icon() const;

    int getHp() const;
    void setHP(int h);
    int getOriginalHP();
    int getMobility() const;
    int getDamage() const;
    int getAttackRange() const;
    int getOwner() const;
    bool canWalkOnWater() const { return walk_water; }
    bool canWalkOnRock() const { return walk_rock; }
    bool canStandOnWater() const { return stand_water; }
    bool canStandOnRock() const { return stand_rock; }

protected:
    bool walk_water;
    bool walk_rock;
    bool stand_water;
    bool stand_rock;
    QString m_name;
    QString m_type;
    QPixmap m_icon;

    int hp;
    int originalHP;
    int mobility;
    int damage;
    int attack_range;
    int owner;
};

class Grounded : public Agent {
public:
    Grounded(const QString &name, const QPixmap &icon, int h, int m, int d, int a, int o);
};

class Water_Walking : public Agent {
public:
    Water_Walking(const QString &name, const QPixmap &icon, int h, int m, int d, int a, int o);
};

class Flying : public Agent {
public:
    Flying(const QString &name, const QPixmap &icon, int h, int m, int d, int a, int o);
};

class Floating : public Agent {
public:
    Floating(const QString &name, const QPixmap &icon, int h, int m, int d, int a, int o);
};

#endif
