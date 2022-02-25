#include "datatex.h"
#include "ui_datatex.h"
#include <QDesktopWidget>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QModelIndex>
#include <QGuiApplication>
#include "sqlfunctions.h"
#include "newdatabasefile.h"
#include "paths.h"
#include "solvedatabaseexercise.h"
#include "basefolder.h"
#include "sqlfunctions.h"
#include "notesdocuments.h"
#include "addfiletoeditor.h"
#include "datatables.h"
#include "backup.h"
#include "solutionsdocument.h"
#include <poppler-qt5.h>
#include "pdfviewer.h"
#include "databasesync.h"
#include "addline.h"
#include "highlighter.h"
#include "updatedocumentcontent.h"




QSqlDatabase DataTex::DataTeX_Settings = QSqlDatabase::addDatabase("QSQLITE","Settings");
QString DataTex::CurrentDataBasePath;
QString DataTex::CurrentNotesFolderPath;
QSqlDatabase DataTex::CurrentTexFilesDataBase;
QSqlDatabase DataTex::CurrentNotesFolderDataBase;
QHash<QString,QSqlDatabase> DataTex::GlobalFilesDatabaseList;
QHash<QString,QSqlDatabase> DataTex::GlobalDocsDatabaseList;
QHash<QString,QString> DataTex::GlobalFilesDatabaseListNames;
QHash<QString,QString> DataTex::GlobalDocsDatabaseListNames;
QString DataTex::CurrentPreamble;
QString DataTex::CurrentPreamble_Content;
QString DataTex::PdfLatex_Command;
QString DataTex::Latex_Command;
QString DataTex::XeLatex_Command;
QString DataTex::LuaLatex_Command;
QString DataTex::Pythontex_Command;
QString DataTex::Bibtex_Command;
QString DataTex::Asymptote_Command;
QString DataTex::RunCommand;
QString DataTex::TexLivePath;
QString DataTex::GlobalSaveLocation;
QHash<QString,QString> DataTex::LatexCommands;
QHash<QString,QStringList> DataTex::LatexCommandsArguments;

DataTex::DataTex(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataTex)
{
    ui->setupUi(this);
    QScreen * screen = QGuiApplication::primaryScreen();
    QRect  screenRect = screen->geometry();
    resize(0.9*screenRect.width(),0.9*screenRect.height());
    move(QPoint(0.05*screenRect.width(),0.05*screenRect.height()));
    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_20->addWidget(FilesTable,1,0,1,3);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    DocumentsTable = new ExtendedTableWidget(this);
    ui->verticalLayout_8->addWidget(DocumentsTable);
    DocumentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentsTable->horizontalHeader()->setSectionsClickable(true);

    CreateMenus_Actions();
    CreateToolBars();
    CreateBuildCommands();
    ui->FilesDatabaseToggle->setChecked(true);
    on_FilesDatabaseToggle_clicked(true);//Set Latex file database page as default
    SettingsDatabase_Variables();//Database names and variables, Latex commands
    //------Load databases and add them to the treewidget
    QStringList FilesDatabases = SqlFunctions::Get_StringList_From_Query("SELECT \"Path\" FROM \"DataBases\"",DataTeX_Settings);
    QStringList FilesDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"DataBases\"",DataTeX_Settings);
    for (int i=0;i<FilesDatabases.count();i++ ) {
        AddDatabaseToTree(0,FilesDatabases.at(i),FilesDatabasesNames.at(i));
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FilesDatabases[i]).baseName());
        database.setDatabaseName(FilesDatabases[i]);
        GlobalFilesDatabaseList.insert(QFileInfo(FilesDatabases[i]).baseName(),database);
        GlobalFilesDatabaseListNames.insert(QFileInfo(FilesDatabases[i]).baseName(),FilesDatabasesNames[i]);
        database.open();
    }
    QStringList DocumentDatabases = SqlFunctions::Get_StringList_From_Query("SELECT \"Path\" FROM \"Notes_Folders\"",DataTeX_Settings);
    QStringList DocumentDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"Notes_Folders\"",DataTeX_Settings);
    for (int i=0;i<DocumentDatabases.count();i++ ) {
        AddDatabaseToTree(1,DocumentDatabases.at(i),DocumentDatabasesNames.at(i));
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(DocumentDatabases[i]).baseName());
        database.setDatabaseName(DocumentDatabases[i]);
        GlobalDocsDatabaseList.insert(QFileInfo(DocumentDatabases[i]).baseName(),database);
        GlobalDocsDatabaseListNames.insert(QFileInfo(DocumentDatabases[i]).baseName(),DocumentDatabasesNames[i]);
        database.open();
    }
    //-------------------------------------------------
    QString filesdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentDataBase);
    QString docsdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentNotesFolderBase);
    CurrentTexFilesDataBase = QSqlDatabase::database(filesdb);
    CurrentNotesFolderDataBase = QSqlDatabase::database(docsdb);
    CurrentDataBasePath = CurrentTexFilesDataBase.databaseName();
    CurrentNotesFolderPath = CurrentNotesFolderDataBase.databaseName();
    DatabaseStructure(CurrentDataBasePath);// Show database folder structure in tree view

    ui->CurrentBaseLabel->setText("Current files database : "+GlobalFilesDatabaseListNames[QFileInfo(CurrentDataBasePath).baseName()]);
    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",CurrentTexFilesDataBase);
    Database_DocTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",CurrentNotesFolderDataBase);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",CurrentNotesFolderDataBase);
    FilterTables_Queries(Database_FileTableFields);
    FilterDocuments(Database_DocumentTableColumns);
    ShowDataBaseFiles();//Load and preview latex files database table
    ShowDocuments();//Load and preview documents database table

    //---Add Pdf widgets to main layout----
    LatexFileView = new PdfViewer(this);
    ui->gridLayout_2->addWidget(LatexFileView);
    LatexFileView->show();

    DocumentView = new PdfViewer(this);
    ui->gridLayout_13->addWidget(DocumentView);
    DocumentView->show();

    FileFromDocumentView = new PdfViewer(this);
    ui->gridLayout_7->addWidget(FileFromDocumentView);
    FileFromDocumentView->show();
    //-------------------------------

    // ----- ui settings -------
    CompileMenu->setEnabled(false);
    PdfLatex->setEnabled(false);
    DeleteLatexFile->setEnabled(false);
    EditLatexFile->setEnabled(false);
    OpenPath->setEnabled(false);
    SaveTex->setEnabled(false);
    SaveTexDoc->setEnabled(false);
    DeleteDocument->setEnabled(false);
    ui->addBibEntry->setEnabled(false);
    ui->removeBibEnrty->setEnabled(false);
    connect(ui->FileEdit, &QTextEdit::textChanged, this, [=](){
        SaveTex->setEnabled(true);
        UndoTex->setEnabled(true);
    });
    connect(ui->DocumentContent, &QTextEdit::textChanged, this, [=](){
        SaveTexDoc->setEnabled(true);
        UndoTexDoc->setEnabled(true);
    });
//    ui->OpenDatabasesTreeWidget->setColumnHidden(1,true);
//    ui->OpenDatabasesTreeWidget->setColumnHidden(2,true);
    ui->OpenDatabasesTreeWidget->resizeColumnToContents(0);
    ui->OpenDatabasesTreeWidget->expandAll();
    ui->splitter_3->setSizes(QList<int>({400, 1}));
    ui->splitter_2->setSizes(QList<int>({400, 1}));
    ui->splitter_4->setSizes(QList<int>({250, 150}));
    ui->splitter_5->setSizes(QList<int>({500, 1}));
    ui->splitter_6->setSizes(QList<int>({250, 150}));
    ui->splitter_7->setSizes(QList<int>({50, 400}));
    ui->splitter_8->setStretchFactor(1, 3);

    //---- Load preambles -----------------------
    FilesPreambleCombo = new QComboBox(this);
    DocumentsPreambleCombo = new QComboBox(this);
    QSqlQuery LoadPreambles(DataTeX_Settings);
    LoadPreambles.exec("SELECT \"Id\",\"Name\",\"Preamble_Content\" FROM \"Preambles\";");
    while(LoadPreambles.next()){
        FilesPreambleCombo->addItem(LoadPreambles.value(1).toString(),QVariant(LoadPreambles.value(0).toString()));
        DocumentsPreambleCombo->addItem(LoadPreambles.value(1).toString(),QVariant(LoadPreambles.value(0).toString()));
    }
    FilesPreambleCombo->setCurrentIndex(-1);
    DocumentsPreambleCombo->setCurrentIndex(-1);
    FilesPreambleCombo->setEnabled(false);
    DocumentsPreambleCombo->setEnabled(false);
    ui->FileContentCommandsHorizontalLayout->addWidget(FilesPreambleCombo);
    ui->horizontalLayout_45->addWidget(DocumentsPreambleCombo);

    loadDatabaseFields();

    ui->ComboCount->addItem(tr("Field"),QVariant(SqlFunctions::CountFiles_by_Field));
    ui->ComboCount->addItem(tr("Chapter"),QVariant(SqlFunctions::CountFiles_by_Chapter));
    ui->ComboCount->addItem(tr("Section"),QVariant(SqlFunctions::CountFiles_by_Section));
    ui->ComboCount->addItem(tr("Exercise type"),QVariant(SqlFunctions::CountFiles_by_ExerciseType));
    ui->ComboCount->addItem(tr("File type"),QVariant(SqlFunctions::CountFiles_by_FileType));

    if(Optional_Metadata_Ids.count()>0){
        MetadataToolButton();
    }
    QToolButton *dtb = new QToolButton();
    dtb->setCheckable(true);
    dtb->setIcon(QIcon(":/images/document-edit.svg"));
    ui->tabWidget_2->tabBar()->setTabButton(1, QTabBar::RightSide, dtb);
    connect(dtb,&QToolButton::toggled,this,[=](bool checked){
        if(!checked){
            dtb->setIcon(QIcon(":/images/document-edit.svg"));
            for (int i=0;i<Doc_lineList.count();i++) {
                Doc_lineList.at(i)->setReadOnly(true);
                Doc_labelList.at(i)->setText(Doc_labelList.at(i)->text().remove("*"));
                Doc_labelList.at(i)->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
            }
        }
        else{
            dtb->setIcon(QIcon(":/images/document-save.svg"));
            for (int i=0;i<Doc_lineList.count();i++) {
                Doc_lineList.at(i)->setReadOnly(false);
                Doc_labelList.at(i)->setText(Doc_labelList.at(i)->text()+"*");
            }
        }
    });

    ui->DocumentTypeCombo->setEnabled(false);
    QToolButton *bibtb = new QToolButton();
    bibtb->setCheckable(true);
    bibtb->setIcon(QIcon(":/images/document-edit.svg"));
    ui->tabWidget->tabBar()->setTabButton(1, QTabBar::RightSide, bibtb);
    connect(bibtb,&QToolButton::toggled,this,[=](bool checked){
        if(!checked){
            ui->DocumentTypeCombo->setEnabled(false);
            bibtb->setIcon(QIcon(":/images/document-edit.svg"));
            for (int i=0;i<Bib_lineList.count();i++) {
                Bib_lineList.at(i)->setReadOnly(true);
                Bib_labelList.at(i)->setText(Bib_labelList.at(i)->text().remove("*"));
                Bib_labelList.at(i)->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
                QString BibSource;
                for (int i=0;i<ui->BibEntriesCombo->count();i++) {
                    if(ui->BibEntriesCombo->itemData(i).toInt()==1){
                        BibSource += BibSourceCode(i)+"\n\n";
                    }
                }
                QSqlQuery bibContent(CurrentTexFilesDataBase);
                bibContent.exec(QString("UPDATE Database_Files SET Bibliography = \"%2\" WHERE Id = \"%1\"").arg(DatabaseFileName,BibSource));

                QString query ="UPDATE \"Bibliography\" SET ";
                QStringList list;
                list.append("\"Document_Type\"=\""+ui->DocumentTypeCombo->currentData().toString()+"\"");
                for (int i=0;i<Bib_lineList.count();i++) {
                    list.append("\""+BibFieldIds[i+2]+"\""+"="+"\""+Bib_lineList[i]->text()+"\"");
                }
                query += list.join(",")+" WHERE \"Citation_Key\"=\""+ui->BibEntriesCombo->currentText()+"\"";
                bibContent.exec(query);
            }
        }
        else{
            ui->DocumentTypeCombo->setEnabled(true);
            bibtb->setIcon(QIcon(":/images/document-save.svg"));
            for (int i=0;i<Bib_lineList.count();i++) {
                Bib_lineList.at(i)->setReadOnly(false);
                Bib_labelList.at(i)->setText(Bib_labelList.at(i)->text()+"*");
            }
        }
    });

    FilesTable->setColumnHidden(Database_FileTableFieldNames.count(),true);
    FilesTable->setColumnHidden(Database_FileTableFieldNames.count()+1,true);
    FilesTable->setColumnHidden(Database_FileTableFieldNames.count()-Optional_Metadata_Ids.count()-1,true);


    connect(ui->tabWidget_2,&QTabWidget::currentChanged,this,[=](int i){
       dtb->setEnabled(i==1);
    });
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[=](int i){
       bibtb->setEnabled(i==1);
    });

    QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = \"SaveLocation\"",DataTeX_Settings);
    GlobalSaveLocation = (list.size()>0 && !list[0].isEmpty()) ? list[0] : QDir::homePath();

    connect(ui->UseEntry,&QCheckBox::clicked,this,[=](bool checked){
        QSqlQuery UseBib(CurrentTexFilesDataBase);
        UseBib.exec(QString("UPDATE \"Bibliography\" SET \"UseBibliography\" = \"%1\" WHERE \"Citation_Key\" = \""+ui->BibEntriesCombo->currentText()+"\"")
                      .arg(checked));
        ui->BibEntriesCombo->setItemData(ui->BibEntriesCombo->currentIndex(),QString::number(checked));
    });
    CloseDatabasefile->setEnabled(false);

    QProcess *process = new QProcess;
    process->start("which",QStringList()<<"tlmgr");
    process->waitForBytesWritten();
    process->waitForFinished(-1);
    TexLivePath = QString(process->readAllStandardOutput()).remove("tlmgr\n");
    connect(ui->UseBibliography,&QCheckBox::clicked,this,[=](bool checked){
        int i = (checked) ? 1 : 0;
        QSqlQuery needsUpdate(CurrentNotesFolderDataBase);
        needsUpdate.exec(QString("UPDATE Documents SET Bibliography = \"%1\" WHERE Id = \"%2\"").arg(QString::number(i),DocumentFileName));
    });
    ui->SaveDocBibContent->setEnabled(false);
    connect(ui->DocBibSourceCode,&QTextEdit::textChanged,this,[=](){
        ui->SaveDocBibContent->setEnabled(true);
//        UndoTex->setEnabled(false);
    });
    connect(ui->SaveDocBibContent,&QPushButton::clicked,this,[=](){
        QString BibContent = ui->DocBibSourceCode->toPlainText();
        QString BibFile = DocumentFilePath;
        BibFile.replace(".tex",".bib");
        QFile file(BibFile);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream ContentStream(&file);
        QString PreviousContent = ContentStream.readAll();
        file.resize(0);
        ContentStream << BibContent;
        file.close();
        ui->SaveDocBibContent->setEnabled(false);
//        UndoTex->setEnabled(false);
        QSqlQuery needsUpdate(CurrentNotesFolderDataBase);
        needsUpdate.exec(QString("UPDATE Documents SET Bibliography = \"%1\" WHERE Id = \"%2\"").arg(BibContent,DocumentFileName));
    });

    hasBib = (CurrentPreamble_Content.contains("bibtex") || CurrentPreamble_Content.contains("biblatex"));
    QString bibwarning = (hasBib) ? QString() : "The current preamble doesn't\ncontain any bibliography package" ;
    ui->UseBibliography->setEnabled(false);
    ui->BibWarning->setText(bibwarning);

    ui->ClearFiltersFD->setEnabled(false);
    ui->ClearFiltersDD->setEnabled(false);
    connect(ui->ClearFiltersFD, &QPushButton::clicked, this, [=](){
        for (int i=0;i<FilesTable->model()->columnCount();i++) {
            FilesTable->filterHeader()->clearFilters();
            ui->ClearFiltersFD->setEnabled(false);
        }
    });
    connect(ui->ClearFiltersDD, &QPushButton::clicked, this, [=](){
        for (int i=0;i<DocumentsTable->model()->columnCount();i++) {
            DocumentsTable->filterHeader()->clearFilters();
            ui->ClearFiltersDD->setEnabled(false);
        }
    });
}

