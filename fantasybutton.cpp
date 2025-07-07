#include "fantasybutton.h"
#include <QPainter>
#include <QPainterPath>

FantasyButton::FantasyButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setMinimumSize(320, 60);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
		//implementing shadows
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(255, 50, 50, 150));
    shadow->setOffset(0, 5);
    setGraphicsEffect(shadow);

    m_glowAnimation = new QPropertyAnimation(this, "glowIntensity");
    m_glowAnimation->setDuration(300);

    m_pressAnimation = new QPropertyAnimation(this, "pressDepth");
    m_pressAnimation->setDuration(100);

    m_pulseAnimation = new QPropertyAnimation(this, "glowIntensity");
    m_pulseAnimation->setDuration(2000);
    m_pulseAnimation->setLoopCount(-1);
    m_pulseAnimation->setStartValue(0.3);
    m_pulseAnimation->setEndValue(0.7);
    m_pulseAnimation->start();
}

void FantasyButton::setGlowIntensity(qreal intensity)
{
    m_glow = intensity;
    update();
}

void FantasyButton::setPressDepth(int depth)
{
    m_pressDepth = depth;
    update();
}

void FantasyButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0, QColor(0, 150, 255).lighter(100 + m_glow * 50));
    gradient.setColorAt(1, QColor(255, 50, 50).lighter(100 + m_glow * 50));

    QRect btnRect = rect().adjusted(0, -m_pressDepth, 0, m_pressDepth);
    int radius = btnRect.height() / 2;

    QPainterPath path;
    path.addRoundedRect(btnRect, radius, radius);

    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawPath(path);

    QPen framePen(QColor(255, 255, 255, 200), 3);
    painter.setPen(framePen);
    painter.setBrush(Qt::NoBrush);

    QRect innerRect = btnRect.adjusted(5, 5, -5, -5);
    QPainterPath innerPath;
    innerPath.addRoundedRect(innerRect, radius - 5, radius - 5);
    painter.drawPath(innerPath);

    painter.setPen(Qt::white);
    QFont font("", 18);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    painter.setFont(font);

    painter.setPen(QColor(0, 0, 0, 150));
    painter.drawText(btnRect.adjusted(2, 2, 2, 2), Qt::AlignCenter, text());

    painter.setPen(Qt::white);
    painter.drawText(btnRect, Qt::AlignCenter, text());

    QLinearGradient edgeGrad(0, 0, 0, height());
    edgeGrad.setColorAt(0, QColor(255, 255, 255, 80));
    edgeGrad.setColorAt(1, Qt::transparent);
    painter.setPen(QPen(QBrush(edgeGrad), 2));
    painter.drawPath(path);
}

void FantasyButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    m_pulseAnimation->pause();
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(1.0);
    m_glowAnimation->start();
}

void FantasyButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(0.3);
    m_glowAnimation->start();
    m_pulseAnimation->resume();
}

void FantasyButton::mousePressEvent(QMouseEvent *event)
{
    m_pressAnimation->setStartValue(m_pressDepth);
    m_pressAnimation->setEndValue(5);
    m_pressAnimation->start();
    QPushButton::mousePressEvent(event);
}

void FantasyButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressAnimation->setStartValue(m_pressDepth);
    m_pressAnimation->setEndValue(0);
    m_pressAnimation->start();
    QPushButton::mouseReleaseEvent(event);
}
