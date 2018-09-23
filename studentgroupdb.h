#ifndef STUDENTGROUPDB_H
#define STUDENTGROUPDB_H
#include <QtSql/QSqlDatabase>

class SGDB
{
public:
    SGDB(QString databaseType, QString host, QString name, QString userName, QString password);
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("LOCALHOST");
    db.setDatabaseName("StudentGroup");
    db.setUserName("postgre");
    db.setPassword("Armen");
private:
    QSqlDatabase m_db;
};

#endif // STUDENTGROUPDB_H
