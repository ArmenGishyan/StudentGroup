#ifndef ADMINDBOX_H
#define ADMINDBOX_H

#include <QDialog>

class QPixmap;
class QWidget;
class QLineEdit;

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
        UNKNOWN = 0,
        POWER = 1,
        ADVANCED = 2,
        NORMAL = 3
    };

    Admin(QString name = "unknown", QString surname = "unknown", Gender gen = Gender::FEMALE, Accessibility access = Accessibility::UNKNOWN,
          QPixmap pic = QPixmap("C://Users//agishyan//Desktop//studentGroupIcons//unknown.png"));

    QString getName() const
    {
        return m_name;
    }

    QString getSurname() const
    {
        return m_surname;
    }

    Admin::Gender getGender() const
    {
        return m_gender;
    }

    QPixmap getPicture()
    {
        return m_picture;
    }

    Accessibility getAccess() const
    {
        return m_access;
    }

private:
    QString m_name;
    QString m_surname;
    QPixmap m_picture;
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
};

#endif // ADMINDBOX_H
