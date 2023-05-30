#include "newdatabasefile.h"
#include "ui_newdatabasefile.h"
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QRadioButton>


NewDatabaseFile::NewDatabaseFile(QWidget *parent, DBFileInfo *fileinfo, int mode) :
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
    qDebug()<<CurrentFileContent;
    ui->NewFileContentText->setEnabled(false);
    ui->DefinitionButton->setProperty("Name","Def");
    ui->TheoremButton->setProperty("Name","Theor");
    ui->FigureButton->setProperty("Name","Fig");
    ui->TableButton->setProperty("Name","Tab");
    ui->PropositionButton->setProperty("Name","Prop");
    ui->LemmButton->setProperty("Name","Lem");
    ui->CorollaryButton->setProperty("Name","Cor");
    ui->AxiomButton->setProperty("Name","Ax");
    ui->SectionExerciseButton->setProperty("Name","SectEx");
    ui->SectionSubjectButton->setProperty("Name","SectSub");
    ui->CombExerciseButton->setProperty("Name","CombEx");
    ui->CombSubjectButton->setProperty("Name","CombSub");
    ui->MethodButton->setProperty("Name","Method");
    ui->ExampleButton->setProperty("Name","Example");
    ui->addChapter->setProperty("Info",tr("Add new chapter for field : "));
    ui->addSection->setProperty("Info",tr("Add new section for chapter : "));
    ui->addExType->setProperty("Info",tr("Add new subsection for section : "));
    saveSelections = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SaveNewFileSelections'",DataTex::DataTeX_Settings).toInt();

    ui->FieldTable->setEnabled(false);
    ui->addField->setEnabled(false);
    ui->addChapter->setEnabled(false);
    ui->addSection->setEnabled(false);
    ui->addExType->setEnabled(false);
    ui->removeField->setEnabled(false);
    ui->removeChapter->setEnabled(false);
    ui->removeSection->setEnabled(false);
    ui->removeExType->setEnabled(false);
    currentbase = DataTex::CurrentTexFilesDataBase;
    DataBase_Path = QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator();
    if(mode==0){
        metadata = new DBFileInfo;
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
    TheoryView->show();

    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase));
    ui->horizontalLayout_13->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->SaveSelectionsCheckBox->setEnabled(false);
    ui->NewFileContentText->toolBar->Save->setVisible(false);
    QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    ui->BuildBox->addItems(DataTex::LatexCommands.keys());
    for (int i=0;i<PreambleIds.count();i++) {
        ui->PreambleBox->addItem(PreambleNames.at(i),QVariant(PreambleIds.at(i)));
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
    qDebug()<<(mode==CloneModeOnlyContent || mode==CloneModeContentAndMetadata);
    if(mode==CloneModeOnlyContent || mode==CloneModeContentAndMetadata){
        CloneModeIsEnabled(mode==CloneModeContentAndMetadata);
    }
    ui->splitter_2->setSizes(QList<int>({150, 300,350}));

    ui->UseExerciseType->setEnabled(false);
    connect(ui->UseExerciseType,&QCheckBox::toggled,this,[=](bool checked){
        ui->SubSections->setEnabled(checked);
        ui->addExType->setEnabled(checked);
        ui->removeExType->setEnabled(checked);
        needsSubSection = checked;
        if(checked){
            SectionClicked();
        }
        else{
            ui->SubSections->clear();
        }
        updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                            arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                                ui->Chapters->currentItem()->data(Qt::UserRole).toString(),
                                ui->Sections->currentItem()->data(Qt::UserRole).toString(),"",FileType));
    });

    connect(ui->FieldTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->FieldTable->selectionModel()->hasSelection()){
            FieldsClicked();
        }
    });
    connect(ui->FieldTable, &QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        FieldsClicked();
        currentField = item->text();
    });
    connect(ui->Chapters,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        ChaptersClicked();
        currentChapter = item->text();
    });
    connect(ui->Chapters->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->Chapters->selectionModel()->hasSelection()){
            ChaptersClicked();
        }
    });

    connect(ui->Sections,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        SectionClicked();
        currentSection= item->text();
    });
    connect(ui->Sections->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->Sections->selectionModel()->hasSelection()){
            SectionClicked();
        }
    });

    connect(ui->SubSections,&QListWidget::itemClicked,this,[=](QListWidgetItem * item){
        SubSectionClicked();
        currentSubSection = item->text();
    });
    connect(ui->SubSections->selectionModel(), &QItemSelectionModel::selectionChanged,this,[=](){
        if(ui->SubSections->selectionModel()->hasSelection()){
            SubSectionClicked();
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
    QString filetype = metadata->FileType.at(0);
    for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
        if(ui->FileTypeGroup->buttons().at(i)->property("Name") == filetype){
            ui->FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
    FieldsClicked();
    for(int i=0;i<ImportedChaptersList.count();i++){
        foreach(QListWidgetItem * item,ui->Chapters->findItems(ImportedChaptersList[i],Qt::MatchExactly)){
            item->setSelected(true);
            ChaptersClicked();
        }
    }
    for(int i=0;i<ImportedSectionList.count();i++){
        foreach(QListWidgetItem * item,ui->Sections->findItems(ImportedSectionList[i],Qt::MatchExactly)){
            item->setSelected(true);
            SectionClicked();
        }
    }
    for(int i=0;i<ImportedSubSectionList.count();i++){
        foreach(QListWidgetItem * item,ui->SubSections->findItems(ImportedSubSectionList[i],Qt::MatchExactly)){
            item->setSelected(true);
            SubSectionClicked();
        }
    }
    ui->DifficultySpin->setValue(metadata->Difficulty);
    int preamble = ui->PreambleBox->findData(metadata->Preamble);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
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
    qDebug()<<cloneMode;
    if(cloneMode == CloneModeContentAndMetadata){
        QString filetype = metadata->FileType.at(0);
        for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
            if(ui->FileTypeGroup->buttons().at(i)->property("Name") == filetype){
                ui->FileTypeGroup->buttons().at(i)->setChecked(true);
            }
        }
        ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata->Field[1],Qt::MatchExactly).at(0));
        metadata->Field[0] = ui->FieldTable->currentItem()->data(Qt::UserRole).toString();
        FieldsClicked();
        for(int i=0;i<ImportedChaptersList.count();i++){
            foreach(QListWidgetItem * item,ui->Chapters->findItems(ImportedChaptersList[i],Qt::MatchExactly)){
                if(metadata->Chapters[i][1]==item->text()){
                    metadata->Chapters[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->Chapters[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    qDebug()<<metadata->Chapters[i];
                }
                item->setSelected(true);
                ChaptersClicked();
            }
        }
        qDebug()<<"Sections from dtex   "<<metadata->Sections;
        for(int i=0;i<ImportedSectionList.count();i++){
            foreach(QListWidgetItem * item,ui->Sections->findItems(ImportedSectionList[i],Qt::MatchExactly)){
                if(metadata->Sections[i][1]==item->text()){
                    metadata->Sections[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->Sections[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    qDebug()<<"Sections from dtex   "<<metadata->Sections[i];
                }
                item->setSelected(true);
                SectionClicked();
            }
        }
        for(int i=0;i<ImportedSubSectionList.count();i++){
            foreach(QListWidgetItem * item,ui->SubSections->findItems(ImportedSubSectionList[i],Qt::MatchExactly)){
                if(metadata->SubSections[i][1]==item->text()){
                    metadata->SubSections[i].replace(0,item->data(Qt::UserRole).toStringList()[0]);
                    metadata->SubSections[i].replace(2,item->data(Qt::UserRole).toStringList()[1]);
                    qDebug()<<metadata->SubSections[i];
                }
                item->setSelected(true);
                SubSectionClicked();
            }
        }
        ui->DifficultySpin->setValue(metadata->Difficulty);
        int preamble = ui->PreambleBox->findData(metadata->Preamble);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
        if ( preamble != -1 ) {
            ui->PreambleBox->setCurrentIndex(preamble);
        }
        int build = ui->BuildBox->findText(metadata->BuildCommand);
        if ( build != -1 ) {
            ui->BuildBox->setCurrentIndex(build);
        }
    }
    ui->DescriptionLine->setText(metadata->Description);
    metadata->Path = FileCommands::NewFilePathAndId(metadata,needsSubSection);
    metadata->Id = QFileInfo(metadata->Path).baseName();
    metadata->Content = FileCommands::ClearMetadataFromContent(metadata->Content);
    ui->NewFileContentText->editor->setText(FileCommands::NewFileText(metadata->Id,metadata->Content,DataTex::CurrentTexFilesDataBase));
}

void NewDatabaseFile::CloneModeIsEnabled(DBFileInfo * fileInfo,bool cloneMetadata)
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
    foreach(QString tag,tags){
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
            ui->Chapters->addItem(Line[0]);
            ui->Chapters->item(ui->Chapters->count()-1)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->FieldTable->currentItem()->data(Qt::UserRole).toString()}));
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
    FieldsClicked();
}


void NewDatabaseFile::on_removeChapter_clicked()
{
    QSqlQuery RemoveChapter(currentbase);
    RemoveChapter.exec("PRAGMA foreign_keys = ON");
    RemoveChapter.exec(QString("DELETE FROM Chapters WHERE Id = \"%1\" AND Field = \"%2\"")
                     .arg(ui->Chapters->currentItem()->data(Qt::UserRole).toString()
                          ,ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    ui->Chapters->takeItem(ui->Chapters->currentRow());
    ChaptersClicked();
}


void NewDatabaseFile::on_addSection_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addSection->property("Info").toString()+currentChapter);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection(currentbase);
        AddSection.prepare(QString("INSERT INTO Sections (Id,Name,Chapter) VALUES(\"%1\",\"%2\",\"%3\")")
                        .arg(Line[1],Line[0],ui->Chapters->currentItem()->data(Qt::UserRole).toStringList()[0]));
        if(AddSection.exec()){
            ui->Sections->addItem(Line[0]);
            ui->Sections->item(ui->Sections->count()-1)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->Chapters->currentItem()->data(Qt::UserRole).toStringList()[0]}));
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
    SectionClicked();
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
            ui->SubSections->addItem(Line[0]);
            ui->SubSections->item(ui->SubSections->count()-1)->setData(Qt::UserRole,QVariant::fromValue<QStringList>({Line[1],ui->Sections->currentItem()->data(Qt::UserRole).toStringList()[0]}));
