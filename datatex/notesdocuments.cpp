#include "notesdocuments.h"
#include "ui_notesdocuments.h"



NotesDocuments::NotesDocuments(QWidget *parent, bool editMode, QStringList metadata, QString fileName,
                               bool cloneMode, QString DocumentContent,bool ImportMode) :
    QDialog(parent),
    ui(new Ui::NotesDocuments)
{
    radiogroup = new QButtonGroup;
    currentbase = DataTex::CurrentDocumentDataBase;
    currentbase_Exercises = DataTex::CurrentTexFilesDataBase;
    NotesPath = QFileInfo(DataTex::CurrentDocumentDatabasePath).absolutePath()+QDir::separator();
    DocContent = DocumentContent;
    isEditMode = editMode;
    isImportMode = ImportMode;
    CurrentDocContent = FileCommands::ClearDocumentContent(DocContent);
    CloneMode = cloneMode;
    DocumentPath = fileName;
    textChanged = false;
    radioList.clear();
    ui->setupUi(this);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",currentbase);
    QStringList horzHeaders;
    horzHeaders << tr("File type") << tr("Name")<<"Path";
    QStringList Headers;
    ui->FoldersStructureView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->FoldersStructureView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Headers << tr("Folder name");
    ui->RemButton->setEnabled(false);
    ui->EditButton->setEnabled(false);
    ui->NameLine->setEnabled(false);
    ui->pathline->setEnabled(false);
    ui->OpenPath->setEnabled(false);
    ui->InsertFilesCheckBox->setEnabled(false);
    view = new QPdfViewer(this);
    ui->splitter_3->addWidget(view);
    view->show();

    fileview = new QPdfViewer(this);
//    fileview->setMinimumWidth(620);
    ui->verticalLayout_10->addWidget(fileview);
    fileview->show();

//    fileview_2 = new QPdfViewer(this);
//    fileview_2->setMinimumWidth(620);
//    fileview_2->show();

    DocumentTable = new ExtendedTableWidget(this);
    ui->verticalLayout_19->addWidget(DocumentTable);
    DocumentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentTable->horizontalHeader()->setSectionsClickable(true);
    if(!CloneMode){
        GetDocTypes();
        LoadFolderStructure();
        LoadDocTable();
    }

    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);
    ui->CreateSubFolder->setEnabled(false);
    ui->addButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
    level = -1;

    QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    ui->BuildBox->addItems(DataTex::LatexCommands.keys());
    for (int i=0;i<PreambleIds.count();i++) {
        ui->PreambleBox->addItem(PreambleNames.at(i),QVariant(PreambleIds.at(i)));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
    connect(ui->NameLine,&QLineEdit::textChanged,this,[=](){
        CurrentDocContent = (!isImportMode) ? ui->DocumentContent->editor->toPlainText() : DocContent;
        if(!ui->NameLine->text().isEmpty()){
            QString Content = "%# Database Document : "+ui->NameLine->text()+"-----------------\n";
            Content += "%@ Document type: "+DocumentType+"\n";
            Content += "%#--------------------------------------------------\n";
            Content += FileCommands::ClearDocumentContent(CurrentDocContent);
            ui->DocumentContent->editor->setText(Content);
            ui->InsertFilesCheckBox->setEnabled(!ui->NameLine->text().isEmpty());
            QTextCursor cursor = ui->DocumentContent->editor->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->DocumentContent->editor->setTextCursor(cursor);
            textChanged = true;
        }
        else{
            ui->DocumentContent->editor->clear();
        }
    });
    ui->DocumentContent->toolBar->Save->setVisible(false);


//    qDebug()<<ClearContent("%# Database Document : Epanalipsi_meleth_synarthshs-----------------\n"
//                          "%@ Document type: Ασκήσεις\n"
//                          "%#--------------------------------------------------\n"
//                          "\\documentclass[11pt,a4paper]{article}\n"
//                          "\\usepackage[utf8]{inputenc}\n"
//                          "\\usepackage{nimbusserif}\n"
//                          "\\usepackage[T1]{fontenc}\n"
//                          "\\begin{document}\n"
//                          "\\begin{center}\n"
//                          "{\\Large\n"
//                          "\\begin{enumerate}\n"
//                          "%# Database File : Analysh-KyrtothtaMonotAkrot-CombEx1----\n"
//                          "\\item Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, \n"
//                          "τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. \n"
//                          "Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.\n"
//                          "\\begin{alist}\n"
//                          "\\item $ f(x)=\frac{x}{x^2+1} $\n"
//                          "\\end{document}");
    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase));
    ui->verticalLayout_20->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);
    DataTex::StretchColumnsToWidth(DocumentTable);
    ui->DatabaseCombo->addItem(tr("Select a database..."));
    for (int i=0;i<DataTex::GlobalDocsDatabaseList.count();i++) {
        QSqlDatabase database = DataTex::GlobalDocsDatabaseList.values().at(i);
        if(database.databaseName()!=DataTex::CurrentDocumentDatabasePath){
            ui->DatabaseCombo->addItem(DataTex::GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()],database.databaseName());
        }
    }
    ui->DatabaseCombo->setVisible(CloneMode);
    ui->DatabaseLabel->setVisible(CloneMode);

    RandomFilesToKeep = 0;
    foreach (QSqlDatabase database, DataTex::GlobalFilesDatabaseList) {
        QString databaseName = QFileInfo(database.databaseName()).baseName();
        ui->FilesDatabasesCombo->addItem(DataTex::GlobalFilesDatabaseListNames[databaseName],QVariant(database.databaseName()));
    }
    ui->FilesDatabasesCombo->setCurrentText(DataTex::GlobalFilesDatabaseListNames[QFileInfo(DataTex::CurrentDataBasePath).baseName()]);
    ui->removeButton->setEnabled(false);
    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,3,0,1,1);
    FilesTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);
    FilesTable->setAlternatingRowColors(true);

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    DataTex::StretchColumns(FilesTable,1.5);

    ui->FilesDatabasesCombo->setEnabled(false);
    ui->addEverything->setEnabled(false);
    ui->splitter->setSizes(QList<int>({1,200}));
    ui->splitter_2->setSizes(QList<int>({1,300}));

    connect(FilesTable,&QTableView::doubleClicked,this,[=](){on_addButton_clicked();});
    //    ui->verticalLayout_2->addWidget(ui->ToolBar);
