#include "notesdocuments.h"
#include "ui_notesdocuments.h"



NotesDocuments::NotesDocuments(QWidget *parent, DTXDocument document, int mode) :
    QDialog(parent),
    ui(new Ui::NotesDocuments)
{
    radiogroup = new QButtonGroup;
    currentbase = DataTex::CurrentDocumentsDataBase;
    currentbase_Exercises = DataTex::CurrentFilesDataBase;
    DocumentsPath = QFileInfo(DataTex::CurrentDocumentsDataBase.Path).absolutePath()+QDir::separator();
    Document = document;
    Mode = mode;
    CurrentDocContent = FileCommands::ClearDocumentContent(Document.Content);
    textChanged = false;
    radioList.clear();
    ui->setupUi(this);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",currentbase.Database);
    ui->FoldersStructureView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->FoldersStructureView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view = new QPdfViewer(this);
    ui->splitter_3->addWidget(view);
    view->show();

    fileview = new QPdfViewer(this);
    ui->verticalLayout_10->addWidget(fileview);
    fileview->show();

    DocumentTable = new ExtendedTableWidget(this);
    ui->verticalLayout_19->addWidget(DocumentTable);
    DocumentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentTable->horizontalHeader()->setSectionsClickable(true);
    if(mode != CloneModeContentAndMetadata && mode != CloneModeOnlyContent){
        GetDocTypes();
        LoadFolderStructure();
        LoadDocTable();
    }

    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);
    level = -1;

    QList<QStringList> Preambles = SqlFunctions::GetRecordList("SELECT Id,Name FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    for(const DTXBuildCommand &build : qAsConst(DataTex::DTXBuildCommands)){
        ui->BuildBox->addItem(build.Name,QVariant::fromValue(build));
    }
    for (int i=0;i<Preambles.count();i++) {
        ui->PreambleBox->addItem(Preambles.at(i)[0],QVariant(Preambles.at(i)[1]));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
    ui->Okbutton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,[=](QString docId){
        CurrentDocContent = (mode != ImportMode) ? ui->DocumentContent->editor->toPlainText() : Document.Content;
        if(!docId.isEmpty()){
            QString Content = "%# Document Id : "+docId+"-----------------\n";
            Content += "%@ DocumentsDB Id : "+currentbase.BaseName+"\n";
            Content += "%#--------------------------------------------------\n";
            Content += FileCommands::ClearDocumentContent(CurrentDocContent);
            ui->DocumentContent->editor->setText(Content);
            ui->Okbutton->button(QDialogButtonBox::Ok)->setEnabled(!docId.isEmpty());
            QTextCursor cursor = ui->DocumentContent->editor->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->DocumentContent->editor->setTextCursor(cursor);
            textChanged = true;
            ui->InsertFilesButton->setEnabled(true);
        }
        else{
            ui->DocumentContent->editor->clear();
            ui->stackedWidget->setCurrentIndex(0);
            ui->InsertFilesButton->setEnabled(false);
        }
    });
    ui->DocumentContent->toolBar->Save->setVisible(false);

    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase.Database));
    ui->verticalLayout_20->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);
    DataTex::StretchColumnsToWidth(DocumentTable);
    if(mode == CloneModeContentAndMetadata || mode == CloneModeOnlyContent){
        ui->DatabaseCombo->addItem(tr("Select a database..."));
        for (DTXDatabase DTXDB : DataTex::GlobalDatabaseList) {
            if(DTXDB.Path!=DataTex::CurrentDocumentsDataBase.Path && DTXDB.Type == DTXDatabaseType::DocumentsDB){
                ui->DatabaseCombo->addItem(DTXDB.Description,DTXDB.Path);
            }
        }
        ui->DatabaseCombo->setVisible(true);
        ui->DatabaseLabel->setVisible(true);
    }
    else{
        ui->DatabaseCombo->setVisible(false);
        ui->DatabaseLabel->setVisible(false);
    }
    RandomFilesToKeep = 0;
    for (DTXDatabase DTXDB : DataTex::GlobalDatabaseList) {
        if(DTXDB.Type == DTXDatabaseType::FilesDB){
            ui->FilesDatabasesCombo->addItem(DTXDB.Description,QVariant::fromValue(DTXDB));
        }
    }
    ui->FilesDatabasesCombo->setCurrentText(DataTex::CurrentFilesDataBase.Description);

    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,3,0,1,1);
    FilesTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    FilesTable->setAlternatingRowColors(true);

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentFilesDataBase.Database);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentFilesDataBase.Database);
    LoadDatabaseFiles(DataTex::CurrentFilesDataBase.Database,SqlFunctions::ShowAllDatabaseFiles);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    DataTex::StretchColumns(FilesTable,1.5);

    ui->splitter->setSizes(QList<int>({1,200}));
    ui->splitter_2->setSizes(QList<int>({1,300}));

    connect(FilesTable,&QTableView::doubleClicked,this,[=](){on_addButton_clicked();});
    connect(ui->RandomSelectionList,&QListWidget::itemSelectionChanged,this,[=](){
        ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
        ui->moveDown->setEnabled(ui->RandomSelectionList->currentRow()!=ui->RandomSelectionList->count()-1);
        ui->moveUp->setEnabled(ui->RandomSelectionList->currentRow()!=0);
    });
    connect(ui->RandomSelectionList,&QListWidget::itemClicked,this,[=](){
        ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
        RandomFilesToKeep = CountRandomFiles();
        ui->moveDown->setEnabled(ui->RandomSelectionList->currentRow()!=ui->RandomSelectionList->count()-1);
        ui->moveUp->setEnabled(ui->RandomSelectionList->currentRow()!=0);
    });
    connect(ui->removeAll,&QPushButton::clicked,this,[=](){
        for(int i=ui->RandomSelectionList->count()-1;i>-1;i--){
            auto *item = ui->RandomSelectionList->takeItem(i);
            delete item;
        }
        RandomFilesToKeep = ui->RandomSelectionList->count();
    });
    connect(ui->moveUp,&QPushButton::clicked,this,[=](){
        if(ui->RandomSelectionList->selectionModel()->hasSelection()){
            int i = ui->RandomSelectionList->currentRow();
            QListWidgetItem * item = ui->RandomSelectionList->currentItem();
            ui->moveDown->setEnabled(true);
            ui->RandomSelectionList->takeItem(i);
            ui->RandomSelectionList->insertItem(i-1,item);
            ui->RandomSelectionList->setCurrentRow(i-1);
            if(i==1){
                ui->moveUp->setEnabled(false);
            }
        }
    });
    connect(ui->moveDown,&QPushButton::clicked,this,[=](){
        int rows = ui->RandomSelectionList->count();
        if(ui->RandomSelectionList->selectionModel()->hasSelection()){
            int i = ui->RandomSelectionList->currentRow();
            ui->moveUp->setEnabled(true);
            QListWidgetItem * item = ui->RandomSelectionList->currentItem();
            ui->RandomSelectionList->takeItem(i);
            ui->RandomSelectionList->insertItem(i+1,item);
            ui->RandomSelectionList->setCurrentRow(i+1);
            if(i==rows-2){
                ui->moveDown->setEnabled(false);
            }
        }
    });
    ui->FilesTabWidget->setCurrentIndex(0);
    ui->splitter->setSizes(QList<int>{300,150});
    connect(ui->filesSelected->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::filesSelected_SelectionChanged);
    connect(ui->SourceCode,&QPushButton::clicked,this,[&](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->DocViewerButton,&QPushButton::clicked,this,[&](){ui->stackedWidget->setCurrentIndex(1);});

    RightClick = new RightClickMenu(this,FilesTable,3);
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
        updateFilter_files(list);
        FilesTable->filterHeader()->adjustPositions();
    });

    connect(ui->InsertFilesButton,&Switch::toggled,this,[&](bool checked){
        ui->stackedWidget_2->setCurrentIndex(checked);
    });

    connect(ui->DocumentContent->editor,&QTextEdit::cursorPositionChanged,this,
            &NotesDocuments::DocumentContent_cursorPositionChanged);
    if(mode == EditMode){
        QModelIndex id = folderModel->index(QFileInfo(Document.Path).absolutePath(), 0);
        on_FoldersStructureView_clicked(id);
        foreach (QAbstractButton * bt,radiogroup->buttons()) {
            if(bt->text()==Document.Type){
                bt->setChecked(true);
            }
        }
        SavePath(id);
        ui->BuildBox->setCurrentIndex(ui->BuildBox->findText(Document.BuildCommand));
//        ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData(Document.Preamble));
        ui->NameLine->setText(Document.Id);
        ui->DocumentContent->editor->setText(Document.Content);
        ui->DocumentDescription->setText(Document.Description);
    }
    if(mode == ImportMode){
        connect(ui->DocumentContent->editor,&QTextEdit::textChanged,this,[=](){
            if(!ui->NameLine->text().isEmpty() && textChanged && !ui->DocumentContent->editor->toPlainText().isEmpty()){
                Document.Content = FileCommands::ClearDocumentContent(ui->DocumentContent->editor->toPlainText());
            }
        });
    }
    DocumentContent_cursorPositionChanged();
    connect(ui->LockUnlockAll,&QPushButton::clicked,this,[&](bool checked){
        for(int i=0;i<ui->RandomSelectionList->count();i++){
            ui->RandomSelectionList->item(i)->setCheckState(static_cast<Qt::CheckState>(2*(checked)));
        }
        CountRandomFiles();
    });
    connect(ui->CleanUnlocked,&QPushButton::clicked,this,[&](){
        for(int i=ui->RandomSelectionList->count()-1;i>-1;i--){
            if(ui->RandomSelectionList->item(i)->checkState() == Qt::Unchecked){
                auto *item = ui->RandomSelectionList->takeItem(i);
                delete item;
            }
        }
    });
    ui->splitter_3->setSizes(QList<int>({ui->splitter_3->height(),0}));

    toolbar = new FileToolBar(ui->DocumentContent,false);
    toolbar->ShowMetadata->setVisible(false);
    toolbar->BuildFile->setVisible(false);
    toolbar->ShowPdf->setChecked(false);
    ui->horizontalLayout_5->insertWidget(2,toolbar);
    connect(toolbar->ShowPdf,&QAction::triggered,this,[&](bool checked){
        ui->splitter_3->setSizes(QList<int>({ui->splitter_3->height()*0.5*(checked),ui->splitter_3->height()*0.5*checked}));
    });

    toolbar->show();
    connect(ui->DocumentContent->editor->document(),&QTextDocument::blockCountChanged,this,[=](){
        findFilePositions();
    });
    findFilePositions();
}

