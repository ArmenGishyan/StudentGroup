#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QString>

class TableModel : QSqlTableModel
{
public:
    TableModel(QObject* parret = nullptr, QSqlDatabase db = QSqlDatabase());
    void execQuery(const QString& str);
};

#endif // TABLEMODEL_H
