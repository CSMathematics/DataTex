#include "newdatabasefile.h"
#include "ui_newdatabasefile.h"
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QRadioButton>
#include <QRegExp>
#include "dtxsettings.h"


NewDatabaseFile::NewDatabaseFile(QWidget *parent, DTXFile *fileinfo, int mode) :
    QDialog(parent),
    ui(new Ui::NewDatabaseFile)
{
    ui->setupUi(this);
    Mode = mode;
    if(mode>0){
        FileName = fileinfo->Id;
        ImportedFileContent = FileCommands::ClearMetadataFromContent(fileinfo->Content);
    }
    else{
        FileName = "";
        ImportedFileContent = "";
    }
    CurrentFileContent = ImportedFileContent;
    // qDebug()<<CurrentFileContent;
    FileTypeGroup = new QButtonGroup(this);
    ui->NewFileContentText->setEnabled(false);
    ui->addChapter->setProperty("Info",tr("Add new chapter for field : "));
    ui->addSection->setProperty("Info",tr("Add new section for chapter : "));
    ui->addExType->setProperty("Info",tr("Add new subsection for section : "));

    QSettings settings;
    settings.beginGroup("NewDatabaseFile");
    saveSelections = settings.value("SaveNewFileSelections").toInt();//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SaveNewFileSelections'",DataTex::DataTeX_Settings).toInt();
    settings.endGroup();

    ui->FieldTable->setEnabled(false);
    ui->addField->setEnabled(false);
    ui->addChapter->setEnabled(false);
    ui->addSection->setEnabled(false);
    ui->addExType->setEnabled(false);
    ui->removeField->setEnabled(false);
    ui->removeChapter->setEnabled(false);
    ui->removeSection->setEnabled(false);
    ui->removeExType->setEnabled(false);
    currentbase = DataTex::CurrentFilesDataBase.Database;
    DataBase_Path = QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator();
    if(mode==0){
        metadata = new DTXFile;
    }
    else{
        metadata = fileinfo;
    }
    ImportedChaptersList = (mode == EditMode || mode == CloneModeContentAndMetadata)?fileinfo->getChaptersNames():QStringList();
    ImportedSectionList = (mode == EditMode || mode == CloneModeContentAndMetadata)?fileinfo->getSectionsNames():QStringList();
    ImportedSubSectionList = (mode == EditMode || mode == CloneModeContentAndMetadata)?fileinfo->getSubSectionsNames():QStringList();
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_ids.append(fields.value(0).toString());
        Field_Names.append(fields.value(1).toString());
    }

    LoadFileTypes();

    TheoryView = new QPdfViewer(this);
    ui->verticalLayout_5->addWidget(TheoryView);
    ui->splitter_2->setSizes(QList<int>({1,1, 400}));
    ui->splitter_4->setSizes(QList<int>({400,1}));
    TheoryView->show();

    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase));
    ui->horizontalLayout_13->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);
    ui->SaveSelectionsCheckBox->setEnabled(false);
    ui->NewFileContentText->toolBar->Save->setVisible(false);
    // QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    // QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    for(const DTXBuildCommand &build : qAsConst(DataTex::DTXBuildCommands)){
        ui->BuildBox->addItem(build.Name,QVariant::fromValue(build));
    }
    DTXSettings dtxsettings;
    QList<QStringList> preambleInfoList = dtxsettings.getCurrentPreambleInfo();
    for (const QStringList &list: preambleInfoList) {
        ui->PreambleBox->addItem(list.at(1),QVariant(list.at(0)));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    connect(ui->NewFileContentText->editor,&QTextEdit::textChanged,this,[=](){
        if(!ui->BuildBox->isEnabled()){
            ui->BuildBox->setEnabled(true);
            ui->PreambleBox->setEnabled(true);
        }
    });
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
    if(mode == EditMode){
        EditModeIsEnabled();
    }
    // qDebug()<<(mode==CloneModeOnlyContent || mode==CloneModeContentAndMetadata);
    if(mode==CloneModeOnlyContent || mode==CloneModeContentAndMetadata){
        CloneModeIsEnabled(mode==CloneModeContentAndMetadata);
    }
    // ui->splitter_2->setSizes(QList<int>({150, 300,350}));

    connect(ui->FieldTable, &QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        FieldsClicked(item);
    });
    connect(ui->FieldTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->FieldTable->selectionModel()->hasSelection()){
            currentField = ui->FieldTable->selectionModel()->currentIndex().data(Qt::DisplayRole).toStringList()[0];
        }
    });

    connect(ui->FilterChapters,&QLineEdit::textChanged,this,[&](QString text){
        for (int k1 = 0; k1 < ui->Chapters->count(); ++k1){
            ui->Chapters->item(k1)->setHidden(!ui->Chapters->item(k1)->text().contains(text,Qt::CaseInsensitive) &&
                                              !ui->Chapters->item(k1)->isSelected());
        }
    });
    connect(ui->FilterSections,&QLineEdit::textChanged,this,[&](QString text){
        for (int k1 = 0; k1 < ui->Sections->count(); ++k1){
            ui->Sections->item(k1)->setHidden(!ui->Sections->item(k1)->text().contains(text,Qt::CaseInsensitive) &&
                                              !ui->Sections->item(k1)->isSelected());
        }
    });
    connect(ui->FilterSubSections,&QLineEdit::textChanged,this,[&](QString text){
        for (int k1 = 0; k1 < ui->SubSections->count(); ++k1){
            ui->SubSections->item(k1)->setHidden(!ui->SubSections->item(k1)->text().contains(text,Qt::CaseInsensitive) &&
                                              !ui->SubSections->item(k1)->isSelected());
        }
    });
    ui->FilterChapters->setEnabled(false);
    ui->FilterSections->setEnabled(false);
    ui->FilterSubSections->setEnabled(false);
    ui->SaveSelectionsCheckBox->setChecked(saveSelections);
    if(saveSelections){
        InitialSettings();
    }
    ui->NewFileContentText->editor->setText(ImportedFileContent);

    connect(ui->Chapters,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        ChaptersClicked(item);
    });

    connect(ui->Sections,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        SectionClicked(item);
    });

    connect(ui->SubSections,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        SubSectionClicked(item);
    });

    connect(ui->Chapters->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->Chapters->selectionModel()->hasSelection()){
            currentChapter = ui->Chapters->selectionModel()->selectedRows()[0].data().toString();
        }
    });
    connect(ui->Sections->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->Sections->selectionModel()->hasSelection()){
            currentSection = ui->Sections->selectionModel()->selectedRows()[0].data().toString();
        }
    });
    connect(ui->SubSections->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->SubSections->selectionModel()->hasSelection()){
            currentSubSection = ui->SubSections->selectionModel()->selectedRows()[0].data().toString();
        }
    });
}

