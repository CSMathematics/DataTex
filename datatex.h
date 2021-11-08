#ifndef DATATEX_H
#define DATATEX_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSql>
#include <QSqlDatabase>
#include <QMetaMethod>
#include <QToolBar>
#include <QToolButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSqlTableModel>
#include <QProcess>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include "pdfviewer.h"
#include "qpdfviewer.h"
#include "ExtendedTableWidget.h"
#include "FilterLineEdit.h"
#include "FilterTableHeader.h"

#include <QtPdf>
#include <QtPdfWidgets>

enum FileData { Id,FileType,Field,Chapter,Section,ExerciseType,
                Difficulty,Path,Date,Solved,Bibliography,FileContent,
                Preamble,BuildCommand,FileDescription };

QT_BEGIN_NAMESPACE
namespace Ui { class DataTex; }
QT_END_NAMESPACE

class QTableView;
class DataTex : public QMainWindow
{
    Q_OBJECT

public:
    DataTex(QWidget *parent = nullptr);
    ~DataTex();

    static QSqlDatabase DataTeX_Settings;
    static QSqlDatabase CurrentTexFilesDataBase;
    static QSqlDatabase CurrentNotesFolderDataBase;
    static QString CurrentDataBasePath;
    static QString CurrentNotesFolderPath;
    static QHash<QString,QSqlDatabase> GlobalFilesDatabaseList;
    static QHash<QString,QSqlDatabase> GlobalDocsDatabaseList;
    static QHash<QString,QString> GlobalFilesDatabaseListNames;
    static QHash<QString,QString> GlobalDocsDatabaseListNames;
    static QString CurrentPreamble;
    static QString CurrentPreamble_Content;


    static QString PdfLatex_Command;
    static QString Latex_Command;
    static QString XeLatex_Command;
    static QString LuaLatex_Command;
    static QString Pythontex_Command;
    static QString Bibtex_Command;
    static QString Asymptote_Command;
    static QString RunCommand;
    static QHash<QString,QString> LatexCommands;
    static QHash<QString,QStringList> LatexCommandsArguments;

private:
    Ui::DataTex *ui;
    ExtendedTableWidget * FilesTable;
    ExtendedTableWidget * DocumentsTable;
    QSortFilterProxyModel *FilesProxyModel;
    QSortFilterProxyModel *DocumentsProxyModel;
    QString datatexpath;
    QSqlDatabase currentbase;
    PdfViewer * LatexFileView;
    PdfViewer * DocumentView;
    PdfViewer * FileFromDocumentView;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
    QStringList Database_DocumentTableColumns;
    QString CurrentBuildCommand;
    QString FileDescription;

    QString DocumentsTable_UpdateQuery;
    QString IsExercise;
    int IconSize;

//--------------- Menus ------------------
    QMenu * FileMenu;
    QMenu * EditMenu;
    QMenu * ViewMenu;
    QMenu * ToolMenu;
    QMenu * SettingsMenu;
    QMenu * HelpMenu;
    QMenu * CompileMenu;
    QMenu * CompileMenuDoc;
    QMenu * LatexFileActions;
    QMenu * DocumentActions;
//--------------- Toolbars ----------------------
    QToolBar * FileToolBar;
    QToolBar * LatexTools_ToolBar;
    QToolBar * DocTools_ToolBar;
    QToolBar * SettingsToolBar;
    QToolBar * CompileBar;
    QToolBar * CompileBarDoc;
//--------------- Actions ------------------------
    // ------ Database actions ---------
    QAction * NewDatabasefile;
    QAction * OpenDatabasefile;
    QAction * CloseDatabasefile;
    QAction * SyncDatabasefile;
    QAction * SaveAsDatabasefile;
    //---- Latex files actions -------
    QAction * NewLatexFile;
    QAction * OpenLatexFile;
    QAction * DeleteLatexFile;
    QAction * EditLatexFile;
    QAction * SolveLatexFile;
    // -------- Compile actions ---------
    QAction * PdfLatex;
    QAction * Latex;
    QAction * XeLatex;
    QAction * LuaLatex;
    QAction * PythonTex;
    QAction * BibTex;
    QAction * Asymptote;
    QAction * SaveTex;
    QAction * UndoTex;
    QAction * OpenPath;