//            ui->SubSections->setCurrentRow(ui->SubSections->count()-1);
//            SubSectionClicked();
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
        SubSectionClicked();
    }
}

void NewDatabaseFile::on_addFileType_clicked()
{
    NewFileType * newFile = new NewFileType(this);
    connect(newFile,&NewFileType::filedata,this,[=](QStringList data){
        QRadioButton * newButton = new QRadioButton(data[1],this);
//        ui->gridLayout_10->removeItem(ui->gridLayout_8);
        ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
        newButton->setProperty("Name",data[0]);
        CustomFileTypesList.append(newButton);
        ui->FileTypeGroup->addButton(newButton);
        connect(newButton, &QRadioButton::toggled, this,
                [=](){
            FileTypeClicked();
        });
        ui->gridLayout_10->addWidget(newButton,ui->gridLayout_10->rowCount(),0);
//        ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
        ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
        QSqlQuery NewFileType(currentbase);
        NewFileType.prepare(QString("INSERT OR IGNORE INTO FileTypes (Id,FileType,FolderName,Solvable) VALUES(:id,:name,:folder,:sol)"));
        NewFileType.bindValue(":id",data[0]);
        NewFileType.bindValue(":name",data[1]);
        NewFileType.bindValue(":folder",data[2]);
        NewFileType.bindValue(":sol",data[3]);
        NewFileType.exec();
        if(data[3]=="1"){
            NewFileType.prepare(QString("INSERT OR IGNORE INTO FileTypes (Id,FileType,FolderName,Solvable,BelongsTo) VALUES(:id,:name,:folder,-1,:fid)"));
            NewFileType.bindValue(":id",data[0]+"-Sol");
            NewFileType.bindValue(":name",data[1]+tr(" - Solution"));
            NewFileType.bindValue(":folder",data[2]+tr(" - Solutions"));
            NewFileType.bindValue(":fid",data[0]);
            NewFileType.exec();
        }
    });
    newFile->show();
    newFile->activateWindow();
}

