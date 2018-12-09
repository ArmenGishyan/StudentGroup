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
    date->setDisplayFormat("dd.MM.yyyy");
    qDebug()<<"---------------------createEditor----------------------------------";
    return date;
}
void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"---------------------DateDelegate::ssetEditorData----------------------------------";
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QDateEdit* date = dynamic_cast<QDateEdit*>(editor);
    date->setDisplayFormat("dd.MM.yyyy");
    QStringList dateStr = value.toDate().toString().split(".", QString::SkipEmptyParts);
    qDebug()<<"value String"<<dateStr;
    date->setDate(value.toDate());
    //date->setDate(QDate(dateStr[2].toInt(),dateStr[1].toInt(),dateStr[0].toInt()));
    date->setDisplayFormat("dd.MM.yyyy");

}
void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"---------------------DateDelegate::setModelData----------------------------------";
    QDateEdit* date = dynamic_cast<QDateEdit*>(editor);
    qDebug()<<"setModel"<<date->date().toString();
    model->setData(index, date->date().toString(), Qt::EditRole);
}
void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"-----------------updateEditorGeometry--------------------------------------";
    QDateEdit* date = dynamic_cast<QDateEdit*>(editor);
    editor->setGeometry(option.rect);
}

