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
#include <QTableWidgetItem>
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
#include "csvfunctions.h"
#include "bibentry.h"
#include "bibauthorseditors.h"



QSqlDatabase DataTex::DataTeX_Settings = QSqlDatabase::addDatabase("QSQLITE","Settings");
QSqlDatabase DataTex::Bibliography_Settings = QSqlDatabase::addDatabase("QSQLITE","BibSettings");
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
QHash<QString,QStringList> DataTex::Optional_Metadata_Ids;
QHash<QString,QStringList> DataTex::Optional_Metadata_Names;
QHash<QString,QStringList> DataTex::Optional_DocMetadata_Ids;
QHash<QString,QStringList> DataTex::Optional_DocMetadata_Names;
QStringList DataTex::DocTypesIds;
QStringList DataTex::DocTypesNames;


DataTex::DataTex(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataTex)
{
    QTranslator translator;
    translator.load(":/languages/DataTex_el_GR.qm");
    QCoreApplication::installTranslator(&translator);
    ui->setupUi(this);
    QScreen * screen = QGuiApplication::primaryScreen();
    QRect  screenRect = screen->geometry();
    resize(0.9*screenRect.width(),0.9*screenRect.height());
    move(QPoint(0.05*screenRect.width(),0.05*screenRect.height()));
    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_20->addWidget(FilesTable,2,0,1,5);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    FilesTable->setAlternatingRowColors(true);
    DocumentsTable = new ExtendedTableWidget(this);
    ui->gridLayout_31->addWidget(DocumentsTable,2,0);
    DocumentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentsTable->horizontalHeader()->setSectionsClickable(true);
    DocumentsTable->setAlternatingRowColors(true);
    BibliographyTable = new ExtendedTableWidget(this);
    ui->gridLayout_21->addWidget(BibliographyTable,1,0,1,4);
    BibliographyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    BibliographyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    BibliographyTable->horizontalHeader()->setSectionsClickable(true);
    BibliographyTable->setAlternatingRowColors(true);

    CreateMenus_Actions();
    CreateToolBars();
    CreateBuildCommands();
    ui->FilesDatabaseToggle->setChecked(true);
    on_FilesDatabaseToggle_clicked(true);//Set Latex file database page as default
    SettingsDatabase_Variables();//Database names and variables, Latex commands
//    QString language = SqlFunctions::Get_StringList_From_Query(QString("SELECT Value FROM Initial_Settings WHERE Setting = 'Language'"),DataTeX_Settings)[0];
//    if(language == "greek"){
//        QTranslator translator;
//        translator.load(":/languages/DataTex_el_GR.qm");
//        QCoreApplication::installTranslator(&translator);
//    }
    //------Load databases and add them to the treewidget
    QStringList FilesDatabases = SqlFunctions::Get_StringList_From_Query("SELECT Path FROM DataBases",DataTeX_Settings);
    QStringList FilesDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM DataBases",DataTeX_Settings);
    for (int i=0;i<FilesDatabases.count();i++ ) {
        AddDatabaseToTree(0,FilesDatabases.at(i),FilesDatabasesNames.at(i));
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FilesDatabases[i]).baseName());
        database.setDatabaseName(FilesDatabases[i]);
        database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
        GlobalFilesDatabaseList.insert(QFileInfo(FilesDatabases[i]).baseName(),database);
        GlobalFilesDatabaseListNames.insert(QFileInfo(FilesDatabases[i]).baseName(),FilesDatabasesNames[i]);
        database.open();

        QStringList list1;
        QStringList list2;
        QSqlQuery Select_DataBase_Optional_Metadata(DataTeX_Settings);
        Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDataBaseOptionalFields)
                                               .arg(QFileInfo(FilesDatabases[i]).baseName()));
        while(Select_DataBase_Optional_Metadata.next()){
            list1.append(Select_DataBase_Optional_Metadata.value(0).toString());
            list2.append(Select_DataBase_Optional_Metadata.value(1).toString());
        }
        Optional_Metadata_Ids.insert(QFileInfo(FilesDatabases[i]).baseName(),list1);
        Optional_Metadata_Names.insert(QFileInfo(FilesDatabases[i]).baseName(),list2);
    }
    QStringList DocumentDatabases = SqlFunctions::Get_StringList_From_Query("SELECT Path FROM Notes_Folders",DataTeX_Settings);
    QStringList DocumentDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Notes_Folders",DataTeX_Settings);
    for (int i=0;i<DocumentDatabases.count();i++ ) {
        AddDatabaseToTree(1,DocumentDatabases.at(i),DocumentDatabasesNames.at(i));
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(DocumentDatabases[i]).baseName());
        database.setDatabaseName(DocumentDatabases[i]);
        database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
        GlobalDocsDatabaseList.insert(QFileInfo(DocumentDatabases[i]).baseName(),database);
        GlobalDocsDatabaseListNames.insert(QFileInfo(DocumentDatabases[i]).baseName(),DocumentDatabasesNames[i]);
        database.open();

        QStringList list1;
        QStringList list2;
        QSqlQuery Doc_Select_DataBase_Optional_Metadata(DataTeX_Settings);
        Doc_Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDocDataBaseOptionalFields)
                                               .arg(QFileInfo(DocumentDatabases[i]).baseName()));
        while(Doc_Select_DataBase_Optional_Metadata.next()){
            list1.append(Doc_Select_DataBase_Optional_Metadata.value(0).toString());
            list2.append(Doc_Select_DataBase_Optional_Metadata.value(1).toString());
        }
        Optional_DocMetadata_Ids.insert(QFileInfo(FilesDatabases[i]).baseName(),list1);
        Optional_DocMetadata_Names.insert(QFileInfo(FilesDatabases[i]).baseName(),list2);
    }
    //-------------------------------------------------
    QString filesdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentDataBase);
    QString docsdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentNotesFolderBase);
    CurrentTexFilesDataBase = QSqlDatabase::database(filesdb);
    CurrentNotesFolderDataBase = QSqlDatabase::database(docsdb);
    CurrentDataBasePath = CurrentTexFilesDataBase.databaseName();
    CurrentNotesFolderPath = CurrentNotesFolderDataBase.databaseName();
    DatabaseStructure(CurrentDataBasePath);// Show database folder structure in tree view

    ui->CurrentBaseLabel->setText(GlobalFilesDatabaseListNames[QFileInfo(CurrentDataBasePath).baseName()]);
    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
    Database_DocTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",CurrentNotesFolderDataBase);
    BibliographyTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
//    BibliographyTableColumns.append({"AuthorConcat","EditorConcat","TranslatorConcat"});
//    BibliographyTableColumns.move(BibliographyTableColumns.count()-1,3);
//    BibliographyTableColumns.move(BibliographyTableColumns.count()-1,3);
//    BibliographyTableColumns.move(BibliographyTableColumns.count()-1,3);

    FilterTables_Queries(Database_FileTableFields);
    FilterDocuments(Database_DocumentTableColumns);
    FilterBibliographyTable(BibliographyTableColumns);
    ShowDataBaseFiles();//Load and preview latex files database table
    ShowDocuments();//Load and preview documents database table
    ShowBibliography();//Load and preview bibliography table