void DataTex::setDefaultAction(QAction* action)
{
    CompileCommands->setDefaultAction(action);
}

void DataTex::setDefaultActionDoc(QAction* action)
{
    CompileCommandsDoc->setDefaultAction(action);
}

void DataTex::CreateMenus_Actions()
{
    FileMenu = menuBar()->addMenu(tr("File"));
    NewDatabasefile = CreateNewAction(FileMenu,NewDatabasefile,SLOT(CreateDatabase()),"Ctrl+N",QIcon(":/images/database-add.svg"),"&Create a new database");
    OpenDatabasefile = CreateNewAction(FileMenu,OpenDatabasefile,SLOT(OpenLoadDatabase()),"Ctrl+O",QIcon(":/images/database-open.svg"),"&Open a database");
    CloseDatabasefile = CreateNewAction(FileMenu,CloseDatabasefile,SLOT(RemoveCurrentDatabase()),"Ctrl+E",QIcon(":/images/database-delete.svg"),"&Close current database");
    SyncDatabasefile = CreateNewAction(FileMenu,SyncDatabasefile,SLOT(DatabaseSyncFiles()),"Ctrl+S",QIcon(":/images/database-sync.svg"),"&Sync files to database");
    SaveAsDatabasefile = CreateNewAction(FileMenu,SaveAsDatabasefile,SLOT(FunctionInProgress()),"Ctrl+Shift+S",QIcon(":/images/database-save.svg"),"&Save As...");
    QMenu * ExportMenu = new QMenu("Export As...",this);
    ExportAsSQL = CreateNewAction(ExportMenu,ExportAsSQL,SLOT(FunctionInProgress()),"Ctrl+",QIcon(":/images/database-save.svg"),"&Export as Sql");
    FileMenu->addMenu(ExportMenu);

    EditMenu = menuBar()->addMenu(tr("&Edit"));

    ViewMenu = menuBar()->addMenu(tr("&View"));

    ToolMenu = menuBar()->addMenu(tr("&LaTeX Exercises"));
    NewLatexFile = CreateNewAction(ToolMenu,NewLatexFile,SLOT(NewDatabaseBaseFile()),"Ctrl+Shift+N",QIcon(":/images/tex.svg"),"&Create a file");
    OpenLatexFile = CreateNewAction(ToolMenu,OpenLatexFile,SLOT(AddFileToDatabase()),"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),"&Open a file");
    DeleteLatexFile = CreateNewAction(ToolMenu,DeleteLatexFile,SLOT(DeleteFileFromBase()),"Ctrl+Shift+D",QIcon(":/images/edit-delete.svg"),"&Delete current file");
    EditLatexFile = CreateNewAction(ToolMenu,EditLatexFile,SLOT(EditFileMeta()),"Ctrl+Shift+E",QIcon(":/images/document-edit.svg"),"&Edit current file");
    SolveLatexFile = CreateNewAction(ToolMenu,SolveLatexFile,SLOT(SolutionFile()),"Ctrl+Alt+S",QIcon(":/images/solve-tex.svgz"),"&Solve current exersive");
    FileCloneToOtherDB = CreateNewAction(ToolMenu,FileCloneToOtherDB,[=](){
        FileClone(DatabaseFilePath,FileContent);
    },"Ctrl+Alt+C",QIcon(":/images/movefiletodatabase.svgz"),"&Clone this file to another database");
    ToolMenu->addSeparator();
    LatexFileActions = new QMenu("Edit actions",this);
    SaveTex = CreateNewAction(LatexFileActions,SaveTex,SLOT(SaveText()),"Alt+Shift+N",QIcon(":/images/document-save.svg"),"&Save file content");
    UndoTex = CreateNewAction(LatexFileActions,UndoTex,SLOT(FunctionInProgress()),"Ctrl+Z",QIcon(":/images/edit-undo.svg"),"&Undo changes");
    OpenPath = CreateNewAction(LatexFileActions,OpenPath,[=](){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabaseFilePath).absolutePath()));},"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),"&Open folder");
    CopyText = CreateNewAction(LatexFileActions,CopyText,[=](){QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(ui->FileEdit->toPlainText());},"Ctrl+Shift+C",QIcon(":/images/edit-copy.svg"),"&Copy content");
    ShowFileTex = CreateNewAction(LatexFileActions,ShowFileTex,[=](){ui->stackedWidget_2->setCurrentIndex(0);},"Ctrl+Shift+T",QIcon(":/images/latex-config.svg"),"&Show source code");
    ShowFilePdf = CreateNewAction(LatexFileActions,ShowFilePdf,[=](){ui->stackedWidget_2->setCurrentIndex(1);},"Ctrl+Shift+P",QIcon(":/images/pdf2.svg"),"&Show pdf");
    ToolMenu->addMenu(LatexFileActions);

    DocToolMenu = menuBar()->addMenu(tr("&Documents"));
    NewDocument = CreateNewAction(DocToolMenu,NewDocument,SLOT(PersonalNotes()),"Ctrl+Alt+N",QIcon(":/images/pdf.svg"),"&Create a new document");
    InsertFileToDocument = CreateNewAction(DocToolMenu,InsertFileToDocument,[=](){InsertFiles();},"Ctrl+Alt+I",QIcon(":/images/document-import.svg"),"&Insert files to document");
    DeleteDocument = CreateNewAction(DocToolMenu,DeleteDocument,SLOT(DeleteDocumentFromBase()),"Ctrl+Alt+D",QIcon(":/images/document-close-2.svg"),"&Delete current document");
    AddDocument = CreateNewAction(DocToolMenu,AddDocument,SLOT(FunctionInProgress()),"Ctrl+Alt+O",QIcon(":/images/add-button.svg"),"&Add document");
    EditDocument = CreateNewAction(DocToolMenu,EditDocument,SLOT(FunctionInProgress()),"Ctrl+Alt+U",QIcon(":/images/document-edit.svg"),"&Edit document's content and metadata");
    CreateSolutionsDoc = CreateNewAction(DocToolMenu,CreateSolutionsDoc,SLOT(CreateSolutionsDocument()),"Ctrl+Alt+L",QIcon(":/images/solve.svg"),"&Create solution document");
    UpdateDocContent = CreateNewAction(DocToolMenu,UpdateDocContent,SLOT(UpdateDocument()),"Ctrl+Alt+U",QIcon(":/images/update-document.svg"),"&Update document's content");

    DocToolMenu->addSeparator();
    DocumentActions = new QMenu("Edit actions",this);
    DocToolMenu->addMenu(DocumentActions);
    SaveTexDoc = CreateNewAction(DocumentActions,SaveTexDoc,SLOT(SaveDocText()),"Ctrl+Alt+S",QIcon(":/images/document-save.svg"),"&Save document content");
    UndoTexDoc = CreateNewAction(DocumentActions,UndoTexDoc,SLOT(FunctionInProgress()),"Ctrl+Alt+Z",QIcon(":/images/edit-undo.svg"),"&Undo changes");
    OpenPathDoc = CreateNewAction(DocumentActions,OpenPathDoc,[=](){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DocumentFilePath).absolutePath()));},"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),"&Open folder");
    CopyTextDoc = CreateNewAction(DocumentActions,CopyTextDoc,[=](){QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(ui->DocumentContent->toPlainText());},"Ctrl+Shift+C",QIcon(":/images/edit-copy.svg"),"&Copy content");
    ShowDocTex = CreateNewAction(LatexFileActions,ShowDocTex,[=](){ui->stackedWidget_3->setCurrentIndex(0);},"Ctrl+Shift+T",QIcon(":/images/latex-config.svg"),"&Show source code");
    ShowDocPdf = CreateNewAction(LatexFileActions,ShowDocPdf,[=](){ui->stackedWidget_3->setCurrentIndex(1);},"Ctrl+Shift+P",QIcon(":/images/pdf2.svg"),"&Show pdf");

    BibliographyMenu = menuBar()->addMenu(tr("&Bibliography"));

    SettingsMenu = menuBar()->addMenu(tr("&Settings"));
    Preamble = CreateNewAction(SettingsMenu,Preamble,SLOT(Preamble_clicked()),"Alt+P",QIcon(":/images/preamble.svg"),"&Preamble menu");
    Datatables = CreateNewAction(SettingsMenu,Datatables,SLOT(DataBaseFields()),"Alt+D",QIcon(":/images/table.svg"),"&Data entries");
    GeneralSettings = CreateNewAction(SettingsMenu,GeneralSettings,SLOT(DataTeX_Preferences()),"Alt+S",QIcon(":/images/settings.svg"),"&Settings");
    BackUpDatabase = CreateNewAction(SettingsMenu,BackUpDatabase,SLOT(BackUp_DataBase_Folders()),"Alt+B",QIcon(":/images/hdd.svg"),"&Back up database");
    ConnectWithTexEditor = CreateNewAction(SettingsMenu,ConnectWithTexEditor,SLOT(FunctionInProgress()),"Alt+T",QIcon(":/images/connect-with-latex.svg"),"&Send to external TeX editor");

    HelpMenu = menuBar()->addMenu(tr("&Help"));
    Info = CreateNewAction(HelpMenu,Info,[=](){QMessageBox::information(this,tr("Info"),tr("This program is licensed to you under the terms of the\nGNU General Public License Version 3 as \npublished by the Free Software Foundation.\n\n"
                                                                                        "DataTex is using \"Extended Table Widget\" from DB Browser for SQLite\n\nThe software is under development..."),
                                                                    QMessageBox::Ok);},"Alt+I",QIcon(":/images/help-about.svg"),"Help");

}

void DataTex::CreateToolBars()
{
    FileToolBar = addToolBar(tr("File"));
    FileToolBar->setObjectName("FileTools");
    FileToolBar->setIconSize(QSize(27,27));
    FileToolBar->addAction(NewDatabasefile);
    FileToolBar->addAction(OpenDatabasefile);
    FileToolBar->addAction(CloseDatabasefile);
    FileToolBar->addAction(SyncDatabasefile);

    LatexTools_ToolBar = addToolBar(tr("Tools"));
    LatexTools_ToolBar->setObjectName("LatexTools");
    LatexTools_ToolBar->setIconSize(QSize(27,27));
    LatexTools_ToolBar->addAction(NewLatexFile);
    LatexTools_ToolBar->addAction(OpenLatexFile);
    LatexTools_ToolBar->addAction(DeleteLatexFile);
    LatexTools_ToolBar->addAction(EditLatexFile);
    LatexTools_ToolBar->addAction(SolveLatexFile);
    LatexTools_ToolBar->addAction(FileCloneToOtherDB);

    DocTools_ToolBar = addToolBar(tr("Document Tools"));
    DocTools_ToolBar->setObjectName("DocTools");
    DocTools_ToolBar->setIconSize(QSize(27,27));
    DocTools_ToolBar->addAction(NewDocument);
    DocTools_ToolBar->addAction(InsertFileToDocument);
    DocTools_ToolBar->addAction(DeleteDocument);
    DocTools_ToolBar->addAction(AddDocument);
    DocTools_ToolBar->addAction(EditDocument);
    DocTools_ToolBar->addAction(CreateSolutionsDoc);
    DocTools_ToolBar->addAction(UpdateDocContent);

    SettingsToolBar = addToolBar(tr("Settings"));
    SettingsToolBar->setObjectName("SettingsTools");
    SettingsToolBar->setIconSize(QSize(27,27));
    SettingsToolBar->addAction(Preamble);
    SettingsToolBar->addAction(Datatables);
    SettingsToolBar->addAction(GeneralSettings);
    SettingsToolBar->addAction(BackUpDatabase);
    SettingsToolBar->addAction(ConnectWithTexEditor);
}

