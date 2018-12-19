#include "lineediticon.h"
#include <QPainter>

LineEditIcon::LineEditIcon(const QIcon icon, QWidget *parent)
    : QLineEdit(parent)
{
    setIcon(icon);
    setClearButtonEnabled(true);
}

LineEditIcon::~LineEditIcon()
{
}

void LineEditIcon::setIcon(QIcon icon)
{
    m_icon = icon;
    if (m_icon.isNull())
        setTextMargins(1, 1, 1, 1);
    else
        setTextMargins(25, 1, 1, 1);
}

void LineEditIcon::paintEvent(QPaintEvent * event)
{
    QLineEdit::paintEvent(event);
    if (!m_icon.isNull()) {
        QPainter painter(this);
        QPixmap pxm = m_icon.pixmap(height() - 6, height());
        int x = 2, cx = pxm.width();

        painter.drawPixmap(x, 10, pxm);
        painter.setPen(QColor("lightgrey"));
        painter.drawLine(cx + 2, 3, cx + 2, height() - 4);
    }
}
