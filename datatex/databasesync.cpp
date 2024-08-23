#include "databasesync.h"
#include "ui_databasesync.h"
#include "sqlfunctions.h"
#include "datatex.h"
#include <qmath.h>
#include "switch.h"
#include "latexeditorwidget.h"
#include "csvfunctions.h"
#include "filecommands.h"

DatabaseSync::DatabaseSync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseSync)
{
    ui->setupUi(this);

    //Load all file databases
    int i=-1;
    for (DTXDatabase DTXDB : DataTex::GlobalDatabaseList) {
        i++;
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,DTXDB.BaseName);
        item->setText(2,DTXDB.Path);
        item->setText(0,DTXDB.Description);
        QString fileCount = SqlFunctions::Get_String_From_Query("SELECT COUNT(Id) FROM Database_Files",DTXDB.Database);
        item->setText(1,fileCount+" files");
        ui->OpenDatabasesTreeWidget->topLevelItem(DTXDB.Type)->addChild(item);
    }

    ui->OpenDatabasesTreeWidget->expandAll();
    ui->OpenDatabasesTreeWidget->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->ResultsTreeWidget->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->ScanFiles->setEnabled(false);
    ui->StartSync->setEnabled(false);
    ui->buttonGroup->setExclusive(false);
    for (int i=0;i<ui->buttonGroup->buttons().count();i++) {
        ui->buttonGroup->buttons().at(i)->setEnabled(false);
    }
    ui->MetadataDifferences->setHorizontalHeaderLabels({"Database field","Values in csv","Values in database"});
    DataTex::StretchColumnsToWidth(ui->MetadataDifferences);
    ui->MetadataDifferences->setColumnHidden(3,true);
    FoundFont.setBold(true);
    redBrush=(QColor(200, 20, 20 ));
    connect(this,SIGNAL(progress(int,int)),this,SLOT(ShowProgress(int,int)));
    connect(ui->ContentFromDatabaseEdit->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->ContentFromFileEdit->verticalScrollBar()->setValue(ui->ContentFromDatabaseEdit->verticalScrollBar()->value());
    });
    connect(ui->ContentFromFileEdit->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->ContentFromDatabaseEdit->verticalScrollBar()->setValue(ui->ContentFromFileEdit->verticalScrollBar()->value());
    });
    ui->ResultsTreeWidget->setColumnHidden(3,true);
}

DatabaseSync::~DatabaseSync()
{
    delete ui;
}

void DatabaseSync::on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()){
        ResultsList.clear();
        ui->ScanFiles->setEnabled(true);
        ui->StartSync->setEnabled(false);
        if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==0){
            Table = "Metadata";
            rem = " files";
            ContentType = "FileContent";
            DataTable = "Database_Files";
            currentBase = DataTex::GlobalDatabaseList.value(item->text(3)).Database;
        }
        else if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==1){
            Table = "DocMetadata";
            rem = " documents";
            ContentType = "Content";
            DataTable = "Documents";
            currentBase = DataTex::GlobalDatabaseList.value(item->text(3)).Database;
        }
        for (int i=0;i<ui->buttonGroup->buttons().count();i++) {
            ui->buttonGroup->buttons().at(i)->setEnabled(true);
            connect(ui->buttonGroup->buttons().at(i),&QCheckBox::clicked,this,[=](){
                if(filesync || metacheck || missfiles){
                    ui->ScanFiles->setEnabled(true);
                }
                else{
                    ui->ScanFiles->setEnabled(false);
                }
            });
        }
        QSqlQuery query(currentBase);
        query.exec(QString("SELECT Id,Name FROM BackUp WHERE Table_Id = '%1'").arg(Table));
        while(query.next()){
            Database_FileTableFields.insert(query.value(0).toString(),query.value(1).toString());
        }
//        Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM BackUp WHERE Table_Id = \"%1\"").arg(Table),currentBase);
        TotalFiles = item->text(1).remove(rem).toInt();
    }
}


