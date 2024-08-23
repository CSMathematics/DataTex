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
#include "settings.h"
#include "solvedatabaseexercise.h"
#include "basefolder.h"
#include "sqlfunctions.h"
#include "notesdocuments.h"
#include "addfiletoeditor.h"
#include "datatables.h"
#include "backup.h"
#include "solutionsdocument.h"
//#include <poppler-qt5.h>
//#include "pdfviewer.h"
#include "databasesync.h"
#include "preamblesettings.h"
#include "latexeditorwidget.h"
#include "updatedocumentcontent.h"
#include "csvfunctions.h"
#include "bibentry.h"
#include "bibauthorseditors.h"
#include "edithistory.h"
#include "clonedatabasefile.h"
#include "minisplitter.h"
#include "databasecreator.h"
#include "qdiffview/qdiffview.h"
#include "encryptdatabase.h"


QSqlDatabase DataTex::DataTeX_Settings = QSqlDatabase::addDatabase("QSQLITE","Settings");
QSqlDatabase DataTex::Bibliography_Settings = QSqlDatabase::addDatabase("QSQLITE","BibSettings");
//QSqlDatabase DataTex::CTANPackages = QSqlDatabase::addDatabase("QSQLITE","CTANPackages");
//QHash<QString,QSqlDatabase> DataTex::GlobalDatabaseList;

QHash<QString,DTXDatabase> DataTex::GlobalDatabaseList;
DTXDatabase DataTex::CurrentFilesDataBase;
DTXDatabase DataTex::CurrentDocumentsDataBase;
DTXDatabase DataTex::CurrentBibliographyDataBase;
DTXDatabase DataTex::CurrentTablesDataBase;
DTXDatabase DataTex::CurrentFiguresDataBase;
DTXDatabase DataTex::CurrentCommandsDataBase;
DTXDatabase DataTex::CurrentPreamblesDataBase;
DTXDatabase DataTex::CurrentPackagesDataBase;
DTXDatabase DataTex::CurrentClassesDataBase;
DTXDatabase DataTex::CurrentDTXDataBase;
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
QHash<QString,QString> DataTex::BuildCommands;
QHash<int,DTXBuildCommand> DataTex::DTXBuildCommands;
//QHash<QString,QStringList> DataTex::Optional_Metadata_Ids;
//QHash<QString,QStringList> DataTex::Optional_Metadata_Names;
QHash<QString,QStringList> DataTex::Optional_DocMetadata_Ids;
QHash<QString,QStringList> DataTex::Optional_DocMetadata_Names;
QStringList DataTex::DocTypesIds;
QStringList DataTex::DocTypesNames;
QTranslator DataTex::translator;
QString DataTex::currentlanguage;
QString DataTex::datatexpath;
QStringList DataTex::SVG_IconPaths;

QChartView * DTXDashBoard::ShowPieChart(QWidget *parent,QList<QStringList> info)
{

    QPieSeries *series = new QPieSeries(parent);
    series->setHoleSize(0.45);
    // slice->setExploded();

    for(QStringList item : info){
        QPieSlice *slice = series->append(item[0], item[2].toInt());
        slice->setColor(QColor("#ff7100").darker(100+25*item[1].toInt()));
        slice->setBorderColor(QColor("#282828"));
        slice->setBorderWidth(3);
        slice->setLabel(item[0]);
        // slice->setLabelBrush(QColor("white"));
    }
    QChartView *chartView = new QChartView(parent);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->legend()->setAlignment(Qt::AlignRight);
    chartView->chart()->legend()->setFont(QFont("Arial", 14));
    chartView->chart()->setTitle("Databases chart");
    chartView->chart()->addSeries(series);
    chartView->chart()->setBackgroundBrush(QBrush(QColor("#282828")));
    chartView->chart()->legend()->setLabelBrush(QColor("#ffffff"));
    chartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
    chartView->chart()->setAnimationEasingCurve(QEasingCurve::OutCubic);
    // chartView->chart()->setAnimationDuration(3000);
    return chartView;
}

DataTex::DataTex(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataTex)
{
    QDirIterator symbolIterator(":/images/MathSymbols/" , QStringList() << "*", QDir::Files,QDirIterator::Subdirectories);
    while (symbolIterator.hasNext()){
        SVG_IconPaths.append(symbolIterator.next());
    }
    SVG_IconPaths.sort();
    ui->setupUi(this);
//    ui->menubar->setHidden(true);
    ui->statusbar->addWidget(ui->statusbarWidget);
    foreach(QAbstractButton * bt,ui->MenuButtons->buttons()){
        int page = abs(ui->MenuButtons->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            for(int i=0;i<ui->OpenDatabasesTreeWidget->topLevelItemCount();i++){
                ui->OpenDatabasesTreeWidget->topLevelItem(i)->setExpanded(false);
            }
            if(page >= DTXDatabaseType::FilesDB+1 && page <= DTXDatabaseType::ClassesDB+1){
                ui->stackedWidget->setCurrentIndex(1);
            }
            else{
                ui->stackedWidget->setCurrentIndex(0);
            }
            ui->TabMenuWidget->setCurrentIndex(page);
            if(page>0)
            ui->OpenDatabasesTreeWidget->topLevelItem(page-1)->setExpanded(true);
        });
    }
    foreach(QAbstractButton * bt,ui->SideBarButtonGroup->buttons()){
        int page = abs(ui->SideBarButtonGroup->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            ui->SideBarMenuWidget->setCurrentIndex(page);
        });
    }
    foreach(QAbstractButton * bt,ui->ActionButtonGroup->buttons()){
        int page = abs(ui->ActionButtonGroup->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            ui->PreviewStackedWidget->setCurrentIndex(page);
        });
    }
    ui->SqlQueryTabWidget->tabBar()->setTabsClosable(true);
    connect(ui->NewSqlButton,&QAbstractButton::clicked,this,[=](){
        int tabs = ui->SqlQueryTabWidget->tabBar()->count();
        ui->SqlQueryTabWidget->tabBar()->insertTab(tabs,(QString("Sql Query %1").arg(QString::number(tabs+1))));
    });
    connect(ui->SqlQueryTabWidget->tabBar(),&QTabBar::tabCloseRequested,this,[=](int tab_index){
        ui->SqlQueryTabWidget->tabBar()->removeTab(tab_index);
    });
    connect(ui->RunSqlButton,&QAbstractButton::clicked,this,[=](){
//        ui->SqlQueryTabWidget->currentIndex();
    });
    //Show side panel
    connect(ui->ShowFolderView,&QPushButton::toggled,this,[&](bool checked){
        ui->SideBar->setVisible(checked);
    });
    ui->ShowFolderView->setChecked(true);
    //---------------
    //Move menu panel
    connect(ui->MenuPosition,&QPushButton::toggled,this,[=](bool checked){
        if(checked){
            ui->horizontalLayout_33->removeWidget(ui->SideMenu);
            ui->SideMenu->setLayout(new QVBoxLayout());
            ui->SideMenuLayOut->setDirection(QBoxLayout::TopToBottom);
            ui->SideMenuLayOut->setSpacing(0);
            ui->splitter_4->insertWidget(0,ui->SideMenu);

            foreach(QAbstractButton * bt,ui->MenuButtons->buttons()){
                qobject_cast<QToolButton*>(bt)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                bt->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            }
            ui->SettingsButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            ui->HelpButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            ui->SettingsButton->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ui->HelpButton->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ui->splitter_4->setSizes(QList<int>({1,200,200,200}));
        }
        else{
            ui->horizontalLayout_33->insertWidget(3,ui->SideMenu);
            ui->SideMenu->setLayout(new QHBoxLayout());
            ui->SideMenuLayOut->setDirection(QBoxLayout::LeftToRight);
            ui->SideMenuLayOut->setSpacing(0);


            foreach(QAbstractButton * bt,ui->MenuButtons->buttons()){
                qobject_cast<QToolButton*>(bt)->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                bt->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            }
            ui->SettingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            ui->HelpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            ui->SettingsButton->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ui->HelpButton->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
            ui->splitter_4->setSizes(QList<int>({200,300,300}));
        }
    });
    //----------------

    //Adjust window size
    QScreen * screen = QGuiApplication::primaryScreen();
    QRect  screenRect = screen->geometry();
    resize(0.9*screenRect.width(),0.9*screenRect.height());
    move(QPoint(0.05*screenRect.width(),0.05*screenRect.height()));
    //-------------------------

    //Load Database files table widget
    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_20->addWidget(FilesTable,3,0,1,5);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    FilesTable->setAlternatingRowColors(true);
    //---------------------------

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(settingsPath);
    if (!dir.exists())dir.mkpath(settingsPath);

    CreateMenus_Actions();
    CreateToolBars();
    CreateBuildCommands();//LaTeX Compile commands
    ui->FilesDatabaseToggle->setChecked(true);//Set File database tab as default
    SettingsDatabase_Variables();//Database names and variables, Latex commands

    connect(ui->ShowHideNenuBar,&QPushButton::toggled,this,[&](bool checked){
        ui->menubar->setVisible(checked);
    });

    connect(ui->SplitTexPdf,&QPushButton::toggled,this,[&](bool checked){
        if(checked){
            ui->splitter_8->setSizes(QList<int>{floor(0.25*ui->splitter_8->size().width()),floor(0.75*ui->splitter_8->size().width())*checked});
            ui->splitter_8->insertWidget(1,ui->FileEditWidget);
            ui->PreviewStackedWidget->setCurrentIndex(2);
        }
        else{
            ui->verticalLayout_6->addWidget(ui->FileEditWidget);
            ui->SourceCodeButton->setChecked(true);
            ui->PreviewStackedWidget->setCurrentIndex(1);
        }
        ui->PdfViewButton->setHidden(checked);
        ui->SourceCodeButton->setHidden(checked);
    });

    connect(ui->ShowHideTable,&QPushButton::toggled,this,[&](bool checked){
        ui->splitter_3->setVisible(checked);
    });
    ui->ShowHideTable->setChecked(true);

    //Load language------
    readSettings();
    if (!DataTex::translator.isEmpty()){
        QCoreApplication::removeTranslator(&DataTex::translator);
    }
    if(!currentlanguage.isEmpty() && !currentlanguage.isNull() && QFile::exists(":/languages/DataTex_"+currentlanguage+".qm")){
        DataTex::translator.load(":/languages/DataTex_"+currentlanguage+".qm");
    }
    else if(QFile::exists(":/languages/DataTex_"+QLocale::system().name()+".qm")){
        DataTex::translator.load(":/languages/DataTex_"+QLocale::system().name()+".qm");
    }
    QCoreApplication::installTranslator(&DataTex::translator);
    //----------------------

    //------Load databases, check encryption and add them to the treewidget

    GlobalDatabaseList.clear();
    QList<QStringList> Databases = SqlFunctions::GetRecordList("SELECT * FROM DataBases",DataTeX_Settings);
//    QStringList FilesDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM DataBases WHERE Type = \"FDB\"",DataTeX_Settings);
    for (int i=0;i<Databases.count();i++ ) {
        bool isDBmissing = false;
        if(!QFileInfo::exists(Databases[i].at(3))){
            isDBmissing = true;
            QFile file(datatexpath+QFileInfo(Databases[i].at(3)).fileName());
            QDir dir(QFileInfo(Databases[i].at(3)).absolutePath());
            if (!dir.exists()){
                dir.mkpath(".");
            }
            file.copy(Databases[i].at(3));
        }
        DTXDatabase DTXDB;
        DTXDB.BaseName = Databases[i].at(0);
        DTXDB.Description = Databases[i].at(1);
        DTXDB.Type = QString(Databases[i].at(2)).toInt();
        DTXDB.Path = Databases[i].at(3);
        DTXDB.Prefix = Databases[i].at(4);
        DTXDB.UsePrefix = !DTXDB.Prefix.isEmpty();
        DTXDB.Username = Databases[i].at(5);
        DTXDB.Password = Databases[i].at(6);
        DTXDB.Encrypt = (!DTXDB.Username.isEmpty() || !DTXDB.Username.isNull()) &&
                        (!DTXDB.Password.isEmpty() || !DTXDB.Password.isNull());
        DTXDB.IsConnected = QString(Databases[i].at(7)).toInt();
        QString table1;
        QString table2;
        switch (DTXDB.Type) {
        case DTXDatabaseType::FilesDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::DocumentsDB:
            table1 = "DocMetadata_per_Database";
            table2 = "DocMetadata";
            break;
        case DTXDatabaseType::BibliographyDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::TablesDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::FiguresDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::CommandsDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::PreamblesDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::PackagesDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        case DTXDatabaseType::ClassesDB:
            table1 = "Metadata_per_Database";
            table2 = "Metadata";
            break;
        }
        QList<QStringList> list = SqlFunctions::GetRecordList(
            QString("SELECT m.Id,md.Metadata_Name,m.Basic,m.DataType,m.VisibleInTable,"
                    "m.ROWID FROM %1 md "
                    "JOIN %2 m ON md.Metadata_Id = m.Id "
                    "WHERE Database_FileName = '%3' AND m.DatabaseType = %4").arg(table1,table2,DTXDB.BaseName,QString::number(DTXDB.Type)),DataTeX_Settings);
        for(int i=0;i<list.count();i++){
            DTXDBFieldInfo fieldInfo;
            fieldInfo.Id = list[i][0];
            fieldInfo.Name = list[i][1];
            fieldInfo.isBasic = list[i][2].toInt();
            fieldInfo.DataType = list[i][3];
            fieldInfo.isVisibleInTable = list[i][4].toInt();
            fieldInfo.Index = list[i][5].toInt();
            DTXDB.DBFieldInfoList.append(fieldInfo);
        }

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",DTXDB.BaseName);
        database.setDatabaseName(DTXDB.Path);
        database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
        DTXDB.Database = database;

        if(DTXDB.Encrypt){
            encryptionData.insert(DTXDB.BaseName,{DTXDB.Username,DTXDB.Password,QString::number(DTXDB.Type)});
            DTXDB.IsConnected = false;
        }
        else if(!DTXDB.Encrypt && DTXDB.IsConnected){
            database.open();
            QList<QStringList> fileTypes = SqlFunctions::GetRecordList("SELECT * FROM FileTypes",database);
            for(QStringList item : fileTypes){
                qDebug()<<fileTypes;
                DTXFileType fileType;
                fileType.Id = item[0];
                fileType.Name = item[1];
                fileType.FolderName = item[2];
                fileType.Solvable = (DTXSolutionState)item[3].toInt();
                fileType.BelongsTo = item[4];
//                fileType.Description = item[5];
                fileType.BuiltIn = true;
                fileType.DBType = (DTXDatabaseType)DTXDB.Type;
                DTXDB.FileTypes.append(fileType);
            }
        }
        GlobalDatabaseList.insert(DTXDB.BaseName,DTXDB);
        AddDatabaseToTree(DTXDB);
//        isDBEncrypted.insert(DTXDB.BaseName,DTXDB.Encrypt);
        if(isDBmissing){
            RestoreDB(0,database);
        }
    }
    connect(this,&DataTex::sendDBConnectionInfo,this,[&](int DBConnected){
        int total = GlobalDatabaseList.count();
        ConnectAllDatabase->setEnabled(DBConnected<total);
        DisconnectAllDatabase->setEnabled(DBConnected>0);
    });
    AreAllDBConnected();
    //-------------------------------------------------

    //Set current database for each database type
    QString filesdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentFilesDatabase);
    QString docsdb = SqlFunctions::GetCurrentDataBase(DataTeX_Settings,SqlFunctions::SelectCurrentDocumentsDatabase);
    CurrentFilesDataBase = GlobalDatabaseList.value(filesdb);
    CurrentDocumentsDataBase = GlobalDatabaseList.value(docsdb);
    CurrentDTXDataBase = CurrentFilesDataBase;
    DatabaseStructure(CurrentFilesDataBase.Path);// Show database folder structure in tree view
    //---------------------------------------------

    //Load Database metadata from entries
    Database_FileTableFields = CurrentFilesDataBase.getIdsList();
    Database_FileTableFieldNames = CurrentFilesDataBase.getNamesList();
    Database_DocTableFieldNames = CurrentDocumentsDataBase.getIdsList();
    Database_DocumentTableColumns = CurrentDocumentsDataBase.getNamesList();
    BibliographyTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
    BibliographyFieldIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
    //-----------------------------------

//    FilterTables_Queries(Database_FileTableFields);
    SqlFunctions::ShowAllFiles(Database_FileTableFields);//Δημιουργία query για προβολή των αρχείων
    FilterDocuments(Database_DocumentTableColumns);//Δημιουργία query για προβολή των εγγράφων
    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή βιβλιογραφίας

