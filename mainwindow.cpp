#include "mainwindow.h"
#include "searchdialogbox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>
#include <QDebug>
#include <QGridLayout>
#include <QIcon>
#include <QAction>
#include "studentgroupdb.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QEvent>
#include <QCheckBox>
#include <QTableWidget>
#include <QSpacerItem>

#include "headermenu.h"
#include "quickmenu.h"
CentralWidget::CentralWidget(QWidget* parent): QWidget (parent)
{
    m_grLay = new QGridLayout;
    createWidgets();
    addWidgetToLayout();
    setLayout(m_grLay);
}

void CentralWidget::createWidgets()
{
    QSet<QString> groups = StudentGroupDB::getGroupNames();
    auto it = groups.begin();
    qDebug()<<"createGroupBoxes";
    while(it != groups.end()) {
        QSqlTableModel* tablemodel = new QSqlTableModel(this,StudentGroupDB::getDatabase());
        tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        tablemodel->setTable("Students");
        tablemodel->setFilter("GroupName = '"+*it+"'");
        tablemodel->select();
        GroupsView* item = new GroupsView(tablemodel,*it,this);

        bool success =  connect(item, &GroupsView::passCurrentView, this, &CentralWidget::throwCurrentView);
        qDebug()<<"item "<<success;
        m_widgets.append(item);
        ++it;
    }

}

bool CentralWidget::contains(const GroupsView* item) const
{
    for(int i = 0;i<m_widgets.size(); ++i) {
        if(item == m_widgets[i]){
            return true;
        }
    }
    return false;
}

void CentralWidget::addWidgetToLayout()
{
    int row = 0;
    for(int i =0;i<m_widgets.size();++i)
    {
        if(i % 3 == 0) {
            row++;
        }
       // m_widgets[i]->setStyleSheet("background-color: #426cda");
        m_widgets[i]->setColumnHidden(0,true);
        m_grLay->addWidget(m_widgets[i],row,i % 3);
    }
}

void CentralWidget::throwCurrentView(GroupsView* view)
{
    qDebug()<<"throw Current View";
    emit passView(view);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), m_currentView(nullptr), m_admin(nullptr)
{
    HeaderMenu* menu = new HeaderMenu(this);
    connect(menu->getSearchBox(), SIGNAL(throwView(GroupsView*)), this, SLOT(setCurrentView(GroupsView*)));
    connect(menu->getAdminBox(), SIGNAL(adminReplaced(Admin*)), this, SLOT(adminSignIn(Admin*)));
    m_currentView = new GroupsView;
    m_centralWidget = new CentralWidget(this);
    connect(m_centralWidget, SIGNAL(passView(GroupsView*)), this, SLOT(setCurrentView(GroupsView*)));
    qDebug()<<"vndfjvk---------vdvnd"<<connect(menu, &HeaderMenu::findStudent,this, &MainWindow::setCurrentView);

    //ComboBoxDelegate* delegate = new ComboBoxDelegate({},this);
    QPushButton* search = new QPushButton("Search");
    QPushButton* admin = new QPushButton("Admin");
    setMinimumWidth(1200);
    setMinimumHeight(800);

    connect(this, SIGNAL(currentViewChanged(bool)), this, SLOT(handleViewChange(bool)));
    g_mainLayout = new QGridLayout;
    g_mainLayout->addWidget(menu,0,15,1,15);
    setLayout(g_mainLayout);
    createToolbar();
    adminSignIn(nullptr);
    g_mainLayout->addWidget(m_toolbar,0,0,1,15);
    g_mainLayout->addWidget(m_centralWidget,1,0,20,30);
    g_mainLayout->addWidget(m_currentView,1,0,20,30);
    handleViewChange(false);
}

MainWindow::~MainWindow(){}

void MainWindow::adminAveliable(bool t)
{
   qDebug()<<"MainWindow::adminAveliable";
   emit adminLogines(t);
}