//    FileCommands::ShowPdfInViewer(CurrentDatabaseFile,DocView);
//    connect(ui->DocumentContent->editor, &QTextEdit::textChanged, this,[=](){ui->Save->setEnabled(true);});
//    SelectedFilesInDocument();
//    ui->RefreshSelection->setEnabled(false);
//    ui->numOfFilesSpin->setEnabled(false);
//    ui->AddRandomFiles->setEnabled(false);
    ui->moveDown->setEnabled(false);
    ui->moveUp->setEnabled(false);
//    connect(ui->RandomSelection,&QCheckBox::clicked,this,[=](bool checked){
//        ui->FilesTabWidget->setCurrentIndex(3);
//        ui->numOfFilesSpin->setEnabled(checked);
//        ui->RefreshSelection->setEnabled(checked);
//        ui->AddRandomFiles->setEnabled(ui->RandomSelectionList->count()!=0);
//    });
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
//            if(ui->RandomSelectionList->item(i)->checkState() == Qt::Unchecked){
                auto *item = ui->RandomSelectionList->takeItem(i);
                delete item;
//            }
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
    connect(ui->PdfPreview,&QPushButton::toggled,this,[&](bool checked){
        ui->splitter_3->setSizes(QList<int>({ui->splitter_3->height()*0.5*(checked),ui->splitter_3->height()*0.5*checked}));
    });
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

    connect(ui->InsertFilesCheckBox,&Switch::toggled,this,[&](bool checked){
        ui->stackedWidget_2->setCurrentIndex(checked);
    });

    connect(ui->DocumentContent->editor,&QTextEdit::cursorPositionChanged,this,&NotesDocuments::on_DocumentContent_cursorPositionChanged);
    if(isEditMode){
        QString DocType = metadata[0];
        QString DocBuildComand = metadata[1];
        QString DocPreamble = metadata[2];
        QString DocDescrition = metadata[3];
        QModelIndex id = folderModel->index(QFileInfo(DocumentPath).absolutePath(), 0);
        on_FoldersStructureView_clicked(id);
        foreach (QAbstractButton * bt,radiogroup->buttons()) {
            if(bt->text()==DocType){
                bt->setChecked(true);
            }
        }
        SavePath(id);
        ui->BuildBox->setCurrentIndex(ui->BuildBox->findText(DocBuildComand));
        ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData(DocPreamble));
        ui->NameLine->setText(QFileInfo(DocumentPath).baseName());
        ui->DocumentContent->editor->setText(DocumentContent);
        ui->DocumentDescription->setText(DocDescrition);
    }
    if(isImportMode){
        connect(ui->DocumentContent->editor,&QTextEdit::textChanged,this,[=](){
            if(!ui->NameLine->text().isEmpty() && textChanged && !ui->DocumentContent->editor->toPlainText().isEmpty()){
                DocContent = FileCommands::ClearDocumentContent(ui->DocumentContent->editor->toPlainText());
            }
        });
    }
    on_DocumentContent_cursorPositionChanged();
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
//    ui->PdfPreview->setChecked(false);
    ui->splitter_3->setSizes(QList<int>({ui->splitter_3->height(),0}));

    toolbar = new FileToolBar(ui->DocumentContent,false);
    toolbar->ShowMetadata->setVisible(false);
    toolbar->BuildFile->setVisible(false);
    toolbar->ShowPdf->setChecked(true);
    ui->horizontalLayout_5->insertWidget(3,toolbar);
    connect(toolbar->ShowPdf,&QAction::triggered,this,[&](bool checked){
        ui->splitter_3->setSizes(QList<int>({ui->splitter_3->height()*0.5*(checked),ui->splitter_3->height()*0.5*checked}));
    });
