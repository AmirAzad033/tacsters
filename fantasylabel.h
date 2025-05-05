#ifndef FANTASYLABEL_H
#define FANTASYLABEL_H

#include <QLabel>
#include <QPropertyAnimation>

class FantasyLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(qreal glow READ glow WRITE setGlow NOTIFY glowChanged)

public:
    explicit FantasyLabel(const QString &text = "", QWidget *parent = nullptr);

    qreal glow() const;
    void setGlow(qreal intensity);
    void setFontSize(int size);

signals:
    void glowChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int m_fontSize;
    qreal m_glow;
    QPropertyAnimation *m_glowAnimation;
    QPropertyAnimation *m_pulseAnimation;
};
#endif