void NewDatabaseFile::on_removeFileType_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete exercise type",tr("The filetype type %1 will be deleted!\nDo you wish to proceed?").arg(FileType),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
            QSqlQuery RemoveExerciseType(currentbase);
            RemoveExerciseType.exec(QString("DELETE FROM FileTypes WHERE Id = \"%1\"").arg(FileType));
    }
}

void NewDatabaseFile::FileTypeClicked()
{
    QRadioButton * button = qobject_cast<QRadioButton*>(sender());
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    FileType = button->property("Name").toString();
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
    }

    ui->FilePathLine->clear();
    ui->FileNameLine->clear();
    CurrentFileContent = FileCommands::ClearMetadataFromContent(ui->NewFileContentText->editor->toPlainText());
    ui->NewFileContentText->setEnabled(false);

    QSqlQuery Solvable(currentbase);
    Solvable.exec(QString("SELECT Solvable FROM FileTypes WHERE Id = \"%1\"").arg(FileType));
    while(Solvable.next()){NeedsSolution = Solvable.value(0).toInt();}
    if(NeedsSolution){
        ui->DifficultySpin->setEnabled(true);
        ui->DifficultySpin->setMinimum(1);
        ui->DifficultySpin->setValue(1);
    }
    else{
        ui->DifficultySpin->setMinimum(0);
        ui->DifficultySpin->setValue(0);
        ui->DifficultySpin->setEnabled(false);
    }
    ui->removeFileType->setEnabled(CustomFileTypesList.contains(button));
    ui->UseExerciseType->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    needsSubSection = FileType == "SectEx" || FileType == "SectSub" || FileType =="CombEx"
            || FileType == "CombSub" || ui->UseExerciseType->isChecked();
    metadata->FileType=SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM FileTypes WHERE Id = '%1'").arg(FileType),currentbase);
}