NotesDocuments::~NotesDocuments()
{
    delete ui;
    delete view;
    delete fileview;
    delete RightClick;
    delete FilesRightClickMenu;
    delete filesTagLine;
    foreach(QRadioButton * bt,radioList){
        delete bt;
    }
}

void NotesDocuments::updateFilter(QStringList values)
{
    SqlFunctions::FilterDatabaseDocuments.clear();
    DataTex::FilterDocuments(Database_DocumentTableColumns);
    int columns = Database_DocumentTableColumns.count();
    for (int i=0;i<columns;i++) {
        SqlFunctions::FilterDatabaseDocuments.replace("replace"+Database_DocumentTableColumns.at(i),values.at(i));
    }
    DataTex::updateTableView(DocumentTable,SqlFunctions::FilterDatabaseDocuments,currentbase.Database,this);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::DocumentTable_selectionChanged);
    DataTex::LoadTableHeaders(DocumentTable,Database_DocumentTableColumns);
}

void NotesDocuments::DocumentTable_selectionChanged()
{
    QItemSelectionModel *select = DocumentTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    QString DocumentFilePath = DocumentTable->model()->data(DocumentTable->model()->index(row,6)).toString();
    FileCommands::ShowPdfInViewer(DocumentFilePath,view);
}

void NotesDocuments::on_CreateBasicFolder_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,&addfolder::newSingleEntry,this,[=](QString newfolder){
        QString Path = folderModel->filePath(ui->FoldersStructureView->currentIndex().parent());
        QDir dir(Path+QDir::separator()+newfolder);
        if (!dir.exists()){
            dir.mkpath(".");
            QSqlQuery InsertQuery(currentbase.Database);
            QStringList list = {"Basic_Folders","SubFolders","SubsubFolders"};
            InsertQuery.exec("PRAGMA foreign_keys = ON");
            InsertQuery.exec(QString("INSERT INTO %1 (Name) VALUES (\"%2\");").arg(list[level-1],newfolder));
        }
        else{
            QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
        }
    });
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::on_Okbutton_accepted()
{
    if(ui->NameLine->isEnabled()){
        Document.Id = ui->NameLine->text();
        if(!Document.Id.isEmpty() && !isWhiteSpace(Document.Id)){
            Document.Path = ui->pathline->text()+Document.Id+".tex";
            QFileInfo NewDocument(Document.Path);
            QDir path = NewDocument.absoluteDir();
            if (!path.exists())
                path.mkpath(".");
            Document.Content = ui->DocumentContent->editor->toPlainText();
            Document.Title = ui->TitleLine->text();
            Document.Date = QDateTime::currentDateTime();
//            Document.Preamble = ui->PreambleBox->currentData().toString();
            Document.BuildCommand = ui->BuildBox->currentText();
            Document.Description = ui->DocumentDescription->toPlainText();
//            Document.Database = currentbase.Database;
            Document.DatabaseName = currentbase.Description;
            QStringList valuesList;
            valuesList << Document.Id<<Document.Title<<Document.Type<<Document.BasicFolder<<
                Document.SubFolder<<Document.SubsubFolder<<Document.Path<<Document.Date.toString("dd/M/yyyy hh:mm")<<
                Document.Content<</*Document.Preamble<<*/Document.BuildCommand<<"0"<<""<<Document.Description;
            QString Values = "(\""+valuesList.join("\",\"")+"\")";
            QSqlQuery AddDocument(currentbase.Database);
            AddDocument.exec(QString("INSERT OR IGNORE INTO \"Basic_Folders\" (\"Name\") VALUES (\"%1\")").arg(Document.BasicFolder));
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders\" (Name) VALUES(:subfolder);"));
            AddDocument.bindValue(":subfolder",Document.SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders_per_Basic\" (\"Sub_Id\",\"Basic_Id\") VALUES(:sub,:basic);"));
            AddDocument.bindValue(":basic",Document.BasicFolder);
            AddDocument.bindValue(":sub",Document.SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders\" (\"Name\") VALUES(:subsub);"));
            AddDocument.bindValue(":subsub",Document.SubsubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders_per_Sub_perBasic\" (\"Subsub_Id\",\"Sub_Id\",\"Basic_Id\") VALUES(:subsub,:sub,:basic);"));
            AddDocument.bindValue(":subsub",Document.SubsubFolder);
            AddDocument.bindValue(":sub",Document.SubFolder);
            AddDocument.bindValue(":basic",Document.BasicFolder);
            AddDocument.exec();
            AddDocument.exec(QString("INSERT OR IGNORE INTO Documents "
                                    "(Id,Title,Document_Type,Basic_Folder,SubFolder,SubsubFolder,Path,Date,Content,Preamble,BuildCommand,NeedsUpdate,Bibliography,Description) "
                                    "VALUES %1;").arg(Values));
            if(Mode != CloneModeContentAndMetadata && Mode != CloneModeOnlyContent){
                emit createnewdocument(Document);
            }
            else{
                emit clonedocument(Document);
            }
            QString BibliographyFile = Document.Path;
            BibliographyFile.replace(".tex",".bib");
            QFile bibfile(BibliographyFile);
            bibfile.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream BibContent(&bibfile);
            BibContent.flush();
            BibContent << "%# Bib File : "+Document.Id+"\n\n%# End of bib file "+Document.Id;
            bibfile.close();

            CsvFunctions::WriteDocDataToCSV(Document.Path,currentbase.Database);

            QSqlQuery insertTag(currentbase.Database);
            tags = tagLine->GetTags();
            foreach(QString tag,tags){
                if(!tag.isEmpty()){
                    insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
                    insertTag.exec("INSERT OR IGNORE INTO Tags_per_Document (Tag_Id,Document_Id) VALUES (\""+tag+"\",\""+Document.Id+"\")");
                }
            }

            QString newDocPath = ui->pathline->text()+ui->NameLine->text()+".tex";
            if(Mode == CloneModeContentAndMetadata || Mode == CloneModeOnlyContent){
                QFile::copy(Document.Path.left(Document.Path.lastIndexOf("."))+".pdf",
                            newDocPath.left(newDocPath.lastIndexOf("."))+".pdf");
            }
            accept();
        }
        else if (Document.Id.isEmpty()==true || isWhiteSpace(Document.Id) == true){
        QMessageBox::warning(this,tr("Error"),tr("Select a file name.\nThe name is "
                                           "empty or contains\nempty space or dot \".\"."),QMessageBox::Ok);}
    }
    else if(ui->NameLine->isEnabled()==false){accept();}
}