void DataTex::CreateBuildCommands()
{
//------------ Compile commands ---------------------------
    //------ Compile File Widgets ------------
    CompileCommands = new QToolButton(this);
    CompileCommands->setPopupMode(QToolButton::MenuButtonPopup);
    ui->FileContentCommandsHorizontalLayout->addWidget(CompileCommands);
    CompileMenu = new QMenu(this);
    Latex = CreateNewAction(CompileMenu,Latex,SLOT(NewDatabaseBaseFile()),"Ctrl+Alt+L",QIcon(":/images/latex-config.svg"),"&LaTeX");
    PdfLatex = CreateNewAction(CompileMenu,PdfLatex,[=](){
        CompileToPdf();ClearOldFiles(DatabaseFilePath);loadImageFile(DatabaseFilePath,LatexFileView);
        QSqlQuery UpdateBuildCommand(CurrentTexFilesDataBase);
        UpdateBuildCommand.exec(QString("UPDATE Database_Files SET BuildCommand = \"PdfLaTeX\" WHERE Id = \"%1\"").arg(DatabaseFileName));
        UpdateBuildCommand.exec(QString("UPDATE Database_Files SET Preamble = \"%2\" WHERE Id = \"%1\"").arg(DatabaseFileName,FilesPreambleCombo->currentData().toString()));
//        ShowDataBaseFiles();
//        FilesTable->model()->setData(FilesTable->model()->index(0,0),"s",Qt::EditRole);
//        emit dataChanged(FilesTable->model()->index(0,0),FilesTable->model()->index(0,0));
//        FilesTable->viewport()->update();
    },"Ctrl+Alt+P",QIcon(":/images/pdflatex.svg"),"&PdfLaTeX");
    XeLatex = CreateNewAction(CompileMenu,XeLatex,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon(":/images/xelatex.svg"),"&XeLaTeX");
    LuaLatex = CreateNewAction(CompileMenu,LuaLatex,SLOT(CompileToPdf()),"Ctrl+Shift+L",QIcon(":/images/lualatex.svg"),"&LuaLaTeX");
    PythonTex = CreateNewAction(CompileMenu,PythonTex,SLOT(Compile()),"Ctrl+Shift+P",QIcon(":/images/pythontex.svg"),"&PythonTex");
    BibTex = CreateNewAction(CompileMenu,BibTex,SLOT(Compile()),"Ctrl+Shift+B",QIcon(":/images/address-book-new.svg"),"&BibTeX");
    Asymptote = CreateNewAction(CompileMenu,Asymptote,SLOT(CompileAsymptote()),"Ctrl+Shift+A",QIcon(":/images/Asymptote.svg"),"&Asymptote");
    CompileCommands->setMenu(CompileMenu);
    CompileCommands->setIconSize(QSize(20,20));
    CompileCommands->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CompileCommands->setDefaultAction(PdfLatex);

    CompileBar = new QToolBar(this);
    CompileBar->addAction(SaveTex);
    CompileBar->addAction(UndoTex);
    CompileBar->addAction(OpenPath);
    CompileBar->addAction(CopyText);
    CompileBar->addAction(ShowFileTex);
    CompileBar->addAction(ShowFilePdf);
    ui->FileContentCommandsHorizontalLayout->addWidget(CompileBar);
    CompileBar->setIconSize(QSize(16,16));

    //------ Compile Document Widgets ------------
    CompileCommandsDoc = new QToolButton(this);
    CompileCommandsDoc->setPopupMode(QToolButton::MenuButtonPopup);
    ui->horizontalLayout_45->addWidget(CompileCommandsDoc);

    CompileBarDoc = new QToolBar(this);
    CompileBarDoc->addAction(SaveTexDoc);
    CompileBarDoc->addAction(UndoTexDoc);
    CompileBarDoc->addAction(OpenPathDoc);
    CompileBarDoc->addAction(CopyTextDoc);
    CompileBarDoc->addAction(ShowDocTex);
    CompileBarDoc->addAction(ShowDocPdf);
    ui->horizontalLayout_45->addWidget(CompileBarDoc);
    CompileBarDoc->setIconSize(QSize(16,16));

    CompileMenuDoc = new QMenu(this);
    LatexDoc = CreateNewAction(CompileMenuDoc,LatexDoc,SLOT(NewDatabaseBaseFile()),"Ctrl+Alt+L",QIcon(":/images/latex-config.svg"),"&LaTeX");
    PdfLatexDoc = CreateNewAction(CompileMenuDoc,PdfLatexDoc,[=](){
        CompileToPdf();
        ClearOldFiles(DocumentFilePath);
        loadImageFile(DocumentFilePath,DocumentView);
        QSqlQuery UpdateBuildCommand(CurrentNotesFolderDataBase);
        UpdateBuildCommand.exec(QString("UPDATE Documents SET BuildCommand = \"PdfLaTeX\" WHERE Id = \"%1\"").arg(DocumentFileName));
        UpdateBuildCommand.exec(QString("UPDATE Documents SET Preamble = \"%2\" WHERE Id = \"%1\"").arg(DocumentFileName,DocumentsPreambleCombo->currentData().toString()));
    },"Ctrl+Alt+P",QIcon(":/images/pdflatex.svg"),"&PdfLaTeX");
    XeLatexDoc = CreateNewAction(CompileMenuDoc,XeLatexDoc,SLOT(NewDatabaseBaseFile()),"Ctrl+Alt+X",QIcon(":/images/xelatex.svg"),"&XeLaTeX");
    LuaLatexDoc = CreateNewAction(CompileMenuDoc,LuaLatexDoc,SLOT(NewDatabaseBaseFile()),"Ctrl+Shift+L",QIcon(":/images/lualatex.svg"),"&LuaLaTeX");
    PythonTexDoc = CreateNewAction(CompileMenuDoc,PythonTexDoc,SLOT(NewDatabaseBaseFile()),"Ctrl+Shift+P",QIcon(":/images/pythontex.svg"),"&PythonTeX");
    BibTexDoc = CreateNewAction(CompileMenuDoc,BibTexDoc,SLOT(Compile()),"Ctrl+Shift+B",QIcon(":/images/address-book-new.svg"),"&BibTeX");
    AsymptoteDoc = CreateNewAction(CompileMenuDoc,AsymptoteDoc,SLOT(CompileAsymptote()),"Ctrl+Shift+A",QIcon(":/images/Asymptote.svg"),"&Asymptote");
    CompileCommandsDoc->setMenu(CompileMenuDoc);
    CompileCommandsDoc->setIconSize(QSize(20,20));
    CompileCommandsDoc->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CompileCommandsDoc->setDefaultAction(PdfLatexDoc);

    QObject::connect(CompileCommands, SIGNAL(triggered(QAction*)),this, SLOT(setDefaultAction(QAction*)));
    QObject::connect(CompileCommandsDoc, SIGNAL(triggered(QAction*)),this, SLOT(setDefaultActionDoc(QAction*)));
}

void DataTex::loadDatabaseFields()
{
    if(labelList.count()>0/* && ui->stackedWidget->currentIndex()==0*/){
        for (int i=labelList.count()-1;i>-1;i--) {
            ui->verticalLayout_3->removeWidget(labelList[i]);
            ui->verticalLayout_3->removeWidget(lineList[i]);
            ui->verticalLayout_3->removeItem(hLayoutList[i]);
            delete labelList.takeAt(0);
            delete lineList.takeAt(0);
            delete hLayoutList.takeAt(0);
        }
    }
    Optional_Metadata_Ids.clear();
    Optional_Metadata_Names.clear();
    labelList.clear();
    hLayoutList.clear();
    lineList.clear();

    QSqlQuery Select_DataBase_Optional_Metadata(DataTeX_Settings);
    Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDataBaseOptionalFields)
                                           .arg(QFileInfo(DataTex::CurrentDataBasePath).baseName()));
    while(Select_DataBase_Optional_Metadata.next()){
        Optional_Metadata_Ids.append(Select_DataBase_Optional_Metadata.value(0).toString());
        Optional_Metadata_Names.append(Select_DataBase_Optional_Metadata.value(1).toString());
    }
    for (int i=0;i<Optional_Metadata_Ids.count();i++ ) {
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(Optional_Metadata_Names.at(i),this);
        QLineEdit * line = new QLineEdit(this);
        hLayoutList.append(hbox);
        labelList.append(label);
        lineList.append(line);
        ui->verticalLayout_3->addLayout(hbox);
        hLayoutList.at(i)->addWidget(label,0);
        hLayoutList.at(i)->addWidget(line,1);
        line->setAlignment(Qt::AlignRight);
        line->setReadOnly(true);
        connect(line,&QLineEdit::textEdited,this,[=](){
            label->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
        });
    }

    if(Doc_labelList.count()>0 && ui->stackedWidget->currentIndex()==1){
        for (int i=Doc_labelList.count()-1;i>-1;i--) {
            ui->verticalLayout_1->removeWidget(Doc_labelList[i]);
            ui->verticalLayout_1->removeWidget(Doc_lineList[i]);
            ui->verticalLayout_1->removeItem(Doc_hLayoutList[i]);
            delete Doc_labelList.takeAt(0);
            delete Doc_lineList.takeAt(0);
            delete Doc_hLayoutList.takeAt(0);
        }
    }
    Optional_DocMetadata_Ids.clear();
    Optional_DocMetadata_Names.clear();
    Doc_labelList.clear();
    Doc_hLayoutList.clear();
    Doc_lineList.clear();

    QSqlQuery Doc_Select_DataBase_Optional_Metadata(DataTeX_Settings);
    Doc_Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDocDataBaseOptionalFields)
                                           .arg(QFileInfo(DataTex::CurrentNotesFolderPath).baseName()));
    while(Doc_Select_DataBase_Optional_Metadata.next()){
        Optional_DocMetadata_Ids.append(Doc_Select_DataBase_Optional_Metadata.value(0).toString());
        Optional_DocMetadata_Names.append(Doc_Select_DataBase_Optional_Metadata.value(1).toString());
    }
    for (int i=0;i<Optional_DocMetadata_Ids.count();i++ ) {
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(Optional_DocMetadata_Names.at(i),this);
        QLineEdit * line = new QLineEdit(this);
        Doc_hLayoutList.append(hbox);
        Doc_labelList.append(label);
        Doc_lineList.append(line);
        ui->verticalLayout_1->addLayout(hbox);
        Doc_hLayoutList.at(i)->addWidget(label,0);
        Doc_hLayoutList.at(i)->addWidget(line,1);
        line->setAlignment(Qt::AlignRight);
        line->setReadOnly(true);
        connect(line,&QLineEdit::textEdited,this,[=](){
            label->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
        });
    }

    if(Bib_labelList.count()>0 && ui->stackedWidget->currentIndex()==0){
        for (int i=Bib_labelList.count()-1;i>-1;i--) {
            ui->verticalLayout_2->removeWidget(Bib_labelList[i]);
            ui->verticalLayout_2->removeWidget(Bib_lineList[i]);
            ui->verticalLayout_2->removeItem(Bib_hLayoutList[i]);
            delete Bib_labelList.takeAt(0);
            delete Bib_lineList.takeAt(0);
            delete Bib_hLayoutList.takeAt(0);
        }
    }
    BibFieldIds.clear();
    BibFieldNames.clear();
    Bib_labelList.clear();
    Bib_hLayoutList.clear();
    Bib_lineList.clear();

    QStringList DocTypesIds = {"@article" ,"@book" ,"@mvbook", "@inbook","@bookinbook ","@suppbook",
                               "@booklet ","@collection ","@mvcollection",
                               "@incollection ","@suppcollection ","@manual",
                               "@misc ","@online ","@patent",
                               "@periodical ","@suppperiodical ","@proceedings",
                               "@mvproceedings ","@inproceedings ","@reference",
                               "@mvreference ","@inreference ","@report",
                               "@thesis ","@unpublished"};

    QStringList DocTypesNames = {tr("Article") ,tr("Book") ,tr("Multivolume book"), tr("Part of a book"),tr("Book in book") ,tr("Supplemental Material in a book"),
                               tr("Booklet") ,tr("Collection") ,tr("Multivolume collection"),
                               tr("Part in a collection") ,tr("Supplemental material in a collection") ,tr("Manual"),
                               tr("Miscellaneous") ,tr("Online resource") ,tr("Patent"),
                               tr("Complete issue of a periodical") ,tr("Supplemental material in a periodical") ,tr("Proceedings"),
                               tr("Multivolume proceedings") ,tr("Article in proceedings") ,tr("Reference"),
                               tr("Multivolume reference") ,tr("Part of a Reference") ,tr("Report"),
                               tr("Thesis") ,tr("Unpublished")};
    for (int i=0;i<DocTypesIds.count();i++) {
        ui->DocumentTypeCombo->addItem(DocTypesNames[i],QVariant(DocTypesIds[i]));
    }

    QSqlQuery Bibliography(DataTeX_Settings);
    Bibliography.exec(QString(SqlFunctions::GetCurrentBibliographyFields)
                                           .arg(QFileInfo(DataTex::CurrentDataBasePath).baseName()));
    while(Bibliography.next()){
        BibFieldIds.append(Bibliography.value(0).toString());
        BibFieldNames.append(Bibliography.value(1).toString());
    }
    for (int i=2;i<BibFieldIds.count();i++ ) {
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(BibFieldNames.at(i),this);
        QLineEdit * line = new QLineEdit(this);
        Bib_hLayoutList.append(hbox);
        Bib_labelList.append(label);
        Bib_lineList.append(line);
        ui->verticalLayout_2->addLayout(hbox);
        hbox->addWidget(label,0);
        hbox->addWidget(line,1);
        line->setAlignment(Qt::AlignRight);
        line->setReadOnly(true);
        connect(line,&QLineEdit::textEdited,this,[=](){
            label->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
            BibSourceCode(ui->DocumentTypeCombo->currentIndex());
        });
    }
    connect(ui->DocumentTypeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(BibSourceCode(int)));
}

