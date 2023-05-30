#include "edithistory.h"
#include "ui_edithistory.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDir>

EditHistory::EditHistory(QWidget *parent, QString filePath, QString buildCommand, QString currentContent, QStringList metadata, bool isDocument) :
    QDialog(parent),
    ui(new Ui::EditHistory)
{
    ui->setupUi(this);
    DatabaseFile = filePath;
    CurrentBuildCommand = buildCommand;
    CurrentContent = currentContent;
    Metadata = metadata;
    ui->Complile->setEnabled(false);
    ui->DeleteEntry->setEnabled(false);
    TempFileView = new QPdfViewer(this);
    ui->gridLayout_3->addWidget(TempFileView,0,0);
    TempFileView->show();
    CurrentFileView = new QPdfViewer(this);
    ui->gridLayout_3->addWidget(CurrentFileView,0,1);
    CurrentFileView->show();
    ui->TexButton->setChecked(true);
    QDir datatexdir(QFileInfo(DatabaseFile).absolutePath()+QDir::separator()+"temp_history"+QDir::separator());
    datatexdir.mkpath(".");

    currentdatabase = (isDocument) ? DataTex::CurrentDocumentDataBase : DataTex::CurrentTexFilesDataBase ;
    QSqlQuery data(currentdatabase);
    data.exec(QString("SELECT Date_Time,Modification,FileContent,Metadata FROM Edit_History WHERE File_Id = '%1'")
                  .arg(QFileInfo(DatabaseFile).baseName()));
    while (data.next()) {
        QListWidgetItem *item = new QListWidgetItem(ui->HistoryList);
        QRadioButton * radio = new QRadioButton(QDateTime::fromString(data.value(0).toString(),"dd/M/yyyy hh:mm").toString("dddd d MMMM yyyy hh:mm")+
                                                              "  ("+data.value(1).toString()+")",this);
        qDebug()<<QDateTime::fromString(data.value(0).toString(),"dd/M/yyyy hh:mm");
        ui->HistoryList->setItemWidget(item, radio);
        item->setData(Qt::UserRole,QVariant(data.value(2).toString()));
        ui->HistoryList->addItem(item);

        connect(radio,&QRadioButton::clicked,this,[=](bool checked){
            if(checked){
                QListWidgetItem * item = ui->HistoryList->selectedItems()[0];
                Content = item->data(Qt::UserRole).toString();
                ui->FileContent->setText(Content);
                ui->CurrentFileContent->setText(CurrentContent);
                LatexTextEdit::ShowDifferences(ui->FileContent,ui->CurrentFileContent);
                currentrow = ui->HistoryList->row(item);
                temp_file = datatexdir.absolutePath()+QDir::separator()+QFileInfo(DatabaseFile).baseName()+QString::number(currentrow)+".tex";
                QFile file(temp_file);
                file.open(QIODevice::ReadWrite);
                QTextStream ContentStream(&file);
                ContentStream << Content;
                file.close();
                if(QFileInfo::exists(temp_file)){
                    FileCommands::ShowPdfInViewer(temp_file,TempFileView);
                }
                FileCommands::ShowPdfInViewer(DatabaseFile,CurrentFileView);
            }
        });
    }
    connect(ui->HistoryList->selectionModel(),&QItemSelectionModel::selectionChanged,[&](){
        if(ui->HistoryList->selectionModel()->hasSelection()){
            ui->Complile->setEnabled(true);
            ui->DeleteEntry->setEnabled(true);
        }
    });
    connect(ui->Complile,&QPushButton::clicked,this,[=](){
        FileCommands::CreateTexFile(temp_file,0,"");
        FileCommands::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],temp_file,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
        FileCommands::ClearOldFiles(temp_file);
        FileCommands::ShowPdfInViewer(temp_file,TempFileView);
    });
    connect(ui->DeleteEntry,&QPushButton::clicked,this,[&](){

    });
    connect(ui->TexButton,&QPushButton::clicked,this,[&](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->PdfButton,&QPushButton::clicked,this,[&](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->MetadataButton,&QPushButton::clicked,this,[&](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(ui->CurrentFileContent->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->FileContent->verticalScrollBar()->setValue(ui->CurrentFileContent->verticalScrollBar()->value());
    });
    connect(ui->FileContent->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        ui->CurrentFileContent->verticalScrollBar()->setValue(ui->FileContent->verticalScrollBar()->value());
    });
    connect(TempFileView->getPdfView()->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        CurrentFileView->getPdfView()->verticalScrollBar()->setValue(TempFileView->getPdfView()->verticalScrollBar()->value());
    });
    connect(CurrentFileView->getPdfView()->verticalScrollBar(),&QScrollBar::valueChanged,this,[=](){
        TempFileView->getPdfView()->verticalScrollBar()->setValue(CurrentFileView->getPdfView()->verticalScrollBar()->value());
    });
}

EditHistory::~EditHistory()
{
    delete ui;
}

void EditHistory::on_buttonBox_accepted()
{

}


void EditHistory::on_buttonBox_rejected()
{

}

