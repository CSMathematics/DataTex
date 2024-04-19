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
#include "qpdfviewer.h"  //pdf预览窗口
#include "ExtendedTableWidget.h" //扩展表组件？
#include "FilterLineEdit.h"
#include "FilterTableHeader.h"
#include "tagslineeditwidget.h"
#include "latexeditorwidget.h" //latex 编辑插件
#include "filecommands.h" //文件命令
#include "sqlfunctions.h"  // sql函数
#include "rightclickmenu.h"  // 右键菜单
#include "graphicsbuilder.h" //创建图片
#include "tablebuilder.h"  //创建表格

#include <QtPdf>
//#include <QtPdfWidgets>

//创建基文件和文档的数据结构
enum FileData { Id,FileType,Field,Chapter,Section,ExerciseType,
                Difficulty,Path,Date,Solved,Bibliography,FileContent,
                Preamble,BuildCommand,FileDescription};

enum DocData { DocId,DocumentType,BasicFolder,SubFolder,SubsubFolder,DocPath,
                DocDate,DocContent,DocPreamble,DocBuildCommand,NeedsUpdate,DocBibliography,
                UseBibliography,FilesInDocument,BibEntries,CustomTags};

//enum NewFileMode {
//    BasicMode,ImportMode,EditMode,CloneModeOnlyContent,CloneModeContentAndMetadata
//};


//将DataTex的UI图放到Ui命名空间中
QT_BEGIN_NAMESPACE
namespace Ui { class DataTex; }
QT_END_NAMESPACE



class QTableView;

class DataTex : public QMainWindow
{
    Q_OBJECT //继承Qt的信号与槽

public:
    DataTex(QWidget *parent = nullptr);
    ~DataTex();

    static QSqlDatabase DataTeX_Settings; //软件设置
    static QSqlDatabase Bibliography_Settings; //参考文献设置
    static QSqlDatabase CurrentTexFilesDataBase; // 当前tex文件数据库
    static QSqlDatabase CurrentDocumentDataBase; // 当前文档数据库
    static QString CurrentDataBasePath; // 当前数据库路径
    static QString CurrentDataBase_basename; // 当前数据库名字
    static QString CurrentDocumentDatabasePath; //当前文档数据库路径
    static QString CurrentDocumentDatabase_basename; //当前文档数据库名字
    static QHash<QString,QSqlDatabase> GlobalFilesDatabaseList; //全局文件数据库列表
    static QHash<QString,QSqlDatabase> GlobalDocsDatabaseList;
    static QHash<QString,QString> GlobalFilesDatabaseListNames;
    static QHash<QString,QString> GlobalDocsDatabaseListNames;
    static QString CurrentPreamble;//序言
    static QString CurrentPreamble_Content;//序言内容
    static QStringList DocTypesIds;//文档类型序号
    static QStringList DocTypesNames;//文档类型名
    static QString PdfLatex_Command;//pdflatex命令
    static QString Latex_Command;//latex命令
    static QString XeLatex_Command;
    static QString LuaLatex_Command;
    static QString Pythontex_Command;
    static QString Bibtex_Command;
    static QString Asymptote_Command;//asymptote绘图命令，赞！！
    static QString RunCommand;//自定义命令？
    static QHash<QString,QString> LatexCommands;
    static QHash<QString,QStringList> LatexCommandsArguments;
    static QString GlobalSaveLocation;//全局保存路径
    static QString TexLivePath;//texlive 路径
    static QHash<QString,QStringList> Optional_Metadata_Ids;
    static QHash<QString,QStringList> Optional_Metadata_Names;
    static QHash<QString,QStringList> Optional_DocMetadata_Ids;
    static QHash<QString,QStringList> Optional_DocMetadata_Names;
    static QTranslator translator;
    static QString currentlanguage;//当前语言
    static QString datatexpath;//datatex路径
    static QStringList SVG_IconPaths;//svg图标路径


private:
    Ui::DataTex *ui;
    ExtendedTableWidget * FilesTable;
    ExtendedTableWidget * DocumentsTable;
    ExtendedTableWidget * BibliographyTable;
    ExtendedTableWidget * CommandsTable;
    ExtendedTableWidget * PreamblePackageClassTable;
    ExtendedTableWidget * TabularsFiguresTableWidget;