void DataTex::SettingsDatabase_Variables()
{
    QStringList MetadataNames;
    MetadataNames <<tr("Name")<<tr("File Type")<<tr("Field")<<tr("Chapter")<<tr("Section")<<tr("Exercise Type")
            <<tr("Difficulty")<<tr("Path")<<tr("Date")
           <<tr("Solved")<<tr("Bibliography")<<tr("File Content")<<tr("Preamble")<<tr("Build Command")
          <<tr("File Description")<<tr("Multi-Section");
    QStringList Bibliography;
    Bibliography <<tr("Citation Key")<<tr("Document Type")<<tr("Title")<<tr("Author")<<tr("Editor")
            <<tr("Publisher")<<tr("Year")<<tr("Month")<<tr("ISBN")
           <<tr("ISSN")<<tr("Pages")<<tr("Series")<<tr("Volume")
          <<tr("Journal")<<tr("School/Institute")<<tr("Number/Issue")
         <<tr("Address")<<tr("DOI")<<tr("URL")<<tr("Edition");

    QStringList DocMetadata;
    DocMetadata <<tr("Name")<<tr("Document Type")<<tr("Basic folder")<<tr("Subfolder")<<tr("Subsubfolder")
            <<tr("Path")<<tr("Date")<<tr("Content")<<"Preamble"
           <<tr("LaTeX build command")<<tr("Needs update")<<tr("Bibliography")<<tr("Use bibliography file");

    datatexpath = QDir::homePath()+QDir::separator()+".datatex"+QDir::separator();
    QDir datatexdir(datatexpath);
    if (!datatexdir.exists())
        datatexdir.mkpath(".");
    QString DataTex_Settings_Path = datatexpath+"DataTex_Settings.db";
    if(!QFileInfo::exists(DataTex_Settings_Path)){
        QFile Settings(":/databases/DataTex_Settings.db");
        Settings.copy(DataTex_Settings_Path);
        QFile(DataTex_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        DataTeX_Settings.setDatabaseName(DataTex_Settings_Path);
        DataTeX_Settings.open();
        //Εγγραφή στο φάκελο Folders τους βασικούς φακέλους που θα περιέχει μια βάση εγγράφων.
        QStringList MetadataList;
        QStringList BibliographyList;
        QStringList DocMetadataList;
        QSqlQuery WriteBasicMetadata(DataTeX_Settings);
        WriteBasicMetadata.exec("SELECT \"Id\" FROM \"Metadata\" ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            MetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<MetadataList.count();i++){
            QString query = "UPDATE \"Metadata\" SET \"Name\" = '"
                            +MetadataNames.at(i)+"' WHERE \"Id\" = '"+MetadataList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }

        WriteBasicMetadata.exec("SELECT \"Id\" FROM \"Bibliography\" ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            BibliographyList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<Bibliography.count();i++){
            QString query = "UPDATE \"Bibliography\" SET \"Name\" = '"
                            +Bibliography.at(i)+"' WHERE \"Id\" = '"+BibliographyList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }

        WriteBasicMetadata.exec("SELECT \"Id\" FROM \"DocMetadata\" ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            DocMetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<DocMetadata.count();i++){
            QString query = "UPDATE \"DocMetadata\" SET \"Name\" = '"
                            +DocMetadata.at(i)+"' WHERE \"Id\" = '"+DocMetadataList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }

        LatexCommandsArguments.insert("PdfLaTeX",{"-interaction=nonstopmode","--shell-escape"});
        LatexCommandsArguments.insert("LaTeX",{"-interaction=nonstopmode","--shell-escape"});
        LatexCommandsArguments.insert("XeLaTeX",{"-interaction=nonstopmode","--shell-escape"});
        LatexCommandsArguments.insert("LuaLaTeX",{"-interaction=nonstopmode","--shell-escape"});
        LatexCommandsArguments.insert("PythonTex",{});
        LatexCommandsArguments.insert("BibTeX",{});
        LatexCommandsArguments.insert("Asymptote",{});

        PdfLatex_Command = SetLatexCommand("Pdflatex_Command","pdflatex",PdfLatex,PdfLatexDoc,LatexCommandsArguments["PdfLaTeX"],".tex");
        Latex_Command = SetLatexCommand("Latex_Command","latex",Latex,LatexDoc,LatexCommandsArguments["LaTeX"],".tex");
        XeLatex_Command = SetLatexCommand("Xelatex_Command","xelatex",XeLatex,XeLatexDoc,LatexCommandsArguments["XeLaTeX"],".tex");
        LuaLatex_Command = SetLatexCommand("Lualatex_Command","lualatex",LuaLatex,LuaLatexDoc,LatexCommandsArguments["LuaLaTeX"],".tex");
        Pythontex_Command = SetLatexCommand("Pythontex_Command","pythontex",PythonTex,PythonTexDoc,LatexCommandsArguments["PythonTex"],".tex");
        Bibtex_Command = SetLatexCommand("Bibtex_Command","bibtex",BibTex,BibTexDoc,LatexCommandsArguments["BibTeX"],".aux");
        Asymptote_Command = SetLatexCommand("Asymptote_Command","asy",Asymptote,AsymptoteDoc,LatexCommandsArguments["Asymptote"],"*.asy");

        LatexCommands.insert("PdfLaTeX",PdfLatex_Command);
        LatexCommands.insert("LaTeX",Latex_Command);
        LatexCommands.insert("XeLaTeX",XeLatex_Command);
        LatexCommands.insert("LuaLaTeX",LuaLatex_Command);
        LatexCommands.insert("PythonTex",Pythontex_Command);
        LatexCommands.insert("BibTeX",Bibtex_Command);
        LatexCommands.insert("Asymptote",Asymptote_Command);
        DataTeX_Settings.close();
    }

    QFile(DataTex_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    DataTeX_Settings.setDatabaseName(DataTex_Settings_Path);
    DataTeX_Settings.open();
//    QString filesdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentDataBase);
//    QString docsdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentNotesFolderBase);
//    CurrentTexFilesDataBase = QSqlDatabase::database(filesdb);
//    CurrentNotesFolderDataBase = QSqlDatabase::database(docsdb);
//    CurrentDataBasePath = CurrentTexFilesDataBase.databaseName();
//    CurrentNotesFolderPath = CurrentNotesFolderDataBase.databaseName();
//    qDebug()<<filesdb<<docsdb;
//    DataTex::CurrentTexFilesDataBase.setDatabaseName(CurrentDataBasePath);
//    DataTex::CurrentTexFilesDataBase.open();
//    DataTex::CurrentNotesFolderDataBase.setDatabaseName(CurrentNotesFolderPath);
//    DataTex::CurrentNotesFolderDataBase.open();
    QStringList list1 = SqlFunctions::Get_StringList_From_Query(SqlFunctions::GetPreamble,DataTeX_Settings);
    if(list1.count()==0){
        DataTex::CurrentPreamble = "";}
    else{DataTex::CurrentPreamble = list1.at(0);}
    QStringList list2 = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                               .arg(DataTex::CurrentPreamble)
                                                               ,DataTeX_Settings);
    if(list2.count()==0){DataTex::CurrentPreamble_Content = "";}
    else{DataTex::CurrentPreamble_Content = list2.at(0);}


    LatexCommandsArguments.insert("PdfLaTeX",{"-interaction=nonstopmode","--shell-escape"});
    LatexCommandsArguments.insert("LaTeX",{"-interaction=nonstopmode","--shell-escape"});
    LatexCommandsArguments.insert("XeLaTeX",{"-interaction=nonstopmode","--shell-escape"});
    LatexCommandsArguments.insert("LuaLaTeX",{"-interaction=nonstopmode","--shell-escape"});
    LatexCommandsArguments.insert("PythonTex",{});
    LatexCommandsArguments.insert("BibTeX",{});
    LatexCommandsArguments.insert("Asymptote",{});

    DataTex::PdfLatex_Command = GetLatexCommand("Pdflatex_Command",PdfLatex,PdfLatexDoc,LatexCommandsArguments["PdfLaTeX"],".tex");
    DataTex::Latex_Command = GetLatexCommand("Latex_Command",Latex,LatexDoc,LatexCommandsArguments["LaTeX"],".tex");
    DataTex::XeLatex_Command = GetLatexCommand("Xelatex_Command",XeLatex,XeLatexDoc,LatexCommandsArguments["XeLaTeX"],".tex");
    DataTex::LuaLatex_Command = GetLatexCommand("Lualatex_Command",LuaLatex,LuaLatexDoc,LatexCommandsArguments["LuaLaTeX"],".tex");
    DataTex::Pythontex_Command = GetLatexCommand("Pythontex_Command",PythonTex,PythonTexDoc,LatexCommandsArguments["PythonTex"],".tex");
    DataTex::Bibtex_Command = GetLatexCommand("Bibtex_Command",BibTex,BibTexDoc,LatexCommandsArguments["BibTeX"],".aux");
    DataTex::Asymptote_Command = GetLatexCommand("Asymptote_Command",Asymptote,AsymptoteDoc,LatexCommandsArguments["Asymptote"],"*.asy");

    LatexCommands.insert("PdfLaTeX",PdfLatex_Command);
    LatexCommands.insert("LaTeX",Latex_Command);
    LatexCommands.insert("XeLaTeX",XeLatex_Command);
    LatexCommands.insert("LuaLaTeX",LuaLatex_Command);
    LatexCommands.insert("PythonTex",Pythontex_Command);
    LatexCommands.insert("BibTeX",Bibtex_Command);
    LatexCommands.insert("Asymptote",Asymptote_Command);
}

void DataTex::DatabaseStructure(QString database)
{
    model = new QFileSystemModel(this);
    model->setRootPath(QFileInfo(database).absolutePath());
    QStringList filters;
    filters.append("*.tex");
    filters.append("*.pdf");
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
    model->insertRow(0);
    ui->DatabaseStructureTreeView->setModel(model);
    QModelIndex index = model->index(QFileInfo(database).absolutePath());
    ui->DatabaseStructureTreeView->setRootIndex(index);
    ui->DatabaseStructureTreeView->expand(index);
    ui->DatabaseStructureTreeView->scrollTo(index);
    ui->DatabaseStructureTreeView->setCurrentIndex(index);
    ui->DatabaseStructureTreeView->resizeColumnToContents(0);
    for (int i=1;i<model->columnCount();i++) {
        ui->DatabaseStructureTreeView->setColumnHidden(i,true);
    }
}

DataTex::~DataTex()
{
    delete ui;
    delete LatexFileView;
    delete DocumentView;
    delete FilesProxyModel;
    delete FilesTable;
    delete DocumentsTable;
    delete DocumentsProxyModel;
    delete FileFromDocumentView;
    delete FileMenu;
    delete EditMenu;
    delete ViewMenu;
    delete ToolMenu;
    delete SettingsMenu;
    delete HelpMenu;
    delete CompileMenu;
    delete CompileMenuDoc;
    delete LatexFileActions;
    delete DocumentActions;
    delete FileToolBar;
    delete LatexTools_ToolBar;
    delete DocTools_ToolBar;
    delete SettingsToolBar;
    delete CompileBar;
    delete CompileBarDoc;
    delete NewDatabasefile;
    delete OpenDatabasefile;
    delete CloseDatabasefile;
    delete SyncDatabasefile;
    delete SaveAsDatabasefile;
    delete NewLatexFile;
    delete OpenLatexFile;
    delete DeleteLatexFile;
    delete EditLatexFile;
    delete SolveLatexFile;
    delete FileCloneToOtherDB;
    delete PdfLatex;
    delete Latex;
    delete XeLatex;
    delete LuaLatex;
    delete PythonTex;
    delete BibTex;
    delete Asymptote;
    delete SaveTex;
    delete UndoTex;
    delete OpenPath;
    delete PdfLatexDoc;
    delete LatexDoc;
    delete XeLatexDoc;
    delete LuaLatexDoc;
    delete PythonTexDoc;
    delete BibTexDoc;
    delete AsymptoteDoc;
    delete SaveTexDoc;
    delete UndoTexDoc;
    delete OpenPathDoc;
    delete NewDocument;
    delete InsertFileToDocument;
    delete AddDocument;
    delete DeleteDocument;
    delete CreateSolutionsDoc;
    delete CreateBibliography;
    delete Preamble;
    delete GeneralSettings;
    delete BackUpDatabase;
    delete Datatables;
    delete ConnectWithTexEditor;
    delete CompileCommands;
    delete CompileCommandsDoc;
}

QAction * DataTex::CreateNewAction(QMenu * Menu, QAction * Action, const char *Function, QString ShortCut, QIcon Icon, const char * Description)
{
    Action = new QAction(Icon,tr(Description), this);
    Action->setShortcut(QString(ShortCut));
    Menu->addAction(Action);
    connect(Action, SIGNAL(triggered()), this, Function);
    return Action;
}

QAction * DataTex::CreateNewAction(QMenu * Menu, QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, const char * Description)
{
    Action = new QAction(Icon,tr(Description), this);
    Action->setShortcut(QString(ShortCut));
    Menu->addAction(Action);
    QObject::connect(Action, &QAction::triggered, this, Function);
    return Action;
}

void DataTex::NewDatabaseBaseFile()
{
    if (CurrentDataBasePath.isEmpty() || CurrentDataBasePath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     tr("Error"),tr("No Files database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        NewDatabaseFile * newfile = new NewDatabaseFile(this,{},{},false,QString());
        connect(newfile,SIGNAL(acceptSignal(QString,QString)),this,
                SLOT(EditNewBaseFile(QString,QString)));
        newfile->show();
        newfile->activateWindow();
    }
}

void DataTex::EditNewBaseFile(QString fileName,QString FileContent)
{
    // Create new Latex file and write content --------
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QTextStream writeContent(&file);
    writeContent.flush();
    writeContent << FileContent;
    file.close();
    // Load and show new entry in table
    ShowDataBaseFiles();
    QAbstractItemModel * m = FilesTable->model();
    QModelIndex ix = FilesTable->currentIndex();
    while (FilesTable->model()->canFetchMore(ix))
           FilesTable->model()->fetchMore(ix);
    int rows = m->rowCount();
    FilesTable->selectRow(rows-1);
    FilesTable->verticalScrollBar()->setValue(FilesTable->verticalScrollBar()->maximum());
    ui->tabWidget->setCurrentIndex(2);
}

void DataTex::SaveContentToDatabase(QString fileName, QString content)
{
    QSqlQuery WriteContent(DataTex::CurrentTexFilesDataBase);
    WriteContent.prepare("UPDATE \"Database_Files\" SET \"FileContent\" = :content WHERE \"Id\" = :file");
    WriteContent.bindValue(":file",QFileInfo(fileName).baseName());
    WriteContent.bindValue(":content",content);
    WriteContent.exec();

    QSqlQuery WriteContent_2(DataTex::CurrentNotesFolderDataBase);
    WriteContent_2.prepare("UPDATE \"Documents\" SET \"Content\" = :content WHERE \"Id\" = :file");
    WriteContent_2.bindValue(":file",QFileInfo(fileName).baseName());
    WriteContent_2.bindValue(":content",content);
    WriteContent_2.exec();
}

void DataTex::SolutionFile()
{
    if (DataTex::CurrentDataBasePath.isEmpty() || DataTex::CurrentDataBasePath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Files database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        QStringList metadata;
        QString Sections;
        if(!DatabaseFileName.isEmpty()){
            metadata.append({DatabaseFileName,Field,FileTypeId});
            Sections = Section;
        }
        SolveDatabaseExercise * newsoldialog = new SolveDatabaseExercise(this,metadata,Sections);
        connect(newsoldialog,&SolveDatabaseExercise::solution,this,[](QString fileName,QString FileContent){
            // Create new Latex file and write content --------
            QFile file(fileName);
            file.open(QIODevice::ReadWrite);
            QTextStream writeContent(&file);
            writeContent.flush();
            writeContent << FileContent;
            file.close();
        });
        newsoldialog->show();
        newsoldialog->activateWindow();
    }
}

void DataTex::InsertFiles()
{
    if (DataTex::CurrentNotesFolderPath.isEmpty() || DataTex::CurrentNotesFolderPath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Document database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        QString currentTexFile = DocumentFilePath;
        AddFileToEditor * inserttexfile = new AddFileToEditor(this,currentTexFile,DocumentBuildCommand);
        inserttexfile->show();
        inserttexfile->activateWindow();
    }
}

void DataTex::PersonalNotes()
{
    if (DataTex::CurrentDataBasePath.isEmpty() || DataTex::CurrentDataBasePath.isNull() ||
            DataTex::CurrentNotesFolderPath.isEmpty() || DataTex::CurrentNotesFolderPath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Document database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
    NotesDocuments * notes = new NotesDocuments(this);
    connect(notes,SIGNAL(createnewdocument(QString,QString)),this,SLOT(CreateNewSheet(QString,QString)));
    connect(notes,&NotesDocuments::insertfiles,this,[=](){InsertFiles();});
    notes->show();
    notes->activateWindow();
    }
}

int CountLines(QString filePath)
{
    QFile file(filePath);
    int line_count=0;
    file.open(QIODevice::ReadOnly); //| QIODevice::Text)
    QTextStream in(&file);
    while( !in.atEnd())
    {
        QString line;
        line=in.readLine()+"\n";
        line_count++;
    }
    file.close();
    return line_count;
}

void DataTex::CreateNewSheet(QString fileName,QString Content)
{
    QString filepath = QFileInfo(fileName).absolutePath();
    QStringList list = filepath.split("/");
    QString filetype = list.last();
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QTextStream writeContent(&file);
    writeContent.flush();
    writeContent << Content;
    file.close();
//    int lines = CountLines(fileName);
    ShowDocuments();
    QAbstractItemModel * m = DocumentsTable->model();
    QModelIndex ix = DocumentsTable->currentIndex();
    while (DocumentsTable->model()->canFetchMore(ix))
           DocumentsTable->model()->fetchMore(ix);
    int rows = m->rowCount();
    DocumentsTable->selectRow(rows-1);
    DocumentsTable->verticalScrollBar()->setValue(DocumentsTable->verticalScrollBar()->maximum());
//    currentEditor()->setCursorPosition(lines-2,0);
}

void DataTex::DataBaseFields()
{
    if (DataTex::CurrentDataBasePath.isEmpty() || DataTex::CurrentDataBasePath.isNull() ||
            DataTex::CurrentNotesFolderPath.isEmpty() || DataTex::CurrentNotesFolderPath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Files or Document database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        DataTables * fieldstable = new DataTables(this);
        fieldstable->show();
        fieldstable->activateWindow();
    }
}

void DataTex::CreateSolutionsDocument()
{
//    QHash<QString, QStringList> Sol_per_exer;
//    for (int i=0;i<3;i++ ) {
//        Sol_per_exer.insert(QString::number(i),{QString::number(i)+".sol",QString::number(i+1)+".sol"});
//    }
    if (DataTex::CurrentDataBasePath.isEmpty() || DataTex::CurrentDataBasePath.isNull() ||
            DataTex::CurrentNotesFolderPath.isEmpty() || DataTex::CurrentNotesFolderPath.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Files database created and a Notes Database.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        if (SolutionsPerExercise.count() == 0) {
                QMessageBox msgBox;
                msgBox.setText("No solutions");
                msgBox.addButton(QMessageBox::Ok);
                msgBox.exec();
                return;
            }
        else{
            SolutionsDocument * solutionbuilder = new SolutionsDocument(this,DocumentFilePath,DocumentBuildCommand,Exer_List,SolutionsPerExercise);
            solutionbuilder->show();
            solutionbuilder->activateWindow();
        }
    }
}

void DataTex::DataTeX_Preferences()
{
    QString path = datatexpath;
    Paths * folderpaths = new Paths(this,path);
    QObject::connect(folderpaths, &Paths::iconsize, this,[=](int size){
        FileToolBar->setIconSize(QSize(size,size));
        LatexTools_ToolBar->setIconSize(QSize(size,size));
        DocTools_ToolBar->setIconSize(QSize(size,size));
        SettingsToolBar->setIconSize(QSize(size,size));
    });
    QObject::connect(folderpaths, &Paths::selectfont, this,[=](QString selectedfont){
        QFont font(selectedfont);
        font.setStyleHint(QFont::Monospace);
        font.setPointSize(10);
        QApplication::setFont(font);});

    folderpaths->show();
    folderpaths->activateWindow();
}

void DataTex::BackUp_DataBase_Folders()
{
    DataTex::FunctionInProgress();
//    BackUp * back = new BackUp(this);
//    back->show();
//    back->activateWindow();
}

void DataTex::DatabaseSyncFiles()
{
    DatabaseSync * datasync = new DatabaseSync(this);
    datasync->show();
    datasync->activateWindow();
}

void DataTex::EditFileMeta()
{
    QStringList meta = {FileType,Field,Chapter,ExerciseType,QString::number(Difficulty),
                 CurrentPreamble,CurrentBuildCommand,FileDescription,FileContent,QString::number(MultiSection),FileTypeId};
    QStringList sections = Section.split("-");
    NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,sections,true,DatabaseFileName);
    connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
            SLOT(EditNewBaseFile(QString,QString)));
    Edit->show();
    Edit->activateWindow();
}

