#include <QIcon>
#include <QAction>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QPainter>
#include <QToolButton>
#include <QMenu>
#include <QToolBar>

#include "headermenu.h"
#include "searchdialogbox.h"
#include "admindbox.h"
#include "lineediticon.h"
#include "studentgroupdb.h"
#include "groupsview.h"

HeaderMenu::HeaderMenu(QWidget *parent) : QWidget(parent),m_adminOptions(nullptr)
{
   QToolBar* toolbar = new QToolBar(this);
   QIcon      menuIcon(":/Icons/menu.png");
   QIcon      homeIcon(":/Icons/menuhome.png");
   QIcon     adminIcon(":/Icons/adminmenu.png");
   QIcon    searchIcon(":/Icons/search.png");
   QIcon   smallSearch(":/Icons/searchsmall.png");

   m_search = new LineEditIcon(smallSearch,this);
   m_search->setPlaceholderText("Name Surname");
   m_search->setFixedHeight(40);

   QPixmap bgPixmap("C://Users//agishyan//Desktop//studentGroupIcons//othermenu.png");
   QPixmap scaled = bgPixmap.scaled( QSize(150, 150), Qt::IgnoreAspectRatio);
   scaled.setDevicePixelRatio(200);
   QPainter* painter = new QPainter(this);

   menuIcon.paint(painter,0,0,150,150);

   Admin admin;
   emit adminChanged(&admin);
   m_menu = new QMenu("Menu", toolbar);
   QToolButton* toolButton = new QToolButton();
   toolButton->setIcon(menuIcon);
   toolButton->setMenu(m_menu);
   toolButton->setPopupMode(QToolButton::InstantPopup);
   toolbar->addWidget(toolButton);
   toolbar->addWidget(m_adminOptions);

   m_menu->setIcon(menuIcon);
   homeAction = new QAction(homeIcon,"Home");
   QAction* adminAction = new QAction(adminIcon,"Admin");
   QAction* searchAction = new QAction(searchIcon,"Search Students");

   m_menu->addAction(homeAction);
   m_menu->addAction(adminAction);
   m_menu->addAction(searchAction);

   m_adminbox = new AdminDBox(this);
   connect(m_adminbox, SIGNAL(adminReplaced(Admin*)), this,  SLOT(adminChanged(Admin*)));
   m_searchBox = new SearchDialogBox();
   m_searchBox->setGroupBoxValue(m_db.getGroupNames());
   m_searchBox->hide();
   m_adminbox->hide();

   connect(adminAction, SIGNAL(triggered()), m_adminbox, SLOT(show()));
   connect(searchAction, SIGNAL(triggered()), m_searchBox, SLOT(show()));

   toolbar->setFixedWidth(100);
   QHBoxLayout*   menuSearch = new QHBoxLayout;
   menuSearch->setMargin(0);
   menuSearch->setSpacing(0);
   menuSearch->addWidget(m_search);
   m_searchbt = new QPushButton("Search");
   m_searchbt->setShortcut(QKeySequence(Qt::Key_Enter));
   m_searchbt->setFixedHeight(40);
   connect(m_searchbt, SIGNAL(clicked(bool)), this, SLOT(findStudents()));
   menuSearch->addWidget(m_searchbt);
   menuSearch->addWidget(toolbar);
   toolbar->setStyleSheet("background-color : #32488d");
   setFixedHeight(40);
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
}
void HeaderMenu::adminChanged(Admin* admin)
{
    QMenu* m_adminMenu = new QMenu(this);

    QToolButton* adminbt = new QToolButton();
    adminbt->setMenu(m_adminMenu);
    adminbt->setPopupMode(QToolButton::InstantPopup);

    if(m_adminOptions == nullptr) {
        m_adminOptions = new QToolButton(this);
    }

    if(admin == nullptr) {
        return;
     }

    m_adminMenu->addAction(new QAction("Name : " + admin->getName()));
    m_adminMenu->addAction(new QAction("Surname : " + admin->getSurname()));
    m_adminMenu->addAction(new QAction("Accessibility : " + QString::number(static_cast<int>(admin->getAccess()))));

    m_adminOptions->setIcon(admin->getPicture());
    m_adminOptions->setMenu(m_adminMenu);
    m_adminOptions->setPopupMode(QToolButton::InstantPopup);
}

HeaderMenu::~HeaderMenu()
{
    delete m_searchBox;
    delete m_adminbox;
}
