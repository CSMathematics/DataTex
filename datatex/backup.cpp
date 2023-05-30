#include "backup.h"
#include "ui_backup.h"
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QDate>
#include "JlCompress.h"
#include "datatex.h"
#include <qmath.h>
#include "csvfunctions.h"
#include "sqlfunctions.h"
#include "filecommands.h"

BackUp::BackUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackUp)
{
    isBackUp = false;
    ui->setupUi(this);
    ui->BackUpRadioButton->setProperty("Text",tr("Select files to back up"));
    ui->RestoreRadioButton->setProperty("Text",tr("Select files to restore"));
    ui->BackUpFilesButton->setEnabled(false);
    ui->KeepFolderStructure->setEnabled(false);
    ui->UpdatePath->setEnabled(false);
    ui->OpenPath->setEnabled(false);
    foreach(QAbstractButton * bt,ui->BackUpGroup->buttons()){
        bt->setEnabled(false);
        connect(bt,&QAbstractButton::toggled,this,[=](){
            DatabaseSelected = ui->BackUpDatabase->isChecked();
            DBFileSelected = ui->BackUpDBFile->isChecked();
            LatexFilesSelected = ui->BackUpLatexFiles->isChecked();
            PdfSelected = ui->BackUpPdfFiles->isChecked();
            CsvSelected = ui->BackUpCsvFiles->isChecked();
            hasSelection = DatabaseSelected || DBFileSelected || LatexFilesSelected || PdfSelected || CsvSelected;
            ui->SelectPath->setEnabled(hasSelection);
            ui->KeepFolderStructure->setEnabled(hasSelection && isBackUp);
            ui->BackUpDatabase->setChecked(DBFileSelected && LatexFilesSelected && PdfSelected && CsvSelected);
        });
    }
    foreach(QAbstractButton * button,ui->buttonGroup->buttons()){
        button->setEnabled(false);
        connect(button,&QAbstractButton::toggled,this,[=](bool checked){
            isBackUp = button==ui->BackUpRadioButton;
            ui->SelectionLabel->setText(button->property("Text").toString());
            ui->BackUpDatabase->setEnabled(checked && isBackUp);
            ui->BackUpDBFile->setEnabled(checked && isBackUp);
            ui->BackUpDBFile->setChecked(checked && !isBackUp);
            ui->BackUpLatexFiles->setEnabled(checked);
            ui->BackUpPdfFiles->setEnabled(checked);
            ui->BackUpCsvFiles->setEnabled(checked);
            foreach(QAbstractButton * bt,ui->BackUpGroup->buttons()){
                bt->setChecked(false);
            }
            ui->BackUpFilesButton->setText((isBackUp) ? tr("BackUp") : tr("Restore"));
            ui->SelectPath->setEnabled(false);
            ui->KeepFolderStructure->setEnabled(isBackUp && checked);
            ui->KeepFolderStructure->setChecked(!isBackUp && checked);
            ui->UpdatePath->setEnabled(!isBackUp && checked);
        });
    }

    for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++ ) {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(3,QFileInfo(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()).baseName());
        item->setText(2,DataTex::GlobalFilesDatabaseList.values().at(i).databaseName());
        ui->OpenDatabasesTreeWidget->topLevelItem(0)->addChild(item);
        QString DatabaseName = QFileInfo(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()).baseName();
        QStringList fileCount = SqlFunctions::Get_StringList_From_Query("SELECT COUNT(Id) FROM Database_Files",DataTex::GlobalFilesDatabaseList[DatabaseName]);
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
        QStringList fileCount = SqlFunctions::Get_StringList_From_Query("SELECT COUNT(Id) FROM Documents",DataTex::GlobalDocsDatabaseList[DatabaseName]);
        if(fileCount.count()){
            ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(i)->setText(1,fileCount.at(0)+" documents");
        }
        ui->OpenDatabasesTreeWidget->topLevelItem(1)->child(i)->setText(0,DataTex::GlobalDocsDatabaseListNames.values().at(i));
    }
    ui->OpenDatabasesTreeWidget->expandAll();
    ui->OpenDatabasesTreeWidget->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    connect(ui->BackUpDBFile,&QAbstractButton::toggled,this,[&](bool checked){
        ui->UpdatePath->setEnabled(checked);
    });
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
    connect(ui->OpenPath,&QPushButton::clicked,this,[&](){
        QDesktopServices::openUrl(QUrl("file:///"+destination));
    });
    connect(ui->UpdatePath,&QAbstractButton::clicked,this,[=](bool checked){UpdatePathSelected = checked;});
    connect(ui->AddDateToName,&QAbstractButton::clicked,this,[=](bool checked){
        folderName = (checked) ? databaseName+" (BackUp "+date+")" : databaseName;
    });
    connect(ui->KeepFolderStructure,&QAbstractButton::clicked,this,[=](bool checked){
        KeepFolderStructure = checked;
    });

}

BackUp::~BackUp()
{
    delete ui;
}

void BackUp::DBMissing(QString database)
{
    isBackUp = true;
    LatexFilesSelected = true;
    PdfSelected = true;
    CsvSelected = true;
    destination = QFileInfo(database).absolutePath();
    on_BackUpFilesButton_clicked();
}

