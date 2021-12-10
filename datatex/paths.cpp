#include "paths.h"
#include "ui_paths.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QGridLayout>
#include <QDebug>
#include <QCloseEvent>
#include "basefolder.h"
#include "datatex.h"
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include "sqlfunctions.h"
#include "addline.h"


Paths::Paths(QWidget *parent, QString path)
    : QDialog(parent)
    , ui(new Ui::Paths)
{
    ui->setupUi(this);

    QStringList DatabasesFileNames;
    QStringList DatabasesNames;
    QSqlQuery fields(DataTex::DataTeX_Settings);
    fields.exec("SELECT \"FileName\",\"Name\" FROM \"DataBases\"");
    while(fields.next()){
        DatabasesFileNames.append(fields.value(0).toString());
        DatabasesNames.append(fields.value(1).toString());
    }

    QSqlQuery currentBase(DataTex::DataTeX_Settings);
    currentBase.exec("SELECT db.Name FROM Current_Database_Notes_Folder cd JOIN DataBases db ON cd.Value = db.FileName;");
    while(currentBase.next()){
        base = currentBase.value(0).toString();
    }

    if(DatabasesFileNames.count()>0){
        for (int i=0;i<DatabasesNames.count();i++ ) {
            ui->ComboBaseList->addItem(DatabasesNames.at(i),QVariant(DatabasesFileNames.at(i)));
        }
        ui->ComboBaseList->setCurrentText(base);
    }
    else{
        ui->ComboBaseList->setEnabled(false);
        ui->DeleteFilesBase->setEnabled(false);
    }

    QStringList NotesFileNames;
    QStringList NotesNames;
    QSqlQuery fields_2(DataTex::DataTeX_Settings);
    fields_2.exec("SELECT \"FileName\",\"Name\" FROM \"Notes_Folders\"");
    while(fields_2.next()){
        NotesFileNames.append(fields_2.value(0).toString());
        NotesNames.append(fields_2.value(1).toString());
    }

    QSqlQuery currentNotesBase(DataTex::DataTeX_Settings);
    currentNotesBase.exec("SELECT db.Name FROM Current_Database_Notes_Folder cd JOIN Notes_Folders db ON cd.Value = db.FileName;");
    while(currentNotesBase.next()){
        note=currentNotesBase.value(0).toString();
    }

    if(NotesFileNames.count()>0){
        for (int i=0;i<NotesFileNames.count();i++ ) {
            ui->ComboNote->addItem(NotesNames.at(i),QVariant(NotesFileNames.at(i)));
        }
        ui->ComboNote->setCurrentText(note);
    }
    else{
        ui->ComboNote->setEnabled(false);
        ui->DeleteBase->setEnabled(false);
    }
    currentbase_Exercises = DataTex::CurrentTexFilesDataBase;
    QSqlQuery LoadPreambles(DataTex::DataTeX_Settings);
    LoadPreambles.exec("SELECT \"Id\",\"Name\" FROM \"Preambles\";");
    while(LoadPreambles.next()){ui->PreambleCombo->
                addItem(LoadPreambles.value(1).toString(),QVariant(LoadPreambles.value(0).toString()));}
    QString CurrentPreamble;
    QSqlQuery CurrentPreambleQuery(DataTex::DataTeX_Settings);
    CurrentPreambleQuery.exec(QString("SELECT \"Name\" FROM \"Preambles\" WHERE \"Id\" = \"%1\";")
                              .arg(DataTex::CurrentPreamble));
    while(CurrentPreambleQuery.next()){CurrentPreamble = CurrentPreambleQuery.value(0).toString();};
    ui->PreambleCombo->setCurrentText(CurrentPreamble);
    ui->ProgramPath->setText(path);
    ui->NotesPath->setText(DataTex::CurrentNotesFolderPath);
    ui->DatabaseLineEdit->setText(DataTex::CurrentDataBasePath);

    LoadTables(ui->ComboBaseList->currentData().toString());
    LoadDocTables(ui->ComboNote->currentData().toString());

    if(ui->PreambleCombo->count()==0){
        ui->PreambleText->setEnabled(false);
        ui->RemovePreambleButton->setEnabled(false);
    }
    ui->RemovePreambleButton->setEnabled(ui->PreambleCombo->currentData().toString()!="Basic");

    ui->SliderValue->setText(QString::number(ui->IconSize->value()));
    QObject::connect(ui->IconSize, &QSlider::valueChanged, this, [=] () {
        ui->SliderValue->setText(QString::number(ui->IconSize->value()));
        emit iconsize(ui->IconSize->value());
    });
    QObject::connect(ui->FontSelect, &QFontComboBox::currentFontChanged, this, [=](){
        emit selectfont(ui->FontSelect->currentFont().toString());
    });

    ui->PdfLatexPath->setText(DataTex::PdfLatex_Command);
    ui->LatexPath->setText(DataTex::Latex_Command);
    ui->XelatexPath->setText(DataTex::XeLatex_Command);
    ui->LualatexPath->setText(DataTex::LuaLatex_Command);
    ui->PythontexPath->setText(DataTex::Pythontex_Command);
    ui->BibtexPath->setText(DataTex::Bibtex_Command);
    ui->AsymptotePath->setText(DataTex::Asymptote_Command);

    ui->DocDatabasePassword->setEnabled(false);
    ui->SaveLocation->setText(DataTex::GlobalSaveLocation);
    ui->DatabasePrefix->setEnabled(false);
    ui->DocDatabasePrefix->setEnabled(false);
}