NewDatabaseFile::~NewDatabaseFile()
{
    delete ui;
    delete metadata;
    delete TheoryView;
    delete tagLine;
}

void NewDatabaseFile::EditModeIsEnabled()
{
    QString filetype = metadata->FileType.Id;
    for (int i=0;i<FileTypeGroup->buttons().count();i++) {
        if(FileTypeGroup->buttons().at(i)->property("Id").value<DTXFileType>().Id == filetype){
            FileTypeGroup->buttons().at(i)->setChecked(true);
            // qDebug()<<filetype;
        }
    }
    ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
    FieldsClicked(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
    for(int i=0;i<ImportedChaptersList.count();i++){
        for(QListWidgetItem * item:ui->Chapters->findItems(ImportedChaptersList[i],Qt::MatchExactly)){
            item->setSelected(true);
            ChaptersClicked(item);
        }
    }
    for(int i=0;i<ImportedSectionList.count();i++){
        for(QListWidgetItem * item:ui->Sections->findItems(ImportedSectionList[i],Qt::MatchExactly)){
            item->setSelected(true);
            SectionClicked(item);
        }
    }
    for(int i=0;i<ImportedSubSectionList.count();i++){
        for(QListWidgetItem * item:ui->SubSections->findItems(ImportedSubSectionList[i],Qt::MatchExactly)){
            item->setSelected(true);
            SubSectionClicked(item);
        }
    }
    ui->DifficultySpin->setValue(metadata->Difficulty);
    int preamble = ui->PreambleBox->findData(metadata->Preamble.Id);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
    if ( preamble != -1 ) {
        ui->PreambleBox->setCurrentIndex(preamble);
    }
    int build = ui->BuildBox->findText(metadata->BuildCommand);
    if ( build != -1 ) {
        ui->BuildBox->setCurrentIndex(build);
    }
    ui->DescriptionLine->setText(metadata->Description);
    ui->NewFileContentText->editor->setText(FileCommands::NewFileText(FileName,ImportedFileContent));
    ui->FilePathLine->setText(QFileInfo(metadata->Path).absolutePath()+QDir::separator());
    ui->FileNameLine->setText(QFileInfo(metadata->Path).fileName());
}

void NewDatabaseFile::CloneModeIsEnabled(int cloneMode)
{
    // qDebug()<<cloneMode;
    if(cloneMode == CloneModeContentAndMetadata){
        QString filetype = metadata->FileType.Id;
        for (int i=0;i<FileTypeGroup->buttons().count();i++) {
            if(FileTypeGroup->buttons().at(i)->property("Id").value<DTXFileType>().Id == filetype){
                FileTypeGroup->buttons().at(i)->setChecked(true);
            }
        }
        ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
        metadata->Field[0] = ui->FieldTable->currentItem()->data(Qt::UserRole).toString();
        FieldsClicked(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
        for(int i=0;i<ImportedChaptersList.count();i++){
            for(QListWidgetItem * item:ui->Chapters->findItems(ImportedChaptersList[i],Qt::MatchExactly)){
                if(metadata->Chapters[i][1]==item->text()){
                    metadata->Chapters[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->Chapters[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    // qDebug()<<metadata->Chapters[i];
                }
                item->setSelected(true);
                ChaptersClicked(item);
            }
        }
        // qDebug()<<"Sections from dtex   "<<metadata->Sections;
        for(int i=0;i<ImportedSectionList.count();i++){
            for(QListWidgetItem * item:ui->Sections->findItems(ImportedSectionList[i],Qt::MatchExactly)){
                if(metadata->Sections[i][1]==item->text()){
                    metadata->Sections[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->Sections[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    // qDebug()<<"Sections from dtex   "<<metadata->Sections[i];
                }
                item->setSelected(true);
                SectionClicked(item);
            }
        }
        for(int i=0;i<ImportedSubSectionList.count();i++){
            for(QListWidgetItem * item:ui->SubSections->findItems(ImportedSubSectionList[i],Qt::MatchExactly)){
                if(metadata->SubSections[i][1]==item->text()){
                    metadata->SubSections[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->SubSections[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    // qDebug()<<metadata->SubSections[i];
                }
                item->setSelected(true);
                SubSectionClicked(item);
            }
        }
        ui->DifficultySpin->setValue(metadata->Difficulty);
        int preamble = ui->PreambleBox->findData(metadata->Preamble.Id);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
        if ( preamble != -1 ) {
            ui->PreambleBox->setCurrentIndex(preamble);
        }
        int build = ui->BuildBox->findText(metadata->BuildCommand);
        if ( build != -1 ) {
            ui->BuildBox->setCurrentIndex(build);
        }
    }
    ui->DescriptionLine->setText(metadata->Description);
    metadata->Path = FileCommands::NewFilePathAndId(metadata,/*needsSubSection*/true);
    metadata->Id = QFileInfo(metadata->Path).baseName();
    metadata->Content = FileCommands::ClearMetadataFromContent(metadata->Content);
    ui->NewFileContentText->editor->setText(FileCommands::NewFileText(metadata->Id,metadata->Content,DataTex::CurrentFilesDataBase.Database));
}

void NewDatabaseFile::CloneModeIsEnabled(DTXFile * fileInfo,bool cloneMetadata)
{
    reset();
    metadata = fileInfo;
    CloneModeIsEnabled(cloneMetadata);
}

void NewDatabaseFile::updateTableView(QTableView * table,QString QueryText)
{
    QSqlQueryModel * model = new QSqlQueryModel(this);
    QSqlQuery query(currentbase);
    query.prepare(QueryText);
    query.exec();
    model->setQuery(query);
    table->setModel(model);
    table->show();
}

void NewDatabaseFile::ExerciseFileList_selection_changed()
{
    QItemSelectionModel *select = ui->ExerciseFileList->selectionModel();
    int row = -1;
    if(select->hasSelection()){ //check if has selection
        row = select->selectedRows().at(0).row();
    }
    QString FilePath = ui->ExerciseFileList->model()->data(ui->ExerciseFileList->model()->index(row,2)).toString();
    QString outName = QFileInfo(FilePath).path() + QDir::separator()+ QFileInfo(FilePath).baseName()+".pdf";
    if(QFileInfo::exists(outName)){
        FileCommands::ShowPdfInViewer(outName,TheoryView);
    }
}

void NewDatabaseFile::on_ExerciseFileList_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString FilePath = ui->ExerciseFileList->model()->data(ui->ExerciseFileList->model()->index(row,2)).toString();
    QString outName = QFileInfo(FilePath).path() + QDir::separator()+ QFileInfo(FilePath).baseName()+".pdf";

    if(QFileInfo::exists(outName)){
        QDesktopServices::openUrl(QUrl("file:///"+outName));
    }
    else{
        FileCommands::CreateTexFile(FilePath,0,"");
//        FileCommands::BuildDocument();
        QDesktopServices::openUrl(QUrl("file:///"+FilePath));
    }
}

void NewDatabaseFile::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Close Window"),tr("Do you want to close the window?"),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        ui->FilePathLine->clear();
        ui->FileNameLine->clear();
        ui->NewFileContentText->editor->clear();
        event->accept();
    }
}

void NewDatabaseFile::on_buttonBox_accepted()
{
    setDBFileInfo();

    QString Content = ui->NewFileContentText->editor->toPlainText();
    QTextStream contentline(&Content);
    QStringList Line;
    while(!contentline.atEnd()){
        Line.append(contentline.readLine());
    }
    Content=Line.join("\\qt_endl");
    if(Mode !=EditMode || Mode == ImportMode){
        FileCommands::AddNewFileToDatabase(metadata,currentbase);
    }
    else if(Mode == EditMode && Mode != ImportMode){
        FileCommands::UpdateFileInfo(metadata,currentbase);
    }
    QSqlQuery insertTag(currentbase);
    tags = tagLine->GetTags();
    for(QString tag:tags){
        if(!tag.isEmpty()){
            insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
            insertTag.exec("INSERT OR IGNORE INTO Tags_per_File (Tag_Id,File_Id) VALUES (\""+tag+"\",\""+metadata->Id+"\")");
        }
    }
    emit acceptSignal(metadata->Path);

    if(ui->SaveSelectionsCheckBox->isChecked()){
        SaveSettings();
    }
    accept();
}

void NewDatabaseFile::on_buttonBox_rejected()
{
    if(ui->SaveSelectionsCheckBox->isEnabled()){
        SaveSettings();
    }
    reject();
}

void NewDatabaseFile::on_addField_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddField(currentbase);
        AddField.prepare(QString("INSERT INTO Fields (Id,Name) VALUES(\"%1\",\"%2\")").arg(Line[1],Line[0]));
        if(AddField.exec()){
            int rows = ui->FieldTable->count();
            ui->FieldTable->addItem(Line[0]);
            ui->FieldTable->item(rows)->setData(Qt::UserRole,Line[1]);
            ui->FieldTable->item(rows)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            ui->FieldTable->item(rows)->setCheckState(Qt::Unchecked);
        }
        else{
            QMessageBox::warning(this,tr("Error"),AddField.lastError().text(),QMessageBox::Ok);
        }
    });
    newData->show();
    newData->activateWindow();
}

void NewDatabaseFile::on_addChapter_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addChapter->property("Info").toString()+currentField);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddChapter(currentbase);
        AddChapter.prepare(QString("INSERT INTO Chapters (Id,Name,Field) VALUES(\"%1\",\"%2\",\"%3\")")
                      .arg(Line[1],Line[0],ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));

        if(AddChapter.exec()){
            int rows = ui->Chapters->count();
            ui->Chapters->addItem(Line[0]);
            ui->Chapters->item(rows)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->FieldTable->currentItem()->data(Qt::UserRole).toString()}));
            ui->Chapters->item(rows)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            ui->Chapters->item(rows)->setCheckState(Qt::Unchecked);
       }
        else{
            QMessageBox::warning(this,tr("Error"),AddChapter.lastError().text(),QMessageBox::Ok);
        }
    });
    newData->show();
    newData->activateWindow();
}

