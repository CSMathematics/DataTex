#include "backup.h"
#include "ui_backup.h"
#include "datatex.h"
#include <qmath.h>
#include "sqlfunctions.h"
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QDate>
#include "JlCompress.h"

BackUp::BackUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackUp)
{
    isBackUp = false;
    ui->setupUi(this);
    ui->BackUpRadioButton->setProperty("Text",tr("Select files to back up"));
    ui->RestoreRadioButton->setProperty("Text",tr("Select files to restore"));
    ui->BackUpFilesButton->setEnabled(false);
    foreach(QAbstractButton * bt,ui->BackUpGroup->buttons()){
        bt->setEnabled(false);
        connect(bt,&QAbstractButton::clicked,this,[=](){
            DatabaseSelected = ui->BackUpDatabase->isChecked();
            DBFileSelected = ui->BackUpDBFile->isChecked();
            LatexFilesSelected = ui->BackUpLatexFiles->isChecked();
            PdfSelected = ui->BackUpPdfFiles->isChecked();
            CsvSelected = ui->BackUpCsvFiles->isChecked();
            hasSelection  = DatabaseSelected || DBFileSelected || LatexFilesSelected || PdfSelected || CsvSelected;
            ui->SelectPath->setEnabled(hasSelection);
        });
    }
    foreach(QAbstractButton * button,ui->buttonGroup->buttons()){
        button->setEnabled(false);
        connect(button,&QAbstractButton::toggled,this,[=](bool checked){
            isBackUp = button==ui->BackUpRadioButton;
            ui->SelectionLabel->setText(button->property("Text").toString());
            ui->BackUpDatabase->setEnabled(checked && isBackUp);
            ui->BackUpDBFile->setEnabled(checked && isBackUp);
            ui->BackUpLatexFiles->setEnabled(checked);
            ui->BackUpPdfFiles->setEnabled(checked);
            ui->BackUpCsvFiles->setEnabled(checked);
            foreach(QAbstractButton * bt,ui->BackUpGroup->buttons()){
                bt->setChecked(false);
            }
            ui->BackUpDBFile->setEnabled(isBackUp && checked);
            ui->BackUpDatabase->setEnabled(isBackUp && checked);
            ui->BackUpFilesButton->setText((isBackUp) ? tr("BackUp") : tr("Restore"));
            ui->SelectPath->setEnabled(false);
        });
    }

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

    connect(ui->BackUpLatexFiles,&QAbstractButton::toggled,this,[&](bool checked){LatexFilesSelected = checked;});
    connect(ui->BackUpPdfFiles,&QAbstractButton::toggled,this,[&](bool checked){PdfSelected = checked;});
    connect(ui->BackUpCsvFiles,&QAbstractButton::toggled,this,[&](bool checked){CsvSelected = checked;});
    connect(ui->BackUpDatabase,&QAbstractButton::toggled,this,[&](bool checked){
        ui->BackUpLatexFiles->setChecked(checked);
        ui->BackUpPdfFiles->setChecked(checked);
        ui->BackUpCsvFiles->setChecked(checked);
        ui->BackUpDBFile->setChecked(checked);
        ui->BackUpLatexFiles->setEnabled(!checked);
        ui->BackUpPdfFiles->setEnabled(!checked);
        ui->BackUpCsvFiles->setEnabled(!checked);
        ui->BackUpDBFile->setEnabled(!checked);
    });
    date = QString::number( QDate::currentDate().day() )+"-"+QString::number( QDate::currentDate().month() )
    +"-"+QString::number( QDate::currentDate().year() );
}

BackUp::~BackUp()
{
    delete ui;
}