//    connect(toolbar,SIGNAL(Build()),this,SLOT(onRebuild()));
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
    delete FilesProxyModel;
    delete FilesModel;
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
    DataTex::updateTableView(DocumentTable,SqlFunctions::FilterDatabaseDocuments,currentbase,this);
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
    DocumentFileName = DocumentTable->model()->data(DocumentTable->model()->index(row,0)).toString();
    DocFileType = DocumentTable->model()->data(DocumentTable->model()->index(row,1)).toString();
    DocumentFilePath = DocumentTable->model()->data(DocumentTable->model()->index(row,5)).toString();
    FileCommands::ShowPdfInViewer(DocumentFilePath,view);
}

void NotesDocuments::on_CreateBasicFolder_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(Basic(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::Basic(QString newfolder)
{
    QString Path;
    QStringList list;
    if(level > 0){
        list = ui->pathline->text().split(QDir::separator());
        list.removeLast();
        list.removeLast();
        Path = list.join(QDir::separator());
//        qDebug()<<level<<Path;
    }
    else {
        Path = QFileInfo(DataTex::CurrentDocumentDatabasePath).absolutePath();
    }
    QDir dir(Path+QDir::separator()+newfolder);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    else{
        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
    }
}

void NotesDocuments::on_OkbuttonBoxFolders_accepted()
{
    if(ui->NameLine->isEnabled()){
        QString texname = ui->NameLine->text();
        QString name;
        if(!texname.isEmpty() && !isWhiteSpace(texname)){
            name = ui->pathline->text()+texname+".tex";
            QFileInfo NewDocument(name);
            QDir path = NewDocument.absoluteDir();
            if (!path.exists())
                path.mkpath(".");
            QString Content = ui->DocumentContent->editor->toPlainText();
            QString pathpdf = ui->pathline->text();
            QString FilePath = pathpdf+texname+".tex";
            QString Title = ui->TitleLine->text();
            QString date = QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm");
            QStringList valuesList;
            valuesList << texname<<Title<<DocumentType<<BasicFolder<<SubFolder<<SubSubFolder<<FilePath<<date<<Content<<
                ui->PreambleBox->currentData().toString()<<ui->BuildBox->currentText()<<"0"<<""<<ui->DocumentDescription->toPlainText();
            QString Values = "(\""+valuesList.join("\",\"")+"\")";
            QSqlQuery AddDocument(currentbase);
            AddDocument.exec(QString("INSERT OR IGNORE INTO \"Basic_Folders\" (\"Name\") VALUES (\"%1\")").arg(BasicFolder));
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders\" (Name) VALUES(:subfolder);"));
            AddDocument.bindValue(":subfolder",SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders_per_Basic\" (\"Sub_Id\",\"Basic_Id\") VALUES(:sub,:basic);"));
            AddDocument.bindValue(":basic",BasicFolder);
            AddDocument.bindValue(":sub",SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders\" (\"Name\") VALUES(:subsub);"));
            AddDocument.bindValue(":subsub",SubSubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders_per_Sub_perBasic\" (\"Subsub_Id\",\"Sub_Id\",\"Basic_Id\") VALUES(:subsub,:sub,:basic);"));
            AddDocument.bindValue(":subsub",SubSubFolder);
            AddDocument.bindValue(":sub",SubFolder);
            AddDocument.bindValue(":basic",BasicFolder);
            AddDocument.exec();
            AddDocument.exec(QString("INSERT OR IGNORE INTO Documents "
                                    "(Id,Title,Document_Type,Basic_Folder,SubFolder,SubsubFolder,Path,Date,Content,Preamble,BuildCommand,NeedsUpdate,Bibliography,Description) "
                                    "VALUES %1;").arg(Values));
            qDebug()<<QString("INSERT OR IGNORE INTO \"Documents\" "
                              "(Id,Title,Document_Type,Basic_Folder,SubFolder,SubsubFolder,Path,Date,Content,Preamble,BuildCommand,NeedsUpdate,Bibliography) "
                              "VALUES %1;").arg(Values);
            qDebug()<<currentbase;
            if(!CloneMode){
                emit createnewdocument(FilePath,Content);
            }
            else{
                QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
                emit clonedocument(FilePath,Content,DataTex::GlobalDocsDatabaseList[database]);
            }