void NewDatabaseFile::FieldsClicked()
{
    Selected_Field_names.clear();
    Selected_Field_ids.clear();
    ui->FilterChapters->setEnabled(true);
    foreach(QListWidgetItem * item,ui->FieldTable->selectedItems()) {
        Selected_Field_ids.append(item->data(Qt::UserRole).toStringList()[0]);
        Selected_Field_names.append(item->text());
    }
    if(ui->FieldTable->selectionModel()->hasSelection()){
        currentField = ui->FieldTable->selectionModel()->currentIndex().data(Qt::DisplayRole).toStringList()[0];
    }
    QStringList Chapter_Names;
    ChapterList = SqlFunctions::GetRecordList(QString("SELECT * FROM Chapters WHERE Field IN (\"%1\")").arg(Selected_Field_ids.join("\",\"")),currentbase);
    qDebug()<<QString("SELECT Name,Id,Field FROM Chapters WHERE Field IN (\"%1\")").arg(Selected_Field_ids.join("\",\""));
    ui->Chapters->clear();
    ui->Sections->clear();
    ui->SubSections->clear();
    for (int item=0;item<ChapterList.count();item++ ) {
        ui->Chapters->addItem(ChapterList.at(item)[1]);
        ui->Chapters->item(item)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(ChapterList[item]));
        Chapter_Names.append(ChapterList.at(item)[1]);
    }
    Selected_Chapters_ids.clear();
    foreach(QListWidgetItem * item,ui->Chapters->selectedItems()) {
        Selected_Chapters_ids.append(item->data(Qt::UserRole).toStringList()[1]);
        Selected_Chapters_names.append(item->text());
    }
    QCompleter *completer = new QCompleter(Chapter_Names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->FilterChapters->setCompleter(completer);
    DataTex::StretchColumnsToWidth(ui->ExerciseFileList);
    ui->ExerciseFileList->setColumnHidden(2,true);
    ui->addChapter->setEnabled(true);
    ui->removeChapter->setEnabled(true);
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.arg(Selected_Field_ids.join("|"),"","","",FileType));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->NewFileContentText->setEnabled(false);
    metadata->Field = GetDataFromSelectionList(ui->FieldTable).at(0);
    qDebug()<<"Field clicked";
}

