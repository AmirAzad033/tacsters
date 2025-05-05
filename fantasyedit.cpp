#include "fantasyedit.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QTextBlock>
#include <QTimer>

FantasyEdit::FantasyEdit(QWidget *parent)
    : QTextEdit(parent)
{
    // Core setup
    setMinimumHeight(60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);

    // Text styling
    QFont medievalFont("Palatino Linotype", 14);
    setFont(medievalFont);
    setStyleSheet("QTextEdit {"
                 "color: #f0e6d2;"
                 "selection-color: #1a1a1a;"
                 "selection-background-color: #d4af37;"
                 "padding: 12px 18px;"
                 "}");

    // Visual effects
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(210, 160, 50, 120));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);

    setupAnimations();
}

void FantasyEdit::setupAnimations()
{
    m_glowAnimation = new QPropertyAnimation(this, "glowIntensity", this);
    m_glowAnimation->setDuration(400);
    m_glowAnimation->setEasingCurve(QEasingCurve::OutQuint);

    m_resizeAnimation = new QPropertyAnimation(this, "minimumHeight", this);
    m_resizeAnimation->setDuration(300);
    m_resizeAnimation->setEasingCurve(QEasingCurve::OutBack);
}

void FantasyEdit::setGlowIntensity(float intensity)
{
    m_glow = qBound(0.3f, intensity, 1.0f);
    update();
}

void FantasyEdit::paintEvent(QPaintEvent *event)
{
    // First draw the custom frame
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawFantasyFrame(painter);

    // Then let QTextEdit handle the text
    QTextEdit::paintEvent(event);

    // Add parchment-like inner shadow
    QPainterPath innerPath;
    innerPath.addRoundedRect(rect().adjusted(3, 3, -3, -3), 8, 8);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 30));
    painter.drawPath(innerPath);
}

void FantasyEdit::drawFantasyFrame(QPainter &painter)
{
    // Base gradient
    QLinearGradient grad(0, 0, width(), 0);
    grad.setColorAt(0, QColor(40, 30, 20));
    grad.setColorAt(1, QColor(70, 50, 30));

    // Glow effect
    if (m_glow > 0.3f) {
        QRadialGradient glowGrad(width()/2, height()/2, qMax(width(), height())/2);
        glowGrad.setColorAt(0, QColor(210, 160, 50, 150 * m_glow));
        glowGrad.setColorAt(1, Qt::transparent);
        painter.setBrush(glowGrad);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());
    }

    // Main frame
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);

    painter.setPen(QPen(QColor(210, 160, 50, 200), 2.5));
    painter.setBrush(grad);
    painter.drawPath(path);

    // Inner gold trim
    painter.setPen(QPen(QColor(210, 160, 50, 120), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 8, 8);
}

void FantasyEdit::focusInEvent(QFocusEvent *event)
{
    m_glowAnimation->stop();
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(1.0f);
    m_glowAnimation->start();

    m_resizeAnimation->stop();
    m_resizeAnimation->setStartValue(height());
    m_resizeAnimation->setEndValue(90);
    m_resizeAnimation->start();

    QTextEdit::focusInEvent(event);
}

void FantasyEdit::focusOutEvent(QFocusEvent *event)
{
    m_glowAnimation->stop();
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(0.3f);
    m_glowAnimation->start();

    m_resizeAnimation->stop();
    m_resizeAnimation->setStartValue(height());
    m_resizeAnimation->setEndValue(60);
    m_resizeAnimation->start();

    QTextEdit::focusOutEvent(event);
}

void FantasyEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    update(); // Ensure frame redraws on resize
}
