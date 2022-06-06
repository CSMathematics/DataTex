#include "databasesync.h"
#include "ui_databasesync.h"
#include "sqlfunctions.h"
#include "datatex.h"
#include <qmath.h>
#include "switch.h"
#include "highlighter.h"
#include "csvfunctions.h"

DatabaseSync::DatabaseSync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseSync)
{
    ui->setupUi(this);

    //Load all file databases
    for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++ ) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,QFileInfo(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()).baseName());
        item->setText(2,DataTex::GlobalFilesDatabaseList.values().at(i).databaseName());
        ui->OpenDatabasesTreeWidget->topLevelItem(0)->addChild(item);
        QString DatabaseName = QFileInfo(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()).baseName();
        QStringList fileCount = SqlFunctions::Get_StringList_From_Query("SELECT COUNT(DISTINCT Id) FROM Database_Files",DataTex::GlobalFilesDatabaseList[DatabaseName]);
        if(fileCount.count()){
            ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(i)->setText(1,fileCount.at(0)+" files");
        }
        ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(i)->setText(0,DataTex::GlobalFilesDatabaseListNames.values().at(i));
    }

    //Load all document databases
    for (int i=0;i<DataTex::GlobalDocsDatabaseList.count();i++ ) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,QFileInfo(DataTex::GlobalDocsDatabaseList.values().at(i).databaseName()).baseName());
        item->setText(2,DataTex::GlobalDocsDatabaseList.values().at(i).databaseName());
        ui->OpenDatabasesTreeWidget->topLevelItem(1)->addChild(item);
        QString DatabaseName = QFileInfo(DataTex::GlobalDocsDatabaseList.values().at(i).databaseName()).baseName();
        QStringList fileCount = SqlFunctions::Get_StringList_From_Query("SELECT COUNT(DISTINCT Id) FROM Documents",DataTex::GlobalDocsDatabaseList[DatabaseName]);
        if(fileCount.count()){
            ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(i)->setText(1,fileCount.at(0)+" documents");
        }
        ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(i)->setText(0,DataTex::GlobalDocsDatabaseListNames.values().at(i));
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

//    QHash<QString,QString> list;
//    QSqlQueryModel *model = new QSqlQueryModel(this);
//    QSqlQuery DatabaseQuery(DataTex::CurrentTexFilesDataBase);
//    DatabaseQuery.exec(CsvFunctions::getFile("Alg-Exis1ou-ExisApolTim-SectEx1"),);
//    model->setQuery(DatabaseQuery);
//    for(int i = 0; i < model->columnCount(); i++)
//    {
//      list.insert(model->headerData(i, Qt::Horizontal).toString(),
//                  model->data(model->index(0,i)).toString());
//    }
//    qDebug()<<CsvFunctions::getFile("/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Εξισώσεις με απόλυτες τιμές/Alg-Exis1ou-ExisApolTim-SectEx1.csv")<<
//              DataTex::Optional_Metadata_Ids;
}

DatabaseSync::~DatabaseSync()
{
    delete ui;
}