void DataTex::CreateTexFile(QString fullFilePath,bool addToPreamble,QString addStuffToPreamble)
{
    QString outputDir = QFileInfo(fullFilePath).absolutePath();
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QString realContent = QString();

    QString sheetFileContent = DataTex::CurrentPreamble_Content+ "\n";
            sheetFileContent += (addToPreamble) ? addStuffToPreamble : QString();
            sheetFileContent += "\n\\begin{document}\n";
    QFile Databasefile(fullFilePath);
    Databasefile.open(QIODevice::ReadOnly | QFile::Text);
    QTextStream exoStream(&Databasefile);
    exoStream.flush();
    realContent = exoStream.readAll();
    sheetFileContent += "\n"+realContent+"\n";
    sheetFileContent += "\n \\end{document}";

    QString sheetFile = outputDir + QDir::separator() + outputFile +"-preview.tex";
    QFile file(sheetFile);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);
    out.flush();
    out << sheetFileContent;
    file.close();
}

void DataTex::BuildDocument(QString CompileCommand,QString fullFilePath,QStringList args,QString ext)
{
    QString outputDir = QFileInfo(fullFilePath).absolutePath();
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QProcess compileProcess;

#ifndef Q_OS_WIN
    QStringList env = QProcess::systemEnvironment();
    int j = env.indexOf(QRegExp("^PATH=(.*)"));
    int limit = env.at(j).indexOf("=");
    QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
    value = "PATH=" + value + ":" + QFileInfo(CompileCommand).path() + ":";
    env.replace(j,value);
    compileProcess.setEnvironment(env);
#endif
    compileProcess.setWorkingDirectory(outputDir);
    QString newTexFile = /*QFileInfo(fullFilePath).absolutePath()+QDir::separator()+*/outputFile +"-preview"+ext;
    QString oldpdf = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +".pdf";
    if (QFileInfo::exists(oldpdf)){QFile(oldpdf).remove();}

//#ifdef Q_OS_WIN
//#else
//#endif
    args << qPrintable(newTexFile);
    compileProcess.start(CompileCommand,args);
    compileProcess.waitForFinished(-1);
    QString errorOutput = QString(compileProcess.readAllStandardOutput());
}

void DataTex::ClearOldFiles(QString fullFilePath)
{
    QStringList extensions;
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QString newTexFile = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +"-preview.tex";
    extensions << ".log" << ".aux" << ".tex" << "-old.pdf" << ".out"<<".run.xml"<<".bcf";
    QString trashFile;
    foreach (QString ext,extensions)
    {
        trashFile = QFileInfo(fullFilePath).path() + QDir::separator() + QFileInfo(fullFilePath).baseName() + "-preview" + ext;
        if (QFileInfo::exists(trashFile)) QFile(trashFile).remove();
    }
    QString PdfOutput = QFileInfo(fullFilePath).path() + QDir::separator()+ QFileInfo(fullFilePath).baseName()+".pdf";
    QString pdffile = newTexFile.replace(".tex",".pdf");
    QFile renamePdf(pdffile);
    renamePdf.rename(pdffile,PdfOutput);
}

void DataTex::on_FilesDatabaseToggle_clicked(bool checked)
{
    if(checked){
        DocumentActions->setEnabled(!checked);
        DocTools_ToolBar->setEnabled(!checked);
        NewDocument->setEnabled(!checked);
        AddDocument->setEnabled(!checked);
        DeleteDocument->setEnabled(!checked);
        InsertFileToDocument->setEnabled(!checked);
        EditDocument->setEnabled(!checked);
        CreateSolutionsDoc->setEnabled(!checked);
        UpdateDocContent->setEnabled(!checked);

        LatexFileActions->setEnabled(checked);
        LatexTools_ToolBar->setEnabled(checked);
        NewLatexFile->setEnabled(checked);
        OpenLatexFile->setEnabled(checked);
        DeleteLatexFile->setEnabled(checked);
        EditLatexFile->setEnabled(checked);
        SolveLatexFile->setEnabled(checked);
        FileCloneToOtherDB->setEnabled(checked);

        ui->stackedWidget->setCurrentIndex(0);
        DatabaseStructure(CurrentDataBasePath);
    }
}

void DataTex::on_DocumentsDatabaseToggle_clicked(bool checked)
{
    if(checked){
        DocumentActions->setEnabled(checked);
        DocTools_ToolBar->setEnabled(checked);
        NewDocument->setEnabled(checked);
        AddDocument->setEnabled(checked);
        LatexTools_ToolBar->setEnabled(!checked);
        LatexFileActions->setEnabled(!checked);
        NewLatexFile->setEnabled(!checked);
        OpenLatexFile->setEnabled(!checked);
        DeleteLatexFile->setEnabled(!checked);
        EditLatexFile->setEnabled(!checked);
        SolveLatexFile->setEnabled(!checked);
        FileCloneToOtherDB->setEnabled(!checked);
        ui->stackedWidget->setCurrentIndex(1);
//        ui->FilesDatabaseToggle->setChecked(!checked);

        DatabaseStructure(CurrentNotesFolderPath);
    }
}

void DataTex::ShowDataBaseFiles()
{
    QSqlQueryModel * Database = new QSqlQueryModel(this);
    //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel = new QSortFilterProxyModel(this);
    QSqlQuery DatabaseQuery(CurrentTexFilesDataBase);
    DatabaseQuery.exec(SqlFunctions::ShowAllDatabaseFiles);
    Database->setQuery(DatabaseQuery);

    //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel->setSourceModel(Database);
    //Φιλτράρισμα και ταξινόμηση --- FilesTable->setModel(FilesProxyModel);
    FilesTable->setModel(Database);
    FilesTable->show();
    //Φιλτράρισμα και ταξινόμηση --- FilesTable->setSortingEnabled(true);
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, [=](){ui->ClearFiltersFD->setEnabled(true);});
    LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    for (int i=0;i<FilesTable->model()->columnCount();i++ ) {
        FilesTable->filterHeader()->placeHolderText(i,FilesTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
    }
    StretchColumns(FilesTable,1.5);
}

void DataTex::updateFilter(/*size_t column, const QString& value*/QStringList values)
{
    if(ui->stackedWidget->currentIndex()==0){
        SqlFunctions::FilesTable_UpdateQuery.clear();
        FilterTables_Queries(Database_FileTableFields);
        int columns = Database_FileTableFields.count();
        for (int i=0;i<columns;i++) {
            SqlFunctions::FilesTable_UpdateQuery.replace("replace"+Database_FileTableFields.at(i),values.at(i));
        }
        FilesTable->setColumnHidden(columns,true);
        FilesTable->setColumnHidden(columns,true);
        FilesTable->setColumnHidden(columns-Optional_Metadata_Ids.count()-1,true);
        updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,CurrentTexFilesDataBase,this);
        connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DataTex::FilesTable_selectionchanged);
        LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
        //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel->setFilterRegExp(QRegExp(".png", Qt::CaseInsensitive,QRegExp::FixedString));
        //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel->setFilterKeyColumn(1);
    }
    else{
        SqlFunctions::FilterDatabaseDocuments.clear();
        FilterDocuments(Database_DocumentTableColumns);
        int columns = Database_DocumentTableColumns.count();
        for (int i=0;i<columns;i++) {
            SqlFunctions::FilterDatabaseDocuments.replace("replace"+Database_DocumentTableColumns.at(i),values.at(i));
        }
//        DocumentsTable->setColumnHidden(columns,true);
        updateTableView(DocumentsTable,SqlFunctions::FilterDatabaseDocuments,CurrentNotesFolderDataBase,this);
        connect(DocumentsTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DataTex::DocumentsTable_selectionChanged);
        LoadTableHeaders(DocumentsTable,Database_DocTableFieldNames);
    }
}

void DataTex::ShowDocuments()
{
    DocumentModel = new QSqlTableModel(this,CurrentNotesFolderDataBase);
    DocumentModel->setTable("Documents");
    DocumentModel->select();
    DocumentsTable->setModel(DocumentModel);
    DocumentsTable->show();
    int columns = DocumentsTable->model()->columnCount();
    DocumentsTable->generateFilters(columns,false);
    connect(DocumentsTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::DocumentsTable_selectionChanged);
    connect(DocumentsTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
    connect(DocumentsTable->filterHeader(), &FilterTableHeader::filterValues, this,[=](){ui->ClearFiltersDD->setEnabled(true);});
    LoadTableHeaders(DocumentsTable,Database_DocTableFieldNames);
    for (int i=0;i<DocumentsTable->model()->columnCount();i++ ) {
        DocumentsTable->filterHeader()->placeHolderText(i,DocumentsTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
    }
    StretchColumns(DocumentsTable,1.5);
}

void DataTex::FilesTable_selectionchanged()
{
    CompileMenu->setEnabled(true);
    PdfLatex->setEnabled(true);
    DeleteLatexFile->setEnabled(true);
    OpenPath->setEnabled(true);
    ui->addBibEntry->setEnabled(true);
    int row = FilesTable->currentIndex().row();
    DatabaseFileName = FilesTable->model()->data(FilesTable->model()->index(row,0)).toString();
    Field = FilesTable->model()->data(FilesTable->model()->index(row,2)).toString();
    Chapter = FilesTable->model()->data(FilesTable->model()->index(row,3)).toString();
    Section = FilesTable->model()->data(FilesTable->model()->index(row,4)).toString();
    ExerciseType = FilesTable->model()->data(FilesTable->model()->index(row,5)).toString();
    FileType = FilesTable->model()->data(FilesTable->model()->index(row,1)).toString();
    DatabaseFilePath = FilesTable->model()->data(FilesTable->model()->index(row,7)).toString();
    Difficulty = FilesTable->model()->data(FilesTable->model()->index(row,6)).toInt();
    Date = FilesTable->model()->data(FilesTable->model()->index(row,8)).toString();
    Solved = FilesTable->model()->data(FilesTable->model()->index(row,9)).toString();
    CurrentBuildCommand = FilesTable->model()->data(FilesTable->model()->index(row,13)).toString();
    FileDescription = FilesTable->model()->data(FilesTable->model()->index(row,14)).toString();
    MultiSection = FilesTable->model()->data(FilesTable->model()->index(row,15)).toInt();
    FileTypeId = FilesTable->model()->data(FilesTable->model()->index(row,Database_FileTableFieldNames.count())).toString();
    Solvable = FilesTable->model()->data(FilesTable->model()->index(row,Database_FileTableFieldNames.count()+1)).toInt();
    ui->DateTimeEdit->setDateTime(QDateTime::fromString(Date,"dd/M/yyyy hh:mm"));
    ui->DifficultySpinBox->setValue(Difficulty);
    OptionalFields = Optional_Metadata_Ids.join(",");
    QSqlQuery OptionalValues(DataTex::CurrentTexFilesDataBase);
    OptionalValues.exec(QString("SELECT %1 FROM Database_Files WHERE Id = \"%2\"").arg(OptionalFields,DatabaseFileName));
    QStringList s;
    while(OptionalValues.next()){
        QSqlRecord record = OptionalValues.record();
        for(int i=0; i < record.count(); i++)
        {
            s << record.value(i).toString();
            lineList.at(i)->setText(record.value(i).toString());
        }
    }

    ui->NameLine->setText(DatabaseFileName);
    ui->FieldLine->setText(Field);
    ui->ChapterLine->setText(Chapter);
    ui->SectionLine->setText(Section);
    ui->ExerciseTypeLine->setText(ExerciseType);
    ui->FileTypeLine->setText(FileType);
    ui->PathLine->setText(DatabaseFilePath);
    ui->SolvedLine->setText(Solved);
    ui->DescriptionLine->setText(FileDescription);

    loadImageFile(DatabaseFilePath,LatexFileView);
//    doc->open(DatabaseFilePath);

    QFile file(DatabaseFilePath);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->FileEdit->setText(FileContent);
    UndoTex->setEnabled(false);
    SaveTex->setEnabled(false);
    SolveLatexFile->setEnabled(Solvable>0);
    EditLatexFile->setEnabled(Solvable>-1);
    FileCloneToOtherDB->setEnabled(Solvable>-1);

    QSqlQuery FileBibliographyQuery(CurrentTexFilesDataBase);
    FileBibliographyQuery.exec(QString("SELECT Citation_Key,UseBibliography FROM \"Bibliography\" WHERE \"Citation_Key\" LIKE \"%1%\"").arg(DatabaseFileName));
    ui->BibEntriesCombo->clear();
    while(FileBibliographyQuery.next()){
        ui->BibEntriesCombo->addItem(FileBibliographyQuery.value(0).toString(),QVariant(FileBibliographyQuery.value(1).toString()));
    }
    ui->removeBibEnrty->setEnabled(ui->BibEntriesCombo->count()>0);
//    BibSourceCode(0);
//    on_BibEntriesCombo_currentIndexChanged(FileBibliographyQuery.value(0).toString());

    FilesPreambleCombo->setEnabled(true);
    CurrentPreamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    int index = FilesPreambleCombo->findData(CurrentPreamble);
    FilesPreambleCombo->setCurrentIndex(index);
    QStringList list = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                      .arg(DataTex::CurrentPreamble)
                                                                      ,DataTeX_Settings);
    if(list.count()>0){CurrentPreamble_Content = list.at(0);}
    getActionFromText(CompileMenu,CompileCommands);
}

void DataTex::getActionFromText(QMenu * menu,QToolButton *button)
{
    int k = -1;
    for (int i=0;i<menu->actions().count();i++) {
        if(menu->actions()[i]->toolTip()==CurrentBuildCommand){
            k=i;
        }
    }
    button->setDefaultAction(menu->actions()[k]);
}

int matchingDigits(QString str1, QString str2)
{
    int matches = 0;
    int minSize;
    if (str1.size() < str2.size()){minSize = str1.size();} else {minSize = str2.size();};
    for (int pos = 0; pos < minSize; ++pos)
    {
        if (str1.at(pos) == str2.at(pos)) ++matches;
    }
    return matches;
}

void DataTex::DocumentsTable_selectionChanged()
{
    QItemSelectionModel *select = DocumentsTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    CompileMenuDoc->setEnabled(true);
    PdfLatexDoc->setEnabled(true);
    DeleteDocument->setEnabled(true);
    OpenPathDoc->setEnabled(true);
    InsertFileToDocument->setEnabled(true);
    DocumentFileName = DocumentsTable->model()->data(DocumentsTable->model()->index(row,0)).toString();
    DocFileType = DocumentsTable->model()->data(DocumentsTable->model()->index(row,1)).toString();
    BasicFolder = DocumentsTable->model()->data(DocumentsTable->model()->index(row,2)).toString();
    SubFolder = DocumentsTable->model()->data(DocumentsTable->model()->index(row,3)).toString();
    SubSubFolder = DocumentsTable->model()->data(DocumentsTable->model()->index(row,4)).toString();
    DocumentFilePath = DocumentsTable->model()->data(DocumentsTable->model()->index(row,5)).toString();
    DocumentDate = DocumentsTable->model()->data(DocumentsTable->model()->index(row,6)).toString();
    DocumentContent = DocumentsTable->model()->data(DocumentsTable->model()->index(row,7)).toString();
    DocumentBuildCommand = DocumentsTable->model()->data(DocumentsTable->model()->index(row,9)).toString();
    DocumentNeedsUpdate = DocumentsTable->model()->data(DocumentsTable->model()->index(row,10)).toInt();
    StuffToAddToPreamble = "\\addbibresource{"+DocumentFileName+".bib}";
    DocumentUseBibliography = DocumentsTable->model()->data(DocumentsTable->model()->index(row,12)).toInt();
    ui->DocumentDateTimeEdit->setDateTime(QDateTime::fromString(DocumentDate,"dd/M/yyyy hh:mm"));

    ui->DocumentNameLine->setText(DocumentFileName);
    ui->DocumentType->setText(DocFileType);
    ui->BasicFolder->setText(BasicFolder);
    ui->SubFolder->setText(SubFolder);
    ui->SubsubFolder->setText(SubSubFolder);
    ui->DocumentPathLine->setText(DocumentFilePath);

    QStringList filesFromDatabase =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT File_Id FROM Files_per_Document WHERE Document_Id = '%1'")
            .arg(QFileInfo(DatabaseFilePath).baseName()),CurrentNotesFolderDataBase);
    QStringList ListOfDatabases =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT DISTINCT Files_Database_Source FROM Files_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentNotesFolderDataBase);
    DatabasesInADocument.clear();
    DatabasesInADocument =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Path FROM Databases WHERE FileName IN (\"%1\")").arg(ListOfDatabases.join("\",\""))
            ,DataTeX_Settings);
    DatabasesInADocument.removeDuplicates();

    QFile TexFile(DocumentFilePath);
    QStringList fileNamesInDocument;
    QString path_rem = "%# Database File : ";
    QString database_rem = "%@ Database source: ";
    TexFile.open (QIODevice::ReadOnly | QIODevice::Text);
    QTextStream Line(&TexFile);
    while (!Line.atEnd()){
        QString LineText=Line.readLine();
        if(LineText.contains(path_rem))fileNamesInDocument.append(LineText.remove(path_rem));
//        if(LineText.contains(database_rem))DatabasesInADocument.append(LineText.remove(database_rem));
    }
    TexFile.close();
    QString files = "(\""+fileNamesInDocument.join("\",\"")+"\")";
    QSqlQueryModel * Files = new QSqlQueryModel(this);
    QStringList datalist = {SqlFunctions::ShowFilesInADocument.arg(files,QFileInfo(CurrentDataBasePath).baseName())};
    QString query;
    QSqlQuery FilesQuery(CurrentTexFilesDataBase);
    for (int i=0;i<DatabasesInADocument.count();i++) {
        if(DatabasesInADocument.at(i)!=CurrentDataBasePath) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DatabasesInADocument.at(i),QFileInfo(DatabasesInADocument.at(i)).baseName()));
            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(files,QFileInfo(DatabasesInADocument.at(i)).baseName()));
        }
    }
    query = datalist.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY \"df\".\"Id\" ");
    Files->setQuery(FilesQuery);
    ui->TexFileTable->setModel(Files);
    ui->TexFileTable->show();
