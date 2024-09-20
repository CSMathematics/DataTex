#include "settings.h"
#include "dtxsettings.h"
#include "preamblesettings.h"
#include "ui_settings.h"


Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    QStringList DatabasesFileNames;
    QStringList DatabasesNames;
    QSqlQuery fields;//(DataTex::DataTeX_Settings);
    fields.exec("SELECT FileName,Name FROM DataBases");
    while(fields.next()){
        DatabasesFileNames.append(fields.value(0).toString());
        DatabasesNames.append(fields.value(1).toString());
    }
    QSqlQuery currentBase;//(DataTex::DataTeX_Settings);
    currentBase.exec("SELECT db.Name FROM Current_Databases cd JOIN DataBases db ON cd.Value = db.FileName;");
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
    QSqlQuery fields_2;//(DataTex::DataTeX_Settings);
    fields_2.exec("SELECT FileName,Name FROM DataBases");
    while(fields_2.next()){
        NotesFileNames.append(fields_2.value(0).toString());
        NotesNames.append(fields_2.value(1).toString());
    }
    QSqlQuery currentNotesBase;//(DataTex::DataTeX_Settings);
    currentNotesBase.exec("SELECT db.Name FROM Current_Databases cd JOIN DataBases db ON cd.Value = db.FileName;");
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
    currentbase_Exercises = DataTex::CurrentFilesDataBase.Database;
    // QSqlQuery LoadPreambles(DataTex::DataTeX_Settings);
    // LoadPreambles.exec("SELECT Id,Name FROM Preambles;");
    DTXSettings dtxsettings;
    QList<QStringList> preambleInfoList = dtxsettings.getCurrentPreambleInfo();
    for(QStringList list: preambleInfoList){
        ui->PreambleCombo->addItem(list[1],QVariant(list[0]));}
    QString CurrentPreamble;
    QSqlQuery CurrentPreambleQuery;//(DataTex::DataTeX_Settings);
    CurrentPreambleQuery.exec(QString("SELECT Name FROM Preambles WHERE Id = \"%1\";")
                              .arg(DataTex::CurrentPreamble));
    while(CurrentPreambleQuery.next()){CurrentPreamble = CurrentPreambleQuery.value(0).toString();};
    ui->PreambleCombo->setCurrentText(CurrentPreamble);
    ui->NotesPath->setText(DataTex::CurrentDocumentsDataBase.Path);
    ui->DatabaseLineEdit->setText(DataTex::CurrentFilesDataBase.Path);

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
    QObject::connect(ui->EditorFontSelect, &QFontComboBox::currentFontChanged, this, [=](QFont font){
        font.setPointSize(ui->EditorFontSize->value());
        emit selectEditorFont(font/*ui->EditorFontSelect->currentFont().toString()*/);
    });
    QObject::connect(ui->EditorFontSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int size){
        QFont font = ui->EditorFontSelect->currentFont();
        font.setPointSize(size);
        emit selectEditorFont(font);
    });

    ui->PdfLatexPath->setText(DataTex::PdfLatex_Command);
    ui->LatexPath->setText(DataTex::Latex_Command);
    ui->XelatexPath->setText(DataTex::XeLatex_Command);
    ui->LualatexPath->setText(DataTex::LuaLatex_Command);
    ui->PythontexPath->setText(DataTex::Pythontex_Command);
    ui->BibtexPath->setText(DataTex::Bibtex_Command);
    ui->AsymptotePath->setText(DataTex::Asymptote_Command);

    ui->DatabasePassword->setEnabled(false);
    ui->DocDatabasePassword->setEnabled(false);
    ui->DatabaseUserName->setEnabled(false);
    ui->DocDatabaseUserName->setEnabled(false);
    ui->KeyGeneratorFDB->setEnabled(false);
    ui->KeyGeneratorDDB->setEnabled(false);
    ui->SaveLocation->setText(QDir::homePath());
    ui->DatabasePrefix->setEnabled(false);
    ui->DocDatabasePrefix->setEnabled(false);

    ui->LanguageSelect->setIconSize(QSize(20,20));
    QDirIterator lang_list(":/languages/" , QStringList() << "*.qm", QDir::Files, QDirIterator::Subdirectories);
    while (lang_list.hasNext()){
        QString Lang = QFileInfo(lang_list.next()).baseName().remove("DataTex_");
        QStringList parts = QLocale(Lang).nativeLanguageName().split(' ', Qt::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i){
            parts[i].replace(0, 1, parts[i][0].toUpper());
        }
        QString LangName = parts.join(" ");
        ui->LanguageSelect->addItem(QIcon(":/languages/"+Lang+".png"),LangName,QVariant(Lang));
        qDebug()<<Lang;
    }
