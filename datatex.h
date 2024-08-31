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
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QActionGroup>
//#include "pdfium/pdfviewerwidget.h"
#include "qpdfviewer.h"
#include "ExtendedTableWidget.h"
#include "FilterLineEdit.h"
#include "FilterTableHeader.h"
#include "tagslineeditwidget.h"
#include "latexeditorwidget.h"
#include "sqlfunctions.h"
#include "rightclickmenu.h"
#include "graphicsbuilder.h"
#include "tablebuilder.h"
#include "databasecreator.h"
#include "filecommands.h"

#include <QtPdf>
//#include <QtPdfWidgets>

enum struct FileData { Id,FileType,Field,Chapter,Section,ExerciseType,
                Difficulty,Path,Date,Solved,Bibliography,FileContent,
                Preamble,BuildCommand,FileDescription};
enum struct DocumentData { Id,Title,DocumentType,BasicFolder,SubFolder,SubsubFolder,Path,
                    Date,Content,Preamble,BuildCommand,NeedsUpdate,Bibliography,
                    UseBibliography,FilesInDocument,BibEntries,CustomTags};
enum struct BibliographyData {
                CitationKey,BibType,Title,Content,Authors,Editors,Translators,Publisher,Year,Month,ISBN,
                ISSN,Pages,Series,Edition,Chapter,Number,Volume,Journal,Institution,School,Issue,Address,DOI,URL,
                Language,Location,Subtitle,Organization,BibKey,Abstract,CrossReference,Note
};
enum struct TableData {Id,TableType,Path,Date,Content,Preamble,BuildCommand,Caption,Description};
enum struct FigureData {Id,FigureType,PlotType,Path,Date,Content,Preamble,BuildCommand,Caption,Description};
enum struct CommandData {Id,Type,Path,Date,Content,Preamble,BuildCommand,Description};
enum struct PreambleData {Id,Name,Class,Path,Date,Content,BuildCommand,Description};
enum struct PackageData {Id,Name,Topic,Path,Date,Content,Description};
enum struct ClassData {Id,Type,Path,Date,Content,Description};

// using namespace QtGraphs;

QT_BEGIN_NAMESPACE
namespace Ui { class DataTex; }
QT_END_NAMESPACE

struct DTXDocument;
struct DTXBuildCommand;
class QTableView;

// class DTXDashBoard {
//     //SELECT Type,count(Type) FROM DataBases GROUP BY Type
// public:
//     QChartView * ShowPieChart(QWidget *parent, QList<QStringList> info);
// };

class DataTex : public QMainWindow
{
    Q_OBJECT

public:
    DataTex(QWidget *parent = nullptr);
    ~DataTex();

    static QSqlDatabase DataTeX_Settings;
    static QSqlDatabase Bibliography_Settings;


    static DTXDatabase CurrentFilesDataBase;
    static DTXDatabase CurrentDocumentsDataBase;
    static DTXDatabase CurrentBibliographyDataBase;
    static DTXDatabase CurrentTablesDataBase;
    static DTXDatabase CurrentFiguresDataBase;
    static DTXDatabase CurrentCommandsDataBase;
    static DTXDatabase CurrentPreamblesDataBase;
    static DTXDatabase CurrentPackagesDataBase;
    static DTXDatabase CurrentClassesDataBase;
    static DTXDatabase CurrentDTXDataBase;

    static QHash<QString,DTXDatabase> GlobalDatabaseList;

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
    static QHash<QString,QString> BuildCommands;
    static QHash<int,DTXBuildCommand> DTXBuildCommands;
//    static QHash<QString,QStringList> LatexCommandsArguments;
    static QString GlobalSaveLocation;
    static QString TexLivePath;
//    static QHash<QString,QStringList> Optional_Metadata_Ids;
//    static QHash<QString,QStringList> Optional_Metadata_Names;
    static QHash<QString,QStringList> Optional_DocMetadata_Ids;
    static QHash<QString,QStringList> Optional_DocMetadata_Names;
    static QTranslator translator;
    static QString currentlanguage;
    static QString datatexpath;
    static QStringList SVG_IconPaths;

private:
    Ui::DataTex *ui;
    ExtendedTableWidget * FilesTable;
    QSortFilterProxyModel *FilesProxyModel;
    QSortFilterProxyModel *DocumentsProxyModel;
    QSortFilterProxyModel *BibliographyProxyModel;
    QSortFilterProxyModel *CommandsProxyModel;
    QSortFilterProxyModel *PreamblePackageClassProxyModel;
    QSortFilterProxyModel *TabularsFiguresProxyModel;