//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή πινάκων
//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή σχημάτων
//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή εντολών
//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή preambple
//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή πακέτων
//    FilterBibliographyTable(BibliographyTableColumns);//Δημιουργία query για προβολή κλάσεων

    ShowDataBaseFiles();//Load and preview latex files database table

    //---Add Pdf widgets to main layout----
    PdfFileView = new QPdfViewer(this);
    ui->verticalLayout_22->addWidget(PdfFileView);
    PdfFileView->show();

    FileFromDocumentView = new QPdfViewer(this);
    ui->gridLayout_8->addWidget(FileFromDocumentView);
    FileFromDocumentView->show();

    BibFileView = new QPdfViewer(this);
    ui->gridLayout_28->addWidget(BibFileView);
    BibFileView->show();

    DocDependenciesFileView = new QPdfViewer(this);
    ui->gridLayout_35->addWidget(DocDependenciesFileView);
    DocDependenciesFileView->show();
    //-------------------------------

    // ----- ui settings -------
    CompileMenu->setEnabled(false);
    PdfLatex->setEnabled(false);
    DeleteLatexFile->setEnabled(false);
    EditLatexFile->setEnabled(false);
    OpenPath->setEnabled(false);
    DeleteDocument->setEnabled(false);
    ui->addBibEntry->setEnabled(false);
    ui->removeBibEnrty->setEnabled(false);

    ui->OpenDatabasesTreeWidget->hideColumn(1);
    ui->OpenDatabasesTreeWidget->hideColumn(2);
    ui->OpenDatabasesTreeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->OpenDatabasesTreeWidget->expandAll();
    ui->splitter_3->setSizes(QList<int>({400, 1}));
    ui->splitter_4->setSizes(QList<int>{floor(0.5*ui->splitter_4->size().width()),floor(0.5*ui->splitter_4->size().width())});
    ui->splitter_5->setSizes(QList<int>({400, 1}));
    ui->splitter_7->setSizes(QList<int>({300, 300}));

    //---- Load preambles -----------------------
    FilesPreambleCombo = new QComboBox(this);
    QSqlQuery LoadPreambles(DataTeX_Settings);
    LoadPreambles.exec("SELECT Id,Name,Preamble_Content FROM Preambles;");
    while(LoadPreambles.next()){
        FilesPreambleCombo->addItem(LoadPreambles.value(1).toString(),QVariant(LoadPreambles.value(0).toString()));
    }
    FilesPreambleCombo->setCurrentIndex(-1);
    FilesPreambleCombo->setEnabled(false);
    ui->FileContentCommandsHorizontalLayout->addWidget(FilesPreambleCombo);
    loadDatabaseFields();
    LoadCountCombo(0);

    int columns = FilesTable->model()->columnCount();
    FilesTable->setColumnHidden(columns-1,true);
    FilesTable->setColumnHidden(columns-2,true);
    FilesTable->setColumnHidden(columns-3,true);

    //Global save location path----
    QString path = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = \"SaveLocation\"",DataTeX_Settings);
    GlobalSaveLocation = (!path.isEmpty()) ? path : QDir::homePath();
    //-----------------------------
    CloseDatabasefile->setEnabled(false);

    //Get Texlive path-----------
    QProcess *process = new QProcess;
    process->start("which",QStringList()<<"tlmgr");
    process->waitForBytesWritten();
    process->waitForFinished(-1);
    TexLivePath = QString(process->readAllStandardOutput()).remove("tlmgr\n");
    //---------------------------

    ui->SaveDocBibContent->setEnabled(false);
    connect(ui->DocBibSourceCode,&QTextEdit::textChanged,this,[=](){
        ui->SaveDocBibContent->setEnabled(true);
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
        QSqlQuery needsUpdate(CurrentDocumentsDataBase.Database);
        needsUpdate.exec(QString("UPDATE Documents SET Bibliography = \"%1\" WHERE Id = \"%2\"").arg(BibContent,DocumentFileName));
    });

    //------ Table filters ---------
    ui->ClearFiltersFD->setEnabled(false);
    connect(ui->ClearFiltersFD, &QPushButton::clicked, this, [=](){
        for (int i=0;i<FilesTable->model()->columnCount();i++) {
            FilesTable->filterHeader()->clearFilters();
            ui->ClearFiltersFD->setEnabled(false);
        }
    });
    //-----------------------------

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
        QSqlQuery write(CurrentFilesDataBase.Database);
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
        ui->DocBibSourceCode->setText(list.join(""));
    });
    connect(ui->BibPerFileTree,&QTreeWidget::itemClicked,this, [=](QTreeWidgetItem * item){
        if(TreeItemIndex(ui->BibPerFileTree->currentIndex())==1){
            ui->stackedWidget_5->setCurrentIndex(0);
            ui->BibPerFileTree->setHeaderLabels({tr("Citation key"),tr("Document type"),tr("Title")});
        }
        else if(TreeItemIndex(ui->BibPerFileTree->currentIndex())==2){
            ui->stackedWidget_5->setCurrentIndex(1);
            FileCommands::ShowPdfInViewer(item->text(1),BibFileView);
            ui->BibPerFileTree->setHeaderLabels({tr("File name"),tr("Path"),tr("Database source")});
        }
    });
    CreateCustomTagWidget();

    //Files Sorting---------------------------
    //Check file sorting from settings
    filesSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortFiles'",DataTeX_Settings).toInt();
    docsSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortDocuments'",DataTeX_Settings).toInt();
    bibSorting = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SortBibliography'",DataTeX_Settings).toInt();

    //Toggle file sorting
    connect(ui->EnableSortingFiles,&QPushButton::toggled,this,[=](bool checked){
        filesSorting = checked;
        QStringList list;
        for(int i= 0;i<FilesTable->model()->columnCount();i++){
            list.append(FilesTable->filterHeader()->filterValue(i));
        }
        updateFilter(list);
        qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
        FilesTable->filterHeader()->adjustPositions();
    });
    //--------------------------

    //--------- Right click menus -------------
    RightClick = new RightClickMenu(this,FilesTable,3);

    connect(ui->stackedWidget,&QStackedWidget::currentChanged,this,[&](int index){LoadCountCombo(index);});
    StretchColumnsToWidth(ui->CountFilesTable);
    connect(ui->ComboCount,&QComboBox::currentTextChanged,this,[=](QString text){
        LoadTableHeaders(ui->CountFilesTable,{text,tr("Number")});
    });
    connect(ui->DocDepContentPreview,&QPushButton::clicked,this,[&](){ui->stackedWidget_7->setCurrentIndex(0);});
    connect(ui->DocDepPdfPreview,&QPushButton::clicked,this,[&](){ui->stackedWidget_7->setCurrentIndex(1);});

    ui->EnableSortingFiles->setChecked(filesSorting);
    connect(FilesTable,&QAbstractItemView::doubleClicked,this,[&](){
        FilesTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    });
    connect(FilesTable->selectionModel(),&QItemSelectionModel::currentRowChanged,this,[&](){
        FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        FilesTable->selectRow(FilesTable->currentIndex().row());
    });
    connect(FilesTable,&ExtendedTableWidget::filesfound,this,[=](int files){
        qDebug()<<"filecount=0"<<files;
        ui->CurrentBaseLabel->setText(CurrentFilesDataBase.Description+" : "+QString::number(files)+tr(" files"));
    });
    connect(ui->ShowDescription,&QPushButton::toggled,this,[=](bool checked){
        ui->splitter_3->setSizes(QList<int>({(1-0.2*checked)*height(),0.2*checked*height()}));
        if(checked){
            ui->stackedWidget_8->setCurrentIndex(0);
        }
    });

    QList<QStringList> FilesDBCount = SqlFunctions::GetRecordList("SELECT dt.Name,Type,count(Type) FROM DataBases d JOIN DatabaseTypes dt ON dt.Id=d.Type GROUP BY Type",DataTeX_Settings);
    // FilesDBCount += tr(" Files databases");
    // ui->FilesDBCount->setText(FilesDBCount);

    DTXDashBoard dashBoard;
    ui->gridLayout_11->addWidget(dashBoard.ShowPieChart(this,FilesDBCount));

}

void DataTex::setDefaultAction(QAction* action)
{
    CompileCommands->setDefaultAction(action);
}

void DataTex::CreateMenus_Actions()
{//---------- File Menu - Database actions ---------------------------
    FileMenu = menuBar()->addMenu(tr("File"));
    NewDatabasefile = CreateNewAction(FileMenu,NewDatabasefile,SLOT(CreateDatabase()),"Ctrl+N",QIcon::fromTheme("DatabaseAdd"),tr("&Create a new database"));
    OpenDatabasefile = CreateNewAction(FileMenu,OpenDatabasefile,SLOT(OpenLoadDatabase()),"Ctrl+O",QIcon::fromTheme("DatabaseOpen"),tr("&Open a database"));
    CloseDatabasefile = CreateNewAction(FileMenu,CloseDatabasefile,SLOT(RemoveCurrentDatabase()),"Ctrl+E",QIcon::fromTheme("EditDelete"),tr("&Close current database"));
    SyncDatabasefile = CreateNewAction(FileMenu,SyncDatabasefile,SLOT(DatabaseSyncFiles()),"Ctrl+S",QIcon::fromTheme("UpdateDocument"),tr("&Sync files to database"));
    SaveAsDatabasefile = CreateNewAction(FileMenu,SaveAsDatabasefile,[=](){/*SelectNewFileInModel(FilesTable);*/},"Ctrl+Shift+S",QIcon::fromTheme("DocumentSave"),tr("&Save As..."));
    ImportDatabaseFrom = CreateNewAction(FileMenu,ImportDatabaseFrom,[](){},"",QIcon::fromTheme("DatabaseImport"),tr("&Import"));
    ExportDatabaseTo = CreateNewAction(FileMenu,ExportDatabaseTo,[](){},"",QIcon::fromTheme("DatabaseExport"),tr("&Export"));
    EncryptDB = CreateNewAction(FileMenu,EncryptDB,[=](){
            EncryptDatabase * enc = new EncryptDatabase(this,CurrentDTXDataBase);
            connect(enc,&EncryptDatabase::DBEncrypted,this,[&](QString DBId){
                QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->findItems(DBId,Qt::MatchExactly | Qt::MatchRecursive,2).at(0);
                item->setIcon(0,QIcon::fromTheme("Locked"));
                GlobalDatabaseList[DBId].Database.close();
                GlobalDatabaseList[DBId].IsConnected = false;
                ui->OpenDatabasesTreeWidget->setCurrentItem(item);
                on_OpenDatabasesTreeWidget_itemClicked(item,0);
                GlobalDatabaseList[DBId].Encrypt = true;
            });
            enc->show();
            enc->activateWindow();
        },"",QIcon::fromTheme("Key"),tr("Encrypt database"));
    ConnectDatabase = CreateNewAction(FileMenu,ConnectDatabase,[=](){
            QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->currentItem();
            SetDatabaseConnected(item);
            AreAllDBConnected();
        },"",QIcon::fromTheme("ConnectDB"),tr("Connect selected database"));
    DisconnectDatabase = CreateNewAction(FileMenu,DisconnectDatabase,[=](){
            QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->currentItem();
            SetDatabaseDisconnected(item);
            AreAllDBConnected();
        },"",QIcon::fromTheme("DisconnectDB"),tr("Disconnect selected database"));
    ConnectAllDatabase = CreateNewAction(FileMenu,ConnectAllDatabase,[=](){
            for(DTXDatabase DTXDB : GlobalDatabaseList.values()){
                if(!DTXDB.IsConnected){
                    QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->findItems(DTXDB.BaseName,Qt::MatchExactly | Qt::MatchRecursive,2).at(0);
                    SetDatabaseConnected(item);
                }
//                ConnectAllDatabase->setEnabled(false);
//                DisconnectAllDatabase->setEnabled(true);
            }
        },"",QIcon::fromTheme("ConnectAllDB"),tr("Connect all databases"));
    DisconnectAllDatabase = CreateNewAction(FileMenu,DisconnectAllDatabase,[=](){
            for(DTXDatabase DTXDB : GlobalDatabaseList.values()){
                if(DTXDB.IsConnected){
                    QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->findItems(DTXDB.BaseName,Qt::MatchExactly | Qt::MatchRecursive,2).at(0);
                    SetDatabaseDisconnected(item);
                }
                DisconnectAllDatabase->setEnabled(false);
                ConnectAllDatabase->setEnabled(true);//If all are unencrypted
            }
        },"",QIcon::fromTheme("DisconnectAllDB"),tr("Disconnect all databases"));
    FileMenu->addSeparator();
    QMenu * ExportMenu = new QMenu(tr("Export As..."),this);
//    ExportAsSQL = CreateNewAction(ExportMenu,ExportAsSQL,SLOT(FunctionInProgress()),"Ctrl+",QIcon::fromTheme("DatabaseSave"),tr("&Export as Sql"));
    FileMenu->addMenu(ExportMenu);
    FileMenu->addSeparator();
    CloseApp = CreateNewAction(FileMenu,CloseApp,[](){qApp->closeAllWindows();},"Ctrl+",QIcon::fromTheme("EditDelete"),tr("&Close"));
//---------- Edit Menu ---------------------------
    EditMenu = menuBar()->addMenu(tr("&Edit"));
//---------- View Menu - Database actions ---------------------------
    ViewMenu = menuBar()->addMenu(tr("&View"));
    ShowFileDescription = CreateNewAction(ViewMenu,ShowFileDescription,[=](){
            ui->splitter_3->setSizes(QList<int>{floor(0.75*ui->splitter_3->size().height()),floor(0.25*ui->splitter_3->size().height())*ShowFileDescription->isChecked()});
        },"",QIcon(""),tr("File description"));
    ShowFileDescription->setCheckable(true);
    ShowFileDescription->setChecked(true);
    ShowFileTabWidget = CreateNewAction(ViewMenu,ShowFileTabWidget,[=](){
            ui->splitter_4->setSizes(QList<int>{floor(0.5*ui->splitter_4->size().width()),floor(0.5*ui->splitter_4->size().width())*ShowFileTabWidget->isChecked()});
        },"",QIcon(""),tr("File info"));
    ShowFileTabWidget->setCheckable(true);
    ShowFileTabWidget->setChecked(true);
    SplitTexAndPdf = CreateNewAction(ViewMenu,SplitTexAndPdf,[=](){
            bool checked = SplitTexAndPdf->isChecked();
//            QSplitter *splitter = new QSplitter(Qt::Horizontal,this);
            if(checked){
                ui->splitter_8->setSizes(QList<int>{floor(0.25*ui->splitter_8->size().width()),floor(0.75*ui->splitter_8->size().width())*checked});
                ui->splitter_8->insertWidget(1,ui->FileEditWidget);
                FileCommands::ShowPdfInViewer(DatabaseFilePath,PdfFileView);
//                splitter->addWidget(ui->FileEdit);
//                splitter->addWidget(PdfFileView);
            }
            else{
                ui->verticalLayout_6->addWidget(ui->FileEditWidget);
                ui->SourceCodeButton->setChecked(true);
            }
            ui->PdfViewButton->setHidden(checked);
            ui->SourceCodeButton->setHidden(checked);
//            ShowFileTex->setEnabled(!checked);
//            ShowFilePdf->setEnabled(!checked);
//            ui->stackedWidget_2->widget(1)->setVisible(!checked);
        },"",QIcon(""),tr("Split tex and pdf view"));
    SplitTexAndPdf->setCheckable(true);
    ViewMenu->addSeparator();
//---------- Tool Menu - File actions ---------------------------
    ToolMenu = menuBar()->addMenu(tr("&LaTeX database files"));
    NewLatexFile = CreateNewAction(ToolMenu,NewLatexFile,SLOT(NewDatabaseBaseFile()),"Ctrl+Shift+N",QIcon::fromTheme("Tex"),tr("&Create a file"));
    ImportTableFromTablesDB = CreateNewAction(ToolMenu,ImportTableFromTablesDB,SLOT(NewTableFile()),"",QIcon::fromTheme("Table"),tr("&Create a table"));
    ImportFigureFromFiguresDB = CreateNewAction(ToolMenu,ImportFigureFromFiguresDB,SLOT(NewGraphicsFile()),"",QIcon::fromTheme("ChartArea"),tr("&New graphics file"));
    OpenLatexFile = CreateNewAction(ToolMenu,OpenLatexFile,SLOT(AddFileToDatabase()),"Ctrl+Shift+O",QIcon::fromTheme("DocumentOpenData"),tr("&Open a file"));
    DeleteLatexFile = CreateNewAction(ToolMenu,DeleteLatexFile,SLOT(DeleteFileFromBase()),"Ctrl+Shift+D",QIcon::fromTheme("EditDelete"),tr("&Delete current file"));
    EditLatexFile = CreateNewAction(ToolMenu,EditLatexFile,SLOT(EditFileMeta()),"Ctrl+Shift+E",QIcon::fromTheme("DocumentEdit"),tr("&Edit current file"));
    SolveLatexFile = CreateNewAction(ToolMenu,SolveLatexFile,SLOT(SolutionFile()),"Ctrl+Alt+S",QIcon::fromTheme("SolveTex"),tr("&Solve current exersive"));
    FileCloneToOtherDB = CreateNewAction(ToolMenu,FileCloneToOtherDB,[=](){
        FileClone();
    },"Ctrl+Alt+C",QIcon::fromTheme("CloneFile"),tr("&Clone this file to another database"));
    FileEditHistory = CreateNewAction(ToolMenu,FileEditHistory,[=](){
//            EditHistory * edit = new EditHistory(this,DatabaseFilePath,CurrentBuildCommand,FileContent,
//            {FileType,Field,Chapter,Section,ExerciseType,QString::number(Difficulty),DatabaseFilePath});
//            edit->show();
//            edit->activateWindow();
        },"Ctrl+Alt+E",QIcon::fromTheme("DocumentOpenRecent"),tr("&Show recent file changes"));
    ToolMenu->addSeparator();
    LatexFileActions = new QMenu("Edit actions",this);
    OpenPath = CreateNewAction(LatexFileActions,OpenPath,[=](){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DatabaseFilePath).absolutePath()));},"Ctrl+Shift+O",QIcon::fromTheme("DocumentOpenData"),tr("&Open folder"));
    QActionGroup *Action_FilePreview = new QActionGroup(this);
    Action_FilePreview->setExclusive(true);

    ShowToolBar = CreateNewAction(LatexFileActions,ShowToolBar,[=](){ui->FileEdit->toolBar->setVisible(!ui->FileEdit->toolBar->isVisible());},"",QIcon::fromTheme("topbar"),tr("&Show toolbar"));
    ShowToolBar->setCheckable(true);
    ShowToolBar->setChecked(true);
    ShowSideBar = CreateNewAction(LatexFileActions,ShowSideBar,[=](){
            ui->FileEdit->mathBar->setVisible(!ui->FileEdit->mathBar->isVisible());
            ui->FileEdit->mathBar->List->setVisible(false);
        },"",QIcon::fromTheme("sidebar"),tr("&Show toolbar"));
    ShowSideBar->setCheckable(true);
    ShowSideBar->setChecked(true);
    ToolMenu->addMenu(LatexFileActions);

