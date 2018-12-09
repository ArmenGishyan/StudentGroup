#include "tablemodel.h"

TableModel::TableModel(QObject* parrent, QSqlDatabase db):QSqlTableModel (parrent,db)
{

}

void TableModel::execQuery(const QString& str)
{

}