//    BibliographyTable->horizontalHeader()->moveSection(BibliographyTable->model()->columnCount()-1,3);
//    BibliographyTable->horizontalHeader()->moveSection(BibliographyTable->model()->columnCount()-1,3);
//    BibliographyTable->horizontalHeader()->moveSection(BibliographyTable->model()->columnCount()-1,3);

    QSqlQuery bibIdsNames(Bibliography_Settings);
    bibIdsNames.exec("SELECT Id,Name FROM Bibliography_Fields WHERE Basic = '1' ORDER BY ROWID");
    while(bibIdsNames.next()){
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(bibIdsNames.value(1).toString(),this);
        QLineEdit * line = new QLineEdit(this);
        Bib_ValueList.insert(bibIdsNames.value(0).toString(),line);
        line->setText(bibIdsNames.value(0).toString());
        ui->verticalLayout_13->addLayout(hbox);
        hbox->addWidget(label,0);
        hbox->addWidget(line,1);
        line->setAlignment(Qt::AlignRight);
        line->setReadOnly(true);
    }
    Bib_ValueList["isbn"]->setInputMask("999-999-9999-99-9");
    Bib_ValueList["issn"]->setInputMask("999-999-9999-99-9");
    bibIdsNames.exec("SELECT Id,Name FROM Bibliography_Fields WHERE Basic = '0' ORDER BY ROWID");
    while(bibIdsNames.next()){
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(bibIdsNames.value(1).toString(),this);
        QLineEdit * line = new QLineEdit(this);
        Bib_ValueList_Opt.insert(bibIdsNames.value(0).toString(),line);
        line->setText(bibIdsNames.value(0).toString());
        ui->verticalLayout_13->addLayout(hbox);
        hbox->addWidget(label,0);
        hbox->addWidget(line,1);
        line->setAlignment(Qt::AlignRight);
        line->setReadOnly(true);
    }

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

    BibFileView = new PdfViewer(this);
    ui->gridLayout_28->addWidget(BibFileView);
    BibFileView->show();

    BibFileView_2 = new PdfViewer(this);
    ui->verticalLayout_5->addWidget(BibFileView_2);
    BibFileView_2->show();

    DocDependenciesFileView = new PdfViewer(this);
    ui->gridLayout_35->addWidget(DocDependenciesFileView);
    DocDependenciesFileView->show();
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
    ui->splitter_5->setSizes(QList<int>({400, 1}));
    ui->splitter_6->setSizes(QList<int>({250, 150}));
    ui->splitter_7->setSizes(QList<int>({300, 300}));
    ui->splitter_8->setStretchFactor(1, 3);
    ui->splitter_11->setSizes(QList<int>({400, 1}));
    ui->splitter_12->setSizes(QList<int>({400, 1}));

    //---- Load preambles -----------------------
    FilesPreambleCombo = new QComboBox(this);
    DocumentsPreambleCombo = new QComboBox(this);
    QSqlQuery LoadPreambles(DataTeX_Settings);
    LoadPreambles.exec("SELECT Id,Name,Preamble_Content FROM Preambles;");
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
    LoadCountCombo(0);

    if(Optional_Metadata_Ids[QFileInfo(CurrentDataBasePath).baseName()].count()>0){
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
    int columns = FilesTable->model()->columnCount();
    FilesTable->setColumnHidden(columns-1,true);
    FilesTable->setColumnHidden(columns-2,true);
    FilesTable->setColumnHidden(columns-3,true);


    connect(ui->tabWidget_2,&QTabWidget::currentChanged,this,[=](int i){
       dtb->setEnabled(i==1);
    });
//    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[=](int i){
//       bibtb->setEnabled(i==1);
//    });

    QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = \"SaveLocation\"",DataTeX_Settings);
    GlobalSaveLocation = (list.size()>0 && !list[0].isEmpty()) ? list[0] : QDir::homePath();
    CloseDatabasefile->setEnabled(false);

    QProcess *process = new QProcess;
    process->start("which",QStringList()<<"tlmgr");
    process->waitForBytesWritten();
    process->waitForFinished(-1);
    TexLivePath = QString(process->readAllStandardOutput()).remove("tlmgr\n");
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
        file.resize(0);
        ContentStream << BibContent;
        file.close();
        ui->SaveDocBibContent->setEnabled(false);
        QSqlQuery needsUpdate(CurrentNotesFolderDataBase);
        needsUpdate.exec(QString("UPDATE Documents SET Bibliography = \"%1\" WHERE Id = \"%2\"").arg(BibContent,DocumentFileName));
    });

    ui->ClearFiltersFD->setEnabled(false);
    ui->ClearFiltersDD->setEnabled(false);
    ui->ClearFiltersB->setEnabled(false);
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
    connect(ui->ClearFiltersB, &QPushButton::clicked, this, [=](){
        for (int i=0;i<BibliographyTable->model()->columnCount();i++) {
            BibliographyTable->filterHeader()->clearFilters();
            ui->ClearFiltersB->setEnabled(false);
        }
    });
    ui->BibEntriesTable->setColumnCount(3);
    ui->BibEntriesTable->setHorizontalHeaderLabels({tr("Citation key"),tr("Document type"),tr("Title")});
    StretchColumnsToWidth(ui->BibEntriesTable);
    connect(ui->ShowBibSourceFile,&QPushButton::clicked,this,[&](){
        ui->stackedWidget_4->setCurrentIndex(0);
        ui->BibSourceCodeLabel->setText("Source code of file : "+DatabaseFileName);
    });
    connect(ui->ShowBibSourceEntry,&QPushButton::clicked,this,[&](){
        ui->stackedWidget_4->setCurrentIndex(1);
        ui->BibSourceCodeLabel->setText("Source code of entry : "+ui->BibEntriesCombo->currentText());
    });
    connect(ui->SaveSourceCode,&QPushButton::clicked,this,[&](){
        QString sourceCode = ui->BibSourceCode->toPlainText();
        QSqlQuery write(CurrentTexFilesDataBase);
        write.exec(QString("UPDATE Database_Files SET Bibliography = \""+sourceCode+"\" WHERE Id = '"+DatabaseFileName+"'"));
    });
    ui->BibPerFileTree->setColumnCount(3);

    ui->BibPerFileTree->setHeaderLabels({tr("Citation key"),tr("Document type"),tr("Title")});

    connect(ui->BibPerFileTree,&QTreeWidget::itemChanged,this, [=](){
        QStringList entries = DocEntries.uniqueKeys();
        for(int r=0;r<ui->BibPerFileTree->topLevelItemCount();r++){
            for(int k=0;k<ui->BibPerFileTree->topLevelItem(r)->childCount();k++){
                QTreeWidgetItem * item = ui->BibPerFileTree->topLevelItem(r)->child(k);
                if(item->checkState(0) == Qt::Checked){
                    entries.append(item->text(0));
                }
                if(item->checkState(0) == Qt::Unchecked){
                    entries.removeAll(item->text(0));
                }
                entries.toSet().toList();
            }
        }
        QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId IN ('"+entries.join("','")+"')",Bibliography_Settings);
        ui->DocBibSourceCode->setPlainText(list.join(""));
    });
    connect(ui->BibPerFileTree,&QTreeWidget::itemClicked,this, [=](QTreeWidgetItem * item){
        if(/*item->parent() && item->child(0)*/TreeItemIndex(ui->BibPerFileTree->currentIndex())==1){
            ui->stackedWidget_5->setCurrentIndex(0);
            ui->BibPerFileTree->setHeaderLabels({tr("Citation key"),tr("Document type"),tr("Title")});
        }
        else if(TreeItemIndex(ui->BibPerFileTree->currentIndex())==2){
            ui->stackedWidget_5->setCurrentIndex(1);
            loadImageFile(item->text(1),BibFileView);
            ui->BibPerFileTree->setHeaderLabels({tr("File name"),tr("Path"),tr("Database source")});
        }
    });
    CreateCustomTagWidget();
    filesSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortFiles'",DataTeX_Settings).toInt();
    docsSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortDocuments'",DataTeX_Settings).toInt();
    bibSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortBibliography'",DataTeX_Settings).toInt();

    connect(ui->EnableSortingFiles,&QPushButton::toggled,this,[=](bool checked){
        filesSorting = checked;
        QStringList list;
        for(int i= 0;i<FilesTable->model()->columnCount();i++){
            list.append(FilesTable->filterHeader()->filterValue(i));
        }
        updateFilter(list);
        qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
    });
    connect(ui->EnableSortingDocs,&QPushButton::toggled,this,[&](bool checked){
        docsSorting = checked;
        QStringList list;
        for(int i= 0;i<DocumentsTable->model()->columnCount();i++){
            list.append(DocumentsTable->filterHeader()->filterValue(i));
        }
        updateFilter(list);
    });
    connect(ui->EnableSortingBib,&QPushButton::toggled,this,[&](bool checked){
        bibSorting = checked;
        QStringList list;
        for(int i= 0;i<BibliographyTable->model()->columnCount();i++){
            list.append(BibliographyTable->filterHeader()->filterValue(i));
        }
        updateFilter(list);
    });

    FilesTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    RightClick = new QDialog(this);
    QVBoxLayout * layout = new QVBoxLayout;
    FilesRightClickMenu = new QListWidget(RightClick);
    layout->addWidget(FilesRightClickMenu);
    layout->setMargin(0);
    RightClick->setLayout(layout);
    RightClick->hide();
    FilesTableHiddenColumns.clear();
    FilesRightClickMenu->clear();
    FilesRightClickMenu->addItem(tr("Select all"));
    FilesRightClickMenu->item(0)->setData(Qt::UserRole,-1);
    FilesRightClickMenu->item(0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    FilesRightClickMenu->item(0)->setCheckState(Qt::Checked);
    for (int i = 0; i < FilesTable->model()->columnCount()-3;i++) {
        FilesTableHiddenColumns.push_back(i);
        FilesRightClickMenu->addItem(FilesTable->model()->headerData(i, Qt::Horizontal,Qt::DisplayRole).toString());
        FilesRightClickMenu->item(i+1)->setData(Qt::UserRole,i);
        FilesRightClickMenu->item(i+1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        FilesRightClickMenu->item(i+1)->setCheckState(Qt::Checked);
    }
    connect(FilesTable->horizontalHeader(),&QHeaderView::customContextMenuRequested, this,[=](QPoint point){
        QPoint globalpos=FilesTable->mapToGlobal(point);
        RightClick->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        RightClick->setGeometry(QRect(globalpos, QSize(200,300)));
        RightClick->exec();
    });
    connect(FilesRightClickMenu, &QListWidget::itemClicked,this,[&](QListWidgetItem *item){
        int row = item->data(Qt::UserRole).toInt();
        if(row>-1){
            if(item->checkState()==Qt::Checked){
                FilesTableHiddenColumns.push_back(row);
            }
            else{
                FilesTableHiddenColumns.removeAll(row);
            }
            FilesTable->setColumnHidden(row,!item->checkState());
        }
        else{
            for (int i = 0; i < FilesTable->model()->columnCount()-3; i++) {
                FilesRightClickMenu->item(i+1)->setCheckState(item->checkState());
                FilesTable->setColumnHidden(i,!item->checkState());
            }
        }
    });

    DocumentsTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    DocRightClick = new QDialog(this);
    QVBoxLayout * layout2 = new QVBoxLayout;
    DocRightClickMenu = new QListWidget(DocRightClick);
    layout2->addWidget(DocRightClickMenu);
    layout2->setMargin(0);
    DocRightClick->setLayout(layout2);
    DocRightClick->hide();
    DocTableHiddenColumns.clear();
    DocRightClickMenu->clear();
    DocRightClickMenu->addItem(tr("Select all"));
    DocRightClickMenu->item(0)->setData(Qt::UserRole,-1);
    DocRightClickMenu->item(0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    DocRightClickMenu->item(0)->setCheckState(Qt::Checked);
    for (int i = 0; i < DocumentsTable->model()->columnCount();i++) {
        DocTableHiddenColumns.push_back(i);
        DocRightClickMenu->addItem(DocumentsTable->model()->headerData(i, Qt::Horizontal,Qt::DisplayRole).toString());
        DocRightClickMenu->item(i+1)->setData(Qt::UserRole,i);
        DocRightClickMenu->item(i+1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        DocRightClickMenu->item(i+1)->setCheckState(Qt::Checked);
    }
    connect(DocumentsTable->horizontalHeader(),&QHeaderView::customContextMenuRequested, this,[=](QPoint point){
        QPoint globalpos=DocumentsTable->mapToGlobal(point);
        DocRightClick->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        DocRightClick->setGeometry(QRect(globalpos, QSize(200,300)));
        DocRightClick->exec();
    });
    connect(DocRightClickMenu, &QListWidget::itemClicked,this,[&](QListWidgetItem *item){
        int row = item->data(Qt::UserRole).toInt();
        if(row>-1){
            if(item->checkState()==Qt::Checked){
                DocTableHiddenColumns.push_back(row);
            }
            else{
                DocTableHiddenColumns.removeAll(row);
            }
            DocumentsTable->setColumnHidden(row,!item->checkState());
        }
        else{
            for (int i = 0; i < DocumentsTable->model()->columnCount(); i++) {
                DocRightClickMenu->item(i+1)->setCheckState(item->checkState());
                DocumentsTable->setColumnHidden(i,!item->checkState());
            }
        }
    });

    BibliographyTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    BibRightClick = new QDialog(this);
    QVBoxLayout * layout3 = new QVBoxLayout;
    BibRightClickMenu = new QListWidget(BibRightClick);
    layout3->addWidget(BibRightClickMenu);
    layout3->setMargin(0);
    BibRightClick->setLayout(layout3);
    BibRightClick->hide();
    BibTableHiddenColumns.clear();
    BibRightClickMenu->clear();
    BibRightClickMenu->addItem(tr("Select all"));
    BibRightClickMenu->item(0)->setData(Qt::UserRole,-1);
    BibRightClickMenu->item(0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    BibRightClickMenu->item(0)->setCheckState(Qt::Checked);
    for (int i = 0; i < BibliographyTable->model()->columnCount();i++) {
        BibTableHiddenColumns.push_back(i);
        BibRightClickMenu->addItem(BibliographyTable->model()->headerData(i, Qt::Horizontal,Qt::DisplayRole).toString());
        BibRightClickMenu->item(i+1)->setData(Qt::UserRole,i);
        BibRightClickMenu->item(i+1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        BibRightClickMenu->item(i+1)->setCheckState(Qt::Checked);
    }
    connect(BibliographyTable->horizontalHeader(),&QHeaderView::customContextMenuRequested, this,[=](QPoint point){
        QPoint globalpos=BibliographyTable->mapToGlobal(point);
        BibRightClick->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        BibRightClick->setGeometry(QRect(globalpos, QSize(200,300)));
        BibRightClick->exec();
    });
    connect(BibRightClickMenu, &QListWidget::itemClicked,this,[&](QListWidgetItem *item){
        int row = item->data(Qt::UserRole).toInt();
        if(row>-1){
            if(item->checkState()==Qt::Checked){
                BibTableHiddenColumns.push_back(row);
            }
            else{
                BibTableHiddenColumns.removeAll(row);
            }
            BibliographyTable->setColumnHidden(row,!item->checkState());
        }
        else{
            for (int i = 0; i < BibliographyTable->model()->columnCount(); i++) {
                BibRightClickMenu->item(i+1)->setCheckState(item->checkState());
                BibliographyTable->setColumnHidden(i,!item->checkState());
            }
        }
    });
    connect(ui->DatabaseStructureButton,&QPushButton::clicked,this,[=](){ui->stackedWidget_6->setCurrentIndex(0);});
    connect(ui->DatabaseInfoButton,&QPushButton::clicked,this,[=](){ui->stackedWidget_6->setCurrentIndex(1);});
    connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,[&](int index){LoadCountCombo(index);});
    StretchColumnsToWidth(ui->CountFilesTable);
//    connect(ui->HideBibInfo,&QPushButton::toggled,this,[&](bool checked){ui->scrollArea_2->setHidden(checked);});
    connect(ui->ComboCount,&QComboBox::currentTextChanged,this,[=](QString text){
        LoadTableHeaders(ui->CountFilesTable,{text,tr("Number")});
    });
    connect(ui->DocDepContentPreview,&QPushButton::clicked,this,[&](){ui->stackedWidget_7->setCurrentIndex(0);});
    connect(ui->DocDepPdfPreview,&QPushButton::clicked,this,[&](){ui->stackedWidget_7->setCurrentIndex(1);});

    ui->EnableSortingFiles->setChecked(filesSorting);
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
    NewDatabasefile = CreateNewAction(FileMenu,NewDatabasefile,SLOT(CreateDatabase()),"Ctrl+N",QIcon(":/images/database-add.svg"),tr("&Create a new database"));
    OpenDatabasefile = CreateNewAction(FileMenu,OpenDatabasefile,SLOT(OpenLoadDatabase()),"Ctrl+O",QIcon(":/images/database-open.svg"),tr("&Open a database"));
    CloseDatabasefile = CreateNewAction(FileMenu,CloseDatabasefile,SLOT(RemoveCurrentDatabase()),"Ctrl+E",QIcon(":/images/database-delete.svg"),tr("&Close current database"));
    SyncDatabasefile = CreateNewAction(FileMenu,SyncDatabasefile,SLOT(DatabaseSyncFiles()),"Ctrl+S",QIcon(":/images/database-sync.svg"),tr("&Sync files to database"));
    SaveAsDatabasefile = CreateNewAction(FileMenu,SaveAsDatabasefile,[=](){/*SelectNewFileInModel(FilesTable);*/},"Ctrl+Shift+S",QIcon(":/images/database-save.svg"),tr("&Save As..."));
    QMenu * ExportMenu = new QMenu(tr("Export As..."),this);
    ExportAsSQL = CreateNewAction(ExportMenu,ExportAsSQL,SLOT(FunctionInProgress()),"Ctrl+",QIcon(":/images/database-save.svg"),tr("&Export as Sql"));
    FileMenu->addMenu(ExportMenu);

    EditMenu = menuBar()->addMenu(tr("&Edit"));

    ViewMenu = menuBar()->addMenu(tr("&View"));

    ToolMenu = menuBar()->addMenu(tr("&LaTeX Exercises"));
    NewLatexFile = CreateNewAction(ToolMenu,NewLatexFile,SLOT(NewDatabaseBaseFile()),"Ctrl+Shift+N",QIcon(":/images/tex.svg"),tr("&Create a file"));
    OpenLatexFile = CreateNewAction(ToolMenu,OpenLatexFile,SLOT(AddFileToDatabase()),"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),tr("&Open a file"));
    DeleteLatexFile = CreateNewAction(ToolMenu,DeleteLatexFile,SLOT(DeleteFileFromBase()),"Ctrl+Shift+D",QIcon(":/images/edit-delete.svg"),tr("&Delete current file"));
    EditLatexFile = CreateNewAction(ToolMenu,EditLatexFile,SLOT(EditFileMeta()),"Ctrl+Shift+E",QIcon(":/images/document-edit.svg"),tr("&Edit current file"));
    SolveLatexFile = CreateNewAction(ToolMenu,SolveLatexFile,SLOT(SolutionFile()),"Ctrl+Alt+S",QIcon(":/images/solve-tex.svgz"),tr("&Solve current exersive"));
    FileCloneToOtherDB = CreateNewAction(ToolMenu,FileCloneToOtherDB,[=](){
        FileClone(DatabaseFilePath,FileContent);
    },"Ctrl+Alt+C",QIcon(":/images/movefiletodatabase.svgz"),tr("&Clone this file to another database"));
    ToolMenu->addSeparator();
    LatexFileActions = new QMenu("Edit actions",this);
    SaveTex = CreateNewAction(LatexFileActions,SaveTex,SLOT(SaveText()),"Alt+Shift+N",QIcon(":/images/document-save.svg"),tr("&Save file content"));
    UndoTex = CreateNewAction(LatexFileActions,UndoTex,SLOT(FunctionInProgress()),"Ctrl+Z",QIcon(":/images/edit-undo.svg"),tr("&Undo changes"));
    OpenPath = CreateNewAction(LatexFileActions,OpenPath,[=](){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabaseFilePath).absolutePath()));},"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),tr("&Open folder"));
    CopyText = CreateNewAction(LatexFileActions,CopyText,[=](){QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(ui->FileEdit->toPlainText());},"Ctrl+Shift+C",QIcon(":/images/edit-copy.svg"),tr("&Copy content"));
    ShowFileTex = CreateNewAction(LatexFileActions,ShowFileTex,[=](){ui->stackedWidget_2->setCurrentIndex(0);},"Ctrl+Shift+T",QIcon(":/images/latex-config.svg"),tr("&Show source code"));
    ShowFilePdf = CreateNewAction(LatexFileActions,ShowFilePdf,[=](){ui->stackedWidget_2->setCurrentIndex(1);},"Ctrl+Shift+P",QIcon(":/images/pdf2.svg"),tr("&Show pdf"));
    ToolMenu->addMenu(LatexFileActions);

    DocToolMenu = menuBar()->addMenu(tr("&Documents"));
    NewDocument = CreateNewAction(DocToolMenu,NewDocument,SLOT(PersonalNotes()),"Ctrl+Alt+N",QIcon(":/images/pdf.svg"),tr("&Create a new document"));
    InsertFileToDocument = CreateNewAction(DocToolMenu,InsertFileToDocument,[=](){InsertFiles();},"Ctrl+Alt+I",QIcon(":/images/document-import.svg"),tr("&Insert files to document"));
    DeleteDocument = CreateNewAction(DocToolMenu,DeleteDocument,SLOT(DeleteDocumentFromBase()),"Ctrl+Alt+D",QIcon(":/images/document-close-2.svg"),tr("&Delete current document"));
    AddDocument = CreateNewAction(DocToolMenu,AddDocument,SLOT(FunctionInProgress()),"Ctrl+Alt+O",QIcon(":/images/add-button.svg"),tr("&Add document"));
    EditDocument = CreateNewAction(DocToolMenu,EditDocument,SLOT(FunctionInProgress()),"Ctrl+Alt+U",QIcon(":/images/document-edit.svg"),tr("&Edit document's content and metadata"));
    CreateSolutionsDoc = CreateNewAction(DocToolMenu,CreateSolutionsDoc,SLOT(CreateSolutionsDocument()),"Ctrl+Alt+L",QIcon(":/images/solve.svg"),tr("&Create solution document"));
    UpdateDocContent = CreateNewAction(DocToolMenu,UpdateDocContent,SLOT(UpdateDocument()),"Ctrl+Alt+U",QIcon(":/images/update-document.svg"),tr("&Update document's content"));
    CloneDocument = CreateNewAction(DocToolMenu,CloneDocument,SLOT(CloneCurrentDocument()),"Ctrl+Alt+C",QIcon(":/images/clonedocument.svg"),tr("&Clone current document to another database"));

    DocToolMenu->addSeparator();
    DocumentActions = new QMenu("Edit actions",this);
    DocToolMenu->addMenu(DocumentActions);
    SaveTexDoc = CreateNewAction(DocumentActions,SaveTexDoc,SLOT(SaveDocText()),"Ctrl+Alt+S",QIcon(":/images/document-save.svg"),tr("&Save document content"));
    UndoTexDoc = CreateNewAction(DocumentActions,UndoTexDoc,SLOT(FunctionInProgress()),"Ctrl+Alt+Z",QIcon(":/images/edit-undo.svg"),tr("&Undo changes"));
    OpenPathDoc = CreateNewAction(DocumentActions,OpenPathDoc,[=](){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DocumentFilePath).absolutePath()));},"Ctrl+Shift+O",QIcon(":/images/document-open-data.svg"),tr("&Open folder"));
    CopyTextDoc = CreateNewAction(DocumentActions,CopyTextDoc,[=](){QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(ui->DocumentContent->toPlainText());},"Ctrl+Shift+C",QIcon(":/images/edit-copy.svg"),tr("&Copy content"));
    ShowDocTex = CreateNewAction(DocumentActions,ShowDocTex,[=](){ui->stackedWidget_3->setCurrentIndex(0);},"Ctrl+Shift+T",QIcon(":/images/latex-config.svg"),tr("&Show source code"));
    ShowDocPdf = CreateNewAction(DocumentActions,ShowDocPdf,[=](){ui->stackedWidget_3->setCurrentIndex(1);},"Ctrl+Shift+P",QIcon(":/images/pdf2.svg"),tr("&Show pdf"));

    BibliographyMenu = menuBar()->addMenu(tr("&Bibliography"));
    NewBibEntry = CreateNewAction(BibliographyMenu,NewBibEntry,[=](){NewBibliographyEntry();},"Ctrl+B",QIcon(":/images/bib-new.svg"),tr("&New bibliographic entry"));
    EditBibEntry = CreateNewAction(BibliographyMenu,EditBibEntry,[=](){EditBibliographyEntry();},"Ctrl+Shift+B",QIcon(":/images/bib-edit.svg"),tr("&Edit current bibliographic entry"));
    AuthorsEditors = CreateNewAction(BibliographyMenu,AuthorsEditors,[=](){OpenAuthorsEditors();},"Ctrl+Shift+E",QIcon(":/images/im-user.svg"),tr("&Authors - Editors"));
    OpenBibEntry = CreateNewAction(BibliographyMenu,OpenBibEntry,[=](){OpenBibliographyEntry();},"Ctrl+Alt+E",QIcon(":/images/kontact-import-wizard.svg"),tr("&Open bibliographic entry"));
    DeleteBibEntry = CreateNewAction(BibliographyMenu,DeleteBibEntry,[=](){DeleteBibliographyEntry();},"Ctrl+Shift+D",QIcon(":/images/bib-delete.svg"),tr("&Delete current bibliographic entry"));

    SettingsMenu = menuBar()->addMenu(tr("&Settings"));
    Preamble = CreateNewAction(SettingsMenu,Preamble,SLOT(Preamble_clicked()),"Alt+P",QIcon(":/images/preamble.svg"),tr("&Preamble menu"));
    Datatables = CreateNewAction(SettingsMenu,Datatables,SLOT(DataBaseFields()),"Alt+D",QIcon(":/images/table.svg"),tr("&Data entries"));
    GeneralSettings = CreateNewAction(SettingsMenu,GeneralSettings,SLOT(DataTeX_Preferences()),"Alt+S",QIcon(":/images/settings.svg"),tr("&Settings"));
    BackUpDatabase = CreateNewAction(SettingsMenu,BackUpDatabase,SLOT(BackUp_DataBase_Folders()),"Alt+B",QIcon(":/images/hdd.svg"),tr("&Back up database"));
    ConnectWithTexEditor = CreateNewAction(SettingsMenu,ConnectWithTexEditor,SLOT(FunctionInProgress()),"Alt+T",QIcon(":/images/connect-with-latex.svg"),tr("&Send to external TeX editor"));

    HelpMenu = menuBar()->addMenu(tr("&Help"));
    Info = CreateNewAction(HelpMenu,Info,[=](){QMessageBox::information(this,tr("Info"),tr("This program is licensed to you under the terms of the\nGNU General Public License Version 3 as \npublished by the Free Software Foundation.\n\n"
                                                                                        "DataTex is using \"Extended Table Widget\" from DB Browser for SQLite\n\nThe software is under development..."),
                                                                    QMessageBox::Ok);},"Alt+I",QIcon(":/images/help-about.svg"),tr("Help"));
    YouTube = CreateNewAction(HelpMenu,YouTube,[=](){
            QDesktopServices::openUrl(QUrl("https://www.youtube.com/channel/UCKyVmM6aYEfIg5CFN0xn_GQ"));
        },"Alt+Y",QIcon(":/images/youtube.svg"),tr("CSMathematics channel"));
    FaceBook = CreateNewAction(HelpMenu,FaceBook,[=](){
            QDesktopServices::openUrl(QUrl("https://www.facebook.com/CSMathematics/"));
        },"Alt+F",QIcon(":/images/facebook-square.svg"),tr("Follow us on Facebook"));
    GitHub = CreateNewAction(HelpMenu,GitHub,[=](){
            QDesktopServices::openUrl(QUrl("https://github.com/CSMathematics/DataTex"));
        },"Alt+G",QIcon(":/images/github.svg"),tr("DataTeX on GitHub"));
    Email = CreateNewAction(HelpMenu,Email,[=](){
            QDesktopServices::openUrl(QUrl("mailto:?to=csmathsoftware@gmail.com&subject=Select a subject&body=Write some text", QUrl::TolerantMode));
        },"Alt+E",QIcon(":/images/envelope.svg"),tr("Contact us"));
    Site = CreateNewAction(HelpMenu,Site,[=](){
            QDesktopServices::openUrl(QUrl("https://csmathematics.github.io/DataTex/"));
        },"Alt+S",QIcon(":/images/globe-europe.svg"),tr("Visit our site"));
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
    DocTools_ToolBar->addAction(CloneDocument);

    BibliographyToolBar = addToolBar(tr("Bibliography Tools"));
    BibliographyToolBar->setObjectName("BibliographyTools");
    BibliographyToolBar->setIconSize(QSize(27,27));
    BibliographyToolBar->addAction(NewBibEntry);
    BibliographyToolBar->addAction(EditBibEntry);
    BibliographyToolBar->addAction(AuthorsEditors);
    BibliographyToolBar->addAction(OpenBibEntry);
    BibliographyToolBar->addAction(DeleteBibEntry);

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
//    Optional_Metadata_Ids.clear();
//    Optional_Metadata_Names.clear();
    labelList.clear();
    hLayoutList.clear();
    lineList.clear();

//    QSqlQuery Select_DataBase_Optional_Metadata(DataTeX_Settings);
//    Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDataBaseOptionalFields)
//                                           .arg(QFileInfo(DataTex::CurrentDataBasePath).baseName()));
//    while(Select_DataBase_Optional_Metadata.next()){
//        Optional_Metadata_Ids.append(Select_DataBase_Optional_Metadata.value(0).toString());
//        Optional_Metadata_Names.append(Select_DataBase_Optional_Metadata.value(1).toString());
//    }
    for (int i=0;i<Optional_Metadata_Ids[QFileInfo(CurrentDataBasePath).baseName()].count();i++ ) {
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(Optional_Metadata_Names[QFileInfo(CurrentDataBasePath).baseName()].at(i),this);
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
//    Optional_DocMetadata_Ids.clear();
//    Optional_DocMetadata_Names.clear();
    Doc_labelList.clear();
    Doc_hLayoutList.clear();
    Doc_lineList.clear();

//    QSqlQuery Doc_Select_DataBase_Optional_Metadata(DataTeX_Settings);
//    Doc_Select_DataBase_Optional_Metadata.exec(QString(SqlFunctions::GetCurrentDocDataBaseOptionalFields)
//                                           .arg(QFileInfo(DataTex::CurrentNotesFolderPath).baseName()));
//    while(Doc_Select_DataBase_Optional_Metadata.next()){
//        Optional_DocMetadata_Ids.insert(QFileInfo(CurrentNotesFolderPath).baseName(),Doc_Select_DataBase_Optional_Metadata.value(0).toString());
//        Optional_DocMetadata_Names.insert(QFileInfo(CurrentNotesFolderPath).baseName(),Doc_Select_DataBase_Optional_Metadata.value(1).toString());
//    }
    for (int i=0;i<Optional_DocMetadata_Ids[QFileInfo(CurrentNotesFolderPath).baseName()].count();i++ ) {
        QHBoxLayout * hbox = new QHBoxLayout(this);
        QLabel * label = new QLabel(Optional_DocMetadata_Names[QFileInfo(CurrentNotesFolderPath).baseName()].at(i),this);
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

    DocTypesIds.clear();
    DocTypesNames.clear();
    DocTypesIds.append({"@article" ,"@book" ,"@mvbook", "@inbook","@bookinbook ","@suppbook",
                               "@booklet ","@collection ","@mvcollection",
                               "@incollection ","@suppcollection ","@manual",
                               "@misc ","@online ","@patent",
                               "@periodical ","@suppperiodical ","@proceedings",
                               "@mvproceedings ","@inproceedings ","@reference",
                               "@mvreference ","@inreference ","@report",
                               "@thesis ","@unpublished"});

    DocTypesNames.append({tr("Article") ,tr("Book") ,tr("Multivolume book"), tr("Part of a book"),tr("Book in book") ,tr("Supplemental Material in a book"),
                               tr("Booklet") ,tr("Collection") ,tr("Multivolume collection"),
                               tr("Part in a collection") ,tr("Supplemental material in a collection") ,tr("Manual"),
                               tr("Miscellaneous") ,tr("Online resource") ,tr("Patent"),
                               tr("Complete issue of a periodical") ,tr("Supplemental material in a periodical") ,tr("Proceedings"),
                               tr("Multivolume proceedings") ,tr("Article in proceedings") ,tr("Reference"),
                               tr("Multivolume reference") ,tr("Part of a Reference") ,tr("Report"),
                               tr("Thesis") ,tr("Unpublished")});
    QSqlQuery CustomDocTypes(Bibliography_Settings);
    CustomDocTypes.exec("SELECT Id,Name FROM DocumentTypes WHERE Basic = '0'");
    while (CustomDocTypes.next()) {
        DocTypesIds.append(CustomDocTypes.value(0).toString());
        DocTypesNames.append(CustomDocTypes.value(1).toString());
    }
}

void DataTex::SettingsDatabase_Variables()
{
    QStringList MetadataNames;
    MetadataNames <<tr("Name")<<tr("File Type")<<tr("Field")<<tr("Chapter")<<tr("Section")<<tr("Exercise Type")
            <<tr("Difficulty")<<tr("Path")<<tr("Date")
           <<tr("Solved_Prooved")<<tr("Bibliography")<<tr("File Content")<<tr("Preamble")<<tr("Build Command")
          <<tr("File Description")<<tr("Multi-Section");
    QStringList Bibliography;
    Bibliography <<tr("Citation Key")<<tr("Document Type")<<tr("Title")<<tr("Author")<<tr("Editor")
            <<tr("Publisher")<<tr("Year")<<tr("Month")<<tr("ISBN")
           <<tr("ISSN")<<tr("Pages")<<tr("Series")<<tr("Edition")<<tr("Chapter")<<tr("Number")<<tr("Volume")
          <<tr("Journal")<<tr("Institution")<<tr("School")<<tr("Issue")<<tr("Address")<<tr("DOI")<<tr("URL")
         <<tr("Language")<<tr("Location")<<tr("Subtitle")
         <<tr("Organization")<<tr("Key")<<tr("Abstract")<<tr("Note")
           <<tr("Cross reference")<<tr("Note")<<tr("Translator");

    QStringList DocMetadata;
    DocMetadata <<tr("Name")<<tr("Document Type")<<tr("Basic folder")<<tr("Subfolder")<<tr("Subsubfolder")
            <<tr("Path")<<tr("Date")<<tr("Content")<<"Preamble"
           <<tr("LaTeX build command")<<tr("Needs update")<<tr("Bibliography")<<tr("Use bibliography file");

    datatexpath = QDir::homePath()+QDir::separator()+".datatex"+QDir::separator();
    QDir datatexdir(datatexpath);
    if (!datatexdir.exists())
        datatexdir.mkpath(".");
    QString Bibliography_Settings_Path = datatexpath+"Bibliography_Settings.db";
    QString DataTex_Settings_Path = datatexpath+"DataTex_Settings.db";
    if(!QFileInfo::exists(Bibliography_Settings_Path)){
        QStringList Bibliography;
        Bibliography <<tr("Citation Key")<<tr("Document Type")<<tr("Title")<<tr("Author")<<tr("Editor")
                <<tr("Publisher")<<tr("Year")<<tr("Month")<<tr("ISBN")
               <<tr("ISSN")<<tr("Pages")<<tr("Series")<<tr("Edition")<<tr("Chapter")<<tr("Number")<<tr("Volume")
              <<tr("Journal")<<tr("Institution")<<tr("School")<<tr("Issue")<<tr("Address")<<tr("DOI")<<tr("URL")
             <<tr("Language")<<tr("Location")<<tr("Subtitle")
             <<tr("Organization")<<tr("Key")<<tr("Abstract")
               <<tr("Cross reference")<<tr("Note")<<tr("Translator");
        QFile Settings(":/databases/Bibliography_Settings.db");
        Settings.copy(Bibliography_Settings_Path);
        QSqlQuery WriteBibNames(Bibliography_Settings);
        QFile(Bibliography_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        Bibliography_Settings.setDatabaseName(Bibliography_Settings_Path);
        Bibliography_Settings.open();
        QStringList BibliographyList;
        WriteBibNames.exec("SELECT Id FROM Bibliography_Fields ORDER BY rowid");
        while(WriteBibNames.next()){
            BibliographyList.append(WriteBibNames.value(0).toString());
        }
        for(int i=0;i<Bibliography.count();i++){
            QString query = "UPDATE Bibliography_Fields SET Name = '"
                            +Bibliography.at(i)+"' WHERE Id = '"+BibliographyList.at(i)+"'";
            WriteBibNames.exec(query);
        }
    }
    if(!QFileInfo::exists(DataTex_Settings_Path)){
        QFile Settings(":/databases/DataTex_Settings.db");
        Settings.copy(DataTex_Settings_Path);
        QFile NoPdf(":/pdfviewer/No_Pdf.pdf");
        NoPdf.copy(DataTex_Settings_Path);
        QFile(DataTex_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        DataTeX_Settings.setDatabaseName(DataTex_Settings_Path);
        DataTeX_Settings.open();
        //Εγγραφή στο φάκελο Folders τους βασικούς φακέλους που θα περιέχει μια βάση εγγράφων.
        QStringList MetadataList;
        QStringList DocMetadataList;
        QSqlQuery WriteBasicMetadata(DataTeX_Settings);
        WriteBasicMetadata.exec("SELECT Id FROM Metadata ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            MetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<MetadataList.count();i++){
            QString query = "UPDATE Metadata SET Name = '"
                            +MetadataNames.at(i)+"' WHERE Id = '"+MetadataList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }
        WriteBasicMetadata.exec("SELECT Id FROM DocMetadata ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            DocMetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<DocMetadata.count();i++){
            QString query = "UPDATE DocMetadata SET Name = '"
                            +DocMetadata.at(i)+"' WHERE Id = '"+DocMetadataList.at(i)+"'";
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
    QFile(Bibliography_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    Bibliography_Settings.setDatabaseName(Bibliography_Settings_Path);
    Bibliography_Settings.open();
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
    QSqlQuery saveSorting(DataTeX_Settings);
    saveSorting.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->EnableSortingFiles->isChecked())+"' WHERE Setting = 'SortFiles'");
    saveSorting.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->EnableSortingDocs->isChecked())+"' WHERE Setting = 'SortDocuments'");
    saveSorting.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->EnableSortingBib->isChecked())+"' WHERE Setting = 'SortBibliography'");
    delete ui;
    delete LatexFileView;
    delete DocumentView;
    delete FilesProxyModel;
    delete FilesTable;
    delete DocumentsTable;
    delete BibliographyTable;
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
    delete RightClick;
    delete FilesRightClickMenu;
    delete DocRightClick;
    delete DocRightClickMenu;
    delete BibRightClick;
    delete BibRightClickMenu;
}

QAction * DataTex::CreateNewAction(QMenu * Menu, QAction * Action, const char *Function, QString ShortCut, QIcon Icon, QString Description)
{
    Action = new QAction(Icon,Description, this);
    Action->setShortcut(QString(ShortCut));
    Menu->addAction(Action);
    connect(Action, SIGNAL(triggered()), this, Function);
    return Action;
}

QAction * DataTex::CreateNewAction(QMenu * Menu, QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description)
{
    Action = new QAction(Icon,Description, this);
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
        NewDatabaseFile * newfile = new NewDatabaseFile(this/*,{},{},{},{},false,QString()*/);
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
    CsvFunctions::WriteDataToCSV(fileName,DataTex::CurrentTexFilesDataBase);
//    qDebug()<<CurrentTexFilesDataBase<<CsvFunctions::getFile(QFileInfo(fileName).baseName());
    // Load and show new entry in table
    ShowDataBaseFiles();
    SelectNewFileInModel(FilesTable,QFileInfo(fileName).baseName());
    ui->tabWidget->setCurrentIndex(2);
}

void DataTex::SaveContentToDatabase(QString fileName, QString content)
{
    QSqlQuery WriteContent(DataTex::CurrentTexFilesDataBase);
    WriteContent.prepare("UPDATE Database_Files SET FileContent = :content WHERE Id = :file");
    WriteContent.bindValue(":file",QFileInfo(fileName).baseName());
    WriteContent.bindValue(":content",content);
    WriteContent.exec();

    QSqlQuery WriteContent_2(DataTex::CurrentNotesFolderDataBase);
    WriteContent_2.prepare("UPDATE Documents SET Content = :content WHERE Id = :file");
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
        connect(newsoldialog,&SolveDatabaseExercise::solution,this,[&](QString fileName,QString FileContent){
            // Create new Latex file and write content --------
            QFile file(fileName);
            file.open(QIODevice::ReadWrite);
            QTextStream writeContent(&file);
            writeContent.flush();
            writeContent << FileContent;
            file.close();
        });
        connect(newsoldialog,&SolveDatabaseExercise::on_close,this,[=](QString newSolution){
            ShowDataBaseFiles();
            SelectNewFileInModel(FilesTable,newSolution);
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
    ShowDocuments();
    SelectNewFileInModel(DocumentsTable,QFileInfo(fileName).baseName());
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
            SolutionsDocument * solutionbuilder = new SolutionsDocument(this,DocumentFilePath,DocumentBuildCommand,SolutionsPerExercise,DatabasePerSolution,exerciseOrder);
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
//    DataTex::FunctionInProgress();
    BackUp * back = new BackUp(this);
    back->show();
    back->activateWindow();
}

void DataTex::DatabaseSyncFiles()
{
    DatabaseSync * datasync = new DatabaseSync(this);
    datasync->show();
    datasync->activateWindow();
}

void DataTex::EditFileMeta()
{
    QHash<QString, QString> meta;
    meta.insert("FileType",FileType);
    meta.insert("Field",Field);
    meta.insert("Path",DatabaseFilePath);
    meta.insert("Difficulty",QString::number(Difficulty));
    meta.insert("Preamble",CurrentPreamble);
    meta.insert("BuildCommand",CurrentBuildCommand);
    meta.insert("FileDescription",FileDescription);
//    meta.insert("FileContent",FileContent);
    QStringList chapters = Chapter.split("|");
    QStringList sections = Section.split("|");
    QStringList subsections = ExerciseType.split("|");
    NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,FileContent,chapters,sections,subsections,true,DatabaseFileName);
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
//    if(checked){
        DocumentActions->setEnabled(!checked);
        DocTools_ToolBar->setEnabled(!checked);
        NewDocument->setEnabled(!checked);
        AddDocument->setEnabled(!checked);
        DeleteDocument->setEnabled(!checked);
        InsertFileToDocument->setEnabled(!checked);
        EditDocument->setEnabled(!checked);
        CreateSolutionsDoc->setEnabled(!checked);
        UpdateDocContent->setEnabled(!checked);
        CloneDocument->setEnabled(!checked);

        LatexFileActions->setEnabled(checked);
        LatexTools_ToolBar->setEnabled(checked);
        NewLatexFile->setEnabled(checked);
        OpenLatexFile->setEnabled(checked);
        DeleteLatexFile->setEnabled(checked);
        EditLatexFile->setEnabled(checked);
        SolveLatexFile->setEnabled(checked);
        FileCloneToOtherDB->setEnabled(checked);

        NewBibEntry->setEnabled(!checked);
        EditBibEntry->setEnabled(!checked);
        AuthorsEditors->setEnabled(!checked);
        OpenBibEntry->setEnabled(!checked);
        DeleteBibEntry->setEnabled(!checked);

        ui->stackedWidget->setCurrentIndex(0);
        DatabaseStructure(CurrentDataBasePath);
//    }
}

void DataTex::on_DocumentsDatabaseToggle_clicked(bool checked)
{
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


        NewBibEntry->setEnabled(!checked);
        EditBibEntry->setEnabled(!checked);
        AuthorsEditors->setEnabled(!checked);
        OpenBibEntry->setEnabled(!checked);
        DeleteBibEntry->setEnabled(!checked);
        ui->stackedWidget->setCurrentIndex(1);
//        ui->FilesDatabaseToggle->setChecked(!checked);

        DatabaseStructure(CurrentNotesFolderPath);
//    }
        ui->EnableSortingDocs->setChecked(docsSorting);
}

void DataTex::ShowDataBaseFiles()
{
    FilesModel = new QSqlQueryModel(this);
    FilesProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
    QSqlQuery DatabaseQuery(CurrentTexFilesDataBase);
    DatabaseQuery.exec(SqlFunctions::ShowAllDatabaseFiles);
    FilesModel->setQuery(DatabaseQuery);
    FilesProxyModel->setSourceModel(FilesModel);//Φιλτράρισμα και ταξινόμηση ---
    if(filesSorting){
        FilesTable->setModel(FilesProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }
    else{
        FilesTable->setModel(FilesModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }

//    FilesTable->setModel(Database);//No filtering
    FilesTable->show();
    FilesTable->setSortingEnabled(filesSorting);//Φιλτράρισμα και ταξινόμηση ---
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
        for(int i= 0;i<values.count();i++){
            if(!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!=""){
                SqlFunctions::FilesTable_UpdateQuery += " WHERE ";
                break;
            }
        }

        QStringList DataFields = QStringList();

        if(!values.at(0).isEmpty() && !values.at(0).isNull()){
            DataFields.append(" df.Id LIKE \"%"+values.at(0)+"%\" ");
        }
        if(!values.at(1).isEmpty() && !values.at(1).isNull()){
            DataFields.append(" ft.FileType LIKE \"%"+values.at(1)+"%\" ");
        }
        if(!values.at(2).isEmpty() && !values.at(2).isNull()){
            DataFields.append(" f.Name LIKE \"%"+values.at(2)+"%\" ");
        }
        if(!values.at(3).isEmpty() && !values.at(3).isNull()){
            DataFields.append(" c.Name LIKE \"%"+values.at(3)+"%\" ");
        }
        if(!values.at(4).isEmpty() && !values.at(4).isNull()){
            DataFields.append(" s.Name LIKE \"%"+values.at(4)+"%\" ");
        }
        if(!values.at(5).isEmpty() && !values.at(5).isNull()){
            DataFields.append(" et.Name LIKE \"%"+values.at(5)+"%\" ");
        }

        for (int i=6;i<Database_FileTableFields.count();i++) {
            if(!values.at(i).isEmpty() && !values.at(i).isNull())
                DataFields.append(" (\"df\".\""+Database_FileTableFields.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
        }

        int columns = Database_FileTableFields.count();
        if(!values.at(columns+2).isNull() && !values.at(columns+2).isEmpty()){
            QStringList tags = values.at(columns+2).split(",");
            DataFields.append(" t.Tag_Id IN (\""+tags.join("\",\"")+"\") ");
        }
        SqlFunctions::FilesTable_UpdateQuery += DataFields.join(" AND ");
        SqlFunctions::FilesTable_UpdateQuery += " GROUP BY df.Id ORDER BY df.rowid;";
        qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
        FilesTable->setColumnHidden(columns,true);
        FilesTable->setColumnHidden(columns+1,true);
        FilesTable->setColumnHidden(columns+2,true);
        updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,CurrentTexFilesDataBase,this);
        //Filter
        FilesProxyModel->setSourceModel(FilesTable->model());//Φιλτράρισμα και ταξινόμηση ---
        QSqlQuery DatabaseQuery(CurrentTexFilesDataBase);
        DatabaseQuery.exec(SqlFunctions::FilesTable_UpdateQuery);
        FilesModel->setQuery(DatabaseQuery);
        FilesProxyModel->setSourceModel(FilesModel);//Φιλτράρισμα και ταξινόμηση ---
        if(filesSorting){
            FilesTable->setModel(FilesProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }
        else{
            FilesTable->setModel(FilesModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }

        FilesTable->setSortingEnabled(filesSorting);//Φιλτράρισμα και ταξινόμηση ---
        //------
        connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DataTex::FilesTable_selectionchanged);
        LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
        //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel->setFilterRegExp(QRegExp(".png", Qt::CaseInsensitive,QRegExp::FixedString));
        //Φιλτράρισμα και ταξινόμηση --- FilesProxyModel->setFilterKeyColumn(1);
    }
    else if(ui->stackedWidget->currentIndex()==1){
        SqlFunctions::FilterDatabaseDocuments.clear();
        FilterDocuments(Database_DocumentTableColumns);
        for(int i= 0;i<values.count();i++){
            if(!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!=""){
                SqlFunctions::FilterDatabaseDocuments += " WHERE ";
                break;
            }
        }
        QStringList DataFields = QStringList();
        for (int i=0;i<Database_DocumentTableColumns.count();i++) {
            if(!values.at(i).isEmpty() && !values.at(i).isNull())
                DataFields.append(" (\"d\".\""+Database_DocumentTableColumns.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
        }
        int columns = Database_DocumentTableColumns.count();
        if(!values.at(columns).isNull() && !values.at(columns).isEmpty()){
            QStringList tags = values.at(columns).split(",");
            DataFields.append(" t.Tag_Id IN (\""+tags.join("\",\"")+"\") ");
        }
        SqlFunctions::FilterDatabaseDocuments += DataFields.join(" AND ");
        SqlFunctions::FilterDatabaseDocuments += " GROUP BY d.Id ORDER BY d.rowid;";
        updateTableView(DocumentsTable,SqlFunctions::FilterDatabaseDocuments,CurrentNotesFolderDataBase,this);
        //Filter
        DocumentsProxyModel->setSourceModel(DocumentsTable->model());//Φιλτράρισμα και ταξινόμηση ---
        QSqlQuery DatabaseQuery(CurrentNotesFolderDataBase);
        DatabaseQuery.exec(SqlFunctions::FilterDatabaseDocuments);
        DocsModel->setQuery(DatabaseQuery);
        if(docsSorting){
            DocumentsTable->setModel(DocumentsProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }
        else{
            DocumentsTable->setModel(DocsModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }

        DocumentsTable->setSortingEnabled(docsSorting);//Φιλτράρισμα και ταξινόμηση ---
        //------
        connect(DocumentsTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DataTex::DocumentsTable_selectionChanged);
        LoadTableHeaders(DocumentsTable,Database_DocTableFieldNames);
    }
    else if(ui->stackedWidget->currentIndex()==2){
        SqlFunctions::FilterBibliographyEntries.clear();
        FilterBibliographyTable(BibliographyTableColumns);
//        for (int i=0;i<BibliographyTableColumns.count();i++) {
//            SqlFunctions::FilterBibliographyEntries.replace("replace"+BibliographyTableColumns.at(i),values.at(i));
//        }
        for(int i= 0;i<values.count();i++){
            if(!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!=""){
                SqlFunctions::FilterBibliographyEntries += " HAVING ";
                break;
            }
        }
        QStringList DataFields = QStringList();
        for (int i=0;i<BibliographyTableColumns.count();i++) {
            if(!values.at(i).isEmpty() && !values.at(i).isNull())
                DataFields.append(" (\"b\".\""+BibliographyTableColumns.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
        }
        SqlFunctions::FilterBibliographyEntries += DataFields.join(" AND ");
        SqlFunctions::FilterBibliographyEntries += " ORDER BY b.ROWID ";
        qDebug()<<SqlFunctions::FilterBibliographyEntries;
        updateTableView(BibliographyTable,SqlFunctions::FilterBibliographyEntries,Bibliography_Settings,this);
        //Filter
        BibliographyProxyModel->setSourceModel(BibliographyTable->model());//Φιλτράρισμα και ταξινόμηση ---
        QSqlQuery DatabaseQuery(Bibliography_Settings);
        DatabaseQuery.exec(SqlFunctions::FilterBibliographyEntries);
        BibModel->setQuery(DatabaseQuery);
        if(bibSorting){
            BibliographyTable->setModel(BibliographyProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }
        else{
            BibliographyTable->setModel(BibModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
        }
        BibliographyTable->setSortingEnabled(bibSorting);//Φιλτράρισμα και ταξινόμηση ---
        //------
        connect(BibliographyTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DataTex::BibliographyTable_selectionChanged);

        QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT name FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
//        list.move(3,list.count()-1);
//        list.move(3,list.count()-1);
//        list.move(3,list.count()-1);
        LoadTableHeaders(BibliographyTable,list);
    }
}

void DataTex::ShowDocuments()
{
    DocsModel = new QSqlQueryModel(this);
    QSqlQuery DatabaseQuery(CurrentNotesFolderDataBase);
    DatabaseQuery.exec(SqlFunctions::ShowDocuments);
    DocsModel->setQuery(DatabaseQuery);
//    DocumentsTable->setModel(Database);
    DocumentsProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
    DocumentsProxyModel->setSourceModel(DocsModel);//Φιλτράρισμα και ταξινόμηση ---
    if(docsSorting){
        DocumentsTable->setModel(DocumentsProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }
    else{
        DocumentsTable->setModel(DocsModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }
    DocumentsTable->show();
    DocumentsTable->setSortingEnabled(docsSorting);//Φιλτράρισμα και ταξινόμηση ---
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
void DataTex::ShowBibliography()
{
    BibModel = new QSqlQueryModel(this);
    QSqlQuery DatabaseQuery(Bibliography_Settings);
    DatabaseQuery.exec(SqlFunctions::ShowBibliographyEntries);
    BibModel->setQuery(DatabaseQuery);
//    BibliographyTable->setModel(Database);
    BibliographyProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
    BibliographyProxyModel->setSourceModel(BibModel);//Φιλτράρισμα και ταξινόμηση ---
    if(bibSorting){
        BibliographyTable->setModel(BibliographyProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }
    else{
        BibliographyTable->setModel(BibModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
    }
    BibliographyTable->show();
    BibliographyTable->setSortingEnabled(bibSorting);//Φιλτράρισμα και ταξινόμηση ---
    int columns = BibliographyTable->model()->columnCount();
    BibliographyTable->generateFilters(columns,false);
    connect(BibliographyTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::BibliographyTable_selectionChanged);
    connect(BibliographyTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
    connect(BibliographyTable->filterHeader(), &FilterTableHeader::filterValues, this, [=](){ui->ClearFiltersB->setEnabled(true);});
    QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT name FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
//    list.move(3,list.count()-1);
//    list.move(3,list.count()-1);
//    list.move(3,list.count()-1);
    LoadTableHeaders(BibliographyTable,list);
    for (int i=0;i<BibliographyTable->model()->columnCount();i++ ) {
        BibliographyTable->filterHeader()->placeHolderText(i,BibliographyTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
    }
    StretchColumns(BibliographyTable,1.5);
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
    Difficulty = FilesTable->model()->data(FilesTable->model()->index(row,6)).toInt();
    DatabaseFilePath = FilesTable->model()->data(FilesTable->model()->index(row,7)).toString();
    Date = FilesTable->model()->data(FilesTable->model()->index(row,8)).toString();
    Solved = FilesTable->model()->data(FilesTable->model()->index(row,9)).toString();
    CurrentBuildCommand = FilesTable->model()->data(FilesTable->model()->index(row,13)).toString();
    FileDescription = FilesTable->model()->data(FilesTable->model()->index(row,14)).toString();
//    MultiSection = FilesTable->model()->data(FilesTable->model()->index(row,15)).toInt();
    FileTypeId = FilesTable->model()->data(FilesTable->model()->index(row,Database_FileTableFieldNames.count())).toString();
    Solvable = FilesTable->model()->data(FilesTable->model()->index(row,Database_FileTableFieldNames.count()+1)).toInt();
    ui->DateTimeEdit->setDateTime(QDateTime::fromString(Date,"dd/M/yyyy hh:mm"));
    ui->DifficultySpinBox->setValue(Difficulty);
    OptionalFields = Optional_Metadata_Ids[QFileInfo(CurrentDataBasePath).baseName()].join(",");
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

    QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentTexFilesDataBase);
    ui->BibEntriesCombo->clear();
    QSqlQuery listOfBibEntries(Bibliography_Settings);
    listOfBibEntries.exec("SELECT * FROM EntrySourceCode");
    while(listOfBibEntries.next()){
        ui->BibEntriesCombo->addItem(listOfBibEntries.value(0).toString(),QVariant((listOfBibEntries.value(1).toString())));
    }
    ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(ui->BibEntriesCombo->currentText()));
    QString sourceCode = QString();
    foreach(QString id,BibEntriesInFile){
        sourceCode += SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId = '"+id+"'",Bibliography_Settings)[0];
    }
    connect(ui->BibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(text));
        ui->stackedWidget_4->setCurrentIndex(1);
        ui->BibSourceCode_per_Entry->setPlainText(ui->BibEntriesCombo->currentData().toString());
        ui->BibSourceCodeLabel->setText("Source code of entry : "+ui->BibEntriesCombo->currentText());
    });
    int i=-1;
    ui->BibEntriesTable->setRowCount(0);
    ui->stackedWidget_4->setCurrentIndex(0);
    ui->BibSourceCode->setPlainText(sourceCode);
    ui->BibSourceCodeLabel->setText("Source code of file : "+DatabaseFileName);
    foreach(QString citationKey,BibEntriesInFile){
        QSqlQuery values(Bibliography_Settings);
        values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
                            "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
                            "WHERE Citation_Key = '%1'").arg(citationKey));
        i++;
        ui->BibEntriesTable->insertRow(i);
        ui->BibEntriesTable->setItem(i,0 , new QTableWidgetItem(citationKey));
        while(values.next()){
            ui->BibEntriesTable->setItem(i,1 , new QTableWidgetItem(values.value(0).toString()));
            ui->BibEntriesTable->setItem(i,2 , new QTableWidgetItem(values.value(1).toString()));
        }
    }

    FilesPreambleCombo->setEnabled(true);
    CurrentPreamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    int index = FilesPreambleCombo->findData(CurrentPreamble);
    FilesPreambleCombo->setCurrentIndex(index);
    QStringList list = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                      .arg(DataTex::CurrentPreamble)
                                                                      ,DataTeX_Settings);
    if(list.count()>0){CurrentPreamble_Content = list.at(0);}
    getActionFromText(CompileMenu,CompileCommands);

    QSqlQuery FilesQuery(CurrentNotesFolderDataBase);
    QSqlQueryModel * Files = new QSqlQueryModel(this);
//    for (int i=0;i<GlobalDocsDatabaseList.values().count();i++) {
//        if(GlobalDocsDatabaseList.values().at(i)!=CurrentNotesFolderPath) {
//            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(GlobalDocsDatabaseList.values().at(i),QFileInfo(GlobalDocsDatabaseList.values().at(i)).baseName()));
//            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(files,GlobalFilesDatabaseListNames[QFileInfo(DatabasesInADocument.at(i)).baseName()]
//                                                                                     ,QFileInfo(DatabasesInADocument.at(i)).baseName()));
//        }
//    }
//    query = datalist.join(" UNION ");
    FilesQuery.exec(QString("SELECT Document_Id,Files_Database_Source,d.Path,d.Content FROM Files_per_Document fpd "
                            "JOIN Documents d ON d.Id = fpd.Document_Id "
                            "WHERE File_Id = '%1'").arg(DatabaseFileName));
    //    qDebug()<<query+" ORDER BY df.Id ";
    Files->setQuery(FilesQuery);
    ui->FileDependenciesTable->setModel(Files);
    ui->FileDependenciesTable->show();
    ui->FileDependenciesTable->setColumnHidden(2,true);
    ui->FileDependenciesTable->setColumnHidden(3,true);
    StretchColumnsToWidth(ui->FileDependenciesTable);
    connect(ui->FileDependenciesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this,[&](){
        int row = ui->FileDependenciesTable->currentIndex().row();
        QString DocPath = ui->FileDependenciesTable->model()->data(ui->FileDependenciesTable->model()->index(row,2)).toString();
        QString DocContent = ui->FileDependenciesTable->model()->data(ui->FileDependenciesTable->model()->index(row,3)).toString();
        ui->DocDependenciesContent->setText(DocContent);
        loadImageFile(DocPath,DocDependenciesFileView);
        int lineNumber = -1;
        int start = -1;
        int end = -1;
        QTextStream parse(&DocContent);
        while(!parse.atEnd()){
            lineNumber++;
            QString line = parse.readLine();
            if(line.contains("%# Database File : "+DatabaseFileName)){
                start = lineNumber;
            }
            if(line.contains("%# End of file "+DatabaseFileName)){
                end = lineNumber;
            }
        }

        QTextBlock block = ui->DocDependenciesContent->document()->findBlockByLineNumber(start);
        QTextCursor newCursor(block);
        ui->DocDependenciesContent->moveCursor(QTextCursor::End);
        ui->DocDependenciesContent->setTextCursor(newCursor);
        int line = ui->DocDependenciesContent->textCursor().blockNumber();
        if(line>=start && line <=end){
            for (int i = start;i<end+1;i++) {
                QTextBlock block = ui->DocDependenciesContent->document()->findBlockByNumber(i);
                QTextCursor cursor(block);
                ui->DocDependenciesContent->moveCursor(QTextCursor::End);
                ui->DocDependenciesContent->setTextCursor(cursor);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor(240,245,245));
                cursor.setBlockFormat(blockFormat);
            }
            ui->DocDependenciesContent->setTextCursor(newCursor);
        }
        else{
            LatexTextEdit::clearFormat(ui->DocDependenciesContent);
        }
    });
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

//    QStringList filesFromDatabase =
//            SqlFunctions::Get_StringList_From_Query(QString("SELECT File_Id FROM Files_per_Document WHERE Document_Id = '%1'")
//            .arg(QFileInfo(DatabaseFilePath).baseName()),CurrentNotesFolderDataBase);
    QStringList ListOfDatabases =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT DISTINCT Files_Database_Source FROM Files_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentNotesFolderDataBase);
    DatabasesInADocument.clear();
    DatabasesInADocument =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Path FROM Databases WHERE FileName IN (\"%1\")").arg(ListOfDatabases.join("\",\""))
            ,DataTeX_Settings);
    DatabasesInADocument.toSet().toList();

    QFile TexFile(DocumentFilePath);
    QStringList fileNamesInDocument;
    QString path_rem = "%# Database File : ";
    exerciseOrder.clear();
    int exercise_index = -1;
    TexFile.open (QIODevice::ReadOnly | QIODevice::Text);
    QTextStream Line(&TexFile);
    while (!Line.atEnd()){
        QString LineText=Line.readLine();
        if(LineText.contains(path_rem)){
            exercise_index++;
            fileNamesInDocument.append(LineText.remove(path_rem));
            exerciseOrder.insert(exercise_index,LineText);
        }
    }
//    ReorderFiles(ui->TexFileTable,exerciseOrder);
    qDebug()<<exerciseOrder;
    TexFile.close();
    QString files = "(\""+fileNamesInDocument.join("\",\"")+"\")";
    QSqlQueryModel * Files = new QSqlQueryModel(this);
    QStringList datalist;
    QString query;
    QSqlQuery FilesQuery(CurrentTexFilesDataBase);
    for (int i=0;i<DatabasesInADocument.count();i++) {
        qDebug()<<DatabasesInADocument.at(i)<<" = "<<CurrentDataBasePath;
        QString name = (DatabasesInADocument.at(i)!=CurrentDataBasePath) ? QFileInfo(DatabasesInADocument.at(i)).baseName() : "main" ;
        datalist.append(SqlFunctions::ShowFilesInADocument.arg(files,GlobalFilesDatabaseListNames[QFileInfo(DatabasesInADocument.at(i)).baseName()]
                        ,name));
        if(DatabasesInADocument.at(i)!=CurrentDataBasePath) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DatabasesInADocument.at(i),QFileInfo(DatabasesInADocument.at(i)).baseName()));
            qDebug()<<QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DatabasesInADocument.at(i),QFileInfo(DatabasesInADocument.at(i)).baseName());
        }
    }
    query = datalist.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY df.Id ");
    qDebug()<<query+" ORDER BY df.Id ";
    qDebug()<<QSqlDatabase::connectionNames();
    Files->setQuery(FilesQuery);
    ui->TexFileTable->setModel(Files);
    ui->TexFileTable->show();
    LoadTableHeaders(ui->TexFileTable,{tr("Id"),tr("Database source"),tr("File type"),tr("Section")
                                        ,tr("Exercise type"),tr("Path"),tr("Solved - Prooved")});
    ui->TexFileTable->setColumnHidden(7,true);
    ui->TexFileTable->setColumnHidden(8,true);
    StretchColumns(ui->TexFileTable,1.5);
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
    QString CurrentFilePreamble = SqlFunctions::Get_StringList_From_Query(QString("SELECT Preamble FROM Documents WHERE Id = \"%1\";")
                              .arg(DocumentFileName),CurrentNotesFolderDataBase).at(0);
    int index = DocumentsPreambleCombo->findData(CurrentFilePreamble);
    DocumentsPreambleCombo->setCurrentIndex(index);
    CurrentPreamble = DocumentsPreambleCombo->currentData().toString();
    setPreamble();

    UpdateDocContent->setEnabled(DocumentNeedsUpdate == 1);
    CloneDocument->setEnabled(true);
    FilePathsInADocument.clear();
    for (int i = 0;i<ui->TexFileTable->model()->rowCount();i++) {
        FilePathsInADocument.insert(ui->TexFileTable->model()->data(DocumentsTable->model()->index(i,5)).toString(),
                                    ui->TexFileTable->model()->data(DocumentsTable->model()->index(i,1)).toString());
    }

    DocOptionalFields = Optional_DocMetadata_Ids[QFileInfo(CurrentNotesFolderPath).baseName()].join(",");
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

    SolutionsPerExercise.clear();
    DatabasePerSolution.clear();
    for (int i=0;i<fileNamesInDocument.count();i++){
        QString db = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,9)).toString();

        QString File = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,0)).toString();
        QString DatabaseName = (db != "main") ? db : QFileInfo(CurrentDataBasePath).baseName() ;
        QString FileType = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,7)).toString();
        QSqlDatabase Database = GlobalFilesDatabaseList[DatabaseName];
        QStringList list = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solvable FROM FileTypes WHERE Id = '%1'").arg(FileType),Database);
        int solvable = (list.count()>0) ? list[0].toInt() : 0 ;
        QStringList Solutions_per_exer =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Path FROM Solutions_per_File WHERE File_Id = '%1'").arg(File),Database);
        if(solvable){
            SolutionsPerExercise.insert(File,Solutions_per_exer);
            DatabasePerSolution.insert(File,Database);
        }
    }