void DatabaseSync::on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()){
        ui->ScanFiles->setEnabled(true);
        ui->StartSync->setEnabled(false);
        QVariant dataType = item->parent()->data(0,Qt::DisplayRole).toString();
        if(dataType == "Latex databases"){
            Table = "Metadata";
            rem = " files";
            ContentType = "FileContent";
            DataTable = "Database_Files";
            currentBase = DataTex::GlobalFilesDatabaseList[item->text(3)];
        }
        else if(dataType == "Document databases"){
            Table = "DocMetadata";
            rem = " documents";
            ContentType = "Content";
            DataTable = "Documents";
            currentBase = DataTex::GlobalDocsDatabaseList[item->text(3)];
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
        ui->ResultsTreeWidget->setHeaderLabels({"File","Sync file content to database","Sync database content to file"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        ui->ContentFromFileEdit->clear();
        ui->ContentFromDatabaseEdit->clear();
        ui->ContentFromFileEdit->insertPlainText(ContentsFromFiles[baseName]);
        ui->ContentFromDatabaseEdit->insertPlainText(ContentsFromDatabase[baseName]);
        for (int i=0;i<LatexTextEdit::buttonlist.count();i++) {
            delete LatexTextEdit::buttonlist[i];
        }
        LatexTextEdit::ShowDifferences(ui->ContentFromFileEdit,ui->ContentFromDatabaseEdit);
    }

    if(topLevel==1 && index.parent().isValid()){
        ui->ResultsTreeWidget->setHeaderLabels({"File","Sync data to csv","Sync data to database"});
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
        ui->ResultsTreeWidget->setHeaderLabels({"File","Create missing files","Delete database entry"});
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
    ui->ResultsTreeWidget->topLevelItem(0)->setText(0,"Content differences");
    ui->ResultsTreeWidget->topLevelItem(1)->setText(0,"Metadata differences");
    ui->ResultsTreeWidget->topLevelItem(2)->child(0)->setText(0,"Tex missing");
    ui->ResultsTreeWidget->topLevelItem(2)->child(1)->setText(0,"Pdf missing");
    ui->ResultsTreeWidget->topLevelItem(2)->child(2)->setText(0,"Csv missing");
    ContentsFromFiles.clear();
    ContentsFromDatabase.clear();
    QStringList FilePaths = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(0);
    QStringList FileContents = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(1);
    for(int i=0;i<FilePaths.count();i++){
        QString baseName = QFileInfo(FilePaths[i]).baseName();
        ContentsFromDatabase.insert(baseName,FileContents[i]);
        QFile file(FilePaths[i]);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        QString FileContent = text.readAll();
        file.close();
        ContentsFromFiles.insert(baseName,FileContent);
        int value = ceil(((i+1)*100)/TotalFiles);
        //Check differences in the content
        if(filesync){
            if(ContentsFromDatabase[baseName]!=FileContent){
                FilesFound++;
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(0)->addChild(item);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                SyncContentToDatabase.insert(baseName,true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    SyncContentToDatabase.insert(baseName,sw1->isChecked());
                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    SyncContentToDatabase.insert(baseName,sw1->isChecked());
                });
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
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
                FilesFound++;
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(1)->addChild(item);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                SyncMetadataToFile.insert(baseName,true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    SyncMetadataToFile.insert(baseName,sw1->isChecked());
                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    SyncMetadataToFile.insert(baseName,sw1->isChecked());
                });
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
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
            if(!QFileInfo::exists(FilePaths[i])){
                FilesFound++;
                TexFilesMissing.insert(baseName,FilePaths[i]);
                QStringList data;
                QSqlQuery dataQuery(currentBase);
                dataQuery.exec(QString("SELECT BuildCommand,Preamble FROM %2 WHERE Id = \"%1\"").arg(baseName,DataTable));
                while (dataQuery.next())
                {
                    QSqlRecord record = dataQuery.record();
                    for(int i=0; i < record.count(); i++)
                    {
                        data << record.value(i).toString();
                    }
                }
                BuildComPreamble.insert(baseName,data);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(0)->addChild(item);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewTexFile.insert(baseName,sw1->isChecked());
                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewTexFile.insert(baseName,sw1->isChecked());
                });
                sw1->setChecked(true);
                CreateNewTexFile.insert(baseName,sw1->isChecked());
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
            }
            if(!QFileInfo::exists(PdfFile)){
                FilesFound++;
                PdfFilesMissing.insert(baseName,PdfFile);
                QStringList data;
                QSqlQuery dataQuery(currentBase);
                dataQuery.exec(QString("SELECT BuildCommand,Preamble FROM %2 WHERE Id = \"%1\"").arg(baseName,DataTable));
                while (dataQuery.next())
                {
                    QSqlRecord record = dataQuery.record();
                    for(int i=0; i < record.count(); i++)
                    {
                        data << record.value(i).toString();
                    }
                }
                BuildComPreamble.insert(baseName,data);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(1)->addChild(item);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewPdfFile.insert(baseName,sw1->isChecked());

                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewPdfFile.insert(baseName,sw1->isChecked());

                });
                sw1->setChecked(true);
                CreateNewPdfFile.insert(baseName,sw1->isChecked());
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
            }
            if(!QFileInfo::exists(CsvFile)){
                FilesFound++;
                CsvFilesMissing.insert(baseName,CsvFile);
                QStringList data;
                QSqlQuery dataQuery(currentBase);
                dataQuery.exec(QString("SELECT BuildCommand,Preamble FROM %2 WHERE Id = \"%1\"").arg(baseName,DataTable));
                while (dataQuery.next())
                {
                    QSqlRecord record = dataQuery.record();
                    for(int i=0; i < record.count(); i++)
                    {
                        data << record.value(i).toString();
                    }
                }
                BuildComPreamble.insert(baseName,data);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(2)->addChild(item);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewCsvFile.insert(baseName,sw1->isChecked());

                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewCsvFile.insert(baseName,sw1->isChecked());

                });
                sw1->setChecked(true);
                CreateNewCsvFile.insert(baseName,sw1->isChecked());
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
            }
            int rows1 = ui->ResultsTreeWidget->topLevelItem(2)->child(0)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(0)->setText(0,"Tex missing : "+QString::number(rows1)+" found");
            int rows2 = ui->ResultsTreeWidget->topLevelItem(2)->child(1)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(1)->setText(0,"Pdf missing : "+QString::number(rows2)+" found");
            int rows3 = ui->ResultsTreeWidget->topLevelItem(2)->child(2)->childCount();
            ui->ResultsTreeWidget->topLevelItem(2)->child(2)->setText(0,"Csv missing : "+QString::number(rows3)+" found");
            if(rows1+rows2+rows3>0){
                ui->ResultsTreeWidget->topLevelItem(2)->setForeground( 0 , redBrush );
                ui->ResultsTreeWidget->topLevelItem(2)->setFont( 0,  FoundFont );
            }
            ui->ResultsTreeWidget->topLevelItem(2)->setText(0,"Missing files :"+QString::number(rows1+rows2+rows3)+" found");
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