void NewDatabaseFile::on_removeField_clicked()
{
    QSqlQuery RemoveField(currentbase);
    RemoveField.exec("PRAGMA foreign_keys = ON");
    RemoveField.exec(QString("DELETE FROM Fields WHERE Id = \"%1\"").arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    delete ui->FieldTable->takeItem(ui->FieldTable->currentRow());
    ui->FieldTable->setCurrentRow(0);
    // FieldsClicked();
}


void NewDatabaseFile::on_removeChapter_clicked()
{
    QSqlQuery RemoveChapter(currentbase);
    RemoveChapter.exec("PRAGMA foreign_keys = ON");
    RemoveChapter.exec(QString("DELETE FROM Chapters WHERE Id = \"%1\" AND Field = \"%2\"")
                     .arg(ui->Chapters->currentItem()->data(Qt::UserRole).toString()
                          ,ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    ui->Chapters->takeItem(ui->Chapters->currentRow());
    // ChaptersClicked();
}


void NewDatabaseFile::on_addSection_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addSection->property("Info").toString()+currentChapter);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection(currentbase);
        AddSection.prepare(QString("INSERT INTO Sections (Id,Name,Chapter) VALUES(\"%1\",\"%2\",\"%3\")")
                        .arg(Line[1],Line[0],ui->Chapters->currentItem()->data(Qt::UserRole).toStringList()[0]));
        if(AddSection.exec()){
            int rows = ui->Sections->count();
            ui->Sections->addItem(Line[0]);
            ui->Sections->item(rows)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->Chapters->currentItem()->data(Qt::UserRole).toStringList()[0]}));
            ui->Sections->item(rows)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            ui->Sections->item(rows)->setCheckState(Qt::Unchecked);
        }
        else{
            QMessageBox::warning(this,tr("Error"),AddSection.lastError().text(),QMessageBox::Ok);
        }
    });
    newData->show();
    newData->activateWindow();
}