void NotesDocuments::on_Okbutton_rejected()
{
    reject();
}

void NotesDocuments::DocTypeSelected(bool checked)
{
    if(checked){
        ui->removeButton->setEnabled(true);
        QRadioButton *btn = static_cast<QRadioButton *>(sender());
        Document.Type = btn->text();
        ui->NameLine->setEnabled(true);
        ui->BuildBox->setEnabled(true);
        ui->PreambleBox->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(0);
        ui->TitleLine->setEnabled(true);
    }
}

bool NotesDocuments::isWhiteSpace(const QString & str)
{
  return QRegExp("\t | \n | \v | \f | \r | |.").exactMatch(str);
}

QString NotesDocuments::SavePath(const QModelIndex &index)
{
    ui->pathline->setEnabled(true);
    ui->OpenPath->setEnabled(true);
    QString path = folderModel->filePath(index)+QDir::separator();
    QString  temp_path = path;
    temp_path.remove(QFileInfo(currentbase.Path).absolutePath()+QDir::separator());
    QStringList Folders = temp_path.split(QDir::separator());
    Document.BasicFolder = Folders[0];
    if(Folders.count()>1){
        if(!Folders[1].isNull() && !Folders[1].isEmpty()){
            Document.SubFolder = Folders[1];
        }
        else{
            Document.SubFolder = "-";
            Document.SubsubFolder = "-";
        }
    }
    if(Folders.count()>2){
        if(!Folders[2].isNull() && !Folders[2].isEmpty()){
            Document.SubsubFolder = Folders[2];
        }
        else{
            Document.SubsubFolder = "-";
        }
    }
    qDebug()<<Document.BasicFolder<<Document.SubFolder<<Document.SubsubFolder;
    ui->pathline->setText(path);
    return path;
}

