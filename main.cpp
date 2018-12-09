#include "mainwindow.h"
#include "searchdialogbox.h"
#include <iostream>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlError>
#include <QTableView>
#include <QLayout>
#include <QHBoxLayout>
#include <QSqlQueryModel>
#include <QVector>
#include <QDebug>
#include <QVariant>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QStringList>
#include <list>
#include <QGroupBox>
#include <QCheckBox>
#include <QTranslator>
#include <QLineEdit>
#include <QStringList>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QObject>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QAbstractButton>
#include "studentgroupdb.h"
#include <string>
#include <algorithm>
#include <QDateEdit>
#include <QListWidgetItem>
#include <QStringList>
#include <string>
#include <map>
#include <QDebug>
#include <QProxyStyle>
#include <QVariant>

//class MyProxyStyle : public QProxyStyle
//{
//  public:
//    int styleHint(StyleHint hint, const QStyleOption *option = 0,
//                  const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const override
//    {QStyle::CE_MenuBarItem
//        if (hint == QStyle::SH_UnderlineShortcut)
//            return 0;
//        return QProxyStyle::styleHint(hint, option, widget, returnData);
//    }
//};
//
//int main(int argc, char **argv)
//{
//    Q_INIT_RESOURCE(textedit);
//
//    QApplication a(argc, argv);
//    a.setStyle(new MyProxyStyle);
//    TextEdit mw;
//    mw.resize(700, 800);
//    mw.show();
//    //...
//}
//
enum Tag {a,b};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyleShee(new MyProxyStyle);
    MainWindow win;
    std::map<int,QString> str = {{1,"Armen"}, {2,"Aram"}};
    qDebug()<<"fv"<<str[1];
    QListWidget* listW = new QListWidget;
    QListWidgetItem* item1 = new QListWidgetItem(listW);
    QListWidgetItem* item2 = new QListWidgetItem(listW);
    QListWidgetItem* item3 = new QListWidgetItem(listW);

   // std::string str = "sdf";

            QStringList list1;
   // list1.append(QString::fromStdString(str));

    item2->setData(Qt::EditRole, "ds");
    qDebug()<<"list item "<<item2->data(Qt::EditRole).toString();
    QDateEdit* date = new QDateEdit;
    item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    item1->setCheckState(Qt::Checked);
    qDebug()<<"flags = "<<item1->flags();
    date->setDate(QDate::currentDate());
   // listW->show();
    win.show();
    unsigned int ads = 0;
    QVariant it;

    qDebug()<<"QVariant = "<<it.toInt();

    qDebug()<<"Tag "<<Tag::a;
    return a.exec();
}