void DatabaseSync::on_ResultsTreeWidget_itemSelectionChanged()
{
    ui->MetadataDifferences->setRowCount(0);
    QModelIndex ix = ui->ResultsTreeWidget->currentIndex();
    QModelIndex index = ix;
    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
    }
    int topLevel = ix.row();
    ui->ResultsTab->setCurrentIndex(topLevel);

    if(topLevel==0 && index.parent().isValid()){
//        ui->ResultsTreeWidget->setHeaderLabels({"File","Sync file content to database","Sync database content to file"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        ui->ContentFromFileEdit->clear();
        ui->ContentFromDatabaseEdit->clear();
        ui->ContentFromFileEdit->insertPlainText(ResultsList[baseName].ContentFromFile);
        ui->ContentFromDatabaseEdit->insertPlainText(ResultsList[baseName].ContentFromDatabase);
        LatexTextEdit::ShowDifferences(ui->ContentFromFileEdit,ui->ContentFromDatabaseEdit);
    }

    if(topLevel==1 && index.parent().isValid()){
//        ui->ResultsTreeWidget->setHeaderLabels({"File","Sync data to csv","Sync data to database"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        int i=-1;
        foreach(QString key,DifferencesInCSV[baseName].keys()){
            i++;
            ui->MetadataDifferences->insertRow(i);
            ui->MetadataDifferences->setItem(i,0, new QTableWidgetItem(Database_FileTableFields[key]));
            ui->MetadataDifferences->setItem(i,1, new QTableWidgetItem(DifferencesInCSV[baseName][key]));
            ui->MetadataDifferences->setItem(i,2, new QTableWidgetItem(DifferencesInDatabase[baseName][key]));
        }
        DataTex::StretchColumnsToWidth(ui->MetadataDifferences);
    }

    if(topLevel==2 && index.parent().isValid()){
//        ui->ResultsTreeWidget->setHeaderLabels({"File","Create missing files","Delete database entry"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        QStringList data;
        QSqlQuery dataQuery(currentBase);
        dataQuery.exec(QString("SELECT Id,BuildCommand,Preamble,Date,Path,%2 FROM %3 WHERE Id = \"%1\"").arg(baseName,ContentType,DataTable));
        while (dataQuery.next())
        {
            QSqlRecord record = dataQuery.record();
            for(int i=0; i < record.count(); i++)
            {
                data << record.value(i).toString();
            }
        }
        if(data.count()>0){
            ui->FileNameLabel->setText(data[0]);
            ui->BuildCommandLabel->setText(data[1]);
            ui->PreambleLabel->setText(data[2]);
            ui->DateLabel->setText(data[3]);
            ui->PathLabel->setText(data[4]);
            ui->ContentOfMissingFile->setText(data[5]);
        }
//        for(int i=0;i<DifferencesInDatabase[baseName].count();i++){
//            ui->MetadataDifferences->insertRow(i);
//            ui->MetadataDifferences->setItem(i,1, new QTableWidgetItem(DifferencesInCSV[baseName].at(i)));
//            ui->MetadataDifferences->setItem(i,2, new QTableWidgetItem(DifferencesInDatabase[baseName].at(i)));
//            ui->MetadataDifferences->setItem(i,0, new QTableWidgetItem(DatabaseFieldsWithDifferences[baseName].at(i)));
//        }
    }
}

