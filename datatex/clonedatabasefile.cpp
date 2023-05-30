#include "clonedatabasefile.h"
#include "ui_clonedatabasefile.h"

CloneDatabaseFile::CloneDatabaseFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloneDatabaseFile)
{
    ui->setupUi(this);
    foreach (QSqlDatabase database, DataTex::GlobalFilesDatabaseList.values()) {
        if(database.databaseName() != DataTex::CurrentDataBasePath){
            QString databaseName = QFileInfo(database.databaseName()).baseName();
            ui->FilesDatabasesCombo->addItem(DataTex::GlobalFilesDatabaseListNames[databaseName],QVariant::fromValue(database));
        }
    }
    SourceDatabase = ui->FilesDatabasesCombo->currentData().value<QSqlDatabase>();//DataTex::GlobalFilesDatabaseList[QFileInfo(ui->FilesDatabasesCombo->currentData().toString()).baseName()];
    SourceDatabaseName = DataTex::GlobalFilesDatabaseListNames[QFileInfo(DataTex::CurrentDataBasePath).baseName()];
    ui->FilesDatabasesCombo->setCurrentText(SourceDatabaseName);
    ui->removeButton->setEnabled(false);
    ui->addEverything->setEnabled(false);
    setWindowTitle(tr("Import files to database : ")+SourceDatabaseName);
    qDebug()<<SourceDatabase.databaseName();
    view = new QPdfViewer(this);
    ui->verticalLayout_6->addWidget(view);
    view->show();
    FileView = new QPdfViewer(this);
    ui->verticalLayout_4->addWidget(FileView);
    FileView->show();
    CreateCustomTagWidget(SourceDatabase);
    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,3,0,1,1);
    FilesTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    FilesTable->setAlternatingRowColors(true);
    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    LoadDatabaseFiles(SourceDatabase);
    DataTex::StretchColumns(FilesTable,1.5);
//    ui->FilesDatabasesCombo->setEnabled(false);

    connect(FilesTable,&QTableView::doubleClicked,this,[=](){
        int row = FilesTable->currentIndex().row();
        AddFiles(row);
    });
    FileCommands::ShowPdfInViewer(CurrentDatabaseFile,FileView);

    ui->FilesTabWidget->setCurrentIndex(0);
    ui->splitter->setSizes(QList<int>{300,150});
    //right-click menu
    RightClick = new RightClickMenu(this,FilesTable,3);
    //---
    FilesTable->setColumnHidden(FilesTable->model()->columnCount()-1,true);
    FilesTable->setColumnHidden(FilesTable->model()->columnCount()-2,true);
    FilesTable->setColumnHidden(FilesTable->model()->columnCount()-3,true);

    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, [&](){ui->ClearFiltersFD->setEnabled(true);});
    ui->ClearFiltersFD->setEnabled(false);
    connect(ui->ClearFiltersFD, &QPushButton::clicked, this, [=](){
        for (int i=0;i<FilesTable->model()->columnCount();i++) {
            FilesTable->filterHeader()->clearFilters();
            ui->ClearFiltersFD->setEnabled(false);
        }
    });
    connect(ui->EnableSortingFiles,&QPushButton::toggled,this,[&](bool checked){
        filesSorting = checked;
        QStringList list;
        for(int i= 0;i<FilesTable->model()->columnCount();i++){
            list.append(FilesTable->filterHeader()->filterValue(i));
        }
        updateFilter(list);
        FilesTable->filterHeader()->adjustPositions();
    });
    connect(ui->addButton,&QPushButton::clicked,this,[&](){
        int row = FilesTable->currentIndex().row();
        AddFiles(row);
    });
    DestinationDatabase = DataTex::CurrentTexFilesDataBase;
    ui->ImportButton->setChecked(true);
    connect(ui->ImportButton,&QPushButton::toggled,this,[&](bool checked){
        int filesSelected = ui->SelectedFiles->model()->rowCount();
        isImportMode = checked;
        ui->FilesDatabasesCombo->setEnabled(checked);
        if(filesSelected!=0){
            QMessageBox::StandardButton resBtn = QMessageBox::warning(this,tr("Discard selections"),tr("Any selected files will be removed.\nDo ypu wish to procceed?"),QMessageBox::StandardButton::No| QMessageBox::Yes,QMessageBox::No);
            if (resBtn == QMessageBox::No) {
                return;
            }
            else{
                SelectedDatabaseList.clear();
                foreach (DBFileInfo * fileInfo, FileList) {
                    FileList.removeOne(fileInfo);
                    delete fileInfo;
                }
                foreach (DBFileInfo * fileInfo, SolutionsList) {
                    SolutionsList.removeOne(fileInfo);
                    delete fileInfo;
                }
//                foreach (auto item, ui->SelectedFiles->) {
//                    SolutionsList.removeOne(fileInfo);
//                    delete fileInfo;
//                }
                SelectedFileList.clear();
                ui->SelectedFiles->clear();
            }
        }
        else{

        }
        if(checked){
            SourceDatabase = ui->FilesDatabasesCombo->currentData().value<QSqlDatabase>();
            DestinationDatabase = DataTex::CurrentTexFilesDataBase;
        }
        else
        {
            SourceDatabase = DataTex::CurrentTexFilesDataBase;
        }
        LoadDatabaseFiles(SourceDatabase);
    });

