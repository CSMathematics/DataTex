#include "databasesync.h"
#include "ui_databasesync.h"
#include "sqlfunctions.h"
#include "datatex.h"
#include <qmath.h>
#include "switch.h"

DatabaseSync::DatabaseSync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseSync)
{
    ui->setupUi(this);
    //Load all file databases
    QStringList FilesDatabases = SqlFunctions::Get_StringList_From_Query("SELECT \"Path\" FROM \"DataBases\"",DataTex::DataTeX_Settings);
    QStringList FilesDatabasesNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"DataBases\"",DataTex::DataTeX_Settings);
    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    for (int i=0;i<FilesDatabases.count();i++ ) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,QFileInfo(FilesDatabases.at(i)).baseName());
        item->setText(2,FilesDatabases.at(i));
        ui->OpenDatabasesTreeWidget->topLevelItem(0)->addChild(item);
    }
    datalist.insert(QFileInfo(DataTex::CurrentDataBasePath).baseName(),DataTex::CurrentTexFilesDataBase);
    for (int i=0;i<FilesDatabases.count();i++) {
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(FilesDatabases.at(i)).baseName());
        if(FilesDatabases.at(i)!=DataTex::CurrentDataBasePath){
            datalist.insert(QFileInfo(FilesDatabases.at(i)).baseName(),database);
            datalist[QFileInfo(FilesDatabases.at(i)).baseName()].setDatabaseName(FilesDatabases.at(i));
            datalist[QFileInfo(FilesDatabases.at(i)).baseName()].open();
        }
//        qDebug()<<QFileInfo(FilesDatabases.at(i)).baseName()<<" is open "<<datalist[QFileInfo(FilesDatabases.at(i)).baseName()].isOpen();
        QStringList fileCount = SqlFunctions::Get_StringList_From_Query("SELECT COUNT(DISTINCT Id) FROM Database_Files",datalist[QFileInfo(FilesDatabases.at(i)).baseName()]);
        if(fileCount.count()){
            ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(i)->setText(1,fileCount.at(0)+" files");
        }
        ui->OpenDatabasesTreeWidget->topLevelItem(0)->child(i)->setText(0,FilesDatabasesNames.at(i));
        datalist[QFileInfo(FilesDatabases.at(i)).baseName()].close();
    }


    //Load all document databases
    QStringList DocumentDatabases = SqlFunctions::Get_StringList_From_Query("SELECT \"Path\" FROM \"Notes_Folders\"",DataTex::DataTeX_Settings);
    for (int i=0;i<DocumentDatabases.count();i++ ) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,QFileInfo(DocumentDatabases.at(i)).baseName());
        item->setText(2,DocumentDatabases.at(i));
        ui->OpenDatabasesTreeWidget->topLevelItem(1)->addChild(item);
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
    for (int c = 1; c < ui->MetadataDifferences->horizontalHeader()->count(); ++c)
    {
        ui->MetadataDifferences->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
    FoundFont.setBold(true);
    redBrush=(QColor(200, 20, 20 ));//Qt::red;
}

DatabaseSync::~DatabaseSync()
{
    delete ui;
//    for (int i=0;i<datalist.count();i++) {
//        if(datalist.keys().at(i)!=DataTex::CurrentDataBasePath){
//            datalist.values()[i].close();
//        }
//    }
}

void DatabaseSync::on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->ScanFiles->setEnabled(true);
    ui->StartSync->setEnabled(false);
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
    currentBase.close();
    currentBase = datalist[item->text(3)];
    currentBase.open();
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",currentBase);
    TotalFiles = item->text(1).remove(" files").toInt();
    CheckMetadata("/home/spyros/Νέα βάση 5_8/Τομέας 1/Ενότητες/Ενότητα 1/Theorems/DTX-f1-Theorem-Th1.tex",currentBase);
    qDebug()<<currentBase;
}