//---------- Document Menu - Document actions ---------------------------
    DocToolMenu = menuBar()->addMenu(tr("&Documents"));
    NewDocument = CreateNewAction(DocToolMenu,NewDocument,SLOT(PersonalNotes()),"Ctrl+Alt+N",QIcon::fromTheme("Pdf"),tr("&Create a new document"));
    InsertFileToDocument = CreateNewAction(DocToolMenu,InsertFileToDocument,[=](){InsertFiles();},"Ctrl+Alt+I",QIcon::fromTheme("DocumentImport"),tr("&Insert files to document"));
    DeleteDocument = CreateNewAction(DocToolMenu,DeleteDocument,SLOT(DeleteDocumentFromBase()),"Ctrl+Alt+D",QIcon::fromTheme("DocumentClose2"),tr("&Delete current document"));
    AddDocument = CreateNewAction(DocToolMenu,AddDocument,SLOT(AddDocToDatabase()),"Ctrl+Alt+O",QIcon::fromTheme("AddButton"),tr("&Add document"));
    EditDocument = CreateNewAction(DocToolMenu,EditDocument,SLOT(EditDocumentDialog()),"Ctrl+Alt+U",QIcon::fromTheme("DocumentEdit"),tr("&Edit document's content and metadata"));
    CreateSolutionsDoc = CreateNewAction(DocToolMenu,CreateSolutionsDoc,SLOT(CreateSolutionsDocument()),"Ctrl+Alt+L",QIcon::fromTheme("Solve"),tr("&Create solution document"));
    UpdateDocContent = CreateNewAction(DocToolMenu,UpdateDocContent,SLOT(UpdateDocument()),"Ctrl+Alt+U",QIcon::fromTheme("UpdateDocument"),tr("&Update document's content"));
    CloneDocument = CreateNewAction(DocToolMenu,CloneDocument,SLOT(CloneCurrentDocument()),"Ctrl+Alt+C",QIcon::fromTheme("CloneFile"),tr("&Clone current document to another database"));
    DocEditHistory = CreateNewAction(DocToolMenu,DocEditHistory,[=](){
//            EditHistory * edit = new EditHistory(this,DocumentFilePath,CurrentBuildCommand,DocumentContent,{},true);
//            edit->show();
//            edit->activateWindow();
        },"Ctrl+Alt+E",QIcon::fromTheme("DocumentOpenRecent"),tr("&Show recent file changes"));

//    DocToolMenu->addSeparator();

//---------- Bibliography Menu - Bibliography actions ---------------------------
    BibliographyMenu = menuBar()->addMenu(tr("&Bibliography"));
    NewBibEntry = CreateNewAction(BibliographyMenu,NewBibEntry,[=](){NewBibliographyEntry();},"Ctrl+B",QIcon::fromTheme("BibNew"),tr("&New bibliographic entry"));
    OpenBibEntry = CreateNewAction(BibliographyMenu,OpenBibEntry,[=](){OpenBibliographyEntry();},"Ctrl+Alt+E",QIcon::fromTheme("KontactImportWizard"),tr("&Open bibliographic entry"));
    EditBibEntry = CreateNewAction(BibliographyMenu,EditBibEntry,[=](){EditBibliographyEntry();},"Ctrl+Shift+B",QIcon::fromTheme("BibEdit"),tr("&Edit current bibliographic entry"));
    DeleteBibEntry = CreateNewAction(BibliographyMenu,DeleteBibEntry,[=](){DeleteBibliographyEntry();},"Ctrl+Shift+D",QIcon::fromTheme("BibDelete"),tr("&Delete current bibliographic entry"));
    AuthorsEditors = CreateNewAction(BibliographyMenu,AuthorsEditors,[=](){OpenAuthorsEditors();},"Ctrl+Shift+E",QIcon::fromTheme("ImUser"),tr("&Authors - Editors"));

//---------- Settings Menu - Settings actions ---------------------------
    SettingsMenu = menuBar()->addMenu(tr("&Settings"));
    NewPreamble = CreateNewAction(SettingsMenu,NewPreamble,[=](){
            PreambleSettings * preamble = new PreambleSettings(this,QString());

            preamble->show();
            preamble->activateWindow();
        },"Alt+P",QIcon::fromTheme("Preamble"),tr("&Preamble menu"));
    Datatables = CreateNewAction(SettingsMenu,Datatables,SLOT(DataBaseFields()),"Alt+D",QIcon::fromTheme("Table"),tr("&Data entries"));
    GeneralSettings = CreateNewAction(SettingsMenu,GeneralSettings,SLOT(DataTeX_Preferences()),"Alt+S",QIcon::fromTheme("Settings"),tr("&Settings"));
    BackUpDatabase = CreateNewAction(SettingsMenu,BackUpDatabase,SLOT(BackUp_DataBase_Folders()),"Alt+B",QIcon::fromTheme("Hdd"),tr("&Back up database"));
    ConnectWithTexEditor = CreateNewAction(SettingsMenu,ConnectWithTexEditor,[=](){
            QString file = (ui->stackedWidget->currentIndex()) ? DocumentFilePath : DatabaseFilePath;
            QString tempFile = FileCommands::CreateTexFile(file,false,QString());
            QDesktopServices::openUrl(QUrl("file:///"+tempFile));
        },"Alt+T",QIcon::fromTheme("ConnectWithLatex"),tr("&Send to external TeX editor"));

//---------- Help Menu - Database actions ---------------------------
    HelpMenu = menuBar()->addMenu(tr("&Help"));
    Info = CreateNewAction(HelpMenu,Info,[=](){QMessageBox::information(this,tr("Info"),tr("This program is licensed to you under the terms of the\nGNU General Public License Version 3 as \npublished by the Free Software Foundation.\n\n"
                                                                                        "DataTex is using \"Extended Table Widget\" from DB Browser for SQLite\n\nThe software is under development..."),
                                                                    QMessageBox::Ok);},"Alt+I",QIcon::fromTheme("HelpAbout"),tr("Help"));
    YouTube = CreateNewAction(HelpMenu,YouTube,[=](){
            QDesktopServices::openUrl(QUrl("https://www.youtube.com/channel/UCKyVmM6aYEfIg5CFN0xn_GQ"));
        },"Alt+Y",QIcon::fromTheme("Youtube"),tr("CSMathematics channel"));
    FaceBook = CreateNewAction(HelpMenu,FaceBook,[=](){
            QDesktopServices::openUrl(QUrl("https://www.facebook.com/CSMathematics/"));
        },"Alt+F",QIcon::fromTheme("FacebookSquare"),tr("Follow us on Facebook"));
    GitHub = CreateNewAction(HelpMenu,GitHub,[=](){
            QDesktopServices::openUrl(QUrl("https://github.com/CSMathematics/DataTex"));
        },"Alt+G",QIcon::fromTheme("Github"),tr("DataTeX on GitHub"));
    Email = CreateNewAction(HelpMenu,Email,[=](){
            QDesktopServices::openUrl(QUrl("mailto:?to=csmathsoftware@gmail.com&subject=Select a subject&body=Write some text", QUrl::TolerantMode));
        },"Alt+E",QIcon::fromTheme("Envelope"),tr("Contact us"));
    Site = CreateNewAction(HelpMenu,Site,[=](){
            QDesktopServices::openUrl(QUrl("https://csmathematics.github.io/DataTex/"));
        },"Alt+S",QIcon::fromTheme("GlobeEurope"),tr("Visit our site"));
}

void DataTex::CreateToolBars()
{
    DatabaseToolBar = addToolBar(tr("File"));
    DatabaseToolBar->setObjectName("FileTools");
    DatabaseToolBar->setIconSize(QSize(20,20));
    DatabaseToolBar->addAction(NewDatabasefile);
    DatabaseToolBar->addAction(OpenDatabasefile);
    DatabaseToolBar->addAction(CloseDatabasefile);
    DatabaseToolBar->addAction(SyncDatabasefile);
    DatabaseToolBar->addAction(ImportDatabaseFrom);
    DatabaseToolBar->addAction(ExportDatabaseTo);
    DatabaseToolBar->addAction(EncryptDB);
    DatabaseToolBar->addAction(ConnectDatabase);
    DatabaseToolBar->addAction(DisconnectDatabase);
    DatabaseToolBar->addAction(ConnectAllDatabase);
    DatabaseToolBar->addAction(DisconnectAllDatabase);
//    DatabaseToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    DatabaseToolBar->setOrientation(Qt::Vertical);
    ui->gridLayout_2->addWidget(DatabaseToolBar,1,0);

    FileToolBar = addToolBar(tr("Tools"));
    FileToolBar->setObjectName("LatexTools");
    FileToolBar->setIconSize(QSize(24,24));
    FileToolBar->addAction(NewLatexFile);
    FileToolBar->addAction(ImportTableFromTablesDB);
    FileToolBar->addAction(ImportFigureFromFiguresDB);
    FileToolBar->addAction(OpenLatexFile);
    FileToolBar->addAction(DeleteLatexFile);
    FileToolBar->addAction(EditLatexFile);
    FileToolBar->addAction(SolveLatexFile);
    FileToolBar->addAction(FileCloneToOtherDB);
    FileToolBar->addAction(FileEditHistory);
//    FileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->FileActions->layout()->addWidget(FileToolBar);

    DocTools_ToolBar = addToolBar(tr("Document Tools"));
    DocTools_ToolBar->setObjectName("DocTools");
    DocTools_ToolBar->setIconSize(QSize(24,24));
    DocTools_ToolBar->addAction(NewDocument);
    DocTools_ToolBar->addAction(InsertFileToDocument);
    DocTools_ToolBar->addAction(DeleteDocument);
    DocTools_ToolBar->addAction(AddDocument);
    DocTools_ToolBar->addAction(EditDocument);
    DocTools_ToolBar->addAction(CreateSolutionsDoc);
    DocTools_ToolBar->addAction(UpdateDocContent);
    DocTools_ToolBar->addAction(CloneDocument);
    DocTools_ToolBar->addAction(DocEditHistory);
//    DocTools_ToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->DocumentActions->layout()->addWidget(DocTools_ToolBar);

    BibliographyToolBar = addToolBar(tr("Bibliography Tools"));
    BibliographyToolBar->setObjectName("BibliographyTools");
    BibliographyToolBar->setIconSize(QSize(24,24));
    BibliographyToolBar->addAction(NewBibEntry);
    BibliographyToolBar->addAction(OpenBibEntry);
    BibliographyToolBar->addAction(EditBibEntry);
    BibliographyToolBar->addAction(DeleteBibEntry);
    BibliographyToolBar->addAction(AuthorsEditors);
//    BibliographyToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->BibliographyActions->layout()->addWidget(BibliographyToolBar);


    CommandToolBar = addToolBar(tr("Commands"));
    CommandToolBar->setObjectName("CommandTools");
    CommandToolBar->setIconSize(QSize(24,24));
    CommandToolBar->addAction(NewPreamble);
    CommandToolBar->addAction(Datatables);
    CommandToolBar->addAction(GeneralSettings);
    CommandToolBar->addAction(BackUpDatabase);
    CommandToolBar->addAction(ConnectWithTexEditor);
//    CommandToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->CommandActions->layout()->addWidget(CommandToolBar);

    PreambleToolBar = addToolBar(tr("Preambles"));
    PreambleToolBar->setObjectName("PreambleTools");
    PreambleToolBar->setIconSize(QSize(24,24));
    PreambleToolBar->addAction(NewPreamble);
    PreambleToolBar->addAction(Datatables);
    PreambleToolBar->addAction(GeneralSettings);
    PreambleToolBar->addAction(BackUpDatabase);
    PreambleToolBar->addAction(ConnectWithTexEditor);
//    PreambleToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->PreambleActions->layout()->addWidget(PreambleToolBar);

    PackageToolBar = addToolBar(tr("Packages"));
    PackageToolBar->setObjectName("PackageTools");
    PackageToolBar->setIconSize(QSize(24,24));
    PackageToolBar->addAction(NewPreamble);
    PackageToolBar->addAction(Datatables);
    PackageToolBar->addAction(GeneralSettings);
    PackageToolBar->addAction(BackUpDatabase);
    PackageToolBar->addAction(ConnectWithTexEditor);
    ui->PackageClassActions->layout()->addWidget(PackageToolBar);

    ClassToolBar = addToolBar(tr("Classes"));
    ClassToolBar->setObjectName("ClassTools");
    ClassToolBar->setIconSize(QSize(24,24));
    ClassToolBar->addAction(NewPreamble);
    ClassToolBar->addAction(Datatables);
    ClassToolBar->addAction(GeneralSettings);
    ClassToolBar->addAction(BackUpDatabase);
    ClassToolBar->addAction(ConnectWithTexEditor);
//    ClassToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->TableFigureActions->layout()->addWidget(ClassToolBar);


    SettingsToolBar = addToolBar(tr("Settings"));
    SettingsToolBar->setObjectName("SettingsTools");
    SettingsToolBar->setIconSize(QSize(24,24));
    SettingsToolBar->addAction(NewPreamble);
    SettingsToolBar->addAction(Datatables);
    SettingsToolBar->addAction(GeneralSettings);
    SettingsToolBar->addAction(BackUpDatabase);
    SettingsToolBar->addAction(ConnectWithTexEditor);
//    SettingsToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->SettingsActions->layout()->addWidget(SettingsToolBar);
}

