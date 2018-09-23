#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("LOCALHOST");
    db.setDatabaseName("StudentGroup");
    db.setUserName("postgre");
    db.setPassword("Armen");


    if(db.open())
    {

        std::cout<<"success"<<std::endl;
    } else {
        std::cout<<"Faild"<<std::endl;
    }
    QSqlQuery query(db);
  /*  bool isSuccess = query.exec("create table Student(StudentId int, LastName varchar(20), "
                                "FirstName varchar(20), GroupName varchar(20), AvarageScore int);");
    */
    bool isSuccess = query.exec("select * from Student");
    qDebug()<<isSuccess;
    while (query.next()) {
        QString name = query.value(2).toString();
        qDebug()<<name;
    }
    /*if(query.exec("select * from student")) {
        qDebug()<<"size"<<query.size();
        for (int i = 0;i < query.size();i++)
        {
            qDebug() <<"name = "<< query.value(i).toInt();
        }
        //int name = query.value(0).toInt();
        //qDebug() <<"name = "<< name;
        std::cout<<"query worked"<<std::endl;
    }*/
    qDebug()<<query.lastError();
    QStringList tables = db.tables();
    qDebug()<<tables.size();

    for(const auto &item : *tables.begin())
    {
        qDebug()<<"name = "<<item;
    }


    MainWindow w;
    w.show();



    return a.exec();
}