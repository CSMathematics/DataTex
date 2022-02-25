#include "newfiletype.h"
#include "ui_newfiletype.h"
#include "datatex.h"
#include "sqlfunctions.h"

NewFileType::NewFileType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileType)
{
    ui->setupUi(this);
    QStringList Ids = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM FileTypes",DataTex::CurrentTexFilesDataBase);
    QStringList Names = SqlFunctions::Get_StringList_From_Query("SELECT FileType FROM FileTypes",DataTex::CurrentTexFilesDataBase);
    QStringList Folders = SqlFunctions::Get_StringList_From_Query("SELECT FolderName FROM FileTypes",DataTex::CurrentTexFilesDataBase);
    setWindowTitle("Select a name/desctription for tis database");
    connect(ui->NameLine,&QLineEdit::textChanged,this,[=](){
        if(Names.contains(ui->NameLine->text())){
            ui->warning->setText("This name already exists");
            ui->OkButton->setEnabled(false);
        }
        else{
            ui->warning->setText("");
            ui->OkButton->setEnabled(true);
        }
    });
    connect(ui->IdLine,&QLineEdit::textChanged,this,[=](){
        if(Ids.contains(ui->IdLine->text())){
            ui->warning->setText("This id already exists");
            ui->OkButton->setEnabled(false);
        }
        else{
            ui->warning->setText("");
            ui->OkButton->setEnabled(true);
        }
    });
    connect(ui->FolderLine,&QLineEdit::textChanged,this,[=](){
        if(Folders.contains(ui->FolderLine->text())){
            ui->warning->setText("This folder already exists");
            ui->OkButton->setEnabled(false);
        }
        else{
            ui->warning->setText("");
            ui->OkButton->setEnabled(true);
        }
    });
}

NewFileType::~NewFileType()
{
    delete ui;
}

void NewFileType::on_OkButton_accepted()
{
    QStringList Line = {ui->IdLine->text(),ui->NameLine->text(),ui->FolderLine->text(),QString::number(ui->NeedsSolution->isChecked())};
    emit filedata(Line);
}