void DataTex::CreateBuildCommands()
{
//------------ Compile commands ---------------------------
    //------ Compile File Widgets ------------
    CompileCommands = new QToolButton(this);
    CompileCommands->setPopupMode(QToolButton::MenuButtonPopup);
    ui->FileContentCommandsHorizontalLayout->addWidget(CompileCommands);
    CompileMenu = new QMenu(this);
    Latex = CreateNewAction(CompileMenu,Latex,SLOT(NewDatabaseBaseFile()),"Ctrl+Alt+L",QIcon::fromTheme("LatexConfig"),"&LaTeX");
    PdfLatex = CreateNewAction(CompileMenu,PdfLatex,[=](){
            CompileToPdf();
            FileCommands::ClearOldFiles(DatabaseFilePath);
            FileCommands::ShowPdfInViewer(DatabaseFilePath,PdfFileView);
        QSqlQuery UpdateBuildCommand(CurrentFilesDataBase.Database);
        UpdateBuildCommand.exec(QString("UPDATE Database_Files SET BuildCommand = \"PdfLaTeX\" WHERE Id = \"%1\"").arg(DatabaseFileName));
        UpdateBuildCommand.exec(QString("UPDATE Database_Files SET Preamble = \"%2\" WHERE Id = \"%1\"").arg(DatabaseFileName,FilesPreambleCombo->currentData().toString()));
//        ShowDataBaseFiles();
//        FilesTable->model()->setData(FilesTable->model()->index(0,0),"s",Qt::EditRole);
//        emit dataChanged(FilesTable->model()->index(0,0),FilesTable->model()->index(0,0));
//        FilesTable->viewport()->update();
    },"Ctrl+Alt+P",QIcon::fromTheme("Pdflatex"),"&PdfLaTeX");
    XeLatex = CreateNewAction(CompileMenu,XeLatex,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&XeLaTeX");
    LuaLatex = CreateNewAction(CompileMenu,LuaLatex,SLOT(CompileToPdf()),"Ctrl+Shift+L",QIcon::fromTheme("Lualatex"),"&LuaLaTeX");
    PythonTex = CreateNewAction(CompileMenu,PythonTex,SLOT(Compile()),"Ctrl+Shift+P",QIcon::fromTheme("Pythontex"),"&PythonTex");
    Asymptote = CreateNewAction(CompileMenu,Asymptote,SLOT(CompileAsymptote()),"Ctrl+Shift+A",QIcon::fromTheme("Asymptote"),"&Asymptote");
    Metapost = CreateNewAction(CompileMenu,Metapost,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("VectorSquare"),"&Metapost");
    KnitR = CreateNewAction(CompileMenu,KnitR,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("ViewStatistics"),"&R (Statistics)");
    Lilypond = CreateNewAction(CompileMenu,Lilypond,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Music"),"&Lilypond");
    Latexmk = CreateNewAction(CompileMenu,Latexmk,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&Latexmk");
    CompileMenu->addSeparator();
    BibTex = CreateNewAction(CompileMenu,BibTex,SLOT(Compile()),"Ctrl+Shift+B",QIcon::fromTheme("AddressBookNew"),"&BibTeX");
    Biber = CreateNewAction(CompileMenu,Biber,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("AddressBookNew"),"&Biber");
    CompileMenu->addSeparator();
    MakeIndex = CreateNewAction(CompileMenu,MakeIndex,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&MakeIndex");
    Xindy = CreateNewAction(CompileMenu,Xindy,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&Xindy");
    Xindex = CreateNewAction(CompileMenu,Xindex,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&Xindex");
    MakeGlossaries = CreateNewAction(CompileMenu,MakeGlossaries,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&MakeGlossaries");

    CompileCommands->setMenu(CompileMenu);
    CompileCommands->setIconSize(QSize(20,20));
    CompileCommands->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CompileCommands->setDefaultAction(PdfLatex);

    ConvertCommands = new QToolButton(this);
    ConvertCommands->setPopupMode(QToolButton::MenuButtonPopup);
    ui->FileContentCommandsHorizontalLayout->addWidget(ConvertCommands);
    ConvertMenu = new QMenu(this);
//    PDF2PNG = CreateNewAction(ConvertMenu,PDF2PNG,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&PDF2PNG");
    DVIPS = CreateNewAction(ConvertMenu,DVIPS,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&DVIPS");
    DVI2PDF = CreateNewAction(ConvertMenu,DVI2PDF,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&DVI2PDF");
    DVI2PNG = CreateNewAction(ConvertMenu,DVI2PNG,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&DVI2PNG");
    PS2PDF = CreateNewAction(ConvertMenu,PS2PDF,SLOT(CompileToPdf()),"Ctrl+Alt+X",QIcon::fromTheme("Xelatex"),"&PS2PDF");
    ConvertCommands->setMenu(ConvertMenu);
    ConvertCommands->setIconSize(QSize(20,20));
    ConvertCommands->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ConvertCommands->setDefaultAction(DVI2PDF);

    CompileBar = new QToolBar(this);
    CompileBar->addAction(OpenPath);
    CompileBar->addSeparator();
    CompileBar->addAction(ShowToolBar);
    CompileBar->addAction(ShowSideBar);
    ui->FileContentCommandsHorizontalLayout->addWidget(CompileBar);
    CompileBar->setIconSize(QSize(16,16));

    QObject::connect(CompileCommands, SIGNAL(triggered(QAction*)),this, SLOT(setDefaultAction(QAction*)));
}

void DataTex::loadDatabaseFields()
{
    int currentVisibleBasicFieldCount = CurrentFilesDataBase.basicVisibleFieldIndexes().count();
    int currentCustomFieldCount = CurrentFilesDataBase.customFieldIds().count();
    int previousBasicFieldCount = valuelabelList.count();
    int previousCustomFieldCount = editList.count();
    qDebug()<<currentVisibleBasicFieldCount<<currentCustomFieldCount;

    if(currentVisibleBasicFieldCount<previousBasicFieldCount){
        for (int i=previousBasicFieldCount-1;i>currentVisibleBasicFieldCount-1;i--) {
            ui->gridLayout_36->removeWidget(labelList[i]);
            ui->gridLayout_36->removeWidget(valuelabelList[i]);
            delete labelList.takeAt(i);//i?
            delete valuelabelList.takeAt(i);
        }
    }
    else{
        for (int i=previousBasicFieldCount;i<currentVisibleBasicFieldCount;i++) {
            QLabel * Name = new QLabel();
            Name->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            QFont bold;
            bold.setBold(true);
            Name->setFont(bold);
            labelList.append(Name);
            ui->gridLayout_36->addWidget(Name,i,0);
            QLabel * Value = new QLabel();
            Value->setWordWrap(true);
            ui->gridLayout_36->addWidget(Value,i,1);
            valuelabelList.append(Value);
        }
    }
    if(currentCustomFieldCount<previousCustomFieldCount){
        for (int i=previousCustomFieldCount-1;i>currentCustomFieldCount-1;i--) {
            ui->gridLayout_36->removeWidget(labelList[previousBasicFieldCount+i]);
            ui->gridLayout_36->removeWidget(lineList[i]);
            ui->gridLayout_36->removeWidget(editList[i]);
            ui->gridLayout_36->removeItem(hLayoutList[i]);
            delete labelList.takeAt(previousBasicFieldCount+i);//0?
            delete lineList.takeAt(i);
            delete editList.takeAt(i);
            delete hLayoutList.takeAt(i);
        }
    }
    else{
        for (int i=previousCustomFieldCount;i<currentCustomFieldCount;i++) {
            QHBoxLayout * hbox = new QHBoxLayout();
            QLineEdit * Value = new QLineEdit();
            QPushButton * edit = new QPushButton(QIcon::fromTheme("DocumentEdit"),QString(),this);
            QLabel * Name = new QLabel();
            Name->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            QFont bold;
            bold.setBold(true);
            Name->setFont(bold);
            labelList.append(Name);
            ui->gridLayout_36->addWidget(Name,currentVisibleBasicFieldCount+i,0);
            Value->setReadOnly(true);
            edit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
            edit->setCheckable(true);
            hbox->setSpacing(0);
            hbox->addWidget(Value);
            hbox->addWidget(edit);
            ui->gridLayout_36->addLayout(hbox,currentVisibleBasicFieldCount+i,1);
            connect(edit,&QPushButton::toggled,this,[=](bool checked){
                Value->setReadOnly(!checked);
                if(checked){
                    edit->setIcon(QIcon::fromTheme("DocumentSave"));
                }
                else{
                    edit->setIcon(QIcon::fromTheme("DocumentEdit"));
                    //Save changes
                }
            });
            connect(Value,&QLineEdit::textEdited,this,[=](){
                Name->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
            });
            lineList.append(Value);
            editList.append(edit);
            hLayoutList.append(hbox);
        }
    }


    int i=0;
    for(DTXDBFieldInfo info : CurrentDTXDataBase.DBFieldInfoList){
        if(info.isVisibleInTable){
            labelList[i]->setText(info.Name);
            i++;
        }
        else{
            FilesTable->setColumnHidden(info.Index-1,true);
        }
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
    MetadataFieldNames <<tr("Name")<<tr("File Type")<<tr("Field")<<tr("Chapter")<<tr("Section")<<tr("Exercise Type")
            <<tr("Difficulty")<<tr("Path")<<tr("Date")
           <<tr("Solved_Prooved")<<tr("Bibliography")<<tr("File Content")<<tr("Preamble")<<tr("Build Command")
                  <<tr("File Description");

    BibliographyFieldNames <<tr("Citation Key")<<tr("Document Type")<<tr("Title")<<tr("Author")<<tr("Editor")
            <<tr("Publisher")<<tr("Year")<<tr("Month")<<tr("ISBN")
           <<tr("ISSN")<<tr("Pages")<<tr("Series")<<tr("Edition")<<tr("Chapter")<<tr("Number")<<tr("Volume")
          <<tr("Journal")<<tr("Institution")<<tr("School")<<tr("Issue")<<tr("Address")<<tr("DOI")<<tr("URL")
         <<tr("Language")<<tr("Location")<<tr("Subtitle")
         <<tr("Organization")<<tr("Key")<<tr("Abstract")<<tr("Note")
           <<tr("Cross reference")<<tr("Note")<<tr("Translator");

    DocMetadataNames <<tr("Name")<<tr("Title")<<tr("Document Type")<<tr("Basic folder")<<tr("Subfolder")<<tr("Subsubfolder")
            <<tr("Path")<<tr("Date")<<tr("Content")<<"Preamble"
           <<tr("LaTeX build command")<<tr("Needs update")<<tr("Bibliography")<<tr("Description")<<tr("Solution document");

    datatexpath = QDir::homePath()+QDir::separator()+".datatex"+QDir::separator();
    QDir dir(datatexpath);
    if (!dir.exists())dir.mkpath(datatexpath);

    QString DataTex_Settings_Path = datatexpath+"DataTex_Settings.db";
    QString Bibliography_Settings_Path = datatexpath+"Bibliography_Settings.db";
    QString CTANDatabasePath = datatexpath+"CTANPackagesDatabase.db";
    if(!QFileInfo::exists(Bibliography_Settings_Path)){
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
        for(int i=0;i<BibliographyFieldNames.count();i++){
            QString query = "UPDATE Bibliography_Fields SET Name = '"
                            +BibliographyFieldNames.at(i)+"' WHERE Id = '"+BibliographyList.at(i)+"'";
            WriteBibNames.exec(query);
        }
    }
    if(!QFileInfo::exists(CTANDatabasePath)){
        QFile CTAN(":/databases/CTANPackagesDatabase.db");
        CTAN.copy(CTANDatabasePath);
        QFile(CTANDatabasePath).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
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
                            +MetadataFieldNames.at(i)+"' WHERE Id = '"+MetadataList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }
        WriteBasicMetadata.exec("SELECT Id FROM DocMetadata ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            DocMetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<DocMetadataNames.count();i++){
            QString query = "UPDATE DocMetadata SET Name = '"
                            +DocMetadataNames.at(i)+"' WHERE Id = '"+DocMetadataList.at(i)+"'";
            QSqlQuery WriteMetaNames(DataTeX_Settings);
            WriteMetaNames.exec(query);
        }

        QList<QStringList> list = SqlFunctions::GetRecordList("SELECT Name,ConsoleCommand FROM BuildCommands",DataTeX_Settings);
        for(QStringList item : list){
            QProcess *process = new QProcess;
            process->start("which",QStringList()<<item.at(1));
            process->waitForBytesWritten();
            process->waitForFinished(-1);
            QString path = QString(process->readAllStandardOutput()).remove("\n");
            QSqlQuery CommandsQuery(DataTeX_Settings);
            CommandsQuery.exec(QString("UPDATE BuildCommands SET Path = '%1' WHERE Name = '%2';").arg(path,item.at(0)));
        }
        DataTeX_Settings.close();
    }

    QFile(DataTex_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    DataTeX_Settings.setDatabaseName(DataTex_Settings_Path);
    DataTeX_Settings.open();
    QFile(Bibliography_Settings_Path).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    Bibliography_Settings.setDatabaseName(Bibliography_Settings_Path);
    Bibliography_Settings.open();
    CurrentPreamble = SqlFunctions::Get_String_From_Query(SqlFunctions::GetPreamble,DataTeX_Settings);
    CurrentPreamble_Content = SqlFunctions::Get_String_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                               .arg(DataTex::CurrentPreamble)
                                                               ,DataTeX_Settings);

    QList<QStringList> list = SqlFunctions::GetRecordList("SELECT FLOOR(power(2,ROWID-1)),* FROM BuildCommands",DataTeX_Settings);
    int index = 0;
    for(QStringList item : list){
        DTXBuildCommand Command;
        Command.Id = item.at(0).toInt();
        Command.Name = item.at(1);
        Command.ConsoleCommand = item.at(2);
        Command.Path = item.at(3);
        Command.CommandArguments = QString(item.at(4)).split(",");
        Command.Extention = item.at(5);
        Command.CommandType = item.at(6);
        if(Command.CommandType == "Build"){
            CompileMenu->actions().at(index)->setData(QVariant::fromValue(Command));
        }
        else if(Command.CommandType == 'Convert'){
            ConvertMenu->actions().at(index-16)->setData(QVariant::fromValue(Command));
        }
        QSqlQuery CommandsQuery(DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE BuildCommands SET Path = '%1' WHERE Name = '%2';").arg(Command.Path,Command.Name));
        DTXBuildCommands.insert(Command.Id,Command);
//        qDebug()<<(Command.CommandType == "Build")<<CompileMenu->actions().at(index)->data().value<DTXBuildCommand>().Name;
        index++;
    }
}

void DataTex::DatabaseStructure(QString database)
{
    model = new QFileSystemModel(this);
    model->setRootPath(QFileInfo(database).absolutePath());
    QStringList filters;
    filters<<"*.tex" <<"*.pdf" <<"*.dtex";
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
    delete ui;
    delete PdfFileView;
    delete FilesProxyModel;
    delete FilesTable;
    delete DocumentsProxyModel;
    delete FileFromDocumentView;
    delete FileMenu;
    delete EditMenu;
    delete ViewMenu;
    delete ToolMenu;
    delete SettingsMenu;
    delete HelpMenu;
    delete CompileMenu;
    delete LatexFileActions;
//    delete DocumentActions;
    delete DatabaseToolBar;
    delete FileToolBar;
    delete DocTools_ToolBar;
    delete SettingsToolBar;
    delete CompileBar;
//    delete CompileBarDoc;
    delete NewDatabasefile;
    delete OpenDatabasefile;
    delete CloseDatabasefile;
    delete SyncDatabasefile;
    delete SaveAsDatabasefile;
    delete ImportDatabaseFrom;
    delete ExportDatabaseTo;
    delete EncryptDB;
    delete ConnectDatabase;
    delete DisconnectDatabase;
    delete ConnectAllDatabase;
    delete DisconnectAllDatabase;
    delete CloseApp;
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
    delete OpenPath;
    delete NewDocument;
    delete InsertFileToDocument;
    delete AddDocument;
    delete DeleteDocument;
    delete CreateSolutionsDoc;
//    delete CreateBibliography;
    delete NewPreamble;
    delete GeneralSettings;
    delete BackUpDatabase;
    delete Datatables;
    delete ConnectWithTexEditor;
    delete CompileCommands;
//    delete CompileCommandsDoc;
    delete RightClick;
//    delete DocRightClick;
//    delete BibRightClick;
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
    if (CurrentFilesDataBase.Path.isEmpty() || CurrentFilesDataBase.Path.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     tr("Error"),tr("No Files database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        NewDatabaseFile * newfile = new NewDatabaseFile(this);
        connect(newfile,SIGNAL(acceptSignal(QString)),this,
                SLOT(NewFileAddedToDatabase(QString)));
        newfile->show();
        newfile->activateWindow();
    }
}

void DataTex::NewGraphicsFile()
{
    if (CurrentFilesDataBase.Path.isEmpty() || CurrentFilesDataBase.Path.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                                                                  tr("Error"),tr("No Files database created.\nDo you wish to create a new database?"),
                                                                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        GraphicsBuilder * newgraphics = new GraphicsBuilder(this);
//        connect(newgraphics,SIGNAL(acceptSignal(QString)),this,
//                SLOT(NewFileAddedToDatabase(QString)));
        newgraphics->show();
        newgraphics->activateWindow();
    }
}

void DataTex::NewTableFile()
{
    if (CurrentFilesDataBase.Path.isEmpty() || CurrentFilesDataBase.Path.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                                                                  tr("Error"),tr("No Files database created.\nDo you wish to create a new database?"),
                                                                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        TableBuilder * newtable = new TableBuilder(this);
        //        connect(newgraphics,SIGNAL(acceptSignal(QString)),this,
        //                SLOT(NewFileAddedToDatabase(QString)));
        newtable->show();
        newtable->activateWindow();
    }
}

void DataTex::NewFileAddedToDatabase(QString filePath)
{
    // Load and show new entry in table
    ShowDataBaseFiles();
    SelectNewFileInModel(FilesTable,QFileInfo(filePath).baseName());
    ui->PreviewStackedWidget->setCurrentIndex(2);
    DBBackUp(CurrentFilesDataBase.Path,datatexpath+QFileInfo(CurrentFilesDataBase.Path).fileName());
}

void DataTex::SolutionFile()
{
    if (DataTex::CurrentFilesDataBase.Path.isEmpty() || DataTex::CurrentFilesDataBase.Path.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Files database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
        SolveDatabaseExercise * newsoldialog = new SolveDatabaseExercise(this,DatabaseFileName);
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
            DBBackUp(CurrentFilesDataBase.Path,datatexpath+QFileInfo(CurrentFilesDataBase.Path).fileName());
        });
        newsoldialog->show();
        newsoldialog->activateWindow();
    }
}

void DataTex::InsertFiles()
{
    DTXDocument * docInfo = new DTXDocument(DocumentFileName,CurrentDocumentsDataBase.Database);
    docInfo->WriteDTexFile();
    if (DataTex::CurrentDocumentsDataBase.Path.isEmpty() || DataTex::CurrentDocumentsDataBase.Path.isNull()){
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
    if (DataTex::CurrentFilesDataBase.Path.isEmpty() || DataTex::CurrentFilesDataBase.Path.isNull() ||
            DataTex::CurrentDocumentsDataBase.Path.isEmpty() || DataTex::CurrentDocumentsDataBase.Path.isNull()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                     "Error",tr("No Document database created.\nDo you wish to create a new database?"),
                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            CreateDatabase();
        }
    }
    else {
    NotesDocuments * notes = new NotesDocuments(this);
        connect(notes,&NotesDocuments::createnewdocument,this,[=](DTXDocument docInfo){
        CreateNewDocument(docInfo);
    });
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

void DataTex::CreateNewDocument(DTXDocument docInfo)
{
    QFile file(docInfo.Path);
    file.open(QIODevice::ReadWrite);
    QTextStream writeContent(&file);
    writeContent.flush();
    writeContent << docInfo.Content;
    file.close();
    ShowDocuments();
    SelectNewFileInModel(FilesTable,docInfo.Id);
    DBBackUp(CurrentDocumentsDataBase.Path,datatexpath+QFileInfo(CurrentDocumentsDataBase.Path).fileName());
    docInfo.WriteDTexFile();
}

void DataTex::DataBaseFields()
{
    if (DataTex::CurrentFilesDataBase.Path.isEmpty() || DataTex::CurrentFilesDataBase.Path.isNull() ||
            DataTex::CurrentDocumentsDataBase.Path.isEmpty() || DataTex::CurrentDocumentsDataBase.Path.isNull()){
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
    if (DataTex::CurrentFilesDataBase.Path.isEmpty() || DataTex::CurrentFilesDataBase.Path.isNull() ||
            DataTex::CurrentDocumentsDataBase.Path.isEmpty() || DataTex::CurrentDocumentsDataBase.Path.isNull()){
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
    Settings * folderpaths = new Settings(this);
    QObject::connect(folderpaths, &Settings::iconsize, this,[=](int size){
        DatabaseToolBar->setIconSize(QSize(size,size));
        FileToolBar->setIconSize(QSize(size,size));
        DocTools_ToolBar->setIconSize(QSize(size,size));
        SettingsToolBar->setIconSize(QSize(size,size));
    });
    QObject::connect(folderpaths, &Settings::selectfont, this,[=](QString selectedfont){
        QFont font(selectedfont);
        font.setStyleHint(QFont::Monospace);
        font.setPointSize(10);
        QApplication::setFont(font);});

    QObject::connect(folderpaths, &Settings::selectEditorFont, this,[=](QFont font){
//        QFont font(selectedfont);
//        font.setStyleHint(QFont::Monospace);
        font.setPointSize(font.pointSize());
        ui->BibSourceCode->setFont(font);
        ui->BibSourceCode_per_Entry->setFont(font);
        ui->DocDependenciesContent->setFont(font);
        ui->FileContent->setFont(font);
        ui->DocBibSourceCode->setFont(font);
//        ui->SourceCodeText->setFont(font);
        ui->FileEdit->editor->setFont(font);
//        ui->DocumentContent->editor->setFont(font);
//        FileEdit_2->editor->setFont(font);
    });

    folderpaths->show();
    folderpaths->activateWindow();
}

void DataTex::BackUp_DataBase_Folders()
{
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
    DTXFile *meta = new DTXFile(DatabaseFileName,CurrentFilesDataBase.Database);
    NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,NewFileMode::EditMode);
    connect(Edit,SIGNAL(acceptSignal(QString)),this,
            SLOT(NewFileAddedToDatabase(QString)));
    Edit->show();
    Edit->activateWindow();
}

void DataTex::EditDocumentDialog()
{
    DTXDocument temp;
    NotesDocuments * editDocument = new NotesDocuments(this,temp,EditMode);
    editDocument->show();
    editDocument->activateWindow();
}

void DataTex::ShowDataBaseFiles()
{
    FilesTable->filterTable("SELECT * FROM FilesDBView"/*SqlFunctions::ShowAllDatabaseFiles*/,CurrentFilesDataBase.Database,filesSorting);
    FilesTable->show();
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        FilesTable_selectionchanged(CurrentDTXDataBase.Type);
    });
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, [=](){ui->ClearFiltersFD->setEnabled(true);});
    LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    for (int i=0;i<FilesTable->model()->columnCount();i++ ) {
        FilesTable->filterHeader()->placeHolderText(i,FilesTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
    }
    StretchColumns(FilesTable,1.5);
}

void DataTex::updateFilter(QStringList values)
{
    if(ui->stackedWidget->currentIndex()==1){
        SqlFunctions::FilterTable(Database_FileTableFields,values,QStringList());
        int columns = Database_FileTableFields.count();
        FilesTable->setColumnHidden(columns,true);
        FilesTable->setColumnHidden(columns+1,true);
        FilesTable->setColumnHidden(columns+2,true);
        updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,CurrentFilesDataBase.Database,this);
        FilesTable->filterTable(SqlFunctions::FilesTable_UpdateQuery,CurrentFilesDataBase.Database,filesSorting);
        //------
        connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
            FilesTable_selectionchanged(CurrentDTXDataBase.Type);
        });
        LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
        connect(FilesTable,&ExtendedTableWidget::filesfound,this,[=](int files){
            ui->CurrentBaseLabel->setText(CurrentFilesDataBase.BaseName+" : "+QString::number(files)/*FileCount(CurrentFilesDataBase.Database,FilesTable)*/+tr(" files"));
        });
    }
    else if(ui->stackedWidget->currentIndex()==2){
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
        updateTableView(FilesTable,SqlFunctions::FilterDatabaseDocuments,CurrentDocumentsDataBase.Database,this);//
        FilesTable->filterTable(SqlFunctions::FilterDatabaseDocuments,CurrentDocumentsDataBase.Database,docsSorting);
        //------
        connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
            FilesTable_selectionchanged(CurrentDTXDataBase.Type);
        });
        LoadTableHeaders(FilesTable,Database_DocTableFieldNames);
    }
    else if(ui->stackedWidget->currentIndex()==3){
        SqlFunctions::FilterBibliographyEntries.clear();
        FilterBibliographyTable(BibliographyTableColumns);
        for(int i= 0;i<values.count();i++){
            if(!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!=""){
                SqlFunctions::FilterBibliographyEntries += " HAVING ";
                break;
            }
        }
        QStringList DataFields = QStringList();
        for (int i=0;i<BibliographyFieldIds.count();i++) {
            if(!values.at(i).isEmpty() && !values.at(i).isNull())
                DataFields.append(" (\"b\".\""+BibliographyFieldIds.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
        }
        SqlFunctions::FilterBibliographyEntries += DataFields.join(" AND ");
        SqlFunctions::FilterBibliographyEntries += " ORDER BY b.ROWID ";
        qDebug()<<SqlFunctions::FilterBibliographyEntries;
        updateTableView(FilesTable,SqlFunctions::FilterBibliographyEntries,Bibliography_Settings,this);
    }
}

void DataTex::ShowDocuments()
{
    FilesTable->filterTable(SqlFunctions::ShowDocuments,CurrentDocumentsDataBase.Database,docsSorting);
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        FilesTable_selectionchanged(CurrentDTXDataBase.Type);
    });
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
    LoadTableHeaders(FilesTable,Database_DocTableFieldNames);
    for (int i=0;i<FilesTable->model()->columnCount();i++ ) {
        FilesTable->filterHeader()->placeHolderText(i,FilesTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
    }
    StretchColumns(FilesTable,1.5);
}
void DataTex::ShowBibliography()
{
//    BibModel = new QSqlQueryModel(this);
//    QSqlQuery DatabaseQuery(Bibliography_Settings);
//    DatabaseQuery.exec(SqlFunctions::ShowBibliographyEntries);
//    BibModel->setQuery(DatabaseQuery);
////    BibliographyTable->setModel(Database);
//    BibliographyProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
//    BibliographyProxyModel->setSourceModel(BibModel);//Φιλτράρισμα και ταξινόμηση ---
//    if(bibSorting){
//        BibliographyTable->setModel(BibliographyProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    else{
//        BibliographyTable->setModel(BibModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    BibliographyTable->setSortingEnabled(bibSorting);//Φιλτράρισμα και ταξινόμηση ---
//    BibliographyTable->filterTable(SqlFunctions::ShowBibliographyEntries,Bibliography_Settings,bibSorting);
//    BibliographyTable->show();
//    int columns = BibliographyTable->model()->columnCount();
//    BibliographyTable->generateFilters(columns,false);
//    connect(BibliographyTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::BibliographyTable_selectionChanged);
//    connect(BibliographyTable->filterHeader(), &FilterTableHeader::filterValues, this, &DataTex::updateFilter);
//    connect(BibliographyTable->filterHeader(), &FilterTableHeader::filterValues, this, [=](){ui->ClearFiltersB->setEnabled(true);});
////    QStringList list = SqlFunctions::Get_StringList_From_Query("SELECT name FROM Bibliography_Fields ORDER BY ROWID",DataTex::Bibliography_Settings);
//    LoadTableHeaders(BibliographyTable,BibliographyTableColumns);
//    for (int i=0;i<BibliographyTable->model()->columnCount();i++ ) {
//        BibliographyTable->filterHeader()->placeHolderText(i,BibliographyTable->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"...");
//    }
//    StretchColumns(BibliographyTable,1.5);
}

void DataTex::FilesTable_selectionchanged(int DatabaseType)
{
    CompileMenu->setEnabled(true);
    PdfLatex->setEnabled(true);
    DeleteLatexFile->setEnabled(true);
    OpenPath->setEnabled(true);
    ui->addBibEntry->setEnabled(true);
    //--------------------------------
    int PathColumn;
    int ContentColumn;
    int BuildCommandColumn;
    switch (DatabaseType) {
    case DTXDatabaseType::FilesDB:
        PathColumn = (int)FileData::Path;
        ContentColumn = (int)FileData::FileContent;
        BuildCommandColumn = (int)FileData::BuildCommand;
        break;
    case DTXDatabaseType::DocumentsDB:
        PathColumn = (int)DocumentData::Path;
        ContentColumn = (int)DocumentData::Content;
        BuildCommandColumn = (int)DocumentData::BuildCommand;
        break;
    case DTXDatabaseType::BibliographyDB:
        PathColumn = -1;
        ContentColumn = (int)BibliographyData::Content;
//        BuildCommandColumn = (int)BibliographyData::BuildCommand;
        break;
    case DTXDatabaseType::TablesDB:
        PathColumn = (int)TableData::Path;
        ContentColumn = (int)TableData::Content;
        BuildCommandColumn = (int)TableData::BuildCommand;
        break;
    case DTXDatabaseType::FiguresDB:
        PathColumn = (int)FigureData::Path;
        ContentColumn = (int)FigureData::Content;
        BuildCommandColumn = (int)FigureData::BuildCommand;
        break;
    case DTXDatabaseType::CommandsDB:
        PathColumn = (int)CommandData::Path;
        ContentColumn = (int)CommandData::Content;
        BuildCommandColumn = (int)CommandData::BuildCommand;
        break;
    case DTXDatabaseType::PreamblesDB:
        PathColumn = (int)PreambleData::Path;
        ContentColumn = (int)PreambleData::Content;
        BuildCommandColumn = (int)PreambleData::BuildCommand;
        break;
    case DTXDatabaseType::PackagesDB:
        PathColumn = (int)PackageData::Path;
        ContentColumn = (int)PackageData::Content;
//        BuildCommandColumn = (int)PackageData::BuildCommand;
        break;
    case DTXDatabaseType::ClassesDB:
        PathColumn = (int)ClassData::Path;
        ContentColumn = (int)ClassData::Content;
//        BuildCommandColumn = (int)ClassData::BuildCommand;
        break;
    }
    int row = FilesTable->currentIndex().row();
    DatabaseFileName = FilesTable->model()->data(FilesTable->model()->index(row,0)).toString();
    DatabaseFilePath = FilesTable->model()->data(FilesTable->model()->index(row,PathColumn)).toString();
//    Date = FilesTable->model()->data(FilesTable->model()->index(row,8)).toString();
//    Solved = FilesTable->model()->data(FilesTable->model()->index(row,9)).toString();
    QString FileContent = FilesTable->model()->data(FilesTable->model()->index(row,ContentColumn)).toString();
    CurrentBuildCommand = FilesTable->model()->data(FilesTable->model()->index(row,BuildCommandColumn)).toString();
    QString FileDescription = FilesTable->model()->data(FilesTable->model()->index(row,14)).toString();
    int Solvable = FilesTable->model()->data(FilesTable->model()->index(row,9)).toInt();
//    ui->DateTimeEdit->setText(QDateTime::fromString(Date,"dd/M/yyyy hh:mm").toString("dddd d MMMM yyyy hh:mm"));
//    ui->DifficultySpinBox->setText(QString::number(Difficulty));
    int m=0;
    for(int k:CurrentFilesDataBase.basicVisibleFieldIndexes()){
        if(!FilesTable->isColumnHidden(k)){
            valuelabelList[m]->setText(FilesTable->model()->data(FilesTable->model()->index(row,k)).toString());
            m++;
        }
    }
    QSqlQuery OptionalValues(DataTex::CurrentFilesDataBase.Database);
    OptionalValues.exec(QString("SELECT %1 FROM Database_Files WHERE Id = \"%2\"").arg(CurrentFilesDataBase.customFieldIds().join(","),DatabaseFileName));
    while(OptionalValues.next()){
        QSqlRecord record = OptionalValues.record();
        for(int i=0; i < record.count(); i++)
        {
            lineList.at(i)->setText(record.value(i).toString());
        }
    }

//    ui->SolutionsLine->setText(SqlFunctions::Get_String_From_Query(QString("SELECT COUNT(*) FROM Solutions_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentFilesDataBase.Database)+tr(" solutions/proofs"));

    ui->FileDescription->setText(FileDescription);

    FileCommands::ShowPdfInViewer(DatabaseFilePath,PdfFileView);
    ui->FileEdit->editor->setText(FileContent);
    ui->FileEdit->editor->setDocumentTitle(DatabaseFilePath);
//    UndoTex->setEnabled(false);
    ui->FileEdit->toolBar->Save->setEnabled(false);
    SolveLatexFile->setEnabled(Solvable == (int)DTXSolutionState::Solvable ||
                               Solvable == (int)DTXSolutionState::Solved ||
                               Solvable == (int)DTXSolutionState::UnSolved ||
                               Solvable == (int)DTXSolutionState::HasIncompleteSolutions);
    qDebug()<<((int)DTXSolutionState::Solvable ||
                 (int)DTXSolutionState::Solved ||
                 (int)DTXSolutionState::UnSolved ||
                 (int)DTXSolutionState::HasIncompleteSolutions)<<Solvable;
    EditLatexFile->setEnabled(Solvable != (int)DTXSolutionState::Solution ||
                              Solvable != (int)DTXSolutionState::SolutionComplete ||
                              Solvable != (int)DTXSolutionState::SolutionIncomplete) ;
    FileCloneToOtherDB->setEnabled(Solvable != (int)DTXSolutionState::Solution ||
                                   Solvable != (int)DTXSolutionState::SolutionComplete ||
                                   Solvable != (int)DTXSolutionState::SolutionIncomplete);

    int modified = SqlFunctions::Get_String_From_Query(
        QString("SELECT COUNT(*) FROM Edit_History WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentDTXDataBase.Database).toInt();
    FileEditHistory->setEnabled(modified>0);

//    QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentFilesDataBase.Database);
//    ui->BibEntriesCombo->clear();
//    QSqlQuery listOfBibEntries(Bibliography_Settings);
//    listOfBibEntries.exec("SELECT * FROM EntrySourceCode");
//    while(listOfBibEntries.next()){
//        ui->BibEntriesCombo->addItem(listOfBibEntries.value(0).toString(),QVariant((listOfBibEntries.value(1).toString())));
//    }
//    ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(ui->BibEntriesCombo->currentText()));
//    QString sourceCode = QString();
//    foreach(QString id,BibEntriesInFile){
//        sourceCode += SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId = '"+id+"'",Bibliography_Settings)[0];
//    }
//    connect(ui->BibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
//        ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(text));
//        ui->stackedWidget_4->setCurrentIndex(1);
//        ui->BibSourceCode_per_Entry->setText(ui->BibEntriesCombo->currentData().toString());
//        ui->BibSourceCodeLabel->setText("Source code of entry : "+ui->BibEntriesCombo->currentText());
//    });
//    int i=-1;
//    ui->BibEntriesTable->setRowCount(0);
//    ui->stackedWidget_4->setCurrentIndex(0);
//    ui->BibSourceCode->setText(sourceCode);
//    ui->BibSourceCodeLabel->setText("Source code of file : "+DatabaseFileName);
//    foreach(QString citationKey,BibEntriesInFile){
//        QSqlQuery values(Bibliography_Settings);
//        values.exec(QString("SELECT dt.Name,title FROM Bibliography b "
//                            "JOIN DocumentTypes dt ON b.Document_Type = dt.Id "
//                            "WHERE Citation_Key = '%1'").arg(citationKey));
//        i++;
//        ui->BibEntriesTable->insertRow(i);
//        ui->BibEntriesTable->setItem(i,0 , new QTableWidgetItem(citationKey));
//        while(values.next()){
//            ui->BibEntriesTable->setItem(i,1 , new QTableWidgetItem(values.value(0).toString()));
//            ui->BibEntriesTable->setItem(i,2 , new QTableWidgetItem(values.value(1).toString()));
//        }
//    }

    FilesPreambleCombo->setEnabled(true);
    CurrentPreamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    int index = FilesPreambleCombo->findData(CurrentPreamble);
    FilesPreambleCombo->setCurrentIndex(index);
    CurrentPreamble_Content = SqlFunctions::Get_String_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                      .arg(DataTex::CurrentPreamble)
                                                                      ,DataTeX_Settings);
    getActionFromText(CompileMenu,CompileCommands);

    QSqlQuery FilesQuery(CurrentDocumentsDataBase.Database);
    QSqlQueryModel * Files = new QSqlQueryModel(this);
//    for (int i=0;i<GlobalDatabaseList.values().count();i++) {
//        if(GlobalDatabaseList.values().at(i)!=CurrentDocumentsDataBase.Path) {
//            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(GlobalDatabaseList.values().at(i),QFileInfo(GlobalDatabaseList.values().at(i)).baseName()));
//            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(files,GlobalDatabaseList[QFileInfo(DatabasesInADocument.at(i)).baseName()]
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
        FileCommands::ShowPdfInViewer(DocPath,DocDependenciesFileView);
        int lineNumber = -1;
        int start = -1;
        int end = -1;
        QTextStream parse(&DocContent);
        while(!parse.atEnd()){
            lineNumber++;
            QString line = parse.readLine();
            if(line.contains("%# File Id : "+DatabaseFileName)){
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
    QItemSelectionModel *select = FilesTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
//    CompileMenuDoc->setEnabled(true);
//    PdfLatexDoc->setEnabled(true);
    DeleteDocument->setEnabled(true);
//    OpenPathDoc->setEnabled(true);
    InsertFileToDocument->setEnabled(true);
    EditDocument->setEnabled(true);
    DocumentFileName = FilesTable->model()->data(FilesTable->model()->index(row,0)).toString();
    DocumentTitle = FilesTable->model()->data(FilesTable->model()->index(row,1)).toString();
    DocumentFilePath = FilesTable->model()->data(FilesTable->model()->index(row,6)).toString();
//    DocumentDate = DocumentsTable->model()->data(DocumentsTable->model()->index(row,7)).toString();
    QString DocumentContent = FilesTable->model()->data(FilesTable->model()->index(row,8)).toString();
    DocumentBuildCommand = FilesTable->model()->data(FilesTable->model()->index(row,10)).toString();
    int DocumentNeedsUpdate = FilesTable->model()->data(FilesTable->model()->index(row,11)).toInt();
    StuffToAddToPreamble = "\\addbibresource{"+DocumentFileName+".bib}";
    DocumentUseBibliography = FilesTable->model()->data(FilesTable->model()->index(row,13)).toInt();
    DocumentDescription = FilesTable->model()->data(FilesTable->model()->index(row,14)).toString();

    QStringList ListOfDatabases =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT DISTINCT Files_Database_Source FROM Files_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentDocumentsDataBase.Database);
    DatabasesInADocument.clear();
    DatabasesInADocument =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Path FROM Databases WHERE FileName IN (\"%1\")").arg(ListOfDatabases.join("\",\""))
            ,DataTeX_Settings);
    DatabasesInADocument.toSet().toList();

    QFile TexFile(DocumentFilePath);
    QStringList fileNamesInDocument;
    QString path_rem = "%# File Id : ";
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
    QSqlQuery FilesQuery(CurrentFilesDataBase.Database);
    for (int i=0;i<DatabasesInADocument.count();i++) {
        qDebug()<<DatabasesInADocument.at(i)<<" = "<<CurrentFilesDataBase.Path;
        QString name = (DatabasesInADocument.at(i)!=CurrentFilesDataBase.Path) ? QFileInfo(DatabasesInADocument.at(i)).baseName() : "main" ;
        datalist.append(SqlFunctions::ShowFilesInADocument.arg(files,GlobalDatabaseList.value(QFileInfo(DatabasesInADocument.at(i)).baseName()).Description
                        ,name));
        if(DatabasesInADocument.at(i)!=CurrentFilesDataBase.Path) {
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

    ui->FileEdit->editor->setText(DocumentContent);
    ui->FileEdit->editor->setDocumentTitle(DocumentFilePath);
    FileCommands::ShowPdfInViewer(DocumentFilePath,PdfFileView);
    connect(ui->TexFileTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &DataTex::TeXFilesTable_selection_changed);
    setPreamble();

    UpdateDocContent->setEnabled(DocumentNeedsUpdate == 1);
    CloneDocument->setEnabled(true);
    FilePathsInADocument.clear();
//    for (int i = 0;i<ui->TexFileTable->model()->rowCount();i++) {
//        FilePathsInADocument.insert(ui->TexFileTable->model()->data(DocumentsTable->model()->index(i,5)).toString(),
//                                    ui->TexFileTable->model()->data(DocumentsTable->model()->index(i,1)).toString());
//    }

    DocOptionalFields = Optional_DocMetadata_Ids[QFileInfo(CurrentDocumentsDataBase.Path).baseName()].join(",");
    QSqlQuery OptionalValues(DataTex::CurrentDocumentsDataBase.Database);
    OptionalValues.exec(QString("SELECT %1 FROM Database_Files WHERE Id = \"%2\"").arg(DocOptionalFields,DocumentFileName));
    QStringList s;
    while(OptionalValues.next()){
        QSqlRecord record = OptionalValues.record();
        for(int i=0; i < record.count(); i++)
        {
            s << record.value(i).toString();
//            Doc_lineList.at(i)->setText(record.value(i).toString());
        }
    }

    SolutionsPerExercise.clear();
    DatabasePerSolution.clear();
    for (int i=0;i<fileNamesInDocument.count();i++){
        QString db = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,9)).toString();

        QString File = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,0)).toString();
        QString DatabaseName = (db != "main") ? db : CurrentFilesDataBase.BaseName ;
        QString FileType = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,7)).toString();
        QSqlDatabase Database = GlobalDatabaseList.value(DatabaseName).Database;
        QStringList list = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solvable FROM FileTypes WHERE Id = '%1'").arg(FileType),Database);
        int solvable = (list.count()>0) ? list[0].toInt() : 0 ;
        QStringList Solutions_per_exer =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Path FROM Solutions_per_File WHERE File_Id = '%1'").arg(File),Database);
        if(solvable){
            SolutionsPerExercise.insert(File,Solutions_per_exer);
            DatabasePerSolution.insert(File,Database);
        }
    }

    if(SolutionsPerExercise.keys().size()>0){CreateSolutionsDoc->setEnabled(true);}
    else if(SolutionsPerExercise.keys().size()==0){CreateSolutionsDoc->setEnabled(false);}

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

    QSqlQuery IndividualEntries(CurrentDocumentsDataBase.Database);
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
//            foreach(QSqlDatabase database,GlobalDatabaseList.values()){
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
    ui->DocBibSourceCode->setText(list.join(""));

    ui->SaveDocBibContent->setEnabled(false);

    int modified = SqlFunctions::Get_String_From_Query(
        QString("SELECT COUNT(*) FROM Edit_History WHERE File_Id = '%1'").arg(DocumentFileName),CurrentDocumentsDataBase.Database).toInt();
    DocEditHistory->setEnabled(modified>0);

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
//    QItemSelectionModel *select = BibliographyTable->selectionModel();
//    int row = -1;
//    if(select->hasSelection()){
//        row = select->selectedRows().at(0).row();
//    }
//    int lastcolumn = BibliographyTable->model()->columnCount()-1;
//    CitationKey = BibliographyTable->model()->data(BibliographyTable->model()->index(row,0)).toString();
//    Bib_ValueList["Citation_Key"]->setText(CitationKey);
//    BibDocType = BibliographyTable->model()->data(BibliographyTable->model()->index(row,1)).toString();
//    Bib_ValueList["Document_Type"]->setText(BibDocType);
//    BibTitle = BibliographyTable->model()->data(BibliographyTable->model()->index(row,2)).toString();
//    Bib_ValueList["title"]->setText(BibTitle);
//    Authors = BibliographyTable->model()->data(BibliographyTable->model()->index(row,3)).toString();
//    Bib_ValueList["author"]->setText(Authors);
//    Editors = BibliographyTable->model()->data(BibliographyTable->model()->index(row,4)).toString();
//    Bib_ValueList["editor"]->setText(Editors);
//    Translators = BibliographyTable->model()->data(BibliographyTable->model()->index(row,5)).toString();
//    Bib_ValueList["translator"]->setText(Translators);
//    Publisher = BibliographyTable->model()->data(BibliographyTable->model()->index(row,6)).toString();
//    Bib_ValueList["publisher"]->setText(Publisher);
//    BibYear = BibliographyTable->model()->data(BibliographyTable->model()->index(row,7)).toInt();
//    Bib_ValueList["year"]->setText(QString::number(BibYear));
//    BibMonth = BibliographyTable->model()->data(BibliographyTable->model()->index(row,8)).toString();
//    Bib_ValueList["month"]->setText(BibMonth);
//    ISBN = BibliographyTable->model()->data(BibliographyTable->model()->index(row,9)).toString();
//    Bib_ValueList["isbn"]->setText(ISBN);
//    ISSN = BibliographyTable->model()->data(BibliographyTable->model()->index(row,10)).toString();
//    Bib_ValueList["issn"]->setText(ISSN);
//    Pages = BibliographyTable->model()->data(BibliographyTable->model()->index(row,11)).toInt();
//    Bib_ValueList["pages"]->setText(QString::number(Pages));
//    Series = BibliographyTable->model()->data(BibliographyTable->model()->index(row,12)).toString();
//    Bib_ValueList["series"]->setText(Series);
//    Edition = BibliographyTable->model()->data(BibliographyTable->model()->index(row,13)).toString();
//    Bib_ValueList["edition"]->setText(Edition);
//    BibChapter = BibliographyTable->model()->data(BibliographyTable->model()->index(row,14)).toString();
//    Bib_ValueList["chapter"]->setText(BibChapter);
//    BibNumber = BibliographyTable->model()->data(BibliographyTable->model()->index(row,15)).toString();
//    Bib_ValueList["number"]->setText(BibNumber);
//    Volume = BibliographyTable->model()->data(BibliographyTable->model()->index(row,16)).toString();
//    Bib_ValueList["volume"]->setText(Volume);
//    Journal = BibliographyTable->model()->data(BibliographyTable->model()->index(row,17)).toString();
//    Bib_ValueList["journal"]->setText(Journal);
//    Institution = BibliographyTable->model()->data(BibliographyTable->model()->index(row,18)).toString();
//    Bib_ValueList["institution"]->setText(Institution);
//    School = BibliographyTable->model()->data(BibliographyTable->model()->index(row,19)).toString();
//    Bib_ValueList["school"]->setText(School);
//    Issue = BibliographyTable->model()->data(BibliographyTable->model()->index(row,20)).toString();
//    Bib_ValueList["issue"]->setText(Issue);
//    Address = BibliographyTable->model()->data(BibliographyTable->model()->index(row,21)).toString();
//    Bib_ValueList["address"]->setText(Address);
//    DOI = BibliographyTable->model()->data(BibliographyTable->model()->index(row,22)).toString();
//    Bib_ValueList["doi"]->setText(DOI);
//    URL = BibliographyTable->model()->data(BibliographyTable->model()->index(row,23)).toString();
//    Bib_ValueList["url"]->setText(URL);
//    Language = BibliographyTable->model()->data(BibliographyTable->model()->index(row,24)).toString();
//    Bib_ValueList["language"]->setText(Language);
//    Location = BibliographyTable->model()->data(BibliographyTable->model()->index(row,25)).toString();
//    Bib_ValueList["location"]->setText(Location);
//    Subtitle = BibliographyTable->model()->data(BibliographyTable->model()->index(row,26)).toString();
//    Bib_ValueList["subtitle"]->setText(Subtitle);
//    Organization = BibliographyTable->model()->data(BibliographyTable->model()->index(row,27)).toString();
//    Bib_ValueList["organization"]->setText(Organization);
//    BibKey = BibliographyTable->model()->data(BibliographyTable->model()->index(row,28)).toString();
//    Bib_ValueList["key"]->setText(BibKey);
//    Abstract = BibliographyTable->model()->data(BibliographyTable->model()->index(row,29)).toString();
//    Bib_ValueList["abstract"]->setText(Abstract);
//    CrossReference = BibliographyTable->model()->data(BibliographyTable->model()->index(row,30)).toString();
//    Bib_ValueList["crossref"]->setText(CrossReference);
//    BibNote = BibliographyTable->model()->data(BibliographyTable->model()->index(row,31)).toString();
//    Bib_ValueList["note"]->setText(BibNote);

//    int k=-1;
//    foreach(QString text,Bib_ValueList_Opt.keys()){
//        k++;
//        Bib_ValueList_Opt[text]->setText(BibliographyTable->model()->
//                data(BibliographyTable->model()->index(row,Bib_ValueList.count()-1)).toString());
//    }
//    QString sourceCode = SqlFunctions::Get_StringList_From_Query("SELECT SourceCode FROM EntrySourceCode WHERE BibId = '"+CitationKey+"'",Bibliography_Settings)[0];
//    ui->SourceCodeText->setText(sourceCode);
//    EditBibEntry->setEnabled(true);
//    DeleteBibEntry->setEnabled(true);

//    QSqlQueryModel * Files = new QSqlQueryModel(this);
//    QSqlQuery Query(CurrentFilesDataBase.Database);
//    QStringList datalist;
//    QString text = QString("SELECT File_Id,\"%2\" AS \"Database source\", df.Path "
//                           "FROM Bib_Entries_per_File bpf "
//                           "JOIN Database_Files df ON df.Id = bpf.File_Id WHERE Bib_Id = '%1' ")
//            .arg(CitationKey,CurrentFilesDataBase.BaseName);
//    datalist.append(text);
//    int i=-1;
//    for (DTXDatabase DTXDB : GlobalDatabaseList) {
//        i++;
//        QString DataPath = DTXDatabase(GlobalDatabaseList.values()[i]).Path;
//        if(DataPath!=CurrentFilesDataBase.Path) {
//            Query.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DTXDB.Path,DTXDB.BaseName));
//            datalist.append(QString("SELECT File_Id,\"%3\" AS \"Database source\", df.Path "
//                                    "FROM %2.Bib_Entries_per_File bpf "
//                                    "JOIN %2.Database_Files df ON df.Id = bpf.File_Id WHERE Bib_Id = '%1'")
//                            .arg(CitationKey,DTXDB.BaseName,DTXDB.Description));
//        }
//    }
//    text = datalist.join(" UNION ");
//    Query.exec(text+" ORDER BY 1 ");
//    Files->setQuery(Query);
//    qDebug()<<text;
//    ui->DatabaseFiles_per_BibEntry->setModel(Files);
//    ui->DatabaseFiles_per_BibEntry->show();
//    QString label = "Database files that contain entry "+CitationKey+" : "+QString::number(ui->DatabaseFiles_per_BibEntry->model()->rowCount())+" files";
//    ui->CitationsLabel->setText(label);
//    StretchColumnsToWidth(ui->DatabaseFiles_per_BibEntry);
//    connect(ui->DatabaseFiles_per_BibEntry->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
//        QItemSelectionModel *select2 = ui->DatabaseFiles_per_BibEntry->selectionModel();
//        int index = -1;
//        if(select2->hasSelection()){
//            index = select2->selectedRows().at(0).row();
//        }
//        FileCommands::ShowPdfInViewer(ui->DatabaseFiles_per_BibEntry->model()->data(ui->DatabaseFiles_per_BibEntry->model()->index(index,2)).toString(),BibFileView);
//    });
}


//--
//void FileCommands::ShowPdfInViewer(QString exoFile, PdfViewerWidget *view)
//{
//    if (exoFile.isEmpty()) {
//        return;}
//    QString pdfFile = /*"file:///"+*/exoFile.replace(".tex",".pdf");
//    if(QFileInfo::exists(pdfFile)){
//        view->openFile(pdfFile,"","");
//    }
//    else{
//        view->openFile(DataTex::GlobalSaveLocation+"No_Pdf.pdf","","");
//    }
//}

//void FileCommands::ShowPdfInViewer(QString exoFile, PdfViewer *view)
//{
    //    if (exoFile.isEmpty()) {
    //        return;}
    //    QString pdfFile = /*"file:///"+*/exoFile.replace(".tex",".pdf");
    //    if(QFileInfo::exists(pdfFile)){
    //        view->setCurrentDocument(pdfFile);
    //    }
    //    else{
    //        view->setCurrentDocument(DataTex::GlobalSaveLocation+"No_Pdf.pdf");
    //    }
//}

void DataTex::CompileToPdf()
{
    QAction * action = qobject_cast<QAction*>(sender());
    ui->FileEdit->toolBar->Save->trigger();
    CurrentPreamble = FilesPreambleCombo->currentData().toString();
    setPreamble();
    FileCommands::CreateTexFile(DatabaseFilePath,DocumentUseBibliography,StuffToAddToPreamble);
    FileCommands::BuildDocument(action->data().value<DTXBuildCommand>(),DatabaseFilePath);
    qDebug()<<action->data().value<DTXBuildCommand>().Path<<QStringList()<<action->data().value<DTXBuildCommand>().CommandArguments;
}

void DataTex::Compile()
{
    QAction * action = qobject_cast<QAction*>(sender());
    FileCommands::BuildDocument(action->data().value<DTXBuildCommand>(),DatabaseFilePath);
}

void DataTex::CompileAsymptote()
{
    QStringList Files;
    QString filename = QFileInfo(DatabaseFilePath).baseName()+"*.asy";
    QDirIterator list(QFileInfo(DatabaseFilePath).absolutePath(),
                      QStringList() << filename, QDir::Files, QDirIterator::Subdirectories);
    while (list.hasNext()){
    Files.append(list.next());}

    QProcess compileProcess;
    compileProcess.setWorkingDirectory(QFileInfo(DatabaseFilePath).absolutePath());
    compileProcess.start(DataTex::Asymptote_Command,Files);
    compileProcess.waitForFinished(-1);
}

void DataTex::CreateDatabase()
{
    DatabaseCreator * NewDatabase = new DatabaseCreator(this);
    connect(NewDatabase,SIGNAL(newbase(DTXDatabase)),this,SLOT(CreateNewDatabase(DTXDatabase)));
    NewDatabase->show();
    NewDatabase->activateWindow();
}

void DataTex::CreateNewDatabase(DTXDatabase DTXDB)
{
    QString FullPath = DTXDB.Path+QDir::separator()+DTXDB.Description+QDir::separator()+DTXDB.BaseName+".db";
    DatabaseStructure(DTXDB.Path);
    AddDatabaseToTree(DTXDB);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",DTXDB.BaseName);
    database.setDatabaseName(FullPath);
    database.setConnectOptions("QSQLITE_ENABLE_REGEXP");
    DTXDB.Database = database;
    GlobalDatabaseList.insert(DTXDB.BaseName,DTXDB);
    ui->SideBarButtonGroup->button(DTXDB.Type+1);
    ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->child(ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->childCount()-1));
    on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->child(ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->childCount()-1),0);
}

void DataTex::UpdateCurrentDatabase(DTXDatabase DTXDB)
{
    QSqlQuery SaveData(DataTeX_Settings);
    QString baseName = DTXDB.BaseName;
    switch (DTXDB.Type) {
    case DTXDatabaseType::FilesDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){
            CurrentFilesDataBase = DTXDB;
            Database_FileTableFields = CurrentFilesDataBase.getIdsList();
            Database_FileTableFieldNames = CurrentFilesDataBase.getNamesList();
//            ShowMetadataInfo();
            qDebug()<<Database_FileTableFields;

//            Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentFilesDataBase.Database);
            //        FilterTables_Queries(Database_FileTableFields);
            SqlFunctions::ShowAllFiles(Database_FileTableFields);
        }
        else{
            encrFileDB_Dialog({DTXDB});
        }
        connect(FilesTable,&ExtendedTableWidget::filesfound,this,[=](int files){
            qDebug()<<"filecount=0"<<files;
            ui->CurrentBaseLabel->setText(GlobalDatabaseList.value(baseName).Description+" : "+QString::number(files)/*FileCount(CurrentFilesDataBase.Database,FilesTable)*/+tr(" files"));
        });
        break;
    case DTXDatabaseType::DocumentsDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_DocumentsDB'").arg(baseName));
        if(!DTXDB.Encrypt){
            CurrentDocumentsDataBase = DTXDB;
            Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",CurrentDocumentsDataBase.Database);
            Database_DocTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp",CurrentDocumentsDataBase.Database);
            ui->CurrentBaseLabel->setText("Current ducument database : "+GlobalDatabaseList.value(QFileInfo(CurrentDocumentsDataBase.Path).baseName()).Description);
            FilterDocuments(Database_DocumentTableColumns);
        }
        else{
            encrFileDB_Dialog({DTXDB});
        }
        break;
    case DTXDatabaseType::BibliographyDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::TablesDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::FiguresDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::CommandsDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::PreamblesDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::PackagesDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    case DTXDatabaseType::ClassesDB:
        SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
        if(!DTXDB.Encrypt){

        }
        else{

        }
        break;
    }
    CurrentDTXDataBase = DTXDB;
}