void BackUp::on_BackUpFilesButton_clicked()
{
    TexFiles.clear();
    PdfFiles.clear();
    CsvFiles.clear();
    newDatabaseUpdated = false;
    BackUpPath = destination+QDir::separator()+folderName;
    QDir dir(BackUpPath);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    else{
        QMessageBox::warning(this,tr("Warning"),QString(tr("The target folder\n%1\nalready exists.").arg(BackUpPath)));
        return;
    }
    if(isBackUp){
        QString SearchPath = QFileInfo(databasePath).absolutePath();
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

        if(DBFileSelected){
            //Copy database file to the destination path
            QProcess *copyprocess = new QProcess;
            copyprocess->start("cp",QStringList()<<"-avr"<<databasePath << BackUpPath);
            copyprocess->waitForBytesWritten();
            copyprocess->waitForFinished(-1);
        }

        if(LatexFilesSelected){
            CopyFiles(TexFiles,tr("LaTeX Files"));
        }
        if(PdfSelected){
            CopyFiles(PdfFiles,tr("Pdf Files"));
        }
        if(CsvSelected){
            CopyFiles(CsvFiles,tr("Csv Files"));
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
            QDirIterator tex_list(BackUpPath, QStringList() << "*.tex",
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
                FileCommands::CreateTexFile(file,0,"");
                FileCommands::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],file
                                       ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
                FileCommands::ClearOldFiles(file);
            }
        }

        if(CsvSelected){
            if(!LatexFilesSelected){
                CreateTexFiles();
            }
            foreach(QString file,TexFiles){
                CsvFunctions::WriteDataToCSV(file,currentBase);
            }
        }

        QProcess *copyprocess = new QProcess;
        copyprocess->start("cp",QStringList()<<"-avr"<<databasePath << BackUpPath);
        copyprocess->waitForBytesWritten();
        copyprocess->waitForFinished(-1);

        if(UpdatePathSelected){
            QSqlDatabase newDatabase = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(databasePath).baseName()+"_new");
            newDatabase.setDatabaseName(BackUpPath+QDir::separator()+QFileInfo(databasePath).fileName());
            newDatabase.open();
            QSqlQuery UpdateNewDatabaseFile(newDatabase);
            foreach(QString file, TexFiles){
                UpdateNewDatabaseFile.exec(
                            QString("UPDATE Database_Files SET Path = \"%1\" WHERE Id = \"%2\"").
                            arg(file.replace(QFileInfo(databasePath).absolutePath(),BackUpPath)
                             ,QFileInfo(file).baseName()));
            }
            newDatabase.close();
        }
    }
    //Compress Database to zip
    if(ui->BackUpZipBase->isChecked()){
        QString zipfile = destination+QDir::separator()+folderName+".zip";
        JlCompress::compressDir(zipfile,destination+QDir::separator()+folderName);
    }
    ui->OpenPath->setEnabled(true);
}

void BackUp::on_OpenDatabasesTreeWidget_itemSelectionChanged()
{
    TexFiles.clear();
    PdfFiles.clear();
    CsvFiles.clear();
    newDatabaseUpdated = false;
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

    if(item->parent()){
        databaseName = item->text(0);
        databasePath = item->text(2);
        qDebug()<<databaseName<<databasePath;
        folderName = databaseName;
        if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==0){
            Table = "Metadata";
            rem = " files";
            ContentType = "FileContent";
            DataTable = "Database_Files";
            currentBase = DataTex::GlobalFilesDatabaseList[item->text(3)];
        }
        else if(ui->OpenDatabasesTreeWidget->currentIndex().parent().row()==1){
            Table = "DocMetadata";
            rem = " documents";
            ContentType = "Content";
            DataTable = "Documents";
            currentBase = DataTex::GlobalDocsDatabaseList[item->text(3)];
        }

        Database_FileTableFields = SqlFunctions::Get_StringList_From_Query(QString("SELECT Id FROM BackUp WHERE Table_Id = '%1'").arg(Table),currentBase);
        Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM BackUp WHERE Table_Id = \"%1\"").arg(Table),currentBase);
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
            qDebug()<<filePath;
            filePath = filePath.replace(QFileInfo(databasePath).absolutePath(),BackUpPath);
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

void BackUp::CopyFiles(QStringList &list, QString folder)
{
    QSqlDatabase newDatabase;
    if(DBFileSelected && UpdatePathSelected && !newDatabaseUpdated){
        newDatabase = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(databasePath).baseName()+"_new");
        newDatabase.setDatabaseName(BackUpPath+QDir::separator()+QFileInfo(databasePath).fileName());
        newDatabase.open();
    }
    foreach(QString file,list){
        QString newFile = file;
        if(!KeepFolderStructure){
            newFile.replace(QFileInfo(file).absolutePath(),BackUpPath+QDir::separator()+folder+QDir::separator());
        }
        else{
            newFile.replace(QFileInfo(databasePath).absolutePath(),BackUpPath);
        }
        QDir dir(QFileInfo(newFile).absolutePath());
        if (!dir.exists()){
            dir.mkpath(".");
        }
        QFile CopyFile(file);
        CopyFile.copy(newFile);
        if(DBFileSelected && UpdatePathSelected && !newDatabaseUpdated){
            QSqlQuery UpdateNewDatabaseFile(newDatabase);
            UpdateNewDatabaseFile.exec(
                        QString("UPDATE Database_Files SET Path = \"%1\" WHERE Id = \"%2\"").
                        arg(newFile.replace(QFileInfo(newFile).suffix(),".tex"),QFileInfo(file).baseName()));
        }
    }
    if(DBFileSelected && UpdatePathSelected && !newDatabaseUpdated){
        newDatabase.close();
        newDatabaseUpdated = true;
    }
}
