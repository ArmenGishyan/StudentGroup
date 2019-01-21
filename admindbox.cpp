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
#include <QRadioButton>

#include "studentgroupdb.h"

Admin::Admin(QString name, QString surname, Admin::Gender gen, Admin::Accessibility access, QPixmap pic)
    :m_name(name), m_surname(surname),
     m_gender(gen),m_access(access), m_picture(pic)
{}

AdminDBox::AdminDBox(QWidget* parrent):QDialog (parrent)
{
    QGridLayout* gmainlayout = new QGridLayout;
    QHBoxLayout* hsearchSignIn = new QHBoxLayout;
    setWindowTitle("Admin");

    setMinimumSize(200,100);
    QPushButton* signIn = new QPushButton("Sign In", this);
    QPushButton* cencel = new QPushButton("Cancel", this);
    connect(signIn, &QPushButton::clicked, this, &AdminDBox::replaceAdmin);
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
    model->select();
    if(model->rowCount() == 0) {
        QMessageBox::critical(this,"Wrong Login","Written wrong Login. Can't find anyone",
             QMessageBox::Ok);
        return;
    }
    model->setFilter("_Password = '" + password +"'");
    model->select();
    if(model->rowCount() == 0) {
        QMessageBox::critical(this,"Wrong Login",QString("Written wrong Password for ") + login,
             QMessageBox::Ok);
        return;
    }
    if(model->lastError().isValid()) {
        return;
    }
    QSqlRecord record = model->record(0);
    QString name = record.value("Name").toString();
    QString surname = record.value("Surname").toString();
    Admin::Accessibility access = record.value("Accessibility").toBool() ?
                                         Admin::Accessibility::POWER : Admin::Accessibility::NORMAL;


    Admin::Gender gender = record.value("Sex").toBool() ?
                                         Admin::Gender::MALE : Admin::Gender::FEMALE;
    Admin* admin = nullptr;
    QIcon   boyAdmin("C://Users//agishyan//Desktop//studentGroupIcons//adminman.png");
    QIcon   girlAdmin("C://Users//agishyan//Desktop//studentGroupIcons//admingirl.png");
    QIcon   noAdmin("C://Users//agishyan//Desktop//studentGroupIcons//unknown.png");

    QPixmap pixmap;
    if(gender == Admin::Gender::MALE) {
       QPixmap pic("C://Users//agishyan//Desktop//studentGroupIcons//adminman.png");
       pixmap = pic;
    } else {
       QPixmap pic("C://Users//agishyan//Desktop//studentGroupIcons//admingirl.png");
       pixmap = pic;
    }
    admin = new Admin(name,surname,gender,access,pixmap);
        emit adminReplaced(admin);

    this->hide();
}