void DataTex::AddDatabaseToTree(DTXDatabase DTXDB)
{
    QTreeWidgetItem * item = new QTreeWidgetItem();
    item->setText(0,DTXDB.Description);
    item->setText(1,DTXDB.Path);
    item->setText(2,DTXDB.BaseName);
    ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->addChild(item);
    if(DTXDB.IsConnected){
        item->setIcon(0,QIcon(":/images/Dark_Theme/ConnectDB.svg"));
    }
    else if(DTXDB.Encrypt){
        item->setIcon(0,QIcon(":/images/Dark_Theme/Locked.svg"));
    }
    else if(!DTXDB.IsConnected && !DTXDB.Encrypt){
        item->setIcon(0,QIcon(":/images/Dark_Theme/DisconnectDB.svg"));
    }
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
        QSqlQuery deleteQuery(CurrentFilesDataBase.Database);
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM Database_Files WHERE Id = \"%1\"").arg(DatabaseFileName));
        ShowDataBaseFiles();
        DBBackUp(CurrentFilesDataBase.Path,datatexpath+QFileInfo(CurrentFilesDataBase.Path).fileName());
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
        QSqlQuery deleteQuery(CurrentDocumentsDataBase.Database);
        deleteQuery.exec(QString("DELETE FROM Documents WHERE Id = \"%1\"").arg(DocumentFileName));
        ShowDocuments();
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(DocumentFilePath).absolutePath()));}
    }
}

