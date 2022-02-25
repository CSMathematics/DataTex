#ifndef ADDFILETOEDITOR_H
#define ADDFILETOEDITOR_H

#include <QDialog>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QFile>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include "sqlfunctions.h"
#include "datatex.h"
#include "ExtendedTableWidget.h"
#include "pdfviewer.h"
//#include <QtPdf>
//#include <QtPdfWidgets>
//#include "qpdfviewer.h"

namespace Ui {
class AddFileToEditor;
}

class AddFileToEditor : public QDialog
{
    Q_OBJECT

public:
    explicit AddFileToEditor(QWidget *parent = nullptr, QString currentTexFile = QString(),
                             QString BuildCommand = QString());
    ~AddFileToEditor();

public slots:

private slots:
    void on_Okbutton_rejected();
    void closeEvent (QCloseEvent *event);
    void on_addButton_clicked();
    void AddFiles(int row);
    void on_removeButton_clicked();
    void on_Okbutton_accepted();
    void on_Rebuild_clicked();
    void FilesTable_selectionchanged();
    void LoadDatabaseFiles(QSqlDatabase database,QString query);
    void updateFilter(QStringList values);
    void on_checkBox_clicked(bool checked);
    void SaveText();
    void on_FilesDatabasesCombo_activated(int index);
    void on_DocumentContent_cursorPositionChanged();
    void on_Save_clicked();
    void on_DocumentContent_textChanged();
    void on_RefreshSelection_clicked();
    void on_numOfFilesSpin_valueChanged(int arg1);
    void SelectedFilesInDocument();
    int CountRandomFiles();
    void on_RandomSelectionList_itemSelectionChanged();
    int CountModelRows();
    void on_AddRandomFiles_clicked();
    void filesSelected_SelectionChanged();
    void on_addEverything_clicked();
    void on_removeSelectedFile_clicked();
    void ShowPdfOfFile(bool checked, QString file);

private:
    Ui::AddFileToEditor *ui;
    QSqlDatabase currentbase;
    QSqlTableModel * tableModel;
    QString PreviewFile;
    QString FileType;
    QString ExSubType;
    QString SolutionFileType;
    QString ComFileType;
    QList<QCheckBox *> checklist;
    QString CurrentDatabaseFile;
    ExtendedTableWidget * FilesTable;
    PdfViewer *view;
//    PdfViewer *rview;
    int NumOfFiles;
    QVector<int> randomlist;
    int RandomFilesToKeep;

    PdfViewer *DocView;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
//    QList<QSqlDatabase> datalist;
    QString CurrentBuildCommand;
    QStringList ExercisesInsideDocument;
    QStringList DatabasesInsideDocument;
    QHash<QString,QList<int>> positions;
    QString DatabaseFileContent;
    PdfViewer * SelectedFileView;

};

#endif // ADDFILETOEDITOR_H