//    connect(ui->warningButton,&QPushButton::clicked,this,[&](){
//        QDialog *d = new QDialog(this);
//        d->setWindowTitle(tr("Metadata resemblences"));
//        QVBoxLayout * v = new QVBoxLayout(this);
////        QVBoxLayout * files = new QVBoxLayout(this);
////        QVBoxLayout * documents = new QVBoxLayout(this);
//        QLabel * message = new QLabel(tr("The following databases are encrypted.\nTo open them please enter the username and the password."));
//        v->addWidget(message);
//        QHBoxLayout * h2 = new QHBoxLayout(this);
//        QLabel * warning = new QLabel(this);
////        QLabel * fl = new QLabel(tr("Files databases"));
////        QLabel * dl = new QLabel(tr("Documents databases"));
//        QDialogButtonBox *dbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,this);
//        h2->addWidget(warning);
//        h2->addWidget(dbox);
//        v->addLayout(h2);
//        d->setLayout(v);
//        d->show();
//    });
}

CloneDatabaseFile::~CloneDatabaseFile()
{
    delete ui;
    delete view;
    delete FileView;
    delete tableModel;
    delete FilesTable;
    delete RightClick;
    delete filesTagLine;
    delete FilesProxyModel;
    delete FilesModel;
}

void CloneDatabaseFile::LoadDatabaseFiles(QSqlDatabase database)
{
    FilesTable->filterTable(SqlFunctions::ShowAllDatabaseFiles,database,filesSorting);
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &CloneDatabaseFile::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &CloneDatabaseFile::updateFilter);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    FilesTable->filterHeader()->adjustPositions();
}

void CloneDatabaseFile::updateFilter(QStringList values)
{
    SqlFunctions::FilterTable(Database_FileTableFields,values);
    int columns = Database_FileTableFields.count();
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,SourceDatabase,this);
    FilesTable->filterTable(SqlFunctions::FilesTable_UpdateQuery,SourceDatabase,filesSorting);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &CloneDatabaseFile::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void CloneDatabaseFile::FilesTable_selectionchanged()
{
    int file_count = FilesTable->selectionModel()->selectedRows().count();
    ui->addEverything->setEnabled(file_count>1);
    ui->CountLabel->setText(QString::number(file_count));
    int row  = FilesTable->selectionModel()->currentIndex().row();
    PreviewFile = FilesTable->model()->data(FilesTable->model()->index(row,7)).toString();
    QString fileContent = FilesTable->model()->data(FilesTable->model()->index(row,11)).toString();
    //    QString preamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    QString buildCommand = FilesTable->model()->data(FilesTable->model()->index(row,13)).toString();
    QString file = PreviewFile;
    QString pdffile = file.replace(".tex",".pdf");
    if(!QFileInfo::exists(pdffile)){
        //        FileCommands::CreateTexFile(PreviewFile,0,""/*preamble*/);
        //        FileCommands::BuildDocument(DataTex::LatexCommands[buildCommand],PreviewFile,DataTex::LatexCommandsArguments[buildCommand],".tex");
        //        FileCommands::ClearOldFiles(PreviewFile);
        //Need to add a 'preamble' variable in CreateTexFile command
    }
    FileCommands::ShowPdfInViewer(PreviewFile,view);
    ui->FileContent->setText(fileContent);
}

