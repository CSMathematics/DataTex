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

AddDatabaseField::AddDatabaseField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDatabaseField)
{
    ui->setupUi(this);
    ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,&AddDatabaseField::EnableAccept);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,&AddDatabaseField::EnableAccept);
}

AddDatabaseField::~AddDatabaseField()
{
    delete ui;
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