//    QSet<QString> FilesMissingFromDatabaseEntries = fileNamesInDocument.toSet().subtract(filesFromDatabase.toSet());
//    QSet<QString> FilesMissingFromDocument = filesFromDatabase.toSet().subtract(fileNamesInDocument.toSet());
//    foreach (const QString &filename, FilesMissingFromDatabaseEntries)
//            qDebug() << " Το αρχείο : "<<  filename <<"λείπει από τη βάση";
//    foreach (const QString &filename, FilesMissingFromDocument)
//            qDebug() << " difference: "<<  filename;
//    if(DocumentsTable->model()->data(DocumentsTable->model()->index(row,7)).toString()=="Mathematics")
    //item.setData(QColor(Qt::red), Qt::FontRole);

    QFile file(DocumentFilePath);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    DocumentContent = text.readAll();
    file.close();

    ui->DocumentContent->setText(DocumentContent);
    SaveTexDoc->setEnabled(false);
    loadImageFile(DocumentFilePath,DocumentView);
    connect(ui->TexFileTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::TeXFilesTable_selection_changed);

    DocumentsPreambleCombo->setEnabled(true);
    QString CurrentFilePreamble = SqlFunctions::Get_StringList_From_Query(QString("SELECT \"Preamble\" FROM \"Documents\" WHERE \"Id\" = \"%1\";")
                              .arg(DocumentFileName),CurrentNotesFolderDataBase).at(0);
    int index = DocumentsPreambleCombo->findData(CurrentFilePreamble);
    DocumentsPreambleCombo->setCurrentIndex(index);
    CurrentPreamble = DocumentsPreambleCombo->currentData().toString();
    setPreamble();

    if(DocumentNeedsUpdate == 1){
        UpdateDocContent->setEnabled(true);
    }
    else{
        UpdateDocContent->setEnabled(false);
    }
    FilePathsInADocument.clear();
    for (int i = 0;i<ui->TexFileTable->model()->rowCount();i++) {
        FilePathsInADocument.append(ui->TexFileTable->model()->data(DocumentsTable->model()->index(i,5)).toString());
    }

    DocOptionalFields = Optional_DocMetadata_Ids.join(",");
    QSqlQuery OptionalValues(DataTex::CurrentNotesFolderDataBase);
    OptionalValues.exec(QString("SELECT %1 FROM Database_Files WHERE Id = \"%2\"").arg(OptionalFields,DocumentFileName));
    QStringList s;
    while(OptionalValues.next()){
        QSqlRecord record = OptionalValues.record();
        for(int i=0; i < record.count(); i++)
        {
            s << record.value(i).toString();
            Doc_lineList.at(i)->setText(record.value(i).toString());
        }
    }

    Exer_List.clear();
    Solutions_List.clear();
    Solved_List.clear();
    Unsolved_List.clear();
    QStringList Ids;
    QStringList solIds;
    QStringList folders;
    QStringList solfolders;
    QStringList Solutions_per_exer;
    Ids<<"SectEx"<<"SectSub"<<"CombEx"<<"CombSub";
    solIds<<"SolSE"<<"SolSS"<<"SolCE"<<"SolCS";
    SolutionsPerExercise.clear();
    for (int i=0;i<fileNamesInDocument.count();i++){
        Solutions_per_exer.clear();
        QString FileType = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,7)).toString();
        if(Ids.contains(FileType)){
            int index = Ids.indexOf(FileType);
            QString exercise = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,5)).toString();
            QString solution = exercise;
//            QString Solution_Exists = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,5)).toString();
//            for (int i=0;i<Ids.count();i++ ) {
                QString folderName = SqlFunctions::Get_StringList_From_Query(
                            SqlFunctions::FolderName.arg(Ids.at(index)),CurrentTexFilesDataBase).at(0);
                folders.append(folderName);
                QString solfolderName = SqlFunctions::Get_StringList_From_Query(
                            SqlFunctions::FolderName.arg(solIds.at(index)),CurrentTexFilesDataBase).at(0);
                solfolders.append(solfolderName);
                solution.replace("-"+Ids.at(index),"-"+solIds.at(index));
                solution.replace(folderName,solfolderName);
//            }
            QString solutionName = QFileInfo(solution).baseName()+"*.tex";
            QDirIterator list(QFileInfo(solution).absolutePath(),QStringList() << solutionName, QDir::Files, QDirIterator::NoIteratorFlags);
            while (list.hasNext()){
                Solutions_per_exer.append(list.next());
            }
            SolutionsPerExercise.append(Solutions_per_exer);
            Exer_List.append(exercise);
//            Solutions_List.append(solved);
        }
    }
//    for (int i=0;i<Exer_List.count();i++) {
//        QFile pathfile;
//        if(!pathfile.exists(Solutions_List.at(i))){
//            Unsolved_List.append(Exer_List.at(i));}
//        if(pathfile.exists(Solutions_List.at(i))){
//            Solved_List.append(Solutions_List.at(i));}
//    }
    if(Exer_List.size()>0){CreateSolutionsDoc->setEnabled(true);}
    else if(Exer_List.size()==0){CreateSolutionsDoc->setEnabled(false);}
    QString solutionsname = QFileInfo(DocumentFilePath).baseName()+"_Solutions";
    QString SolutionsPdfFile = QFileInfo(DocumentFilePath).absolutePath()+QDir::separator()+solutionsname+".pdf";
    QString PdfFile = QFileInfo(DocumentFilePath).absolutePath()+QDir::separator()+QFileInfo(DocumentFilePath).baseName()+".pdf";

    ui->BibPerFileList->clear();
    for(int i=0;i<ui->TexFileTable->model()->rowCount();i++){
        ui->BibPerFileList->addItem(ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,0)).toString());
        ui->BibPerFileList->item(i)->setData(Qt::UserRole,ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,8)).toString());
        ui->BibPerFileList->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        ui->BibPerFileList->item(i)->setCheckState(Qt::Checked);
    }
    QString BibContent;
    connect(ui->BibPerFileList,&QListWidget::itemChanged,this, [=](){
        QString Content;
        for(int i=0;i<ui->BibPerFileList->count();i++){
            if(ui->BibPerFileList->item(i)->checkState() == Qt::Checked){
                Content += ui->BibPerFileList->item(i)->data(Qt::UserRole).toString()+"\n\n";
            }
        }
        ui->DocBibSourceCode->setPlainText(Content);
    });
    for(int i=0;i<ui->BibPerFileList->count();i++){
        BibContent += ui->BibPerFileList->item(i)->data(Qt::UserRole).toString()+"\n\n";
    }
    ui->DocBibSourceCode->setPlainText(BibContent);
    ui->SaveDocBibContent->setEnabled(false);
    ui->UseBibliography->setEnabled(hasBib);
}

//Προσωρινό
//void DataTex::loadImageFile(QString exoFile, qpdfview::MainWindow *view)
//{
//    if (exoFile.isEmpty()) {
//        return;}
//    QString pdfFile = exoFile.replace(".tex",".pdf");
//    if(QFileInfo::exists(pdfFile)){
//        view->open(pdfFile);
//    }
//}
//--

void DataTex::loadImageFile(QString exoFile, PdfViewer *view)
{
    if (exoFile.isEmpty()) {
        return;}
    QString pdfFile = /*"file:///"+*/exoFile.replace(".tex",".pdf");
    if(QFileInfo::exists(pdfFile)){
        view->setCurrentDocument(pdfFile);
    }
    else{
        view->setCurrentDocument(DataTex::GlobalSaveLocation+"No_Pdf.pdf");
    }
}

void DataTex::CompileToPdf()
{
    QAction * action = qobject_cast<QAction*>(sender());
    QString path;
    if(ui->stackedWidget->currentIndex()==0){
        path = DatabaseFilePath;
        SaveText();
        CurrentPreamble = FilesPreambleCombo->currentData().toString();
        setPreamble();
    }
    else{
        path = DocumentFilePath;
        SaveDocText();
        CurrentPreamble = DocumentsPreambleCombo->currentData().toString();
        setPreamble();
    }
    CreateTexFile(path,DocumentUseBibliography,StuffToAddToPreamble);
    BuildDocument(action->data().toString(),path,action->property("args").toStringList(),action->property("ext").toString());
}

void DataTex::Compile()
{
    QAction * action = qobject_cast<QAction*>(sender());
    QString path;
    if(ui->stackedWidget->currentIndex()==0){path = DatabaseFilePath;}
    else{path = DocumentFilePath;}
    BuildDocument(action->data().toString(),path,action->property("args").toStringList(),action->property("ext").toString());
}

void DataTex::CompileAsymptote()
{
    QStringList Files;
    QString path;
    if(ui->stackedWidget->currentIndex()==0){path = DatabaseFilePath;}
    else{path = DocumentFilePath;}
    QString filename = QFileInfo(path).baseName()+"*.asy";
    QDirIterator list(QFileInfo(path).absolutePath(),
                      QStringList() << filename, QDir::Files, QDirIterator::Subdirectories);
    while (list.hasNext()){
    Files.append(list.next());}

    QProcess compileProcess;
    compileProcess.setWorkingDirectory(QFileInfo(path).absolutePath());
    compileProcess.start(DataTex::Asymptote_Command,Files);
    compileProcess.waitForFinished(-1);
}

void DataTex::SaveText()
{
    FileContent = ui->FileEdit->toPlainText();
    QFile file(DatabaseFilePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream ContentStream(&file);
    QString PreviousContent = ContentStream.readAll();
    file.resize(0);
    ContentStream << FileContent;
    file.close();
    SaveTex->setEnabled(false);
    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(DatabaseFileName,FileContent);
    if(PreviousContent != FileContent){
        QSqlQuery needsUpdate(CurrentNotesFolderDataBase);
        needsUpdate.exec(QString("UPDATE Documents SET NeedsUpdate = 1 WHERE Id IN (SELECT Document_Id FROM Files_per_Document WHERE File_Id = \"%1\")").arg(DatabaseFileName));
    }
}

void DataTex::SaveDocText()
{
    DocumentContent = ui->DocumentContent->toPlainText();
    QFile file(DocumentFilePath);
    file.resize(0);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream Line(&file);
    Line << DocumentContent;
    file.close();
    SaveTexDoc->setEnabled(false);
    UndoTexDoc->setEnabled(false);
    DataTex::SaveContentToDatabase(DocumentFilePath,DocumentContent);
}

void DataTex::CreateDatabase()
{
    BaseFolder * NewDatabase = new BaseFolder(this);
    connect(NewDatabase,SIGNAL(newbase(QString,QString,QString,QString)),this,SLOT(CreateNewDatabase(QString,QString,QString,QString)));
    NewDatabase->show();
    NewDatabase->activateWindow();
}

void DataTex::CreateNewDatabase(QString Path,QString FolderName,QString fileName,QString DatabaseType)
{
    QString FullPath = Path+QDir::separator()+FolderName+QDir::separator()+fileName+".db";
    DatabaseStructure(FullPath);
    if(DatabaseType == "Files"){
        AddDatabaseToTree(0,FullPath,FolderName);
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1),0);
    }
    else{
        AddDatabaseToTree(1,FullPath,FolderName);
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(ui->OpenDatabasesTreeWidget->topLevelItem(1)->childCount()-1),0);
    }
}

void DataTex::UpdateCurrentDatabase(QString FullPath)
{
    QSqlQuery SaveData(DataTeX_Settings);
    SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_DataBase'")
                  .arg(QFileInfo(FullPath).baseName()));
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
    database.setDatabaseName(FullPath);
    GlobalFilesDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
    DataTex::CurrentTexFilesDataBase = database;
    DataTex::CurrentDataBasePath = FullPath;
    DataTex::CurrentTexFilesDataBase.open();
    ui->CurrentBaseLabel->setText("Current files database : "+GlobalFilesDatabaseListNames[QFileInfo(CurrentDataBasePath).baseName()]);
}

void DataTex::UpdateCurrentNotesDatabase(QString FullPath)
{
    QSqlQuery SaveData(DataTeX_Settings);
    SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_Notes_Folder'")
                  .arg(QFileInfo(FullPath).baseName()));
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
    database.setDatabaseName(FullPath);
    GlobalDocsDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
    DataTex::CurrentNotesFolderDataBase = database;
    DataTex::CurrentNotesFolderPath = FullPath;
    DataTex::CurrentNotesFolderDataBase.open();
}

void DataTex::AddDatabaseToTree(int row,QString databasePath,QString databaseName)
{
    QTreeWidgetItem * item = new QTreeWidgetItem();
    item->setText(0,databaseName);
    item->setText(1,databasePath);
    item->setText(2,QFileInfo(databasePath).baseName());
    ui->OpenDatabasesTreeWidget->topLevelItem(row)->addChild(item);
}

