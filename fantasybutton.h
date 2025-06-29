#ifndef FANTASYBUTTON_H
#define FANTASYBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

class FantasyButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)
    Q_PROPERTY(int pressDepth READ pressDepth WRITE setPressDepth)

public:
    explicit FantasyButton(const QString &text, QWidget *parent = nullptr);
    qreal glowIntensity() const { return m_glow; }
    void setGlowIntensity(qreal intensity);
    int pressDepth() const { return m_pressDepth; }
    void setPressDepth(int depth);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal m_glow = 0.0;
    int m_pressDepth = 0;
    QPropertyAnimation *m_glowAnimation;
    QPropertyAnimation *m_pressAnimation;
    QPropertyAnimation *m_pulseAnimation;
};

#endif