//            if(ui->InsertFilesCheckBox->isChecked()){
//                emit insertfiles();
//            }
            QString BibliographyFile = FilePath;
            BibliographyFile.replace(".tex",".bib");
            QFile bibfile(BibliographyFile);
            bibfile.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream BibContent(&bibfile);
            BibContent.flush();
            BibContent << "%# Bib File : "+texname+"\n\n%# End of bib file "+texname;
            bibfile.close();

            CsvFunctions::WriteDocDataToCSV(FilePath,currentbase);

            QSqlQuery insertTag(currentbase);
            tags = tagLine->GetTags();
            foreach(QString tag,tags){
                if(!tag.isEmpty()){
                    insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
                    insertTag.exec("INSERT OR IGNORE INTO Tags_per_Document (Tag_Id,Document_Id) VALUES (\""+tag+"\",\""+QFileInfo(FilePath).baseName()+"\")");
                }
            }

            QString newDocPath = ui->pathline->text()+ui->NameLine->text()+".tex";
            if(CloneMode){QFile::copy(DocumentPath.left(DocumentPath.lastIndexOf("."))+".pdf",
                            newDocPath.left(newDocPath.lastIndexOf("."))+".pdf");}
            accept();
        }
        else if (texname.isEmpty()==true || isWhiteSpace(texname) ==true){
        QMessageBox::warning(this,tr("Error"),tr("Select a file name.\nThe name is "
                                           "empty or contains\nempty space or dot \".\"."),QMessageBox::Ok);}
    }
    else if(ui->NameLine->isEnabled()==false){accept();}
}

void NotesDocuments::on_OkbuttonBoxFolders_rejected()
{
    reject();
}

void NotesDocuments::Folder(bool checked)
{
    if(checked){
        ui->removeButton->setEnabled(true);
        QRadioButton *btn = static_cast<QRadioButton *>(sender());
        DocumentType = btn->text();
        ui->NameLine->setEnabled(true);
        ui->BuildBox->setEnabled(true);
        ui->PreambleBox->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(0);
    }
}

