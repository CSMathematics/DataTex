#ifndef SOLVEDATABASEEXERCISE_H
#define SOLVEDATABASEEXERCISE_H

#include <QDialog>
#include <QWidget>
#include <QFile>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QAbstractButton>
#include <QtSql>
#include <QSqlDatabase>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QCloseEvent>
#include <QCheckBox>
#include "sqlfunctions.h"
#include "datatex.h"
#include "csvfunctions.h"
#include "ExtendedTableWidget.h"
//#include "pdfviewer.h"
#include <QtPdf>
//#include <QtPdfWidgets>
#include "qpdfviewer.h"
#include "latexeditorwidget.h"
#include "filecommands.h"

namespace Ui {
class SolveDatabaseExercise;
}

class SolveDatabaseExercise : public QDialog
{
    Q_OBJECT

public:
    explicit SolveDatabaseExercise(QWidget *parent = nullptr, QString fileName = QString());
    ~SolveDatabaseExercise();
    void GetNewSolutionContent(QString text);

    QString FieldsTex;
    QFile FieldsTexFile;
    QStringList ExerciseFiles;
    QStringList SubjectFiles;
    QStringList ComExerciseFiles;
    QStringList ComSubjectFiles;

private slots:
    void ExerciseTable_SelectionChanged();
    void RebuildSolution();
    void on_CloseButton_clicked();
    void CreateSolution();
    void on_NewSolutionButton_clicked();
    void on_DeleteCurrentSolution_clicked();
    void on_SolutionsCombo_currentTextChanged(QString id);
    void updateFilter(QStringList values);
    void closeEvent (QCloseEvent *event);
    void RebuildExercise();

private:
    Ui::SolveDatabaseExercise *ui;
    QSqlDatabase currentbase;
    QString Exercise;
    QStringList Solutions;
    QString NewSolution;
    QString SolutionType;
    LatexTextWidget * exercise;
    QPdfViewer *view;
    QPdfViewer *viewSolution;
    ExtendedTableWidget * ExerciseTable;
    QString CurrentBuildCommand;
    QHash<QString,QString> FileTypeSolIds;
    QHash<QString,QString> FileTypeFolders;
    QHash<QString,QString> FileTypeFoldersSol;
    QString FileType;
    FileToolBar * exer_Toolbar = nullptr;
    FileToolBar * sol_Toolbar = nullptr;
    QStringList Database_FileTableFields;
    bool filesSorting;

signals:
    void on_close(QString text);
    void solution(QString SolutionsTexFile,QString FileContent);
};

#endif // SOLVEDATABASEEXERCISE_H