void NewDatabaseFile::on_removeSection_clicked()
{
    QSqlQuery RemoveSection(currentbase);
    RemoveSection.exec("PRAGMA foreign_keys = ON");
    RemoveSection.exec(QString("DELETE FROM Sections WHERE Id = \"%1\"")
                       .arg(ui->Sections->currentItem()->data(Qt::UserRole).toString()));
    ui->Sections->takeItem(ui->Sections->currentRow());
    // SectionClicked();
}

void NewDatabaseFile::on_addExType_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addExType->property("Info").toString()+currentSection);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection1(currentbase);
        QSqlQuery AddSection2(currentbase);
        AddSection1.prepare(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")").arg(Line[1],Line[0]));
        AddSection2.prepare(QString("INSERT OR IGNORE INTO \"Sections_Exercises\" (\"Exercise_Id\",\"Section_Id\") VALUES(\"%1\",\"%2\")")
                        .arg(Line[1],ui->Sections->currentItem()->data(Qt::UserRole).toStringList()[0]));
        if(AddSection1.exec() && AddSection2.exec()){
            int rows = ui->SubSections->count();
            ui->SubSections->addItem(Line[0]);
            ui->SubSections->item(rows)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->Sections->currentItem()->data(Qt::UserRole).toStringList()[0]}));
            ui->SubSections->item(rows)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            ui->SubSections->item(rows)->setCheckState(Qt::Unchecked);
        }
        else{
            QMessageBox::warning(this,tr("Error"),AddSection2.lastError().text(),QMessageBox::Ok);
        }
    });

    newData->show();
    newData->activateWindow();
}

void NewDatabaseFile::on_removeExType_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete exercise type",tr("The exercise type %1 will be deleted!\nDo you wish to proceed?").arg(ui->SubSections->currentItem()->data(Qt::UserRole).toString()),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
            QSqlQuery RemoveExerciseType(currentbase);
            RemoveExerciseType.exec(QString("DELETE FROM \"Sections_Exercises\" WHERE \"Exercise_Id\" = \"%1\" AND \"Section_Id\" = \"%2\"")
                             .arg(ui->SubSections->currentItem()->data(Qt::UserRole).toString(),
                                  ui->Sections->currentItem()->data(Qt::UserRole).toString()));
        ui->SubSections->takeItem(ui->SubSections->currentRow());
        // SubSectionClicked();
    }
}