    QAction * PdfLatexDoc;
    QAction * LatexDoc;
    QAction * XeLatexDoc;
    QAction * LuaLatexDoc;
    QAction * PythonTexDoc;
    QAction * BibTexDoc;
    QAction * AsymptoteDoc;
    QAction * SaveTexDoc;
    QAction * UndoTexDoc;
    QAction * OpenPathDoc;
    // -------- Doccument actions ---------
    QAction * NewDocument;
    QAction * InsertFileToDocument;
    QAction * AddDocument;
    QAction * DeleteDocument;
    QAction * CreateSolutionsDoc;
    QAction * EditDocument;
    QAction * CreateBibliography;
    QAction * UpdateDocContent;
    // -------- Settings actions ----------
    QAction * Preamble;
    QAction * GeneralSettings;
    QAction * BackUpDatabase;
    QAction * Datatables;
    QAction * ConnectWithTexEditor;

// ------------- ToolButtons -------------
    QToolButton * CompileCommands;
    QToolButton * CompileCommandsDoc;


    QSqlTableModel * DocumentModel;

    QComboBox * FilesPreambleCombo;
    QComboBox * DocumentsPreambleCombo;
    //QDialog * MetadataDialog;
    //QMap<QString,QString> MetadataMap;
    //QList<QWidget *> MetaWidgets;
    //QStringList Basic_Metadata_Ids;
    //QStringList Basic_Metadata_Names;
    //QStringList Optional_Metadata_Ids;
    //QStringList Optional_Metadata_Names;
    //QStringList Bibliography_Ids;
    //QStringList Bibliography_Names;
    //QMap<QString,QString> mapIdsNames;
    //QMap<QString,QLineEdit *> mapIdsWidgets;
    //QList<QLabel *> labelList;
    //QList<QLineEdit *> lineList;
    //QList<QHBoxLayout *>hLayoutList;
    QString OptionalFields;
    QString DocOptionalFields;

    //Latex Files metadata variables---------
    QString DatabaseFileName;
    QString Field;
    QString Chapter;
    QString Section;
    QString ExerciseType;
    QString FileType;
    QString DatabaseFilePath;
    int Difficulty;
    QString Date;
    QString Solved;
    QString FileContent;
    //----------------------------------------
    //Latex Documents metadata variables---------
    QString DocumentFileName;
    QString DocFileType;
    QString BasicFolder;
    QString SubFolder;
    QString SubSubFolder;
    QString DocumentFilePath;
    QString DatabaseSource;
    QString DocumentDate;
    QString DocumentContent;
    QString DocumentBuildCommand;
    int DocumentNeedsUpdate;
    QStringList FilePathsInADocument;
    QStringList DatabasesInADocument;
    //----------------------------------------
//    QList<QLineEdit *> LineEditList;
    QStringList Exer_List;
    QStringList Solutions_List;
    QStringList Solved_List;
    QStringList Unsolved_List;
    QList<QStringList> SolutionsPerExercise;