void NotesDocuments::on_FoldersStructureView_clicked(const QModelIndex &index)
{
    ui->CreateSubFolder->setEnabled(true);
    ui->EditButton->setEnabled(true);
    ui->addButton->setEnabled(true);
    ui->RemButton->setEnabled(true);
    if(radioList.count()>0){
        if(!radioList[0]->isEnabled()){
            for (int i=0;i<radioList.count();i++) {
                radioList.at(i)->setEnabled(true);
            }
        }
    }
    QModelIndex ix = index;
    int depth = 0;
    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
      depth++;
    }
    int result;
    QDir dir(DocumentsPath);
    for(result=0;dir.cdUp();++result){}
    level = depth-result;
    tagLine->setEnabled(true);
    ui->CreateSubFolder->setEnabled(level >= 3);
}


void NotesDocuments::on_CreateSubFolder_clicked()
{
    addfolder * newFolder = new addfolder(this);
    connect(newFolder,&addfolder::newSingleEntry,this,[=](QString newfolder){
        QDir dir(ui->pathline->text()+newfolder);
        if (!dir.exists()){
            dir.mkpath(".");
            QModelIndex in = ui->FoldersStructureView->currentIndex();
            ui->FoldersStructureView->expand(in);
            QSqlQuery InsertQuery(currentbase.Database);
            QStringList list = {"Basic_Folders","SubFolders","SubsubFolders"};
            InsertQuery.exec("PRAGMA foreign_keys = ON");
            InsertQuery.exec(QString("INSERT INTO %1 (Name) VALUES (\"%2\");").arg(list[level],newfolder));
            qDebug()<<QString("INSERT INTO %1 (Name) VALUES (\"%2\");").arg(list[level],newfolder);
        }
        else{
            QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
        }
    });
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::on_RemButton_clicked()
{
    QCheckBox *cb = new QCheckBox("Μετάβαση στο φάκελο");
    QString currentDir = ui->FoldersStructureView->currentIndex().data(Qt::DisplayRole).toString();
    QDir oldDir(folderModel->filePath(ui->FoldersStructureView->currentIndex()));
    QMessageBox msgbox;
    msgbox.setText(tr("The folder will be deleted from the database!\nDelete the folder manually in case it contains files.\nDo you wish to proceed?"));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery DeleteQuery(currentbase.Database);
        QStringList list = {"Basic_Folders","SubFolders","SubsubFolders"};
        DeleteQuery.exec("PRAGMA foreign_keys = ON");
        DeleteQuery.exec(QString("DELETE FROM %1 WHERE Name = \"%2\";").arg(list[level-1],currentDir));
        if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+DocumentsPath));}
        oldDir.rmdir(oldDir.path());
    }
}

