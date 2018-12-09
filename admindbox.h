#ifndef ADMINDBOX_H
#define ADMINDBOX_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDebug>

class QPixmap;

//enum class Gender : unsigned int;
//enum class Accessibility : unsigned int;

class Admin
{
public:
    enum class Gender
    {
        MALE = 0,
        FEMALE = 1
    };

    enum class Accessibility
    {
        POWER = 0,
        ADVANCED = 1,
        NORMAL = 2
    };

    Admin() = default;
    Admin(QString name, QString surname, Gender gen, Accessibility access);


    QString getName() const
    {
        return m_name;
    }

    void setName(const QString& name)
    {
        m_name = name;
    }
    QString getSurname() const
    {
        return m_surname;
    }

    QString setSurname(const QString& surname)
    {
        m_surname = surname;
    }
    Admin::Gender getGender() const
    {
        return m_gender;
    }
    void setGender(Gender gender)
    {
        m_gender = gender;
    }
    QPixmap* getPicture()
    {
        return m_picture;
    }


private:
    QString m_name;
    QString m_surname;
    QPixmap* m_picture;
    Gender m_gender;
    Accessibility m_access;
};

class StudentGroupDB;


class AdminDBox : public QDialog
{
    Q_OBJECT
public:
    AdminDBox(QWidget* parrent = nullptr);

signals:
    void adminReplaced(Admin*);

public slots:
    void replaceAdmin();

private:
    QLineEdit* m_login;
    QLineEdit* m_password;
    Admin* m_admin;
};

#endif // ADMINDBOX_H
