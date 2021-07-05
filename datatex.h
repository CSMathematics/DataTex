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
    static QString CurrentPreamble;
    static QString CurrentPreamble_Content;
    static QString CurrentPdfLatexBuildCommand;

    static QString PdfLatex_Command;
    static QString Latex_Command;
    static QString XeLatex_Command;
    static QString LuaLatex_Command;
    static QString Pythontex_Command;
    static QString Bibtex_Command;
    static QString Asymptote_Command;
    static QString RunCommand;
    static QHash<QString,QString> LatexCommandsValues;

private:
    Ui::DataTex *ui;
    QString datatexpath;
    QSqlDatabase currentbase;
    PdfViewer * LatexFileView;
    PdfViewer * DocumentView;
    PdfViewer * FileFromDocumentView;
    QStringList LatexCommands;
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
    QAction * SaveDatabasefile;
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
    QAction * CreateBibliography;
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
    //----------------------------------------
//    QList<QLineEdit *> LineEditList;
    QStringList Exer_List;
    QStringList Solutions_List;
    QStringList Solved_List;
    QStringList Unsolved_List;
    QHash<QString,QStringList> SolutionsPerExercise;

    QStringList Optional_Metadata_Ids;
    QStringList Optional_Metadata_Names;
    QList<QLabel *> labelList;
    QList<QLineEdit *> lineList;
    QList<QHBoxLayout *> hLayoutList;

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
    void EditNewBaseFile(QString fileName,QMap<QString,QString> metapairs,QStringList SectionList);
    void SaveContentToDatabase(QString fileName, QString content);
    void SolutionFile();
    void InsertFiles();
    void AddFilesToEditor(QStringList files);
    void EditDataBase();
    void PersonalNotes();
    void CreateNewSheet(QString fileName);
    void DataBaseFields();
    void CreateSolutionsDocument();
    void DataTeX_Preferences();
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
    void CreateNewDatabase(QString path,QString FolderName,QString fileName);
    void UpdateCurrentDatabase(QString fileName);
    void UpdateCurrentNotesDatabase(QString fileName);
    void AddDatabaseToTree(int row,QString databasePath);
    void DeleteFileFromBase();

    void on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    QString GetLatexCommand(QString SQLCommandSetting, QAction *Action, QAction *Action2, QStringList args, QString ext);
    void CompileToPdf();
    void Compile();
    void CompileAsymptote();
    void FileEdit_Changed();
    void TeXFilesTable_selection_changed();
    void loadDatabaseFields();


public slots:
    static void CreateTexFile(QString fullFilePath);
    static void BuildDocument(QString CompileCommand, QString fullFilePath, QStringList args, QString ext);
    static void ClearOldFiles(QString fullFilePath);
    static void loadImageFile(QString exoFile, PdfViewer * view);
//    static void BuildChain(QStringList ListOfCommands);
};

#endif // DATATEX_H