bool NotesDocuments::isWhiteSpace(const QString & str)
{
  return QRegExp("\t | \n | \v | \f | \r | |.").exactMatch(str);
}

void NotesDocuments::EditBasicForders_DataTex(QString Line)
{
//    QString BasicFolder = ui->FoldersStructureView->currentItem()->text();
//    int row = ui->FoldersStructureView->currentRow();
//        QStringList folders;
//        for (int i=0;i<ui->FoldersStructureView->rowCount();i++) {
//            folders.append(ui->FoldersStructureView->item(i,0)->text());
//        }
//        if(folders.contains(Line)){
//            QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(Line));
//        return;}
//        ui->FoldersStructureView->item(row,0)->setText(QString(Line));
//        QFile::rename(NotesPath+BasicFolder+QDir::separator(),NotesPath+Line+QDir::separator());
//        QSqlQuery EditQuery(currentbase);
//        EditQuery.exec("PRAGMA foreign_keys = ON");
//        EditQuery.prepare(QString("UPDATE \"Basic_Folders\" SET \"Name\" = :newname WHERE \"Name\" = :name;"));
//        EditQuery.bindValue(":newname",Line);
//        EditQuery.bindValue(":name",BasicFolder);
//        EditQuery.exec();
//        EditQuery.prepare(QString("UPDATE \"Documents\" SET \"Path\" = replace(\"Path\",:basic,:line) WHERE \"Basic_Folder\" = :line;"));
//        EditQuery.bindValue(":line",Line);
//        EditQuery.bindValue(":basic",BasicFolder);
//        EditQuery.exec();
//        emit pathline(Line);
}

//void NotesDocuments::on_EditBasicButton_clicked()
//{
//    QString BasicFolder = ui->FoldersStructureView->currentItem()->text();
//    newFolder = new addfolder(this);
//    newFolder->EditFolder(BasicFolder);
//    connect(this,SIGNAL(pathline(QString)),newFolder,SLOT(EditFolder(QString)));
//    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(EditBasicFolders_DataTex(QString)));
//    newFolder->show();
//    newFolder->activateWindow();
//}

QString NotesDocuments::SavePath(const QModelIndex &index)
{
    ui->pathline->setEnabled(true);
    ui->OpenPath->setEnabled(true);
    QString path = folderModel->filePath(index)+QDir::separator();
    QString  temp_path = path;
    temp_path.remove(QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator());
    QStringList Folders = temp_path.split(QDir::separator());
    BasicFolder = Folders[0];
    if(Folders.count()>1){
        if(!Folders[1].isNull() && !Folders[1].isEmpty()){
            SubFolder = Folders[1];
        }
        else{
            SubFolder = "-";
            SubSubFolder = "-";
        }
    }
    if(Folders.count()>2){
        if(!Folders[2].isNull() && !Folders[2].isEmpty()){
            SubSubFolder = Folders[2];
        }
        else{
            SubSubFolder = "-";
        }
    }
    qDebug()<<BasicFolder<<SubFolder<<SubSubFolder;
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
    QDir dir(NotesPath);
    for(result=0;dir.cdUp();++result){}
    level = depth-result;
//    qDebug()<<depth-result<<index;
    tagLine->setEnabled(true);
}


