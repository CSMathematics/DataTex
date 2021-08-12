#ifndef SOLVEDATABASEEXERCISE_H
#define SOLVEDATABASEEXERCISE_H

#include <QDialog>
#include <QWidget>
#include <QFile>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QFile>
#include <QAbstractButton>
#include <QtSql>
#include <QSqlDatabase>
#include <QRadioButton>
#include "ExtendedTableWidget.h"
#include "pdfviewer.h"
#include <QtPdf>
#include <QtPdfWidgets>
#include "qpdfviewer.h"

namespace Ui {
class SolveDatabaseExercise;
}

class SolveDatabaseExercise : public QDialog
{
    Q_OBJECT

public:
    explicit SolveDatabaseExercise(QWidget *parent = nullptr);
    ~SolveDatabaseExercise();
    void GetNewSolutionContent(QString text);

    QString FieldsTex;
//    QString EidiThematwnTex;
    QFile FieldsTexFile;
//    QFile EidiThematwnTexFile;
    QStringList ExerciseFiles;
    QStringList SubjectFiles;
    QStringList ComExerciseFiles;
    QStringList ComSubjectFiles;

private slots:
    void on_FieldsList_itemClicked(QListWidgetItem *item);

    QString getFileType();

    void on_FileType_checked(bool checked);

    void on_SectionList_itemSelectionChanged();

    void ExerciseTable_SelectionChanged();
    void LoadTableHeaders();
    void on_RecompileButton_clicked();
    void on_CloseButton_clicked(QAbstractButton *button);
    void CreateSolution(QString filetype);
//    void on_TexPage_clicked(bool checked);
//    void on_PdfPage_clicked(bool checked);
    void on_NewSolutionButton_clicked();
    void on_DeleteCurrentSolution_clicked();
    void SaveText();
    void on_SolutionsCombo_currentIndexChanged(int index);
    void updateFilter(QStringList values);
    void on_SaveContent_clicked();

private:
    Ui::SolveDatabaseExercise *ui;
    QSqlDatabase currentbase;
    QRadioButton * radiobutton;
    QString Exercise;
    QStringList Solutions;
    int SolutionsCount;
    QString NewSolution;
    QString SolutionType;
    QPdfViewer *view;
    PdfViewer *viewSolution;
    ExtendedTableWidget * ExerciseTable;
    QString CurrentBuildCommand;
    QString FileContent;

signals:

    void SolutionFile(QString SolutionsTexFile,QMap<QString,QString> data,QStringList Sections);
};

#endif // SOLVEDATABASEEXERCISE_H