void CloneDatabaseFile::on_Okbutton_rejected()
{
    reject();
}

void CloneDatabaseFile::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                                                               tr("Close window"),tr("Do you want to close the window?"),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void CloneDatabaseFile::AddFiles(int row)
{
    QString Id = FilesTable->model()->data(FilesTable->model()->index(row,0)).toString();
    QString FileType = FilesTable->model()->data(FilesTable->model()->index(row,1)).toString();
    QString Field = FilesTable->model()->data(FilesTable->model()->index(row,2)).toString();
    QStringList Chapters = FilesTable->model()->data(FilesTable->model()->index(row,3)).toString().split("|");
    QStringList Sections = FilesTable->model()->data(FilesTable->model()->index(row,4)).toString().split("|");
    QStringList SubSections = FilesTable->model()->data(FilesTable->model()->index(row,5)).toString().split("|");
    QString Difficulty = FilesTable->model()->data(FilesTable->model()->index(row,6)).toString();
    QString Path = FilesTable->model()->data(FilesTable->model()->index(row,7)).toString();
    QString Date = FilesTable->model()->data(FilesTable->model()->index(row,8)).toString();
    QString Solved = FilesTable->model()->data(FilesTable->model()->index(row,9)).toString();
    QString Bibliography = FilesTable->model()->data(FilesTable->model()->index(row,10)).toString();
    QString Content = FilesTable->model()->data(FilesTable->model()->index(row,11)).toString();
    QString Preamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    QString Build = FilesTable->model()->data(FilesTable->model()->index(row,13)).toString();
    QString Description = FilesTable->model()->data(FilesTable->model()->index(row,14)).toString();
    QString databaseName = ui->FilesDatabasesCombo->currentText();
    QStringList list = {Chapters.join("|"),Sections.join("|"),SubSections.join("|")};
    QFont bold(ui->SelectedFiles->font());
    bold.setBold(true);
    if(!SelectedDatabaseList.contains(databaseName)){
        QTreeWidgetItem * item = new QTreeWidgetItem({databaseName});
        item->setFont(0,bold);
        ui->SelectedFiles->addTopLevelItem(item);
    }
    SelectedDatabaseList.insert(databaseName);
    DBFileInfo *FileData = new DBFileInfo;
    FileData->Id = Id;
    FileData->FileType<<SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM FileTypes WHERE FileType = \'%1\'").arg(FileType),SourceDatabase);
    FileData->Field<<SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM Fields WHERE Name = \'%1\'").arg(Field),SourceDatabase);
    FileData->Chapters<<FileData->setRecordList(SqlFunctions::GetChapterInfo.arg(Id),SourceDatabase);
    FileData->Sections<<FileData->setRecordList(SqlFunctions::GetSectionInfo.arg(Id),SourceDatabase);
    FileData->SubSections<<FileData->setRecordList(SqlFunctions::GetSubsectionInfo.arg(Id),SourceDatabase);
    FileData->Difficulty = Difficulty.toInt();
    FileData->Date = QDateTime::fromString(Date,"dd/M/yyyy hh:mm");
    FileData->Solved = (Solved == tr("YES")) ? true : false;
    FileData->Bibliography = Bibliography;
    FileData->Preamble = Preamble;
    FileData->BuildCommand = Build;
    FileData->Description = Description;
    FileData->misc = list;
    FileData->Path = Path;//FileCommands::NewFilePathAndId(FileData,needsSubSection);
    FileData->Content = FileCommands::ClearMetadataFromContent(Content);
    FileData->Database = SourceDatabase;
    FileData->DatabaseName = ui->FilesDatabasesCombo->currentText();

    for(int i=0;i<ui->SelectedFiles->topLevelItemCount();i++){
        if(ui->SelectedFiles->topLevelItem(i)->text(0)==databaseName){
            if(!SelectedFileList.value(databaseName).contains(Id)){
                QTreeWidgetItem * subitem = new QTreeWidgetItem({Id});
                subitem->setData(0,Qt::UserRole,QVariant::fromValue(FileData));
                ui->SelectedFiles->topLevelItem(i)->addChild(subitem);
//                bool hasDiff = ;
//                if(hasDiff){
//                    ui->warningLabel->setText("Data resemblences");
//                    ui->warningButton->setEnabled(true);
//                }

                if(!isImportMode){
                    QComboBox *comboBox = new QComboBox(this);
                    foreach (QSqlDatabase database, DataTex::GlobalFilesDatabaseList.values()) {
                        if(database.databaseName() != DataTex::CurrentDataBasePath){
                            QString databaseName = QFileInfo(database.databaseName()).baseName();
                            comboBox->addItem(DataTex::GlobalFilesDatabaseListNames[databaseName],QVariant::fromValue(database));
                            subitem->setData(1,Qt::UserRole,QVariant::fromValue(comboBox->currentData().value<QSqlDatabase>()));
                        }
                    }
                    ui->SelectedFiles->setItemWidget(subitem, 1, comboBox);
                    connect(comboBox,QOverload<int>::of(&QComboBox::activated),this,[=](){
                        subitem->setData(1,Qt::UserRole,QVariant::fromValue(comboBox->currentData().value<QSqlDatabase>()));
                        //update resemblence button;
                    });
                    ActionsWidget(subitem,CheckMetadataResemblances(FileData,comboBox->currentData().value<QSqlDatabase>()));
                }
                else{
                    subitem->setText(1,SourceDatabaseName);
                    ActionsWidget(subitem,CheckMetadataResemblances(FileData,DestinationDatabase));
                }

                QString solutions = SqlFunctions::Get_String_From_Query(QString("SELECT count(Solution_Id) FROM Solutions_per_File WHERE File_Id = '%1'").arg(Id),SourceDatabase);
                if(solutions.toInt()>0){
                    QCheckBox *includeSolutions = new QCheckBox(QString(" %1/%2 solutions").arg(solutions,solutions),this);
                    ui->SelectedFiles->setItemWidget(subitem,2,includeSolutions);
                    QStringList solutions_ids = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Id FROM Solutions_per_File WHERE File_Id = '%1'").arg(Id),SourceDatabase);
                    subitem->setData(2,Qt::UserRole,QVariant::fromValue(solutions_ids));
                    includeSolutions->setChecked(true);
                    foreach (QString id,solutions_ids) {
                        QStringList solutions_ids = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Id FROM Solutions_per_File WHERE File_Id = '%1'").arg(Id),SourceDatabase);
                        subitem->setData(2,Qt::UserRole,QVariant::fromValue(solutions_ids));
                        QTreeWidgetItem * subsubitem = new QTreeWidgetItem({id,QString()});
                        QCheckBox *includeSolution = new QCheckBox(this);
                        includeSolution->setChecked(true);
                        DBFileInfo *sol_info = new DBFileInfo(id,SourceDatabase);
                        sol_info->misc.setValue(subitem->data(0,Qt::UserRole).value<DBFileInfo*>());
                        subsubitem->setData(0,Qt::UserRole,QVariant::fromValue(sol_info));
                        subitem->addChild(subsubitem);
                        ui->SelectedFiles->setItemWidget(subsubitem,2,includeSolution);
                        SelectedFileList[databaseName].insert(id);
                        SolutionsList.append(sol_info);
                        connect(includeSolutions,&QCheckBox::clicked,this,[=](bool checked){
                            includeSolution->setChecked(checked);
                        });
                        connect(includeSolution,&QCheckBox::toggled,this,[=](bool checked){
                            if(checked){
                                SolutionsList.append(sol_info);
                            }
                            else{
                                SolutionsList.removeOne(sol_info);
                            }
                            includeSolutions->setText(QString(" %1/%2 solutions")
                                            .arg(QString::number(SolutionsList.count()),solutions));
                            includeSolutions->setChecked(SolutionsList.count()>0);
                            qDebug()<<SolutionsList.count();
                        });
                    }
                }
                SelectedFileList[databaseName].insert(Id);
            }
            else{
                QMessageBox::warning(this,"title","file exists",QMessageBox::Ok);
            }
        }
    }
    if(!FileList.contains(FileData)){FileList.append(FileData);}
    ui->SelectedFiles->header()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->SelectedFiles->expandAll();
}