    //QSortFilterProxyModel对数据模型进行排序和过滤
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
    QHash<QString,bool> isDBEncrypted;//是否加密
    QHash<QString,QStringList> encryptionData;//加密数据
    int fdb_enc;//文件加密
    int ddb_enc;//文档加密
    int bibliography_db_enc;
    int commands_db_enc;
    int preamble_db_enc;
    int tabularfigure_db_enc;

    QString DocumentsTable_UpdateQuery;//更新查询
    QString FileTypeId;
    int Solvable;//可解析？
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
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, const char * Function, QString ShortCut, QIcon Icon, QString Description); // 创建操作命令
    QAction * CreateNewAction(QMenu * Menu, QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description); //const char * Function 有返回值，void无返回值
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
    Qt::ItemFlags flags(const QModelIndex &index) const;//返回与特定模型索引相关联的项的标记。这些标记定义了项的行为，例如它是否可以被选中，是否可以被编辑，等等。
    bool setData(const QModelIndex &index,const QVariant &value, int role);//往某列中写入数据
    QVariant data(const QModelIndex &index, int role) const;//从某列中获取数据
    //点击数据库行为
    void on_FilesDatabaseToggle_clicked(bool checked);
    void on_DocumentsDatabaseToggle_clicked(bool checked);
    //改变选定的文档触发行为
    void FilesTable_selectionchanged();
    void DocumentsTable_selectionChanged();
    void BibliographyTable_selectionChanged();

    // latex命令读写
    QString SetLatexCommand(QString SQLCommandSetting, QString Command, QAction * Action, QAction * Action2 , QStringList args, QString ext);
    QString GetLatexCommand(QString SQLCommandSetting, QAction *Action, QAction *Action2, QStringList args, QString ext);

    //增删改查
    void CreateDatabase();
    void CreateNewDatabase(QString Path, QString FolderName, QString fileName, QString DatabaseType);
    void UpdateCurrentDatabase(QString FullPath);
    void UpdateCurrentNotesDatabase(QString FullPath);
    void AddDatabaseToTree(int row, QString databasePath, QString databaseName);
    void DeleteFileFromBase();
    void DeleteDocumentFromBase();


    void Preamble_clicked();
    void AddPreamble(QStringList preamble);
    void setPreamble();

    //点击数据库展开列表
    void on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_DatabaseStructureTreeView_clicked(const QModelIndex &index);
    void on_DatabaseStructureTreeView_doubleClicked(const QModelIndex &index);

    //编译行为
    void CompileToPdf();
    void Compile();
    void CompileAsymptote();

    void TeXFilesTable_selection_changed();
    void loadDatabaseFields();
    void updateFilter(QStringList values);
    void getActionFromText(QMenu *menu, QToolButton *button);



    void on_ComboCount_currentIndexChanged(int index);
    void OpenLoadDatabase();
    void OpenDatabaseInfo(QString filePath, QString FolderName);
    void RemoveCurrentDatabase();
    void UpdateDocument();
    void CloneCurrentDocument();
    void AddFileToDatabase();
    void AddDocToDatabase();

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

    //从sql文件中恢复数据库
    void RestoreDB(int dbtype,QSqlDatabase database);

    void encrFileDB_Dialog(QStringList databases,bool loadSingleDatabase);
    QString FileCount(QSqlDatabase database, ExtendedTableWidget *table);


//    QList<QTableWidgetItem *> takeRow(QTableView *table, int row);
//    void setRow(QTableView *table, int row, const QList<QTableWidgetItem*>& rowItems);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    // virtual void changeEvent(QEvent *e);
    virtual void changeEvent(QEvent *e) override; //武修改，去除警告
    void closeEvent(QCloseEvent *event) override;

public slots:
//    static void BuildChain(QStringList ListOfCommands);
    static void updateTableView(QTableView * table, QString QueryText, QSqlDatabase Database, QObject *parent);
    static void FilterTables_Queries(QStringList list);
    static void FilterDocuments(QStringList list);
    static void FilterBibliographyTable(QStringList list);
    static void LoadTableHeaders(QTableView * table, QStringList list);
    static void FunctionInProgress();
    //拉伸列表
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