void DatabaseSync::on_ScanFiles_clicked()
{
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(0)->takeChildren()){delete i;}
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(1)->takeChildren()){delete i;}
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(2)->child(0)->takeChildren()){delete i;}
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(2)->child(1)->takeChildren()){delete i;}
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(2)->child(2)->takeChildren()){delete i;}
    foreach(auto i, ui->ResultsTreeWidget->topLevelItem(2)->child(3)->takeChildren()){delete i;}
    ui->ResultsTreeWidget->topLevelItem(0)->setText(0,"Content differences");
    ui->ResultsTreeWidget->topLevelItem(1)->setText(0,"Metadata differences");
    ui->ResultsTreeWidget->topLevelItem(2)->child(0)->setText(0,"Tex missing");
    ui->ResultsTreeWidget->topLevelItem(2)->child(1)->setText(0,"Pdf missing");
    ui->ResultsTreeWidget->topLevelItem(2)->child(2)->setText(0,"Csv missing");
    ui->ResultsTreeWidget->topLevelItem(2)->child(3)->setText(0,"DTex missing");
    ContentsFromFiles.clear();
    ContentsFromDatabase.clear();
    QStringList FilePaths = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(0);
    QStringList FileContents = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(1);
    qDebug()<<FilePaths;
    for(int i=0;i<FilePaths.count();i++){
        QString baseName = QFileInfo(FilePaths[i]).baseName();
        int value = ceil(((i+1)*100)/TotalFiles);
        if(filesync){
            QFile file(FilePaths[i]);
            file.open(QIODevice::ReadOnly);
            QTextStream text(&file);
            QString FileContent = text.readAll();
            file.close();
            //Check differences in the content
            if(FileContents[i]!=FileContent){
                FileScanResults result;
                result.Path = FilePaths[i];
                result.ContentFromDatabase = FileContents[i];
                result.ContentFromFile = FileContent;
                FilesFound++;
                result.SyncContentFlag = SyncContentToDatabase;
                AddItem(result,ContentDifferences,0);
            }
            int rows = ui->ResultsTreeWidget->topLevelItem(0)->childCount();
            ui->ResultsTreeWidget->topLevelItem(0)->setText(0,"Content differences : "+QString::number(rows)+" found");
            if(rows>0){
                ui->ResultsTreeWidget->topLevelItem(0)->setForeground( 0 , redBrush );
                ui->ResultsTreeWidget->topLevelItem(0)->setFont( 0,  FoundFont );
            }
        }

        //Check for metadata differences
        if(metacheck){
            QString csvFile = FilePaths[i];
            csvFile.replace(".tex",".csv");
            if(QFileInfo::exists(csvFile) && CheckMetadata(FilePaths[i],currentBase)){
                FileScanResults result;
                result.Path = FilePaths[i];
                FilesFound++;
                result.SyncMetadataFlag = MirrorFromDatabase;
                AddItem(result,Metadata,1);
            }
            int rows = ui->ResultsTreeWidget->topLevelItem(1)->childCount();
            ui->ResultsTreeWidget->topLevelItem(1)->setText(0,"Metadata differences : "+QString::number(rows)+" found");
            if(rows>0){
                ui->ResultsTreeWidget->topLevelItem(1)->setForeground( 0 , redBrush );
                ui->ResultsTreeWidget->topLevelItem(1)->setFont( 0,  FoundFont );
            }
        }
        //Check if tex,pdf or csv file is missing
        if(missfiles){
            QString PdfFile = FilePaths[i];
            PdfFile.replace(".tex",".pdf");
            QString CsvFile = FilePaths[i];
            CsvFile.replace(".tex",".csv");
            QString DTexFile = FilePaths[i];
            DTexFile.replace(".tex",".dtex");
            FileScanResults result;
            if(!QFileInfo::exists(FilePaths[i])){
                FileScanResults result;
                result.Path = FilePaths[i];
                FilesFound++;
                result.CreateMissingFileFlag |= CreateTex;
                AddItem(result,MissingFiles,2,0);
            }
            if(!QFileInfo::exists(PdfFile)){
                FileScanResults result;// = ScanResultInList(FilePaths[i]);
                result.Path = FilePaths[i];
                FilesFound++;
                result.CreateMissingFileFlag |= CreatePdf;
                QStringList list = SqlFunctions::Get_Record_From_Query(QString("SELECT BuildCommand,Preamble FROM %2 WHERE Id = \"%1\"").arg(baseName,DataTable),currentBase);
                result.BuildCommand = list[0];
                result.Preamble = list[1];
                AddItem(result,MissingFiles,2,1);
            }
            if(!QFileInfo::exists(CsvFile)){
                FileScanResults result;
                result.Path = FilePaths[i];
                FilesFound++;
                result.CreateMissingFileFlag |= CreateCSV;
                AddItem(result,MissingFiles,2,2);
            }
            if(!QFileInfo::exists(DTexFile)){

                result.Path = FilePaths[i];
                FilesFound++;
                result.CreateMissingFileFlag |= CreateDTex;
                AddItem(result,MissingFiles,2,3);
            }
            int rows1 = ui->ResultsTreeWidget->topLevelItem(2)->child(0)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(0)->setText(0,"Tex missing : "+QString::number(rows1)+" found");
            int rows2 = ui->ResultsTreeWidget->topLevelItem(2)->child(1)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(1)->setText(0,"Pdf missing : "+QString::number(rows2)+" found");
            int rows3 = ui->ResultsTreeWidget->topLevelItem(2)->child(2)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(2)->setText(0,"Csv missing : "+QString::number(rows3)+" found");
            int rows4 = ui->ResultsTreeWidget->topLevelItem(2)->child(3)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(3)->setText(0,"DTex missing : "+QString::number(rows4)+" found");
            if(rows1+rows2+rows3+rows4>0){
                ui->ResultsTreeWidget->topLevelItem(2)->setForeground( 0 , redBrush );
                ui->ResultsTreeWidget->topLevelItem(2)->setFont( 0,  FoundFont );
            }
            ui->ResultsTreeWidget->topLevelItem(2)->setText(0,"Missing files :"+QString::number(rows1+rows2+rows3+rows4)+" found");
        }

        ui->progressBar->setValue(value);
        if(ui->progressBar->value()==100){
            ui->StartSync->setEnabled(true);
        }
    }
    ui->ScanFiles->setEnabled(false);
    for (int i=0;i<ui->buttonGroup->buttons().count();i++) {
        ui->buttonGroup->buttons().at(i)->setEnabled(false);
    }

}