//    ui->LanguageSelect->setCurrentIndex(ui->LanguageSelect->findData(DataTex::currentlanguage,Qt::UserRole,Qt::MatchExactly));
    QSettings settings;
    currentLanguage = settings.value("Application_Settings/Language").toString();
    ui->EditorFontSelect->setCurrentFont(settings.value("Application_Settings/EditorFont").toString());
    ui->EditorFontSize->setValue(settings.value("Application_Settings/EditorFontSize").toInt());
    ui->commandColor->setText(settings.value("Application_Settings/EditorCommandColor").toString());
    ui->LanguageSelect->setCurrentIndex(ui->LanguageSelect->findData(currentLanguage,Qt::UserRole,Qt::MatchExactly));

    currentTheme = settings.value("Application_Settings/Theme").toString();
    ui->ThemeSelect->addItem("linux_breath",QVariant("0"));
    ui->ThemeSelect->addItem("light",QVariant("0"));
    ui->ThemeSelect->addItem("darkstyle",QVariant("1"));
    ui->ThemeSelect->addItem("dark-blue",QVariant("1"));
    ui->ThemeSelect->addItem("dark-red",QVariant("1"));
    ui->ThemeSelect->addItem("dark-purple",QVariant("1"));
    ui->ThemeSelect->addItem("dark-green",QVariant("1"));
    ui->ThemeSelect->setCurrentIndex(ui->ThemeSelect->findText(currentTheme,Qt::MatchExactly));

    ui->savePasswordFDB->setEnabled(false);
    ui->savePasswordDDB->setEnabled(false);
    ui->saveFDBPrefix->setEnabled(false);
    ui->saveDDBPrefix->setEnabled(false);
    connect(ui->savePasswordFDB,&QPushButton::clicked,this,[&](){
        ui->EncryptDatabase->setChecked(false);
        QSqlQuery fdb_encription;//(DataTex::DataTeX_Settings);
        QString Password = QCryptographicHash::hash(ui->DatabasePassword->text().toUtf8(),QCryptographicHash::Sha256);
        QString db = QFileInfo(ui->ComboBaseList->currentData().toString()).baseName();
        fdb_encription.exec(QString("UPDATE DataBases SET UserName = '%1' WHERE FileName = '%2'")
                                .arg(ui->DatabaseUserName->text(),QFileInfo(ui->ComboBaseList->currentData().toString()).baseName()));
        fdb_encription.exec(QString("UPDATE DataBases SET PassWord = '%1' WHERE FileName = '%2'")
                                .arg(Password,db));

    });
    connect(ui->savePasswordDDB,&QPushButton::clicked,this,[&](){
        ui->EncryptDocDatabase->setChecked(false);
        QSqlQuery fdb_encription;//(DataTex::DataTeX_Settings);
        QString Password = QCryptographicHash::hash(ui->DocDatabasePassword->text().toUtf8(),QCryptographicHash::Sha256).data();
        QString db = QFileInfo(ui->ComboNote->currentData().toString()).baseName();
        fdb_encription.exec(QString("UPDATE DataBases SET UserName = '%1' WHERE FileName = '%2'")
                                .arg(ui->DocDatabaseUserName->text(),db));
        fdb_encription.exec(QString("UPDATE DataBases SET PassWord = '%1' WHERE FileName = '%2'")
                                .arg(Password,db));
    });

    connect(ui->saveFDBPrefix,&QPushButton::clicked,this,[&](){
        ui->UseDatabasePrefix->setChecked(false);
        QSqlQuery fdb_prefix;//(DataTex::DataTeX_Settings);
        QString db = QFileInfo(ui->ComboBaseList->currentData().toString()).baseName();
        fdb_prefix.exec(QString("UPDATE DataBases SET Prefix = '%1' WHERE FileName = '%2'")
                                .arg(ui->DatabasePrefix->text(),db));

    });
    connect(ui->savePasswordDDB,&QPushButton::clicked,this,[&](){
        ui->UseDocDatabasePrefix->setChecked(false);
        QSqlQuery fdb_prefix;//(DataTex::DataTeX_Settings);
        QString db = QFileInfo(ui->ComboNote->currentData().toString()).baseName();
        fdb_prefix.exec(QString("UPDATE DataBases SET Prefix = '%1' WHERE FileName = '%2'")
                            .arg(ui->DocDatabasePrefix->text(),db));
    });
    QString enc = QCryptographicHash::hash("password",QCryptographicHash::Sha1).toBase64();
    qDebug()<<enc;
}

