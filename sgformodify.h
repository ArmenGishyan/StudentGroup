#ifndef SGFORMODIFY_H
#define SGFORMODIFY_H

#include <QObject>
#include <QWidget>

class QTableView;
class QToolBar;
class GroupView;

class SGForModify
{
public:
    SGForModify();
	void setTableView(GroupView*);

private:
    GroupView* m_currentTable;
    QToolBar* m_toolbar;
};

#endif // SGFORMODIFY_H