void NewDatabaseFile::ChaptersClicked()
{
    Selected_Chapters_names.clear();
    Selected_Chapters_ids.clear();
    ui->FilterSections->setEnabled(true);
    foreach(QListWidgetItem * item,ui->Chapters->selectedItems()) {
        Selected_Chapters_ids.append(item->data(Qt::UserRole).toStringList()[0]);
        Selected_Chapters_names.append(item->text());
    }

    if(ui->Chapters->selectionModel()->hasSelection()){
        currentChapter = ui->Chapters->selectionModel()->currentIndex().data(Qt::DisplayRole).toString();
    }

    Sections_Names.clear();
    Sections_ids.clear();
    SectionList = SqlFunctions::GetRecordList(QString("SELECT * FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.join("\",\"")),currentbase);
    qDebug()<<QString("SELECT Name,Id,Chapter FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.join("\",\""));
    ui->Sections->clear();
    for (int item=0;item<SectionList.count();item++ ) {
        ui->Sections->addItem(SectionList.at(item)[1]);
        Sections_Names.append(SectionList.at(item)[1]);
        ui->Sections->item(item)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(SectionList[item]));
    }
    QCompleter *completer = new QCompleter(Sections_Names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->FilterSections->setCompleter(completer);
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                    arg(Selected_Field_ids.join("|"),
                        Selected_Chapters_ids.join("|"),"","",FileType));
    ui->addSection->setEnabled(true);
    ui->removeSection->setEnabled(true);
    ui->FileInfo->clear();
    ui->NewFileContentText->setEnabled(false);
    metadata->Chapters = GetDataFromSelectionList(ui->Chapters);
    qDebug()<<metadata->Chapters;
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
}

void NewDatabaseFile::SectionClicked()
{
    Selected_Sections_names.clear();
    Selected_Sections_ids.clear();
    foreach(QListWidgetItem * item,ui->Sections->selectedItems()) {
        Selected_Sections_ids.append(item->data(Qt::UserRole).toStringList()[0]);
        Selected_Sections_names.append(item->text());
    }

    if(ui->Sections->selectionModel()->hasSelection()){
        currentSection = ui->Sections->selectionModel()->currentIndex().data(Qt::DisplayRole).toString();
    }

    if(needsSubSection){
        ui->FilterSubSections->setEnabled(true);
        SubSections_Names.clear();
        SubSections_ids.clear();
        SubSectionList = SqlFunctions::GetRecordList(QString("SELECT DISTINCT se.Exercise_Id,e.Name,"
                                                             "replace(group_concat(se.Section_Id),',','|') "
                                                             "FROM Sections_Exercises se "
                                                             "JOIN Exercise_Types e ON e.Id=se.Exercise_Id "
                                                             "WHERE se.Section_Id IN (\"%1\")"
                                                             "GROUP BY 1").arg(Selected_Sections_ids.join("\",\"")),currentbase);
        qDebug()<<QString("SELECT DISTINCT e.Name,se.Exercise_Id,se.Section_Id FROM Sections_Exercises se JOIN Exercise_Types e ON e.Id=se.Exercise_Id WHERE se.Section_Id IN (\"%1\")").arg(Selected_Sections_ids.join("\",\""));
        ui->SubSections->clear();
        for (int item=0;item<SubSectionList.count();item++) {
            ui->SubSections->addItem(SubSectionList.at(item)[1]);
            SubSections_Names.append(SubSectionList.at(item)[1]);
            ui->SubSections->item(item)->setData(Qt::UserRole,QVariant::fromValue<QStringList>(SubSectionList.at(item)));
        }
        ui->addExType->setEnabled(true);
        ui->removeExType->setEnabled(true);
        QCompleter *completer = new QCompleter(SubSections_Names, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->FilterSubSections->setCompleter(completer);
    }
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                    arg(Selected_Field_ids.join("|"),
                        Selected_Chapters_ids.join("|"),Selected_Sections_ids.join("|"),"",FileType));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!needsSubSection);
    ui->SaveSelectionsCheckBox->setEnabled(!needsSubSection);
    tagLine->setEnabled(!needsSubSection);
    foreach(QAbstractButton * button,CustomFileTypesList){
        ui->UseExerciseType->setEnabled(button->isChecked());
    }
    ui->NewFileContentText->setEnabled(true);
    UpdateFileInfo();
    metadata->Sections = GetDataFromSelectionList(ui->Sections);
}

