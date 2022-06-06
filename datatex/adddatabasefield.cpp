#include "adddatabasefield.h"
#include "ui_adddatabasefield.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QLineEdit>
#include <QFileInfo>
#include <QDesktopServices>
#include <QGridLayout>
#include <QDebug>
#include <QModelIndex>
#include <QPushButton>
#include "sqlfunctions.h"
#include "datatex.h"

AddDatabaseField::AddDatabaseField(QWidget *parent,QString Info,bool isSubSection) :
    QDialog(parent),
    ui(new Ui::AddDatabaseField)
{
    ui->setupUi(this);
    IsSubSection = isSubSection;
    ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,&AddDatabaseField::EnableAccept);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,&AddDatabaseField::EnableAccept);
    idAction = new QAction(this);
    nameAction = new QAction(this);
    ui->InfoLabel->setHidden(Info.isEmpty() || Info.isNull());
    ui->InfoLabel->setText(Info);
    QRegExp pk("[A-Za-z0-9]{1,}");
    QRegExpValidator * validator = new QRegExpValidator( pk, this );
    ui->CodeLine->setValidator(validator);
    if(IsSubSection){
        QSqlQuery GetSubSecIds(DataTex::CurrentTexFilesDataBase);
        GetSubSecIds.exec("SELECT * FROM Exercise_Types WHERE Id <> \"-\"");
        while(GetSubSecIds.next()){
            MapSubSecIds.insert(GetSubSecIds.value(0).toString(),GetSubSecIds.value(1).toString());
            MapSubSecNames.insert(GetSubSecIds.value(1).toString(),GetSubSecIds.value(0).toString());
        }
        QCompleter *IdCompleter = new QCompleter(MapSubSecIds.keys(), this);
        IdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        ui->CodeLine->setCompleter(IdCompleter);
        QCompleter *NameCompleter = new QCompleter(MapSubSecNames.keys(), this);
        NameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        ui->NameLine->setCompleter(NameCompleter);
    }
    QStringList SubSections_Names = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Exercise_Types WHERE Id <> \"-\"",DataTex::CurrentTexFilesDataBase);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,[&](QString text){
        if(text.length()>7){
            ui->warning->setText(tr("Use short primary keys\n(<8 characters) for short file names."));
        }
        else{
            ui->warning->clear();
        }
        if(IsSubSection){
            if(MapSubSecIds.keys().contains(text)){
                ui->NameLine->setText(MapSubSecIds[text]);
                idAction = ui->CodeLine->addAction(QIcon(":/images/document-encrypted.svg"), QLineEdit::LeadingPosition);
            }
            else{
                ui->CodeLine->removeAction(idAction);
            }
        }
    });
    connect(ui->NameLine,&QLineEdit::textChanged,this,[&](QString text){
        if(text.length()>20){
            ui->warning->setText(tr("Keep your names between 15-20\ncharacters for short file names."));
        }
        else{
            ui->warning->clear();
        }
        if(IsSubSection){
            if(MapSubSecNames.keys().contains(text)){
                ui->CodeLine->setText(MapSubSecNames[text]);
                nameAction = ui->NameLine->addAction(QIcon(":/images/document-encrypted.svg"), QLineEdit::LeadingPosition);
            }
            else{
                ui->NameLine->removeAction(nameAction);
            }
        }
    });
}

AddDatabaseField::~AddDatabaseField()
{
    delete ui;
    delete nameAction;
    delete idAction;
}

void AddDatabaseField::EditLine_DataTex(QStringList Line)
{
    ui->NameLine->setText(Line[0]);
    ui->CodeLine->setText(Line[1]);
}

void AddDatabaseField::EnableAccept()
{
    if(!ui->CodeLine->text().isEmpty() && !ui->NameLine->text().isEmpty()){
        ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else {ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);}
}

QStringList AddDatabaseField::on_OkButton_accepted()
{
    QStringList Line = {ui->NameLine->text(),ui->CodeLine->text(),""};
    emit newline(Line);
    accept();
    return Line;
}

void AddDatabaseField::on_OkButton_rejected()
{
    reject();
}