Paths::~Paths()
{
    delete ui;
}

void Paths::LoadTables(QString database)
{
    QSqlQueryModel * Metadata = new QSqlQueryModel(this);
    QSqlQueryModel * Bibliography = new QSqlQueryModel(this);
    QSqlQuery tableQuery(DataTex::DataTeX_Settings);
    tableQuery.prepare(QString("SELECT m.Id AS 'Field Id',m.Name AS 'Name' FROM Metadata_per_Database md JOIN Metadata m ON md.Metadata_Id = m.Id WHERE Database_FileName = \"%1\"")
                       .arg(database));
    tableQuery.exec();
    Metadata->setQuery(tableQuery);
    ui->DatabaseFieldTable->setModel(Metadata);
    ui->DatabaseFieldTable->show();

    QSqlQuery tableQuery_2(DataTex::DataTeX_Settings);
    tableQuery_2.prepare(QString("SELECT b.Id AS 'Field Id',b.Name AS 'Name' FROM Bibliographic_Fields_per_Database bd JOIN Bibliography b ON bd.Bibliographic_Field = b.Id WHERE Database = \"%1\"")
                       .arg(database));
    tableQuery_2.exec();
    Bibliography->setQuery(tableQuery_2);
    ui->BibliographyFieldsTable->setModel(Bibliography);
    ui->BibliographyFieldsTable->show();
    for (int c = 0; c < ui->DatabaseFieldTable->horizontalHeader()->count(); ++c)
    {
        ui->DatabaseFieldTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
    for (int c = 0; c < ui->BibliographyFieldsTable->horizontalHeader()->count(); ++c)
    {
        ui->BibliographyFieldsTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void Paths::LoadDocTables(QString database)
{
    QSqlQueryModel * DocMetadata = new QSqlQueryModel(this);
    QSqlQuery tableQuery(DataTex::DataTeX_Settings);
    tableQuery.prepare(QString("SELECT m.Id AS 'Field Id',m.Name AS 'Name' FROM DocMetadata_per_Database md JOIN DocMetadata m ON md.Metadata_Id = m.Id WHERE Database_FileName = \"%1\"")
                       .arg(database));
    tableQuery.exec();
    DocMetadata->setQuery(tableQuery);
    ui->DatabaseFolderTable->setModel(DocMetadata);
    ui->DatabaseFolderTable->show();
    for (int c = 0; c < ui->DatabaseFolderTable->horizontalHeader()->count(); ++c)
    {
        ui->DatabaseFolderTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void Paths::on_BaseButton_clicked()
{
    newbasefolder = new BaseFolder(this);
    connect(newbasefolder,SIGNAL(newbase(QString,QString,QString)),this,SLOT(CreateBaseFolder(QString,QString,QString)));
    newbasefolder->show();
    newbasefolder->activateWindow();
}

void Paths::CreateBaseFolder(QString path,QString FolderName,QString fileName)
{
    QString FullPath = path+QDir::separator()+FolderName+QDir::separator()+fileName+".db";
    QSqlQuery AddBaseQuery(DataTex::DataTeX_Settings);
    AddBaseQuery.exec(QString("INSERT INTO \"Databases\" (\"FileName\",\"Name\",\"Path\") VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(fileName,FolderName,FullPath));
    ui->DatabaseLineEdit->setText(FullPath);
    ui->ComboBaseList->addItem(FolderName,QVariant(fileName));
    ui->ComboBaseList->setCurrentText(FolderName);
    ui->ComboBaseList->setEnabled(true);
    ui->DeleteFilesBase->setEnabled(true);
}

void Paths::CreateNoteFolder(QString path,QString FolderName,QString FileName)
{
    QString FullPath = path+FolderName+QDir::separator()+FileName+".db";
    ui->NotesPath->setText(FullPath);
    ui->ComboNote->addItem(FolderName,QVariant(FileName));
    ui->ComboNote->setCurrentText(FolderName);
    ui->ComboNote->setEnabled(true);
    ui->DeleteBase->setEnabled(true);
}

void Paths::on_buttonBox_accepted()
{
    QString Text = ui->PreambleText->toPlainText();
    QSqlQuery WritePreambleQuery(DataTex::DataTeX_Settings);
    QString preamble = ui->PreambleCombo->currentData().toString();
    QSqlQuery SaveData(DataTex::DataTeX_Settings);
    SaveData.exec(QString("UPDATE \"Initial_Settings\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_Preamble'")
                  .arg(preamble));
    WritePreambleQuery.exec(QString("UPDATE \"Preambles\" SET \"Preamble_Content\" = \"%1\" "
                                    "WHERE \"Id\" = \"%2\";").arg(Text,ui->PreambleCombo->currentData().toString()));
//    if(ui->PreambleCombo->count()>0){
//    DataTex::CurrentPreamble =
//            SqlFunctions::Get_StringList_From_Query(SqlFunctions::GetPreamble,DataTex::DataTeX_Settings).at(0);
//    DataTex::CurrentPreamble_Content =
//            SqlFunctions::Get_StringList_From_Query(QString(SqlFunctions::GetPreamble_Content)
//                                                    .arg(DataTex::CurrentPreamble)
//                                                    ,DataTex::DataTeX_Settings).at(0);
//    }

//    QString baseFileName = ui->ComboBaseList->currentData().toString();
//    QString notesFileName = ui->ComboNote->currentData().toString();
//    SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_DataBase'")
//                  .arg(baseFileName));
//    SaveData.exec(QString("UPDATE \"Current_Database_Notes_Folder\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'Current_Notes_Folder'")
//                  .arg(notesFileName));
//    DataTex::CurrentTexFilesDataBase.close();
//    DataTex::CurrentNotesFolderDataBase.close();
//    DataTex::CurrentDataBasePath =
//            SqlFunctions::GetCurrentDataBase(DataTex::DataTeX_Settings,SqlFunctions::SelectCurrentDataBase);
//    DataTex::CurrentNotesFolderPath =
//            SqlFunctions::GetCurrentDataBase(DataTex::DataTeX_Settings,SqlFunctions::SelectCurrentNotesFolderBase);
//    DataTex::CurrentTexFilesDataBase.setDatabaseName(DataTex::CurrentDataBasePath);
//    DataTex::CurrentTexFilesDataBase.open();
//    DataTex::CurrentNotesFolderDataBase.setDatabaseName(DataTex::CurrentNotesFolderPath);
//    DataTex::CurrentNotesFolderDataBase.open();
    accept();
}

void Paths::on_NoteButton_clicked()
{
    newnotefolder = new BaseFolder(this);
    connect(newnotefolder,SIGNAL(newdocbase(QString,QString,QString)),this,SLOT(CreateNoteFolder(QString,QString,QString)));
    newnotefolder->show();
    newnotefolder->activateWindow();
}

void Paths::on_buttonBox_rejected()
{
    reject();
}

void Paths::on_ComboBaseList_currentIndexChanged(int index)
{
    if(index!=-1){
        QString basename = ui->ComboBaseList->currentData().toString();
        QString path;
        LoadTables(basename);
        QSqlQuery Path(DataTex::DataTeX_Settings);
        Path.exec(QString("SELECT \"Path\" From \"Databases\" WHERE \"FileName\" = \"%1\"").arg(basename));
        while(Path.next()){
            path = Path.value(0).toString();
        }
        ui->DatabaseLineEdit->setText(path);
    }
}

void Paths::on_DeleteFilesBase_clicked()
{
    QString Base = ui->ComboBaseList->currentText();
    QString BasePath = QFileInfo(ui->DatabaseLineEdit->text()).absolutePath();
    QCheckBox *cb = new QCheckBox(tr("Open folder"));
    QMessageBox msgbox;
    msgbox.setText(tr("The database %1 wil be deleted!\nDo you wish to proceed?").arg(Base));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);

    if(msgbox.exec() == QMessageBox::Ok){
        QSqlQuery DeleteQuery(DataTex::DataTeX_Settings);
        DeleteQuery.exec("PRAGMA foreign_keys = ON");
        DeleteQuery.exec(QString("DELETE FROM \"Databases\" WHERE \"FileName\" = \"%1\"").arg(ui->ComboBaseList->currentData().toString()));
        if(ui->ComboBaseList->count()==1){
            ui->ComboBaseList->clear();
            ui->DatabaseFieldTable->reset();
        }
        else {
            ui->ComboBaseList->removeItem(ui->ComboBaseList->currentIndex());
    }
    if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+BasePath));}
    }
}

void Paths::on_ComboNote_currentIndexChanged(int index)
{
    if(index!=-1){
        QString notefolder = ui->ComboNote->currentData().toString();
        QString path;
        LoadDocTables(notefolder);
        QSqlQuery Path(DataTex::DataTeX_Settings);
        Path.exec(QString("SELECT \"Path\" From \"Notes_Folders\" WHERE \"FileName\" = \"%1\"").arg(notefolder));
        while(Path.next()){
            path = Path.value(0).toString();
        }
        ui->NotesPath->setText(path);
    }
}

void Paths::on_DeleteBase_clicked()
{
    QString Notes = ui->ComboNote->currentText();
    QString NotesPath = QFileInfo(ui->NotesPath->text()).absolutePath();
    QCheckBox *cb = new QCheckBox(tr("Open folder"));
    QMessageBox msgbox;
    msgbox.setText(tr("The database %1 wil be deleted!\nDo you wish to proceed?").arg(Notes));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);

    if(msgbox.exec() == QMessageBox::Ok){
        QSqlQuery DeleteQuery(DataTex::DataTeX_Settings);
        DeleteQuery.exec("PRAGMA foreign_keys = ON");
        DeleteQuery.exec(QString("DELETE FROM \"Notes_Folders\" WHERE \"FileName\" = \"%1\"").arg(ui->ComboNote->currentData().toString()));
        if(ui->ComboNote->count()==1){
            ui->ComboNote->clear();
        }
        else {
        ui->ComboNote->removeItem(ui->ComboNote->currentIndex());
    }
    if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+NotesPath));}
    }
}