Settings::~Settings()
{
    delete ui;
}

void Settings::LoadTables(QString database)
{
    QSqlQueryModel * Metadata = new QSqlQueryModel(this);
//    QSqlQueryModel * Bibliography = new QSqlQueryModel(this);
    QSqlQuery tableQuery;//(DataTex::DataTeX_Settings);
    tableQuery.prepare(QString("SELECT m.Id AS 'Field Id',m.Name AS 'Name' FROM Metadata_per_Database md JOIN Metadata m ON md.Metadata_Id = m.Id WHERE Database_FileName = \"%1\"")
                       .arg(database));
    tableQuery.exec();
    Metadata->setQuery(tableQuery);
    ui->DatabaseFieldTable->setModel(Metadata);
    ui->DatabaseFieldTable->show();
    for (int c = 0; c < ui->DatabaseFieldTable->horizontalHeader()->count(); ++c)
    {
        ui->DatabaseFieldTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void Settings::LoadDocTables(QString database)
{
    QSqlQueryModel * DocMetadata = new QSqlQueryModel(this);
    QSqlQuery tableQuery;//(DataTex::DataTeX_Settings);
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

void Settings::on_BaseButton_clicked()
{
    newbasefolder = new BaseFolder(this);
    connect(newbasefolder,SIGNAL(newbase(QString,QString,QString)),this,SLOT(CreateBaseFolder(QString,QString,QString)));
    newbasefolder->show();
    newbasefolder->activateWindow();
}

void Settings::CreateBaseFolder(QString path,QString FolderName,QString fileName)
{
    QString FullPath = path+QDir::separator()+FolderName+QDir::separator()+fileName+".db";
    QSqlQuery AddBaseQuery;//(DataTex::DataTeX_Settings);
    AddBaseQuery.exec(QString("INSERT INTO Databases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(fileName,FolderName,FullPath));
    ui->DatabaseLineEdit->setText(FullPath);
    ui->ComboBaseList->addItem(FolderName,QVariant(fileName));
    ui->ComboBaseList->setCurrentText(FolderName);
    ui->ComboBaseList->setEnabled(true);
    ui->DeleteFilesBase->setEnabled(true);
}

void Settings::CreateNoteFolder(QString path,QString FolderName,QString FileName)
{
    QString FullPath = path+FolderName+QDir::separator()+FileName+".db";
    ui->NotesPath->setText(FullPath);
    ui->ComboNote->addItem(FolderName,QVariant(FileName));
    ui->ComboNote->setCurrentText(FolderName);
    ui->ComboNote->setEnabled(true);
    ui->DeleteBase->setEnabled(true);
}

void Settings::on_buttonBox_accepted()
{
    QString Text = ui->PreambleText->toPlainText();
    QSqlQuery WritePreambleQuery;//(DataTex::DataTeX_Settings);
    QString preamble = ui->PreambleCombo->currentData().toString();
    QSqlQuery SaveData;//(DataTex::DataTeX_Settings);
    SaveData.exec(QString("UPDATE Initial_Settings SET Value = \"%1\" WHERE Setting = 'Current_Preamble'")
                  .arg(preamble));
    WritePreambleQuery.exec(QString("UPDATE Preambles SET Preamble_Content = \"%1\" "
                                    "WHERE Id = \"%2\";").arg(Text,ui->PreambleCombo->currentData().toString()));
//    SelectLanguage(ui->LanguageSelect->currentData().toString());
    QSqlQuery saveFont;//(DataTex::DataTeX_Settings);
    saveFont.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2'").arg(ui->EditorFontSelect->currentText(),"EditorFont"));

    QSettings settings;
    settings.beginGroup("Application_Settings");
    settings.setValue("EditorFont",ui->EditorFontSelect->currentText());
    settings.setValue("EditorFontSize",ui->EditorFontSize->value());
    settings.endGroup();

    //Αν εγιγαν αλλαγές τότε
    if(currentLanguage != ui->LanguageSelect->currentData().toString() ||
        currentTheme != ui->ThemeSelect->currentText()){
        QMessageBox::StandardButton resBtn = QMessageBox::warning( this,
                                                                  tr("Warning"),tr("The app needs to restart to apply changes."),
                                                                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            settings.setValue("Application_Settings/Language",ui->LanguageSelect->currentData().toString());
            settings.setValue("Application_Settings/Theme",ui->ThemeSelect->currentText());
            settings.setValue("Application_Settings/IsThemeDark",ui->ThemeSelect->currentData().toString());
            settings.endGroup();
            accept();
            QString program = qApp->arguments()[0];
            QStringList arguments = qApp->arguments().mid(1); // remove the 1st argument - the program name
            qApp->quit();
            QProcess::startDetached(program, arguments);
        }
        else{
            accept();
        }
    }
//    else{
//        if(currentTheme != ui->ThemeSelect->currentData().toString()){
//            //Apply new theme
//            settings.setValue("Application_Settings/Theme",ui->ThemeSelect->currentData().toString());
//            QFile styleFile(":/qdarkstyle/"+ui->ThemeSelect->currentData().toString()+"/"+ui->ThemeSelect->currentData().toString()+".qss");
//            styleFile.open(QFile::ReadOnly);
//            QByteArray bytes = styleFile.readAll();
//            QApplication *app = (QApplication*)QApplication::instance();
//            app->setStyleSheet(bytes);
//            styleFile.close();
//            //---------------
//        }
//        accept();
//    }
}

void Settings::on_NoteButton_clicked()
{
    newnotefolder = new BaseFolder(this);
    connect(newnotefolder,SIGNAL(newdocbase(QString,QString,QString)),this,SLOT(CreateNoteFolder(QString,QString,QString)));
    newnotefolder->show();
    newnotefolder->activateWindow();
}

void Settings::on_buttonBox_rejected()
{
    reject();
}

void Settings::on_ComboBaseList_currentIndexChanged(int index)
{
    if(index!=-1){
        QString basename = ui->ComboBaseList->currentData().toString();
        QString path;
        LoadTables(basename);
        QSqlQuery Path;//(DataTex::DataTeX_Settings);
        Path.exec(QString("SELECT Path From Databases WHERE FileName = \"%1\"").arg(basename));
        while(Path.next()){
            path = Path.value(0).toString();
        }
        ui->DatabaseLineEdit->setText(path);
//        QString user = SqlFunctions::Get_String_From_Query(
//            QString("SELECT UserName FROM DataBases WHERE FileName = '%1'").arg(basename),DataTex::DataTeX_Settings);
//        QString pass = SqlFunctions::Get_String_From_Query(
//            QString("SELECT PassWord FROM DataBases WHERE FileName = '%1'").arg(basename),DataTex::DataTeX_Settings);
//        ui->DatabaseUserName->setText(user);
//        ui->DatabasePassword->setText(pass);
        QString prefix;// = SqlFunctions::Get_String_From_Query(QString("SELECT Prefix FROM DataBases WHERE FileName = '%1'").arg(basename),DataTex::DataTeX_Settings);
        ui->DatabasePrefix->setText(prefix);
    }
}

void Settings::on_DeleteFilesBase_clicked()
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
        QSqlQuery DeleteQuery;//(DataTex::DataTeX_Settings);
        DeleteQuery.exec("PRAGMA foreign_keys = ON");
        DeleteQuery.exec(QString("DELETE FROM Databases WHERE FileName = \"%1\"").arg(ui->ComboBaseList->currentData().toString()));
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

void Settings::on_ComboNote_currentIndexChanged(int index)
{
    if(index!=-1){
        QString notefolder = ui->ComboNote->currentData().toString();
        QString path;
        LoadDocTables(notefolder);
        QSqlQuery Path;//(DataTex::DataTeX_Settings);
        Path.exec(QString("SELECT Path From DataBases WHERE FileName = \"%1\"").arg(notefolder));
        while(Path.next()){
            path = Path.value(0).toString();
        }
        ui->NotesPath->setText(path);
//        QString user = SqlFunctions::Get_String_From_Query(
//            QString("SELECT UserName FROM DataBases WHERE FileName = '%1'").arg(notefolder),DataTex::DataTeX_Settings);
//        QString pass = SqlFunctions::Get_String_From_Query(
//            QString("SELECT PassWord FROM DataBases WHERE FileName = '%1'").arg(notefolder),DataTex::DataTeX_Settings);
//        ui->DocDatabaseUserName->setText(user);
//        ui->DocDatabasePassword->setText(pass);
    }
}

void Settings::on_DeleteBase_clicked()
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
        QSqlQuery DeleteQuery;//(DataTex::DataTeX_Settings);
        DeleteQuery.exec("PRAGMA foreign_keys = ON");
        DeleteQuery.exec(QString("DELETE FROM DataBases WHERE FileName = \"%1\"").arg(ui->ComboNote->currentData().toString()));
        if(ui->ComboNote->count()==1){
            ui->ComboNote->clear();
        }
        else {
        ui->ComboNote->removeItem(ui->ComboNote->currentIndex());
    }
    if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+NotesPath));}
    }
}

