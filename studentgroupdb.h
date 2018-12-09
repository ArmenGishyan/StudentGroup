#ifndef STUDENTGROUPDB_H
#define STUDENTGROUPDB_H
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <memory>
#include "searchdialogbox.h"

class QTableView;
class StudentGroupDB
{
public:
    bool execQuery(const QString& queryStr);
    std::shared_ptr<QTableView> getTable(const QString& tableName) const;
    QStringList getAllTables() const;
    static QSet<QString> getGroupNames();
    QSqlTableModel* findStudent(SearchResult sReault);
    static QSqlDatabase getDatabase();

private:
    static QSqlDatabase m_db;
};

#endif // STUDENTGROUPDB_H