void DataTex::DeleteFileFromBase()
{
    QCheckBox *cb = new QCheckBox(tr("Delete File"));
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the file %1").arg(DatabaseFileName));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(CurrentTexFilesDataBase);
        deleteQuery.exec(QString("DELETE FROM \"Database_Files\" WHERE \"Id\" = \"%1\"").arg(DatabaseFileName));
        ShowDataBaseFiles();
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabaseFilePath).absolutePath()));}
    }
}

void DataTex::DeleteDocumentFromBase()
{
    QCheckBox *cb = new QCheckBox(tr("Delete File"));
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the file %1").arg(DocumentFileName));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(CurrentNotesFolderDataBase);
        deleteQuery.exec(QString("DELETE FROM \"Documents\" WHERE \"Id\" = \"%1\"").arg(DocumentFileName));
        ShowDocuments();
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DocumentFilePath).absolutePath()));}
    }
}

QString DataTex::SetLatexCommand(QString SQLCommandSetting, QString Command, QAction * Action,QAction * Action2,QStringList args,QString ext)
{
    QProcess *process = new QProcess;
    process->start("which",QStringList()<<Command);
    process->waitForBytesWritten();
    process->waitForFinished(-1);
    QString output = QString(process->readAllStandardOutput()).remove("\n");
    Action->setData(output);
    Action->setProperty("args",args);
    Action->setProperty("ext",ext);
    Action2->setData(output);
    Action2->setProperty("args",args);
    Action2->setProperty("ext",ext);
    QSqlQuery CommandsQuery(DataTeX_Settings);
    CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(output,SQLCommandSetting));
    return output;
}

QString DataTex::GetLatexCommand(QString SQLCommandSetting,QAction * Action,QAction * Action2,QStringList args,QString ext)
{
    QSqlQuery CommandsQuery(DataTeX_Settings);
    QString command;
    CommandsQuery.exec(QString("SELECT Value FROM Initial_Settings WHERE Setting = '%1';").arg(SQLCommandSetting));
    while(CommandsQuery.next()){command = CommandsQuery.value(0).toString();}
    Action->setData(command);
    Action->setProperty("args",args);
    Action->setProperty("ext",ext);
    Action2->setData(command);
    Action2->setProperty("args",args);
    Action2->setProperty("ext",ext);
    return command;
}

void DataTex::on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    CloseDatabasefile->setEnabled(true);
    if(item->parent()){
        QString Database = item->parent()->text(0);
        if(Database == "Latex databases"){
            ui->FilesDatabaseToggle->setChecked(true);
            if(CurrentDataBasePath!=item->text(1)){UpdateCurrentDatabase(item->text(1));}
            on_FilesDatabaseToggle_clicked(true);
            DatabaseStructure(CurrentDataBasePath);
            Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
            Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
            FilterTables_Queries(Database_FileTableFields);
            ShowDataBaseFiles();
            for (int i=0;i<FilesTable->model()->columnCount();i++) {
                FilesTable->setColumnHidden(i,false);
            }
            FilesTable->setColumnHidden(Database_FileTableFieldNames.count(),true);
            loadDatabaseFields();
        }
        else if(Database == "Document databases"){
            ui->DocumentsDatabaseToggle->setChecked(true);
            if(CurrentNotesFolderPath!=item->text(1)){UpdateCurrentNotesDatabase(item->text(1));}
            on_DocumentsDatabaseToggle_clicked(true);
            DatabaseStructure(CurrentNotesFolderPath);
            Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",CurrentNotesFolderDataBase);
            FilterDocuments(Database_DocumentTableColumns);
            ShowDocuments();
            loadDatabaseFields();
        }
        QWidget * w = ui->tabWidget->tabBar()->tabButton(0,QTabBar::RightSide);
        int count = Optional_Metadata_Ids.count();
        if(count>0 && !w){
            MetadataToolButton();
        }
        else if(count==0 && w){
            ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->deleteLater();
            ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, 0);
        }
        else if((count>0 && w)||(count==0 && !w)){
            return;
        }
    }
}

void DataTex::TeXFilesTable_selection_changed()
{
    QItemSelectionModel *select = ui->TexFileTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){ //check if has selection
        row = select->selectedRows().at(0).row();
    }
    QString FilePath = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(row,5)).toString();
    QString File_content;
    QFile file(FilePath);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    File_content = text.readAll();
    file.close();
    ui->FileContent->setText(File_content);
    loadImageFile(FilePath,FileFromDocumentView);
}

void DataTex::updateTableView(QTableView * table,QString QueryText,QSqlDatabase Database,QObject * parent)
{
    QSqlQueryModel * model = new QSqlQueryModel(parent);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(parent);
    QSqlQuery query(Database);
    query.exec(QueryText);
    model->setQuery(query);
    table->setModel(model);
    proxyModel->setSourceModel(model);
    table->show();
    table->setSortingEnabled(true);
}

void DataTex::FilterTables_Queries(QStringList list)
{
    SqlFunctions::ShowAllDatabaseFiles = "SELECT \"df\".\"Id\" ,\"ft\".\"FileType\" "
            ",\"f\".\"Name\" , \"c\".\"Name\" , "
            "replace(group_concat(DISTINCT s.Name),',','-') "
            ",\"se\".\"Exercise_Name\" ,";

    SqlFunctions::FilesTable_UpdateQuery = "SELECT \"df\".\"Id\" ,\"ft\".\"FileType\" ,\"f\".\"Name\" ,"
            "\"c\".\"Name\" , replace(group_concat(DISTINCT s.Name),',','-') ,\"se\".\"Exercise_Name\" ,";
    QStringList Select;
    QStringList DataFields;
    for (int i=6;i<list.count();i++) {
//        FilesTable->model()->setHeaderData(i,Qt::Horizontal,Database_FileTableFields.at(i),Qt::UserRole);
        Select.append("\""+list.at(i)+"\"");
        DataFields.append("(\"df\".\""+list.at(i)+"\" LIKE "+"\"%replace"+list.at(i)+
                          "%\" OR \"df\".\""+list.at(i)+"\" ISNULL)");
    }
    SqlFunctions::FilesTable_UpdateQuery += Select.join(",")+",\"df\".\"FileType\",\"ft\".\"Solvable\"";
    SqlFunctions::ShowAllDatabaseFiles += Select.join(",")+",\"df\".\"FileType\",\"ft\".\"Solvable\"";
    SqlFunctions::ShowAllDatabaseFiles += "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\" "
                                          "ON \"ft\".\"Id\" = \"df\".\"FileType\" JOIN \"Fields\" \"f\" ON \"f\".\"Id\" = \"df\".\"Field\" "
                                          "LEFT JOIN \"Chapters\" \"c\" ON \"c\".\"Id\" = \"df\".\"Chapter\" "
                                          "JOIN \"Sections\" \"s\" ON \"s\".\"Id\" = \"df\".\"Section\""
                                          "LEFT JOIN \"Sections_Exercises\" \"se\" ON \"se\".\"Exercise_Id\" = \"df\".\"ExerciseType\" "
                                          "GROUP BY \"df\".\"Id\""
                                          "ORDER BY \"df\".rowid;";
    SqlFunctions::FilesTable_UpdateQuery +="FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" ft ON \"ft\".\"Id\" = \"df\".\"FileType\" "
                             "JOIN \"Fields\" \"f\" ON \"f\".\"Id\" = \"df\".\"Field\" LEFT JOIN \"Chapters\" \"c\" ON \"c\".\"Id\" = \"df\".\"Chapter\" "
                             "JOIN \"Sections\" \"s\" ON \"s\".\"Id\" = \"df\".\"Section\" "
                             "LEFT JOIN \"Sections_Exercises\" \"se\" ON \"se\".\"Exercise_Id\" = \"df\".\"ExerciseType\" "
                             "WHERE \"df\".\"Id\" LIKE \"%replaceId%\" "
                             " AND \"ft\".\"FileType\" LIKE \"%replaceFileType%\" "
                             " AND \"f\".\"Name\" LIKE \"%replaceField%\" "
                             " AND (\"c\".\"Name\" LIKE \"%replaceChapter%\" OR \"df\".\"Chapter\" ISNULL) "
                             " AND \"s\".\"Name\" LIKE \"%replaceSection%\" "
                             " AND CASE WHEN (SELECT COUNT(*) FROM \"Sections_Exercises\")>0 "
                             " THEN (\"se\".\"Exercise_Name\" LIKE \"%replaceExerciseType%\" OR \"df\".\"ExerciseType\" ISNULL OR \"df\".\"ExerciseType\" = \"-\") "
                             " ELSE 1=1 "
                             " END AND ";
    SqlFunctions::FilesTable_UpdateQuery += DataFields.join(" AND ");
    SqlFunctions::FilesTable_UpdateQuery += "GROUP BY df.Id "
                              "ORDER BY df.rowid;";
//    qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
//    qDebug()<<SqlFunctions::ShowAllDatabaseFiles;
}

void DataTex::FilterDocuments(QStringList list)
{
    SqlFunctions::FilterDatabaseDocuments = "SELECT * FROM \"Documents\" WHERE ";
    QStringList filters;
    for (int i=0;i<list.count();i++) {
         filters.append("(\""+list.at(i)+"\" LIKE "+"\"%replace"+list.at(i)+
                "%\" OR \""+list.at(i)+"\" ISNULL)");
    }
    SqlFunctions::FilterDatabaseDocuments += filters.join(" AND ");
}

void DataTex::LoadTableHeaders(QTableView * table,QStringList list)
{
    for (int i=0;i<list.count();i++) {
        table->model()->setHeaderData(i,Qt::Horizontal,list.at(i),Qt::DisplayRole);
    }
}

void DataTex::on_DatabaseStructureTreeView_clicked(const QModelIndex &index)
{
    QModelIndex ix = index;
    int depth = 0;
    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
      depth++;
    }
    int result;
    QDir dir(CurrentDataBasePath);
    for(result=0;dir.cdUp();++result){}
//    FilesTable->setFilter(depth-result+2,index.data().toString());
}

void DataTex::on_ComboCount_currentIndexChanged(int index)
{
    updateTableView(ui->CountFilesTable,ui->ComboCount->currentData().toString(),CurrentTexFilesDataBase,this);
}

QString DataTex::NewFileText(QString fileName,QString FileContent)
{
    QString text;
    text = "%# Database File : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ Database source: "+QFileInfo(CurrentDataBasePath).baseName()+"\n";
    text += FileContent+"\n";
    text += "%# End of file "+QFileInfo(fileName).baseName();
    return text;
}

QString DataTex::NewFileText(QString fileName,QString FileContent,QSqlDatabase database)
{
    QString text;
    text = "%# Database File : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ Database source: "+QFileInfo(database.databaseName()).baseName()+"\n";
    text += FileContent+"\n";
    text += "%# End of file "+QFileInfo(fileName).baseName();
    return text;
}

void DataTex::setPreamble()
{
    QStringList list = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                      .arg(DataTex::CurrentPreamble)
                                                                      ,DataTeX_Settings);
    if(list.count()>0){CurrentPreamble_Content = list.at(0);}
}

void DataTex::Preamble_clicked()
{
    AddLine * newPreamle = new AddLine(this);
    connect(newPreamle,SIGNAL(grammhline(QStringList)),this,SLOT(AddPreamble(QStringList)));
    newPreamle->show();
    newPreamle->activateWindow();
}

void DataTex::AddPreamble(QStringList preamble)
{
    QSqlQuery AddPreamble(DataTeX_Settings);
    AddPreamble.exec(QString("INSERT OR IGNORE INTO \"Preambles\" (\"Id\",\"Name\",\"Preamble_Content\") VALUES (\"%1\",\"%2\",\"%3\")")
                     .arg(preamble[1],preamble[0],preamble[2]));
}

void DataTex::FunctionInProgress()
{
    QMessageBox msgBox;
    msgBox.setText("This function will soon be completed.");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void DataTex::OpenLoadDatabase()
{
    QStringList FileDatabaseIds = SqlFunctions::Get_StringList_From_Query("SELECT \"FileName\" FROM \"DataBases\"",DataTeX_Settings);
    QStringList FileDatabaseNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"DataBases\"",DataTeX_Settings);
    QStringList DocumentsDatabaseIds = SqlFunctions::Get_StringList_From_Query("SELECT \"FileName\" FROM \"Note_Folders\"",DataTeX_Settings);
    QStringList DocumentsDatabaseNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"Note_Folders\"",DataTeX_Settings);
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Database"), QDir::homePath(), tr("Sqlite Databases (*.db)"));
    if(filePath.isEmpty()) {
        return;
    }
    if(FileDatabaseIds.contains(QFileInfo(filePath).baseName()) || DocumentsDatabaseIds.contains(QFileInfo(filePath).baseName())){
        QMessageBox::warning( this,tr("Error"),tr("This database already exists."),
                     QMessageBox::Ok);
    }
    else{
        QStringList list = QFileInfo(filePath).absolutePath().split(QDir::separator());
        QString folder = list.last();
        OpenDatabaseInfo(filePath,folder);
    }
}

void DataTex::OpenDatabaseInfo(QString filePath,QString FolderName)
{
    QString baseName = QFileInfo(filePath).baseName();
    QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE",baseName);
    newDatabase.setDatabaseName(filePath);
    newDatabase.open();
    QSqlQuery SaveData(DataTeX_Settings);
    QStringList Tables = SqlFunctions::Get_StringList_From_Query("SELECT name FROM main.sqlite_master WHERE type='table';",newDatabase);
    newDatabase.close();
    if(Tables.count()>0){
        if(Tables.contains("Database_Files")){
            AddDatabaseToTree(0,filePath,FolderName);
            newDatabase.setDatabaseName(filePath);
            DataTex::CurrentTexFilesDataBase = newDatabase;
            DataTex::CurrentDataBasePath = filePath;
            GlobalFilesDatabaseList.insert(QFileInfo(filePath).baseName(),newDatabase);
            DataTex::CurrentTexFilesDataBase.open();
            SaveData.exec(QString("INSERT INTO \"Databases\" (\"FileName\",\"Name\",\"Path\") VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_DataBase'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
            QStringList BibIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Bibliography'",CurrentTexFilesDataBase);
            QStringList BibNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Bibliography'",CurrentTexFilesDataBase);

            QSqlQuery add(DataTeX_Settings);
            for (int i=0;i<MetadataIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO \"Metadata\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO \"Metadata_per_Database\" (\"Database_FileName\",\"Metadata_Id\") VALUES (\""+baseName+"\",\""+MetadataIds.at(i)+"\")");
            }
            for (int i=0;i<BibIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO \"Bibliography\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+BibIds.at(i)+"\",\""+BibNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO \"Bibliographic_Fields_per_Database\" (\"Database\",\"Bibliographic_Field\") VALUES (\""+baseName+"\",\""+BibIds.at(i)+"\")");
            }
            on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1),0);
        }
        else if(Tables.contains("Documents")){
            AddDatabaseToTree(1,filePath,FolderName);
            newDatabase.setDatabaseName(filePath);
            DataTex::CurrentNotesFolderDataBase = newDatabase;
            DataTex::CurrentNotesFolderPath = filePath;
            GlobalDocsDatabaseList.insert(QFileInfo(filePath).baseName(),newDatabase);
            DataTex::CurrentNotesFolderDataBase.open();
            SaveData.exec(QString("INSERT INTO \"Notes_Folders\" (\"FileName\",\"Name\",\"Path\") VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_Notes_Folder'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
            QSqlQuery add(DataTeX_Settings);
            for (int i=0;i<MetadataIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO \"DocMetadata\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO \"DocMetadata_per_Database\" (\"Database_FileName\",\"Metadata_Id\") VALUES (\""+baseName+"\",\""+MetadataIds.at(i)+"\")");
            }
            on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(ui->OpenDatabasesTreeWidget->topLevelItem(1)->childCount()-1),0);
        }
        else{
            QMessageBox::warning( this,tr("Error"),tr("This isn't a LaTeX file or\nLaTeX document database."),
                         QMessageBox::Ok);
        }
    }
}

