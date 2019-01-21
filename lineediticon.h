#ifndef LINEEDITICON_H
#define LINEEDITICON_H

#include <QWidget>
#include <QLineEdit>
#include <QIcon>


class LineEditIcon : public QLineEdit
{
    Q_OBJECT

public:
    LineEditIcon(const QIcon icon, QWidget *parent = Q_NULLPTR);
    void setIcon(QIcon icon);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QIcon m_icon;
};

#endif // LINEEDITICON_H