void BackUp::on_BackUpFilesButton_clicked()
{
    TexFiles.clear();
    PdfFiles.clear();
    CsvFiles.clear();

    if(isBackUp){
        QString SearchPath = QFileInfo(basepathbackup).absolutePath();
        if(LatexFilesSelected){
            QDirIterator tex_list(SearchPath , QStringList() << "*.tex", QDir::Files, QDirIterator::Subdirectories);
            while (tex_list.hasNext()){
            TexFiles.append(tex_list.next());}
        }
        if(PdfSelected){
            QDirIterator pdf_list(SearchPath , QStringList() << "*.pdf", QDir::Files, QDirIterator::Subdirectories);
            while (pdf_list.hasNext()){
            PdfFiles.append(pdf_list.next());}
        }
        if(CsvSelected){
            QDirIterator csv_list(SearchPath , QStringList() << "*.csv", QDir::Files, QDirIterator::Subdirectories);
            while (csv_list.hasNext()){
            CsvFiles.append(csv_list.next());}
        }
        QString rembase = basepathbackup;
        QString RemovePath = rembase.remove(basenamebackup+QDir::separator());
        QString path;

        QString BackUpPath = destination+QDir::separator()+basenamebackup+" (BackUp "+date+")";
        QString CopySource;
        QString CopyDestination;
        if(DBFileSelected && !DatabaseSelected){
            //Copy database file to the destination path
            CopySource = basepathbackup;
            CopyDestination = BackUpPath;
            QDir dir(BackUpPath);
            if (!dir.exists()){
                dir.mkpath(".");
            }
         }
        else if(DatabaseSelected){
            //Copy the entire database folder to the destination path
            CopySource = QFileInfo(basepathbackup).absolutePath();
            CopyDestination = destination+QDir::separator();
        }

        QProcess *copyprocess = new QProcess;
        copyprocess->start("cp",QStringList()<<"-avr"<<CopySource << CopyDestination);
        copyprocess->waitForBytesWritten();
        copyprocess->waitForFinished(-1);

        if(LatexFilesSelected && !DatabaseSelected){
        QDir dir(BackUpPath+QDir::separator()+tr("LaTeX Files")+QDir::separator());
        if (!dir.exists())
            dir.mkpath(".");
            foreach(QString file,TexFiles){
                QFileInfo texfile(file);
                path = texfile.absoluteFilePath().remove(RemovePath);
                QFile newfile(file);
                newfile.copy(BackUpPath+QDir::separator()+tr("LaTeX Files")+QDir::separator()+texfile.fileName());
            }
        }

        if(PdfSelected && !DatabaseSelected){
        QDir dir(BackUpPath+QDir::separator()+tr("Pdf Files")+QDir::separator());
        if (!dir.exists())
            dir.mkpath(".");
            foreach(QString file,PdfFiles){
            QFileInfo pdffile(file);
            path = pdffile.absoluteFilePath().remove(RemovePath);
            QFile newfile(file);
            newfile.copy(BackUpPath+QDir::separator()+tr("Pdf Files")+QDir::separator()+pdffile.fileName());
            }
        }

        if(CsvSelected && !DatabaseSelected){
        QDir dir(BackUpPath+QDir::separator()+tr("Csv Files")+QDir::separator());
        if (!dir.exists())
            dir.mkpath(".");
            foreach(QString file,CsvFiles){
            QFileInfo csvfile(file);
            path = csvfile.absoluteFilePath().remove(RemovePath);
            QFile newfile(file);
            newfile.copy(BackUpPath+QDir::separator()+tr("Csv Files")+QDir::separator()+csvfile.fileName());
            }
        }
        //Compress Database to zip
        if(ui->BackUpZipBase->isChecked()){
            QString zipfile = BackUpPath+".zip";
            JlCompress::compressDir(zipfile,(!DatabaseSelected) ? BackUpPath : destination+QDir::separator()+basenamebackup);
        }
    }
    else{
        QStringList FilePaths = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(0);
        QStringList FileContents = SqlFunctions::ComboList_Single(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase,"").at(1);
        for(int i=0;i<FilePaths.count();i++){
            QString baseName = QFileInfo(FilePaths[i]).baseName();
            ContentsFromDatabase.insert(baseName,FileContents[i]);
        }
        TexFiles = SqlFunctions::Get_StringList_From_Query(QString("SELECT DISTINCT Path,%1 FROM %2").arg(ContentType,DataTable),currentBase);
        if(LatexFilesSelected){
            CreateTexFiles();
        }
        if(PdfSelected){
            if(!LatexFilesSelected){
                CreateTexFiles();
            }
            QStringList list;
            QDirIterator tex_list(destination+QDir::separator()+basenamebackup, QStringList() << "*.tex",
                                  QDir::Files, QDirIterator::Subdirectories);
            while (tex_list.hasNext()){
            list.append(tex_list.next());}
            foreach(QString file,list){
                QStringList data;
                QSqlQuery dataQuery(currentBase);
                dataQuery.exec(QString("SELECT BuildCommand,Preamble FROM %2 WHERE Id = \"%1\"")
                               .arg(QFileInfo(file).baseName(),DataTable));
                while (dataQuery.next())
                {
                    QSqlRecord record = dataQuery.record();
                    for(int i=0; i < record.count(); i++)
                    {
                        data << record.value(i).toString();
                    }
                }
                BuildComPreamble.insert(QFileInfo(file).baseName(),data);
                QString CurrentBuildCommand = BuildComPreamble[QFileInfo(file).baseName()][0];
                QString Preamble = BuildComPreamble[QFileInfo(file).baseName()][1];
                QStringList clist = SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
                                                                           .arg(Preamble)
                                                                           ,DataTex::DataTeX_Settings);
                if(clist.count()==0){DataTex::CurrentPreamble_Content = "";}
                else{DataTex::CurrentPreamble_Content = clist.at(0);}
                DataTex::CreateTexFile(file,0,"");
                DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],file
                                       ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
                DataTex::ClearOldFiles(file);
            }
        }

        if(CsvSelected){
            if(!LatexFilesSelected){
                CreateTexFiles();
            }
            foreach(QString file,TexFiles){
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
                metadataFromDatabase.swapItemsAt(metadataFromDatabase.count()-1,Section);
                metadataFromDatabase.removeAt(metadataFromDatabase.count()-1);
                QString csvFile = file;
                QString newMetadata;
                csvFile = csvFile.replace(QFileInfo(basepathbackup).absolutePath(),destination+QDir::separator()+basenamebackup);
                csvFile.replace(".tex",".csv");
                QTextStream contentline(&metadataFromDatabase[FileContent]);// = metadataFromDatabase[FileContent].replace("\n","\\n");
                QStringList Line;
                while(!contentline.atEnd()){
                    Line.append(contentline.readLine());
                }
                metadataFromDatabase[FileContent]=Line.join("\\qt_endl");
                QTextStream bibcontentline(&metadataFromDatabase[FileData::Bibliography]);// = metadataFromDatabase[FileContent].replace("\n","\\n");
                QStringList bibLine;
                while(!bibcontentline.atEnd()){
                    bibLine.append(bibcontentline.readLine());
                }
                metadataFromDatabase[FileData::Bibliography]=bibLine.join("\\qt_endl");
                for (int i=0;i<metadataFromDatabase.count();i++) {
                    newMetadata += Database_FileTableFields[i]+","+metadataFromDatabase[i]+"\n";
                }
                QFile CSV(csvFile);
                CSV.open (QIODevice::ReadWrite | QIODevice::Text);
                CSV.resize(0);
                QTextStream Content(&CSV);
                Content << newMetadata;
                CSV.close();
            }
        }

        QProcess *copyprocess = new QProcess;
        copyprocess->start("cp",QStringList()<<"-avr"<<basepathbackup << destination+QDir::separator()+basenamebackup);
        copyprocess->waitForBytesWritten();
        copyprocess->waitForFinished(-1);
    }
    if(ui->OpenFolder->isChecked()){QDesktopServices::openUrl(QUrl("file:///"+destination));}
}