    QSqlDatabase currentbase;
    QPdfViewer * PdfFileView;
//    QPdfViewer * DocumentView;
    QPdfViewer * FileFromDocumentView;
    QPdfViewer * BibFileView;
//    QPdfViewer * BibFileView_2;
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
//    QString FileDescription;
    QHash<QString,bool> isDBEncrypted;
    QHash<QString,QStringList> encryptionData;
    int bibliography_db_enc;
    int commands_db_enc;
    int preamble_db_enc;
    int tabularfigure_db_enc;

    QString DocumentsTable_UpdateQuery;
//    QString FileTypeId;

    int IconSize;

    QMenuBar * MenuBarWidget;

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
    QMenu * ConvertMenu;
    QMenu * LatexFileActions;
//    QMenu * DocumentActions;
//--------------- Toolbars ----------------------
    QToolBar * DatabaseToolBar;
    QToolBar * FileToolBar;
    QToolBar * DocTools_ToolBar;
    QToolBar * BibliographyToolBar;
    QToolBar * TabularToolBar;
    QToolBar * FigureToolBar;
    QToolBar * CommandToolBar;
    QToolBar * PreambleToolBar;
    QToolBar * PackageToolBar;
    QToolBar * ClassToolBar;

    QToolBar * SettingsToolBar;
    QToolBar * CompileBar;
    QToolBar * ConvertBar;
//    QToolBar * CompileBarDoc;
//--------------- Actions ------------------------
    // ------ Database actions ---------
    QAction * NewDatabasefile;
    QAction * OpenDatabasefile;
    QAction * CloseDatabasefile;
    QAction * SyncDatabasefile;
    QAction * SaveAsDatabasefile;
    QAction * ImportDatabaseFrom;
    QAction * ExportDatabaseTo;
    QAction * EncryptDB;
    QAction * ConnectDatabase;
    QAction * DisconnectDatabase;
    QAction * ConnectAllDatabase;
    QAction * DisconnectAllDatabase;
    QAction * CloseApp;

    //---- Files actions -------
    QAction * NewLatexFile;
    QAction * ImportTableFromTablesDB;//Μονο το σύνδεσμο - link
    QAction * ImportFigureFromFiguresDB;//Μονο το σύνδεσμο - link
    QAction * OpenLatexFile;
    QAction * DeleteLatexFile;
    QAction * EditLatexFile;
    QAction * SolveLatexFile;
    QAction * FileCloneToOtherDB;
    QAction * FileEditHistory;
    QAction * ManageFileBibliography;

    // -------- Document actions ---------
    QAction * NewDocument;
    QAction * InsertFileToDocument;
    QAction * AddDocument;
    QAction * DeleteDocument;
    QAction * CreateSolutionsDoc;
    QAction * EditDocument;
    QAction * UpdateDocContent;
    QAction * CloneDocument;
    QAction * DocEditHistory;
    QAction * ManageDocumentBibliography;

    // -------- Bibliography actions ---------
    QAction * NewBibEntry;
    QAction * EditBibEntry;
    QAction * AuthorsEditors;
    QAction * OpenBibEntry;
    QAction * DeleteBibEntry;

    // -------- Table actions ---------
    QAction * NewTableEntry;
    QAction * OpenTableEntry;
    QAction * EditTableEntry;
    QAction * DeleteTableEntry;
    QAction * ExportTableToFilesDB;//Μονο το σύνδεσμο - link

    // -------- Figure actions ---------
    QAction * NewFigureEntry;
    QAction * OpenFigureEntry;
    QAction * EditFigureEntry;
    QAction * DeleteFigureEntry;
    QAction * ExportFigureToFilesDB;//Μονο το σύνδεσμο - link

    // -------- Command actions ---------
    QAction * NewCommandEntry;
    QAction * OpenCommandEntry;
    QAction * EditCommandEntry;
    QAction * DeleteCommandEntry;

    // -------- Preamble actions ---------
    QAction * NewPreambleEntry;
    QAction * OpenPreambleEntry;
    QAction * InsertStuffToPreamble;//Commands, Packages
    QAction * EditPreambleEntry;
    QAction * DeletePreambleEntry;

    // -------- Package actions ---------
    QAction * NewPackageEntry;
    QAction * OpenPackageEntry;
    QAction * InsertStuffToPackage;//Commands
    QAction * EditPackageEntry;
    QAction * DeletePackageEntry;

    // -------- Class actions ---------
    QAction * NewClassEntry;
    QAction * OpenClassEntry;
    QAction * InsertStuffToClass;
    QAction * EditClassEntry;
    QAction * DeleteClassEntry;

    // -------- Settings actions ----------
    QAction * NewPreamble;
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