void DataTex::on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()){
     CloseDatabasefile->setEnabled(true);
     QString Database = item->text(2);
     DTXDatabase DTXDB = GlobalDatabaseList.value(Database);
     ConnectDatabase->setEnabled(!DTXDB.IsConnected);
     DisconnectDatabase->setEnabled(DTXDB.IsConnected);
        if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==0){
            ui->FilesDatabaseToggle->setChecked(true);
            UpdateCurrentDatabase(GlobalDatabaseList.value(item->text(2)));
            DatabaseStructure(CurrentFilesDataBase.Path);
            ShowDataBaseFiles();
            int columns = FilesTable->model()->columnCount();
            for (int i=0;i<columns;i++) {
                FilesTable->setColumnHidden(i,false);
            }
            FilesTable->setColumnHidden(columns-1,true);
            FilesTable->setColumnHidden(columns-2,true);
            FilesTable->setColumnHidden(columns-3,true);
            loadDatabaseFields();
            ui->FilesTagFilter->setChecked(false);
            delete filesTagLine;
            CreateCustomTagWidget();
            on_ComboCount_currentIndexChanged(0);
            FilesTable->filterHeader()->adjustPositions();
            runQuery_Root("UPDATE Initial_Settings SET Value = '0' WHERE Setting = 'SaveNewFileSelections'",DataTeX_Settings);
        }
        else if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==1){
            ui->DocumentsDatabaseToggle->setChecked(true);
            UpdateCurrentDatabase(GlobalDatabaseList.value(item->text(2)));
            DatabaseStructure(CurrentDocumentsDataBase.Path);
            ShowDocuments();
            loadDatabaseFields();
            LoadTableHeaders(FilesTable,Database_DocTableFieldNames);
//            ui->DocumentsTagFilter->setChecked(false);
            delete docsTagLine;
            CreateCustomTagWidget();
            on_ComboCount_currentIndexChanged(1);
            FilesTable->filterHeader()->adjustPositions();
        }
