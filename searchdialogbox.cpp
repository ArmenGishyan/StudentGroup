#include <iostream>

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QIcon>
#include <QString>
#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QFormLayout>
#include <QDebug>
#include <QCheckBox>
#include <QSqlTableModel>
#include <QSqlError>
#include <QApplication>
#include <QCoreApplication>
#include <QSpacerItem>
#include <QSplitter>
#include <QButtonGroup>
#include <memory>

#include "searchdialogbox.h"
#include "studentgroupdb.h"
#include "groupsview.h"

SpinSliderWidget::SpinSliderWidget(QString label,int lowScore, int highScore,int defValue, QWidget *parrent):QWidget(parrent)
{
    m_spinBox = new QSpinBox(this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(lowScore, highScore);
    m_spinBox->setRange(lowScore, highScore);
    m_spinBox->setValue(defValue);
    m_slider->setValue(defValue);

    QObject::connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider, SLOT(setValue(int)));
    QObject::connect(m_slider, SIGNAL(valueChanged(int)), m_spinBox, SLOT(setValue(int)));
    QObject::connect(m_slider,  SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

    QHBoxLayout* lay_p = new QHBoxLayout(this);
    lay_p->addWidget(new QLabel(label, this));
    lay_p->addWidget(m_spinBox);
    lay_p->addWidget(m_slider);
    setLayout(lay_p);
}

int SpinSliderWidget::getScore() const
{
    return m_spinBox->value();
}

void SpinSliderWidget::setScore(const int value)
{
    m_spinBox->setValue(value);
}

void SpinSliderWidget::valueChanged(int score)
{
    emit scoreChanged(score);
}

SearchDialogBox::SearchDialogBox(QWidget *parrent):QWidget(parrent)
{
    QVBoxLayout* searchLayout = new QVBoxLayout;
    QVBoxLayout* scoreSlider = new QVBoxLayout;
    scoreSlider->setSpacing(0);

    m_lowsScore = new SpinSliderWidget("LowScore", 0, 100, 0, this);
    m_highScore = new SpinSliderWidget("HighScore", 0, 100, 100, this);
    scoreSlider->addWidget(m_lowsScore);
    scoreSlider->addWidget(m_highScore);
    setFixedWidth(500);

    QFormLayout* nameSurname = new QFormLayout;
    m_nameBox = new QLineEdit(this);
    m_nameBox->setClearButtonEnabled(true);
    m_surnameBox = new QLineEdit(this);
    m_surnameBox->setClearButtonEnabled(true);
    nameSurname->addRow(QObject::tr("Student's name \t"), m_nameBox);
    nameSurname->addRow(QObject::tr("Student's surname \t"), m_surnameBox);

    QGridLayout* checkBoxes = new QGridLayout;
    QHBoxLayout* jobscore = new QHBoxLayout;
    m_jobexperience = new QButtonGroup(this);
    m_jobexperience->addButton(new QCheckBox("Yes"), 0);
    m_jobexperience->addButton(new QCheckBox("No"), 1);
    m_jobexperience->addButton(new QCheckBox("All"), 2);
    checkBoxes->addWidget(new QLabel("Job Experience", this),0,0,1,2);
    checkBoxes->addWidget(m_jobexperience->button(0),1,0,2,1);
    checkBoxes->addWidget(m_jobexperience->button(1),1,1,2,2);
    checkBoxes->addWidget(m_jobexperience->button(2),1,2,2,3);
    m_jobexperience->button(2)->setChecked(true);

    m_withScores = new QCheckBox("Yes/No");
    checkBoxes->addWidget(new QLabel("With Scores", this),0,6,1,1);
    checkBoxes->addWidget(m_withScores,1,6,2,1);
    connect(m_withScores, SIGNAL(clicked(bool)), m_lowsScore, SLOT(setDisabled(bool)));
    connect(m_withScores, SIGNAL(clicked(bool)), m_highScore, SLOT(setDisabled(bool)));

    jobscore->addLayout(checkBoxes);

    QHBoxLayout* groupNameLayout = new QHBoxLayout;
    groupNameLayout->addWidget(new QLabel("Group Name"));
    m_groupName = new QComboBox(this);
    m_groupName->addItem("All");
    groupNameLayout->addWidget(m_groupName);

    QHBoxLayout* regionsLay = new QHBoxLayout;
    m_regionsBox = new QComboBox(this);
    m_regionsBox->addItems(StudentGroupDB::getRegions()<<"All");
    m_regionsBox->setCurrentText("All");
    regionsLay->addWidget(new QLabel("Region"));
    regionsLay->addWidget(m_regionsBox);

    QHBoxLayout* actionBtLayout = new QHBoxLayout;
    m_searchbt = new QPushButton(this);
    m_searchbt->setText(QObject::tr("Search"));
    connect(m_searchbt, SIGNAL(clicked(bool)), this, SLOT(searchStudent()));

    m_cancel = new QPushButton("Cancel");
    connect(m_cancel, SIGNAL(clicked(bool)), this, SLOT(hide()));
    actionBtLayout->addWidget(m_cancel);
    actionBtLayout->addWidget(m_searchbt);
    m_cancel->installEventFilter(this);
    m_searchbt->installEventFilter(this);

    searchLayout->setSpacing(10);
    QSplitter* spl = new QSplitter(this);
    searchLayout->addLayout(nameSurname);
    searchLayout->addLayout(scoreSlider);
    searchLayout->addWidget(spl);
    searchLayout->addLayout(jobscore);
    searchLayout->addWidget(spl);
    searchLayout->addLayout(groupNameLayout);
    searchLayout->addLayout(regionsLay);
    searchLayout->addWidget(spl);
    searchLayout->addLayout(actionBtLayout);
    setLayout(searchLayout);
    setWindowTitle("Search");
    show();

    QObject::connect(m_lowsScore, SIGNAL(scoreChanged(int)), this, SLOT(checkScoreRanges(int)));
    QObject::connect(m_highScore, SIGNAL(scoreChanged(int)), this, SLOT(checkScoreRanges(int)));
}
void SearchDialogBox::checkScoreRanges(const int lowScore) const
{
    int scoreH = m_highScore->getScore();
    int scoreL = m_lowsScore->getScore();
    if (scoreL > scoreH) {
        m_lowsScore->setScore(scoreL-1);
    }
}

bool SearchDialogBox::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_cancel) {
        if (event->type() == QEvent::MouseButtonPress) {
            this->hide();
            return true;
        }
    }

    return QObject::eventFilter(obj,event);
}

