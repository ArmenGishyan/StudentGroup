#include "studentgroupdb.h"

SGDB::SGDB(QString databaseType, QString host, QString name, QString userName, QString password)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}