//    for (int i=0;i<Exer_List.count();i++) {
//        QFile pathfile;
//        if(!pathfile.exists(Solutions_List.at(i))){
//            Unsolved_List.append(Exer_List.at(i));}
//        if(pathfile.exists(Solutions_List.at(i))){
//            Solved_List.append(Solutions_List.at(i));}
//    }
    if(SolutionsPerExercise.keys().size()>0){CreateSolutionsDoc->setEnabled(true);}
    else if(SolutionsPerExercise.keys().size()==0){CreateSolutionsDoc->setEnabled(false);}
//    QString solutionsname = QFileInfo(DocumentFilePath).baseName()+"_Solutions";
//    QString SolutionsPdfFile = QFileInfo(DocumentFilePath).absolutePath()+QDir::separator()+solutionsname+".pdf";
//    QString PdfFile = QFileInfo(DocumentFilePath).absolutePath()+QDir::separator()+QFileInfo(DocumentFilePath).baseName()+".pdf";

    DocEntries.clear();
    ui->DocBibEntriesCombo->clear();
    foreach(auto i, ui->BibPerFileTree->topLevelItem(0)->takeChildren()){delete i;}
    foreach(auto i, ui->BibPerFileTree->topLevelItem(1)->takeChildren()){delete i;}
    for(int i=0;i<ui->TexFileTable->model()->rowCount();i++){
        QString entries = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,8)).toString();
        QString file = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,0)).toString();
        QString filePath = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,5)).toString();
        QString database = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,9)).toString();
        QStringList list = entries.split("|");
        if(!entries.isEmpty() && !entries.isNull()){
            foreach(QString entry,list){
                DocEntries.insert(entry,{file,filePath,database});
            }
        }
    }

    for(int i=0;i<DocEntries.uniqueKeys().count();i++){
        QTreeWidgetItem * item = new QTreeWidgetItem();
        ui->BibPerFileTree->topLevelItem(0)->addChild(item);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(0,Qt::Checked);

        for(int k=0;k<DocEntries.values(DocEntries.uniqueKeys()[i]).count();k++){
            QTreeWidgetItem * subitem = new QTreeWidgetItem();
            subitem->setText(0,DocEntries.values(DocEntries.uniqueKeys()[i])[k][0]);
            subitem->setText(1,DocEntries.values(DocEntries.uniqueKeys()[i])[k][1]);
            subitem->setText(2,DocEntries.values(DocEntries.uniqueKeys()[i])[k][2]);
            item->addChild(subitem);
        }
        item->setText(0,DocEntries.uniqueKeys()[i]+"  ("+QString::number(ui->BibPerFileTree->topLevelItem(0)->child(i)->childCount())
                             +tr(" files use this entry)"));
        QSqlQuery bibInfo(Bibliography_Settings);
        bibInfo.exec("SELECT d.Name,b.title "
                     "FROM Bibliography b "
                     "JOIN DocumentTypes d ON d.Id = b.Document_Type "
                     "WHERE Citation_Key = '"+DocEntries.uniqueKeys()[i]+"'");

        while(bibInfo.next()){
            item->setText(1,bibInfo.value(0).toString());
            item->setText(2,bibInfo.value(1).toString());
        }
    }

    QSqlQuery IndividualEntries(CurrentNotesFolderDataBase);
    IndividualEntries.exec(QString("SELECT Bib_Id FROM BibEntries_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName));
    while(IndividualEntries.next()){
        int i=-1;
        if(!DocEntries.uniqueKeys().contains(IndividualEntries.value(0).toString())){
            i++;
            QString entry = IndividualEntries.value(0).toString();
            DocEntries.insert(entry,{});
            QTreeWidgetItem * item = new QTreeWidgetItem();
            item->setText(0,entry);
            QSqlQuery values(Bibliography_Settings);
            values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
                                "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
                                "WHERE Citation_Key = '%1'").arg(entry));
            while(values.next()){
                item->setText(1,values.value(0).toString());
                item->setText(2,values.value(1).toString());
            }
            ui->BibPerFileTree->topLevelItem(1)->addChild(item);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            item->setCheckState(0,Qt::Checked);
//            foreach(QSqlDatabase database,GlobalFilesDatabaseList.values()){
//                QSqlQuery FilesPerEntry(database);
//                FilesPerEntry.exec(QString("SELECT File_Id,Path "
//                                           "FROM Bib_Entries_per_File bpf "
//                                           "JOIN Database_Files df ON df.Id = bpf.File_Id "
//                                           "WHERE Bib_Id = '%1'").arg(entry));
//                while(FilesPerEntry.next()){
//                    QTreeWidgetItem * item = new QTreeWidgetItem();
//                    item->setText(0,FilesPerEntry.value(0).toString());
//                    item->setText(1,FilesPerEntry.value(1).toString());
//                    item->setText(2,QFileInfo(database.databaseName()).baseName());
//                    ui->BibPerFileTree->topLevelItem(1)->child(i)->addChild(item);
//                }
//            }
        }
    }
    QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId IN ('"+DocEntries.uniqueKeys().join("','")+"')",Bibliography_Settings);
    ui->DocBibSourceCode->setPlainText(list.join(""));

    ui->SaveDocBibContent->setEnabled(false);

    QSqlQuery listOfBibEntries(Bibliography_Settings);
    listOfBibEntries.exec("SELECT * FROM EntrySourceCode");
    while(listOfBibEntries.next()){
        ui->DocBibEntriesCombo->addItem(listOfBibEntries.value(0).toString(),QVariant((listOfBibEntries.value(1).toString())));
    }
    ui->addDocBibEntry->setEnabled(!DocEntries.contains(ui->DocBibEntriesCombo->currentText()));
    connect(ui->DocBibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addDocBibEntry->setEnabled(!DocEntries.contains(text));
        ui->stackedWidget_5->setCurrentIndex(0);
    });
    ui->BibPerFileTree->expandAll();
}

