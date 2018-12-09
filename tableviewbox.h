#ifndef TABLEVIEWBOX_H
#define TABLEVIEWBOX_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>

class TableViewBox : public QWidget
{
    Q_OBJECT
public:
    explicit TableViewBox(QWidget *parent = nullptr);
    QTableView* getTableView() const
    {
        return m_tableView;
    }


signals:

public slots:

public:
    QSqlTableModel* m_tabelModel;
    QTableView*  m_tableView;

};

#endif // TABLEVIEWBOX_H
