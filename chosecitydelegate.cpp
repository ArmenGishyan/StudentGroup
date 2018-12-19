#include "chosecitydelegate.h"
#include <QComboBox>
#include <QVariant>
#include <QDebug>
#include <QDateEdit>
#include <QDate>

ChoseCityDelegate::ChoseCityDelegate(QObject* parent):QItemDelegate (parent)
{
    qDebug()<<"----------------------------------------constructor--------------";
}

QWidget* ChoseCityDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* cbox = new QComboBox(parent);
    QStringList cities = {"Yerevan", "Ararat", "Armavir", "Aragatsotn", "Gegharkunik", "Kotayk", "Lori", "Shirak", "Syunik", "Tavush", "Vayots Dzor", "Artsakh"};
    cbox->insertItems(0,cities);
    qDebug()<<"---------------------createEditor----------------------------------";
    setEditorData(cbox, index);
    cbox->show();
    return cbox;
}
void ChoseCityDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"---------------------setEditorData----------------------------------";
    QVariant value = index.model()->data(index, Qt::EditRole);
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    cbox->setCurrentText(value.toString());

}
void ChoseCityDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"---------------------setModelData----------------------------------";
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    model->setData(index, cbox->currentText(), Qt::EditRole);
}
void ChoseCityDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"-----------------updateEditorGeometry--------------------------------------";
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    editor->setGeometry(option.rect);
}

DateDelegate::DateDelegate(QObject* parent):QItemDelegate (parent)
{}

QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit* date = new QDateEdit(parent);
    qDebug()<<"Create--------------------";
    date->setDisplayFormat("dd.MM.yyyy");
    //setEditorData(date,index);
    qDebug()<<"---------------------createEditor----------------------------------";
    return date;
}
void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"---------------------DateDelegate::ssetEditorData----------------------------------";
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QDateEdit* date = dynamic_cast<QDateEdit*>(editor);
    date->setDisplayFormat("dd.MM.yyyy");
    QStringList dateStr = value.toString().split(".", QString::SkipEmptyParts);
    if(dateStr.size() == 3) {
        date->setDate(QDate(dateStr[2].toInt(),dateStr[1].toInt(),dateStr[0].toInt()));
    }

}
void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"---------------------DateDelegate::setModelData----------------------------------";
    QDateEdit* date = static_cast<QDateEdit*>(editor);
    model->setData(index, date->date().toString("dd.MM.yyyy"), Qt::EditRole);

}
void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit* date = dynamic_cast<QDateEdit*>(editor);
    date->setDisplayFormat("dd.MM.yyyy");
    editor->setGeometry(option.rect);
}

