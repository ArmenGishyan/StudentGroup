#ifndef EMAILWRITER_H
#define EMAILWRITER_H

#include <QWidget>

//#include "SmtpClient-for-Qt/src/emailaddress.h"
//#include "SmtpClient-for-Qt/src/smtpclient.h"
//#include "SmtpClient-for-Qt/src/mimetext.h"

class QTextEdit;
class QLineEdit;
class QCheckBox;

class EmailWriter : public QWidget
{
    Q_OBJECT
public:
    explicit EmailWriter(QWidget *parent = nullptr);


signals:

public slots:

private:
    QCheckBox* m_all;
    QStringList* m_address;
    QLineEdit* m_subject;
    QTextEdit* m_messageBox;

};

#endif // EMAILWRITER_H