//        QWidget * w = ui->tabWidget->tabBar()->tabButton(0,QTabBar::RightSide);
//        int count = Optional_Metadata_Ids[CurrentFilesDataBase.BaseName].count();
//        if(count>0 && !w){
//            MetadataToolButton();
//        }
//        else if(count==0 && w){
//            ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->deleteLater();
//            ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, 0);
//        }
//        else if((count>0 && w)||(count==0 && !w)){
//            return;
//        }
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
    FileCommands::ShowPdfInViewer(FilePath,FileFromDocumentView);
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
    SqlFunctions::ShowAllDatabaseFiles = "SELECT df.Id ,ft.Name "
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
            "JOIN Sections s ON s.Id = spf.Section_Id  "
            "LEFT JOIN Sections_Exercises se ON se.Section_Id = s.Id "
            "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
            "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
            "LEFT JOIN Tags_per_File t ON t.File_Id = df.Id ";
    SqlFunctions::FilesTable_UpdateQuery = SqlFunctions::ShowAllDatabaseFiles;
    SqlFunctions::ShowAllDatabaseFiles += "GROUP BY df.Id "
                                          "ORDER BY df.rowid;";

//    qDebug()<<SqlFunctions::FilesTable_UpdateQuery;
    qDebug()<<SqlFunctions::ShowAllDatabaseFiles;
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
    QDir dir(CurrentFilesDataBase.Path);
    for(result=0;dir.cdUp();++result){}
    bool isFile = !model->isDir(index);
    QString fileName = QFileInfo(index.data(Qt::DisplayRole).toString()).baseName();
    if(isFile){
        SelectNewFileInModel(FilesTable,fileName);
    }
}

