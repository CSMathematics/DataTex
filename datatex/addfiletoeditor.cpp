#include "addfiletoeditor.h"
#include "ui_addfiletoeditor.h"
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QDirIterator>
#include <QVector>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QProcess>
#include <QTextBlock>
#include <QTextCursor>
#include "datatex.h"
#include "switch.h"



AddFileToEditor::AddFileToEditor(QWidget *parent,QString currentTexFile, QString BuildCommand) :
    QDialog(parent),
    ui(new Ui::AddFileToEditor)
{
    ui->setupUi(this);

    CurrentDatabaseFile = currentTexFile;
    currentbase = DataTex::CurrentTexFilesDataBase;
    CurrentBuildCommand = BuildCommand;
    RandomFilesToKeep = 0;
    int current = 0;
    for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
        ui->FilesDatabasesCombo->addItem(DataTex::GlobalFilesDatabaseListNames.values().at(i),QVariant(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()));
        if(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName() == DataTex::CurrentDataBasePath){current = i;}
    }
    ui->FilesDatabasesCombo->setCurrentIndex(current);
    ui->removeButton->setEnabled(false);
    ui->Save->setEnabled(false);
    ui->ShowPdfOfFile->setEnabled(false);
    ui->ShowRandomPdf->setEnabled(false);
    setWindowTitle(tr("Insert files to document : ")+QFileInfo(CurrentDatabaseFile).fileName());

    view = new PdfViewer(this);
    view->setMinimumWidth(620);
    ui->verticalLayout_6->addWidget(view);
    view->show();
    DocView = new PdfViewer(this);
    ui->verticalLayout_4->addWidget(DocView);
    DocView->show();
