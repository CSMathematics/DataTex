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



AddFileToEditor::AddFileToEditor(QWidget *parent,QString currentTexFile, QString BuildCommand) :
    QDialog(parent),
    ui(new Ui::AddFileToEditor)
{
    ui->setupUi(this);

    CurrentDatabaseFile = currentTexFile;
    currentbase = DataTex::CurrentTexFilesDataBase;
    CurrentBuildCommand = BuildCommand;
    int current = 0;
    for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
        ui->FilesDatabasesCombo->addItem(DataTex::GlobalFilesDatabaseListNames.values().at(i),QVariant(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName()));
        if(DataTex::GlobalFilesDatabaseList.values().at(i).databaseName() == DataTex::CurrentDataBasePath){current = i;}
    }
    ui->FilesDatabasesCombo->setCurrentIndex(current);
    ui->removeButton->setEnabled(false);
    ui->Save->setEnabled(false);
    ui->CurrentFileLabel->setText(tr("Insert files to document : ")+QFileInfo(CurrentDatabaseFile).fileName());

    view = new PdfViewer(this);
    view->setMinimumWidth(620);
    ui->gridLayout_17->addWidget(view,1,0);
    view->show();
    DocView = new PdfViewer(this);
    ui->verticalLayout_4->addWidget(DocView);
    DocView->show();

    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,3,0);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);

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
    ui->tabWidget->setCornerWidget(ui->ToolBar);
    DataTex::loadImageFile(CurrentDatabaseFile,DocView);
    ui->Save->setEnabled(false);
    connect(ui->DocumentContent, &QTextEdit::textChanged, this,[=](){ui->Save->setEnabled(true);});
}

AddFileToEditor::~AddFileToEditor()
{
    delete ui;
    delete view;
    delete DocView;
}

QList<QStringList> AddFileToEditor::getDatabaseFields(QSqlDatabase database)
{
    QList<QStringList> list;
    list.append(SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",database));
    list.append(SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",database));
    return list;
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
    int columns = Database_FileTableFields.count();
    for (int i=0;i<columns;i++) {
        SqlFunctions::FilesTable_UpdateQuery.replace("replace"+Database_FileTableFields.at(i),values.at(i));
    }
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase,this);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AddFileToEditor::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void AddFileToEditor::FilesTable_selectionchanged()
{
    PreviewFile.clear();
    int row  = FilesTable->selectionModel()->currentIndex().row();
    QVariant data = FilesTable->model()->data(FilesTable->model()->index(row,7));
    QString fullFilePath = data.toString();
    QString file = fullFilePath;
    QString pdffile = file.replace(".tex",".pdf");
    PreviewFile = fullFilePath;
    if(!QFileInfo::exists(pdffile)){
        DataTex::CreateTexFile(fullFilePath,0,"");

        DataTex::loadImageFile(fullFilePath,view);
    }
    else {DataTex::loadImageFile(fullFilePath,view);}
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
}

void AddFileToEditor::on_removeButton_clicked()
{
    ui->removeButton->setEnabled(false);
    QString Content = ui->DocumentContent->toPlainText();
    Content.remove(DatabaseFileContent);
    ui->DocumentContent->setText(Content);
}

void AddFileToEditor::on_Okbutton_accepted()
{
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
    writeExercisesPerDatabase.exec("INSERT OR IGNORE INTO Files_per_Document (\"Document_Id\",\"File_Id\",\"Files_Database_Source\") VALUES "+valuesQuery.join(","));
    writeExercisesPerDatabase.exec("DELETE FROM Files_per_Document WHERE Document_Id = \"DTX-test_24_9\" AND File_Id NOT IN (\""+ExercisesInsideDocument.join("\",\"")+"\")");
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

void AddFileToEditor::on_FilesDatabasesCombo_currentIndexChanged(int index)
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
            positions.append({start,end});
        }
    }
    int line = ui->DocumentContent->textCursor().blockNumber();
    int fileIndex =-1;
    for (int k=0;k<positions.count();k++) {
        if(line>=positions[k][0] && line <=positions[k][1]){
            fileIndex = k;
            for (int i = positions[fileIndex][0];i<positions[fileIndex][1]+1;i++) {
                QTextBlock block = ui->DocumentContent->document()->findBlockByNumber(i);
                QTextCursor cursor(block);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor(240,245,245));
                cursor.setBlockFormat(blockFormat);
                DatabaseFileContent.append(block.text()+"\n");
            }
            ui->removeButton->setEnabled(true);
            qDebug()<<DatabaseFileContent;
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