void DatabaseSync::ExclusiveSwitches(Switch * sw1,Switch * sw2)
{
    if(sw1->isChecked()){
        sw2->setChecked(false);
    }
    else{
        sw2->setChecked(true);
    }
}

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

bool DatabaseSync::CheckMetadata(QString file,QSqlDatabase database)
{
    QString baseName = QFileInfo(file).baseName();
    QList<QHash<QString, QString>> compResults = CompareLists(CsvFunctions::ReadCsv(file),
                                                             SqlFunctions::ReadRow(CsvFunctions::getFile(baseName,database.databaseName()),database));
    DifferencesInCSV.insert(baseName,compResults[0]);
    DifferencesInDatabase.insert(baseName,compResults[1]);
    return DifferencesInCSV[baseName].count()>0;
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
    int fileSynced = 0;
    QSqlQuery WriteContent(currentBase);
    if(filesync){
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(0)->childCount();i++){
            QString filePath= ui->ResultsTreeWidget->topLevelItem(0)->child(i)->text(3);
            QString file = ui->ResultsTreeWidget->topLevelItem(0)->child(i)->text(0);
            QDir datatexdir(QFileInfo(filePath).absolutePath());
            if (!datatexdir.exists())
                datatexdir.mkpath(".");
            if(SyncContentToDatabase[file]==true) {
                WriteContent.exec(QString("UPDATE %3 SET %4 = \"%1\" WHERE Id = \"%2\"").arg(ContentsFromFiles[file],file,DataTable,ContentType));
            }
            else{
                qDebug()<<"false";
                QFile texFile(filePath);
                texFile.open(QIODevice::ReadWrite);
                QTextStream writeContent(&texFile);
                writeContent.flush();
                texFile.resize(0);
                writeContent << ContentsFromDatabase[file];
                texFile.close();
            }
            fileSynced++;
            emit progress(fileSynced,FilesFound);
        }
    }
    if(metacheck){
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(1)->childCount();i++){
            QString filePath = ui->ResultsTreeWidget->topLevelItem(1)->child(i)->text(3);
            QString file = ui->ResultsTreeWidget->topLevelItem(1)->child(i)->text(0);
            QDir datatexdir(QFileInfo(filePath).absolutePath());
            if (!datatexdir.exists())
                datatexdir.mkpath(".");
            if(SyncMetadataToFile[file]==true) {
//                SyncMetadataToCsvFile(filePath);
                CsvFunctions::WriteDataToCSV(filePath,currentBase);
                qDebug()<<"true";
            }
            else{
                SyncMetadataToDatabase(filePath);
            }
            fileSynced++;
            emit progress(fileSynced,FilesFound);
        }
    }
    if(missfiles){
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(2)->child(0)->childCount();i++){
            QString texfile = ui->ResultsTreeWidget->topLevelItem(2)->child(0)->child(i)->text(0);
            QString filePath= ui->ResultsTreeWidget->topLevelItem(2)->child(0)->child(i)->text(3);
            QDir datatexdir(QFileInfo(filePath).absolutePath());
            if (!datatexdir.exists())
                datatexdir.mkpath(".");
            if(CreateNewTexFile[texfile]==true) {
                QFile texFile(filePath);
                texFile.open(QIODevice::ReadWrite);
                QTextStream writeContent(&texFile);
                writeContent.flush();
                writeContent << ContentsFromDatabase[texfile];
                texFile.close();
                qDebug()<<"create new tex file";
            }
            else{
                QSqlQuery deleteQuery(currentBase);
                deleteQuery.exec(QString("DELETE FROM \"%2\" WHERE \"Id\" = \"%1\"").arg(texfile,DataTable));
                qDebug()<<"delete entry from database";
            }
            fileSynced++;
            emit progress(fileSynced,FilesFound);
        }

        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(2)->child(1)->childCount();i++){
            QString pdffile = ui->ResultsTreeWidget->topLevelItem(2)->child(1)->child(i)->text(0);
            QString filePath= ui->ResultsTreeWidget->topLevelItem(2)->child(1)->child(i)->text(3);
            QDir datatexdir(QFileInfo(filePath).absolutePath());
            if (!datatexdir.exists())
                datatexdir.mkpath(".");
            if(CreateNewPdfFile[pdffile]==true) {
                if(CreateNewTexFile[pdffile]==true) {
                    QFile texFile(filePath);
                    texFile.open(QIODevice::ReadWrite);
                    QTextStream writeContent(&texFile);
                    writeContent.flush();
                    writeContent << ContentsFromDatabase[pdffile];
                    texFile.close();
                }
                QString CurrentBuildCommand = BuildComPreamble[pdffile][0];
                QString Preamble = BuildComPreamble[pdffile][1];
                QStringList list = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                           .arg(Preamble)
                                                                           ,DataTex::DataTeX_Settings);
                if(list.count()==0){DataTex::CurrentPreamble_Content = "";}
                else{DataTex::CurrentPreamble_Content = list.at(0);}
                DataTex::CreateTexFile(filePath,0,"");
                DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],filePath
                                       ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
                DataTex::ClearOldFiles(filePath);
                qDebug()<<"run tex file";
            }
            else{
                QSqlQuery deleteQuery(currentBase);
                deleteQuery.exec(QString("DELETE FROM \"%2\" WHERE \"Id\" = \"%1\"").arg(pdffile,DataTable));
                qDebug()<<"delete entry from database";
            }
            fileSynced++;
            emit progress(fileSynced,FilesFound);
        }
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(2)->child(2)->childCount();i++){
            QString csvfile = ui->ResultsTreeWidget->topLevelItem(2)->child(2)->child(i)->text(0);
            QString filePath = ui->ResultsTreeWidget->topLevelItem(2)->child(2)->child(i)->text(3);
            QDir datatexdir(QFileInfo(filePath).absolutePath());
            if (!datatexdir.exists())
                datatexdir.mkpath(".");
            if(CreateNewCsvFile[csvfile]==true) {
                CsvFunctions::WriteDataToCSV(filePath,currentBase);
                qDebug()<<"create new csv file";
            }
            else{
                QSqlQuery deleteQuery(currentBase);
                deleteQuery.exec(QString("DELETE FROM \"%2\" WHERE \"Id\" = \"%1\"").arg(csvfile,DataTable));
                qDebug()<<"delete entry from database";
            }
            fileSynced++;
            emit progress(fileSynced,FilesFound);
        }
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
