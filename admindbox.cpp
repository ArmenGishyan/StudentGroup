#include "admindbox.h"
#include <QToolBar>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QSqlTableModel>
#include <QModelIndex>

#include "studentgroupdb.h"

Admin::Admin(QString name, QString surname, Admin::Gender gen, Admin::Accessibility access)
    :m_name(name), m_surname(surname),
     m_gender(gen), m_access(access)
{}

AdminDBox::AdminDBox(QWidget* parrent):QDialog (parrent), m_admin(nullptr)
{
    QGridLayout* gmainlayout = new QGridLayout;
    QHBoxLayout* hsearchSignIn = new QHBoxLayout;
    setWindowTitle("Admin");

    setMinimumSize(200,100);
    QPushButton* signIn = new QPushButton("Sign In", this);
    QPushButton* cencel = new QPushButton("Cancel", this);
    qDebug()<<"conected = "<<connect(signIn, &QPushButton::clicked, this, &AdminDBox::replaceAdmin);
    connect(cencel, SIGNAL(clicked()), this, SLOT(hide()));
    hsearchSignIn->addWidget(cencel);
    hsearchSignIn->addWidget(signIn);

    QFormLayout* nameform = new QFormLayout;
    m_login = new QLineEdit(this);
    m_login->setClearButtonEnabled(true);

    m_password = new QLineEdit(this);
    m_password->setClearButtonEnabled(true);

    nameform->addRow(QObject::tr("Login"), m_login);
    nameform->addRow(QObject::tr("Password"), m_password);

    m_password->setEchoMode(QLineEdit::Password);

    gmainlayout->addLayout(nameform,0,0,1,0);
    gmainlayout->addLayout(hsearchSignIn,1,0,1,1);
    setLayout(gmainlayout);
}

void AdminDBox::replaceAdmin()
{
    QString login = m_login->text();
    QString password = m_password->text();

    if(login.isEmpty() || password.isEmpty()) {
        return;
    }

    QSqlQuery query("Select * from Admins", StudentGroupDB::getDatabase());
    QSqlTableModel* model = new QSqlTableModel(this, StudentGroupDB::getDatabase());
    model->setTable("Admins");
    model->setFilter("Login = '" + login +"'");
    model->setFilter("_Password = '" + password +"'");
    model->select();

    QString name = model->record(0).value("Name").toString(); //.toString();
    QString surname = model->record(0).value("Surname").toString();
    Admin::Accessibility access = model->record(0).value("Accessibility").toBool() ?
                                         Admin::Accessibility::POWER : Admin::Accessibility::NORMAL;

    Admin::Gender gender = model->record(0).value("Accessibility").toBool() ?
                                         Admin::Gender::MALE : Admin::Gender::FEMALE;
    delete m_admin;
    m_admin = new Admin(name,surname,gender,access);
        emit adminReplaced(m_admin);

    qDebug()<<"is Avelable "<<name;
}
