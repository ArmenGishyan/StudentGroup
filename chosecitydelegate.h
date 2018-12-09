#ifndef CHOSECITYDELEGATE_H
#define CHOSECITYDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QItemDelegate>
#include <QModelIndex>


class ChoseCityDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ChoseCityDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class DateDelegate : public QItemDelegate
{
public:
    explicit DateDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // CHOSECITYDELEGATE_H