void DataTex::BibliographyTable_selectionChanged()
{
    QItemSelectionModel *select = BibliographyTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    int lastcolumn = BibliographyTable->model()->columnCount()-1;
    CitationKey = BibliographyTable->model()->data(BibliographyTable->model()->index(row,0)).toString();
    Bib_ValueList["Citation_Key"]->setText(CitationKey);
    BibDocType = BibliographyTable->model()->data(BibliographyTable->model()->index(row,1)).toString();
    Bib_ValueList["Document_Type"]->setText(BibDocType);
    BibTitle = BibliographyTable->model()->data(BibliographyTable->model()->index(row,2)).toString();
    Bib_ValueList["title"]->setText(BibTitle);
    Authors = BibliographyTable->model()->data(BibliographyTable->model()->index(row,3)).toString();
    Bib_ValueList["author"]->setText(Authors);
    Editors = BibliographyTable->model()->data(BibliographyTable->model()->index(row,4)).toString();
    Bib_ValueList["editor"]->setText(Editors);
    Translators = BibliographyTable->model()->data(BibliographyTable->model()->index(row,5)).toString();
    Bib_ValueList["translator"]->setText(Translators);
    Publisher = BibliographyTable->model()->data(BibliographyTable->model()->index(row,6)).toString();
    Bib_ValueList["publisher"]->setText(Publisher);
    BibYear = BibliographyTable->model()->data(BibliographyTable->model()->index(row,7)).toInt();
    Bib_ValueList["year"]->setText(QString::number(BibYear));
    BibMonth = BibliographyTable->model()->data(BibliographyTable->model()->index(row,8)).toString();
    Bib_ValueList["month"]->setText(BibMonth);
    ISBN = BibliographyTable->model()->data(BibliographyTable->model()->index(row,9)).toString();
    Bib_ValueList["isbn"]->setText(ISBN);
    ISSN = BibliographyTable->model()->data(BibliographyTable->model()->index(row,10)).toString();
    Bib_ValueList["issn"]->setText(ISSN);
    Pages = BibliographyTable->model()->data(BibliographyTable->model()->index(row,11)).toInt();
    Bib_ValueList["pages"]->setText(QString::number(Pages));
    Series = BibliographyTable->model()->data(BibliographyTable->model()->index(row,12)).toString();
    Bib_ValueList["series"]->setText(Series);
    Edition = BibliographyTable->model()->data(BibliographyTable->model()->index(row,13)).toString();
    Bib_ValueList["edition"]->setText(Edition);
    BibChapter = BibliographyTable->model()->data(BibliographyTable->model()->index(row,14)).toString();
    Bib_ValueList["chapter"]->setText(BibChapter);
    BibNumber = BibliographyTable->model()->data(BibliographyTable->model()->index(row,15)).toString();
    Bib_ValueList["number"]->setText(BibNumber);
    Volume = BibliographyTable->model()->data(BibliographyTable->model()->index(row,16)).toString();
    Bib_ValueList["volume"]->setText(Volume);
    Journal = BibliographyTable->model()->data(BibliographyTable->model()->index(row,17)).toString();
    Bib_ValueList["journal"]->setText(Journal);
    Institution = BibliographyTable->model()->data(BibliographyTable->model()->index(row,18)).toString();
    Bib_ValueList["institution"]->setText(Institution);
    School = BibliographyTable->model()->data(BibliographyTable->model()->index(row,19)).toString();
    Bib_ValueList["school"]->setText(School);
    Issue = BibliographyTable->model()->data(BibliographyTable->model()->index(row,20)).toString();
    Bib_ValueList["issue"]->setText(Issue);
    Address = BibliographyTable->model()->data(BibliographyTable->model()->index(row,21)).toString();
    Bib_ValueList["address"]->setText(Address);
    DOI = BibliographyTable->model()->data(BibliographyTable->model()->index(row,22)).toString();
    Bib_ValueList["doi"]->setText(DOI);
    URL = BibliographyTable->model()->data(BibliographyTable->model()->index(row,23)).toString();
    Bib_ValueList["url"]->setText(URL);
    Language = BibliographyTable->model()->data(BibliographyTable->model()->index(row,24)).toString();
    Bib_ValueList["language"]->setText(Language);
    Location = BibliographyTable->model()->data(BibliographyTable->model()->index(row,25)).toString();
    Bib_ValueList["location"]->setText(Location);
    Subtitle = BibliographyTable->model()->data(BibliographyTable->model()->index(row,26)).toString();
    Bib_ValueList["subtitle"]->setText(Subtitle);
    Organization = BibliographyTable->model()->data(BibliographyTable->model()->index(row,27)).toString();
    Bib_ValueList["organization"]->setText(Organization);
    BibKey = BibliographyTable->model()->data(BibliographyTable->model()->index(row,28)).toString();
    Bib_ValueList["key"]->setText(BibKey);
    Abstract = BibliographyTable->model()->data(BibliographyTable->model()->index(row,29)).toString();
    Bib_ValueList["abstract"]->setText(Abstract);
    CrossReference = BibliographyTable->model()->data(BibliographyTable->model()->index(row,30)).toString();
    Bib_ValueList["crossref"]->setText(CrossReference);
    BibNote = BibliographyTable->model()->data(BibliographyTable->model()->index(row,31)).toString();
    Bib_ValueList["note"]->setText(BibNote);

    int k=-1;
    foreach(QString text,Bib_ValueList_Opt.keys()){
        k++;
        Bib_ValueList_Opt[text]->setText(BibliographyTable->model()->
                data(BibliographyTable->model()->index(row,Bib_ValueList.count()-1)).toString());
    }
    QString sourceCode = SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId = '"+CitationKey+"'",Bibliography_Settings)[0];
    ui->SourceCodeText->setText(sourceCode);
    EditBibEntry->setEnabled(true);
    DeleteBibEntry->setEnabled(true);

    QSqlQueryModel * Files = new QSqlQueryModel(this);
    QSqlQuery Query(CurrentTexFilesDataBase);
    QStringList datalist;
    QString text = QString("SELECT File_Id,\"%2\" AS \"Database source\", df.Path "
                           "FROM Bib_Entries_per_File bpf "
                           "JOIN Database_Files df ON df.Id = bpf.File_Id WHERE Bib_Id = '%1' ")
            .arg(CitationKey,GlobalFilesDatabaseListNames[QFileInfo(CurrentDataBasePath).baseName()]);
    datalist.append(text);
    for (int i=0;i<GlobalFilesDatabaseList.count();i++) {
        QString DataPath = GlobalFilesDatabaseList.values()[i].databaseName();
        if(DataPath!=CurrentDataBasePath) {
            Query.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DataPath,QFileInfo(DataPath).baseName()));
            datalist.append(QString("SELECT File_Id,\"%3\" AS \"Database source\", df.Path "
                                    "FROM %2.Bib_Entries_per_File bpf "
                                    "JOIN %2.Database_Files df ON df.Id = bpf.File_Id WHERE Bib_Id = '%1'")
                            .arg(CitationKey,QFileInfo(DataPath).baseName(),GlobalFilesDatabaseListNames[QFileInfo(DataPath).baseName()]));
        }
    }
    text = datalist.join(" UNION ");
    Query.exec(text+" ORDER BY 1 ");
    Files->setQuery(Query);
    qDebug()<<text;
    ui->DatabaseFiles_per_BibEntry->setModel(Files);
    ui->DatabaseFiles_per_BibEntry->show();
    QString label = "Database files that contain entry "+CitationKey+" : "+QString::number(ui->DatabaseFiles_per_BibEntry->model()->rowCount())+" files";
    ui->CitationsLabel->setText(label);
    StretchColumnsToWidth(ui->DatabaseFiles_per_BibEntry);
    connect(ui->DatabaseFiles_per_BibEntry->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        QItemSelectionModel *select2 = ui->DatabaseFiles_per_BibEntry->selectionModel();
        int index = -1;
        if(select2->hasSelection()){
            index = select2->selectedRows().at(0).row();
        }
        loadImageFile(ui->DatabaseFiles_per_BibEntry->model()->data(ui->DatabaseFiles_per_BibEntry->model()->index(index,2)).toString(),BibFileView_2);
    });
}