void NewDatabaseFile::on_addFileType_clicked()
{
    NewFileType * newFile = new NewFileType(this,(DTXDatabaseType)DTXDatabaseType::FilesDB);
    connect(newFile,&NewFileType::filedata,this,[=](DTXFileType data){
        QRadioButton * newButton = new QRadioButton(data.Name,this);
        ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
        newButton->setProperty("Id",QVariant::fromValue(data));
        CustomFileTypesList.append(newButton);
        FileTypeGroup->addButton(newButton);
        connect(newButton, &QRadioButton::toggled, this,
                [=](){
            FileTypeClicked();
        });
        ui->gridLayout_10->addWidget(newButton,ui->gridLayout_10->rowCount(),0);
//        ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
        ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
        NewFileType::CreateNewDatabaseFileType(DataTex::CurrentFilesDataBase.Database,DataTex::CurrentFilesDataBase.Type,data);
    });
    newFile->show();
    newFile->activateWindow();
}

void NewDatabaseFile::on_removeFileType_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete exercise type",tr("The filetype type %1 will be deleted!\nDo you wish to proceed?").arg(FileType.Id),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
            QSqlQuery RemoveExerciseType(currentbase);
            RemoveExerciseType.exec(QString("DELETE FROM FileTypes WHERE Id = \"%1\"").arg(FileType.Id));
    }
}

void NewDatabaseFile::FileTypeClicked()
{
    QRadioButton * button = qobject_cast<QRadioButton*>(sender());
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    FileType = button->property("Id").value<DTXFileType>();
    metadata->FileType = FileType;
    ui->FieldTable->setEnabled(true);
    ui->addField->setEnabled(true);
    ui->removeField->setEnabled(true);
    ui->FilterChapters->setEnabled(false);
    ui->FilterSections->setEnabled(false);
    ui->FilterSubSections->setEnabled(false);
    ui->FieldTable->clear();
    ui->Chapters->clearSelection();
    ui->Sections->clearSelection();
    ui->SubSections->clearSelection();
    for (int item=0;item<Field_Names.count();item++ ) {
        ui->FieldTable->addItem(Field_Names.at(item));
        ui->FieldTable->item(item)->setData(Qt::UserRole,
                        QVariant::fromValue<QStringList>({Field_ids.at(item),Field_Names.at(item)}));
        ui->FieldTable->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        ui->FieldTable->item(item)->setCheckState(Qt::Unchecked);
    }

    ui->FilePathLine->clear();
    ui->FileNameLine->clear();
    CurrentFileContent = FileCommands::ClearMetadataFromContent(ui->NewFileContentText->editor->toPlainText());
    ui->NewFileContentText->setEnabled(false);

    ui->removeFileType->setEnabled(CustomFileTypesList.contains(button));
}

void NewDatabaseFile::FieldsClicked(QListWidgetItem * item)
{
    // Selected_Field_names.clear();
    // Selected_Field_ids.clear();
    ui->FilterChapters->setEnabled(true);
    // for(QListWidgetItem * item:ui->FieldTable->selectedItems()) {
    //     Selected_Field_ids.append(item->data(Qt::UserRole).toStringList()[0]);
    //     Selected_Field_names.append(item->text());
    // }

    QString FieldId = item->data(Qt::UserRole).toStringList()[0];
    QString FieldName = item->text();
    if(item->checkState() == Qt::Checked){
        Selected_Field_ids.insert(FieldId);
        Selected_Field_names.insert(FieldName);
    }
    else{
        Selected_Field_ids.remove(FieldId);
        Selected_Field_names.remove(FieldName);
    }

    QStringList Chapter_Names;
    QList<QStringList> ChapterList = SqlFunctions::GetRecordList(QString("SELECT * FROM Chapters WHERE Field IN (\"%1\")").arg(Selected_Field_ids.values().join("\",\"")),currentbase);
    // qDebug()<<QString("SELECT Name,Id,Field FROM Chapters WHERE Field IN (\"%1\")").arg(Selected_Field_ids.join("\",\""));
    ui->Chapters->clear();
    ui->Sections->clear();
    ui->SubSections->clear();
    for (int i=0;i<ChapterList.count();i++ ) {
        ui->Chapters->addItem(ChapterList.at(i)[1]);
        ui->Chapters->item(i)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(ChapterList[i]));
        ui->Chapters->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        QString chapterId = ChapterList.at(i)[0];
        bool isSelected = Selected_Chapters_ids.contains(chapterId);
        ui->Chapters->item(i)->setCheckState((Qt::CheckState)(2*isSelected));
        Chapter_Names.append(ChapterList.at(i)[1]);
    }

    QCompleter *completer = new QCompleter(Chapter_Names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->FilterChapters->setCompleter(completer);
    DataTex::StretchColumnsToWidth(ui->ExerciseFileList);
    ui->ExerciseFileList->setColumnHidden(2,true);
    ui->addChapter->setEnabled(true);
    ui->removeChapter->setEnabled(true);
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.arg(Selected_Field_ids.values().join("|"),"","","",FileType.Id));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->NewFileContentText->setEnabled(false);
    metadata->Field = GetDataFromSelectionList(ui->FieldTable).at(0);
    // qDebug()<<"Field clicked";
}