void CloneDatabaseFile::on_removeButton_clicked()
{
    if(ui->SelectedFiles->selectionModel()->hasSelection()){
        auto *item = ui->SelectedFiles->currentItem();
        int index = ui->SelectedFiles->currentIndex().parent().row();
        FileList.removeAll(item->data(0,Qt::UserRole).value<DBFileInfo*>());
        if(item->parent()){
            QString database = item->parent()->text(0);
            SelectedFileList[database].remove(item->text(0));
            int k = ui->SelectedFiles->currentItem()->childCount();
            while(ui->SelectedFiles->currentItem()->childCount()>0) {
                k--;
                SelectedFileList[database].remove(item->child(k)->text(0));
                SolutionsList.removeAll(item->child(k)->data(0,Qt::UserRole).value<DBFileInfo*>());
                delete ui->SelectedFiles->currentItem()->takeChild(k);
            }
            if(item->parent()->childCount()==1){
                SelectedDatabaseList.remove(database);
                delete item->parent()->takeChild(ui->SelectedFiles->currentIndex().row());
                delete ui->SelectedFiles->takeTopLevelItem(index);
            }
            else{
                item->parent()->takeChild(ui->SelectedFiles->currentIndex().row());
                delete item;
            }
        }
    }
}

void CloneDatabaseFile::RemoveFile(int row)
{
//    if(ui->SelectedFiles->selectionModel()->hasSelection()){
//        auto *item = ui->SelectedFiles->item
//        int index = ui->SelectedFiles->currentIndex().parent().row();
//        FileList.removeAll(item->data(0,Qt::UserRole).value<DBFileInfo*>());
//        if(item->parent()){
//            QString database = item->parent()->text(0);
//            SelectedFileList[database].remove(item->text(0));
//            int k = ui->SelectedFiles->currentItem()->childCount();
//            while(ui->SelectedFiles->currentItem()->childCount()>0) {
//                k--;
//                SelectedFileList[database].remove(item->child(k)->text(0));
//                SolutionsList.removeAll(item->child(k)->data(0,Qt::UserRole).value<DBFileInfo*>());
//                delete ui->SelectedFiles->currentItem()->takeChild(k);
//            }
//            if(item->parent()->childCount()==1){
//                SelectedDatabaseList.remove(database);
//                delete item->parent()->takeChild(ui->SelectedFiles->currentIndex().row());
//                delete ui->SelectedFiles->takeTopLevelItem(index);
//            }
//            else{
//                item->parent()->takeChild(ui->SelectedFiles->currentIndex().row());
//                delete item;
//            }
//        }
//    }
}

