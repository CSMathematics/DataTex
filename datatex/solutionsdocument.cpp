#include "solutionsdocument.h"
#include "ui_solutionsdocument.h"
#include <QFileInfo>

SolutionsDocument::SolutionsDocument(QWidget *parent,QHash<QString,QStringList> SolutionsPerExercise) :
    QDialog(parent),
    ui(new Ui::SolutionsDocument)
{
    ui->setupUi(this);
    ui->ExercisesInDocument->setColumnCount(4);
    for (int i=0;i<SolutionsPerExercise.count();i++ ) {
        QString exercise = QFileInfo(SolutionsPerExercise.keys().at(i)).baseName();
        QStringList solutions = QStringList();
        MultiList *s = new MultiList();
        for (int k=0;k<SolutionsPerExercise.values().at(i).count();k++ ) {
            solutions.append(QFileInfo(SolutionsPerExercise.values().at(i).at(k)).baseName());
        }
        s->addItems(solutions);
        s->setCheckedItems({solutions.at(0)});
        MultiList *sp = new MultiList();
        sp->addItems(SolutionsPerExercise.values().at(i));
        sp->setCheckedItems({SolutionsPerExercise.values().at(i).at(0)});
        ui->ExercisesInDocument->insertRow(i);
        ui->ExercisesInDocument->setCellWidget(i,2,s);
        ui->ExercisesInDocument->setCellWidget(i,3,sp);
        ui->ExercisesInDocument->setColumnHidden(1,true);
        ui->ExercisesInDocument->setColumnHidden(3,true);
        ui->ExercisesInDocument->setItem(i,0 , new QTableWidgetItem(exercise));
        ui->ExercisesInDocument->setItem(i,1 , new QTableWidgetItem(SolutionsPerExercise.keys().at(i)));
//        connect(s, SIGNAL(currentTextChanged(QString)),this, SLOT(SolutionChanged(QString)));
    }
}

void SolutionsDocument::SolutionChanged(QString item)
{
//    QStringList solutions;
//    for (int i=0;i<ui->ExercisesInDocument->rowCount();i++) {
//        solutions.append(ui->ExercisesInDocument->item(i,3)->text().split(" + "));
//    }
//    qDebug()<<solutions.join(",");
}

SolutionsDocument::~SolutionsDocument()
{
    delete ui;
}

void SolutionsDocument::on_ExercisesInDocument_cellClicked(int row, int column)
{
//    s = new QListWidget(this);
//    s->addItem("1st");
//    s->addItem("2nd");

//    for (int i=0;i<s->count();i++ ) {
//            s->item(i)->setFlags(s->item(i)->flags() | Qt::ItemIsUserCheckable);
//            s->item(i)->setCheckState(Qt::Unchecked);
//            }

//    MultiList *s = new MultiList();
//        s->addItems(QStringList() << "One" << "Two" << "Three" << "Four");
//        s->setCheckedItems(QStringList() << "One" << "Two");
//    ui->ExercisesInDocument->setCellWidget(row,column,s);
}