//void DatabaseSync::ExclusiveSwitches(Switch * sw1,Switch * sw2)
//{
//    if(sw1->isChecked()){
//        sw2->setChecked(false);
//    }
//    else{
//        sw2->setChecked(true);
//    }
//}

void DatabaseSync::on_SyncCheck_clicked(bool checked)
{
    filesync = checked;
}


void DatabaseSync::on_MetaCheck_clicked(bool checked)
{
    metacheck = checked;
}


void DatabaseSync::on_MissingCheck_clicked(bool checked)
{
    missfiles = checked;
}

bool DatabaseSync::CheckMetadata(QString result,QSqlDatabase database)
{
//    QString Id = result.Id;
//    QList<QHash<QString, QString>> compResults = CompareLists(CsvFunctions::ReadCsv(result.Path),
//                                                             SqlFunctions::ReadRow(CsvFunctions::getFile(Id,database.databaseName()),database));
//    result.DifferencesInCSV.insert(Id,compResults[0]);
//    result.DifferencesInDatabase.insert(Id,compResults[1]);
    return true;//DifferencesInCSV[Id].count()>0;
}

QList<QHash<QString, QString>> DatabaseSync::CompareLists(QHash<QString,QString> list1, QHash<QString,QString> list2)
{
    QHash<QString,QString> diff1;
    QHash<QString,QString> diff2;
    foreach(QString key,list1.keys()){
        if(list1[key]!=list2[key]){
            diff1.insert(key,list1[key]);
            diff2.insert(key,list2[key]);
        }
    }
    return {diff1,diff2};
}

void DatabaseSync::on_StartSync_clicked()
{
    QHash<QString, FileScanResults>::const_iterator i;
    int FilesSynced = 0;
    for (i = ResultsList.constBegin(); i != ResultsList.constEnd(); ++i)
        if(i.value().SyncFile){
            Sync(i.value());
            FilesSynced++;
            ui->SyncProgressBar->setValue(FilesSynced*100/ResultsList.count());
//            emit progress(FilesSynced,ResultsList.count());
            qDebug()<<FilesSynced<<" of "<<ResultsList.count();
        }
}