void Settings::on_AddBase_clicked()
{
    QString Database = QFileDialog::getOpenFileName(this,tr("Select a Database File"),QDir::homePath(),"sqlite db Files (*.db)");
    if(Database.isEmpty())return;
    QString DatabaseName = QFileInfo(Database).baseName();
    QStringList list = QFileInfo(Database).absolutePath().split(QDir::separator());
    QString folderName = list.last();
    QSqlQuery AddNotesQuery;//(DataTex::DataTeX_Settings);
    AddNotesQuery.exec(QString("INSERT INTO Databases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(DatabaseName,folderName,Database));

    QSqlDatabase addeddatabaseFile;
    addeddatabaseFile = QSqlDatabase::addDatabase("QSQLITE","addedbase");
    addeddatabaseFile.setDatabaseName(Database);
    addeddatabaseFile.open();

    QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    addeddatabaseFile.close();

    QSqlQuery add;//(DataTex::DataTeX_Settings);
    for (int i=0;i<MetadataIds.count();i++) {
        add.exec(QString("INSERT OR IGNORE INTO Metadata (Id,Name,Basic) VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
        add.exec("INSERT OR IGNORE INTO Metadata_per_Database (Database_FileName,Metadata_Id) VALUES (\""+DatabaseName+"\",\""+MetadataIds.at(i)+"\")");
    }
    ui->DatabaseLineEdit->setEnabled(true);
    ui->DatabaseLineEdit->setText(Database);
    ui->ComboBaseList->addItem(folderName,QVariant(DatabaseName));
    ui->ComboBaseList->setCurrentText(folderName);
    ui->DeleteFilesBase->setEnabled(true);
}

void Settings::on_PreambleCombo_currentIndexChanged(const QString &arg1)
{
    ui->RemovePreambleButton->setEnabled(ui->PreambleCombo->currentData().toString()!="Basic");
    QString Preambletext;
    ui->PreambleText->clear();
    QSqlQuery PreambleQuery;//(DataTex::DataTeX_Settings);
    PreambleQuery.exec(QString("SELECT Preamble_Content FROM Preambles WHERE Id = \"%1\";")
                       .arg(ui->PreambleCombo->currentData().toString()));
    while(PreambleQuery.next()){Preambletext = PreambleQuery.value(0).toString();}
    ui->PreambleText->insertPlainText(Preambletext);
}

void Settings::on_AddPreambleButton_clicked()
{
    PreambleSettings * newPreamle = new PreambleSettings(this);
    connect(newPreamle,SIGNAL(newpreamblesignal(QStringList)),this,SLOT(AddPreamble(QStringList)));
    newPreamle->show();
    newPreamle->activateWindow();
}

void Settings::AddPreamble(QStringList preamble)
{
    QSqlQuery AddPreamble;//(DataTex::DataTeX_Settings);
    AddPreamble.exec(QString("INSERT OR IGNORE INTO Preambles (Id,Name,Preamble_Content) VALUES (\"%1\",\"%2\",\"%3\")")
                     .arg(preamble[1],preamble[0],preamble[2]));
    ui->PreambleCombo->addItem(preamble[0],QVariant(preamble[1]));
    ui->PreambleCombo->setCurrentText(preamble[0]);
    ui->PreambleText->setText(preamble[2]);
}

void Settings::on_RemovePreambleButton_clicked()
{
    QString currentPreamble = ui->PreambleCombo->currentText();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Delete preamble"),tr("The preamble %1 will be deleted!\nDo you wish to proceed?")
                                    .arg(currentPreamble),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        QSqlQuery RemovePreamble;//(DataTex::DataTeX_Settings);
        RemovePreamble.exec(QString("DELETE FROM Preambles WHERE Id = \"%1\"").arg(ui->PreambleCombo->currentData().toString()));
        ui->PreambleCombo->removeItem(ui->PreambleCombo->currentIndex());
    }
}

void Settings::on_ListOfSettings_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

PasswordLineEdit::PasswordLineEdit(QWidget *parent):
    QLineEdit(parent)
{
    setEchoMode(QLineEdit::Password);
    QAction *action = addAction(QIcon::fromTheme("EyeOff"), QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton *>(action->associatedObjects().last());
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, &QToolButton::pressed, this, &PasswordLineEdit::onPressed);
    connect(button, &QToolButton::released, this, &PasswordLineEdit::onReleased);
}

void PasswordLineEdit::onPressed(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon::fromTheme("EyeOn"));
    setEchoMode(QLineEdit::Normal);
}

void PasswordLineEdit::onReleased(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon::fromTheme("EyeOff"));
    setEchoMode(QLineEdit::Password);
}

void PasswordLineEdit::enterEvent(QEnterEvent *event){
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

void Settings::on_AddDocDatabaseButton_clicked()
{
    QString Database = QFileDialog::getOpenFileName(this,tr("Select a Database File"),QDir::homePath(),"sqlite db Files (*.db)");
    if(Database.isEmpty())return;
    QString DatabaseName = QFileInfo(Database).baseName();
    QStringList list = QFileInfo(Database).absolutePath().split(QDir::separator());
    QString folderName = list.last();
    QSqlQuery AddNotesQuery;//(DataTex::DataTeX_Settings);
    AddNotesQuery.exec(QString("INSERT INTO DataBases (FileName,Name,Path) VALUES (\"%1\",\"%2\",\"%3\")")
                       .arg(DatabaseName,folderName,Database));

    QSqlDatabase addeddatabaseFile;
    addeddatabaseFile = QSqlDatabase::addDatabase("QSQLITE","addedbase");
    addeddatabaseFile.setDatabaseName(Database);
    addeddatabaseFile.open();

    QStringList MetadataIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    QStringList MetadataNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",addeddatabaseFile);
    addeddatabaseFile.close();

    QSqlQuery add;//(DataTex::DataTeX_Settings);
    for (int i=0;i<MetadataIds.count();i++) {
        add.exec(QString("INSERT OR IGNORE INTO DocMetadata (Id,Name,Basic) VALUES (\""+MetadataIds.at(i)+"\",\""+MetadataNames.at(i)+"\",0)"));
        add.exec("INSERT OR IGNORE INTO DocMetadata_per_Database (Database_FileName,Metadata_Id) VALUES (\""+DatabaseName+"\",\""+MetadataIds.at(i)+"\")");
    }
    ui->NotesPath->setEnabled(true);
    ui->NotesPath->setText(Database);
    ui->ComboNote->addItem(folderName,QVariant(DatabaseName));
    ui->ComboNote->setCurrentText(folderName);
    ui->DeleteBase->setEnabled(true);
}

void Settings::on_EncryptDocDatabase_toggled(bool checked)
{
    ui->DocDatabasePassword->setEnabled(checked);
    ui->KeyGeneratorDDB->setEnabled(checked);
    ui->DocDatabaseUserName->setEnabled(checked);
    ui->savePasswordDDB->setEnabled(checked);
}

void Settings::on_UseDocDatabasePrefix_clicked(bool checked)
{
    ui->DocDatabasePrefix->setEnabled(checked);
    ui->saveDDBPrefix->setChecked(checked);
}

void Settings::on_UseDatabasePrefix_clicked(bool checked)
{
    ui->DatabasePrefix->setEnabled(checked);
    ui->saveFDBPrefix->setEnabled(checked);
}

void Settings::on_EncryptDatabase_toggled(bool checked)
{
    ui->DatabasePassword->setEnabled(checked);
    ui->KeyGeneratorFDB->setEnabled(checked);
    ui->DatabaseUserName->setEnabled(checked);
    ui->savePasswordFDB->setEnabled(checked);
}

void Settings::on_OpenSaveLocation_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Select a save location"),DataTex::GlobalSaveLocation);
    if(path.isEmpty())return;
    DataTex::GlobalSaveLocation = path;
    ui->SaveLocation->setText(DataTex::GlobalSaveLocation);
    QSqlQuery query;//(DataTex::DataTeX_Settings);
    query.exec(QString("UPDATE Initial_Settings SET Value = \"%1\" WHERE Setting = 'SaveLocation'").arg(path));
}

void Settings::on_OpenPdfLatexPath_clicked()
{
    QString pdflatex = QFileDialog::getOpenFileName(this,
            tr("PdfLaTeX path"),DataTex::TexLivePath, "pdflatex");
    if(pdflatex.isEmpty()){
        return;
    }
    else{
        DataTex::PdfLatex_Command = pdflatex;
        DataTex::DTXBuildCommands[(int)CompileEngine::PdfLaTeX].Path = pdflatex;//CurrentBuildCommand
        ui->PdfLatexPath->setText(pdflatex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(pdflatex,"Pdflatex_Command"));
    }
}

void Settings::on_OpenLatexPath_clicked()
{
    QString latex = QFileDialog::getOpenFileName(this,
            tr("LaTeX path"),DataTex::TexLivePath, "latex");
    if(latex.isEmpty()){
        return;
    }
    else{
        DataTex::Latex_Command = latex;
        DataTex::DTXBuildCommands[(int)CompileEngine::LaTeX].Path = latex;
        ui->LatexPath->setText(latex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(latex,"Latex_Command"));
    }
}

void Settings::on_OpenXeLatexPath_clicked()
{
    QString xelatex = QFileDialog::getOpenFileName(this,
            tr("XeLaTeX path"),DataTex::TexLivePath, "xelatex");
    if(xelatex.isEmpty()){
        return;
    }
    else{
        DataTex::XeLatex_Command = xelatex;
        DataTex::DTXBuildCommands[(int)CompileEngine::XeLaTeX].Path = xelatex;
        ui->XelatexPath->setText(xelatex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(xelatex,"Xelatex_Command"));
    }
}

void Settings::on_OpenLuaLatexPath_clicked()
{
    QString lualatex = QFileDialog::getOpenFileName(this,
            tr("LuaLaTeX path"),DataTex::TexLivePath, "lualatex");
    if(lualatex.isEmpty()){
        return;
    }
    else{
        DataTex::LuaLatex_Command = lualatex;
        DataTex::DTXBuildCommands[(int)CompileEngine::LuaLaTeX].Path = lualatex;
        ui->LualatexPath->setText(lualatex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(lualatex,"Lualatex_Command"));
    }
}

void Settings::on_OpenPythontexPath_clicked()
{
    QString pythontex = QFileDialog::getOpenFileName(this,
            tr("PythonTeX path"),DataTex::TexLivePath, "pythontex");
    if(pythontex.isEmpty()){
        return;
    }
    else{
        DataTex::Pythontex_Command = pythontex;
        DataTex::DTXBuildCommands[(int)CompileEngine::PythonTex].Path = pythontex;
        ui->PythontexPath->setText(pythontex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(pythontex,"Pythontex_Command"));
    }
}

void Settings::on_OpenBibtexPath_clicked()
{
    QString bibtex = QFileDialog::getOpenFileName(this,
            tr("BibTeX path"),DataTex::TexLivePath, "bibtex");
    if(bibtex.isEmpty()){
        return;
    }
    else{
        DataTex::Bibtex_Command = bibtex;
        DataTex::DTXBuildCommands[(int)CompileEngine::BibTeX].Path = bibtex;
        ui->BibtexPath->setText(bibtex);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(bibtex,"Bibtex_Command"));
    }
}

