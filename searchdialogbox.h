#ifndef SEARCHDIALOGBOX_H
#define SEARCHDIALOGBOX_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

class GroupsView;

class StudentGroupDB;
class QCheckBox;

struct SearchResult
{
    QString name;
    QString surname;
    int lowScore;
    int highScore;
    QString Group;
};

class SpinSliderWidget : public QWidget
{
    Q_OBJECT
public:
    SpinSliderWidget(QString label,int lowScore, int highScore, int defValue = 0, QWidget* parrent = nullptr);
    virtual ~SpinSliderWidget() = default;

    int getScore() const;
    void setScore(const int value);

signals:
    void scoreChanged(int score);

public slots:
    void valueChanged(int score);

private:
    QSpinBox* m_spinBox;
    QSlider*  m_slider;

};

class SearchDialogBox : public QWidget
{
    Q_OBJECT
public:
    SearchDialogBox(QWidget* parrent = nullptr);
    virtual ~SearchDialogBox() = default;
    bool eventFilter(QObject *obj, QEvent* e) override;
    void setGroupBoxValue(const QSet<QString> value);

signals:
    void throwView(GroupsView*);

public slots:
    SearchResult getSreachResult() const;
    void searchStudent();

private:
    SpinSliderWidget* m_lowsScore;
    SpinSliderWidget* m_highScore;
    QLineEdit* m_nameBox;
    QLineEdit* m_surnameBox;
    QComboBox* m_groupName;
    QPushButton* m_searchbt;
    QPushButton* m_cancel;
    QCheckBox* m_workedOrNot;
    QCheckBox* m_withScores;
    QComboBox* m_regionsBox;

private slots:
    void checkScoreRanges(const int) const;
};

#endif // SEARCHDIALOGBOX_H