    QStringList Optional_Metadata_Ids;
    QStringList Optional_Metadata_Names;
    QList<QLabel *> labelList;
    QList<QLineEdit *> lineList;
    QList<QHBoxLayout *> hLayoutList;
    QStringList Optional_DocMetadata_Ids;
    QStringList Optional_DocMetadata_Names;
    QList<QLabel *> Doc_labelList;
    QList<QLineEdit *> Doc_lineList;
    QList<QHBoxLayout *> Doc_hLayoutList;
    QFileSystemModel *model;

//    QStringList Metadata;
//    QSqlDatabase currentbase;
//    QLineEdit * IdLine;
//    QComboBox * FieldCombo;
//    QComboBox * ChapterCombo;
//    QComboBox * SectionCombo;
//    QComboBox * ExerciseTypeCombo;
//    QComboBox * FileTypeCombo;
//    QLineEdit * PathLine;
//    QSpinBox * DifficultySpin;
//    QDateTimeEdit * DateTimeEdit;
//    QLineEdit * SolvedLine;
//    QTextEdit * FileText;
//    QGridLayout * layout;
//    QListWidget * SectionsList;
//    int currentSectionRow = -1;
//    QDialogButtonBox * MetadataOkButton;
//    QList<QHBoxLayout *> hLayoutList2;
//    QList<QLineEdit *> BibLineEditList;
//    QMap<QString,QLineEdit *> mapBibliographyWidgets;
//    QMap<QString,QString> mapBibliographyValues;
//    QStringList bibDescriptions;
//    QStringList bibNames;

private slots:
    void ShowDataBaseFiles();
    void ShowDocuments();
    void CreateToolBars();
    void CreateMenus_Actions();
    void CreateBuildCommands();
    void SettingsDatabase_Variables();
    void DatabaseStructure(QString database);
    void NewDatabaseBaseFile();
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, const char * Function, QString ShortCut, QIcon Icon, const char * Description);
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, const char * Description);
    void EditNewBaseFile(QString fileName, QString FileContent);
    void SolutionFile();
    void InsertFiles();
    void AddFilesToEditor(QStringList files);
    void EditDataBase();
    void PersonalNotes();
    void CreateNewSheet(QString fileName, QString Content);
    void DataBaseFields();
    void CreateSolutionsDocument();
    void DataTeX_Preferences();
    void DatabaseSyncFiles();
    void EditFileMeta();
    void BackUp_DataBase_Folders();
    void setDefaultAction(QAction * action);
    void setDefaultActionDoc(QAction * action);

    void on_FilesDatabaseToggle_clicked(bool checked);
    void on_DocumentsDatabaseToggle_clicked(bool checked);
    void FilesTable_selectionchanged();
    void DocumentsTable_selectionChanged();
    QString SetLatexCommand(QString SQLCommandSetting, QString Command, QAction * Action, QAction * Action2 , QStringList args, QString ext);
    void SaveText();
    void SaveDocText();

    void CreateDatabase();
    void CreateNewDatabase(QString Path, QString FolderName, QString fileName, QString DatabaseType);
    void UpdateCurrentDatabase(QString FullPath);
    void UpdateCurrentNotesDatabase(QString FullPath);
    void AddDatabaseToTree(int row, QString databasePath, QString databaseName);
    void DeleteFileFromBase();
    void DeleteDocumentFromBase();
    void Preamble_clicked();
    void AddPreamble(QStringList preamble);

    void on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    QString GetLatexCommand(QString SQLCommandSetting, QAction *Action, QAction *Action2, QStringList args, QString ext);
    void CompileToPdf();
    void Compile();
    void CompileAsymptote();
    void TeXFilesTable_selection_changed();
    void loadDatabaseFields();
    void updateFilter(QStringList values);
    void getActionFromText(QMenu *menu, QToolButton *button);
    void on_DatabaseStructureTreeView_clicked(const QModelIndex &index);
    void setPreamble();
    void on_ComboCount_currentIndexChanged(int index);
    void OpenLoadDatabase();
    void OpenDatabaseInfo(QString filePath, QString FolderName);
    void RemoveCurrentDatabase();
    void UpdateDocument();
    void AddFileToDatabase();
    void on_DatabaseStructureTreeView_doubleClicked(const QModelIndex &index);

public slots:
    static void CreateTexFile(QString fullFilePath);
    static void BuildDocument(QString CompileCommand, QString fullFilePath, QStringList args, QString ext);
    static void ClearOldFiles(QString fullFilePath);
    static void loadImageFile(QString exoFile, PdfViewer * view);

    static void loadImageFile(QString exoFile, QPdfViewer * view);

//    static void BuildChain(QStringList ListOfCommands);
    static void updateTableView(QTableView * table, QString QueryText, QSqlDatabase Database, QObject *parent);
    static QString NewFileText(QString fileName);
    static void SaveContentToDatabase(QString fileName, QString content);
    static void FilterTables_Queries(QStringList list);
    static void FilterDocuments(QStringList list);
    static void LoadTableHeaders(QTableView * table, QStringList list);
    static void FunctionInProgress();
};

#endif // DATATEX_H