void CloneDatabaseFile::on_Okbutton_accepted()
{
    foreach (DBFileInfo * info, FileList) {
        if(info->misc.value<int>()==NewFileMode::CloneModeContentAndMetadata){
            //Sql queries to clone metadata to destination database
            QSqlQuery WriteData(DataTex::CurrentTexFilesDataBase);
            WriteData.exec(QString("INSERT OR IGNORE INTO Fields (Id,Name) VALUES(\"%1\",\"%2\")").arg(info->Field[0],info->Field[1]));
            foreach (QStringList chapter,info->Chapters) {
                WriteData.exec(QString("INSERT OR IGNORE INTO Chapters (Id,Name,Field) VALUES(\"%1\",\"%2\",\"%3\")")
                                   .arg(chapter[0],chapter[1],chapter[2]));
            }
            foreach (QStringList section,info->Sections) {
                WriteData.exec(QString("INSERT OR IGNORE INTO Sections (Id,Name,Chapter) VALUES(\"%1\",\"%2\",\"%3\")")
                                   .arg(section[0],section[1],section[2]));
            }
            foreach (QStringList subsection,info->SubSections) {
                WriteData.exec(QString("INSERT OR IGNORE INTO Exercise_Types (Id,Name) VALUES(\"%1\",\"%2\")")
                                   .arg(subsection[0],subsection[1]));
                WriteData.exec(QString("INSERT OR IGNORE INTO Sections_Exercises (Exercise_Id,Section_Id) VALUES(\"%1\",\"%2\")")
                                   .arg(subsection[0],subsection[2]));
            }
            WriteData.prepare(QString("INSERT OR IGNORE INTO FileTypes (Id,FileType,FolderName,Solvable) VALUES(:id,:name,:folder,:sol)"));
            WriteData.bindValue(":id",info->FileType[0]);
            WriteData.bindValue(":name",info->FileType[1]);
            WriteData.bindValue(":folder",info->FileType[2]);
            WriteData.bindValue(":sol",info->FileType[3]);
            WriteData.exec();
            if(info->FileType[3]=="1"){
                WriteData.prepare(QString("INSERT OR IGNORE INTO FileTypes (Id,FileType,FolderName,Solvable,BelongsTo) VALUES(:id,:name,:folder,-1,:fid)"));
                WriteData.bindValue(":id",info->FileType[0]+"-Sol");
                WriteData.bindValue(":name",info->FileType[1]+tr(" - Solution"));
                WriteData.bindValue(":folder",info->FileType[2]+tr(" - Solutions"));
                WriteData.bindValue(":fid",info->FileType[0]);
                WriteData.exec();
            }
        }
    }
    int count = FileList.count();
    NewDatabaseFile * file = new NewDatabaseFile(this,FileList.at(0),FileList.at(0)->misc.value<int>());
    file->hideButton(true);
    file->setFinishButton(count==1);
    connect(file,&NewDatabaseFile::sendFileInfo,this,[=](int i,DBFileInfo * fileInfo,int move)mutable{
        FileList[i]=fileInfo;
        file->setFinishButton(i==count-1);
        if(move==1){i++;}else{i--;}
        qDebug()<<i<<" of "<<count;
        if(i<count){
            file->CloneModeIsEnabled(FileList.at(i),FileList.at(i)->misc.value<int>());
        }
        else{
            emit file->acceptClone();
            file->reject();
        }
    });

    connect(file,&NewDatabaseFile::acceptClone,this,[=](){
        foreach (DBFileInfo *fileinfo,FileList) {
            //            fileinfo->Content = FileCommands::NewFileText(fileinfo->Id,fileinfo->Content,DataTex::CurrentTexFilesDataBase);
            FileCommands::AddNewFileToDatabase(fileinfo,DataTex::CurrentTexFilesDataBase);
            emit acceptSignal(fileinfo->Path);
            delete fileinfo;
        }
        foreach (DBFileInfo *solutioninfo,SolutionsList) {
            DBFileInfo *info = solutioninfo->misc.value<DBFileInfo*>();
            QString content = solutioninfo->Content;
            solutioninfo = FileCommands::CreateSolutionData(info,DataTex::CurrentTexFilesDataBase);
            content = FileCommands::ClearMetadataFromContent(content);
            solutioninfo->Content = FileCommands::NewFileText(solutioninfo->Id,content,DataTex::CurrentTexFilesDataBase);
            FileCommands::AddNewFileToDatabase(solutioninfo,DataTex::CurrentTexFilesDataBase);
            QSqlQuery solutions_per_file(DataTex::CurrentTexFilesDataBase);
            solutions_per_file.exec(QString("INSERT INTO Solutions_per_File (Solution_Id,Solution_Path,File_Id) VALUES ('%1', '%2','%3');").arg(solutioninfo->Id,solutioninfo->Path,info->Id));
            delete info;
            delete solutioninfo;
        }
    });
    file->show();
    file->activateWindow();
    accept();
}