//Προσωρινό
//void DataTex::loadImageFile(QString exoFile, QPdfViewer *view)
//{
//    if (exoFile.isEmpty()) {
//        return;}
//    QString pdfFile = "file://"+exoFile.replace(".tex",".pdf");
//    if(QFileInfo::exists(QUrl(pdfFile).toLocalFile())){
//        view->open(QUrl(pdfFile));
//    }
//    else{
//        view->open(QUrl("file://"+DataTex::GlobalSaveLocation+"No_Pdf.pdf"));
//    }
//}
//--

void DataTex::loadImageFile(QString exoFile, PdfViewer *view)
{
//    if (exoFile.isEmpty()) {
//        return;}
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
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
        database.setDatabaseName(FullPath);
        database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
        GlobalFilesDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
        GlobalFilesDatabaseListNames.insert(QFileInfo(FullPath).baseName(),FolderName);
        ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1));
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1),0);
    }
    else{
        AddDatabaseToTree(1,FullPath,FolderName);
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
        database.setDatabaseName(FullPath);
        database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
        GlobalDocsDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
        GlobalDocsDatabaseListNames.insert(QFileInfo(FullPath).baseName(),FolderName);
        ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(ui->OpenDatabasesTreeWidget->topLevelItem(1)->childCount()-1));
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(ui->OpenDatabasesTreeWidget->topLevelItem(1)->childCount()-1),0);
    }
}

