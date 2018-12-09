#include "groupsview.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QDebug>
#include <QObject>
#include <QHeaderView>
#include <QToolBar>
#include <QAbstractItemModel>
#include <QSqlError>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <algorithm>

GroupsView::GroupsView(QSqlTableModel* model, QString Name, QWidget *parent) : QWidget(parent)
{
    m_delegate = new ChoseCityDelegate(this);
    m_dateDelegate = new DateDelegate(this);
    m_groupName = new QPushButton(Name,this);
    m_groupTable = new QTableView(this);
    m_groupTable->setModel(model);
    m_groupTable->setItemDelegateForColumn(3,m_delegate);
    m_groupTable->setItemDelegateForColumn(5,m_dateDelegate);
    m_groupTable->setItemDelegateForColumn(6,m_dateDelegate);
    setMinimumSize(200,200);

    QVBoxLayout* it = new QVBoxLayout;
    it->addWidget(m_groupName);
    it->addWidget(m_groupTable);
    setLayout(it);
    QHeaderView* header = m_groupTable->horizontalHeader();
    QObject::connect(m_groupName, SIGNAL(clicked(bool)), this, SLOT(passActiveView()));
    QObject::connect(header, SIGNAL(sectionClicked(int)), this, SLOT(handleSectionClick(int)));
}

void GroupsView::setColumnHidden(int column, bool hide)
{
    m_groupTable->setColumnHidden(column, hide);
}

void GroupsView::passActiveView()
{
    qDebug()<<"handleGroupNameClick";
    emit passCurrentView(this);
}

void GroupsView::handleSectionClick(int ind)
{
   qDebug()<<"handleSectionClick"<<"ind " <<ind;
   QAbstractItemModel* modelBase = m_groupTable->model();
   QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(modelBase);
   if(!model) {
       qDebug()<<"model is nullptr";
       return;
   }
   qDebug()<<"inside";
   model->setSort(ind, m_sortOrder);
   model->select();
   m_groupTable->setModel(model);
}

void GroupsView::saveChanges(bool state)
{
    qDebug()<<"save changes -----------------";
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
    qDebug()<<"is save"<<model->submitAll();
    qDebug()<<"in save last error"<<model->lastError();
}

void GroupsView::addStudent(bool t)
{
   qDebug()<<"state = "<<t;
   qDebug()<<"addStudent";

   QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
   if(!model)
       return;
   qDebug()<<"row inserted"<<model->insertRow(model->rowCount());
  // QModelIndex nIndex = model->index(1,7);
  // qDebug()<<"flag = "<<nIndex.flags();
  // model->setData(nIndex, m_groupName->text());
   m_groupTable->setModel(model);
   m_groupTable->setColumnHidden(7, true);
}

void GroupsView::removeStudent()
{
    qDebug()<<"Remove Student";
    QItemSelectionModel *selections = m_groupTable->selectionModel();
    QModelIndexList list = selections->selectedRows();
    if(list.empty()) {
        QMessageBox::information(this,"Select Student","Please select any student before \n press a button !",
                QDialogButtonBox::Ok);
        return;
    }
    int rowindex = list.begin()->row();
    qDebug()<<"list size"<<list.size();
   //` QString name = m_groupTable->model()->data(*(list.begin()+=2)).toString();
   //` QString surname = m_groupTable->model()->data(*(list.begin()+=3)).toString();

     int x = QMessageBox::information(this,"Delete Student","Are you sure delete a ",
             QMessageBox::Ok | QMessageBox::Cancel);

     QSqlTableModel* model =  dynamic_cast<QSqlTableModel*>(m_groupTable->model());
     if(!model) {
         return;
     }
     if(x == QMessageBox::Ok) {
        model->removeRow(rowindex);
        model->submitAll();
     } else {
        //model->revertRow(rowindex);
     }
}
void GroupsView::revertChanges(bool state)
{
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
    if(!model){
        return;
    }


    model->revertAll();
    m_groupTable->setModel(model);
}

bool GroupsView::writeAsCSV()
{
    QString dir = QDir::homePath();
    QString name = m_groupName->text();

    QString openfile = QFileDialog::getSaveFileName(nullptr, tr("Save As CSV"), dir + "/" + name, tr("CSV (*.csv)"));
    QFile csvFile(openfile);
    bool open = csvFile.open(QIODevice::WriteOnly | QFile::Truncate);

    QString textData;
    QAbstractItemModel* model = m_groupTable->model();
    QVariant data = model->headerData(1,Qt::Horizontal);;
    qDebug()<<"header data "<<data.toString();
    int rows = m_groupTable->model()->rowCount();
    int columns = m_groupTable->model()->columnCount();

    for(int i=1;i<columns;++i){
        textData+=model->headerData(i,Qt::Horizontal).toString()+", ";
    }
    textData+="\n";
    for (int i = 0; i < rows; i++) {

        for (int j = 1; j < columns; j++) {

                textData += model->data(model->index(i,j)).toString();
                textData += ", ";      // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv
    if(open) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
    return true;
}

void GroupsView::adminSignIn(bool t)
{
    qDebug()<<"GroupsView::adminSignIn";
    m_isAdminAveliable = t;
   // QList<QAction*> actions = m_toolBar->actions();
    //std::for_each(actions.begin(),actions.end(),[t](QAction* it){it->setEnabled(t);});
}

