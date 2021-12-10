#include "addline.h"
#include "ui_addline.h"
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
#include "sqlfunctions.h"
#include "datatex.h"

QFile TomeasFileLine(QDir::currentPath().remove("bin")+"Tomeas.csv");

AddLine::AddLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLine)
{
    ui->setupUi(this);
    Packages = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Texlive_Packages",DataTex::DataTeX_Settings);
    Descriptions = SqlFunctions::Get_StringList_From_Query("SELECT Description FROM Texlive_Packages",DataTex::DataTeX_Settings);
    filter = new QSortFilterProxyModel(this);
    model = new QStringListModel(Packages,this);
    filter->setSourceModel(model);
    ui->PackageList->setModel(filter);
    for (int i=0;i<Packages.count();i++) {
        MapDescriptions.insert(Packages[i],Descriptions[i]);
    }
    connect(ui->PackageFilter,&QLineEdit::textChanged,this,[=](QString text){
        filter->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,
                                                    QRegExp::FixedString));
        filter->setFilterKeyColumn(0);
    });
    connect(ui->PackageList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &AddLine::PackageList_SelectionChanged);
    ui->splitter->setSizes(QList<int>({1, 200}));
    ui->textEdit->setText("\\documentclass[]{<Select document type>}\n");
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
    ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,&AddLine::EnableAccept);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,&AddLine::EnableAccept);
    ui->PackageList->setCurrentIndex(ui->PackageList->model()->index(0,0));

}

AddLine::~AddLine()
{
    delete ui;
}

void AddLine::EditLine_DataTex(QStringList Line)
{
    ui->NameLine->setText(Line[0]);
    ui->CodeLine->setText(Line[1]);
}


QStringList AddLine::on_OkButton_accepted()
{
    QStringList Line = {ui->NameLine->text(),ui->CodeLine->text(),ui->textEdit->toPlainText()};
    emit grammhline(Line);
    accept();
    return Line;
}

void AddLine::PackageList_SelectionChanged()
{
    QItemSelectionModel *select = ui->PackageList->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    if(row>-1){
        ui->PackageDescription->setText(MapDescriptions[ui->PackageList->model()->index(row,0).data(Qt::DisplayRole).toString()]);
    }
}


void AddLine::on_OkButton_rejected()
{
    reject();
}


void AddLine::on_AddButton_clicked()
{
    QString package = ui->PackageList->currentIndex().data().toString();
    ui->textEdit->insertPlainText("\\usepackage{"+package+"}\n");
}


void AddLine::on_RemoveButton_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int row = cursor.blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, row);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    cursor.deleteChar();
    ui->textEdit->setTextCursor(cursor);
}


void AddLine::on_textEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int row = cursor.blockNumber();
}

void AddLine::EnableAccept()
{
    if(!ui->CodeLine->text().isEmpty() && !ui->NameLine->text().isEmpty()){
        ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else {ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);}
}