void NotesDocuments::on_EditButton_clicked()
{
    QString currentDir = ui->FoldersStructureView->currentIndex().data(Qt::DisplayRole).toString();
    QString oldPath = folderModel->filePath(ui->FoldersStructureView->currentIndex());
    newFolder = new addfolder(this);
    newFolder->EditFolder(currentDir);
    connect(this,SIGNAL(pathline(QString)),newFolder,SLOT(EditFolder(QString)));
    connect(newFolder,&addfolder::newSingleEntry,this,[=](QString newdir)mutable{
        QDir Dir(oldPath);
        QString temp = oldPath;
        QString newPath = temp.replace(currentDir,newdir);
        Dir.rename(oldPath,newPath);
        QSqlQuery UpdateQuery(currentbase.Database);
        QStringList list = {"Basic_Folders","SubFolders","SubsubFolders"};
        UpdateQuery.exec("PRAGMA foreign_keys = ON");
        UpdateQuery.exec(QString("UPDATE %1 SET Name = '%2' WHERE Name = '%3';").arg(list[level-1],newdir,currentDir));
    });
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::on_addButton_clicked()
{
    NewFileType * docType = new NewFileType(this,(DTXDatabaseType)DTXDatabaseType::DocumentsDB);
    connect(docType,&NewFileType::filedata,this,[=](DTXFileType newDocType){
        bool newDocTypeAdded = NewFileType::CreateNewDatabaseFileType(currentbase.Database,currentbase.Type, newDocType);
        int rows = ui->gridLayout_4->rowCount();
        int widgets = ui->gridLayout_4->count();
        if(newDocTypeAdded){
            QRadioButton * radio = new QRadioButton(newDocType.Name);
            radioList.append(radio);
            radiogroup->addButton(radio);
            ui->gridLayout_4->addWidget(radio,rows-1+(widgets+1)%2,widgets%2);
            connect(radio, &QRadioButton::toggled, this, &NotesDocuments::DocTypeSelected);
        }
        else{
            QMessageBox::warning(this,tr("Error"),tr("The document type %1 already exists.").arg(newDocType.Name),QMessageBox::Ok);
        }
    });
    docType->show();
    docType->activateWindow();
}

void NotesDocuments::on_removeButton_clicked()
{
    foreach(QRadioButton * bt,radioList){
        if(bt->isChecked()){
            radiogroup->removeButton(bt);
            radioList.removeAll(bt);
            QSqlQuery removeFileType(currentbase.Database);
            removeFileType.exec(QString("DELETE FROM Document_Types WHERE Name = \"%1\"").arg(bt->text()));
            delete bt;
            ui->removeButton->setEnabled(false);
        }
    }
}

void NotesDocuments::on_OpenPath_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///"+ui->pathline->text()));
}

void NotesDocuments::on_DatabaseCombo_activated(int index)
{
//    qDebug()<<DataTex::GlobalDatabaseList;
    QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
    currentbase = DataTex::GlobalDatabaseList.value(database);
    DocumentsPath = QFileInfo(currentbase.Path).absolutePath()+QDir::separator();
    GetDocTypes();
    LoadFolderStructure();
    LoadDocTable();
//    qDebug()<<DataTex::GlobalDatabaseList;
}