void SearchDialogBox::setGroupBoxValue(const QSet<QString> value)
{
    for(const QString& item : value) {
        m_groupName->addItem(item);
    }
}

void SearchDialogBox::searchStudent()
{
    SearchResult result = getSreachResult();
    QStringList searchStr;
    if(!result.name.isEmpty()) {
        searchStr.append("Name = '"+result.name + "'");
        searchStr.append(" and ");
    }

    if(!result.surname.isEmpty()) {
        searchStr.append("Surname = '"+result.surname + "'");
        searchStr.append(" and ");
    }

    if(!m_withScores->isChecked()) {
        searchStr.append("AverageScore <= '" + QString::number(result.highScore) + "'");
        searchStr.append("and AverageScore >= '" + QString::number(result.lowScore) + "'");
        searchStr.append(" and ");
    }

    QString str;
    if(!m_jobexperience->button(2)->isChecked()) {
       if(m_jobexperience->button(0)->isChecked()) {
           searchStr.append("WorkedOrNot = '"+m_jobexperience->button(0)->text()+"'");
           searchStr.append(" and ");
       } else if(m_jobexperience->button(1)->isChecked()) {
           str.append("WorkedOrNot = '"+m_jobexperience->button(1)->text()+"'");
           searchStr.append(" and ");
       }
    }

    if(m_regionsBox->currentText() != "All") {
        searchStr.append("Address = '"+m_regionsBox->currentText()+"'");
        searchStr.append(" and ");
    }

    if(m_groupName->currentText() != "All") {
        searchStr.append("GroupName = '"+m_groupName->currentText()+"'");
        searchStr.append(" and ");
    }

    if(m_regionsBox->currentText() != "All") {
        searchStr.append("Address = '" + m_regionsBox->currentText()+"'");
        searchStr.append(" and ");
    }
    searchStr.pop_back();
    QSqlTableModel* model = new QSqlTableModel(nullptr,StudentGroupDB::getDatabase());
    searchStr.join(" ");

    model->setTable("Students");
    model->setFilter(searchStr.join(" "));
    model->select();
    model->lastError();

    GroupsView* view = new GroupsView(model,"Search Result");
    emit throwView(view);
}
SearchResult SearchDialogBox::getSreachResult() const
{
    SearchResult sResult;
    sResult.name = m_nameBox->text();
    sResult.name;
    sResult.surname = m_surnameBox->text();
    sResult.lowScore = m_lowsScore->getScore();
    sResult.highScore = m_highScore->getScore();
    sResult.Group = m_groupName->currentText();

    return sResult;
}
