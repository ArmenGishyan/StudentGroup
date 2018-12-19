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
#include <QVariant>
#include <QScrollBar>
#include <algorithm>
#include <QDate>

GroupsView::GroupsView(QSqlTableModel* model, QString Name, QWidget *parent, QAbstractItemView::EditTriggers editRole) : QWidget(parent)
{
    QStringList cities = {"Yerevan", "Ararat", "Armavir", "Aragatsotn", "Gegharkunik", "Kotayk", "Lori", "Shirak", "Syunik", "Tavush", "Vayots Dzor", "Artsakh"};
    QStringList workStatus = {"Yes", "No"};
    //ComboBoxDelegate* cityName = new ComboBoxDelegate(cities,this);
    //ComboBoxDelegate* jobStatus = new ComboBoxDelegate(workStatus,this);
    //SpinBoxDelegate* score = new SpinBoxDelegate(0, 100, this);
    //ComboBoxDelegate* groupName = new ComboBoxDelegate(QStringList()<<Name<<"",this);
    m_groupName = new QPushButton(Name, this);
    m_groupTable = new QTableView(this);
    m_groupTable->setModel(model);
    m_groupTable->setEditTriggers(editRole);
    QDate minDate(1950,01,01);
    DateDelegate* birthDate = new DateDelegate(this);
    DateDelegate* date = new DateDelegate(this);
    //StyleDelegate* style  = new StyleDelegate(Qt::red,this);
   // m_groupTable->setItemDelegateForColumn(2, style);
    //m_groupTable->setItemDelegateForColumn(3, cityName);
    m_groupTable->setItemDelegateForColumn(4, birthDate);
   // m_groupTable->setItemDelegateForColumn(5, date);
   // m_groupTable->setItemDelegateForColumn(6, date);
  //  m_groupTable->setItemDelegateForColumn(7, groupName);
  //  m_groupTable->setItemDelegateForColumn(9, jobStatus);
  //  m_groupTable->setItemDelegateForColumn(8, score);
   // m_groupTable->horizontalHeader()->setStyleSheet("background-color : #D7DBDD");
   // m_groupTable->verticalHeader()->setStyleSheet("background-color : #D7DBDD");
    setMinimumSize(200,200);
    m_groupTable->setAlternatingRowColors(true);
    m_groupTable->setStyleSheet("alternate-background-color: gray");

    QVBoxLayout* it = new QVBoxLayout;
    it->addWidget(m_groupName);
    it->addWidget(m_groupTable);
    setLayout(it);
    m_groupTable->horizontalScrollBar()->setHidden(true);
    m_groupTable->showGrid();
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

void GroupsView::saveChanges()
{
    qDebug()<<"save changes -----------------";
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
    qDebug()<<"is save"<<model->submitAll();
    m_groupTable->setModel(model);
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
   //m_groupTable
   //m_groupTable->setColumnHidden(7, true);
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

void GroupsView::fillUp()
{
    qDebug()<<"GroupsView::fillUp";
    int columnindex = 0;
    int rowIndex = 0;
    QVariant data;
    QAbstractItemModel* model = m_groupTable->model();
    getSelectedCell(data,rowIndex,columnindex);

    for(int i = rowIndex-1; i>=0; --i) {
        qDebug()<<"inside func";
        model->setData(model->index(i,columnindex),data);
    }
}

void GroupsView::fillDown()
{
    qDebug()<<"GroupsView::fillDown";
    int columnindex = 0;
    int rowIndex = 0;
    QVariant data;
    QAbstractItemModel* model = m_groupTable->model();
    getSelectedCell(data,rowIndex,columnindex);
    const int rowCount =  model->rowCount();
    for(int i = rowIndex+1; i<rowCount; ++i) {
        model->setData(model->index(i,columnindex),data);
    }
}

void GroupsView::setEditRole(QAbstractItemView::EditTriggers editRole)
{
   m_groupTable->setEditTriggers(editRole);
}

void GroupsView::getSelectedCell(QVariant& value, int& index, int& columnIndex)
{
    QItemSelectionModel *selections = m_groupTable->selectionModel();
    QModelIndexList list = selections->selectedIndexes();
    if(list.isEmpty()) {
        QMessageBox::information(this,"Fill Column","Please select any cell \n before click on the button",
             QMessageBox::Ok);
        return;
    }
    columnIndex = list.begin()->column();
    index = list.begin()->row();

    QAbstractItemModel* model = m_groupTable->model();
    value = model->index(index, columnIndex).data();
}
