#ifndef HEADERMENU_H
#define HEADERMENU_H

#include <QObject>
#include <QWidget>
#include "studentgroupdb.h"


class QLineEdit;
class QLabel;
class QMenu;
class SearchDialogBox;
class AdminDBox;
class LineEditIcon;
class QPushButton;
class GroupsView;

class HeaderMenu : public QWidget
{
    Q_OBJECT
public:
    explicit HeaderMenu(QWidget *parent = nullptr);
    bool validateSearchString(QStringList);

signals:
    void findStudent(GroupsView*);

public slots:
    void findStudents();
    SearchDialogBox* getSearchBox() const
    {
        return m_searchBox;
    }

private:
    LineEditIcon* m_search;
    QLabel* currentAdmin;
    QMenu* m_menu;
    AdminDBox* m_adminbox;
    SearchDialogBox* m_searchBox;
    QPushButton* m_searchbt;
    StudentGroupDB m_db;
};

#endif // HEADERMENU_H
