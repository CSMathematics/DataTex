#include "addfolder.h"
#include "ui_addfolder.h"

#include <QtCore>
#include <QtGui>

addfolder::addfolder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addfolder)
{
    ui->setupUi(this);

}

addfolder::~addfolder()
{
    delete ui;
}

void addfolder::EditFolder(QString line)
{
    ui->NameLine->setText(line);
}

QString addfolder::on_newName()
{
    QString Name = ui->NameLine->text();
    return Name;
}

void addfolder::on_OkbuttonBoxLine_accepted()
{
    QString newfolder = ui->NameLine->text();
    emit newSingleEntry(newfolder);
    accept();
}

void addfolder::on_OkbuttonBoxLine_rejected()
{
    reject();
}
