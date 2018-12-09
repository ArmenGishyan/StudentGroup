#ifndef GROUPSVIEW_H
#define GROUPSVIEW_H

#include <QWidget>
#include <QPushButton>
#include "chosecitydelegate.h"

class QTableView;
class QSqlTableModel;
class QToolBar;
class GroupsView;


class GroupsView : public QWidget
{
    Q_OBJECT
public:
    explicit GroupsView(QSqlTableModel* model = nullptr, QString Name = "None", QWidget *parent = nullptr);
    void setColumnHidden(int column,bool hide);
    QString getGroupName() const
    {
        return m_groupName->text();
    }
    QTableView* getTable() const
    {
        return m_groupTable;
    }
    QAction* getGoHmoeAction() const
    {
        return m_home;
    }

signals:
    void onGroupNameClicked(QString);
    void addStudent();
    void passCurrentView(GroupsView*);

public slots:
    void passActiveView();
    void handleSectionClick(int);
    void saveChanges(bool state);
    void addStudent(bool);
    void removeStudent();
    void revertChanges(bool);
    bool writeAsCSV();
    void setAsceSort() {m_sortOrder = Qt::AscendingOrder;}
    void setDescSort() {m_sortOrder = Qt::DescendingOrder;}
    void adminSignIn(bool);

private:
    QTableView* m_groupTable;
    QPushButton* m_groupName;
    QAction* m_home;
    Qt::SortOrder m_sortOrder;
    ChoseCityDelegate* m_delegate;
    DateDelegate* m_dateDelegate;
    bool m_isAdminAveliable;
};

#endif // GROUPSVIEW_H
