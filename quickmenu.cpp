#include "quickmenu.h"
#include <QIcon>
#include <QAction>

QuickMenu::QuickMenu(QWidget *parent) : QWidget(parent)
{
   QIcon menuIcon("C://Users//agishyan//Desktop//studentGroupIcons//menu.png");
   QAction* menuAction = new QAction(menuIcon,"Menu");
   setStyleSheet("background-color : red");
   setMinimumSize(QSize(2000,2000));
   addAction(menuAction);

}