void NewDatabaseFile::ChaptersClicked(QListWidgetItem * item)
{
    QString ChapterId = item->data(Qt::UserRole).toStringList()[0];
    QString ChapterName = item->text();
    if(item->checkState() == Qt::Checked){
        Selected_Chapters_ids.insert(ChapterId);
        Selected_Chapters_names.insert(ChapterName);
    }
    else{
        Selected_Chapters_ids.remove(ChapterId);
        Selected_Chapters_names.remove(ChapterName);
    }
    ui->FilterSections->setEnabled(Selected_Chapters_ids.count()>0);

    if(ui->Chapters->selectionModel()->hasSelection()){
        currentChapter = ui->Chapters->selectionModel()->currentIndex().data(Qt::DisplayRole).toString();
    }

    QStringList Sections_Names;
    QList<QStringList> SectionList = SqlFunctions::GetRecordList(QString("SELECT * FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.values().join("\",\"")),currentbase);
    // qDebug()<<QString("SELECT Name,Id,Chapter FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.join("\",\""));
    ui->Sections->clear();
    for (int i=0;i<SectionList.count();i++ ) {
        ui->Sections->addItem(SectionList.at(i)[1]);
        Sections_Names.append(SectionList.at(i)[1]);
        ui->Sections->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        QString sectionId = SectionList.at(i)[0];
        bool isSelected = Selected_Sections_ids.contains(sectionId);
        ui->Sections->item(i)->setCheckState((Qt::CheckState)(2*isSelected));
        ui->Sections->item(i)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(SectionList.at(i)));
    }
    QCompleter *completer = new QCompleter(Sections_Names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->FilterSections->setCompleter(completer);
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                    arg(Selected_Field_ids.values().join("|"),Selected_Chapters_ids.values().join("|"),"","",FileType.Id));
    ui->addSection->setEnabled(true);
    ui->removeSection->setEnabled(true);
    ui->FileInfo->clear();
    ui->NewFileContentText->setEnabled(false);
    metadata->Chapters = GetDataFromSelectionList(ui->Chapters);
    // qDebug()<<metadata->Chapters;
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
}

void NewDatabaseFile::SectionClicked(QListWidgetItem * item)
{
    QString SectionId = item->data(Qt::UserRole).toStringList()[0];
    QString SectionName = item->text();
    if(item->checkState() == Qt::Checked){
        Selected_Sections_ids.insert(SectionId);
        Selected_Sections_names.insert(SectionName);
    }
    else{
        Selected_Sections_ids.remove(SectionId);
        Selected_Sections_names.remove(SectionName);
    }
    // qDebug()<<Selected_Sections_names;
    ui->FilterSubSections->setEnabled(Selected_Sections_ids.count()>0);
    if(ui->Sections->selectionModel()->hasSelection()){
        currentSection = ui->Sections->selectionModel()->currentIndex().data(Qt::DisplayRole).toString();
    }

//    if(needsSubSection){
        ui->FilterSubSections->setEnabled(true);
        QStringList SubSections_Names;
        // SubSections_ids.clear();
        QList<QStringList> SubSectionList = SqlFunctions::GetRecordList(QString("SELECT DISTINCT se.Exercise_Id,e.Name,"
                                                             "replace(group_concat(se.Section_Id),',','|') "
                                                             "FROM Sections_Exercises se "
                                                             "JOIN Exercise_Types e ON e.Id=se.Exercise_Id "
                                                             "WHERE se.Section_Id IN (\"%1\")"
                                                             "GROUP BY 1").arg(Selected_Sections_ids.values().join("\",\"")),currentbase);
        // qDebug()<<QString("SELECT DISTINCT e.Name,se.Exercise_Id,se.Section_Id FROM Sections_Exercises se JOIN Exercise_Types e ON e.Id=se.Exercise_Id WHERE se.Section_Id IN (\"%1\")").arg(Selected_Sections_ids.join("\",\""));
        ui->SubSections->clear();
        for (int i=0;i<SubSectionList.count();i++) {
            ui->SubSections->addItem(SubSectionList.at(i)[1]);
            SubSections_Names.append(SubSectionList.at(i)[1]);
            ui->SubSections->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            QString subSectionId = SubSectionList.at(i)[0];
            bool isSelected = Selected_SubSections_ids.contains(subSectionId);
            ui->SubSections->item(i)->setCheckState((Qt::CheckState)(2*isSelected));
            ui->SubSections->item(i)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(SubSectionList.at(i)));
        }
        ui->addExType->setEnabled(true);
        ui->removeExType->setEnabled(true);
        QCompleter *completer = new QCompleter(SubSections_Names, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->FilterSubSections->setCompleter(completer);
//    }
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                    arg(Selected_Field_ids.values().join("|"),
                        Selected_Chapters_ids.values().join("|"),Selected_Sections_ids.values().join("|"),"",FileType.Id));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->NewFileContentText->setEnabled(true);
    UpdateFileInfo();
    metadata->Sections = GetDataFromSelectionList(ui->Sections);
}