void NotesDocuments::GetDocTypes()
{
    radioList.clear();
    foreach(QAbstractButton *button, radiogroup->buttons()) {
        ui->gridLayout_4->removeWidget(button);
        radiogroup->removeButton(button);
        delete button;
    }

    QStringList FileTypeList;
    QSqlQuery DocTypes(currentbase.Database);
    DocTypes.exec(SqlFunctions::GetDocumentTypes);
    while(DocTypes.next()){
        FileTypeList.append(DocTypes.value(0).toString());
    }
    double items = FileTypeList.count();
    int half = (int)ceil(items/2);
    for (int i=0;i<half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i),i,0);
        radioList.at(i)->setEnabled(false);
        radiogroup->addButton(radioList.at(i));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::DocTypeSelected);
    }
    for (int i=0;i<items-half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i+half));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i+half),i,1);
        radioList.at(i+half)->setEnabled(false);
        radiogroup->addButton(radioList.at(i+half));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::DocTypeSelected);
    }
    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);

}

void NotesDocuments::LoadFolderStructure()
{
    folderModel = new QFileSystemModel(this);
    folderModel->setRootPath(QFileInfo(currentbase.Path).absolutePath());
    folderModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    folderModel->setNameFilterDisables(false);
    folderModel->insertRow(0);
    ui->FoldersStructureView->setModel(folderModel);
    QModelIndex index = folderModel->index(QFileInfo(currentbase.Path).absolutePath());
    ui->FoldersStructureView->setRootIndex(index);
    ui->FoldersStructureView->expand(index);
    ui->FoldersStructureView->scrollTo(index);
    ui->FoldersStructureView->setCurrentIndex(index);
    ui->FoldersStructureView->resizeColumnToContents(0);
    for (int i=1;i<folderModel->columnCount();i++) {
        ui->FoldersStructureView->setColumnHidden(i,true);
    }
}

void NotesDocuments::LoadDocTable()
{
    QSqlTableModel * DocumentModel = new QSqlTableModel(this,currentbase.Database);
    DocumentModel->setTable("Documents");
    DocumentModel->select();
    DocumentModel->setHeaderData(1, Qt::Horizontal, tr("Document type"));
    DocumentTable->setModel(DocumentModel);
    DocumentTable->show();
    int columns = DocumentTable->model()->columnCount();
    DocumentTable->generateFilters(columns,false);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NotesDocuments::DocumentTable_selectionChanged);
    connect(DocumentTable->filterHeader(), &FilterTableHeader::filterValues, this, &NotesDocuments::updateFilter);
    DataTex::StretchColumns(DocumentTable,1.5);
    for (int i=2;i<DocumentTable->model()->columnCount();i++) {
        DocumentTable->setColumnHidden(i,true);
    }
}

void NotesDocuments::LoadDatabaseFiles(QSqlDatabase database,QString query)
{
    FilesTable->filterTable(SqlFunctions::ShowAllDatabaseFiles,currentbase_Exercises.Database,filesSorting);
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NotesDocuments::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &NotesDocuments::updateFilter_files);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void NotesDocuments::on_RandomSelectionList_itemSelectionChanged()
{
    if(ui->RandomSelectionList->selectionModel()->hasSelection()){
        QString file = ui->RandomSelectionList->currentItem()->data(Qt::UserRole).toString();
        FileCommands::ShowPdfInViewer(file,view);
    }
}


void NotesDocuments::on_AddRandomFiles_clicked()
{
    for(int i=0;i<ui->RandomSelectionList->count();++i){
        if(ui->RandomSelectionList->item(i)->checkState() == Qt::Checked){
            AddFiles(randomlist[i]);
        }
    }
    ui->RandomSelectionList->clear();
    RandomFilesToKeep = 0;
    ui->SelectedFilesLabel->clear();
}

int NotesDocuments::CountRandomFiles()
{
    int count =0;
    for(int i=0;i<ui->RandomSelectionList->count();++i){
        if(ui->RandomSelectionList->item(i)->checkState() == Qt::Checked){
            count++;
        }
    }
    ui->SelectedFilesLabel->setText(tr("Add %1 files").arg(QString::number(count)));
    return count;
}

int NotesDocuments::CountModelRows()
{
    QAbstractItemModel * model = FilesTable->model();
    QModelIndex ix = FilesTable->currentIndex();
    while (model->canFetchMore(ix))
        model->fetchMore(ix);
    int rows = model->rowCount();
    return rows;
}