//    rview = new PdfViewer(this);
//    ui->verticalLayout_9->addWidget(rview);
//    rview->show();

    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,4,0);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
    DataTex::StretchColumns(FilesTable,1.5);

    ui->FilesDatabasesCombo->setEnabled(false);
    ui->splitter->setSizes(QList<int>({1,200}));
    ui->splitter_2->setSizes(QList<int>({1,300}));

    QString FileContent;
    QFile file(CurrentDatabaseFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->setText(FileContent);

    connect(FilesTable,&QTableView::doubleClicked,this,[=](){on_addButton_clicked();});
//    ui->verticalLayout_2->addWidget(ui->ToolBar);
    DataTex::loadImageFile(CurrentDatabaseFile,DocView);
    ui->Save->setEnabled(false);
    connect(ui->DocumentContent, &QTextEdit::textChanged, this,[=](){ui->Save->setEnabled(true);});
    SelectedFilesInDocument();
    ui->RefreshSelection->setEnabled(false);
    ui->numOfFilesSpin->setEnabled(false);
    ui->AddRandomFiles->setEnabled(false);
    ui->moveDown->setEnabled(false);
    ui->moveUp->setEnabled(false);
    connect(ui->RandomSelection,&QCheckBox::clicked,this,[=](bool checked){
        ui->FilesTabWidget->setCurrentIndex(3);
        ui->numOfFilesSpin->setEnabled(checked);
        ui->RefreshSelection->setEnabled(checked);
        ui->AddRandomFiles->setEnabled(ui->RandomSelectionList->count()!=0);
    });
    connect(ui->RandomSelectionList,&QListWidget::itemSelectionChanged,this,[=](){
        ui->ShowRandomPdf->setEnabled(true);
        ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
        ui->moveDown->setEnabled(ui->RandomSelectionList->currentRow()!=ui->RandomSelectionList->count()-1);
        ui->moveUp->setEnabled(ui->RandomSelectionList->currentRow()!=0);
    });
    connect(ui->RandomSelectionList,&QListWidget::itemClicked,this,[=](){
        ui->ShowRandomPdf->setEnabled(true);
        ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
        RandomFilesToKeep = CountRandomFiles();
        ui->moveDown->setEnabled(ui->RandomSelectionList->currentRow()!=ui->RandomSelectionList->count()-1);
        ui->moveUp->setEnabled(ui->RandomSelectionList->currentRow()!=0);
    });
    connect(ui->removeUnSelected,&QPushButton::clicked,this,[=](){
        for(int i=ui->RandomSelectionList->count()-1;i>-1;i--){
            if(ui->RandomSelectionList->item(i)->checkState() == Qt::Unchecked){
                ui->RandomSelectionList->takeItem(i);
            }
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
    connect(ui->ShowPdfOfFile,&QPushButton::toggled,this,[=](bool checked){
        ShowPdfOfFile(checked,ui->filesSelected->model()->index(ui->filesSelected->selectionModel()->currentIndex().row(),5).data().toString());
    });
    connect(ui->ShowRandomPdf,&QPushButton::toggled,this,[=](bool checked){
        ShowPdfOfFile(checked,ui->RandomSelectionList->currentItem()->data(Qt::UserRole).toString());
    });
    SelectedFileView = new PdfViewer(this);
    ui->verticalLayout_2->addWidget(SelectedFileView);
    SelectedFileView->hide();
    connect(ui->FilesTabWidget->tabBar(),&QTabBar::currentChanged,this,[&](){
        SelectedFileView->hide();
        ui->ShowPdfOfFile->setChecked(false);
        ui->ShowRandomPdf->setChecked(false);
    });
    connect(ui->SourceCode,&QPushButton::clicked,this,[&](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->PdfPreview,&QPushButton::clicked,this,[&](){ui->stackedWidget->setCurrentIndex(1);});

}

AddFileToEditor::~AddFileToEditor()
{
    delete ui;
    delete view;
    delete DocView;
}

void AddFileToEditor::LoadDatabaseFiles(QSqlDatabase database,QString query)
{
    QSqlQueryModel * DatabaseModel = new QSqlQueryModel(this);
    QSqlQuery DatabaseQuery(database);
    DatabaseQuery.exec(query);
    DatabaseModel->setQuery(DatabaseQuery);
    FilesTable->setModel(DatabaseModel);
    FilesTable->show();
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &AddFileToEditor::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &AddFileToEditor::updateFilter);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void AddFileToEditor::updateFilter(/*size_t column, const QString& value*/QStringList values)
{
    SqlFunctions::FilesTable_UpdateQuery.clear();
    DataTex::FilterTables_Queries(Database_FileTableFields);
    for(int i= 0;i<values.count();i++){
        if(!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!=""){
            SqlFunctions::FilesTable_UpdateQuery += " WHERE ";
            break;
        }
    }

    QStringList DataFields = QStringList();

    if(!values.at(0).isEmpty() && !values.at(0).isNull()){
        DataFields.append(" df.Id LIKE \"%"+values.at(0)+"%\" ");
    }
    if(!values.at(1).isEmpty() && !values.at(1).isNull()){
        DataFields.append(" ft.FileType LIKE \"%"+values.at(1)+"%\" ");
    }
    if(!values.at(2).isEmpty() && !values.at(2).isNull()){
        DataFields.append(" f.Name LIKE \"%"+values.at(2)+"%\" ");
    }
    if(!values.at(3).isEmpty() && !values.at(3).isNull()){
        DataFields.append(" c.Name LIKE \"%"+values.at(3)+"%\" ");
    }
    if(!values.at(4).isEmpty() && !values.at(4).isNull()){
        DataFields.append(" s.Name LIKE \"%"+values.at(4)+"%\" ");
    }
    if(!values.at(5).isEmpty() && !values.at(5).isNull()){
        DataFields.append(" et.Name LIKE \"%"+values.at(5)+"%\" ");
    }

    for (int i=6;i<Database_FileTableFields.count();i++) {
        if(!values.at(i).isEmpty() && !values.at(i).isNull())
            DataFields.append(" (\"df\".\""+Database_FileTableFields.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
    }

    int columns = Database_FileTableFields.count();
    if(!values.at(columns+2).isNull() && !values.at(columns+2).isEmpty()){
        QStringList tags = values.at(columns+2).split(",");
        DataFields.append(" t.Tag_Id IN (\""+tags.join("\",\"")+"\") ");
    }
    SqlFunctions::FilesTable_UpdateQuery += DataFields.join(" AND ");
    SqlFunctions::FilesTable_UpdateQuery += " GROUP BY df.Id ORDER BY df.rowid;";
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase,this);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AddFileToEditor::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
    ui->numOfFilesSpin->setMaximum(CountModelRows());
}

void AddFileToEditor::FilesTable_selectionchanged()
{
    int row  = FilesTable->selectionModel()->currentIndex().row();
    PreviewFile = FilesTable->model()->data(FilesTable->model()->index(row,7)).toString();
    QString fileContent = FilesTable->model()->data(FilesTable->model()->index(row,11)).toString();
//    QString preamble = FilesTable->model()->data(FilesTable->model()->index(row,12)).toString();
    QString buildCommand = FilesTable->model()->data(FilesTable->model()->index(row,13)).toString();
    QString file = PreviewFile;
    QString pdffile = file.replace(".tex",".pdf");
    if(!QFileInfo::exists(pdffile)){
//        DataTex::CreateTexFile(PreviewFile,0,""/*preamble*/);
//        DataTex::BuildDocument(DataTex::LatexCommands[buildCommand],PreviewFile,DataTex::LatexCommandsArguments[buildCommand],".tex");
//        DataTex::ClearOldFiles(PreviewFile);
        //Need to add a 'preamble' variable in CreateTexFile command
    }
    DataTex::loadImageFile(PreviewFile,view);
    ui->FileContent->setPlainText(fileContent);
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
    QTextCursor prev_cursor = ui->DocumentContent->textCursor();
    QString FileContent;
    QFile file(FilesTable->model()->data(FilesTable->model()->index(row,7)).toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->insertPlainText(FileContent+"\n\n");
    ui->DocumentContent->setTextCursor(prev_cursor);
    SelectedFilesInDocument();
}

void AddFileToEditor::on_addButton_clicked()
{
    QTextCursor prev_cursor = ui->DocumentContent->textCursor();
    QString FileContent;
    int row = FilesTable->currentIndex().row();
    QFile file(FilesTable->model()->data(FilesTable->model()->index(row,7)).toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->insertPlainText(FileContent+"\n\n");
    ui->DocumentContent->setTextCursor(prev_cursor);
    SelectedFilesInDocument();
}

void AddFileToEditor::on_removeButton_clicked()
{
    ui->removeButton->setEnabled(false);
    QString Content = ui->DocumentContent->toPlainText();
    Content.remove(DatabaseFileContent);
    ui->DocumentContent->setText(Content);
    SelectedFilesInDocument();
}

void AddFileToEditor::on_Okbutton_accepted()
{
    ExercisesInsideDocument.clear();
    DatabasesInsideDocument.clear();
    QStringList paths;
    QString content = ui->DocumentContent->toPlainText();
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
    QSqlQuery writeExercisesPerDatabase(DataTex::CurrentNotesFolderDataBase);
    writeExercisesPerDatabase.exec("INSERT OR IGNORE INTO Files_per_Document (Document_Id,File_Id,Files_Database_Source) VALUES "+valuesQuery.join(","));
    writeExercisesPerDatabase.exec(QString("DELETE FROM Files_per_Document WHERE Document_Id = \"%1\" AND File_Id NOT IN (\""+ExercisesInsideDocument.join("\",\"")+"\")").arg(CurrentDatabaseFile));
    SaveText();
    accept();
}

void AddFileToEditor::SaveText()
{
    QString FileContent = ui->DocumentContent->toPlainText();
    QFile file(CurrentDatabaseFile);
    file.resize(0);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream ContentStream(&file);
    ContentStream << FileContent;
    file.close();
    ui->Save->setEnabled(false);
//    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(CurrentDatabaseFile,FileContent);
}

void AddFileToEditor::on_Rebuild_clicked()
{
    SaveText();
    DataTex::CreateTexFile(CurrentDatabaseFile,0,"");
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],CurrentDatabaseFile,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(CurrentDatabaseFile);
    DataTex::loadImageFile(CurrentDatabaseFile,DocView);
}

void AddFileToEditor::on_checkBox_clicked(bool checked)
{
    if(checked){
        LoadDatabaseFiles(currentbase,SqlFunctions::ShowAllDatabaseFiles);
        ui->FilesDatabasesCombo->setEnabled(checked);
    }
    else {
        ui->FilesDatabasesCombo->setEnabled(checked);
        LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
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
}

void AddFileToEditor::on_DocumentContent_cursorPositionChanged()
{
    int start = -1;
    int end = -1;
    int lineNumber = -1;
    LatexTextEdit::clearFormat(ui->DocumentContent);
    positions.clear();
    DatabaseFileContent.clear();
    QString FileContent = ui->DocumentContent->toPlainText();
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
    int line = ui->DocumentContent->textCursor().blockNumber();
    int fileIndex =-1;
    for (int k=0;k<positions.count();k++) {
        if(line>=positions.values()[k][0] && line <=positions.values()[k][1]){
            fileIndex = k;
            for (int i = positions.values()[fileIndex][0];i<positions.values()[fileIndex][1]+1;i++) {
                QTextBlock block = ui->DocumentContent->document()->findBlockByNumber(i);
                QTextCursor cursor(block);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor(240,245,245));
                cursor.setBlockFormat(blockFormat);
                DatabaseFileContent.append(block.text()+"\n");
            }
            ui->removeButton->setEnabled(true);
            break;
        }
        else{
            LatexTextEdit::clearFormat(ui->DocumentContent);
            ui->removeButton->setEnabled(false);
        }
    }
}

void AddFileToEditor::on_Save_clicked()
{
    SaveText();
    ui->Save->setEnabled(false);
}

void AddFileToEditor::on_DocumentContent_textChanged()
{
    ui->Save->setEnabled(true);
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
    QStringList files;
    for (int i = 0;i<ui->RandomSelectionList->count();i++) {
        files.append(ui->RandomSelectionList->item(i)->text());
    }
    int count = ui->RandomSelectionList->count();
    for (int i=0;i<NumOfFiles-RandomFilesToKeep;i++) {
        QString file = FilesTable->model()->data(FilesTable->model()->index(list[i],7)).toString();
        if(!files.contains(QFileInfo(file).baseName())){
            ui->RandomSelectionList->addItem(QFileInfo(file).baseName());
            ui->RandomSelectionList->item(count+i)->setData(Qt::UserRole,file);
            ui->RandomSelectionList->item(count+i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            ui->RandomSelectionList->item(count+i)->setCheckState(Qt::Checked);
        }
    }
    ui->SelectedFilesLabel->setText("Add "+QString::number(CountRandomFiles())+" files");
    randomlist = list;
    ui->AddRandomFiles->setEnabled(true);
    RandomFilesToKeep = ui->RandomSelectionList->count();
}

void AddFileToEditor::on_numOfFilesSpin_valueChanged(int arg1)
{
    NumOfFiles = arg1;
}

void AddFileToEditor::SelectedFilesInDocument()
{
    ExercisesInsideDocument.clear();
    DatabasesInsideDocument.clear();
    QString DocContent = ui->DocumentContent->toPlainText();
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
    QStringList datalist = {SqlFunctions::ShowFilesInADocument.arg(files,QFileInfo(DataTex::CurrentDataBasePath).baseName())};
    QString query;
    QSqlQuery FilesQuery(DataTex::CurrentTexFilesDataBase);
    for (int i=0;i<DatabasesInsideDocument.count();i++) {
        if(DatabasesInsideDocument.at(i)!=QFileInfo(DataTex::CurrentDataBasePath).baseName()) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(Databases.at(i),DatabasesInsideDocument[i]));
            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(files,DatabasesInsideDocument[i]));
        }
    }
    query = datalist.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY df.Id ");
    qDebug()<<query+" ORDER BY df.Id ";
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
        if(ui->ShowRandomPdf->isChecked()){
            DataTex::loadImageFile(file,SelectedFileView);
        }
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
    int rows = FilesTable->model()->rowCount();
    for(int i=0;i<rows;i++){
        AddFiles(i);
    }
}

void AddFileToEditor::filesSelected_SelectionChanged()
{
    ui->ShowPdfOfFile->setEnabled(true);
    ui->removeSelectedFile->setEnabled(true);
    int row = ui->filesSelected->selectionModel()->currentIndex().row();
    QString file = ui->filesSelected->model()->data(ui->filesSelected->model()->index(row,0)).toString();
    QTextBlock block = ui->DocumentContent->document()->findBlockByLineNumber(positions[file][0]);
    QTextCursor newCursor(block);
    ui->DocumentContent->moveCursor(QTextCursor::End);
    ui->DocumentContent->setTextCursor(newCursor);
    if(ui->ShowPdfOfFile->isChecked()){
        DataTex::loadImageFile(ui->filesSelected->model()->index(row,5).data().toString(),SelectedFileView);
    }
}


void AddFileToEditor::on_removeSelectedFile_clicked()
{
    on_removeButton_clicked();
}


void AddFileToEditor::ShowPdfOfFile(bool checked,QString file)
{
    if(checked){
        SelectedFileView->show();
        DataTex::loadImageFile(file,SelectedFileView);
    }
    else{
        SelectedFileView->hide();
    }
}

