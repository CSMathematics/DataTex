#include "addfiletoeditor.h"
#include "ui_addfiletoeditor.h"

AddFileToEditor::AddFileToEditor(QWidget *parent,QString currentTexFile, QString BuildCommand) :
    QDialog(parent),
    ui(new Ui::AddFileToEditor)
{
    ui->setupUi(this);
    CurrentDatabaseFile = currentTexFile;
    CurrentBuildCommand = BuildCommand;
    RandomFilesToKeep = 0;
    foreach (QSqlDatabase database, DataTex::GlobalFilesDatabaseList.values()) {
        QString databaseName = QFileInfo(database.databaseName()).baseName();
        ui->FilesDatabasesCombo->addItem(DataTex::GlobalFilesDatabaseListNames[databaseName],QVariant(database.databaseName()));
    }
    currentbase = DataTex::CurrentTexFilesDataBase;
    ui->FilesDatabasesCombo->setCurrentText(DataTex::GlobalFilesDatabaseListNames[QFileInfo(DataTex::CurrentDataBasePath).baseName()]);
    ui->removeButton->setEnabled(false);
    ui->addEverything->setEnabled(false);
    setWindowTitle(tr("Insert files to document : ")+QFileInfo(CurrentDatabaseFile).fileName());
    view = new QPdfViewer(this);
    ui->verticalLayout_6->addWidget(view);
    view->show();
    DocView = new QPdfViewer(this);
    ui->splitter_2->addWidget(DocView);
    DocView->show();
    CreateCustomTagWidget(currentbase);
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
    QString FileContent;
    QFile file(CurrentDatabaseFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->editor->setText(FileContent);
    ui->DocumentContent->editor->setDocumentTitle(CurrentDatabaseFile);
    connect(FilesTable,&QTableView::doubleClicked,this,[=](){on_addButton_clicked();});
    FileCommands::ShowPdfInViewer(CurrentDatabaseFile,DocView);
    SelectedFilesInDocument();
    ui->AddRandomFiles->setEnabled(false);
    ui->moveDown->setEnabled(false);
    ui->moveUp->setEnabled(false);
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
            this, &AddFileToEditor::filesSelected_SelectionChanged);
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
    connect(ui->DocumentContent->editor,&QTextEdit::cursorPositionChanged,this,&AddFileToEditor::on_DocumentContent_cursorPositionChanged);
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
    connect(ui->ShowPdfSide,&QPushButton::toggled,this,[&](bool checked){
        ui->splitter_2->setOrientation(static_cast<Qt::Orientation>(2-checked));
        ui->splitter->setHidden(checked);
    });

    toolbar = new FileToolBar(ui->DocumentContent,false);
    toolbar->ShowMetadata->setVisible(false);
    toolbar->ShowPdf->setChecked(true);
    ui->horizontalLayout_5->insertWidget(0,toolbar);
    connect(toolbar->ShowPdf,&QAction::triggered,this,[&](bool checked){
        ui->splitter_2->setSizes(QList<int>({ui->splitter_2->height()*0.5*(checked),ui->splitter_2->height()*0.5*checked}));
    });
    connect(toolbar,SIGNAL(Build()),this,SLOT(onRebuild()));
    toolbar->show();
    connect(ui->DocumentContent->editor->document(),&QTextDocument::blockCountChanged,this,[=](){
        findFilePositions();
    });
    findFilePositions();
    ui->DocPdf->setChecked(true);
    connect(ui->FilePdf,&QPushButton::toggled,this,[&](bool checked){
        if(checked){
            FileCommands::ShowPdfInViewer(singlefile,DocView);
        }
        else{
            FileCommands::ShowPdfInViewer(CurrentDatabaseFile,DocView);
        }
    });
    connect(ui->numOfFilesSpin,QOverload<int>::of(&QSpinBox::valueChanged),this,[&](int value){
        NumOfFiles = value;
    });
}

AddFileToEditor::~AddFileToEditor()
{
    delete ui;
    delete view;
    delete DocView;
    delete tableModel;
    delete FilesTable;
    delete RightClick;
    delete FilesRightClickMenu;
    delete filesTagLine;
    delete FilesProxyModel;
    delete FilesModel;
    delete toolbar;
}

