#ifndef ADDFILETOEDITOR_H
#define ADDFILETOEDITOR_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtPdf>
//#include <QtPdfWidgets>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QTextBlock>
#include <QTextCursor>
#include "datatex.h"
#include "sqlfunctions.h"
#include "qpdfviewer.h"
#include "ExtendedTableWidget.h"
//#include "pdfviewer.h"
#include "switch.h"
#include "rightclickmenu.h"
#include "filecommands.h"

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
    void onRebuild();
    void FilesTable_selectionchanged();
    void LoadDatabaseFiles(QSqlDatabase database,QString query);
    void updateFilter(QStringList values);
    void on_checkBox_clicked(bool checked);
    void on_FilesDatabasesCombo_activated(int index);
    void on_DocumentContent_cursorPositionChanged();
    void on_RefreshSelection_clicked();
    void SelectedFilesInDocument();
    int CountRandomFiles();
    void on_RandomSelectionList_itemSelectionChanged();
    int CountModelRows();
    void on_AddRandomFiles_clicked();
    void filesSelected_SelectionChanged();
    void on_addEverything_clicked();
    void on_removeSelectedFile_clicked();
    void CreateCustomTagWidget(QSqlDatabase database);
    void findFilePositions();

private:
    Ui::AddFileToEditor *ui;
    DTXDocument Document;
    QSqlDatabase currentbase;
    QSqlTableModel * tableModel;
    QString PreviewFile;
    QString FileType;
    QString ExSubType;
    QString SolutionFileType;
    QString ComFileType;
    QList<QCheckBox *> checklist;
    QString CurrentFile;
    ExtendedTableWidget * FilesTable;
    QPdfViewer *view;
    int NumOfFiles;
    QVector<int> randomlist;
    int RandomFilesToKeep;
    QPdfViewer *DocView;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
    QString CurrentBuildCommand;
    QStringList ExercisesInsideDocument;
    QStringList DatabasesInsideDocument;
    QHash<QString,QList<int>> positions;
    QString DatabaseFileContent;
    RightClickMenu * RightClick;
    QListWidget * FilesRightClickMenu;
    QVector<int> FilesTableHiddenColumns;
    TagsFilterWidget * filesTagLine = nullptr;
    QSortFilterProxyModel *FilesProxyModel;
    bool filesSorting;
    QSqlQueryModel * FilesModel;
    bool isCursorInsideFile = false;
    FileToolBar * toolbar = nullptr;
    QString singlefile;
};

#endif // ADDFILETOEDITOR_H
