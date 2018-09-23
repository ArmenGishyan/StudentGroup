#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
protected:
#ifndef QT_NO_CONTEXTMENU
    //void contextMenuEvent(QContextMenuEvent *ev) override;
#endif // QT_NO_CONTEXTMENU

private:
    QLabel *infoLabel;
};

#endif // MAINWINDOW_H