void NewDatabaseFile::SubSectionClicked()
{
    Selected_SubSections_names.clear();
    Selected_SubSections_ids.clear();
    foreach(QListWidgetItem * item,ui->SubSections->selectedItems()) {
        Selected_SubSections_ids.append(item->data(Qt::UserRole).toStringList()[0]);
        Selected_SubSections_names.append(item->text());
    }
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(needsSubSection);
    ui->SaveSelectionsCheckBox->setEnabled(needsSubSection);
    tagLine->setEnabled(needsSubSection);
    foreach(QAbstractButton * button,CustomFileTypesList){
        ui->UseExerciseType->setEnabled(button->isChecked());
    }
    UpdateFileInfo();
    metadata->SubSections = GetDataFromSelectionList(ui->SubSections);
}

void NewDatabaseFile::UpdateFileInfo()
{
    CurrentFileContent = FileCommands::ClearMetadataFromContent(ui->NewFileContentText->editor->toPlainText());
    QString Fields =  Selected_Field_names.join(" , ");
    QString Chapters = Selected_Chapters_names.join(" , ");
    QString Sections = Selected_Sections_names.join(" , ");
    QString SubSections = Selected_SubSections_names.join(" , ");
    ui->FileInfo->setText("#### File Info<br />---------------<br />**Fields**<br />"+Fields+"<br />**Chapters**<br />"+Chapters+"<br />**Sections**<br />"+Sections+
                          "<br />**SubSections**<br />"+SubSections);
//    if(CloneMode)
//        CloneDatabaseFile::NewFilePathAndId(metadata, needsSubSection);
//    else
        NewFilePathAndId();
}

void NewDatabaseFile::NewFilePathAndId()
{
    QString folderName = SqlFunctions::Get_StringList_From_Query(
                SqlFunctions::FolderName.arg(FileType),currentbase).at(0);
    QString Fields = Selected_Field_names.join("|");
    QString Chapters = Selected_Chapters_names.join("|");
    QString Sections = Selected_Sections_names.join("|");
    QString FieldId = Selected_Field_ids.join("");
    QString ChapterId = Selected_Chapters_ids.join("");
    QString SectionId = Selected_Sections_ids.join("");
    QString ExTypeId = Selected_SubSections_ids.join("");
    QString Path = DataBase_Path+Fields+QDir::separator()+Chapters+QDir::separator()+Sections+QDir::separator()+folderName+QDir::separator();
    QString prefix = SqlFunctions::Get_String_From_Query(QString("SELECT Prefix FROM DataBases WHERE FileName = '%1'").arg(QFileInfo(currentbase.databaseName()).baseName()),DataTex::DataTeX_Settings);
    prefix = (!prefix.isEmpty() && !prefix.isNull()) ? prefix+"-" : QString() ;
    QString fileId = (needsSubSection) ? prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+ExTypeId+"-"+FileType
                                       : prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+FileType;
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
    qDebug()<<CurrentFileContent;
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
    foreach(QAbstractButton *button, ui->FileTypeGroup->buttons()) {
        if(CustomFileTypesList.contains(button)){
            ui->gridLayout_10->removeWidget(button);
            ui->FileTypeGroup->removeButton(button);
            delete button;
        }
    }
    CustomFileTypesList.clear();
    QSqlQuery FileTypes(currentbase);
    FileTypes.exec("SELECT FileType,Id,FolderName FROM FileTypes WHERE Solvable >-1");
    int line = -1;
//    ui->gridLayout_10->removeItem(ui->gridLayout_8);
    ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
    while(FileTypes.next()){
        line++;
        if(line>13){
            QRadioButton * button = new QRadioButton(FileTypes.value(0).toString(),this);
            button->setProperty("Name",FileTypes.value(1).toString());
            CustomFileTypesList.append(button);
            ui->FileTypeGroup->addButton(button);
            ui->gridLayout_10->addWidget(button,ui->gridLayout_10->rowCount(),0);
        }
    }
    for(int i=0;i<ui->FileTypeGroup->buttons().count();i++){
        connect(ui->FileTypeGroup->buttons().at(i), &QRadioButton::toggled, this,[=](){
                    FileTypeClicked();
                });
    }
//    ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
    ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
    ui->removeFileType->setEnabled(false);
}