void NewDatabaseFile::SubSectionClicked(QListWidgetItem * item)
{
    QString SubSectionId = item->data(Qt::UserRole).toStringList()[0];
    QString SubSectionName = item->text();
    if(item->checkState() == Qt::Checked){
            Selected_SubSections_ids.insert(SubSectionId);
            Selected_SubSections_names.insert(SubSectionName);
    }
    else{
            Selected_SubSections_ids.remove(SubSectionId);
            Selected_SubSections_names.remove(SubSectionName);
    }
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    UpdateFileInfo();
    metadata->SubSections = GetDataFromSelectionList(ui->SubSections);
}

void NewDatabaseFile::UpdateFileInfo()
{
    CurrentFileContent = FileCommands::ClearMetadataFromContent(ui->NewFileContentText->editor->toPlainText());
    QString Fields =  Selected_Field_names.values().join(" , ");
    QString Chapters = Selected_Chapters_names.values().join("\n\t- ");
    QString Sections = Selected_Sections_names.values().join("\n\t- ");
    QString SubSections = Selected_SubSections_names.values().join("\n\t- ");
    QString Text = "### File Info\n- **Fields**\n";
    Text += Fields+"\n";
    Text += "- **Chapters**\n\t- "+Chapters+"\n ";
    Text += "- **Sections**\n\t- "+Sections+"\n ";
    Text += "- **SubSections**\n\t- "+SubSections+"\n ";
    ui->FileInfo->setMarkdown(Text);
//    if(CloneMode)
//        CloneDatabaseFile::NewFilePathAndId(metadata, needsSubSection);
//    else
    NewFilePathAndId();
}

void NewDatabaseFile::NewFilePathAndId()
{
    QString Fields = Selected_Field_names.values().join("|");
    QString Chapters = Selected_Chapters_names.values().join("|");
    QString Sections = Selected_Sections_names.values().join("|");
    QString FieldId = Selected_Field_ids.values().join("");
    QString ChapterId = Selected_Chapters_ids.values().join("");
    QString SectionId = Selected_Sections_ids.values().join("");
    QString Path = DataBase_Path+Fields+QDir::separator()+Chapters+QDir::separator()+Sections+QDir::separator()+FileType.FolderName+QDir::separator();
    QString prefix;// = SqlFunctions::Get_String_From_Query(QString("SELECT Prefix FROM DataBases WHERE FileName = '%1'").arg(QFileInfo(currentbase.databaseName()).baseName()),DataTex::DataTeX_Settings);
    prefix = (!prefix.isEmpty() && !prefix.isNull()) ? prefix+"-" : QString() ;
    QString fileId = prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+FileType.Id;
    QStringList ExistingFiles = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT Id FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(fileId),currentbase);
    QRegExp file_index("[0-9]{1,}");
    int fileNumber = 1;
    while(ExistingFiles.contains(fileId+QString::number(fileNumber))){
        fileNumber++;
    }
    file_index.indexIn((Mode == EditMode) ? QFileInfo(metadata->Path).baseName() : "",1);
    QString number = file_index.capturedTexts().last();
    int filecount = (Mode != EditMode) ? fileNumber : number.toInt();
    QString fileName = Path+fileId+QString::number(filecount)+".tex";
    ui->NewFileContentText->editor->setText(FileCommands::NewFileText(fileName,CurrentFileContent,currentbase));
    ui->FilePathLine->setText(Path);
    ui->FileNameLine->setText(fileId+QString::number(filecount)+".tex");
    // qDebug()<<CurrentFileContent;
}

QList<QListWidgetItem *> NewDatabaseFile::FindListItemByData(QListWidget *list,QString text)
{
    QList<QListWidgetItem *> items;
    QListWidgetItem *item = nullptr;
    int k =-1;
    for (int i = 0; i < list->count(); ++i) {
        if (list->item(i)->data(Qt::UserRole).toString() == text) {
            k = i;
            item = list->item(k);
            items.append(item);
        }
    }
    return items;
}

void NewDatabaseFile::LoadFileTypes()
{
    ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
    for(DTXFileType filetype : qAsConst(DataTex::CurrentFilesDataBase.FileTypes)){
        if(filetype.Solvable != DTXSolutionState::Solution){
            QRadioButton * button = new QRadioButton(filetype.Name,this);
            button->setProperty("Id",QVariant::fromValue(filetype));
            FileTypeGroup->addButton(button);
            ui->gridLayout_10->addWidget(button,ui->gridLayout_10->rowCount(),0);
            connect(button, &QRadioButton::toggled, this,[=](){
                FileTypeClicked();
            });
        }
    }
    ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
    ui->removeFileType->setEnabled(false);
}