    // -------- Compile actions ---------
    QAction * Latex;
    QAction * PdfLatex;
    QAction * XeLatex;
    QAction * LuaLatex;
    QAction * PythonTex;
    QAction * Asymptote;
    QAction * Metapost;
    QAction * KnitR;
    QAction * Lilypond;
    QAction * BibTex;
    QAction * Biber;
    QAction * Latexmk;
    QAction * MakeGlossaries;
    QAction * MakeIndex;
    QAction * Xindy;
    QAction * Xindex;
    //---- Convert actions -------
    QAction * DVIPS;
    QAction * DVI2PDF;
    QAction * DVI2PNG;
    QAction * PS2PDF;

    QAction * OpenPath;
    QAction * ShowToolBar;
    QAction * ShowSideBar;

// ------------- ToolButtons -------------
    QToolButton * CompileCommands;
    QToolButton * ConvertCommands;

    QComboBox * FilesPreambleCombo;
    QString DocOptionalFields;
    //Latex Files metadata variables---------
    QString DatabaseFileName;
    QString DatabaseFilePath;
    //----------------------------------------
    //Latex Documents metadata variables---------
    QString DocumentFileName;
    QString DocumentTitle;
    QString DocumentFilePath;
    QString DatabaseSource;
    QString DocumentBuildCommand;
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
    QList<QLabel *> valuelabelList;
    QList<QLineEdit *> lineList;
    QList<QPushButton *> editList;
    QList<QHBoxLayout *> hLayoutList;
    QHash<QString,QLineEdit *> Bib_ValueList;
    QHash<QString,QLineEdit *> Bib_ValueList_Opt;
    QFileSystemModel *model;
    QToolButton *tb;
    QStringList stringList;
    QString CloneSolution;
    TagsFilterWidget * filesTagLine;
    TagsFilterWidget * docsTagLine;
    RightClickMenu * RightClick;
//    RightClickMenu * DocRightClick;
//    RightClickMenu * BibRightClick;
    bool filesSorting;
    bool docsSorting;
    bool bibSorting;
    QSqlQueryModel * FilesModel;
//    QSqlQueryModel * DocsModel;
//    QSqlQueryModel * BibModel;
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
    void CreateNewDocument(DTXDocument docInfo);
    void DataBaseFields();
    void CreateSolutionsDocument();
    void DataTeX_Preferences();
    void DatabaseSyncFiles();
    void EditFileMeta();
    void EditDocumentDialog();
    void BackUp_DataBase_Folders();
    void setDefaultAction(QAction * action);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index,const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

    void FilesTable_selectionchanged(int DatabaseType);
    void DocumentsTable_selectionChanged();
    void BibliographyTable_selectionChanged();
//    QString SetLatexCommand(QAction * Action, DTXBuildCommand Command);
    void CreateDatabase();
    void CreateNewDatabase(DTXDatabase DTXDB);
    void UpdateCurrentDatabase(DTXDatabase DTXDB);
    void AddDatabaseToTree(DTXDatabase DTXDB);
    void DeleteFileFromBase();
    void DeleteDocumentFromBase();
    void Preamble_clicked();
    void AddPreamble(QStringList preamble);
    void on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
//    QString GetLatexCommand(QString SQLCommandSetting, QAction *Action, QStringList args, QString ext);
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
    void on_addBibEntry_clicked();
    void on_addDocBibEntry_clicked();
    void FileClone();
//    void SelectNewFileInModel(QTableView *table);

    void NewBibliographyEntry();
    void EditBibliographyEntry();
    void DeleteBibliographyEntry();
    void OpenBibliographyEntry();
    void OpenAuthorsEditors();
    void on_NewBibEntry_CurrentFile_clicked();
    void on_NewBibEntry_CurrentDocument_clicked();
    int TreeItemIndex(QModelIndex index);
    void CreateCustomTagWidget();
    void LoadCountCombo(int index);
    void ReorderFiles(QTableView * table,QMap<int,QString> exerciseOrder);
    void readSettings();
    void writeSettings();
    void RestoreDB(int dbtype,QSqlDatabase database);
    void encrFileDB_Dialog(QList<DTXDatabase> databases);
    QString FileCount(QSqlDatabase database, ExtendedTableWidget *table);
    void changeIcon(bool checked, QString unchecked_icon, QString checked_icon);


//    QList<QTableWidgetItem *> takeRow(QTableView *table, int row);
//    void setRow(QTableView *table, int row, const QList<QTableWidgetItem*>& rowItems);
    void on_CloseButton_clicked();
    void on_MaximizeButton_clicked();
    void on_MinimizeButton_clicked();
    void SetDatabaseConnected(QTreeWidgetItem *item);
    void SetDatabaseDisconnected(QTreeWidgetItem * item);
    void AreAllDBConnected();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    virtual void changeEvent(QEvent *e) override;
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
    void sendDBConnectionInfo(int DBConnected);
};

#endif // DATATEX_H
