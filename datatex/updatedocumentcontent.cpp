#include "updatedocumentcontent.h"
#include "ui_updatedocumentcontent.h"
#include <QTextStream>
#include <QFile>
#include <QTextEdit>
#include <QScrollBar>
#include "sqlfunctions.h"
#include "datatex.h"

UpdateDocumentContent::UpdateDocumentContent(QWidget *parent,QString Document,QStringList files,QStringList databases) :
    QDialog(parent),
    ui(new Ui::UpdateDocumentContent)
{
    ui->setupUi(this);
//    FilesInDocument.clear();
//    FilesContentsFormFile.clear();
//    FilesContentsFormDocument.clear();
    CurrentDocument = Document;
    FilesInDocument = files;
    QString Content;
    QFile file(CurrentDocument);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text(&file);
    text.flush();
    Content = text.readAll();
    file.close();
    ui->CurrentDocumentContent->editor->setText(Content);
    GetFileContentsFromFile();
    GetFileContentsFromDocument();
    qDebug()<<FilesContentsFormDocument<<FilesContentsFormFile;
    QStringList fileNames;
    for (int i = 0;i<FilesInDocument.count();i++) {
        fileNames.append(QFileInfo(files[i]).baseName());
        if(Content.contains(FilesContentsFormDocument.value(FilesInDocument[i]))){
            Content.replace(FilesContentsFormDocument.value(FilesInDocument[i]),FilesContentsFormFile.value(FilesInDocument[i]));
        }
    }
    ui->UpdatedDocumentContent->editor->setText(Content);

    QString f = "(\""+fileNames.join("\",\"")+"\")";
    QSqlQueryModel * Files = new QSqlQueryModel(this);
    QStringList datalist = {SqlFunctions::ShowFilesInADocument.arg(f,QFileInfo(DataTex::CurrentFilesDataBase.Path).baseName())};
    QString query;
    QSqlQuery FilesQuery(DataTex::CurrentFilesDataBase.Database);
    for (int i=0;i<databases.count();i++) {
        if(databases.at(i)!=DataTex::CurrentFilesDataBase.Path) {
            FilesQuery.exec(QString("ATTACH DATABASE \"%1\" AS \"%2\" ").arg(databases.at(i),QFileInfo(databases.at(i)).baseName()));
            datalist.append(SqlFunctions::ShowFilesInADocument_DifferentDatabase.arg(f,QFileInfo(databases.at(i)).baseName()));
        }
    }
    query = datalist.join(" UNION ");
    FilesQuery.exec(query+" ORDER BY \"df\".\"Id\" ");
    Files->setQuery(FilesQuery);
    ui->FilesTable->setModel(Files);
    ui->FilesTable->show();
    LatexTextEdit::ShowDifferences(ui->UpdatedDocumentContent->editor,ui->CurrentDocumentContent->editor);
//    LatexTextEdit::ShowButtons(ui->UpdatedDocumentContent);
    connect(ui->CurrentDocumentContent->editor->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->UpdatedDocumentContent->editor->verticalScrollBar()->setValue(ui->CurrentDocumentContent->editor->verticalScrollBar()->value());
    });
    connect(ui->UpdatedDocumentContent->editor->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->CurrentDocumentContent->editor->verticalScrollBar()->setValue(ui->UpdatedDocumentContent->editor->verticalScrollBar()->value());
    });
    connect(ui->FilesTable->selectionModel(),&QItemSelectionModel::selectionChanged,this,&UpdateDocumentContent::FilesTable_selection_changed);
//    connect(ui->UpdatedDocumentContent,&LatexTextEdit::cursorPositionChanged,ui->UpdatedDocumentContent,[=](){
//        LatexTextEdit::ShowDifferences(ui->UpdatedDocumentContent,ui->CurrentDocumentContent);
//    });
//    connect(ui->CurrentDocumentContent,&LatexTextEdit::cursorPositionChanged,ui->CurrentDocumentContent,[=](){
//        LatexTextEdit::ShowDifferences(ui->UpdatedDocumentContent,ui->CurrentDocumentContent);
//    });
}

UpdateDocumentContent::~UpdateDocumentContent()
{
    delete ui;
    for (int i=0;i<DatabasesInDocument.count();i++) {
        if(DatabasesInDocument[i].databaseName()!=DataTex::CurrentFilesDataBase.Path){
            DatabasesInDocument[i].close();
        }
    }
}

void UpdateDocumentContent::GetFileContentsFromFile()
{
    for (int i = 0;i<FilesInDocument.count();i++) {
        QFile file(FilesInDocument[i]);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        text.flush();
        QString FileContent = text.readAll();
        file.close();
        FilesContentsFormFile.insert(FilesInDocument[i],FileContent);
    }
}

void UpdateDocumentContent::GetFileContentsFromDocument()
{
    QString DocContent = ui->CurrentDocumentContent->editor->toPlainText();
    int lineNumber = -1;
    QStringList Lines;
    QString FileText;
    QString fileName;
    QTextStream parse(&DocContent);
    bool readEnabled = false;
    while(!parse.atEnd()){
        QString line = parse.readLine();
        if(line.startsWith("%# File Id")){
            lineNumber++;
            readEnabled = true;
            fileName = line;
            fileName.remove("%# File Id : ");
            foreach(QString file,FilesInDocument){
                if(fileName == QFileInfo(file).baseName()){
                    fileName = file;
                }
            }
        }
        if(readEnabled){
            Lines.append(line);
        }

        if(line.startsWith("%# End of file")){
            readEnabled = false;
            FileText += Lines.join("\n");
            FilesContentsFormDocument.insert(fileName,FileText);
            Lines.clear();
            FileText.clear();
        }
    }
}

void UpdateDocumentContent::FilesTable_selection_changed()
{
    QItemSelectionModel *select = ui->FilesTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    QString FilePath = ui->FilesTable->model()->data(ui->FilesTable->model()->index(row,5)).toString();
    QString File_content;
    QFile file(FilePath);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    File_content = text.readAll();
    file.close();
    ui->FileContent->setText(File_content);
//    ShowPdfInViewer(FilePath,FileFromDocumentView);
}
