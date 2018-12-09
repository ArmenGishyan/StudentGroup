#include "headermenu.h"
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSpacerItem>
#include <QWidgetAction>
#include <QLineEdit>
#include <QObject>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QPainter>
#include <memory>

#include "searchdialogbox.h"
#include "admindbox.h"
#include "lineediticon.h"
#include "studentgroupdb.h"
#include "groupsview.h"

HeaderMenu::HeaderMenu(QWidget *parent) : QWidget(parent)
{

   QMenuBar* menubar = new QMenuBar(this);
  // QIcon   menuIcon("C://Users//agishyan//Desktop//studentGroupIcons//menu.png");
   QIcon   homeIcon("C://Users//agishyan//Desktop//studentGroupIcons//menuhome.png");
   QIcon   adminIcon("C://Users//agishyan//Desktop//studentGroupIcons//adminmenu.png");
   QIcon   searchIcon("C://Users//agishyan//Desktop//studentGroupIcons//search.png");
   QIcon   boyAdmin("C://Users//agishyan//Desktop//studentGroupIcons//adminman.png");
   QIcon   girlAdmin("C://Users//agishyan//Desktop//studentGroupIcons//admingirl.png");
   QIcon   noAdmin("C://Users//agishyan//Desktop//studentGroupIcons//unknown.png");
   QIcon   smallSearch("C://Users//agishyan//Desktop//studentGroupIcons//searchsmall.png");

   m_search = new LineEditIcon(smallSearch,this);
   m_search->setPlaceholderText("Name Surname");
   m_search->setFixedHeight(50);

   QPixmap bgPixmap("C://Users//agishyan//Desktop//studentGroupIcons//othermenu.png");
   QPixmap scaled = bgPixmap.scaled( QSize(150, 150), Qt::IgnoreAspectRatio);
   scaled.setDevicePixelRatio(200);
   QIcon menuIcon(scaled);
   QPainter* painter = new QPainter(this);

   menuIcon.paint(painter,0,0,150,150);

   m_menu = new QMenu("Menu",menubar);

   m_menu->setIcon(menuIcon);
   QAction* homeAction = new QAction(homeIcon,"Home");
   QAction* adminAction = new QAction(adminIcon,"Admin");
   QAction* searchAction = new QAction(searchIcon,"Search Students");
   QAction* currentAdmin = new QAction(noAdmin,"NO Admin");


   m_menu->addAction(homeAction);
   m_menu->addAction(adminAction);
   m_menu->addAction(searchAction);

   m_adminbox = new AdminDBox(this);
   m_searchBox = new SearchDialogBox();
   m_searchBox->setGroupBoxValue(m_db.getGroupNames());
   m_searchBox->hide();
   m_adminbox->hide();

   qDebug()<<"is admin to main "<<connect(m_adminbox, SIGNAL(adminSign(bool)), this, SLOT(adminAveliable(bool)));
   connect(adminAction, SIGNAL(triggered()), m_adminbox, SLOT(show()));
   connect(searchAction, SIGNAL(triggered()), m_searchBox, SLOT(show()));

   menubar->addAction(currentAdmin);
   menubar->addMenu(m_menu);
   QHBoxLayout*   menuSearch = new QHBoxLayout;
   menuSearch->setMargin(0);
   menuSearch->setSpacing(0);
   menuSearch->addWidget(m_search);
   m_searchbt = new QPushButton("Search");
   m_searchbt->setFixedHeight(50);
   connect(m_searchbt, SIGNAL(clicked(bool)), this, SLOT(findStudents()));
   menuSearch->addWidget(m_searchbt);
   menuSearch->addWidget(menubar);
   menubar->setStyleSheet("background-color : #32488d");
   setFixedHeight(50);
   setLayout(menuSearch);

}

bool HeaderMenu::validateSearchString(QStringList str)
{
    if(str.size() == 1 && str.size() == 2) {
        return true;
    } else {
        return false;
    }


}

void HeaderMenu::findStudents()
{
    QStringList searchstr = m_search->text().split(" ", QString::SkipEmptyParts);
    if(searchstr.empty()){
        return;
    };
    qDebug()<<"search string"<<searchstr;
    qDebug()<<"HeaderMenu::findStudents";

    QSqlTableModel* model = new QSqlTableModel(this,StudentGroupDB::getDatabase());
    model->setTable("Students");
    QString infoText = "";
    if(searchstr.size() == 1) {
        model->setFilter("Name = '"+searchstr[0] + "'");
        model->select();
        if(model->rowCount() == 0) {
            infoText = "Couldn't find Student \n " + searchstr[0];
            model->setFilter("Surname = '"+searchstr[0] + "'");
            model->select();
        }
    } else {
         infoText = "Couldn't find Student \n " + searchstr[0] + " " + searchstr[1];
         model->setFilter("Name = '"+searchstr[0] + "'");
         model->setFilter("Surname = '"+searchstr[1] + "'");
         model->select();
         if(model->rowCount() == 0) {
            model->setFilter("Name = '"+searchstr[1] + "'");
            model->setFilter("Surname = '"+searchstr[0] + "'");
            model->select();
         }
    }

    if(model->rowCount() == 0) {
        QMessageBox::information(this,"Nothing Found",infoText,
                QDialogButtonBox::Ok);
        return;

    }

    GroupsView* student = new GroupsView(model,"Student");
    emit findStudent(student);
    qDebug()<<"Find Student success"<<model->select();


}
