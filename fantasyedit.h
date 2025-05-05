#ifndef FANTASYEDIT_H
#define FANTASYEDIT_H

#include <QTextEdit>
#include <QPropertyAnimation>

class FantasyEdit : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(float glowIntensity READ glowIntensity WRITE setGlowIntensity)

public:
    explicit FantasyEdit(QWidget *parent = nullptr);

    float glowIntensity() const { return m_glow; }
    void setGlowIntensity(float intensity);

protected:
    void paintEvent(QPaintEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupAnimations();
    void drawFantasyFrame(QPainter &painter);

    float m_glow = 0.3f;
    QPropertyAnimation *m_glowAnimation;
    QPropertyAnimation *m_resizeAnimation;
};

#endif // FANTASYEDIT_H