void DatabaseSync::ShowProgress(int files,int total)
{
    ui->SyncProgressBar->setValue(files*100/total);
}

void DatabaseSync::SyncMetadataToDatabase(QString file)
{
    QHash<QString,QString> Metadata = CsvFunctions::ReadCsv(file);
    QSqlQuery WriteQuery(currentBase);
    QString query ="UPDATE \""+DataTable+"\" SET ";
    QStringList list;
    foreach(QString key,Metadata.keys()) {
        list.append("\""+key+"\""+"="+"\""+Metadata[key]+"\"");
    }
    query += list.join(",")+" WHERE \"Id\"=\""+QFileInfo(file).baseName()+"\"";
    WriteQuery.exec(query);
}

void * DatabaseSync::AddItem(FileScanResults result,int flag,int level1,int level2)
{
    QTreeWidgetItem * item = new QTreeWidgetItem();
    QComboBox * ActionCombo = new QComboBox(this);
    result.Id = QFileInfo(result.Path).baseName();
    item->setText(2,result.Path);
    item->setText(0,result.Id);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    item->setCheckState(0,Qt::Checked);
    QString file = item->text(0);
    ResultsList.insert(file,result);
    switch (flag) {
    case ContentDifferences:
        ActionCombo->addItem(tr("Sync file from database"),QVariant(SyncContentToDatabase));
        ActionCombo->addItem(tr("Sync database from file"),QVariant(SyncContentToFile));
        ActionCombo->addItem(tr("Delete file"),QVariant(DeleteEntry));
        connect(ActionCombo,QOverload<int>::of(&QComboBox::activated),this,[=](int index)/*mutable*/{
            ResultsList[file].SyncContentFlag = ActionCombo->currentData().value<SyncFlags>();
            qDebug()<<file<<"   "<<ResultsList[file].SyncContentFlag;
        });
        break;
    case Metadata:
        ActionCombo->addItem(tr("Sync all from database"),QVariant(MirrorFromDatabase));
        ActionCombo->addItem(tr("Sync all from dtex"),QVariant(MirrorFromDTex));
        ActionCombo->addItem(tr("Sync all from csv"),QVariant(MirrorFromCSV));
        ActionCombo->addItem(tr("Delete file"),QVariant(DeleteEntry));
        connect(ActionCombo,QOverload<int>::of(&QComboBox::activated),this,[=](int index)/*mutable*/{
            ResultsList[file].SyncMetadataFlag = ActionCombo->currentData().value<SyncFlags>();
            qDebug()<<file<<"   "<<ResultsList[file].SyncMetadataFlag;
        });
        break;
    case MissingFiles:
        ActionCombo->addItem(tr("Create missing file"),QVariant(true));
        ActionCombo->addItem(tr("Delete file"),QVariant(false));
        connect(ActionCombo,QOverload<int>::of(&QComboBox::activated),this,[=](int index)/*mutable*/{
            if(index==0){
                if(level2==0){
                    ResultsList[file].CreateMissingFileFlag |= CreateTex;
                }
                if(level2==1){
                    ResultsList[file].CreateMissingFileFlag |= CreatePdf;
                }
                if(level2==2){
                    ResultsList[file].CreateMissingFileFlag |= CreateCSV;
                }
                if(level2==3){
                    ResultsList[file].CreateMissingFileFlag |= CreateDTex;
                }
            }
            else{
                ResultsList[file].CreateMissingFileFlag = DeleteEntry;
            }
            qDebug()<<file<<"   "<<ResultsList[file].CreateMissingFileFlag;
        });
        break;
    }
    if(level2==-1){
        ui->ResultsTreeWidget->topLevelItem(level1)->addChild(item);
    }
    else{
        ui->ResultsTreeWidget->topLevelItem(level1)->child(level2)->addChild(item);
    }
    connect(ui->ResultsTreeWidget, &QTreeWidget::itemClicked,this,[=]()/*mutable*/{
        ResultsList[file].SyncFile = static_cast<bool>(item->checkState(0));
        ActionCombo->setEnabled(ResultsList[file].SyncFile);
        qDebug()<<file<<"   "<<ResultsList[file].SyncFile;
    });
    ui->ResultsTreeWidget->setItemWidget(item,1,ActionCombo);
}

