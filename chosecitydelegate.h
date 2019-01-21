#ifndef CHOSECITYDELEGATE_H
#define CHOSECITYDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QDate>

class QObject;
class QWidget;

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(QStringList items = {}, QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
   QStringList m_itemsList;
};

class DateDelegate : public QItemDelegate
{
public:
    explicit DateDelegate(QDate min, QObject* parent = nullptr, QDate max = QDate::currentDate());
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
   QDate min;
   QDate max;

};


class SpinBoxDelegate : public QItemDelegate
{
public:
    explicit SpinBoxDelegate(int low, int high, QObject* parent = nullptr);

     QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
     void setEditorData(QWidget *editor, const QModelIndex &index) const override;
     void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
     void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int m_lowScore;
    int m_highScore;
};


#endif // CHOSECITYDELEGATE_H