void CloneDatabaseFile::CreateCustomTagWidget(QSqlDatabase database)
{
    if(!ui->FilesTagFilter->isChecked()){delete filesTagLine;}
    filesTagLine = new TagsFilterWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",database));
    filesTagLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    ui->gridLayout_16->addWidget(filesTagLine,4,0,1,2);
    connect(filesTagLine,&TagsFilterWidget::SelectedTags,this,[=](QStringList list){
        FilesTable->setFilter(FilesTable->model()->columnCount()-1,list.join(","));
    });
    filesTagLine->setVisible(false);
    connect(ui->FilesTagFilter, &QPushButton::toggled, this, [=](bool checked){
        filesTagLine->setVisible(checked);
    });
}

//void CloneDatabaseFile::on_checkBox_clicked(bool checked)
//{
//    if(checked){
//        LoadDatabaseFiles(SourceDatabase);
//        ui->FilesDatabasesCombo->setEnabled(checked);
//        FilesTable->filterHeader()->adjustPositions();
//    }
//    else {
//        ui->FilesDatabasesCombo->setEnabled(checked);
//        LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase);
//        FilesTable->filterHeader()->adjustPositions();
//    }
//}

void CloneDatabaseFile::on_FilesDatabasesCombo_activated(int index)
{
    if(index>-1 /*&& ui->checkBox->isChecked()*/){
//        for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
//            if(DataTex::GlobalFilesDatabaseList.values()[i]==ui->FilesDatabasesCombo->currentData().value<QSqlDatabase>()){
//                index = i;
//            }
//        }
        SourceDatabase = ui->FilesDatabasesCombo->currentData().value<QSqlDatabase>();//DataTex::GlobalFilesDatabaseList.values().at(index);
        LoadDatabaseFiles(SourceDatabase);
    }
    ui->FilesTagFilter->setChecked(false);
    CreateCustomTagWidget(SourceDatabase);
    FilesTable->filterHeader()->adjustPositions();
}

