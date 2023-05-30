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
#include <QFileSystemModel>
#include <QTableWidgetItem>
#include <QSettings>
#include <QActionGroup>
//#include "pdfium/pdfviewerwidget.h"
#include "qpdfviewer.h"
#include "ExtendedTableWidget.h"
#include "FilterLineEdit.h"
#include "FilterTableHeader.h"
#include "tagslineeditwidget.h"
#include "latexeditorwidget.h"
#include "filecommands.h"
#include "sqlfunctions.h"
#include "rightclickmenu.h"
#include "graphicsbuilder.h"
#include "tablebuilder.h"

#include <QtPdf>
//#include <QtPdfWidgets>

enum FileData { Id,FileType,Field,Chapter,Section,ExerciseType,
                Difficulty,Path,Date,Solved,Bibliography,FileContent,
                Preamble,BuildCommand,FileDescription};

enum DocData { DocId,DocumentType,BasicFolder,SubFolder,SubsubFolder,DocPath,
                DocDate,DocContent,DocPreamble,DocBuildCommand,NeedsUpdate,DocBibliography,
                UseBibliography,FilesInDocument,BibEntries,CustomTags};

//enum NewFileMode {
//    BasicMode,ImportMode,EditMode,CloneModeOnlyContent,CloneModeContentAndMetadata
//};


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
    static QSqlDatabase Bibliography_Settings;
    static QSqlDatabase CurrentTexFilesDataBase;
    static QSqlDatabase CurrentDocumentDataBase;
    static QString CurrentDataBasePath;
    static QString CurrentDataBase_basename;
    static QString CurrentDocumentDatabasePath;
    static QString CurrentDocumentDatabase_basename;
    static QHash<QString,QSqlDatabase> GlobalFilesDatabaseList;
    static QHash<QString,QSqlDatabase> GlobalDocsDatabaseList;
    static QHash<QString,QString> GlobalFilesDatabaseListNames;
    static QHash<QString,QString> GlobalDocsDatabaseListNames;
    static QString CurrentPreamble;
    static QString CurrentPreamble_Content;
    static QStringList DocTypesIds;
    static QStringList DocTypesNames;
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
    static QString GlobalSaveLocation;
    static QString TexLivePath;
    static QHash<QString,QStringList> Optional_Metadata_Ids;
    static QHash<QString,QStringList> Optional_Metadata_Names;
    static QHash<QString,QStringList> Optional_DocMetadata_Ids;
    static QHash<QString,QStringList> Optional_DocMetadata_Names;
    static QTranslator translator;
    static QString currentlanguage;
    static QString datatexpath;
    static QStringList SVG_IconPaths;


private:
    Ui::DataTex *ui;
    ExtendedTableWidget * FilesTable;
    ExtendedTableWidget * DocumentsTable;
    ExtendedTableWidget * BibliographyTable;
    ExtendedTableWidget * CommandsTable;
    ExtendedTableWidget * PreamblePackageClassTable;
    ExtendedTableWidget * TabularsFiguresTableWidget;

    QSortFilterProxyModel *FilesProxyModel;
    QSortFilterProxyModel *DocumentsProxyModel;
    QSortFilterProxyModel *BibliographyProxyModel;
    QSortFilterProxyModel *CommandsProxyModel;
    QSortFilterProxyModel *PreamblePackageClassProxyModel;
    QSortFilterProxyModel *TabularsFiguresProxyModel;

    QSqlDatabase currentbase;
    QPdfViewer * LatexFileView;
    QPdfViewer * DocumentView;
    QPdfViewer * FileFromDocumentView;
    QPdfViewer * BibFileView;
    QPdfViewer * BibFileView_2;
    QPdfViewer * DocDependenciesFileView;

    QPdfViewer * CommandSampleView;
    QPdfViewer * PreambleSampleView;
    QPdfViewer * TableFigureSampleView;

    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
    QStringList Database_DocTableFieldNames;
    QStringList Database_DocumentTableColumns;
    QStringList BibliographyTableColumns;
    QStringList BibliographyFieldIds;
    QStringList CommandTableFields;
    QStringList CommandTableFieldNames;
    QStringList PreamblePackageClassTableFieldIds;
    QStringList PreamblePackageClassTableFieldNames;
    QStringList TabularFigureTableFieldIds;
    QStringList TabularFigureTableFieldNames;

    QString CurrentBuildCommand;
    QString FileDescription;
    QHash<QString,bool> isDBEncrypted;
    QHash<QString,QStringList> encryptionData;
    int fdb_enc;
    int ddb_enc;
    int bibliography_db_enc;
    int commands_db_enc;
    int preamble_db_enc;
    int tabularfigure_db_enc;

    QString DocumentsTable_UpdateQuery;
    QString FileTypeId;
    int Solvable;
    int IconSize;