void NotesDocuments::AddFiles(int row)
{
    QTextCursor prev_cursor = ui->DocumentContent->editor->textCursor();
    QString FileContent;
    QFile file(FilesTable->model()->data(FilesTable->model()->index(row,7)).toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->editor->insertPlainText(FileContent+"\n\n");
    ui->DocumentContent->editor->setTextCursor(prev_cursor);
    SelectedFilesInDocument();
    DTXIncludedFile infile;
    infile.Id = FilesTable->model()->data(FilesTable->model()->index(row,0)).toString();
    infile.DatabaseId = ui->FilesDatabasesCombo->currentData().value<DTXDatabase>().BaseName;
    infile.DatabaseType = (DTXDatabaseType)ui->FilesDatabasesCombo->currentData().value<DTXDatabase>().Type;
    Document.FilesIncluded.append(infile);
}

void NotesDocuments::on_addfileButton_clicked()
{
    QTextCursor prev_cursor = ui->DocumentContent->editor->textCursor();
    QString FileContent;
    int row = FilesTable->currentIndex().row();
    QFile file(FilesTable->model()->data(FilesTable->model()->index(row,7)).toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->editor->insertPlainText(FileContent+"\n\n");
    ui->DocumentContent->editor->setTextCursor(prev_cursor);
    SelectedFilesInDocument();
}

void NotesDocuments::on_removefileButton_clicked()
{
    ui->removefileButton->setEnabled(false);
    QString Content = ui->DocumentContent->editor->toPlainText();
    Content.remove(DatabaseFileContent);
    ui->DocumentContent->editor->setText(Content);
    SelectedFilesInDocument();
}

void NotesDocuments::updateFilter_files(QStringList values)
{
    SqlFunctions::FilterTable(Database_FileTableFields,values);
    int columns = Database_FileTableFields.count();
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase_Exercises.Database,this);
    FilesTable->filterTable(SqlFunctions::FilesTable_UpdateQuery,currentbase_Exercises.Database,filesSorting);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
}

void NotesDocuments::FilesTable_selectionchanged()
{
    int file_count = FilesTable->selectionModel()->selectedRows().count();
    ui->addfileButton->setEnabled(true);
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
        //        FileCommands::BuildDocument(DataTex::DTXBuildCommands[buildCommand],PreviewFile,DataTex::LatexCommandsArguments[buildCommand],".tex");
        //        FileCommands::ClearOldFiles(PreviewFile);
        //Need to add a 'preamble' variable in CreateTexFile command
    }
    FileCommands::ShowPdfInViewer(PreviewFile,fileview);
    ui->FileContent->setText(fileContent);
}

void NotesDocuments::filesSelected_SelectionChanged()
{
    int row = ui->filesSelected->selectionModel()->currentIndex().row();
    QString file = ui->filesSelected->model()->data(ui->filesSelected->model()->index(row,0)).toString();
    QTextBlock block = ui->DocumentContent->editor->document()->findBlockByLineNumber(positions[file][0]);
    QTextCursor newCursor(block);
    ui->DocumentContent->editor->setTextCursor(newCursor);
    FileCommands::ShowPdfInViewer(ui->filesSelected->model()->index(row,5).data().toString(),view);
}

void NotesDocuments::ShowPdfOfFile(bool checked,QString file)
{
    if(checked){
        view->show();
        FileCommands::ShowPdfInViewer(file,view);
    }
    else{
        view->hide();
    }
}

void NotesDocuments::SelectedFilesInDocument()
{
    QStringList FilesInsideDocument;
    QStringList DatabasesInsideDocument;
    Document.FilesIncluded.clear();
    QString DocContent = ui->DocumentContent->editor->toPlainText();
    QTextStream textstream(&DocContent);
    int i=-1;
    while (!textstream.atEnd()){
        QString LineText=textstream.readLine();
        if(LineText.contains("%# File Id : ")){
            i++;
            qDebug()<<i;
            DTXIncludedFile infile;
            QString FileId = LineText.remove("%# File Id : ");
            FilesInsideDocument.append(FileId);
            infile.Id = FileId;
            Document.FilesIncluded.append(infile);
        }
        if(LineText.contains("%@ FilesDB Id : ")){
            QString databaseId = LineText.remove("%@ FilesDB Id : ");
            DatabasesInsideDocument.append(databaseId);
            Document.FilesIncluded[i].DatabaseId = databaseId;
            Document.FilesIncluded[i].DatabaseType = DTXDatabaseType::FilesDB;
        }
    }
    DatabasesInsideDocument.removeDuplicates();
    QString files = "(\""+FilesInsideDocument.join("\",\"")+"\")";
    QSqlQueryModel * FilesModel = new QSqlQueryModel(this);
    QStringList DataQueries;// = {SqlFunctions::ShowFilesInADocument.arg(files,QFileInfo(DataTex::CurrentFilesDataBase.Path).baseName())};
    QSqlQuery FilesQuery(DataTex::CurrentFilesDataBase.Database);
    for (QString databaseId : DatabasesInsideDocument) {
        DTXDatabase DTXDB = DataTex::GlobalDatabaseList.value(databaseId);
        QString name = (DTXDB.Path!=DataTex::CurrentFilesDataBase.Path) ? DTXDB.BaseName : "main" ;
        DataQueries.append(SqlFunctions::ShowFilesInADocument.arg(files,DTXDB.BaseName,name));
        if(DTXDB.Path!=DataTex::CurrentFilesDataBase.Path) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(DTXDB.Path,DTXDB.BaseName));
        }
    }
    QString query = DataQueries.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY df.Id ");
    qDebug()<<query+" ORDER BY df.Id ";
    FilesModel->setQuery(FilesQuery);
    ui->filesSelected->setModel(FilesModel);
    ui->filesSelected->show();
    connect(ui->filesSelected->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::filesSelected_SelectionChanged);
    DataTex::StretchColumns(ui->filesSelected,1.5);
}

