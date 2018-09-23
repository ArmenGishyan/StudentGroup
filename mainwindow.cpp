#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    /*QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(tr("<i> Choose a menu </i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::)/
    */
    QMenuBar *menuber = new QMenuBar(this);
    QMenu * file = menuber->addMenu("File");
    QAction *newfile = new QAction(tr("New"), this);
    newfile->setShortcut(QKeySequence::New);
    file->addAction(newfile);

}

MainWindow::~MainWindow()
{
}
