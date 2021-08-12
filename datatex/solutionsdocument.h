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
#include <QtPdf>
#include <QtPdfWidgets>
#include "qpdfviewer.h"


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
    void ItemChecked(QListWidgetItem * item);
    void DocumentText();

    void on_ExercisesInDocument_itemClicked(QTableWidgetItem *item);

    void on_SolutionCombo_currentIndexChanged(int index);

    void on_BuildButton_clicked();
    void SaveText();

private:
    Ui::SolutionsDocument *ui;
    QString SolutionDocumentName;
    QHash<QList<int>,QWidget *> CellWidgets;
    QListWidget * s;
    QHash<QString,QStringList> Solutions;
    QHash<QString,QString> SolutionContent;
    PdfViewer *view;
    QPdfViewer *DocView;
    QList<QStringList> SolutionsPerExercisePreview;
    QString CurrentBuildCommand;
};

#endif // SOLUTIONSDOCUMENT_H