int CloneDatabaseFile::CountModelRows()
{
    QAbstractItemModel * model = FilesTable->model();
    QModelIndex ix = FilesTable->currentIndex();
    while (model->canFetchMore(ix))
        model->fetchMore(ix);
    int rows = model->rowCount();
    return rows;
}

void CloneDatabaseFile::on_addEverything_clicked()
{
    foreach (QModelIndex index,FilesTable->selectionModel()->selectedRows(0)) {
        AddFiles(index.row());
    }
}

void CloneDatabaseFile::on_SelectedFiles_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()/* && !item->parent()->parent()*/){
//        ui->CopyData->setEnabled(true);
        DBFileInfo *info = item->data(0,Qt::UserRole).value<DBFileInfo*>();
        QStringList chapters = info->getChaptersNames();
        QStringList sections = info->getSectionsNames();
        QStringList subsections = info->getSubSectionsNames();
        needsSubSection = QString(info->FileType.at(3)).toInt();
        ui->MetadataTable->resizeRowsToContents();
        hasSelection = true;
        ui->SourceCodeBrowser->setText(info->Content);
        FileCommands::ShowPdfInViewer(info->Path,FileView);
        ui->BibliographyBrowser->setText(info->Bibliography);
        ui->removeButton->setEnabled(true);
        ui->MetadataTable->item(0,0)->setText(info->Id);
        ui->MetadataTable->item(1,0)->setText(info->Field[1]);
        ui->MetadataTable->item(2,0)->setText(chapters.join("|"));
        ui->MetadataTable->item(3,0)->setText(sections.join("|"));
        ui->MetadataTable->item(4,0)->setText(subsections.join("|"));
        ui->MetadataTable->item(5,0)->setText(info->FileType[1]);
        ui->MetadataTable->item(6,0)->setText(QString::number(info->Difficulty));
        ui->MetadataTable->item(7,0)->setText(info->Path);
        ui->MetadataTable->item(8,0)->setText(info->Date.toString("dddd d MMMM yyyy hh:mm"));
        ui->MetadataTable->item(9,0)->setText((info->Solved) ? tr("YES") : tr("NO"));
        qDebug()<<info->FileType;
        ui->removeButton->setEnabled(!item->parent() || !item->parent()->parent());
    }
}

void CloneDatabaseFile::disableItems(QComboBox * combo,int excludeItems,bool checked)
{
    QStandardItemModel *model =
        qobject_cast<QStandardItemModel *>(combo->model());
    for(int i=excludeItems;i<model->rowCount();i++){
        model->item(i)->setFlags(checked ? model->item(i)->flags() & ~Qt::ItemIsEnabled
                                         : model->item(i)->flags() | Qt::ItemIsEnabled);
    }
}