void DataTex::UpdateCurrentDatabase(QString FullPath)
{
    QSqlQuery SaveData(DataTeX_Settings);
    SaveData.exec(QString("UPDATE Current_Database_Notes_Folder SET Value = \"%1\" WHERE Setting = 'Current_DataBase'")
                  .arg(QFileInfo(FullPath).baseName()));
    QSqlDatabase database = GlobalFilesDatabaseList[QFileInfo(FullPath).baseName()];// = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
//    database.setDatabaseName(FullPath);
//    database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
//    GlobalFilesDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
    DataTex::CurrentTexFilesDataBase = database;
    DataTex::CurrentDataBasePath = FullPath;
    DataTex::CurrentTexFilesDataBase.open();
    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    ui->CurrentBaseLabel->setText(GlobalFilesDatabaseListNames[QFileInfo(CurrentDataBasePath).baseName()]);
    FilterTables_Queries(Database_FileTableFields);
}

void DataTex::UpdateCurrentNotesDatabase(QString FullPath)
{
    QSqlQuery SaveData(DataTeX_Settings);
    SaveData.exec(QString("UPDATE Current_Database_Notes_Folder SET Value = \"%1\" WHERE Setting = 'Current_Notes_Folder'")
                  .arg(QFileInfo(FullPath).baseName()));
    QSqlDatabase database = GlobalDocsDatabaseList[QFileInfo(FullPath).baseName()];//QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FullPath).baseName());
    database.setDatabaseName(FullPath);
    database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
//    GlobalDocsDatabaseList.insert(QFileInfo(FullPath).baseName(),database);
    DataTex::CurrentNotesFolderDataBase = database;
    DataTex::CurrentNotesFolderPath = FullPath;
    DataTex::CurrentNotesFolderDataBase.open();
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",CurrentNotesFolderDataBase);
    Database_DocTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
    ui->CurrentDocBaseLabel->setText("Current ducument database : "+GlobalDocsDatabaseListNames[QFileInfo(CurrentNotesFolderPath).baseName()]);
    FilterDocuments(Database_DocumentTableColumns);
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
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM Database_Files WHERE Id = \"%1\"").arg(DatabaseFileName));
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
        deleteQuery.exec(QString("DELETE FROM Documents WHERE Id = \"%1\"").arg(DocumentFileName));
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
        if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==0){
            ui->FilesDatabaseToggle->setChecked(true);
            if(CurrentDataBasePath!=item->text(1)){UpdateCurrentDatabase(item->text(1));}
            on_FilesDatabaseToggle_clicked(true);
            DatabaseStructure(CurrentDataBasePath);

            ShowDataBaseFiles();
            int columns = FilesTable->model()->columnCount();
            for (int i=0;i<columns;i++) {
                FilesTable->setColumnHidden(i,false);
            }
//            FilesTable->setColumnHidden(Database_FileTableFieldNames.count(),true);
            FilesTable->setColumnHidden(columns-1,true);
            FilesTable->setColumnHidden(columns-2,true);
            FilesTable->setColumnHidden(columns-3,true);
            loadDatabaseFields();

            //
            ui->FilesTagFilter->setChecked(false);
            delete filesTagLine;
            CreateCustomTagWidget();
            on_ComboCount_currentIndexChanged(0);
        }
        else if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==1){
            ui->DocumentsDatabaseToggle->setChecked(true);
            if(CurrentNotesFolderPath!=item->text(1)){UpdateCurrentNotesDatabase(item->text(1));}
            on_DocumentsDatabaseToggle_clicked(true);
            DatabaseStructure(CurrentNotesFolderPath);

            ShowDocuments();
            loadDatabaseFields();
            LoadTableHeaders(DocumentsTable,Database_DocTableFieldNames);
            ui->DocumentsTagFilter->setChecked(false);
            delete docsTagLine;
            CreateCustomTagWidget();
            on_ComboCount_currentIndexChanged(1);
        }
        QWidget * w = ui->tabWidget->tabBar()->tabButton(0,QTabBar::RightSide);
        int count = Optional_Metadata_Ids[QFileInfo(CurrentDataBasePath).baseName()].count();
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
    SqlFunctions::ShowAllDatabaseFiles = "SELECT df.Id ,ft.FileType "
            ",f.Name , replace(group_concat(DISTINCT c.Name),',','|') , "
            "replace(group_concat(DISTINCT s.Name),',','|') "
            ",replace(group_concat(DISTINCT et.Name),',','|') ,";
    QStringList Select;
    QStringList DataFields;
    for (int i=6;i<list.count();i++) {
            Select.append("\""+list.at(i)+"\"");
            DataFields.append("(\"df\".\""+list.at(i)+"\" LIKE "+"\"%replace"+list.at(i)+
                              "%\" OR \"df\".\""+list.at(i)+"\" ISNULL)");
    }
    SqlFunctions::ShowAllDatabaseFiles += Select.join(",")+",df.FileType,ft.Solvable,"
                                                             "replace(group_concat(DISTINCT t.Tag_Id),',','|')  ";
    SqlFunctions::ShowAllDatabaseFiles += "FROM Database_Files df "
            "JOIN FileTypes ft ON ft.Id = df.FileType "
            "JOIN Fields f ON f.Id = df.Field "
            "JOIN Chapters_per_File cpf ON cpf.File_Id=df.Id "
            "JOIN Chapters c ON c.Id = cpf.Chapter_Id "
            "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
            "JOIN Sections s  ON s.Id = spf.Section_Id  "
            "LEFT JOIN Sections_Exercises se ON se.Section_Id = s.Id "
            "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
            "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
            "LEFT JOIN Tags_per_File t ON t.File_Id = df.Id ";
    SqlFunctions::FilesTable_UpdateQuery = SqlFunctions::ShowAllDatabaseFiles;
    SqlFunctions::ShowAllDatabaseFiles += "GROUP BY df.Id "
                                          "ORDER BY df.rowid;";

//    qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
//    qDebug()<<SqlFunctions::ShowAllDatabaseFiles;
}

void DataTex::FilterDocuments(QStringList list)
{
    SqlFunctions::FilterDatabaseDocuments = "SELECT d.*,replace(group_concat(DISTINCT t.Tag_Id),',','|') AS Custom_Tags "
                                            "FROM Documents d "
                                            "LEFT JOIN Tags_per_Document t ON t.Document_Id = d.Id ";
}

void DataTex::FilterBibliographyTable(QStringList list)
{
    SqlFunctions::ShowBibliographyEntries.replace("otherBibFields",
                  SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Bibliography_Fields WHERE ROWID>6 ORDER BY ROWID",
                  DataTex::Bibliography_Settings).join(","));
    SqlFunctions::FilterBibliographyEntries = SqlFunctions::ShowBibliographyEntries;
    SqlFunctions::FilterBibliographyEntries.remove("ORDER BY b.ROWID");
//    QStringList filters;
//    for (int i=0;i<list.count();i++) {
//         filters.append("(\""+list.at(i)+"\" LIKE "+"\"%replace"+list.at(i)+
//                "%\" OR \""+list.at(i)+"\" ISNULL)");
//    }
//    SqlFunctions::FilterBibliographyEntries += "HAVING "+filters.join(" AND ");
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
}

void DataTex::on_ComboCount_currentIndexChanged(int index)
{
    QSqlDatabase database;
    int i = ui->stackedWidget->currentIndex();

    switch (i) {
    case 0:
        database = CurrentTexFilesDataBase;
        break;
    case 1:
        database = CurrentNotesFolderDataBase;
        break;
    case 2:
        database = Bibliography_Settings;
        break;
    }
    updateTableView(ui->CountFilesTable,ui->ComboCount->currentData().toString(),database,this);
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
    AddPreamble.exec(QString("INSERT OR IGNORE INTO Preambles (Id,Name,Preamble_Content) VALUES (\"%1\",\"%2\",\"%3\")")
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
    QStringList FileDatabaseIds = SqlFunctions::Get_StringList_From_Query("SELECT FileName FROM DataBases",DataTeX_Settings);
    QStringList FileDatabaseNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM DataBases",DataTeX_Settings);
    QStringList DocumentsDatabaseIds = SqlFunctions::Get_StringList_From_Query("SELECT FileName FROM Note_Folders",DataTeX_Settings);
    QStringList DocumentsDatabaseNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Note_Folders",DataTeX_Settings);
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
    newDatabase.setConnectOptions("QSQLITE_ENABLE_REGEXP");
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
            SaveData.exec(QString("INSERT INTO Databases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE Current_Database_Notes_Folder SET Value = \"%1\" WHERE Setting = 'Current_DataBase'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentTexFilesDataBase);
//            QStringList BibIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Bibliography'",CurrentTexFilesDataBase);
//            QStringList BibNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Bibliography'",CurrentTexFilesDataBase);

            QSqlQuery add(DataTeX_Settings);
            for (int i=0;i<MetadataIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO Metadata (Id,Name,Basic) VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO Metadata_per_Database (Database_FileName,Metadata_Id) VALUES (\""+baseName+"\",\""+MetadataIds.at(i)+"\")");
            }
//            for (int i=0;i<BibIds.count();i++) {
//                add.exec(QString("INSERT OR IGNORE INTO Bibliography (Id,Name,Basic) VALUES (\""+BibIds.at(i)+"\",\""+BibNames.at(i)+"\",0)"));
//                add.exec("INSERT OR IGNORE INTO Bibliographic_Fields_per_Database (Database,Bibliographic_Field) VALUES (\""+baseName+"\",\""+BibIds.at(i)+"\")");
//            }
            ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1));
            on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1),0);
        }
        else if(Tables.contains("Documents")){
            AddDatabaseToTree(1,filePath,FolderName);
            newDatabase.setDatabaseName(filePath);
            DataTex::CurrentNotesFolderDataBase = newDatabase;
            DataTex::CurrentNotesFolderPath = filePath;
            GlobalDocsDatabaseList.insert(QFileInfo(filePath).baseName(),newDatabase);
            DataTex::CurrentNotesFolderDataBase.open();
            SaveData.exec(QString("INSERT INTO Notes_Folders (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE Current_Database_Notes_Folder SET Value = \"%1\" WHERE Setting = 'Current_Notes_Folder'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",CurrentNotesFolderDataBase);
            QSqlQuery add(DataTeX_Settings);
            for (int i=0;i<MetadataIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO DocMetadata (Id,Name,Basic) VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO DocMetadata_per_Database (Database_FileName,Metadata_Id) VALUES (\""+baseName+"\",\""+MetadataIds.at(i)+"\")");
            }
            ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(ui->OpenDatabasesTreeWidget->topLevelItem(1)->childCount()-1));
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
        ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(parent)->child(0));
        on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(parent)->child(0),0);
        delete DeleteItem[0];
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabasePath).absolutePath()));}
    }
}