void NotesDocuments::on_RefreshSelection_clicked()
{
    int rows = CountModelRows();
    NumOfFiles = ui->numOfFilesSpin->value();
    QVector<int> list;
    while(list.size()<rows){
        quint32 v = QRandomGenerator::global()->bounded(0, rows);
        if(!list.contains(v)){
            list.push_back(v);
        }
    }
    for(int i=ui->RandomSelectionList->count()-1;i>-1;i--){
        if(ui->RandomSelectionList->item(i)->checkState() == Qt::Unchecked){
            auto *item = ui->RandomSelectionList->takeItem(i);
            delete item;
        }
    }
    QStringList files;
    for (int i = 0;i<ui->RandomSelectionList->count();i++) {
        files.append(ui->RandomSelectionList->item(i)->text());
    }
    int i=-1;
    while (ui->RandomSelectionList->count()<NumOfFiles) {
        i++;
        QString file = FilesTable->model()->data(FilesTable->model()->index(list[i],7)).toString();
        if(!files.contains(QFileInfo(file).baseName())){
            QListWidgetItem * item = new QListWidgetItem(QFileInfo(file).baseName());
            item->setData(Qt::UserRole,file);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            item->setCheckState(Qt::Checked);
            ui->RandomSelectionList->addItem(item);
        }
    }
    ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
    randomlist = list;
    ui->AddRandomFiles->setEnabled(true);
    RandomFilesToKeep = ui->RandomSelectionList->count();
}

void NotesDocuments::on_numOfFilesSpin_valueChanged(int arg1)
{
    NumOfFiles = arg1;
}

void NotesDocuments::on_addEverything_clicked()
{
    foreach (QModelIndex index,FilesTable->selectionModel()->selectedRows(0)) {
        AddFiles(index.row());
    }
}

void NotesDocuments::on_removeSelectedFile_clicked()
{
    on_removeButton_clicked();
}

void NotesDocuments::CreateCustomTagWidget(QSqlDatabase database)
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

void NotesDocuments::on_checkBox_clicked(bool checked)
{
    if(checked){
        LoadDatabaseFiles(currentbase_Exercises.Database,SqlFunctions::ShowAllDatabaseFiles);
        ui->FilesDatabasesCombo->setEnabled(checked);
        FilesTable->filterHeader()->adjustPositions();
    }
    else {
        ui->FilesDatabasesCombo->setEnabled(checked);
        LoadDatabaseFiles(DataTex::CurrentFilesDataBase.Database,SqlFunctions::ShowAllDatabaseFiles);
        FilesTable->filterHeader()->adjustPositions();
    }
}

void NotesDocuments::on_FilesDatabasesCombo_activated(int index)
{
    if(ui->checkBox->isChecked()){
        currentbase_Exercises = ui->FilesDatabasesCombo->currentData().value<DTXDatabase>();
        LoadDatabaseFiles(currentbase_Exercises.Database,SqlFunctions::ShowAllDatabaseFiles);
    }
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    ui->FilesTagFilter->setChecked(false);
    CreateCustomTagWidget(currentbase_Exercises.Database);
    FilesTable->filterHeader()->adjustPositions();
}

void NotesDocuments::DocumentContent_cursorPositionChanged()
{
    LatexTextEdit::clearFormat(ui->DocumentContent->editor);
    DatabaseFileContent.clear();
    findFilePositions();
    int line = ui->DocumentContent->editor->textCursor().blockNumber();
    for (auto i = positions.cbegin(), end = positions.cend(); i != end; ++i) {
        if(line>=i.value()[0] && line <=i.value()[1]){
            QString file = i.key();
            QAbstractItemModel * m = ui->filesSelected->model();
            QModelIndex ix = ui->filesSelected->currentIndex();
            while (ui->filesSelected->model()->canFetchMore(ix))
                ui->filesSelected->model()->fetchMore(ix);
            QModelIndexList matchList = m->match(m->index(0,0), Qt::DisplayRole,
                                                 file, -1,  Qt::MatchFlags(Qt::MatchContains|Qt::MatchWrap));
            FileCommands::ShowPdfInViewer(ui->filesSelected->model()->data(ui->filesSelected->model()->index(matchList[0].row(),5)).toString(),view);
            for (int k = i.value()[0];k<i.value()[1]+1;k++) {
                QTextBlock block = ui->DocumentContent->editor->document()->findBlockByNumber(k);
                QTextCursor cursor(block);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor("#717171"));
                cursor.setBlockFormat(blockFormat);
                DatabaseFileContent.append(block.text()+"\n");
            }
            ui->removefileButton->setEnabled(true);
            break;
        }
        else{
            LatexTextEdit::clearFormat(ui->DocumentContent->editor);
            ui->removefileButton->setEnabled(false);
        }
    }
}

void NotesDocuments::findFilePositions()
{
    positions.clear();
    int start = -1;
    int end = -1;
    int lineNumber = -1;
    QString FileContent = ui->DocumentContent->editor->toPlainText();
    QTextStream parse(&FileContent);
    while(!parse.atEnd()){
        lineNumber++;
        QString line = parse.readLine();
        if(line.startsWith("%# File Id")){
            start = lineNumber;
        }
        if(line.startsWith("%# End of file")){
            end = lineNumber;
            positions.insert(line.remove("%# End of file "),{start,end});
        }
    }
}

