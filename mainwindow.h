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

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(QWidget* parent = nullptr);

    void createWidgets();
    void addWidgetToLayout();
    void showEvent(QShowEvent *event)
    {
        qDebug()<<"showEvent";
       //addWidgetToLayout();
        for(int i=0;i<m_widgets.size();++i){
            if(!m_widgets[i]->isVisible()) {
                qDebug()<<"inside if show event";
               // m_widgets[i]->resize(m_widgets[i-1]->size());
                m_widgets[i]->setVisible(true);
            }
        }

        addWidgetToLayout();

    }
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
    ~MainWindow();
    void createToolbar();
    void actionEvent(QActionEvent * act) override;


signals:
    void adminLogines(bool);
    void currentViewChanged(bool);

public slots:
    void adminAveliable(bool);
    void setCurrentView(GroupsView* view);
    void handleViewChange(bool);

private slots:
    void saveChanges(bool state);
    void addStudent(bool);
    void removeStudent();
    void revertChanges(bool);
    void writeAsCSV();
    void setAsceSort();
    void setDescSort();
    void adminSignIn(bool);
    void goHome();

private:
    QPushButton* m_admin;
    QPushButton* m_search;
    TableViewBox* m_viewBox;
    CentralWidget* m_centralWidget;
    GroupsView* m_currentView;
    ChoseCityDelegate* delegate;
    DateDelegate* m_dateDelegate;
    QToolBar* m_toolbar;
    QGridLayout* g_mainLayout ;
};

#endif // MAINWINDOW_H