void AddFileToEditor::LoadDatabaseFiles(QSqlDatabase database,QString query)
{
//    FilesModel = new QSqlQueryModel(this);
//    FilesProxyModel = new QSortFilterProxyModel(this);//Φιλτράρισμα και ταξινόμηση ---
//    QSqlQuery DatabaseQuery(currentbase);
//    DatabaseQuery.exec(SqlFunctions::ShowAllDatabaseFiles);
//    FilesModel->setQuery(DatabaseQuery);
//    FilesProxyModel->setSourceModel(FilesModel);//Φιλτράρισμα και ταξινόμηση ---
//    if(filesSorting){
//        FilesTable->setModel(FilesProxyModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
//    else{
//        FilesTable->setModel(FilesModel);// Φιλτράρισμα και ταξινόμηση ---Filtering
//    }
    FilesTable->filterTable(SqlFunctions::ShowAllDatabaseFiles,currentbase,filesSorting);
    FilesTable->show();
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &AddFileToEditor::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &AddFileToEditor::updateFilter);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void AddFileToEditor::updateFilter(QStringList values)
{
    SqlFunctions::FilterTable(Database_FileTableFields,values);
    int columns = Database_FileTableFields.count();
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase,this);
    //Filter
//    FilesProxyModel->setSourceModel(FilesTable->model());//Φιλτράρισμα και ταξινόμηση ---
//    QSqlQuery DatabaseQuery(currentbase);
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
    FilesTable->filterTable(SqlFunctions::FilesTable_UpdateQuery,currentbase,filesSorting);
    //------
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AddFileToEditor::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
}

void AddFileToEditor::FilesTable_selectionchanged()
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

void AddFileToEditor::on_Okbutton_rejected()
{
    reject();
}

void AddFileToEditor::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Close window"),tr("Do you want to close the window?"),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void AddFileToEditor::AddFiles(int row)
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

void AddFileToEditor::on_addButton_clicked()
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

void AddFileToEditor::on_removeButton_clicked()
{
    ui->removeButton->setEnabled(false);
    QString Content = ui->DocumentContent->editor->toPlainText();
    Content.remove(DatabaseFileContent);
    ui->DocumentContent->editor->setText(Content);
    SelectedFilesInDocument();
}

void AddFileToEditor::on_Okbutton_accepted()
{
    ExercisesInsideDocument.clear();
    DatabasesInsideDocument.clear();
    QStringList paths;
    QString content = ui->DocumentContent->editor->toPlainText();
    QTextStream textstream(&content);
    while (!textstream.atEnd()){
        QString LineText=textstream.readLine();
        if(LineText.contains("%# Database File : ")){ExercisesInsideDocument.append(LineText.remove("%# Database File : "));}
        if(LineText.contains("%@ Database source: ")){DatabasesInsideDocument.append(LineText.remove("%@ Database source: "));}
    }
    QStringList valuesQuery;
    for (int i = 0;i<ExercisesInsideDocument.count();i++ ) {
        valuesQuery.append("(\""+QFileInfo(CurrentDatabaseFile).baseName()+"\",\""+ExercisesInsideDocument[i]+"\",\""+DatabasesInsideDocument[i]+"\")");
    }
    QSqlQuery writeExercisesPerDatabase(DataTex::CurrentDocumentDataBase);
    writeExercisesPerDatabase.exec("INSERT OR IGNORE INTO Files_per_Document (Document_Id,File_Id,Files_Database_Source) VALUES "+valuesQuery.join(","));
    writeExercisesPerDatabase.exec(QString("DELETE FROM Files_per_Document WHERE Document_Id = \"%1\" AND File_Id NOT IN (\""+ExercisesInsideDocument.join("\",\"")+"\")").arg(CurrentDatabaseFile));
    QSettings settings;
    QString datatexpath = settings.value("Application_Settings/datatexpath").toString();
    DataTex::DBBackUp(DataTex::CurrentDocumentDatabasePath,datatexpath+QFileInfo(DataTex::CurrentDocumentDatabasePath).fileName());
    accept();
}

void AddFileToEditor::onRebuild()
{
    ui->DocumentContent->toolBar->Save->trigger();
    FileCommands::CreateTexFile(CurrentDatabaseFile,0,"");
    FileCommands::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],CurrentDatabaseFile,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    FileCommands::ClearOldFiles(CurrentDatabaseFile);
    FileCommands::ShowPdfInViewer(CurrentDatabaseFile,DocView);
}

