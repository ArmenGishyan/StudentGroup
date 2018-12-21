#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QTableView>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlRecord>
#include "studentgroupdb.h"

QSqlDatabase StudentGroupDB::m_db = QSqlDatabase();
QSqlDatabase StudentGroupDB::getDatabase()
{
    if(StudentGroupDB::m_db.isOpen()) {
        return StudentGroupDB::m_db;
    } else {
        StudentGroupDB::m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("C:\\Users\\agishyan\\Desktop\\SQL\\studentgroup.db");
        assert(StudentGroupDB::m_db.open() && "Can't open DB");
        return StudentGroupDB::m_db;
    }
}


std::shared_ptr<QTableView> StudentGroupDB::getTable(const QString& tableName) const
{
    QSqlTableModel *model = new QSqlTableModel(nullptr, getDatabase());
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    std::shared_ptr<QTableView> view(new QTableView());
    view.get()->setModel(model);
    return view;
}

QStringList StudentGroupDB::getAllTables() const
{
    return getDatabase().tables();
}

QSet<QString> StudentGroupDB::getGroupNames()
{
    QSqlQuery query(getDatabase());
    QSet<QString> studentsGroupName;
    assert(query.exec("select GroupName from Students"));
    qDebug()<<query.lastError();
    while (query.next()) {
           QString str = query.value(0).toString();
           if(!str.isEmpty()) {
                studentsGroupName.insert(str);
           }
    }
    return studentsGroupName;
}

QSqlTableModel* StudentGroupDB::findStudent(SearchResult sResult)
{
    QSqlTableModel *model = new QSqlTableModel(nullptr,getDatabase());
    QString query;
    if(!sResult.name.isEmpty()) {
        query.append("Name = '" + QString(sResult.name+"'"));
    }
    if(!sResult.surname.isEmpty()) {
        query.append(" and Surname = '"+sResult.surname+"'");
    }
    qDebug()<<query;
    qDebug()<<"Error"<<model->lastError();
    model->setTable("Students");
    model->setFilter(query);
    model->select();
    return model;
}

QStringList StudentGroupDB::getRegions()
{
    QStringList cities = {"Yerevan", "Ararat", "Armavir", "Aragatsotn",
                          "Gegharkunik", "Kotayk", "Lori", "Shirak", "Syunik",
                          "Tavush", "Vayots Dzor", "Artsakh"};

    return cities;
}