void NewDatabaseFile::InitialSettings()
{
    QSettings settings;
    settings.beginGroup("NewDatabaseFile");
    QString fileType = settings.value("NewDatabaseFile_CurrentFileType").toString();//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentFileType'",DataTex::DataTeX_Settings);
    QString field = settings.value("NewDatabaseFile_CurrentField").toString();//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentField'",DataTex::DataTeX_Settings);
    QStringList chapters =
        settings.value("NewDatabaseFile_CurrentChapter").toString().split(",");//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentChapter'",DataTex::DataTeX_Settings).split(",");
    QStringList sections = settings.value("NewDatabaseFile_CurrentSection").toString().split(",");//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentSection'",DataTex::DataTeX_Settings).split(",");
    QStringList subsections = settings.value("NewDatabaseFile_ExerciseType").toString().split(",");//SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_ExerciseType'",DataTex::DataTeX_Settings).split(",");
    settings.endGroup();
    for (int i=0;i<FileTypeGroup->buttons().count();i++) {
        if(FileTypeGroup->buttons().at(i)->property("Id").value<DTXFileType>().Id == fileType){
            FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    if(!field.isEmpty()){
        ui->FieldTable->setCurrentItem(FindListItemByData(ui->FieldTable,field).at(0));
        currentField = ui->FieldTable->currentItem()->text();
    }
    if(!chapters.isEmpty()){
        for(QString chapter:chapters){
            for(QListWidgetItem * item:FindListItemByData(ui->Chapters,chapter)){
                item->setSelected(true);
                currentChapter = item->text();
                ChaptersClicked(item);
            }
        }
    }
    if(!sections.isEmpty()){
        for(QString section:sections){
            for(QListWidgetItem * item:FindListItemByData(ui->Sections,section)){
                item->setSelected(true);
                currentSection = item->text();
                SectionClicked(item);
            }
        }
    }
    if(!subsections.isEmpty()){
        for(QString subsection:subsections){
            for(QListWidgetItem * item:FindListItemByData(ui->SubSections,subsection)){
                item->setSelected(true);
                currentSubSection = item->text();
                SubSectionClicked(item);
            }
        }
    }
}

void NewDatabaseFile::SaveSettings()
{
    QSqlQuery SaveSelections;//(DataTex::DataTeX_Settings);
//    QProcess::execute("chmod",{"777",DataTex::getDataTexPath()});
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->SaveSelectionsCheckBox->isChecked())+"' WHERE Setting = 'SaveNewFileSelections'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+FileType.Id+"' WHERE Setting = 'NewDatabaseFile_CurrentFileType'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+ui->FieldTable->currentItem()->data(Qt::UserRole).toString()+"' WHERE Setting = 'NewDatabaseFile_CurrentField'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Chapters_ids.values().join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentChapter'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Sections_ids.values().join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentSection'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_SubSections_ids.values().join(",")+"' WHERE Setting = 'NewDatabaseFile_ExerciseType'");
//    QProcess::execute("chmod",{"555",DataTex::getDataTexPath()});
}

void NewDatabaseFile::hideButton(bool setHidden)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setHidden(setHidden);
}

QList<QStringList> NewDatabaseFile::GetDataFromSelectionList(QListWidget *list)
{
    QList<QStringList> data;
    for (int i = 0; i < list->selectedItems().count(); ++i) {
        data.append(list->selectedItems().at(i)->data(Qt::UserRole).toStringList());
    }
    return data;
}

void NewDatabaseFile::setDBFileInfo()
{
    QString fileName = ui->FileNameLine->text().remove(".tex");
    QString FileContent = ui->NewFileContentText->editor->toPlainText();
    metadata->Id = fileName;
    metadata->Difficulty = ui->DifficultySpin->value();
    metadata->Path = ui->FilePathLine->text()+fileName+".tex";
    metadata->Date = QDateTime::currentDateTime();
    if(FileType.Solvable == DTXSolutionState::Solvable){
        metadata->Solved = DTXSolutionState::UnSolved;
    }
    else{
        metadata->Solved = DTXSolutionState::NotSolvable;
    }
    metadata->Bibliography = QString();
    metadata->Content = FileCommands::ClearMetadataFromContent(FileContent);
    metadata->Content = FileCommands::NewFileText(metadata->Id,metadata->Content,DataTex::CurrentFilesDataBase.Database);
    metadata->Preamble.Id = ui->PreambleBox->currentData().toString();
    metadata->BuildCommand = ui->BuildBox->currentText();
    metadata->Description = ui->DescriptionLine->toPlainText();
    metadata->Tags = tagLine->GetTags().toList();
    metadata->Solutions = QStringList();
    metadata->Database.Id = currentbase.databaseName();
    metadata->DatabaseId = QFileInfo(currentbase.databaseName()).baseName();
}

DTXFile * NewDatabaseFile::getFileInfo()
{
    setDBFileInfo();
    return metadata;
}

void NewDatabaseFile::on_NextButton_clicked()
{
    setDBFileInfo();
    emit sendFileInfo(index,metadata,move::Next);
    index++;
}


void NewDatabaseFile::on_BackButton_clicked()
{
    setDBFileInfo();
    emit sendFileInfo(index,metadata,move::Back);
    index--;
    ui->BackButton->setEnabled(index==0);
}

void NewDatabaseFile::reset()
{
    FileTypeGroup->setExclusive(false);
    for(QAbstractButton *button: FileTypeGroup->buttons()) {
        button->setChecked(false);
    }
    FileTypeGroup->setExclusive(false);
    ui->FileInfo->clear();
    ui->FieldTable->clear();
    ui->Chapters->clear();
    ui->Sections->clear();
    ui->SubSections->clear();
    ui->DescriptionLine->clear();
    ui->FileNameLine->clear();
    ui->FilePathLine->clear();
}

void NewDatabaseFile::setFinishButton(bool isLast)
{
    if(isLast){
        ui->NextButton->setText(tr("Finish"));
    }
    else{
        ui->NextButton->setText(tr("Next"));
    }
}