//--------------- Menus ------------------
    QMenu * FileMenu;
    QMenu * EditMenu;
    QMenu * ViewMenu;
    QMenu * ToolMenu;
    QMenu * DocToolMenu;
    QMenu * BibliographyMenu;
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
    QToolBar * BibliographyToolBar;
    QToolBar * CommandToolBar;
    QToolBar * PreamblePackageClassToolBar;
    QToolBar * TabularFigureToolBar;
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
    QAction * ExportAsSQL;
    QAction * CloseApp;
    //---- Latex files actions -------
    QAction * NewLatexFile;
    QAction * NewGraphicsBuilder;
    QAction * NewTableBuilder;
    QAction * OpenLatexFile;
    QAction * DeleteLatexFile;
    QAction * EditLatexFile;
    QAction * SolveLatexFile;
    QAction * FileCloneToOtherDB;
    QAction * FileEditHistory;
    // -------- Compile actions ---------
    QAction * PdfLatex;
    QAction * Latex;
    QAction * XeLatex;
    QAction * LuaLatex;
    QAction * PythonTex;
    QAction * BibTex;
    QAction * Asymptote;
    QAction * OpenPath;
    QAction * ShowFileTex;
    QAction * ShowFilePdf;
    QAction * ShowToolBar;
    QAction * ShowSideBar;

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
    QAction * ShowDocTex;
    QAction * ShowDocPdf;
    QAction * ShowDocToolBar;
    QAction * ShowDocSideBar;

    // -------- Bibliography actions ---------
    QAction * NewBibEntry;
    QAction * EditBibEntry;
    QAction * AuthorsEditors;
    QAction * OpenBibEntry;
    QAction * DeleteBibEntry;
    // -------- Document actions ---------
    QAction * NewDocument;
    QAction * InsertFileToDocument;
    QAction * AddDocument;
    QAction * DeleteDocument;
    QAction * CreateSolutionsDoc;
    QAction * EditDocument;
    QAction * CreateBibliography;
    QAction * UpdateDocContent;
    QAction * CloneDocument;
    QAction * DocEditHistory;
    // -------- Settings actions ----------
    QAction * Preamble;
    QAction * GeneralSettings;
    QAction * BackUpDatabase;
    QAction * Datatables;
    QAction * ConnectWithTexEditor;
    // -------- Help actions ----------
    QAction * Info;
    QAction * YouTube;
    QAction * FaceBook;
    QAction * GitHub;
    QAction * Email;
    QAction * Site;
    //---- View actions -------
    QAction * ShowFileDescription;
    QAction * ShowFileTabWidget;
    QAction * SplitTexAndPdf;

    QAction * ShowDocDescription;
    QAction * ShowDocTabWidget;

    // -------- Command actions ---------
    QAction * NewCommand;
    QAction * AddCommand;
    QAction * DeleteCommand;
    QAction * EditCommand;
    // -------- Command actions ---------
    QAction * NewPreamblePackageClass;
    QAction * InsertCommandTo_PreamblePackageClass;
    QAction * AddPreamblePackageClass;
    QAction * DeletePreamblePackageClass;
    QAction * EditPreamblePackageClass;
    // -------- Table actions ---------


// ------------- ToolButtons -------------
    QToolButton * CompileCommands;
    QToolButton * CompileCommandsDoc;

    QComboBox * FilesPreambleCombo;
    QComboBox * DocumentsPreambleCombo;
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
    QString DocumentTitle;
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
    int DocumentUseBibliography = 0;
    QString DocumentDescription;
    QString StuffToAddToPreamble = QString();
    QHash<QString,QString> FilePathsInADocument;
    QStringList DatabasesInADocument;
    //Bibliography metadata variables---------
    QString CitationKey;
    QString BibDocType;
    QString BibTitle;
    QString Authors;
    QString Editors;
    QString Translators;
    QString Publisher;
    int BibYear;
    QString BibMonth;
    QString ISBN;
    QString ISSN;
    int Pages;
    QString Series;
    QString Edition;
    QString BibChapter;
    QString BibNumber;
    QString Volume;
    QString Journal;
    QString Institution;
    QString School;
    QString Issue;
    QString Address;
    QString DOI;
    QString URL;
    QString Language;
    QString Location;
    QString Subtitle;
    QString Organization;
    QString BibKey;
    QString Abstract;
    QString CrossReference;
    QString BibNote;
    QMultiHash<QString,QStringList> DocEntries;
    //----------------------------------------
    QHash<QString,QStringList> SolutionsPerExercise;
    QHash<QString,QSqlDatabase> DatabasePerSolution;
    QMap<int,QString> exerciseOrder;
    QList<QLabel *> labelList;
    QList<QLineEdit *> lineList;
    QList<QHBoxLayout *> hLayoutList;
    QList<QLabel *> Doc_labelList;
    QList<QLineEdit *> Doc_lineList;
    QList<QHBoxLayout *> Doc_hLayoutList;
    QHash<QString,QLineEdit *> Bib_ValueList;
    QHash<QString,QLineEdit *> Bib_ValueList_Opt;
    QFileSystemModel *model;
    QToolButton *tb;
    QStringList stringList;
    QString CloneSolution;
    TagsFilterWidget * filesTagLine;
    TagsFilterWidget * docsTagLine;
    RightClickMenu * RightClick;
    RightClickMenu * DocRightClick;
    RightClickMenu * BibRightClick;
    bool filesSorting;
    bool docsSorting;
    bool bibSorting;
    QSqlQueryModel * FilesModel;
    QSqlQueryModel * DocsModel;
    QSqlQueryModel * BibModel;
    QStringList MetadataFieldNames;
    QStringList DocMetadataNames;
    QStringList BibliographyFieldNames;

