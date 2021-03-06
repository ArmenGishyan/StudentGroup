#ifndef HEADERMENU_H
#define HEADERMENU_H

#include "studentgroupdb.h"
#include <QEvent>
#include <QHideEvent>

class QLabel;
class QMenu;
class SearchDialogBox;
class AdminDBox;
class LineEditIcon;
class QPushButton;
class GroupsView;
class Admin;
class QToolButton;

class HeaderMenu : public QWidget
{
    Q_OBJECT
public:
    explicit HeaderMenu(QWidget *parent = nullptr);
    ~HeaderMenu();
    bool validateSearchString(QStringList);
    void adminOption();
    void showEvent(QShowEvent* ev)
    {
        qDebug()<<"show event";
        qDebug()<<"spontaneous "<<ev->spontaneous();
        QWidget::showEvent(ev);
    }

    void hideEvent(QHideEvent* ev)
    {
        qDebug()<<"hide event";
        qDebug()<<"spontaneous "<<ev->spontaneous();
        QWidget::hideEvent(ev);

    }

    void closeEvent(QCloseEvent* ev)
    {
        qDebug()<<"close event";
        QWidget::closeEvent(ev);
    }

signals:
    void findStudent(GroupsView*);

public slots:
    void findStudents();
    SearchDialogBox* getSearchBox() const
    {
        return m_searchBox;
    }

    AdminDBox* getAdminBox()
    {
        return m_adminbox;
    }

    QAction* getHomeAction()
    {
       return homeAction;
    }


    void adminChanged(Admin*);

private:
    LineEditIcon* m_search;
    QToolButton* m_adminOptions;
    QLabel* currentAdmin;
    QMenu* m_menu;
    QAction* homeAction;
    AdminDBox* m_adminbox;
    SearchDialogBox* m_searchBox;
    QPushButton* m_searchbt;
    StudentGroupDB m_db;
};

#endif // HEADERMENU_H