void Settings::on_OpenAsymptotePath_clicked()
{
    QString asymptote = QFileDialog::getOpenFileName(this,
            tr("Asymptote path"),DataTex::TexLivePath, "asymptote");
    if(asymptote.isEmpty()){
        return;
    }
    else{
        DataTex::PdfLatex_Command = asymptote;
        DataTex::DTXBuildCommands[(int)CompileEngine::Asymptote].Path = asymptote;
        ui->AsymptotePath->setText(asymptote);
        QSqlQuery CommandsQuery;//(DataTex::DataTeX_Settings);
        CommandsQuery.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(asymptote,"Asymptote_Command"));
    }
}

void Settings::SelectLanguage(QString language)
{
    if (!DataTex::translator.isEmpty()){
        QCoreApplication::removeTranslator(&DataTex::translator);
    }
    DataTex::translator.load(":/languages/DataTex_"+language+".qm");
    QCoreApplication::installTranslator(&DataTex::translator);
    QSqlQuery saveLang;//(DataTex::DataTeX_Settings);
    saveLang.exec(QString("UPDATE Initial_Settings SET Value = '%1' WHERE Setting = '%2';").arg(language,"Language"));
}

void Settings::on_commandColorButton_clicked()
{
    QSettings settings;
    QString c = settings.value("Application_Settings/EditorCommandColor").toString();
    QString currentColor = (!c.isEmpty() && !c.isNull()) ? c : "#aa0000" ;
    QColorDialog * cd = new QColorDialog(currentColor,this);
    connect(cd,&QColorDialog::colorSelected,this,[=](QColor color){
        QSettings settings;
        settings.setValue("Application_Settings/EditorCommandColor",color.name());
        ui->commandColor->setText(color.name());
        ui->commandColorButton->setStyleSheet("background-color: "+color.name());
    });
    cd->show();
    cd->activateWindow();
}


void Settings::on_KeyGeneratorFDB_clicked()
{
    KeyGenerator * newKey = new KeyGenerator(this);
    connect(newKey,&KeyGenerator::password,this,[&](QString password){
        ui->DatabasePassword->setText(password);
    });
    newKey->show();
    newKey->activateWindow();
}


void Settings::on_KeyGeneratorDDB_clicked()
{
    KeyGenerator * newKey = new KeyGenerator(this);
    connect(newKey,&KeyGenerator::password,this,[&](QString password){
        ui->DocDatabasePassword->setText(password);
    });
    newKey->show();
    newKey->activateWindow();
}

