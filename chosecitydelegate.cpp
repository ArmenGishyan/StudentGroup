#include "chosecitydelegate.h"
#include <QComboBox>
#include <QVariant>
#include <QDebug>
#include <QDateEdit>
#include <QDate>
#include <QSpinBox>

ComboBoxDelegate::ComboBoxDelegate(QStringList items, QObject* parent):QItemDelegate (parent), m_itemsList(items)
{
    qDebug()<<"----------------------------------------constructor--------------";
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox* cbox = new QComboBox(parent);
    cbox->insertItems(0,m_itemsList);
    qDebug()<<"---------------------createEditor----------------------------------";
    setEditorData(cbox, index);
    cbox->show();
    return cbox;
}
void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"---------------------setEditorData----------------------------------";
    QVariant value = index.model()->data(index, Qt::EditRole);
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    cbox->setCurrentText(value.toString());

}
void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"---------------------setModelData----------------------------------";
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    model->setData(index, cbox->currentText(), Qt::EditRole);
}
void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"-----------------updateEditorGeometry--------------------------------------";
    QComboBox* cbox = dynamic_cast<QComboBox*>(editor);
    editor->setGeometry(option.rect);
}
DateDelegate::DateDelegate(QDate min, QObject* parent, QDate max):QItemDelegate (parent)
  ,min(min), max(max)
{
    qDebug()<<"data";
}

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


SpinBoxDelegate::SpinBoxDelegate(int low, int high, QObject* parent):QItemDelegate
(parent),
                                                                     m_lowScore(low),
                                                                     m_highScore(high)
{}

QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox* spinBox = new QSpinBox(parent);
    spinBox->setRange(m_lowScore,m_highScore);

    return spinBox;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"---------------------DateDelegate::ssetEditorData----------------------------------";
    //QVariant value
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QSpinBox* spinBox = dynamic_cast<QSpinBox*>(editor);
    spinBox->setValue(value.toInt());
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinBox = dynamic_cast<QSpinBox*>(editor);
    qDebug()<<"---------------------DateDelegate::setModelData----------------------------------";
    model->setData(index, spinBox->value(), Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"-----------------updateEditorGeometry--------------------------------------";
    QSpinBox* spinBox = dynamic_cast<QSpinBox*>(editor);
    editor->setGeometry(option.rect);
}