void Paths::on_AddBase_clicked()
{
    QString Database = QFileDialog::getOpenFileName(this,tr("Select a Database File"),QDir::homePath(),"sqlite db Files (*.db)");
    if(Database.isEmpty())return;
    QString DatabaseName = QFileInfo(Database).baseName();
    QStringList list = QFileInfo(Database).absolutePath().split(QDir::separator());
    QString folderName = list.last();
    QSqlQuery AddNotesQuery(DataTex::DataTeX_Settings);
    AddNotesQuery.exec(QString("INSERT INTO \"Databases\" (\"FileName\",\"Name\",\"Path\") VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(DatabaseName,folderName,Database));

    QSqlDatabase addeddatabaseFile;
    addeddatabaseFile = QSqlDatabase::addDatabase("QSQLITE","addedbase");
    addeddatabaseFile.setDatabaseName(Database);
    addeddatabaseFile.open();

    QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    QStringList BibIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Bibliography'",addeddatabaseFile);
    QStringList BibNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Bibliography'",addeddatabaseFile);
    addeddatabaseFile.close();

    QSqlQuery add(DataTex::DataTeX_Settings);
    for (int i=0;i<MetadataIds.count();i++) {
        add.exec(QString("INSERT OR IGNORE INTO \"Metadata\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
        add.exec("INSERT OR IGNORE INTO \"Metadata_per_Database\" (\"Database_FileName\",\"Metadata_Id\") VALUES (\""+DatabaseName+"\",\""+MetadataIds.at(i)+"\")");
    }

    for (int i=0;i<BibIds.count();i++) {
        add.exec(QString("INSERT OR IGNORE INTO \"Bibliography\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+BibIds.at(i)+"\",\""+BibNames.at(i)+"\",0)"));
        add.exec("INSERT OR IGNORE INTO \"Bibliographic_Fields_per_Database\" (\"Database\",\"Bibliographic_Field\") VALUES (\""+DatabaseName+"\",\""+BibIds.at(i)+"\")");
    }

    ui->DatabaseLineEdit->setEnabled(true);
    ui->DatabaseLineEdit->setText(Database);
    ui->ComboBaseList->addItem(folderName,QVariant(DatabaseName));
    ui->ComboBaseList->setCurrentText(folderName);
    ui->DeleteFilesBase->setEnabled(true);
}

void Paths::on_PreambleCombo_currentIndexChanged(const QString &arg1)
{
    ui->RemovePreambleButton->setEnabled(ui->PreambleCombo->currentData().toString()!="Basic");
    QString Preambletext;
    ui->PreambleText->clear();
    QSqlQuery PreambleQuery(DataTex::DataTeX_Settings);
    PreambleQuery.exec(QString("SELECT \"Preamble_Content\" FROM \"Preambles\" WHERE \"Id\" = \"%1\";")
                       .arg(ui->PreambleCombo->currentData().toString()));
    while(PreambleQuery.next()){Preambletext = PreambleQuery.value(0).toString();}
    ui->PreambleText->append(Preambletext);
}

void Paths::on_AddPreambleButton_clicked()
{
    AddLine * newPreamle = new AddLine(this);
    connect(newPreamle,SIGNAL(grammhline(QStringList)),this,SLOT(AddPreamble(QStringList)));
    newPreamle->show();
    newPreamle->activateWindow();
}

void Paths::AddPreamble(QStringList preamble)
{
    QSqlQuery AddPreamble(DataTex::DataTeX_Settings);
    AddPreamble.exec(QString("INSERT OR IGNORE INTO \"Preambles\" (\"Id\",\"Name\",\"Preamble_Content\") VALUES (\"%1\",\"%2\",\"%3\")")
                     .arg(preamble[1],preamble[0],preamble[2]));
    ui->PreambleCombo->addItem(preamble[0],QVariant(preamble[1]));
    ui->PreambleCombo->setCurrentText(preamble[0]);
    ui->PreambleText->setPlainText(preamble[2]);
}

void Paths::on_RemovePreambleButton_clicked()
{
    QString currentPreamble = ui->PreambleCombo->currentText();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Delete preamble"),tr("The preamble %1 will be deleted!\nDo you wish to proceed?")
                                    .arg(currentPreamble),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        QSqlQuery RemovePreamble(DataTex::DataTeX_Settings);
        RemovePreamble.exec(QString("DELETE FROM Preambles WHERE Id = \"%1\"").arg(ui->PreambleCombo->currentData().toString()));
        ui->PreambleCombo->removeItem(ui->PreambleCombo->currentIndex());
    }
}

void Paths::on_OpenSettingsButton_clicked()
{
    QString Path = ui->ProgramPath->text();
    QDesktopServices::openUrl(QUrl("file:///"+Path));
}

void Paths::on_ListOfSettings_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

PasswordLineEdit::PasswordLineEdit(QWidget *parent):
    QLineEdit(parent)
{
    setEchoMode(QLineEdit::Password);
    QAction *action = addAction(QIcon(":/images/eyeOff.svg"), QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton *>(action->associatedWidgets().last());
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, &QToolButton::pressed, this, &PasswordLineEdit::onPressed);
    connect(button, &QToolButton::released, this, &PasswordLineEdit::onReleased);
}

void PasswordLineEdit::onPressed(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon(":/images/eyeOn.svg"));
    setEchoMode(QLineEdit::Normal);
}

void PasswordLineEdit::onReleased(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon(":/images/eyeOff.svg"));
    setEchoMode(QLineEdit::Password);
}

void PasswordLineEdit::enterEvent(QEvent *event){
    button->show();
    QLineEdit::enterEvent(event);
}

void PasswordLineEdit::leaveEvent(QEvent *event){
    button->hide();
    QLineEdit::leaveEvent(event);
}

void PasswordLineEdit::focusInEvent(QFocusEvent *event){
    button->show();
    QLineEdit::focusInEvent(event);
}

void PasswordLineEdit::focusOutEvent(QFocusEvent *event){
    button->hide();
    QLineEdit::focusOutEvent(event);
}

void Paths::on_AddDocDatabaseButton_clicked()
{
    QString Database = QFileDialog::getOpenFileName(this,tr("Select a Database File"),QDir::homePath(),"sqlite db Files (*.db)");
    if(Database.isEmpty())return;
    QString DatabaseName = QFileInfo(Database).baseName();
    QStringList list = QFileInfo(Database).absolutePath().split(QDir::separator());
    QString folderName = list.last();
    QSqlQuery AddNotesQuery(DataTex::DataTeX_Settings);
    AddNotesQuery.exec(QString("INSERT INTO \"Notes_Folders\" (\"FileName\",\"Name\",\"Path\") VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(DatabaseName,folderName,Database));

    QSqlDatabase addeddatabaseFile;
    addeddatabaseFile = QSqlDatabase::addDatabase("QSQLITE","addedbase");
    addeddatabaseFile.setDatabaseName(Database);
    addeddatabaseFile.open();

    QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    addeddatabaseFile.close();

    QSqlQuery add(DataTex::DataTeX_Settings);
    for (int i=0;i<MetadataIds.count();i++) {
        add.exec(QString("INSERT OR IGNORE INTO \"DocMetadata\" (\"Id\",\"Name\",\"Basic\") VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
        add.exec("INSERT OR IGNORE INTO \"DocMetadata_per_Database\" (\"Database_FileName\",\"Metadata_Id\") VALUES (\""+DatabaseName+"\",\""+MetadataIds.at(i)+"\")");
    }
    ui->NotesPath->setEnabled(true);
    ui->NotesPath->setText(Database);
    ui->ComboNote->addItem(folderName,QVariant(DatabaseName));
    ui->ComboNote->setCurrentText(folderName);
    ui->DeleteBase->setEnabled(true);
}

void Paths::on_EncryptDocDatabase_clicked(bool checked)
{
    ui->DocDatabasePassword->setEnabled(checked);
}

void Paths::on_UseDocDatabasePrefix_clicked(bool checked)
{
    ui->DocDatabasePrefix->setEnabled(checked);
}

void Paths::on_UseDatabasePrefix_clicked(bool checked)
{
    ui->DatabasePrefix->setEnabled(checked);
}

void Paths::on_EncryptDatabase_clicked(bool checked)
{
    ui->DatabasePassword->setEnabled(checked);
}


void Paths::on_OpenSaveLocation_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Select a save location"),DataTex::GlobalSaveLocation);
    if(path.isEmpty())return;
    DataTex::GlobalSaveLocation = path;
    ui->SaveLocation->setText(DataTex::GlobalSaveLocation);
    QSqlQuery query(DataTex::DataTeX_Settings);
    query.exec(QString("UPDATE \"Initial_Settings\" SET \"Value\" = \"%1\" WHERE \"Setting\" = 'SaveLocation'").arg(path));
}


void Paths::on_OpenPdfLatexPath_clicked()
{
    QString pdflatex = QFileDialog::getOpenFileName(this,
            tr("PdfLaTeX path"),DataTex::TexLivePath, "pdflatex");
    if(pdflatex.isEmpty()){
        return;
    }
    else{
        DataTex::PdfLatex_Command = pdflatex;
        DataTex::LatexCommands["PdfLaTeX"] = pdflatex;
        ui->PdfLatexPath->setText(pdflatex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(pdflatex,"Pdflatex_Command"));
    }
}

void Paths::on_OpenLatexPath_clicked()
{
    QString latex = QFileDialog::getOpenFileName(this,
            tr("LaTeX path"),DataTex::TexLivePath, "latex");
    if(latex.isEmpty()){
        return;
    }
    else{
        DataTex::Latex_Command = latex;
        DataTex::LatexCommands["LaTeX"] = latex;
        ui->LatexPath->setText(latex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(latex,"Latex_Command"));
    }
}

void Paths::on_OpenXeLatexPath_clicked()
{
    QString xelatex = QFileDialog::getOpenFileName(this,
            tr("XeLaTeX path"),DataTex::TexLivePath, "xelatex");
    if(xelatex.isEmpty()){
        return;
    }
    else{
        DataTex::XeLatex_Command = xelatex;
        DataTex::LatexCommands["XeLaTeX"] = xelatex;
        ui->XelatexPath->setText(xelatex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(xelatex,"Xelatex_Command"));
    }
}

void Paths::on_OpenLuaLatexPath_clicked()
{
    QString lualatex = QFileDialog::getOpenFileName(this,
            tr("LuaLaTeX path"),DataTex::TexLivePath, "lualatex");
    if(lualatex.isEmpty()){
        return;
    }
    else{
        DataTex::LuaLatex_Command = lualatex;
        DataTex::LatexCommands["LuaLaTeX"] = lualatex;
        ui->LualatexPath->setText(lualatex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(lualatex,"Lualatex_Command"));
    }
}

void Paths::on_OpenPythontexPath_clicked()
{
    QString pythontex = QFileDialog::getOpenFileName(this,
            tr("PythonTeX path"),DataTex::TexLivePath, "pythontex");
    if(pythontex.isEmpty()){
        return;
    }
    else{
        DataTex::Pythontex_Command = pythontex;
        DataTex::LatexCommands["PythonTex"] = pythontex;
        ui->PythontexPath->setText(pythontex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(pythontex,"Pythontex_Command"));
    }
}

void Paths::on_OpenBibtexPath_clicked()
{
    QString bibtex = QFileDialog::getOpenFileName(this,
            tr("BibTeX path"),DataTex::TexLivePath, "bibtex");
    if(bibtex.isEmpty()){
        return;
    }
    else{
        DataTex::Bibtex_Command = bibtex;
        DataTex::LatexCommands["BibTeX"] = bibtex;
        ui->BibtexPath->setText(bibtex);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(bibtex,"Bibtex_Command"));
    }
}

void Paths::on_OpenAsymptotePath_clicked()
{
    QString asymptote = QFileDialog::getOpenFileName(this,
            tr("Asymptote path"),DataTex::TexLivePath, "asymptote");
    if(asymptote.isEmpty()){
        return;
    }
    else{
        DataTex::PdfLatex_Command = asymptote;
        DataTex::LatexCommands["Asymptote"] = asymptote;
        ui->AsymptotePath->setText(asymptote);
        QSqlQuery CommandsQuery(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(asymptote,"Asymptote_Command"));
    }
}

