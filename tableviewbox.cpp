#include "tableviewbox.h"
#include <QDebug>

TableViewBox::TableViewBox(QWidget *parent) : QWidget(parent)
{
    m_tableView = new QTableView(this);
    qDebug()<<"TableView";
  //  m_tableView->show();
  //  m_tableView->setStyleSheet("background-color : yellow");
  //  m_tableView->show();
}
//TableViewBox::TableViewBox(QSqlDatabase db, QWidget *parent) : QWidget(parent)
//{
//
//}