void DataTex::UpdateDocument()
{
//    UpdateDocumentContent * upDoc = new UpdateDocumentContent(this,DocumentFilePath,FilePathsInADocument,DatabasesInADocument);
//    connect(upDoc,SIGNAL(),this,SLOT());
//    upDoc->show();
//    upDoc->activateWindow();
}

void DataTex::CloneCurrentDocument()
{
    NotesDocuments * clone = new NotesDocuments(this,false,{},DocumentFilePath,true,DocumentContent);
//    connect(clone,SIGNAL(createnewdocument(QString,QString)),this,SLOT(CreateNewSheet(QString,QString)));
    connect(clone,&NotesDocuments::clonedocument,this,[&](QString path,QString Content,QSqlDatabase database){
        QSqlQuery write(database);
        write.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(CurrentNotesFolderPath,QFileInfo(CurrentNotesFolderPath).baseName()));
        write.exec(QString("INSERT INTO Files_per_Document (Document_Id,File_Id,Files_Database_Source) SELECT \"%1\",File_Id,Files_Database_Source "
                           "FROM \"%3\".Files_per_Document WHERE Document_Id = '%2'")
                       .arg(QFileInfo(path).baseName(),DocumentFileName,QFileInfo(CurrentNotesFolderPath).baseName()));
        write.exec(QString("INSERT INTO BibEntries_per_Document (Bib_Id,Document_Id) SELECT Bib_Id,\"%1\" "
                           "FROM \"%3\".BibEntries_per_Document WHERE Document_Id = '%2' ")
                       .arg(QFileInfo(path).baseName(),DocumentFileName,QFileInfo(CurrentNotesFolderPath).baseName()));
        write.exec(QString("DETACH DATABASE \"%1\"").arg(QFileInfo(CurrentNotesFolderPath).baseName()));
        CreateNewSheet(path,Content);
        QList<QTreeWidgetItem*> clist = ui->OpenDatabasesTreeWidget->findItems(GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()], Qt::MatchContains|Qt::MatchRecursive, 0);
        ui->OpenDatabasesTreeWidget->setCurrentItem(clist[0]);
        on_OpenDatabasesTreeWidget_itemClicked(clist[0],0);
        qDebug()<<clist[0]->text(0);
    });
    connect(clone,&NotesDocuments::insertfiles,this,[=](){InsertFiles();});
    clone->show();
    clone->activateWindow();
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
        qDebug()<<LineText;
        if(LineText.contains("%@ Database source: ")){
            database = LineText.remove("%@ Database source: ");
            break;
        }
    }
    Tex.close();
    QTextStream text(&Tex);
    Tex.open (QIODevice::ReadOnly);
    QString content = text.readAll();
    Tex.close();
    if(database.isEmpty()){
        QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Error"),tr("This file was doens't have a database source.\n"
                                                  "Do you wish to open it anyway?"),
                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        }
        else{
            QHash<QString, QString> meta;
            meta.insert("FileType",QString());
            meta.insert("Field",QString());
            meta.insert("Path",QString());
            meta.insert("Difficulty",QString());
            meta.insert("Preamble",QString());
            meta.insert("BuildCommand",QString());
            meta.insert("FileDescription",QString());
            NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,content);
            connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
                    SLOT(EditNewBaseFile(QString,QString)));
            Edit->show();
            Edit->activateWindow();
            return;
        }
    }
    else if(database != QFileInfo(CurrentDataBasePath).baseName()){
        QMessageBox::StandardButton resBtn =
        QMessageBox::warning( this,tr("Error"),tr("This file was created from another database\n"
                                                  "and some metadada values may differ.\n"
                                                  "Do you wish to add this file to the current database anyway?"),
                              QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        }
        else{
            QHash<QString, QString> meta;
            meta.insert("FileType",QString());
            meta.insert("Field",QString());
            meta.insert("Path",QString());
            meta.insert("Difficulty",QString());
            meta.insert("Preamble",QString());
            meta.insert("BuildCommand",QString());
            meta.insert("FileDescription",QString());
            NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,content);
            connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
                    SLOT(EditNewBaseFile(QString,QString)));
            Edit->show();
            Edit->activateWindow();
            return;
        }
    }

    QString csvFile = filePath;
    csvFile.replace(".tex",".csv");

//    QFile CSV(csvFile);
    if(QFileInfo::exists(csvFile)){
        QHash<QString,QString> MetadataValues = CsvFunctions::ReadCsv(csvFile);
        MetadataValues.remove("Bibliography");
        MetadataValues.remove("Solved");
        MetadataValues.remove("Date");
        QString content = MetadataValues["FileContent"];
        QStringList Chapters = MetadataValues["Chapter"].split("|");
        QStringList Sections = MetadataValues["Section"].split("|");
        QStringList SubSections = MetadataValues["ExerciseType"].split("|");
        MetadataValues.remove("Chapter");
        MetadataValues.remove("Section");
        MetadataValues.remove("ExerciseType");
        MetadataValues.remove("Id");
        NewDatabaseFile * Edit = new NewDatabaseFile(this,MetadataValues,content,Chapters,Sections,SubSections,true,QFileInfo(filePath).baseName(),false,true);
        connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
                SLOT(EditNewBaseFile(QString,QString)));
        Edit->show();
        Edit->activateWindow();
    }
}

QString DataTex::BibSourceCode()
{
    ui->SourceCodeText->clear();
    QString sourceCode = Bib_ValueList["Citation_Key"]->text()+"{"+
            Bib_ValueList["Document_Type"]->text()+",\n";
//    if(!Bib_ValueList["title"]->text().isEmpty() && !Bib_ValueList["title"]->text().isNull()){
//            sourceCode += "\t title = {"+Bib_ValueList["title"]->text()+"}"+",\n";}

    for(int i = 2;i<Bib_ValueList.count();i++){
        QString text = Bib_ValueList[Bib_ValueList.keys().at(i)]->text();
        if(!text.isEmpty() && !text.isNull() && Bib_ValueList[Bib_ValueList.keys().at(i)]->hasAcceptableInput()){
                sourceCode += "\t "+Bib_ValueList.keys().at(i)+" = {"+text+"}"+",\n";}
    }

    sourceCode += "}";
    ui->SourceCodeText->setPlainText(sourceCode);
    return sourceCode;
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
            QString query = "UPDATE Database_Files SET ";
            for (int i=0;i<lineList.count();i++) {
                lineList.at(i)->setReadOnly(true);
                labelList.at(i)->setText(labelList.at(i)->text().remove("*"));
                labelList.at(i)->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
                ui->DifficultySpinBox->setReadOnly(true);
                ui->DifficultySpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
                list.append("\""+Optional_Metadata_Ids[QFileInfo(CurrentDataBasePath).baseName()][i]+"\" = \""+lineList[i]->text()+"\" ");
            }
            query += list.join(",");
            query += " WHERE Id = \""+DatabaseFileName+"\"";
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
    QSqlQuery BibliographyQuery(CurrentTexFilesDataBase);
    BibliographyQuery.exec(QString("INSERT INTO Bib_Entries_per_File (Bib_Id,File_Id) VALUES('%1','%2')")
                           .arg(ui->BibEntriesCombo->currentText(),DatabaseFileName));
    QSqlQuery values(Bibliography_Settings);
    values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
                        "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
                        "WHERE Citation_Key = '%1'").arg(ui->BibEntriesCombo->currentText()));
    int i = ui->BibEntriesTable->rowCount();
    ui->BibEntriesTable->insertRow(i);
    ui->BibEntriesTable->setItem(i,0 , new QTableWidgetItem(ui->BibEntriesCombo->currentText()));
    while(values.next()){
        ui->BibEntriesTable->setItem(i,1 , new QTableWidgetItem(values.value(0).toString()));
        ui->BibEntriesTable->setItem(i,2 , new QTableWidgetItem(values.value(1).toString()));
    }
    QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentTexFilesDataBase);
    connect(ui->BibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(text));
    });
    ui->addBibEntry->setEnabled(false);
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

void DataTex::StretchColumns(QTreeView * Tree, float stretchFactor)
{
    QFont myFont("Noto Sans", 10);
    QFontMetrics fm(myFont);
    for (int c = 0; c < Tree->model()->columnCount(); ++c)
    {
        QString str = Tree->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Tree->setColumnWidth(c,width);
        Tree->header()->setMinimumSectionSize(100);
    }
}

