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
#include <QSqlRecord>
#include <algorithm>
#include <QDate>

#include "studentgroupdb.h"
#include "chosecitydelegate.h"

GroupsView::GroupsView(QSqlTableModel* model, QString Name, QWidget *parent, QAbstractItemView::EditTriggers editRole) : QWidget(parent)
{
    QStringList workStatus = {"Yes", "No"};
    ComboBoxDelegate* cityName = new ComboBoxDelegate(StudentGroupDB::getRegions(),this);
    ComboBoxDelegate* jobStatus = new ComboBoxDelegate(workStatus,this);
    SpinBoxDelegate* score = new SpinBoxDelegate(0, 100, this);
    ComboBoxDelegate* groupName = new ComboBoxDelegate(QStringList()<<Name<<"",this);
    m_groupName = new QPushButton(Name, this);
    m_groupTable = new QTableView(this);
    m_groupTable->setModel(model);
    m_groupTable->setEditTriggers(editRole);
    QDate minDate(1950,01,01);
    DateDelegate* birthDate = new DateDelegate(minDate, this);
    DateDelegate* date = new DateDelegate(minDate, this);
    m_groupTable->setItemDelegateForColumn(3, cityName);
    m_groupTable->setItemDelegateForColumn(4, birthDate);
    m_groupTable->setItemDelegateForColumn(5, date);
    m_groupTable->setItemDelegateForColumn(6, date);
    m_groupTable->setItemDelegateForColumn(7, groupName);
    m_groupTable->setItemDelegateForColumn(9, jobStatus);
    m_groupTable->setItemDelegateForColumn(8, score);
    setMinimumSize(200,200);
    m_groupTable->setAlternatingRowColors(true);
    m_groupTable->setStyleSheet("alternate-background-color: #DCDFE1");

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
    emit passCurrentView(this);
}

void GroupsView::handleSectionClick(int ind)
{
   QAbstractItemModel* modelBase = m_groupTable->model();
   QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(modelBase);
   if(!model) {
       return;
   }

   model->setSort(ind, m_sortOrder);
   model->select();
   m_groupTable->setModel(model);
}

void GroupsView::saveChanges()
{
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
    bool succes = model->submitAll();
    if(succes) {
        QMessageBox::information(this,"Save Changes","Your Changes successfully saved!",
                QDialogButtonBox::Ok);
    } else {
        QMessageBox::critical(this,"Save Changes","Somthing happen wrong. Please try again!");
        return;
    }

    m_groupTable->setModel(model);
}

void GroupsView::addStudent(bool t)
{
   QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_groupTable->model());
   if(!model)
       return;

   m_groupTable->setModel(model);
}

void GroupsView::removeStudent()
{
    QItemSelectionModel *selections = m_groupTable->selectionModel();
    QModelIndexList list = selections->selectedRows();
    if(list.empty()) {
        QMessageBox::information(this,"Select Student","Please select any student before \n press a button !",
                QDialogButtonBox::Ok);
        return;
    }
    int rowindex = list.begin()->row();
    int x = QMessageBox::information(this,"Delete Student","Are you sure delete a ",
             QMessageBox::Ok | QMessageBox::Cancel);

    QSqlTableModel* model =  dynamic_cast<QSqlTableModel*>(m_groupTable->model());
    if(!model) {
        return;
    }
    if(x == QMessageBox::Ok) {
       model->removeRow(rowindex);
       model->submitAll();
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

    int rows = m_groupTable->model()->rowCount();
    int columns = m_groupTable->model()->columnCount();

    for(int i=1;i<columns;++i){
        textData+=model->headerData(i,Qt::Horizontal).toString()+", ";
    }
    textData+="\n";
    for (int i = 0; i < rows; i++) {

        for (int j = 1; j < columns; j++) {

                textData += model->data(model->index(i,j)).toString();
                textData += ", ";
        }
        textData += "\n";
    }

    if(open) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();
    }
    return true;
}

void GroupsView::fillUp()
{
    int columnindex = 0;
    int rowIndex = 0;
    QVariant data;
    QAbstractItemModel* model = m_groupTable->model();
    getSelectedCell(data,rowIndex,columnindex);

    for(int i = rowIndex-1; i>=0; --i) {
        model->setData(model->index(i,columnindex),data);
    }
}

void GroupsView::fillDown()
{
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
