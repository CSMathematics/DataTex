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
                               QHash<QString, QStringList> SolutionsPerExercise = QHash<QString,QStringList>(),
                               QHash<QString,QSqlDatabase> DatabasePerSolution = QHash<QString,QSqlDatabase>(),
                               QMap<int,QString> exerciseOrder = QMap<int,QString>());
    ~SolutionsDocument();

private slots:
    void ItemChecked(QTreeWidgetItem *item, int column);
    void DocumentText();
    void on_BuildButton_clicked();
    void SaveText();
    void on_ExercisesInDocument_clicked(const QModelIndex &index);

private:
    Ui::SolutionsDocument *ui;
    QString SolutionDocumentName;
    QHash<QString,QStringList> Solutions;
    QHash<QString,QString> SolutionContent;
    PdfViewer *view;
    PdfViewer *DocView;
    QHash<QString,QStringList> SolutionsPerExercisePreview;
    QHash<QString,QSqlDatabase> databasePerSolutionFile;
    QMap<int,QString> fileOrder;
    QString CurrentBuildCommand;
    QString CurrentExercise;
    QString CurrentSolution;
    bool docReady;
};

#endif // SOLUTIONSDOCUMENT_H
