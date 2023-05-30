#ifndef NOTESDOCUMENTS_H
#define NOTESDOCUMENTS_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QModelIndex>
#include <algorithm>
#include <QtSql>
#include <QSqlDatabase>
#include <QCloseEvent>
#include <QFileSystemModel>
#include "addfolder.h"
#include "addfiletoeditor.h"
#include "csvfunctions.h"
#include "datatex.h"
#include "ExtendedTableWidget.h"
#include "qpdfviewer.h"
#include "sqlfunctions.h"
#include "switch.h"
#include "tagslineeditwidget.h"
#include "rightclickmenu.h"

namespace Ui {
class NotesDocuments;
}

class NotesDocuments : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDocuments(QWidget *parent = nullptr, bool editMode = false, QStringList metadata = QStringList(),
                            QString fileName = QString()
                            , bool cloneMode = false, QString DocumentContent = QString(), bool ImportMode = false);
    ~NotesDocuments();

public slots:

    void EditBasicForders_DataTex(QString line);

private slots:

    bool isWhiteSpace(const QString & str);
    void Folder(bool checked);
    void on_CreateBasicFolder_clicked();
    void on_OkbuttonBoxFolders_accepted();
    void on_OkbuttonBoxFolders_rejected();
    QString SavePath(const QModelIndex &index);
    void Basic(QString newfolder);
    void on_FoldersStructureView_clicked(const QModelIndex &index);
    void DocumentTable_selectionChanged();
    void updateFilter(QStringList values);
    void NewSubFolder(QString newfolder);
    void on_CreateSubFolder_clicked();
    void on_RemButton_clicked();
    void on_EditButton_clicked();
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void AddDocType(QString docType);
    void on_OpenPath_clicked();
    void on_DatabaseCombo_activated(int index);
    void GetDocTypes();
    void LoadFolderStructure();
    void LoadDocTable();
    void LoadDatabaseFiles(QSqlDatabase database,QString query);
    int CountModelRows();
    int CountRandomFiles();
    void FilesTable_selectionchanged();

    void updateFilter_files(QStringList values);
    void on_RefreshSelection_clicked();
    void on_numOfFilesSpin_valueChanged(int arg1);
    void SelectedFilesInDocument();

    void on_RandomSelectionList_itemSelectionChanged();
    void on_AddRandomFiles_clicked();
    void filesSelected_SelectionChanged();
    void on_addEverything_clicked();
    void on_removeSelectedFile_clicked();
    void ShowPdfOfFile(bool checked, QString file);
    void CreateCustomTagWidget(QSqlDatabase database);
    void on_addfileButton_clicked();
    void AddFiles(int row);
    void on_removefileButton_clicked();
    void on_checkBox_clicked(bool checked);
    void on_FilesDatabasesCombo_activated(int index);
    void on_DocumentContent_cursorPositionChanged();
    void findFilePositions();

private:
    Ui::NotesDocuments *ui;
    QFileSystemModel *folderModel;
    addfolder * newFolder;
    QSqlDatabase currentbase;
    QSqlDatabase currentbase_Exercises;
    QString DocumentPath;
    QButtonGroup * radiogroup;
    QList<QRadioButton * >  radioList;
    QString BasicFolderFile;
    QString NotesPath;
    QStringList Exer_List;
    QStringList Solutions_List;
    QStringList Solved_List;
    QStringList Unsolved_List;
    QPdfViewer * view;
    ExtendedTableWidget * DocumentTable;
    QStringList Database_DocumentTableColumns;
    QString DocumentFileName;
    QString DocumentType;
    QString DocFileType;
    QString BasicFolder;
    QString SubFolder;
    QString SubSubFolder;
    QString DocumentFilePath;
    int level;
    QVector<QString> tags;
    TagsLineEditWidget * tagLine;
    QString DocContent;
    bool CloneMode;
    QString CurrentDocContent;
    ExtendedTableWidget * FilesTable;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
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
    QString CurrentDatabaseFile;
    int NumOfFiles;
    QVector<int> randomlist;
    int RandomFilesToKeep;
    QString PreviewFile;
    QPdfViewer *fileview;
    bool isEditMode;
    bool isImportMode;
    bool textChanged;
    bool isCursorInsideFile = false;
    FileToolBar * toolbar;

signals:
    void pathline(QString line);
    void createnewdocument(QString file,QString content);
    void clonedocument(QString file,QString content,QSqlDatabase database);
    void insertfiles();
    void nolines();
    void OpenSolutionFile(QString SolutionsTexFile);
};

#endif // NOTESDOCUMENTS_H
