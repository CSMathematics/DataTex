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
#include <QButtonGroup>
#include "ExtendedTableWidget.h"
#include "pdfviewer.h"
//#include <QtPdf>
//#include <QtPdfWidgets>
//#include "qpdfviewer.h"

namespace Ui {
class SolveDatabaseExercise;
}

class SolveDatabaseExercise : public QDialog
{
    Q_OBJECT

public:
    explicit SolveDatabaseExercise(QWidget *parent = nullptr, QStringList meta = QStringList()
            , QString sections = QString());
    ~SolveDatabaseExercise();
    void GetNewSolutionContent(QString text);

    QString FieldsTex;
    QFile FieldsTexFile;
    QStringList ExerciseFiles;
    QStringList SubjectFiles;
    QStringList ComExerciseFiles;
    QStringList ComSubjectFiles;

private slots:
    void on_FieldsList_itemClicked(QListWidgetItem *item);
    void on_FileType_checked(bool checked);
    void on_SectionList_itemSelectionChanged();
    void ExerciseTable_SelectionChanged();
    void LoadTableHeaders();
    void on_RecompileButton_clicked();
    void on_CloseButton_clicked();
    void CreateSolution(QString filetype);
    void on_NewSolutionButton_clicked();
    void on_DeleteCurrentSolution_clicked();
    void SaveText();
    void on_SolutionsCombo_currentIndexChanged(int index);
    void updateFilter(QStringList values);
    void on_SaveContent_clicked();
    void on_ShowTexButton_clicked();
    void on_ShowPdfButton_clicked();
    void closeEvent (QCloseEvent *event);

private:
    Ui::SolveDatabaseExercise *ui;
    QSqlDatabase currentbase;
    QRadioButton * radiobutton;
    QString Exercise;
    QStringList Solutions;
    QString NewSolution;
    QString SolutionType;
    PdfViewer *view;
    PdfViewer *viewSolution;
    ExtendedTableWidget * ExerciseTable;
    QString CurrentBuildCommand;
    QString FileContent;
    QStringList metadata;
    QString Sections;
    QList<QAbstractButton *> CustomFileTypesList;
    QButtonGroup FileTypeGroup;
    QHash<QString,QString> FileTypeSolIds;
    QHash<QString,QString> FileTypeFolders;
    QHash<QString,QString> FileTypeFoldersSol;
    QString FileType;
    QString tempFile;

signals:
    void on_close(QString text);
    void solution(QString SolutionsTexFile,QString FileContent);
};

#endif // SOLVEDATABASEEXERCISE_H
