#include "fantasylabel.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
FantasyLabel::FantasyLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent), m_fontSize(16), m_glow(0.5)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setAlignment(Qt::AlignCenter);
    setWordWrap(true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(255, 100, 255, 150));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);

    QFont font;
    font.setPointSize(m_fontSize);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    font.setBold(true);
    font.setFamily("Arial");
    setFont(font);

    m_glowAnimation = new QPropertyAnimation(this, "glow");
    m_glowAnimation->setDuration(300);

    m_pulseAnimation = new QPropertyAnimation(this, "glow");
    m_pulseAnimation->setDuration(2000);
    m_pulseAnimation->setLoopCount(-1);
    m_pulseAnimation->setStartValue(0.5);
    m_pulseAnimation->setEndValue(0.9);
    m_pulseAnimation->start();
}
qreal FantasyLabel::glow() const
{
    return m_glow;
}

void FantasyLabel::setGlow(qreal intensity)
{
    if (qFuzzyCompare(m_glow, intensity))
        return;

    m_glow = intensity;
    update();
    emit glowChanged();
}

void FantasyLabel::setFontSize(int size)
{
    m_fontSize = size;
    QFont f = font();
    f.setPointSize(size);
    setFont(f);
    update();
}

void FantasyLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFontMetrics fm(font());
    QRect textRect = rect().adjusted(10, 10, -10, -10);

    QLinearGradient outlineGradient(0, 0, width(), 0);
    outlineGradient.setColorAt(0, QColor(255, 100, 255));
    outlineGradient.setColorAt(1, QColor(100, 100, 255));

    QLinearGradient textGradient(0, 0, width(), 0);
    textGradient.setColorAt(0, QColor(255, 255, 255));
    textGradient.setColorAt(1, QColor(220, 220, 255));

    QPainterPath textPath;
    QStringList lines = text().split('\n');
    int lineHeight = fm.height();
    int totalHeight = lines.count() * lineHeight;
    int startY = (height() - totalHeight) / 2 + fm.ascent();

    for (int i = 0; i < lines.count(); i++) {
        QString line = lines[i];
        int lineWidth = fm.horizontalAdvance(line);
        int startX = (width() - lineWidth) / 2;
        textPath.addText(startX, startY + (i * lineHeight), font(), line);
    }


    painter.setPen(QPen(outlineGradient, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(textPath);

    painter.setPen(Qt::NoPen);
    painter.setBrush(textGradient);
    painter.drawPath(textPath);

    if (m_glow > 0.6) {
        painter.setPen(QPen(QColor(255, 255, 255, m_glow * 80), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(textPath);
    }
}

void FantasyLabel::enterEvent(QEnterEvent *event)
{
    QLabel::enterEvent(event);
    m_pulseAnimation->pause();
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(1.0);
    m_glowAnimation->start();
}

void FantasyLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_glowAnimation->setStartValue(m_glow);
    m_glowAnimation->setEndValue(0.5);
    m_glowAnimation->start();
    m_pulseAnimation->resume();
}