void DataTex::RemoveCurrentDatabase()
{
    QString DatabaseName;
    QString DatabasePath;
    QString DatabaseType;
    QString DatabaseTable;
    if(ui->stackedWidget->currentIndex()==0){
        DatabaseName = QFileInfo(CurrentDataBasePath).baseName();
        DatabasePath = CurrentDataBasePath;
        DatabaseType = "Databases";
        DatabaseTable = "Metadata_per_Database";
    }
    else{
        DatabaseName = QFileInfo(CurrentNotesFolderPath).baseName();
        DatabasePath = CurrentNotesFolderPath;
        DatabaseType = "Notes_Folders";
        DatabaseTable = "DocMetadata_per_Database";
    }
    QList<QTreeWidgetItem *> DeleteItem = ui->OpenDatabasesTreeWidget->findItems(DatabaseName,Qt::MatchExactly | Qt::MatchRecursive,2);
    QCheckBox *cb = new QCheckBox(tr("Delete File"));
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the database %1").arg(DeleteItem[0]->text(0)));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(DataTeX_Settings);
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM %2 WHERE FileName = \"%1\"").arg(DatabaseName,DatabaseType));
        deleteQuery.exec(QString("DELETE FROM %2 WHERE Database_FileName = \"%1\"").arg(DatabaseName,DatabaseTable));

        int parent = ui->OpenDatabasesTreeWidget->indexOfTopLevelItem(DeleteItem[0]->parent());
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(parent)->child(0),0);
        delete DeleteItem[0];
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabasePath).absolutePath()));}
    }
}

void DataTex::UpdateDocument()
{
    UpdateDocumentContent * upDoc = new UpdateDocumentContent(this,DocumentFilePath,FilePathsInADocument,DatabasesInADocument);
//    connect(upDoc,SIGNAL(),this,SLOT());
    upDoc->show();
    upDoc->activateWindow();
}

void DataTex::on_DatabaseStructureTreeView_doubleClicked(const QModelIndex &index)
{
    QString file = model->filePath(index);
    QDesktopServices::openUrl(QUrl("file:///"+file));
}

void DataTex::AddFileToDatabase()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open existing file"), QDir::homePath(), tr("Latex file (*.tex)"));
    if(filePath.isEmpty()) {
        return;
    }
    QFile Tex(filePath);
    QString database;
    Tex.open (QIODevice::ReadOnly);
    QTextStream Content(&Tex);
    while (!Content.atEnd()){
        QString LineText=Content.readLine();
        if(LineText.contains("%@ Database source: ")){
            database = LineText.remove("%@ Database source: ");
            break;
        }
    }
    Tex.close();
    if(database.isEmpty()){
        QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Error"),tr("This file was doens't have a database source.\n"
                                                  "Do you wish to open it anyway?"),
                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        };
    }
    else if(database != QFileInfo(CurrentDataBasePath).baseName()){
        QMessageBox::StandardButton resBtn =
        QMessageBox::warning( this,tr("Error"),tr("This file was created from another database\n"
                                                  "and some metadada values may differ.\n"
                                                  "Do you wish to add this file to the current database anyway?"),
                              QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        };
    }

    QString csvFile = filePath;
    csvFile.replace(".tex",".csv");

    QFile CSV(csvFile);
    if(QFileInfo::exists(csvFile)){
        CSV.open (QIODevice::ReadOnly);
        QTextStream Content(&CSV);
        QStringList MetadataIds;
        QStringList MetadataValues;
        QString Id;
        QString Value;
        int line = -1;
        while (!Content.atEnd()){
            line++;
            QString LineText=Content.readLine();
            QStringList fields = LineText.split(",");
            Id = fields.at(0);
            if(line == FileData::Section || line == FileData::Bibliography || line == FileData::FileContent){
                fields.removeAt(0);
                Value = fields.join(",");
            }
            else{
                Value = fields.at(1);
            }
            Value.replace("\\qt_endl","\n");
            MetadataIds.append(Id);
            MetadataValues.append(Value);
        }
        CSV.close();
//        {FileType,Field,Chapter,ExerciseType,QString::number(Difficulty),
//                         CurrentPreamble,CurrentBuildCommand,FileDescription,FileContent,FileTypeId};
        MetadataValues[FileData::Field]=SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM Fields WHERE Id = \"%1\"").arg(MetadataValues[FileData::Field]),CurrentTexFilesDataBase)[0];
        MetadataValues[FileData::Chapter]=SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM Chapters WHERE Id = \"%1\"").arg(MetadataValues[FileData::Chapter]),CurrentTexFilesDataBase)[0];
        MetadataValues[FileData::Section]=SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM Sections WHERE Id = \"%1\"").arg(MetadataValues[FileData::Section]),CurrentTexFilesDataBase)[0];
        MetadataValues[FileData::ExerciseType]=SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM Exercise_Types WHERE Id = \"%1\"").arg(MetadataValues[FileData::ExerciseType]),CurrentTexFilesDataBase)[0];
        MetadataValues.move(FileData::FileContent,FileData::FileDescription);
        MetadataValues.removeAt(FileData::Bibliography);
        MetadataValues.removeAt(FileData::Solved);
        MetadataValues.removeAt(FileData::Date);
        MetadataValues.removeAt(FileData::Path);
        QStringList Sections = {MetadataValues.at(FileData::Section)};//Comb
        MetadataValues.append(MetadataValues.at(FileData::FileType));
        MetadataValues.removeAt(FileData::Section);
        MetadataValues.removeAt(FileData::Id);
        NewDatabaseFile * Edit = new NewDatabaseFile(this,MetadataValues,Sections,true,QFileInfo(filePath).baseName());
        connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
                SLOT(EditNewBaseFile(QString,QString)));
        Edit->show();
        Edit->activateWindow();
    }
}

QString DataTex::BibSourceCode(int index)
{
    QString sourceCode = ui->DocumentTypeCombo->itemData(index).toString()+"{"+BibValues[0]+",\n";
    int k = -1;
    for (int i=2;i<BibValues.count();i++) {
        if(!Bib_lineList[i-2]->text().isEmpty() && !Bib_lineList[i-2]->text().isNull()){
            k=i;
            sourceCode += "\t"+BibFieldIds[k]+" = {"+Bib_lineList[k-2]->text()+"}"+",\n";
        }
    }
    sourceCode += "}";
    ui->BibSourceCode->setPlainText(sourceCode);
    return sourceCode;
}

void DataTex::on_BibEntriesCombo_currentIndexChanged(QString text)
{
    QSqlQuery FileBibliographyQuery(CurrentTexFilesDataBase);
    FileBibliographyQuery.exec(QString("SELECT * FROM \"Bibliography\" WHERE \"Citation_Key\" = \"%1\"").arg(text));
    BibValues.clear();
    while(FileBibliographyQuery.next()){
        QSqlRecord record = FileBibliographyQuery.record();
        for(int i=0; i < record.count()-1; i++)
        {
            BibValues << record.value(i).toString();
            if(i>1){
                Bib_lineList.at(i-2)->setText(record.value(i).toString());
            }
        }
    }
    ui->UseEntry->setChecked(ui->BibEntriesCombo->currentData().toInt());
    ui->CitationKeyLine->setText(text);
    if(BibValues.count()>0){
        int doctype = ui->DocumentTypeCombo->findData(BibValues.at(1));
        ui->DocumentTypeCombo->setCurrentIndex(doctype);
        BibSourceCode(0);
    }
}

void DataTex::MetadataToolButton()
{
    tb = new QToolButton();
    tb->setCheckable(true);
    tb->setIcon(QIcon(":/images/document-edit.svg"));
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
    tb->setEnabled(ui->tabWidget->currentIndex()==0);
    connect(tb,&QToolButton::toggled,this,[=](bool checked){
        if(!checked){
            tb->setIcon(QIcon(":/images/document-edit.svg"));
            QStringList list;
            QString query = "UPDATE \"Database_Files\" SET ";
            for (int i=0;i<lineList.count();i++) {
                lineList.at(i)->setReadOnly(true);
                labelList.at(i)->setText(labelList.at(i)->text().remove("*"));
                labelList.at(i)->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
                ui->DifficultySpinBox->setReadOnly(true);
                ui->DifficultySpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
                list.append("\""+Optional_Metadata_Ids[i]+"\" = \""+lineList[i]->text()+"\" ");
            }
            query += list.join(",");
            query += " WHERE \"Id\" = \""+DatabaseFileName+"\"";
            QSqlQuery update(CurrentTexFilesDataBase);
            update.exec(query);
        }
        else{
            tb->setIcon(QIcon(":/images/document-save.svg"));
            for (int i=0;i<lineList.count();i++) {
                lineList.at(i)->setReadOnly(false);
                labelList.at(i)->setText(labelList.at(i)->text()+"*");
            }
        }
    });
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[=](int i){
        if(ui->tabWidget->tabBar()->tabButton(0,QTabBar::RightSide)){
            tb->setEnabled(i==0);
        }
    });
}

Qt::ItemFlags DataTex::flags(const QModelIndex &index) const
{
//    if (!index.isValid())
//        return Qt::ItemIsEnabled;

    return Qt::ItemIsEditable;
}

bool DataTex::setData(const QModelIndex &index,const QVariant &value, int role)
{
//    if (index.isValid() && role == Qt::EditRole) {
//        QModelIndex topLeft = index(0, 0);
//        QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);

//        emit dataChanged(topLeft, bottomRight);
//        return true;
//    }
    return false;
}

QVariant DataTex::data(const QModelIndex &index, int role) const
{
//    if (!index.isValid())
//        return QVariant();

//    if (index.row() >= stringList.size())
//        return QVariant();

//    if (role == Qt::DisplayRole || role == Qt::EditRole)
//        return stringList.at(index.row());
//    else
        return QVariant();
}

void DataTex::on_addBibEntry_clicked()
{
    QString count = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT COUNT(Id) FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(DatabaseFileName),CurrentTexFilesDataBase).at(0);
    int filecount = 0;
    if(count == "0"){
        filecount = 1;
    }
    else{
        filecount = count.toInt()+1;
    }

    QString newEntry = DatabaseFileName+"_"+QString::number(filecount);
    QString query = "INSERT INTO \"Bibliography\" (\"Citation_Key\",\"Document_Type\") VALUES(\"%1\",\"@article\")";
    QSqlQuery BibliographyQuery(CurrentTexFilesDataBase);
    BibliographyQuery.exec(QString(query).arg(newEntry));
    ui->BibEntriesCombo->addItem(newEntry);
    on_BibEntriesCombo_currentIndexChanged(newEntry);
}

void DataTex::StretchColumns(QTableView * Table, float stretchFactor)
{
    QFont myFont("Noto Sans", 10);
    QFontMetrics fm(myFont);
    for (int c = 0; c < Table->horizontalHeader()->count(); ++c)
    {
        QString str = Table->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Table->setColumnWidth(c,width);
        Table->horizontalHeader()->setMinimumSectionSize(100);
    }
}

void DataTex::FileClone(QString filePath,QString FileContent)
{
    QStringList metadata = {"","","","","","","","",FileContent,"","",""};
    QString count = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT COUNT(Id) FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(QFileInfo(filePath).baseName()),CurrentTexFilesDataBase).at(0);
    int SolutionsCount = (count == "0") ? 0 : count.toInt();
    QMap<QString,QString> SolMap;
    QSqlQuery SolutionContents(CurrentTexFilesDataBase);
    SolutionContents.exec(QString("SELECT FileContent FROM Database_Files WHERE Solved = \"%1\"").arg(QFileInfo(filePath).baseName()));
    while(SolutionContents.next()){
        QString text = NewDatabaseFile::ClearMetadataFromContent(SolutionContents.value(0).toString());
        SolMap.insert("Sol"+QString::number(SolutionContents.at()),text);
    }
    NewDatabaseFile * cloneFile = new NewDatabaseFile(this,metadata,{},false,filePath,true);
    connect(cloneFile,&NewDatabaseFile::acceptClone,this,[=](QString Database,QString newFilePath,QString newFileContent){
        UpdateCurrentDatabase(Database);
        EditNewBaseFile(newFilePath,newFileContent);

        /* Copy any existing solutions of the chosen file
        CloneSolution = newFilePath;
        CloneContent = newFileContent;
        QHash<QString,QString> FileTypeSolIds;
        QHash<QString,QString> FileTypeFolders;
        QHash<QString,QString> FileTypeFoldersSol;
        QSqlQuery FileTypes(GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
        FileTypes.exec("SELECT a.Id,b.Id,a.FolderName,b.FolderName,a.FileType FROM "
                       "(SELECT FileType,Id,FolderName FROM FileTypes WHERE Solvable = 1) as a "
                       "JOIN (SELECT FileType,Id,FolderName,BelongsTo FROM FileTypes WHERE Solvable = -1) as b "
                       "ON a.Id=b.BelongsTo");
        while(FileTypes.next()){
            FileTypeSolIds.insert(FileTypes.value(0).toString(),FileTypes.value(1).toString());
            FileTypeFolders.insert(FileTypes.value(0).toString(),FileTypes.value(2).toString());
            FileTypeFoldersSol.insert(FileTypes.value(0).toString(),FileTypes.value(3).toString());
        }

        CloneSolution.replace("-"+FileTypeId,"-"+FileTypeSolIds[FileTypeId]);
        CloneSolution.replace(FileTypeFolders[FileTypeId],FileTypeFoldersSol[FileTypeId]);

        QDir SolutionDir(QFileInfo(CloneSolution).absolutePath());
        if (!SolutionDir.exists()){
            SolutionDir.mkpath(".");}
        for(int i=1;i<SolutionsCount+1;i++){
            QString newSolution = CloneSolution;
            QFile::copy(newFilePath,newSolution.replace(".tex","-"+QString::number(i)+".tex"));
            QFile file(newSolution);
            file.open(QIODevice::ReadWrite);
            QTextStream stream(&file);
            QString text;
            while(!stream.atEnd()){
                QString line = stream.readLine()+"\n";
                if(line.startsWith("%# Database File :") || line.startsWith("%# End of file")){
                    line.replace("-"+FileTypeId,"-"+FileTypeSolIds[FileTypeId]);
                }
                text += line;
            }
            text.replace(NewDatabaseFile::ClearMetadataFromContent(CloneContent),SolMap["Sol"+QString::number(i-1)]);
            file.resize(0);
            stream<<text;
            file.close();

//            text.replace(NewDatabaseFile::ClearMetadataFromContent(SolMap[SolutionsCount]))
        }*/
    });
    cloneFile->show();
    cloneFile->activateWindow();
//    QFile::copy("/home/spyros/Βάση δεδομένων - Β Λυκείου/Άλγεβρα/Συστήματα/Γραμμικά συστήματα/Exercises/Μέθοδος αντικατάστασης/DTX-Alg-Syst-GrSys-MethAnt-SectEx2.tex",
//                "/home/spyros/Βάση δεδομένων - Β Λυκείου/Άλγεβρα/Συστήματα/Γραμμικά συστήματα/Exercise solutions/Μέθοδος αντικατάστασης/DTX-Alg-Syst-GrSys-MethAnt-SolSE2.tex");
}