void DataTex::on_ComboCount_currentIndexChanged(int index)
{
    QSqlDatabase database;
    int i = ui->stackedWidget->currentIndex();

    switch (i) {
    case 0:
        database = CurrentFilesDataBase.Database;
        break;
    case 1:
        database = CurrentDocumentsDataBase.Database;
        break;
    case 2:
        database = Bibliography_Settings;
        break;
    }
    updateTableView(ui->CountFilesTable,ui->ComboCount->currentData().toString(),database,this);
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
    PreambleSettings * newPreamle = new PreambleSettings(this);
    connect(newPreamle,SIGNAL(newpreamblesignal(QStringList)),this,SLOT(AddPreamble(QStringList)));
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
    qDebug()<<"4) db opened";
    QSqlQuery SaveData(DataTeX_Settings);
    QStringList Tables = SqlFunctions::Get_StringList_From_Query("SELECT name FROM main.sqlite_master WHERE type='table';",newDatabase);
    newDatabase.close();
    DTXDatabase DTXDB;
    //DTXDB Εισαγωγή τιμών μεταβλητών
    if(Tables.count()>0){
        if(Tables.contains("Database_Files")){
            AddDatabaseToTree(DTXDB);
            newDatabase.setDatabaseName(filePath);
            DataTex::CurrentFilesDataBase.Database = newDatabase;
            DataTex::CurrentFilesDataBase.Path = filePath;
            GlobalDatabaseList.insert(QFileInfo(filePath).baseName(),DTXDB);
            DataTex::CurrentFilesDataBase.Database.open();
            qDebug()<<"5) db opened";
            SaveData.exec(QString("INSERT INTO Databases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_FilesDB'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp",CurrentFilesDataBase.Database);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp",CurrentFilesDataBase.Database);

            QSqlQuery add(DataTeX_Settings);
            for (int i=0;i<MetadataIds.count();i++) {
                add.exec(QString("INSERT OR IGNORE INTO Metadata (Id,Name,Basic) VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
                add.exec("INSERT OR IGNORE INTO Metadata_per_Database (Database_FileName,Metadata_Id) VALUES (\""+baseName+"\",\""+MetadataIds.at(i)+"\")");
            }
            ui->OpenDatabasesTreeWidget->setCurrentItem(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1));
            on_OpenDatabasesTreeWidget_itemClicked(ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(ui->OpenDatabasesTreeWidget->topLevelItem(0)->childCount()-1),0);
        }
        else if(Tables.contains("Documents")){
            AddDatabaseToTree(DTXDB);
            newDatabase.setDatabaseName(filePath);
            DataTex::CurrentDocumentsDataBase.Database = newDatabase;
            DataTex::CurrentDocumentsDataBase.Path = filePath;
            GlobalDatabaseList.insert(QFileInfo(filePath).baseName(),DTXDB);
            DataTex::CurrentDocumentsDataBase.Database.open();
            SaveData.exec(QString("INSERT INTO DataBases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                               .arg(baseName,FolderName,filePath));
            SaveData.exec(QString("UPDATE Current_Databases SET Value = \"%1\" WHERE Setting = 'Current_DocumentsDB'").arg(baseName));
            QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp",CurrentDocumentsDataBase.Database);
            QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp",CurrentDocumentsDataBase.Database);
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
        DatabaseName = CurrentFilesDataBase.BaseName;
        DatabasePath = CurrentFilesDataBase.Path;
        DatabaseType = "Databases";
        DatabaseTable = "Metadata_per_Database";
    }
    else{
        DatabaseName = QFileInfo(CurrentDocumentsDataBase.Path).baseName();
        DatabasePath = CurrentDocumentsDataBase.Path;
        DatabaseType = "DataBases";
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
    DTXDocument temp;
    NotesDocuments * clone = new NotesDocuments(this,temp,CloneModeContentAndMetadata);// Add option for ContentOnly
//    connect(clone,SIGNAL(createnewdocument(DTXDocument)),this,SLOT(CreateNewDocument(DTXDocument)));
    connect(clone,&NotesDocuments::clonedocument,this,[&](DTXDocument document){
        QSqlQuery write(GlobalDatabaseList.value(document.Database.Id).Database);
        write.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(CurrentDocumentsDataBase.Path,QFileInfo(CurrentDocumentsDataBase.Path).baseName()));
        write.exec(QString("INSERT INTO Files_per_Document (Document_Id,File_Id,Files_Database_Source) SELECT \"%1\",File_Id,Files_Database_Source "
                           "FROM \"%3\".Files_per_Document WHERE Document_Id = '%2'")
                       .arg(document.Id,DocumentFileName,QFileInfo(CurrentDocumentsDataBase.Path).baseName()));
        write.exec(QString("INSERT INTO BibEntries_per_Document (Bib_Id,Document_Id) SELECT Bib_Id,\"%1\" "
                           "FROM \"%3\".BibEntries_per_Document WHERE Document_Id = '%2' ")
                       .arg(document.Id,DocumentFileName,QFileInfo(CurrentDocumentsDataBase.Path).baseName()));
        write.exec(QString("DETACH DATABASE \"%1\"").arg(QFileInfo(CurrentDocumentsDataBase.Path).baseName()));
//        CreateNewDocument(DTXDocument docInfo);
        QList<QTreeWidgetItem*> clist = ui->OpenDatabasesTreeWidget->findItems(GlobalDatabaseList.value(document.Database.Id).Description, Qt::MatchContains|Qt::MatchRecursive, 0);
        ui->OpenDatabasesTreeWidget->setCurrentItem(clist[0]);
        on_OpenDatabasesTreeWidget_itemClicked(clist[0],0);
        qDebug()<<clist[0]->text(0);
    });
//    connect(clone,&NotesDocuments::insertfiles,this,[=](){InsertFiles();});
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
    QString database = FileCommands::GetSingleFileIdFromFile(filePath).value(QFileInfo(filePath).baseName());
    QFile File(filePath);
    QTextStream in(&File);
    File.open (QIODevice::ReadOnly);
    QString text = in.readAll();
    QString preamble = FileCommands::GetPreamble(text);
    QString content = FileCommands::ClearMetadataFromContent(text).remove(preamble).trimmed();
    qDebug()<<content;
    File.close();
    if(database.isEmpty() || database != CurrentFilesDataBase.BaseName){
        QString message;
        if(database.isEmpty()){
            message = tr("This file doens't have a database source.\n"
               "Do you wish to open it anyway?");
        }
        else if(database != CurrentFilesDataBase.BaseName){
            message = tr("This file was created from another database\n"
                         "and some metadada values may differ.\n"
                         "Do you wish to add this file to the current database anyway?");
        }
        QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Error"),message,
                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        }
        else{
            DTXFile *meta = new DTXFile;
            meta->Id = QFileInfo(filePath).baseName();
            meta->Content = content;
            NewDatabaseFile * Edit = new NewDatabaseFile(this,meta,NewFileMode::ImportMode/*false,false,true*/);
            connect(Edit,&NewDatabaseFile::acceptSignal,this,[=](QString fileName){
                NewFileAddedToDatabase(fileName);
                delete meta;
            });
            Edit->show();
            Edit->activateWindow();
            return;
        }
        if(!preamble.isEmpty()){

        }
    }
    qDebug()<<CsvFunctions::ReadCsv(filePath);
//    else if(database != CurrentFilesDataBase.BaseName){
//        QMessageBox::StandardButton resBtn =
//        QMessageBox::warning( this,tr("Error"),tr("This file was created from another database\n"
//                                                  "and some metadada values may differ.\n"
//                                                  "Do you wish to add this file to the current database anyway?"),
//                              QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
//        if (resBtn == QMessageBox::No) {
//            return;
//        }
//        else{
//            DTXFile *meta = new DTXFile;
//            meta->Id = QFileInfo(filePath).baseName();
//            meta->Content = content;
//            NewDatabaseFile * Edit = new NewDatabaseFile(this,meta/*,content*/);
//            connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
//                    SLOT(NewFileAddedToDatabase(QString,QString)));
//            Edit->show();
//            Edit->activateWindow();
//            return;
//        }
//    }

    QString csvFile = filePath;
    csvFile.replace(".tex",".csv");

    if(QFileInfo::exists(csvFile)){
//        DTXFile *MetadataValues = new DTXFile;
//        NewDatabaseFile * Edit = new NewDatabaseFile(this,MetadataValues,true,false,true);
//        connect(Edit,SIGNAL(acceptSignal(QString,QString)),this,
//                SLOT(NewFileAddedToDatabase(QString,QString)));
//        Edit->show();
//        Edit->activateWindow();
    }
}

void DataTex::AddDocToDatabase()
{
    QString docPath = QFileDialog::getOpenFileName(this,
                      tr("Open existing file"), QDir::homePath(), tr("Latex file (*.tex)"));
    if(docPath.isEmpty()) {
        return;
    }

    QFile file(docPath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString Content = stream.readAll();
    QString preamble = FileCommands::GetPreamble(Content);
    Content.remove(preamble);
    Content = FileCommands::ClearDocumentContent(Content);
    qDebug()<<Content;
    DTXDocument temp;
    NotesDocuments * openDoc = new NotesDocuments(this,temp,ImportMode);
    connect(openDoc,&NotesDocuments::createnewdocument,this,[=](DTXDocument docInfo){
        CreateNewDocument(docInfo);
        QMessageBox::StandardButton resBtn =
            QMessageBox::warning( this,tr("Add preamble"),tr("Do you want to add the document's preamble\nto preamble list?"),
                                 QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        }
        else{
            PreambleSettings * addPreambleToDatabase = new PreambleSettings(this,preamble);
            connect(addPreambleToDatabase,SIGNAL(newpreamblesignal(QStringList)),this,SLOT(AddPreamble(QStringList)));
            addPreambleToDatabase->show();
            addPreambleToDatabase->activateWindow();
        }
    });
    openDoc->show();
    openDoc->activateWindow();
}

QString DataTex::BibSourceCode()
{
//    ui->SourceCodeText->clear();
    QString sourceCode = Bib_ValueList["Citation_Key"]->text()+"{"+
            Bib_ValueList["Document_Type"]->text()+",\n";

    for(int i = 2;i<Bib_ValueList.count();i++){
        QString text = Bib_ValueList[Bib_ValueList.keys().at(i)]->text();
        if(!text.isEmpty() && !text.isNull() && Bib_ValueList[Bib_ValueList.keys().at(i)]->hasAcceptableInput()){
                sourceCode += "\t "+Bib_ValueList.keys().at(i)+" = {"+text+"}"+",\n";}
    }

    sourceCode += "}";
//    ui->SourceCodeText->setText(sourceCode);
    return sourceCode;
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
    QSqlQuery BibliographyQuery(CurrentFilesDataBase.Database);
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
    QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentFilesDataBase.Database);
    connect(ui->BibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addBibEntry->setEnabled(!BibEntriesInFile.contains(text));
    });
    ui->addBibEntry->setEnabled(false);
}

void DataTex::StretchColumns(QTableView * Table, float stretchFactor)
{
    QFont myFont = Table->horizontalHeader()->font();
    QFontMetrics fm(myFont);
    for (int c = 0; c < Table->horizontalHeader()->count(); ++c)
    {
        QString str = Table->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Table->setColumnWidth(c,width);
    }
    Table->horizontalHeader()->setMinimumSectionSize(100);
}

void DataTex::StretchColumns(QTreeView * Tree, float stretchFactor)
{
    QFont myFont = Tree->header()->font();
    QFontMetrics fm(myFont);
    for (int c = 0; c < Tree->model()->columnCount(); ++c)
    {
        QString str = Tree->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Tree->setColumnWidth(c,width);
    }
    Tree->header()->setMinimumSectionSize(100);
}

void DataTex::FileClone()
{
    CloneDatabaseFile * cloneFile = new CloneDatabaseFile(this);
    connect(cloneFile,&CloneDatabaseFile::acceptSignal,this,[=](QString FilePath){
        NewFileAddedToDatabase(FilePath);
    });
    cloneFile->show();
    cloneFile->activateWindow();
}

bool DataTex::SelectNewFileInModel(QTableView * table,QString newFile)
{
    QAbstractItemModel * m = table->model();
    QModelIndex ix = table->currentIndex();
    while (table->model()->canFetchMore(ix))
           table->model()->fetchMore(ix);
    QModelIndexList matchList = m->match(m->index(0,0), Qt::DisplayRole,
                               newFile, -1,  Qt::MatchFlags(Qt::MatchContains|Qt::MatchWrap));

    bool fileFound = matchList.count()>=1;
    if(matchList.count()>=1){
        table->setCurrentIndex(matchList.first());
        table->scrollTo(matchList.first());
    }
    return fileFound;
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
        SelectNewFileInModel(FilesTable,id);
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
        QSqlQuery deleteCitation(CurrentFilesDataBase.Database);
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

void DataTex::on_NewBibEntry_CurrentFile_clicked()
{
    BibEntry * newEntryforFile = new BibEntry(this,false,false,QHash<QString,QString>());
    connect(newEntryforFile,&BibEntry::accepted,this,[=](){
        QSqlQuery BibliographyQuery(CurrentFilesDataBase.Database);
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
        QStringList BibEntriesInFile = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM Bib_Entries_per_File WHERE File_Id = '%1'").arg(DatabaseFileName),CurrentFilesDataBase.Database);
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
    QSqlQuery BibliographyQuery(CurrentDocumentsDataBase.Database);
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
//    foreach(QSqlDatabase database,GlobalDatabaseList.values()){
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

    QStringList BibEntriesInDocument = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM BibEntries_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentDocumentsDataBase.Database);
    connect(ui->DocBibEntriesCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->addDocBibEntry->setEnabled(!BibEntriesInDocument.contains(text));
    });
    ui->addDocBibEntry->setEnabled(false);
}

void DataTex::on_NewBibEntry_CurrentDocument_clicked()
{
    BibEntry * newEntryforDocument = new BibEntry(this,false,false,QHash<QString,QString>());
    connect(newEntryforDocument,&BibEntry::accepted,this,[=](){
        QSqlQuery BibliographyQuery(CurrentDocumentsDataBase.Database);
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
        QStringList BibEntriesInDocument = SqlFunctions::Get_StringList_From_Query(QString("SELECT Bib_Id FROM BibEntries_per_Document WHERE Document_Id = '%1'").arg(DocumentFileName),CurrentDocumentsDataBase.Database);
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
    filesTagLine = new TagsFilterWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",DataTex::CurrentFilesDataBase.Database));
    filesTagLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ui->verticalLayout_20->addWidget(filesTagLine);
    connect(filesTagLine,&TagsFilterWidget::SelectedTags,this,[=](QStringList list){
        FilesTable->setFilter(FilesTable->model()->columnCount()-1,list.join(","));
        qDebug()<<list;
    });
    connect(ui->FilesTagFilter, &QPushButton::toggled, this, [=](bool checked){
        ui->splitter_3->setSizes(QList<int>({(1-0.2*checked)*height(),0.2*checked*height()}));
        if(checked){ui->stackedWidget_8->setCurrentIndex(1);}
    });
    ui->FilesTagFilter->setChecked(false);

    docsTagLine = new TagsFilterWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",DataTex::CurrentDocumentsDataBase.Database));
    docsTagLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    docsTagLine->setVisible(false);
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
    if(e->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        QMainWindow::changeEvent(e);
        QStringList MetadataList;
        QStringList DocMetadataList;
        QSqlQuery WriteBasicMetadata(DataTeX_Settings);
        WriteBasicMetadata.exec("SELECT Id FROM Metadata WHERE Basic = '1' ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            MetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<MetadataList.count();i++){
            QString query = "UPDATE Metadata SET Name = '"
                            +tr(MetadataFieldNames[i].toLocal8Bit().data())+"' WHERE Id = '"+MetadataList.at(i)+"'";
            WriteBasicMetadata.exec(query);
        }
        WriteBasicMetadata.exec("SELECT Id FROM DocMetadata WHERE Basic = '1' ORDER BY rowid");
        while(WriteBasicMetadata.next()){
            DocMetadataList.append(WriteBasicMetadata.value(0).toString());
        }
        for(int i=0;i<DocMetadataNames.count();i++){
            QString query = "UPDATE DocMetadata SET Name = '"
                            +tr(DocMetadataNames.at(i).toLocal8Bit().data())+"' WHERE Id = '"+DocMetadataList.at(i)+"'";
            WriteBasicMetadata.exec(query);
        }
        writeSettings();
    }
}

void DataTex::closeEvent(QCloseEvent *event)
{
//    if (userReallyWantsToQuit()) {
    writeSettings();
        event->accept();
//    } else {
//        event->ignore();
//    }
}

void DataTex::readSettings()
{
    QSettings settings;
//    settings.beginGroup("LaTeX_Settings");
//    PdfLatex_Command = settings.value("Pdflatex_Command").toString();
//    XeLatex_Command = settings.value("Xelatex_Command").toString();
//    Latex_Command = settings.value("Latex_Command").toString();
//    LuaLatex_Command = settings.value("Lualatex_Command").toString();
//    Bibtex_Command = settings.value("Bibtex_Command").toString();
//    Pythontex_Command = settings.value("Pythontex_Command").toString();
//    Asymptote_Command = settings.value("Asymptote_Command").toString();
//    CurrentPreamble = settings.value("Current_Preamble").toString();
//    settings.endGroup();
    settings.beginGroup("Application_Settings");
    currentlanguage = settings.value("Language").toString();
    filesSorting = settings.value("SortFiles").toBool();
    docsSorting = settings.value("SortDocuments").toBool();
    bibSorting = settings.value("SortBibliography").toBool();
    settings.endGroup();
}

void DataTex::writeSettings()
{
    QSettings settings;
//    settings.beginGroup("LaTeX_Settings");
//    settings.setValue("Pdflatex_Command", PdfLatex_Command);
//    settings.setValue("Xelatex_Command", XeLatex_Command);
//    settings.setValue("Latex_Command", Latex_Command);
//    settings.setValue("Lualatex_Command", LuaLatex_Command);
//    settings.setValue("Bibtex_Command", Bibtex_Command);
//    settings.setValue("Pythontex_Command", Pythontex_Command);
//    settings.setValue("Asymptote_Command", Asymptote_Command);
//    settings.setValue("Current_Preamble", CurrentPreamble);
//    settings.endGroup();
    settings.beginGroup("Application_Settings");
    settings.setValue("SortFiles", filesSorting);
    settings.setValue("SortDocuments", docsSorting);
    settings.setValue("SortBibliography", bibSorting);
    settings.setValue("datatexpath", datatexpath);
    settings.endGroup();
}

void DataTex::DBBackUp(QString database, QString dest_path)
{
    QFile file(database);
//    QProcess::execute("chmod",{"777",datatexpath});
    if (QFile::exists(dest_path))
    {
        QFile::remove(dest_path);
    }
    file.copy(dest_path);
//    QProcess::execute("chmod",{"555",datatexpath});
}

void DataTex::RestoreDB(int dbtype,QSqlDatabase database)
{
    QString ContentType;
    QString DataTable;
    if(dbtype==0){
        ContentType = "FileContent";
        DataTable = "Database_Files";
    }
    else if(dbtype==1){
        ContentType = "Content";
        DataTable = "Documents";
    }
    QSqlQuery files(database);
    files.exec(QString("SELECT DISTINCT Path,%1,BuildCommand,Preamble FROM %2").arg(ContentType,DataTable));
    while(files.next()){
        QString filePath = files.value(0).toString();
        QString fileContent = files.value(1).toString();
        QString CurrentBuildCommand = files.value(2).toString();
        QString Preamble = files.value(3).toString();
        DataTex::CurrentPreamble_Content = SqlFunctions::Get_String_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                        .arg(Preamble)
                                                                    ,DataTex::DataTeX_Settings);
        QDir dir(QFileInfo(filePath).absolutePath());
        if (!dir.exists())
            dir.mkpath(".");
        QFile texFile(filePath);
        texFile.open(QIODevice::ReadWrite);
        QTextStream writeContent(&texFile);
        writeContent.flush();
        writeContent << fileContent;
        texFile.close();
//        if(PdfSelected){
//            FileCommands::CreateTexFile(file,0,"");
//            FileCommands::BuildDocument(DataTex::DTXBuildCommands[CurrentBuildCommand],file
//                                   ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
//            FileCommands::ClearOldFiles(file);
//        }
        CsvFunctions::WriteDataToCSV(filePath,database);
    }
}

void DataTex::encrFileDB_Dialog(QList<DTXDatabase> databases)
{
    QDialog *d = new QDialog(this);
    d->setWindowTitle(tr("Encrypted databases"));
    QGridLayout * grid = new QGridLayout(this);
    QVBoxLayout * vbox = new QVBoxLayout(this);
    QLabel * message = new QLabel(tr("The following databases are encrypted.\nTo open them please enter the username and the password."));
    grid->addWidget(message);
    QHBoxLayout * h2 = new QHBoxLayout(this);
    QLabel * warning = new QLabel(this);
//    QLabel * fl = new QLabel(tr("Files databases"));
//    QLabel * dl = new QLabel(tr("Documents databases"));
    QDialogButtonBox *dbox = new QDialogButtonBox(QDialogButtonBox::Close,this);
    h2->addWidget(warning);
    h2->addWidget(dbox);

    for (DTXDatabase &db : databases) {
        QHBoxLayout * h = new QHBoxLayout(this);
        QLabel * l = new QLabel(db.Description);
        QLineEdit * user = new QLineEdit(this);
        PasswordLineEdit * password = new PasswordLineEdit(this);
        QPushButton *openDB = new QPushButton(QIcon::fromTheme("DocumentEncrypted"),"",this);
            h->addWidget(l);
            h->addWidget(user);
            h->addWidget(password);
            h->addWidget(openDB);
            vbox->addLayout(h);
        user->setPlaceholderText(tr("User name..."));
        password->setPlaceholderText(tr("Password..."));
        connect(openDB,&QPushButton::clicked,this,[=](){
            QString cur_user = user->text();
            const QByteArray pass = password->text().toUtf8();
            QString cur_pass = QCryptographicHash::hash(pass,QCryptographicHash::Sha256);
            if(cur_user==db.Username && cur_pass==db.Password){
//                db.Encrypt = false;
                QList<QTreeWidgetItem*> clist = ui->OpenDatabasesTreeWidget->findItems(db.BaseName, Qt::MatchContains|Qt::MatchRecursive, 2);
                ui->OpenDatabasesTreeWidget->setCurrentItem(clist[0]);
                on_OpenDatabasesTreeWidget_itemClicked(clist[0],0);
                UpdateCurrentDatabase(GlobalDatabaseList.value(db.BaseName));
                switch (db.Type) {
                case DTXDatabaseType::FilesDB:
                    ShowDataBaseFiles();
                    break;
                case DTXDatabaseType::DocumentsDB:
                    ShowDocuments();
                    break;
                case DTXDatabaseType::BibliographyDB:

                    break;
                case DTXDatabaseType::CommandsDB:

                    break;
                case DTXDatabaseType::PreamblesDB:

                    break;
                case DTXDatabaseType::PackagesDB:

                    break;
                case DTXDatabaseType::ClassesDB:

                    break;
                case DTXDatabaseType::TablesDB:

                    break;
                case DTXDatabaseType::FiguresDB:

                    break;
                }

                openDB->setIcon(QIcon::fromTheme("DialogOkApply"));
                openDB->setEnabled(false);
                user->setEnabled(false);
                password->setEnabled(false);
                warning->clear();
            }
            else{
                warning->setText(tr("Wrong username or password!"));
            }
        });
        connect(dbox->button(QDialogButtonBox::Close),&QPushButton::clicked,this,[=](){
            d->close();
        });
    }
//    if(fdb_enc>0 && databases.count()>0){
//        grid->addWidget(fl);
//    }
    grid->addLayout(vbox,1,0);
//    if(ddb_enc>0 && databases.count()>0){
//        grid->addWidget(dl);
//    }
    grid->addLayout(h2,2,0);
    d->setLayout(grid);
    d->show();
}

void DataTex::runQuery_Root(QString queryText,QSqlDatabase database)
{
    /* Make dir writable for DataTex_Settings
     * QProcess::execute("chmod",{"777",datatexpath});
     * run query for DT and Bib databases
     * QProcess::execute("chmod",{"555",datatexpath});
     *  and back readable again */
//    QProcess::execute("chmod",{"777",datatexpath});
    QSqlQuery query(database);
    query.exec(queryText);
//    QProcess::execute("chmod",{"555",datatexpath});
}

QString DataTex::getDataTexPath()
{
    return datatexpath;
}

void DataTex::initialize(QString dtexFile)
{
//    QString dtexFile = QCoreApplication::arguments().last();
    if(QCoreApplication::arguments().size() > 1 && QFile::exists(dtexFile)){
        DTXDatabaseInfo DBInfo = FileCommands::GetDatabaseTypeFromDTexFile(dtexFile);
        QString texFile = dtexFile;
        texFile.replace(".dtex",".tex");
        bool databaseExists = (GlobalDatabaseList.contains(DBInfo.Id));
        if(databaseExists){
            QList<QTreeWidgetItem*> clist = ui->OpenDatabasesTreeWidget->findItems(DBInfo.Name, Qt::MatchContains|Qt::MatchRecursive, 0);
            ui->OpenDatabasesTreeWidget->setCurrentItem(clist[0]);
            on_OpenDatabasesTreeWidget_itemClicked(clist[0],0);
            if(DBInfo.Type == DTXDatabaseType::FilesDB){
                DTXFile *fileInfo = new DTXFile(dtexFile);
                if(SelectNewFileInModel(FilesTable,fileInfo->Id)){
                    return;
                }
                else{
                    NewDatabaseFile * newfile = new NewDatabaseFile(this,fileInfo,NewFileMode::CloneModeContentAndMetadata/*false,true*/);
//                    connect(newfile,&NewDatabaseFile::acceptClone,this,[=](){
//                        ShowDataBaseFiles();
//                    });
                    newfile->show();
                    newfile->activateWindow();
                }
            }
            else if(DBInfo.Type == DTXDatabaseType::DocumentsDB){
                DTXDocument *docInfo = new DTXDocument(dtexFile);
                if(SelectNewFileInModel(FilesTable,docInfo->Id)){
                    return;
                }
                else{
                    DTXDocument temp;
                    NotesDocuments * newdoc = new NotesDocuments(this,temp,ImportMode);//?
                    //προσωρινό. να γίνει αλλαγή σε NotesDocuments(this,docInfo,false,false,true)
                    connect(newdoc,&NotesDocuments::createnewdocument,this,[=](){
                        ShowDocuments();
                    });
                    newdoc->show();
                    newdoc->activateWindow();
                }
            }
        }

        else if(!databaseExists){
            QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Database missing"),tr("The database %1 is missing\nfrom the database list.\nWould you like to add it?").arg(DBInfo.Name),
                                     QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
            if (resBtn == QMessageBox::No) {
                return;
            }
            else{
                QString filePath = QFileDialog::getOpenFileName(this,tr("Open Database"),
                                   QFileInfo(DBInfo.Path).absolutePath(),
                                   tr("Sqlite Databases (*.db)"));
                if(filePath.isEmpty()) {
                    return;
                }
                else{
                    QStringList list = QFileInfo(filePath).absolutePath().split(QDir::separator());
                    QString folder = list.last();
                    OpenDatabaseInfo(filePath,folder);
                }
            }
        }
    }
}

void DataTex::onOtherInstanceMessage(const QString &file)   // Added slot for messages to the single instance
{
    show();
    setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    this->showNormal();
    initialize(file);
}

QString DataTex::FileCount(QSqlDatabase database,ExtendedTableWidget * table)
{
//    QAbstractItemModel * m = table->model();
//    QModelIndex ix = table->currentIndex();
//    while (table->model()->canFetchMore(ix))
//        table->model()->fetchMore(ix);
//    return QString::number(m->rowCount());
    return SqlFunctions::Get_String_From_Query("SELECT COUNT(Id) FROM Database_Files",database);
}

void DataTex::changeIcon(bool checked,QString unchecked_icon, QString checked_icon)
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        if (checked)
        {
            button->setIcon(QIcon(checked_icon));
        }
        else
        {
            button->setIcon(QIcon(unchecked_icon));
        }
    }
}

void DataTex::on_CloseButton_clicked()
{
    qApp->exit();
}


void DataTex::on_MaximizeButton_clicked()
{
    if(!isMaximized()){
        showMaximized();
        ui->MaximizeButton->setIcon(QIcon(":/images/SideBar/expand"));
    }
    else{
        showNormal();
        ui->MaximizeButton->setIcon(QIcon(":/images/SideBar/chevron-up"));
    }
}


void DataTex::on_MinimizeButton_clicked()
{
    showMinimized();
}

void DataTex::SetDatabaseConnected(QTreeWidgetItem * item)
{
    QString db = item->text(2);
    if(!GlobalDatabaseList.value(db).Encrypt){
        GlobalDatabaseList[db].Database.open();
        GlobalDatabaseList[db].IsConnected = true;
        item->setIcon(0,QIcon::fromTheme("ConnectDB"));
        ui->OpenDatabasesTreeWidget->setCurrentItem(item);
        on_OpenDatabasesTreeWidget_itemClicked(item,0);
        ConnectDatabase->setEnabled(false);
        DisconnectDatabase->setEnabled(true);
        QSqlQuery setConnected(DataTeX_Settings);
        setConnected.exec(QString("UPDATE DataBases SET IsConnected = %1 WHERE FileName = '%2'").arg("1",db));
    }
}

void DataTex::SetDatabaseDisconnected(QTreeWidgetItem *item)
{
    QString db = item->text(2);
    GlobalDatabaseList[db].Database.close();
    GlobalDatabaseList[db].IsConnected = false;
    item->setIcon(0,QIcon::fromTheme("DisconnectDB"));
    ui->OpenDatabasesTreeWidget->setCurrentItem(item);
    on_OpenDatabasesTreeWidget_itemClicked(item,0);
    ConnectDatabase->setEnabled(true);
    DisconnectDatabase->setEnabled(false);
    QSqlQuery setConnected(DataTeX_Settings);
    setConnected.exec(QString("UPDATE DataBases SET IsConnected = %1 WHERE FileName = '%2'").arg("0",db));
}

void DataTex::AreAllDBConnected()
{
    int DBConnected = 0;
    for(DTXDatabase DTXDB : GlobalDatabaseList.values()){
        if(DTXDB.IsConnected){
            DBConnected++;
        }
    }
    emit sendDBConnectionInfo(DBConnected);
}