void DatabaseSync::on_ResultsTreeWidget_itemSelectionChanged()
{
    ui->MetadataDifferences->setRowCount(0);
    QModelIndex ix = ui->ResultsTreeWidget->currentIndex();
    QModelIndex index = ui->ResultsTreeWidget->currentIndex();
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
    }

    if(topLevel==1 && index.parent().isValid()){
        ui->ResultsTreeWidget->setHeaderLabels({"File","Sync data to csv","Sync data to database"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        for(int i=0;i<DifferencesInDatabase[baseName].count();i++){
            ui->MetadataDifferences->insertRow(i);
            ui->MetadataDifferences->setItem(i,2, new QTableWidgetItem(DifferencesInCSV[baseName].at(i)));
            ui->MetadataDifferences->setItem(i,1, new QTableWidgetItem(DifferencesInDatabase[baseName].at(i)));
            ui->MetadataDifferences->setItem(i,0, new QTableWidgetItem(DatabaseFieldsWithDifferences[baseName].at(i)));
        }
    }

    if(topLevel==2 && index.parent().isValid()){
        ui->ResultsTreeWidget->setHeaderLabels({"File","Create missing files","Delete database entry"});
        QString baseName = ui->ResultsTreeWidget->currentItem()->text(0);
        QStringList data;
        QSqlQuery dataQuery(currentBase);
        dataQuery.exec(QString("SELECT Id,BuildCommand,Preamble,Date,Path,FileContent FROM Database_Files WHERE Id = \"%1\"").arg(baseName));
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
        for(int i=0;i<DifferencesInDatabase[baseName].count();i++){
            ui->MetadataDifferences->insertRow(i);
            ui->MetadataDifferences->setItem(i,1, new QTableWidgetItem(DifferencesInCSV[baseName].at(i)));
            ui->MetadataDifferences->setItem(i,2, new QTableWidgetItem(DifferencesInDatabase[baseName].at(i)));
            ui->MetadataDifferences->setItem(i,0, new QTableWidgetItem(DatabaseFieldsWithDifferences[baseName].at(i)));
        }
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
    QStringList FilePaths = SqlFunctions::ComboList_Single("SELECT DISTINCT Path,FileContent FROM Database_Files",currentBase,"").at(0);
    QStringList FileContents = SqlFunctions::ComboList_Single("SELECT DISTINCT Path,FileContent FROM Database_Files",currentBase,"").at(1);
    for(int i=0;i<FilePaths.count();i++){
        QString baseName = QFileInfo(FilePaths[i]).baseName();
        ContentsFromDatabase.insert(baseName,FileContents[i]);
        QFile file(FilePaths[i]);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        text.flush();
        QString FileContent = text.readAll();
        file.close();
        ContentsFromFiles.insert(baseName,FileContent);
        int value = ceil(((i+1)*100)/TotalFiles);
        //Check differences in the content
        if(filesync){
            if(ContentsFromDatabase[baseName]!=FileContent){
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(0)->addChild(item);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                SyncContentToDatabase.insert(baseName,true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    SyncContentToDatabase.insert(baseName,sw1->isChecked());
                    qDebug()<<SyncContentToDatabase[baseName];
                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    SyncContentToDatabase.insert(baseName,sw1->isChecked());
                    qDebug()<<SyncContentToDatabase[baseName];
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
            if(CheckMetadata(FilePaths[i],currentBase)){
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(1)->addChild(item);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                SyncMetadataToFile.insert(baseName,true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    SyncMetadataToFile.insert(baseName,sw1->isChecked());
                    qDebug()<<SyncMetadataToFile[baseName];
                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    SyncMetadataToFile.insert(baseName,sw1->isChecked());
                    qDebug()<<SyncMetadataToFile[baseName];
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
                TexFilesMissing.insert(baseName,FilePaths[i]);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(0)->addChild(item);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewTexFile.insert(baseName,sw1->isChecked());

                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewTexFile.insert(baseName,sw1->isChecked());

                });
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
            }
            if(!QFileInfo::exists(PdfFile)){
                PdfFilesMissing.insert(baseName,PdfFile);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(1)->addChild(item);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewPdfFile.insert(baseName,sw1->isChecked());

                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewPdfFile.insert(baseName,sw1->isChecked());

                });
                ui->ResultsTreeWidget->setItemWidget(item, 1, sw1);
                ui->ResultsTreeWidget->setItemWidget(item, 2, sw2);
            }
            if(!QFileInfo::exists(CsvFile)){
                CsvFilesMissing.insert(baseName,CsvFile);
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,baseName);
                item->setText(3,FilePaths[i]);
                ui->ResultsTreeWidget->topLevelItem(2)->child(2)->addChild(item);
                Switch * sw1 = new Switch(this);
                Switch * sw2 = new Switch(this);
                sw1->setChecked(true);
                connect(sw1,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw1,sw2);
                    CreateNewCsvFile.insert(baseName,sw1->isChecked());

                });
                connect(sw2,&QAbstractButton::clicked,this,[=](){
                    ExclusiveSwitches(sw2,sw1);
                    CreateNewCsvFile.insert(baseName,sw1->isChecked());

                });
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
    bool hasDifferences = false;
    QSqlQuery RowValues(database);
    QStringList metadataFromDatabase;
    QStringList metadataFromCSV;
    RowValues.exec(SqlFunctions::SelestExerciseRow.arg(QFileInfo(file).baseName()));
    while (RowValues.next())
    {
        QSqlRecord record = RowValues.record();
        for(int i=0; i < record.count(); i++)
        {
            metadataFromDatabase << record.value(i).toString();
        }
    }

    QString csvFile = file;
    QString baseName = QFileInfo(file).baseName();
    csvFile.replace(".tex",".csv");
    QFile CSV(csvFile);
    CSV.open (QIODevice::ReadOnly | QIODevice::Text);
    QTextStream Line(&CSV);
    while (!Line.atEnd()){
        QString LineText=Line.readLine(); // reads line from file
        const QStringList fields = LineText.split(","); // appends first column to list, ',' is separator
        metadataFromCSV.append(QString(fields.at(1)).replace("\\n","\n"));
    }
    CSV.close();

    QList<QStringList> compResults = CompareLists(metadataFromCSV,metadataFromDatabase);
    DifferencesInCSV.insert(baseName,compResults[0]);
    DifferencesInDatabase.insert(baseName,compResults[1]);
    DatabaseFieldsWithDifferences.insert(baseName,compResults[2]);
    if(DifferencesInDatabase[baseName].count()>0){
        hasDifferences = true;
    }
    return hasDifferences;
}

QList<QStringList> DatabaseSync::CompareLists(QStringList list1, QStringList list2)
{
    QList<int> diff_positions;
    QStringList diffInList1;
    QStringList diffInList2;
    QStringList FieldList;
    int minSize = list1.size() < list2.size() ? list1.size() : list2.size();
    for (int pos = 0; pos < minSize; ++pos)
    {
        if (list1.at(pos) != list2.at(pos)){
            diff_positions.append(pos);
        }
    }
    foreach(int i,diff_positions) {
//        qDebug()<< "Οι διαφορές είναι "+list1.at(i)+" και "+list2.at(i);
        diffInList1.append(list1.at(i));
        diffInList2.append(list2.at(i));
        FieldList.append(Database_FileTableFieldNames.at(i));
    }
    QList<QStringList> differences = {diffInList1,diffInList2,FieldList};
    return differences;
}

void DatabaseSync::on_StartSync_clicked()
{
    int content = 0;
    int metadata = 0;
    int filesmissing = 0;
    int texmissing = 0;
    int pdfmissing = 0;
    int csvmissing = 0;
    QSqlQuery WriteContent(currentBase);
    if(filesync){
        content = ui->ResultsTreeWidget->topLevelItem(0)->childCount();
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(0)->childCount();i++){
            QString filePath= ui->ResultsTreeWidget->topLevelItem(0)->child(i)->text(3);
            QString file = ui->ResultsTreeWidget->topLevelItem(0)->child(i)->text(0);
            if(SyncContentToDatabase[file]==true) {
                WriteContent.exec(QString("UPDATE Database_Files SET FileContent = \"%1\" WHERE Id = \"%2\"").arg(ContentsFromFiles[file],file));
            }
            else{
                QFile texFile(filePath);
                texFile.open(QIODevice::ReadWrite);
                QTextStream writeContent(&texFile);
                writeContent.flush();
                writeContent << ContentsFromDatabase[file];
                texFile.close();
            }
        }
    }
    if(metacheck){
        metadata = ui->ResultsTreeWidget->topLevelItem(1)->childCount();
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(1)->childCount();i++){
            QString filePath = ui->ResultsTreeWidget->topLevelItem(1)->child(i)->text(3);
            QString file = ui->ResultsTreeWidget->topLevelItem(1)->child(i)->text(0);
            if(SyncMetadataToFile[file]==true) {
                SyncMetadataToCsvFile(filePath);
            }
            else{qDebug()<<"false"<<file;}
        }
    }
    if(missfiles){
        texmissing = ui->ResultsTreeWidget->topLevelItem(2)->child(0)->childCount();
        pdfmissing = ui->ResultsTreeWidget->topLevelItem(2)->child(1)->childCount();
        csvmissing = ui->ResultsTreeWidget->topLevelItem(2)->child(2)->childCount();
        filesmissing = texmissing+pdfmissing+csvmissing;
        for(int i=0;i<ui->ResultsTreeWidget->topLevelItem(2)->child(0)->childCount();i++){
            QString texfile = ui->ResultsTreeWidget->topLevelItem(2)->child(0)->child(i)->text(0);
            if(CreateNewTexFile[texfile]==true) {
                qDebug()<<"create new tex file";
            }
            else{qDebug()<<"delete entry from database";}
        }
    }
    int total = content+metadata+filesmissing;
    ui->SyncProgressBar->setValue(total);
}

void DatabaseSync::SyncMetadataToCsvFile(QString file)
{
    QStringList metadataFromDatabase;
    QSqlQuery RowValues(currentBase);
    RowValues.exec(SqlFunctions::SelestExerciseRow.arg(QFileInfo(file).baseName()));
    while (RowValues.next())
    {
        QSqlRecord record = RowValues.record();
        for(int i=0; i < record.count(); i++)
        {
            metadataFromDatabase << record.value(i).toString();
        }
    }

    QString csvFile = file;
    QString newMetadata;
    csvFile.replace(".tex",".csv");
//    metadataFromDatabase[11] = "\""+metadataFromDatabase[11]+"\"";
    for (int i=0;i<metadataFromDatabase.count();i++) {
        newMetadata += Database_FileTableFields[i]+","+metadataFromDatabase[i]+"\n";
    }
//    QFile CSV(csvFile);
//    CSV.open (QIODevice::ReadWrite | QIODevice::Text);
//    CSV.resize(0);
//    QTextStream Content(&CSV);
//    Content << newMetadata;
//    CSV.close();
    qDebug()<<metadataFromDatabase[11];
}