void MainWindow::createToolbar()
{
   m_toolbar = new QToolBar(this);
   m_toolbar->setFixedHeight(40);

   QPixmap addStudent("C://Users//agishyan//Desktop//studentGroupIcons//addstudent.png");
   QIcon addIcon("C://Users//agishyan//Desktop//studentGroupIcons//addstudent.png");
   QAction* m_addStud = new QAction(addIcon,"Add Student");
   bool success = connect(m_addStud, SIGNAL(triggered(bool)), this, SLOT(addStudent(bool)));
   qDebug()<<"add Student"<<success;
   m_toolbar->addAction(m_addStud);

   QPixmap pixremove("C://Users//agishyan//Desktop//studentGroupIcons//removestudent.png");
   QIcon removeStudent(pixremove);
   QAction* m_removeStud = new QAction(removeStudent,"Remove Student");
   m_toolbar->addAction(m_removeStud);
   connect(m_removeStud, SIGNAL(triggered()), this , SLOT(removeStudent()));

   QPixmap pixDescSort("C://Users//agishyan//Desktop//studentGroupIcons//asce_sort.png");
   QIcon   descIcon(pixDescSort);
   QAction* m_descSort = new QAction(descIcon,"sort descending order");
   connect(m_descSort, SIGNAL(triggered()), this, SLOT(setDescSort()));
   m_toolbar->addAction(m_descSort);
   m_descSort->setCheckable(true);

   QPixmap pixAsceSort("C://Users//agishyan//Desktop//studentGroupIcons//desc_sort.png");
   QIcon   asceIcon(pixAsceSort);
   QAction* m_asceSort = new QAction(asceIcon,"sort ascending order");
   connect(m_asceSort, SIGNAL(triggered()), this, SLOT(setAsceSort()));
   m_toolbar->addAction(m_asceSort);
   m_asceSort->setCheckable(true);

   QActionGroup* sortActions = new QActionGroup(this);
   sortActions->addAction(m_asceSort);
   sortActions->addAction(m_descSort);


   QPixmap pixHome("C://Users//agishyan//Desktop//studentGroupIcons//Home.png");
   QIcon   homeIcon(pixHome);
   QAction* home = new QAction(homeIcon,"Home");
   m_toolbar->addAction(home);

   qDebug()<<"-----------Home"<<connect(home, SIGNAL(triggered()), this, SLOT(goHome()));
  // QPixmap change("C://Users//agishyan//Desktop//studentGroupIcons//workshop-icon.png");
  // QIcon   changeIcon(change);
  // QAction* m_change = new QAction(changeIcon,"sort descending order");
  // m_toolbar->addAction(m_change);

   QPixmap SaveChanges("C://Users//agishyan//Desktop//studentGroupIcons//save.png");
   QIcon   saveCh(SaveChanges);
   QAction* saveChanges = new QAction(saveCh,"Save Changes");
   qDebug()<<"save changes result"<<connect(saveChanges,SIGNAL(triggered()), this, SLOT(saveChanges()));
   m_toolbar->addAction(saveChanges);

   QPixmap revertChanges("C://Users//agishyan//Desktop//studentGroupIcons//revert.png");
   QIcon   revertCh(revertChanges);
   QAction* revert = new QAction(revertCh,"Revert all Unsaved Changes");
   m_toolbar->addAction(revert);

   QPixmap csvIcon("C://Users//agishyan//Desktop//studentGroupIcons//csvicon.png");
   QIcon   csv(csvIcon);
   QAction* csvAction = new QAction(csv,"Extract as CSV");
   m_toolbar->addAction(csvAction);
   connect(csvAction, SIGNAL(triggered()), this, SLOT(writeAsCSV()));


   QPixmap fillup("C://Users//agishyan//Desktop//studentGroupIcons//fillup.png");
   QIcon   fillupIcon(fillup);
   QAction* fillupAction = new QAction(fillupIcon,"Fill Up Column");
   m_toolbar->addAction(fillupAction);
   connect(fillupAction, SIGNAL(triggered()), this, SLOT(fillUp()));

   QPixmap filldown("C://Users//agishyan//Desktop//studentGroupIcons//filldown.png");
   QIcon   filldownIcon(filldown);
   QAction* filldownAction = new QAction(filldownIcon,"Fill Up Column");
   m_toolbar->addAction(filldownAction);
   connect(filldownAction, SIGNAL(triggered()), this, SLOT(fillDown()));

   QPixmap emailIcon("C://Users//agishyan//Desktop//studentGroupIcons/emailIcon.png");
   QIcon   eIcon(emailIcon);
   QAction* emailAction = new QAction(emailIcon,"Write Email");
   m_toolbar->addAction(emailAction);
   //connect(emailAction, SIGNAL(triggered()), this, SLOT(fillDown()));

   qDebug()<<"revert changes result"<<connect(revert,SIGNAL(triggered(bool)), this, SLOT(revertChanges(bool)));
   m_toolbar->setStyleSheet("background-color : #32488d");
   QList<QAction*> act = m_toolbar->actions();
   qDebug()<<"action size is "<<act.size();
}