void NewDatabaseFile::InitialSettings()
{
    QString fileType = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentFileType'",DataTex::DataTeX_Settings);
    QString field = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentField'",DataTex::DataTeX_Settings);
    QStringList chapters =
        SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentChapter'",DataTex::DataTeX_Settings).split(",");
    QStringList sections = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_CurrentSection'",DataTex::DataTeX_Settings).split(",");
    QStringList subsections = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'NewDatabaseFile_ExerciseType'",DataTex::DataTeX_Settings).split(",");
    for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
        if(ui->FileTypeGroup->buttons().at(i)->property("Name") == fileType){
            ui->FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    if(!field.isEmpty()){
        ui->FieldTable->setCurrentItem(FindListItemByData(ui->FieldTable,field).at(0));
        currentField = ui->FieldTable->currentItem()->text();
    }
    if(!chapters.isEmpty()){
        foreach(QString chapter,chapters){
            foreach(QListWidgetItem * item,FindListItemByData(ui->Chapters,chapter)){
                item->setSelected(true);
                currentChapter = item->text();
                ChaptersClicked();
            }
        }
    }
    if(!sections.isEmpty()){
        foreach(QString section,sections){
            foreach(QListWidgetItem * item,FindListItemByData(ui->Sections,section)){
                item->setSelected(true);
                currentSection = item->text();
                SectionClicked();
            }
        }
    }
    if(!subsections.isEmpty()){
        foreach(QString subsection,subsections){
            foreach(QListWidgetItem * item,FindListItemByData(ui->SubSections,subsection)){
                item->setSelected(true);
                currentSubSection = item->text();
                SubSectionClicked();
            }
        }
    }
}

void NewDatabaseFile::SaveSettings()
{
    QSqlQuery SaveSelections(DataTex::DataTeX_Settings);
//    QProcess::execute("chmod",{"777",DataTex::getDataTexPath()});
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->SaveSelectionsCheckBox->isChecked())+"' WHERE Setting = 'SaveNewFileSelections'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+FileType+"' WHERE Setting = 'NewDatabaseFile_CurrentFileType'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+ui->FieldTable->currentItem()->data(Qt::UserRole).toString()+"' WHERE Setting = 'NewDatabaseFile_CurrentField'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Chapters_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentChapter'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Sections_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentSection'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_SubSections_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_ExerciseType'");
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
    QString Path = ui->FilePathLine->text();
    QString FileContent = ui->NewFileContentText->editor->toPlainText();


    metadata->Id = fileName;
    metadata->Difficulty = ui->DifficultySpin->value();
    metadata->Path = Path+fileName+".tex";
    metadata->Date = QDateTime::currentDateTime();
    metadata->Solved = false;
    metadata->Bibliography = QString();
    metadata->Content = FileCommands::ClearMetadataFromContent(FileContent);
    metadata->Content = FileCommands::NewFileText(metadata->Id,metadata->Content,DataTex::CurrentTexFilesDataBase);
    metadata->Preamble = ui->PreambleBox->currentData().toString();
    metadata->BuildCommand = ui->BuildBox->currentText();
    metadata->Description = ui->DescriptionLine->text();
    metadata->Tags = tagLine->GetTags().toList();
    metadata->Solutions = QStringList();
    metadata->Database = currentbase;
    metadata->DatabaseName = currentbase.databaseName();
}

DBFileInfo * NewDatabaseFile::getFileInfo()
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
    ui->FileTypeGroup->setExclusive(false);
    foreach(QAbstractButton *button, ui->FileTypeGroup->buttons()) {
        button->setChecked(false);
    }
    ui->FileTypeGroup->setExclusive(false);
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