void NotesDocuments::on_CreateSubFolder_clicked()
{
    addfolder * newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(NewSubFolder(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::NewSubFolder(QString newfolder)
{
    QDir dir(ui->pathline->text()+newfolder);
    if (!dir.exists()){
        dir.mkpath(".");
        QModelIndex in = ui->FoldersStructureView->currentIndex();
        ui->FoldersStructureView->expand(in);
    }
    else{
        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
    }
}


void NotesDocuments::on_RemButton_clicked()
{
    DataTex::FunctionInProgress();
//    QCheckBox *cb = new QCheckBox("Μετάβαση στο φάκελο");
//    QMessageBox msgbox;
//    msgbox.setText(tr("The folder will be deleted from the database!\nDelete the folder manually in case it contains files.\nDo you wish to proceed?"));
//    msgbox.setIcon(QMessageBox::Icon::Question);
//    msgbox.addButton(QMessageBox::Ok);
//    msgbox.addButton(QMessageBox::Cancel);
//    msgbox.setDefaultButton(QMessageBox::Cancel);
//    msgbox.setCheckBox(cb);
//    if (msgbox.exec() == QMessageBox::Ok) {
//        QString DeleteQuery;
//        QSqlQuery RemQuery(currentbase);
//        RemQuery.exec("PRAGMA foreign_keys = ON");
//        switch (level) {
//        case 0:
//            DeleteQuery = QString("DELETE FROM \"Basic_Folders\" WHERE \"Name\" = \"%1\";").arg(BasicFolder);
//            break;
//        case 1:
//            DeleteQuery = QString("DELETE FROM \"SubFolders_per_Basic\" WHERE \"Sub_Id\" = \"%1\";").arg(SubFolder);
//            break;
//        case 2:
//            DeleteQuery = QString("DELETE FROM \"SubsubFolders_per_Sub_perBasic\" WHERE \"Subsub_Id\" = \"%1\";").arg(SubSubFolder);
//            break;
//        }
//        RemQuery.exec(DeleteQuery);
//         if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+NotesPath));}
//    }
}

void NotesDocuments::on_EditButton_clicked()
{
    DataTex::FunctionInProgress();
}

void NotesDocuments::on_addButton_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(AddDocType(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::AddDocType(QString docType)
{
    int rows = ui->gridLayout_4->rowCount();
    int widgets = ui->gridLayout_4->count();
    QSqlQuery AddDoc(currentbase);
    AddDoc.prepare(QString("INSERT INTO Document_Types (Name) VALUES (\"%1\")").arg(docType));
    if(AddDoc.exec()){
        QRadioButton * radio = new QRadioButton(docType);
        radioList.append(radio);
        radiogroup->addButton(radio);
        ui->gridLayout_4->addWidget(radio,rows-1+(widgets+1)%2,widgets%2);
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    else{
        QMessageBox::warning(this,tr("Error"),tr("The document type %1 already exists.").arg(docType),QMessageBox::Ok);
    }
}

void NotesDocuments::on_removeButton_clicked()
{
    foreach(QRadioButton * bt,radioList){
        if(bt->isChecked()){
            radiogroup->removeButton(bt);
            radioList.removeAll(bt);
            QSqlQuery removeFileType(currentbase);
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
//    qDebug()<<DataTex::GlobalDocsDatabaseList;
    QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
    currentbase = DataTex::GlobalDocsDatabaseList[database];
    NotesPath = QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator();
    GetDocTypes();
    LoadFolderStructure();
    LoadDocTable();
//    qDebug()<<DataTex::GlobalDocsDatabaseList;
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
    QSqlQuery DocTypes(currentbase);
    DocTypes.exec(SqlFunctions::GetDocumentTypes);
    while(DocTypes.next()){
        FileTypeList.append(DocTypes.value(0).toString());
    }
    double items = FileTypeList.count();
    int half = ceil(items/2);
    for (int i=0;i<half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i),i,0);
        radioList.at(i)->setEnabled(false);
        radiogroup->addButton(radioList.at(i));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    for (int i=0;i<items-half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i+half));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i+half),i,1);
        radioList.at(i+half)->setEnabled(false);
        radiogroup->addButton(radioList.at(i+half));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);

}

void NotesDocuments::LoadFolderStructure()
{
    folderModel = new QFileSystemModel(this);
    folderModel->setRootPath(QFileInfo(currentbase.databaseName()).absolutePath());
    folderModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    folderModel->setNameFilterDisables(false);
    folderModel->insertRow(0);
    ui->FoldersStructureView->setModel(folderModel);
    QModelIndex index = folderModel->index(QFileInfo(currentbase.databaseName()).absolutePath());
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
    QSqlTableModel * DocumentModel = new QSqlTableModel(this,currentbase);
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
//    FilesModel = new QSqlQueryModel(this);
//    FilesProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
//    QSqlQuery DatabaseQuery(currentbase_Exercises);
//    DatabaseQuery.exec(SqlFunctions::ShowAllDatabaseFiles);
//    FilesModel->setQuery(DatabaseQuery);
//    FilesProxyModel->setSourceModel(FilesModel);//Φιλτράρισμα και ταξινόμηση ---
//    if(filesSorting){
//        FilesTable->setModel(FilesProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    else{
//        FilesTable->setModel(FilesModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    FilesTable->show();
    FilesTable->filterTable(SqlFunctions::ShowAllDatabaseFiles,currentbase_Exercises,filesSorting);
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
//        if(ui->ShowRandomPdf->isChecked()){
            FileCommands::ShowPdfInViewer(file,view);
//        }
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

void NotesDocuments::updateFilter_files(/*size_t column, const QString& value*/QStringList values)
{
    SqlFunctions::FilterTable(Database_FileTableFields,values);
    int columns = Database_FileTableFields.count();
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase_Exercises,this);
    //Filter
//    FilesProxyModel->setSourceModel(FilesTable->model());//Φιλτράρισμα και ταξινόμηση ---
//    QSqlQuery DatabaseQuery(currentbase_Exercises);
//    DatabaseQuery.exec(SqlFunctions::FilesTable_UpdateQuery);
//    FilesModel->setQuery(DatabaseQuery);
//    FilesProxyModel->setSourceModel(FilesModel);//Φιλτράρισμα και ταξινόμηση ---
//    if(filesSorting){
//        FilesTable->setModel(FilesProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    else{
//        FilesTable->setModel(FilesModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }

//    FilesTable->setSortingEnabled(filesSorting);//Φιλτράρισμα και ταξινόμηση ---
    FilesTable->filterTable(SqlFunctions::FilesTable_UpdateQuery,currentbase_Exercises,filesSorting);
    //------
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
}

void NotesDocuments::FilesTable_selectionchanged()
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
    ExercisesInsideDocument.clear();
    DatabasesInsideDocument.clear();
    QString DocContent = ui->DocumentContent->editor->toPlainText();
    QTextStream textstream(&DocContent);
    while (!textstream.atEnd()){
        QString LineText=textstream.readLine();
        if(LineText.contains("%# Database File : ")){ExercisesInsideDocument.append(LineText.remove("%# Database File : "));}
        if(LineText.contains("%@ Database source: ")){DatabasesInsideDocument.append(LineText.remove("%@ Database source: "));}
    }
    DatabasesInsideDocument.removeDuplicates();
    QStringList Databases =
        SqlFunctions::Get_StringList_From_Query(QString("SELECT Path FROM Databases WHERE FileName IN (\"%1\")").arg(DatabasesInsideDocument.join("\",\""))
                                                ,DataTex::DataTeX_Settings);
    QString files = "(\""+ExercisesInsideDocument.join("\",\"")+"\")";
    QSqlQueryModel * Files = new QSqlQueryModel(this);
    QStringList datalist;// = {SqlFunctions::ShowFilesInADocument.arg(files,QFileInfo(DataTex::CurrentDataBasePath).baseName())};
    QString query;
    QSqlQuery FilesQuery(DataTex::CurrentTexFilesDataBase);
    //    for (int i=0;i<DatabasesInsideDocument.count();i++) {
    //        if(DatabasesInsideDocument.at(i)!=QFileInfo(DataTex::CurrentDataBasePath).baseName()) {
    //            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(Databases.at(i),DatabasesInsideDocument[i]));
    //            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(files,DatabasesInsideDocument[i]));
    //        }
    //    }
    for (int i=0;i<DatabasesInsideDocument.count();i++) {
        QString name = (Databases.at(i)!=DataTex::CurrentDataBasePath) ? QFileInfo(Databases.at(i)).baseName() : "main" ;
        datalist.append(SqlFunctions::ShowFilesInADocument.arg(files,DatabasesInsideDocument[i],name));
        if(Databases.at(i)!=DataTex::CurrentDataBasePath) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(Databases.at(i),QFileInfo(Databases.at(i)).baseName()));
        }
    }
    query = datalist.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY df.Id ");
//    qDebug()<<query+" ORDER BY df.Id ";
    Files->setQuery(FilesQuery);
    ui->filesSelected->setModel(Files);
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
//        qDebug()<<list;
    });
    filesTagLine->setVisible(false);
    connect(ui->FilesTagFilter, &QPushButton::toggled, this, [=](bool checked){
        filesTagLine->setVisible(checked);
    });
}