void MainWindow::setCurrentView(GroupsView* view)
{
    qDebug()<<"current view";
    if(view) {
        if(!m_centralWidget->contains(m_currentView)) {
            delete m_currentView;
            m_currentView = view;
        } else {
            m_currentView = view;
        }
        emit currentViewChanged(true);
    } else {
        emit currentViewChanged(false);
    }
}

void MainWindow::saveChanges()
{
    if(m_currentView) {
        m_currentView->saveChanges();
    }
}
void MainWindow::addStudent(bool t)
{
    if(m_currentView) {
        m_currentView->addStudent(t);
    }
}

void MainWindow::removeStudent()
{
    if(m_currentView) {
        m_currentView->removeStudent();
    }
}
void MainWindow::revertChanges(bool t)
{
    if(m_currentView) {
        m_currentView->revertChanges(t);
    }
}
void MainWindow::writeAsCSV()
{
    QAction* action = dynamic_cast<QAction*>(QObject::sender());
    if(!action) {
       return;
    }
    action->setEnabled(false);
    if(m_currentView) {
        if(m_currentView->writeAsCSV()) {
            action->setEnabled(true);
        }
    }
}
void MainWindow::setAsceSort()
{
    if(m_currentView) {
        m_currentView->setAsceSort();
    }
}
void MainWindow::setDescSort()
{
    if(m_currentView) {
        m_currentView->setDescSort();
    }
}
void MainWindow::adminSignIn(Admin* admin)
{
    qDebug()<<"adminSignIn";
    delete m_admin;
    m_admin = admin;
    if(m_admin != nullptr) {
        //m_toolbar->setEnabled(true);
        hideUnhideTools(true);
        if(m_currentView != nullptr) {
            m_currentView->setEditRole(
                        (QAbstractItemView::EditTrigger::DoubleClicked | QAbstractItemView::EditTrigger::EditKeyPressed));
        }
    } else {
        hideUnhideTools(false);
        //m_toolbar->setDisabled(false);
    }
}

void MainWindow::goHome()
{
    qDebug()<<"goHome";
   handleViewChange(false);
}
void MainWindow::handleViewChange(bool t)
{
        qDebug()<<"inside else"<< t;
    if(t) {

        qDebug()<<"inside if";
        //m_toolbar->show();
        m_currentView->show();
        if(m_admin != nullptr) {
            m_currentView->setEditRole(
                        (QAbstractItemView::EditTrigger::DoubleClicked | QAbstractItemView::EditTrigger::EditKeyPressed));
            //m_toolbar->setEnabled(true);
            hideUnhideTools(true);
        } else {
            hideUnhideTools(false);
             //m_toolbar->setEnabled(false);
        }
        g_mainLayout->addWidget(m_currentView,1,0,20,30);
        m_centralWidget->hide();
    } else {
        qDebug()<<"inside else";
        if(m_currentView) {
            qDebug()<<" != nullptr";
           m_currentView->hide();

        }
        hideUnhideTools(false);
        //m_toolbar->hide();
        g_mainLayout->addWidget(m_centralWidget,1,0,20,30);
        m_centralWidget->show();
    }
}

void MainWindow::fillDown()
{
    qDebug()<<"MainWindow::fillDown";
    if(m_currentView) {
        m_currentView->fillDown();
    }
}

void MainWindow::fillUp()
{
    qDebug()<<"MainWindow::fillup";
    if(m_currentView) {
        m_currentView->fillUp();
    }
}

void MainWindow::hideUnhideTools(bool t)
{
    if(m_toolbar) {
        QList<QAction*> list = m_toolbar->actions();
        for(int i = 0; i<list.size(); ++i) {
            list[i]->setVisible(t);
        }
    }
}