void DataTex::FileClone(QString filePath,QString FileContent)
{
    QHash<QString,QString> metadata;
    metadata.insert("FileType",FileType);
    metadata.insert("Field",Field);
    metadata.insert("Difficulty",QString::number(Difficulty));
    metadata.insert("Preamble",CurrentPreamble);
    metadata.insert("BuildCommand",CurrentBuildCommand);
    metadata.insert("FileDescription",FileDescription);
//    metadata.insert("FileContent",FileContent);
    QStringList ExistingSolutions = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT Solution_Path "
                        "FROM Solutions_per_File spf "
                        "WHERE File_Id = \"%1\"").arg(QFileInfo(filePath).baseName()),CurrentTexFilesDataBase);
    QHash<QString,QString> SolMap;
    QSqlQuery SolutionContents(CurrentTexFilesDataBase);
    SolutionContents.exec(QString("SELECT Id,FileContent "
                                  "FROM Database_Files "
                                  "WHERE Id IN (SELECT Solution_Id FROM Solutions_per_File WHERE File_Id = '%1')").arg(QFileInfo(filePath).baseName()));
    while(SolutionContents.next()){
        QString text = NewDatabaseFile::ClearMetadataFromContent(SolutionContents.value(1).toString());
        SolMap.insert(SolutionContents.value(0).toString()/*"Sol"+QString::number(SolutionContents.at())*/,text);
    }
    NewDatabaseFile * cloneFile = new NewDatabaseFile(this,metadata,FileContent,{},{},{},false,filePath,true);
    connect(cloneFile,&NewDatabaseFile::acceptClone,this,[=](QString Database,QString newFilePath,QString newFileContent){
        QList<QTreeWidgetItem*> clist = ui->OpenDatabasesTreeWidget->findItems(GlobalFilesDatabaseListNames[QFileInfo(Database).baseName()], Qt::MatchContains|Qt::MatchRecursive, 0);
        ui->OpenDatabasesTreeWidget->setCurrentItem(clist[0]);
        on_OpenDatabasesTreeWidget_itemClicked(clist[0],0);
        loadDatabaseFields();
        EditNewBaseFile(newFilePath,newFileContent);
        QFile::copy(filePath.left(filePath.lastIndexOf("."))+".pdf",
                    newFilePath.left(newFilePath.lastIndexOf("."))+".pdf");

        if(ExistingSolutions.count()>0){
            //Copy any existing solutions of the chosen file
            CloneSolution = newFilePath;
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
            for(int i=0;i<ExistingSolutions.count();i++){
                QString newSolution = CloneSolution;
                newSolution.replace(".tex","-"+QString::number(i+1)+".tex");
                QFile file(newSolution);
                file.open(QIODevice::ReadWrite);
                QTextStream stream(&file);
                file.resize(0);
                stream<<NewFileText(newSolution,SolMap.values()[i],GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                file.close();
                QFile::copy(ExistingSolutions[i].left(ExistingSolutions[i].lastIndexOf("."))+".pdf",
                            newSolution.left(newSolution.lastIndexOf("."))+".pdf");

                QSqlQuery solved(GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                solved.exec(QString("INSERT INTO Solutions_per_File (Solution_Id,Solution_Path,File_Id) VALUES ('%1', '%2','%3');")
                            .arg(QFileInfo(newSolution).baseName(),newSolution,QFileInfo(newFilePath).baseName()));
                solved.exec(SqlFunctions::UpdateSolution.arg(QFileInfo(newFilePath).baseName()));
                QSqlQuery Columns(GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                QHash<QString,QString> mapValues;
                QStringList valuesList;
                QSqlQuery RowValues(GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                RowValues.exec(SqlFunctions::SelestExerciseRow.arg(QFileInfo(newFilePath).baseName()));
                while (RowValues.next())
                {
                    QSqlRecord record = RowValues.record();
                    for(int i=0; i < record.count(); i++)
                    {
                        valuesList << record.value(i).toString();
                    }
                }
                QStringList ColumnNames;
                Columns.exec("SELECT \"name\" FROM pragma_table_info(\'Database_Files\');");
                while (Columns.next()){ColumnNames.append(Columns.value(0).toString());}

                for(int i=0;i<ColumnNames.count();i++){
                    mapValues.insert(ColumnNames.at(i),valuesList.at(i));
                }
                mapValues.insert("Date",QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm"));
                mapValues.insert("FileType",FileTypeId);
                mapValues.insert("Path",newSolution);
                mapValues.insert("Solved_Prooved","-");
                mapValues.insert("Id",QFileInfo(newSolution).baseName());
                mapValues.remove("File_Content");
                QStringList WriteValues;
                WriteValues.append("(\""+mapValues.values().join("\",\"")+"\",\""+SolMap.values()[i]+"\")");
                //-------------------
                QSqlQuery writeExercise(GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                writeExercise.exec("INSERT INTO \"Database_Files\" (\""+mapValues.keys().join("\",\"")+"\",\"FileContent\") VALUES "+WriteValues.join(","));
                QStringList ChaptersList = SqlFunctions::Get_StringList_From_Query(
                            QString("SELECT cpf.Chapter_Id "
                            "FROM Chapters_per_File cpf "
                            "WHERE cpf.File_Id = '%1'").arg(QFileInfo(newFilePath).baseName()),GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                foreach(QString chapter,ChaptersList){
                    writeExercise.exec(QString("INSERT INTO Chapters_per_File (File_Id, Chapter_Id) VALUES ('%1', '%2');").arg(QFileInfo(newSolution).baseName(),chapter));
                }
                QStringList SectionsList = SqlFunctions::Get_StringList_From_Query(
                            QString("SELECT spf.Section_Id "
                            "FROM Sections_per_File spf "
                            "WHERE spf.File_Id = '%1'").arg(QFileInfo(newFilePath).baseName()),GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                foreach(QString section,SectionsList){
                    writeExercise.exec(QString("INSERT INTO Sections_per_File (File_Id, Section_Id) VALUES ('%1', '%2');").arg(QFileInfo(newSolution).baseName(),section));
                }
                QStringList SubSectionsList = SqlFunctions::Get_StringList_From_Query(
                            QString("SELECT epf.ExerciseType_Id "
                            "FROM ExerciseTypes_per_File epf "
                            "WHERE epf.File_Id = '%1'").arg(QFileInfo(newFilePath).baseName()),GlobalFilesDatabaseList[QFileInfo(Database).baseName()]);
                foreach(QString subsection,SubSectionsList){
                    writeExercise.exec(QString("INSERT INTO ExerciseTypes_per_File (File_Id, ExerciseType_Id) VALUES ('%1', '%2');").arg(QFileInfo(newSolution).baseName(),subsection));
                }
            }
        }
    });
    cloneFile->show();
    cloneFile->activateWindow();
}

void DataTex::SelectNewFileInModel(QTableView * table,QString newFile)
{
    QAbstractItemModel * m = table->model();
    QModelIndex ix = table->currentIndex();
    while (table->model()->canFetchMore(ix))
           table->model()->fetchMore(ix);
    QModelIndexList matchList = m->match(m->index(0,0), Qt::DisplayRole,
                               newFile, -1,  Qt::MatchFlags(Qt::MatchContains|Qt::MatchWrap));

    if(matchList.count()>=1){
        table->setCurrentIndex(matchList.first());
        table->scrollTo(matchList.first());
    }
//    table->selectRow(rows-1);
//    table->selectRow(Items[0].row());
//    table->verticalScrollBar()->setValue(table->verticalScrollBar()->maximum());
}

void DataTex::StretchColumnsToWidth(QTableView * table)
{
    for (int c = 0; c < table->horizontalHeader()->count()-1; ++c)
    {
        table->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void DataTex::NewBibliographyEntry()
{
    BibEntry * newbib = new BibEntry(this,false,false,QHash<QString,QString>());
    connect(newbib,&BibEntry::accepted,this,[=](){
        QString id = newbib->getBibValues()["Citation_Key"];
        ShowBibliography();
        SelectNewFileInModel(BibliographyTable,id);
    });
    newbib->show();
    newbib->activateWindow();
}

void DataTex::EditBibliographyEntry()
{
    QHash<QString,QString> editValues;
    foreach(QString text,Bib_ValueList.keys()){
        editValues.insert(text,Bib_ValueList[text]->text());
    }
    foreach(QString text,Bib_ValueList_Opt.keys()){
        editValues.insert(text,Bib_ValueList_Opt[text]->text());
    }
    BibEntry * editbib = new BibEntry(this,true,false,editValues);
    editbib->show();
    editbib->activateWindow();

}

void DataTex::DeleteBibliographyEntry()
{
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the entry %1").arg(CitationKey));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(Bibliography_Settings);
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM Bibliography WHERE Citation_Key = '%1'").arg(CitationKey));
        QSqlQuery deleteCitation(CurrentTexFilesDataBase);
        deleteCitation.exec(QString("DELETE FROM Bib_Entries_per_File WHERE Bib_Id = '%1'").arg(CitationKey));
        ShowBibliography();
    }
}

void DataTex::OpenBibliographyEntry()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open existing file"), QDir::homePath(), tr("Latex file (*.bib)"));
    if(filePath.isEmpty()) {
        return;
    }

    QStringList BibEntriesFound;
    QFile BibFile(filePath);
    QList<QHash<QString,QString>> BibEntryList;
    QString text;
    BibFile.open (QIODevice::ReadOnly);
    QTextStream Content(&BibFile);
    QString BibContent = Content.readAll();
    BibFile.close();
    BibContent = BibContent.trimmed();
    QTextStream TrimmedContent(&BibContent);
    while (!TrimmedContent.atEnd()){
        QString LineText=TrimmedContent.readLine();
        if(LineText == "}"){
            text += "}";
            text = text.trimmed();
            BibEntriesFound.append(text);
            text.clear();
            continue;
        }
        if(LineText.startsWith("@") || LineText != "}"){
           text += LineText+"\n";
        }
    }
    foreach(QString text,BibEntriesFound){
        QHash<QString,QString> values;
        QTextStream entry(&text);
        while (!entry.atEnd()){
            QString LineText=entry.readLine();
            if(LineText.startsWith("@")){
                values.insert("Document_Type",LineText.section("{",0,0));
                values.insert("Citation_Key",LineText.section("{",1,1));
                values["Citation_Key"].chop(1);
            }
            else if(LineText != "}"){
                QString key = LineText.section("=",0,0);
                key = key.trimmed();
                QString value = LineText.section("=",1,1);
                value = value.trimmed();
                value.remove(0,1);
                value.chop(2);
                values.insert(key,value);
            }
            if(LineText == "}"){break;}
        }
        BibEntryList.append(values);
    }
    QWizard *w = new QWizard(this);
    QStringList CitationKeys = SqlFunctions::Get_StringList_From_Query("",Bibliography_Settings);
    for(int i = 0;i<BibEntryList.count();i++){
        QGridLayout *gl = new QGridLayout(this);
        QWizardPage *page = new QWizardPage(this);
        page->setLayout(gl);
        w->addPage(page);
        BibEntry *b = new BibEntry(this,false,true,BibEntryList[i]);
        gl->addWidget(b);
        connect(w,&QWizard::currentIdChanged,this,[=](){
            w->button(QWizard::NextButton)->setEnabled(!b->BibEntryExists(BibEntryList[i]["Citation_Key"]));
            w->button(QWizard::FinishButton)->setEnabled(!b->BibEntryExists(BibEntryList[i]["Citation_Key"]));
        });
        connect(b,&BibEntry::citationExists,this,[=](bool exists){
            w->button(QWizard::NextButton)->setEnabled(!exists);
            w->button(QWizard::FinishButton)->setEnabled(!exists);
        });
        connect(w,&QWizard::accepted,this,[=](){
            b->InsertValues(b->getBibValues());
        });
    }
    connect(w,&QWizard::accepted,this,[=](){
        ShowBibliography();
    });
    w->setGeometry(100,100,800,500);
    w->show();
    w->activateWindow();
}

void DataTex::OpenAuthorsEditors()
{
    BibAuthorsEditors *authors = new BibAuthorsEditors(this);
    authors->show();
    authors->activateWindow();
}

QStringList DataTex::GetListWidgetItems(QListWidget * list)
{
    QStringList items;
    for(int i=0;i<list->count();i++){
        items.append(list->item(i)->text());
    }
    return items;
}

void DataTex::on_BibliographyDatabaseToggle_clicked(bool checked)
{
    DocumentActions->setEnabled(!checked);
    DocTools_ToolBar->setEnabled(!checked);
    NewDocument->setEnabled(!checked);
    AddDocument->setEnabled(!checked);

    LatexTools_ToolBar->setEnabled(!checked);
    LatexFileActions->setEnabled(!checked);
    NewLatexFile->setEnabled(!checked);
    OpenLatexFile->setEnabled(!checked);
    DeleteLatexFile->setEnabled(!checked);
    EditLatexFile->setEnabled(!checked);
    SolveLatexFile->setEnabled(!checked);
    FileCloneToOtherDB->setEnabled(!checked);


    NewBibEntry->setEnabled(checked);
    AuthorsEditors->setEnabled(checked);
    OpenBibEntry->setEnabled(checked);
    ui->stackedWidget->setCurrentIndex(2);
    ui->EnableSortingBib->setChecked(bibSorting);
}

void DataTex::on_NewBibEntry_CurrentFile_clicked()
{
    BibEntry * newEntryforFile = new BibEntry(this,false,false,QHash<QString,QString>());
    connect(newEntryforFile,&BibEntry::accepted,this,[=](){
        QSqlQuery BibliographyQuery(CurrentTexFilesDataBase);
        QString citationKey = newEntryforFile->getBibValues()["Citation_Key"];
        BibliographyQuery.exec(QString("INSERT INTO Bib_Entries_per_File (Bib_Id,File_Id) VALUES('%1','%2')")
                               .arg(citationKey,DatabaseFileName));
        ui->BibEntriesCombo->addItem(citationKey);
        QSqlQuery values(Bibliography_Settings);
        values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
                            "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
                            "WHERE Citation_Key = '%1'").arg(ui->BibEntriesCombo->currentText()));
        int i = ui->BibEntriesTable->rowCount();
        ui->BibEntriesTable->insertRow(i);
        ui->BibEntriesTable->setItem(i,0 , new QTableWidgetItem(ui->BibEntriesCombo->currentText()));
        while(values.next()){
            ui->BibEntriesTable->setItem(i,1 , new QTableWidgetItem(values.value(0).toString()));
            ui->BibEntriesTable->setItem(i,2 , new QTableWidgetItem(values.value(1).toString()));
        }
        QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentTexFilesDataBase);
        connect(ui->BibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
            ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(text));
        });
        ui->BibEntriesCombo->setEnabled(false);
    });
    newEntryforFile->show();
    newEntryforFile->activateWindow();
}

void DataTex::on_addDocBibEntry_clicked()
{
    QString newEntry = ui->DocBibEntriesCombo->currentText();
    QSqlQuery BibliographyQuery(CurrentNotesFolderDataBase);
    BibliographyQuery.exec(QString("INSERT INTO BibEntries_per_Document (Bib_Id,Document_Id) VALUES('%1','%2')")
                           .arg(newEntry,DocumentFileName));

    int i = ui->BibPerFileTree->topLevelItem(1)->childCount();
    QTreeWidgetItem * item = new QTreeWidgetItem();
    QSqlQuery values(Bibliography_Settings);
    values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
                        "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
                        "WHERE Citation_Key = '%1'").arg(newEntry));
    item->setText(0,newEntry);
    while(values.next()){
        item->setText(1,values.value(0).toString());
        item->setText(2,values.value(1).toString());
    }
    ui->BibPerFileTree->topLevelItem(1)->addChild(item);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    item->setCheckState(0,Qt::Checked);
//    foreach(QSqlDatabase database,GlobalFilesDatabaseList.values()){
//        QSqlQuery FilesPerEntry(database);
//        FilesPerEntry.exec(QString("SELECT File_Id,Path "
//                                   "FROM Bib_Entries_per_File bpf "
//                                   "JOIN Database_Files df ON df.Id = bpf.File_Id "
//                                   "WHERE Bib_Id = '%1'").arg(newEntry));
//        while(FilesPerEntry.next()){
//            QTreeWidgetItem * item = new QTreeWidgetItem();
//            item->setText(0,FilesPerEntry.value(0).toString());
//            item->setText(1,FilesPerEntry.value(1).toString());
//            item->setText(2,QFileInfo(database.databaseName()).baseName());
//            ui->BibPerFileTree->topLevelItem(1)->child(i)->addChild(item);
//        }
//    }

    QStringList BibEntriesInDocument = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM BibEntries_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentNotesFolderDataBase);
    connect(ui->DocBibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addDocBibEntry->setEnabled(!BibEntriesInDocument.contains(text));
    });
    ui->addDocBibEntry->setEnabled(false);
}

void DataTex::on_NewBibEntry_CurrentDocument_clicked()
{
    BibEntry * newEntryforDocument = new BibEntry(this,false,false,QHash<QString,QString>());
    connect(newEntryforDocument,&BibEntry::accepted,this,[=](){
        QSqlQuery BibliographyQuery(CurrentNotesFolderDataBase);
        QString citationKey = newEntryforDocument->getBibValues()["Citation_Key"];
        QString DocType = newEntryforDocument->getBibValues()["Document_Type"];
        BibliographyQuery.exec(QString("INSERT INTO BibEntries_per_Document (Bib_Id,Document_Id) VALUES('%1','%2')")
                               .arg(citationKey,DocumentFileName));
        ui->DocBibEntriesCombo->addItem(citationKey);
        int i = ui->BibPerFileTree->topLevelItem(1)->childCount();
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0,citationKey);
        item->setText(1,DocType);
        ui->BibPerFileTree->topLevelItem(1)->addChild(item);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(0,Qt::Checked);
        QStringList BibEntriesInDocument = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM BibEntries_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentNotesFolderDataBase);
        connect(ui->DocBibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
            ui->addDocBibEntry->setEnabled(!BibEntriesInDocument.contains(text));
        });
        ui->DocBibEntriesCombo->setEnabled(false);
    });
    newEntryforDocument->show();
    newEntryforDocument->activateWindow();
}

int DataTex::TreeItemIndex(QModelIndex index)
{
    int depth = 0;
    while ( index.parent().isValid() )
    {
        index = index.parent();
        depth++;
    }
    return depth;
}

void DataTex::CreateCustomTagWidget()
{
    filesTagLine = new TagsFilterWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",DataTex::CurrentTexFilesDataBase));
    filesTagLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ui->gridLayout_20->addWidget(filesTagLine,1,0,1,4);
    connect(filesTagLine,&TagsFilterWidget::SelectedTags,this,[=](QStringList list){
        FilesTable->setFilter(FilesTable->model()->columnCount()-1,list.join(","));
        qDebug()<<list;
    });
    filesTagLine->setVisible(false);
    connect(ui->FilesTagFilter, &QPushButton::toggled, this, [=](bool checked){
        filesTagLine->setVisible(checked);
    });

    docsTagLine = new TagsFilterWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",DataTex::CurrentNotesFolderDataBase));
    docsTagLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ui->gridLayout_31->addWidget(docsTagLine,1,0);
    connect(docsTagLine,&TagsFilterWidget::SelectedTags,this,[=](QStringList list){
        DocumentsTable->setFilter(DocumentsTable->model()->columnCount()-1,list.join(","));
        qDebug()<<list;
    });
    docsTagLine->setVisible(false);
    connect(ui->DocumentsTagFilter, &QPushButton::toggled, this, [=](bool checked){
        docsTagLine->setVisible(checked);
    });
}

void DataTex::LoadCountCombo(int index)
{
    switch(index) {
    case 0:
        ui->ComboCount->clear();
        ui->ComboCount->addItem(tr("Field"),QVariant(SqlFunctions::CountFiles_by_Field));
        ui->ComboCount->addItem(tr("Chapter"),QVariant(SqlFunctions::CountFiles_by_Chapter));
        ui->ComboCount->addItem(tr("Section"),QVariant(SqlFunctions::CountFiles_by_Section));
        ui->ComboCount->addItem(tr("Exercise type"),QVariant(SqlFunctions::CountFiles_by_ExerciseType));
        ui->ComboCount->addItem(tr("File type"),QVariant(SqlFunctions::CountFiles_by_FileType));
        break;
    case 1:
        ui->ComboCount->clear();
        ui->ComboCount->addItem(tr("Document type"),QVariant(SqlFunctions::CountDocs_by_DocType));
        ui->ComboCount->addItem(tr("Basic folder"),QVariant(SqlFunctions::CountDocs_by_BasicFolder));
        ui->ComboCount->addItem(tr("Sub folder"),QVariant(SqlFunctions::CountDocs_by_SubFolder));
        ui->ComboCount->addItem(tr("Subsub folder"),QVariant(SqlFunctions::CountDocs_by_SubsubFolder));
        break;
    case 2:
        ui->ComboCount->clear();
        ui->ComboCount->addItem(tr("Document type"),QVariant(SqlFunctions::CountBib_by_DocType));
        ui->ComboCount->addItem(tr("Author"),QVariant(SqlFunctions::CountBib_by_Author));
        ui->ComboCount->addItem(tr("Publisher"),QVariant(SqlFunctions::CountBib_by_Publisher));
        break;
    }
    StretchColumnsToWidth(ui->CountFilesTable);
}
void DataTex::mousePressEvent(QMouseEvent* event)
{
//    FilesRightClickMenu->hide();
}


void DataTex::ReorderFiles(QTableView * table,QMap<int,QString> exerciseOrder)
{
    for(int i=0;i<table->model()->rowCount();i++){
        QString file = exerciseOrder.value(i);
        int row = exerciseOrder.key(file);
        while(row>=i){
//            // take whole rows
//            QList<QTableWidgetItem*> sourceItems = takeRow(table,sourceRow);
//            QList<QTableWidgetItem*> destItems = takeRow(table,destRow);

//            // set back in reverse order
//            setRow(table,sourceRow, destItems);
//            setRow(table,destRow, sourceItems);

//table->model()->index(row,0,this);
//table->model()->moveRow(table->model()->index(row,0,this),row,table->model()->index(i,0,this),i);

        }
    }
}

// takes and returns the whole row
//QList<QTableWidgetItem*> DataTex::takeRow(QTableView * table,int row)
//{
//    QList<QTableWidgetItem*> rowItems;
//    for (int col = 0; col < table->model()->columnCount(); ++col)
//    {
//        rowItems << table->model()->takeItem(row, col);
//    }
//    return rowItems;
//}

//// sets the whole row
//void DataTex::setRow(QTableView * table,int row, const QList<QTableWidgetItem*>& rowItems)
//{
//    for (int col = 0; col < table->model()->columnCount(); ++col)
//    {
//        table->model()->setItem(row, col, rowItems.at(col));
//    }
//}

void DataTex::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