void DatabaseSync::Sync(FileScanResults results)
{
    if(results.SyncContentFlag & SyncContentToDatabase){
//        QSqlQuery WriteContent(currentBase);
//        WriteContent.exec(QString("UPDATE %3 SET %4 = \"%1\" WHERE Id = \"%2\"").arg(results.ContentFromFile,results.Id,DataTable,ContentType));
        qDebug()<<results.Id<<"   1. sync to db";
    }
    if((results.SyncContentFlag & SyncContentToFile) | (results.CreateMissingFileFlag & CreateTex)){
//        QDir datatexdir(QFileInfo(filePath).absolutePath());
//        if (!datatexdir.exists())
//            datatexdir.mkpath(".");
//        QFile texFile(results.Path);
//        texFile.open(QIODevice::ReadWrite);
//        QTextStream writeContent(&texFile);
//        writeContent.flush();
//        texFile.resize(0);
//        writeContent << results.ContentFromDatabase;
//        texFile.close();
        qDebug()<<results.Id<<"   2. sync to file";
    }
    if(results.SyncMetadataFlag & MirrorFromDatabase){
//        CsvFunctions::WriteDataToCSV(results.Path,currentBase);
        //dtex
        qDebug()<<results.Id<<"  3. Mirror from db";
    }
    if(results.SyncMetadataFlag & MirrorFromDTex){
//        SyncMetadataToDatabase(results.Path);
    //csv
    qDebug()<<results.Id<<"  4. Mirror from dtex";
    }
    if(results.SyncMetadataFlag & MirrorFromCSV){
//        SyncMetadataToDatabase(results.Path);
        //dtex
        qDebug()<<results.Id<<"  5. Mirror from csv";
    }
    if(results.CreateMissingFileFlag & CreatePdf){
//        DataTex::CurrentPreamble_Content = SqlFunctions::Get_String_From_Query(QString(SqlFunctions::GetPreamble_Content)
//                                                                       .arg(Preamble)
//                                                                   ,DataTex::DataTeX_Settings);
//        FileCommands::CreateTexFile(results.Path,0,"");
//        FileCommands::BuildDocument(DataTex::DTXBuildCommands[results.BuildCommand],results.Path
//                                    ,DataTex::LatexCommandsArguments[results.BuildCommand],".tex");
//        FileCommands::ClearOldFiles(results.Path);
        qDebug()<<results.Id<<"  6. Create pdf";
    }
    if(results.CreateMissingFileFlag & CreateCSV){
//        CsvFunctions::WriteDataToCSV(results.Path,currentBase);
        qDebug()<<results.Id<<"  7. Create csv";
    }
    if(results.CreateMissingFileFlag & CreateDTex){
//        if(rem == " files"){
//            DTXFile fileInfo = DTXFile(results.Id,currentBase);
//            DTXFile::WriteDTexFile(fileInfo);
//        }
//        else if(rem == " documents"){
//            DTXDocument docInfo = DTXDocument(results.Id,currentBase);
//            DTXDocument::WriteDTexFile(docInfo);
//        }
        qDebug()<<results.Id<<"  8. Create dtex";
    }
    if((results.SyncContentFlag & DeleteEntry) | (results.SyncMetadataFlag & DeleteEntry) |
         (results.CreateMissingFileFlag & DeleteEntry)){
//        QSqlQuery deleteQuery(currentBase);
//        deleteQuery.exec(QString("DELETE FROM \"%2\" WHERE \"Id\" = \"%1\"").arg(results.Id,DataTable));
        qDebug()<<results.Id<<"  9. delete entry from database";
    }
}
