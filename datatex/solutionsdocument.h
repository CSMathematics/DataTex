#ifndef SOLUTIONSDOCUMENT_H
#define SOLUTIONSDOCUMENT_H

#include <QDialog>
#include <QtWidgets>
#include <QList>
#include <QtSql>
#include <QSqlQuery>
#include "datatex.h"
#include "sqlfunctions.h"
#include "pdfviewer.h"
//#include <QtPdf>
//#include <QtPdfWidgets>
//#include "qpdfviewer.h"


namespace Ui {
class SolutionsDocument;
}

class SolutionsDocument : public QDialog
{
    Q_OBJECT

public:
    explicit SolutionsDocument(QWidget *parent = nullptr, QString fileName = QString(),
                               QString BuildCommand = QString(),
                               QStringList Exercises = QStringList() ,
                               QList<QStringList> SolutionsPerExercise = QList<QStringList>());
    ~SolutionsDocument();

private slots:
    void ItemChecked(QTreeWidgetItem *item, int column);
    void DocumentText();
    void on_SolutionCombo_currentIndexChanged(int index);
    void on_BuildButton_clicked();
    void SaveText();
    void on_ExercisesInDocument_clicked(const QModelIndex &index);

private:
    Ui::SolutionsDocument *ui;
    QString SolutionDocumentName;
    QHash<QList<int>,QWidget *> CellWidgets;
    QListWidget * s;
    QHash<QString,QStringList> Solutions;
    QHash<QString,QString> SolutionContent;
    PdfViewer *view;
    PdfViewer *DocView;
    QList<QStringList> SolutionsPerExercisePreview;
    QString CurrentBuildCommand;
    QStringList ExercisesInDocument;
    int depth = 0;
    int solIndex = 0;
    int exIndex = -1;
};

#endif // SOLUTIONSDOCUMENT_H