void NotesDocuments::on_checkBox_clicked(bool checked)
{
    if(checked){
        LoadDatabaseFiles(currentbase_Exercises,SqlFunctions::ShowAllDatabaseFiles);
        ui->FilesDatabasesCombo->setEnabled(checked);
        FilesTable->filterHeader()->adjustPositions();
    }
    else {
        ui->FilesDatabasesCombo->setEnabled(checked);
        LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
        FilesTable->filterHeader()->adjustPositions();
    }
}

void NotesDocuments::on_FilesDatabasesCombo_activated(int index)
{
    if(index>-1 && ui->checkBox->isChecked()){
        for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
            if(DataTex::GlobalFilesDatabaseList.values()[i].databaseName()==ui->FilesDatabasesCombo->currentData().toString()){
                index = i;
            }
        }
        currentbase_Exercises = DataTex::GlobalFilesDatabaseList.values().at(index);
        LoadDatabaseFiles(currentbase_Exercises,SqlFunctions::ShowAllDatabaseFiles);
    }
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    ui->FilesTagFilter->setChecked(false);
    CreateCustomTagWidget(currentbase_Exercises);
    FilesTable->filterHeader()->adjustPositions();
}

void NotesDocuments::on_DocumentContent_cursorPositionChanged()
{
    LatexTextEdit::clearFormat(ui->DocumentContent->editor);
    DatabaseFileContent.clear();
    positions.clear();
    int start = -1;
    int end = -1;
    int lineNumber = -1;
    QString FileContent = ui->DocumentContent->editor->toPlainText();
    QTextStream parse(&FileContent);
    while(!parse.atEnd()){
        lineNumber++;
        QString line = parse.readLine();
        if(line.startsWith("%# Database File")){
            start = lineNumber;
        }
        if(line.startsWith("%# End of file")){
            end = lineNumber;
            positions.insert(line.remove("%# End of file "),{start,end});
        }
    }
    int line = ui->DocumentContent->editor->textCursor().blockNumber();
    int fileIndex =-1;
    for (int k=0;k<positions.count();k++) {
        if(line>=positions.values()[k][0] && line <=positions.values()[k][1]){
            fileIndex = k;
            isCursorInsideFile = true;
        }
        else{
            isCursorInsideFile = false;
        }
    }
    if(isCursorInsideFile){
            QString file = positions.key({positions.values()[fileIndex][0],positions.values()[fileIndex][1]});
            QAbstractItemModel * m = ui->filesSelected->model();
            QModelIndex ix = ui->filesSelected->currentIndex();
            while (ui->filesSelected->model()->canFetchMore(ix))
                ui->filesSelected->model()->fetchMore(ix);
            QModelIndexList matchList = m->match(m->index(0,0), Qt::DisplayRole,
                                                 file, -1,  Qt::MatchFlags(Qt::MatchContains|Qt::MatchWrap));
            FileCommands::ShowPdfInViewer(ui->filesSelected->model()->data(ui->filesSelected->model()->index(matchList[0].row(),5)).toString(),view);
            for (int i = positions.values()[fileIndex][0];i<positions.values()[fileIndex][1]+1;i++) {
                QTextBlock block = ui->DocumentContent->editor->document()->findBlockByNumber(i);
                QTextCursor cursor(block);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor(240,245,245));
                cursor.setBlockFormat(blockFormat);
                DatabaseFileContent.append(block.text()+"\n");
            }
            ui->removeButton->setEnabled(true);
        }
        else{
            LatexTextEdit::clearFormat(ui->DocumentContent->editor);
            ui->removeButton->setEnabled(false);
        }

//    qDebug()<<positions;
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
        if(line.startsWith("%# Database File")){
            start = lineNumber;
        }
        if(line.startsWith("%# End of file")){
            end = lineNumber;
            positions.insert(line.remove("%# End of file "),{start,end});
        }
    }
    qDebug()<<"Positions"<<positions;
}