void AddFileToEditor::on_checkBox_clicked(bool checked)
{
    if(checked){
        LoadDatabaseFiles(currentbase,SqlFunctions::ShowAllDatabaseFiles);
        ui->FilesDatabasesCombo->setEnabled(checked);
        FilesTable->filterHeader()->adjustPositions();
    }
    else {
        ui->FilesDatabasesCombo->setEnabled(checked);
        LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
        FilesTable->filterHeader()->adjustPositions();
    }
}

void AddFileToEditor::on_FilesDatabasesCombo_activated(int index)
{
    if(index>-1 && ui->checkBox->isChecked()){
        for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
            if(DataTex::GlobalFilesDatabaseList.values()[i].databaseName()==ui->FilesDatabasesCombo->currentData().toString()){
                index = i;
            }
        }
        currentbase = DataTex::GlobalFilesDatabaseList.values().at(index);
        LoadDatabaseFiles(currentbase,SqlFunctions::ShowAllDatabaseFiles);
    }
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    ui->FilesTagFilter->setChecked(false);
    CreateCustomTagWidget(currentbase);
    FilesTable->filterHeader()->adjustPositions();
}

void AddFileToEditor::on_DocumentContent_cursorPositionChanged()
{
    LatexTextEdit::clearFormat(ui->DocumentContent->editor);
    DatabaseFileContent.clear();
    int line = ui->DocumentContent->editor->textCursor().blockNumber();
    int fileIndex =-1;
    for (int k=0;k<positions.count();k++) {
        if(line>=positions.values()[k][0] && line <=positions.values()[k][1]){
            fileIndex = k;
            isCursorInsideFile = true;
            break;
        }
        else{
            isCursorInsideFile = false;
        }
    }
    if(isCursorInsideFile){
        QString file = positions.key({positions.values()[fileIndex][0],positions.values()[fileIndex][1]});
        DataTex::SelectNewFileInModel(ui->filesSelected,file);
        if(ui->FilePdf->isChecked()){
            FileCommands::ShowPdfInViewer(singlefile,DocView);
        }
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
}

void AddFileToEditor::on_RefreshSelection_clicked()
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

void AddFileToEditor::SelectedFilesInDocument()
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
            this, &AddFileToEditor::filesSelected_SelectionChanged);
    DataTex::StretchColumns(ui->filesSelected,1.5);
}

void AddFileToEditor::on_RandomSelectionList_itemSelectionChanged()
{
    if(ui->RandomSelectionList->selectionModel()->hasSelection()){
        QString file = ui->RandomSelectionList->currentItem()->data(Qt::UserRole).toString();
        FileCommands::ShowPdfInViewer(file,DocView);
    }
}

void AddFileToEditor::on_AddRandomFiles_clicked()
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

int AddFileToEditor::CountRandomFiles()
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

int AddFileToEditor::CountModelRows()
{
    QAbstractItemModel * model = FilesTable->model();
    QModelIndex ix = FilesTable->currentIndex();
    while (model->canFetchMore(ix))
           model->fetchMore(ix);
    int rows = model->rowCount();
    return rows;
}

void AddFileToEditor::on_addEverything_clicked()
{
    foreach (QModelIndex index,FilesTable->selectionModel()->selectedRows(0)) {
        AddFiles(index.row());
    }
}

void AddFileToEditor::filesSelected_SelectionChanged()
{
    int row = ui->filesSelected->selectionModel()->currentIndex().row();
    singlefile = ui->filesSelected->model()->data(ui->filesSelected->model()->index(row,5)).toString();
    QString basename = QFileInfo(singlefile).baseName();
    QTextBlock block = ui->DocumentContent->editor->document()->findBlockByLineNumber(positions[basename][0]);
    QTextCursor newCursor(block);
    ui->DocumentContent->editor->setTextCursor(newCursor);
    if(ui->FilePdf->isChecked()){
        FileCommands::ShowPdfInViewer(singlefile,DocView);
    }
}


void AddFileToEditor::on_removeSelectedFile_clicked()
{
    on_removeButton_clicked();
}


void AddFileToEditor::CreateCustomTagWidget(QSqlDatabase database)
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

void AddFileToEditor::findFilePositions()
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
//    qDebug()<<"Positions"<<positions;
}