private slots:
    void ShowDataBaseFiles();
    void ShowDocuments();
    void ShowBibliography();
    void CreateToolBars();
    void CreateMenus_Actions();
    void CreateBuildCommands();
    void SettingsDatabase_Variables();
    void DatabaseStructure(QString database);
    void NewDatabaseBaseFile();
    void NewGraphicsFile();
    void NewTableFile();
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, const char * Function, QString ShortCut, QIcon Icon, QString Description);
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description);
    void NewFileAddedToDatabase(QString filePath);
    void SolutionFile();
    void InsertFiles();
    void PersonalNotes();
    void CreateNewDocument(QString filePath, QString Content);
    void DataBaseFields();
    void CreateSolutionsDocument();
    void DataTeX_Preferences();
    void DatabaseSyncFiles();
    void EditFileMeta();
    void EditDocumentDialog();
    void BackUp_DataBase_Folders();
    void setDefaultAction(QAction * action);
    void setDefaultActionDoc(QAction * action);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index,const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    void on_FilesDatabaseToggle_clicked(bool checked);
    void on_DocumentsDatabaseToggle_clicked(bool checked);
    void FilesTable_selectionchanged();
    void DocumentsTable_selectionChanged();
    void BibliographyTable_selectionChanged();
    QString SetLatexCommand(QString SQLCommandSetting, QString Command, QAction * Action, QAction * Action2 , QStringList args, QString ext);
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
    void CloneCurrentDocument();
    void AddFileToDatabase();
    void AddDocToDatabase();
    void on_DatabaseStructureTreeView_doubleClicked(const QModelIndex &index);
    QString BibSourceCode();
    void MetadataToolButton();
    void on_addBibEntry_clicked();
    void on_addDocBibEntry_clicked();
    void FileClone();
//    void SelectNewFileInModel(QTableView *table);

    void NewBibliographyEntry();
    void EditBibliographyEntry();
    void DeleteBibliographyEntry();
    void OpenBibliographyEntry();
    void OpenAuthorsEditors();
    void on_BibliographyDatabaseToggle_clicked(bool checked);
    void on_NewBibEntry_CurrentFile_clicked();
    void on_NewBibEntry_CurrentDocument_clicked();
    int TreeItemIndex(QModelIndex index);
    void CreateCustomTagWidget();
    void LoadCountCombo(int index);
    void ReorderFiles(QTableView * table,QMap<int,QString> exerciseOrder);
    void readSettings();
    void writeSettings();
    void RestoreDB(int dbtype,QSqlDatabase database);
    void encrFileDB_Dialog(QStringList databases,bool loadSingleDatabase);
    QString FileCount(QSqlDatabase database, ExtendedTableWidget *table);


//    QList<QTableWidgetItem *> takeRow(QTableView *table, int row);
//    void setRow(QTableView *table, int row, const QList<QTableWidgetItem*>& rowItems);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event) override;

public slots:
//    static void BuildChain(QStringList ListOfCommands);
    static void updateTableView(QTableView * table, QString QueryText, QSqlDatabase Database, QObject *parent);
    static void FilterTables_Queries(QStringList list);
    static void FilterDocuments(QStringList list);
    static void FilterBibliographyTable(QStringList list);
    static void LoadTableHeaders(QTableView * table, QStringList list);
    static void FunctionInProgress();
    static void StretchColumns(QTableView * Table,float stretchFactor);
    static void StretchColumns(QTreeView * Tree,float stretchFactor);
    static void StretchColumnsToWidth(QTableView *table);
    static QStringList GetListWidgetItems(QListWidget * list);
//    static QHash<QString,QString> ReadRow(QTableView * table);
    static void DBBackUp(QString database,QString dest_path);
    static void runQuery_Root(QString queryText, QSqlDatabase database);
    static QString getDataTexPath();
    void initialize(QString dtexFile);
    void onOtherInstanceMessage(const QString &file);
    static bool SelectNewFileInModel(QTableView *table, QString newFile);

signals:
    void dataChanged(QModelIndex index, QModelIndex index2);
};

#endif // DATATEX_H
