#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSqlTableModel>
#include <QToolBar>
#include <QMenuBar>
#include <QVector>
#include <QEvent>

#include "admindbox.h"
#include "tableviewbox.h"
#include "groupsview.h"
#include "chosecitydelegate.h"


class StudentGroupDB;
class QPushButton;
class SearchDialogBox;
class QAction;
class QGridLayout;
class Admin;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(QWidget* parent = nullptr);
    void createWidgets();
    void addWidgetToLayout();
    bool contains(const GroupsView* view) const;
    void showEvent(QShowEvent *event);

signals:
    void passView(GroupsView*);

private slots:
    void throwCurrentView(GroupsView* view);

private:
    QVector<GroupsView*> m_widgets;
    QGridLayout* m_grLay;
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void createToolbar();
    void hideUnhideTools(bool);

signals:
    void adminLogines(bool);
    void currentViewChanged(bool);

public slots:
    void adminAveliable(bool);
    void setCurrentView(GroupsView* view);
    void handleViewChange(bool);

private slots:
    void saveChanges();
    void addStudent(bool);
    void removeStudent();
    void revertChanges(bool);
    void writeAsCSV();
    void setAsceSort();
    void setDescSort();
    void adminSignIn(Admin*);
    void goHome();
    void fillUp();
    void fillDown();

private:
    QPushButton* m_search;
    TableViewBox* m_viewBox;
    CentralWidget* m_centralWidget;
    GroupsView* m_currentView;
    QToolBar* m_toolbar;
    QGridLayout* g_mainLayout ;
    Admin* m_admin;
};

#endif // MAINWINDOW_H