void BackUp::on_OpenDatabasesTreeWidget_itemSelectionChanged()
{
    TexFiles.clear();
    PdfFiles.clear();
    CsvFiles.clear();
    ui->BackUpZipBase->setChecked(false);
    ui->buttonGroup->setExclusive(false);
    foreach(QAbstractButton * button,ui->buttonGroup->buttons()){
        button->setEnabled(true);
        button->setChecked(false);
    }
    ui->buttonGroup->setExclusive(true);
    ui->BackUpFilesButton->setEnabled(false);
    ui->SelectPath->setEnabled(false);

    QTreeWidgetItem * item = ui->OpenDatabasesTreeWidget->currentItem();
    QVariant dataType = item->parent()->data(0,Qt::DisplayRole).toString();

    if(item->parent()){
        basenamebackup = item->text(0);
        basepathbackup = item->text(2);
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

        Database_FileTableFields = SqlFunctions::Get_StringList_From_Query(QString("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = '%1'").arg(Table),currentBase);
        Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query(QString("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = \"%1\"").arg(Table),currentBase);
    }
}


void BackUp::on_SelectPath_clicked()
{
    destination = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(destination.isEmpty()) return;
    ui->DestinationLine->setText(destination);
    ui->BackUpFilesButton->setEnabled(true);
}

void BackUp::CreateTexFiles()
{
    if(TexFiles.count()>0){
        foreach(QString filePath,TexFiles){
            filePath = filePath.replace(QFileInfo(basepathbackup).absolutePath(),destination+QDir::separator()+basenamebackup);
            QDir dir(QFileInfo(filePath).absolutePath());
            if (!dir.exists())
                dir.mkpath(".");
            QFile texFile(filePath);
            texFile.open(QIODevice::ReadWrite);
            QTextStream writeContent(&texFile);
            writeContent.flush();
            writeContent << ContentsFromDatabase[QFileInfo(filePath).baseName()];
            texFile.close();
        }
    }
}