bool CloneDatabaseFile::CheckMetadataResemblances(DBFileInfo * fileInfo, QSqlDatabase destinationDatabase)
{
    DBFileInfo resemblenceInfo;
    QStringList check = SqlFunctions::Get_Record_From_Query(QString("SELECT Id='%1',Name='%2' FROM Fields WHERE (Id = '%1' OR Name = '%2')").arg(fileInfo->Field[0],fileInfo->Field[1]),destinationDatabase);

    QStringList chapters;
    QStringList sections;
    QStringList subsections;
    for(QStringList chapter:qAsConst(fileInfo->Chapters)){
        resemblenceInfo.Chapters.append(SqlFunctions::Get_Record_From_Query(QString("SELECT Id='%1',Name='%2',* FROM Chapters WHERE (Id = '%1' OR Name = '%2')").arg(chapter[0],chapter[1]),destinationDatabase));
    }
    //     = SqlFunctions::Get_Record_From_Query(QString("SELECT Id='%1',Name='%2' FROM Fields WHERE (Id = '%1' OR Name = '%2')").arg(fileInfo->Field[0],fileInfo->Field[1]),database);
    //     = SqlFunctions::Get_Record_From_Query(QString("SELECT Id='%1',Name='%2' FROM Fields WHERE (Id = '%1' OR Name = '%2')").arg(fileInfo->Field[0],fileInfo->Field[1]),database);
//    qDebug()<<fields;
    //    delete combo;
    emit sendResemblenceInfo(resemblenceInfo);
    return (/*fields.count()>0 || */chapters.count()>0 || sections.count()>0 || subsections.count()>0);
}

void CloneDatabaseFile::ActionsWidget(QTreeWidgetItem * item,bool hassResemb)
{
    Switch * cloneMetadata = new Switch(tr("Clone metadata"),ui->SelectedFiles);
    CloneMetadataSwitches.append(cloneMetadata);
    QHBoxLayout *l = new QHBoxLayout(ui->SelectedFiles);
    l->addWidget(cloneMetadata);
    QWidget *container = new QWidget(ui->SelectedFiles);
    container->setLayout(l);
    int cloneMode = NewFileMode::CloneModeOnlyContent;
    item->data(0,Qt::UserRole).value<DBFileInfo*>()->misc.setValue(cloneMode);
    connect(cloneMetadata,&QCheckBox::toggled,this,[=](bool checked)mutable{
        cloneMode = NewFileMode::CloneModeOnlyContent+checked;
        item->data(0,Qt::UserRole).value<DBFileInfo*>()->misc.setValue(cloneMode);
    });
    if(hassResemb){
        QPushButton * showResemb = new QPushButton(ui->SelectedFiles);
        MetadataResemblancesButtons.append(showResemb);
        showResemb->setEnabled(false);
        showResemb->setIcon(QIcon(":/images/exclamation-triangle.svg"));
        showResemb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
        l->addWidget(showResemb);
        connect(cloneMetadata,&QCheckBox::toggled,this,[=](bool checked){
            showResemb->setEnabled(checked);
        });
        connect(showResemb,&QPushButton::clicked,this,[=](){
            ShowReseblences(item->data(0,Qt::UserRole).value<DBFileInfo*>());
        });
    }
    ui->SelectedFiles->setItemWidget(item,3,container);
}

void CloneDatabaseFile::ShowReseblences(DBFileInfo * fileInfo)
{
    QDialog *d = new QDialog(this);
    d->setWindowTitle(tr("Metadata resemblences"));
    QVBoxLayout * v = new QVBoxLayout(this);
    //        QVBoxLayout * files = new QVBoxLayout(this);
    //        QVBoxLayout * documents = new QVBoxLayout(this);
    QLabel * message = new QLabel(tr(""));
    v->addWidget(message);
    QHBoxLayout * h2 = new QHBoxLayout(this);
    QLabel * fl = new QLabel(fileInfo->Id);
    //        QLabel * dl = new QLabel(tr("Documents databases"));
    QDialogButtonBox *dbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,this);
    h2->addWidget(fl);
    h2->addWidget(dbox);
    v->addLayout(h2);
    d->setLayout(v);
    d->show();
}
