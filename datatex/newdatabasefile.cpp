#include "newdatabasefile.h"
#include "ui_newdatabasefile.h"
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QRadioButton>
#include "adddatabasefield.h"
#include "sqlfunctions.h"
#include "datatex.h"
#include "newfiletype.h"
#include "csvfunctions.h"


NewDatabaseFile::NewDatabaseFile(QWidget *parent, QHash<QString, QString> meta,QString fileContent,
                                 QStringList chapters, QStringList sections, QStringList subsections,
                                 bool editMode, QString fileName, bool cloneMode, bool insertMode) :
    QDialog(parent),
    ui(new Ui::NewDatabaseFile)
{
    ui->setupUi(this);
    EditMode = editMode;
    CloneMode = cloneMode;
    InsertMode = insertMode;
    FileName = fileName;
    ImportedFileContent = ClearMetadataFromContent(fileContent);
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
    ui->DefinitionButton->setEnabled(!CloneMode);
    ui->TheoremButton->setEnabled(!CloneMode);
    ui->FigureButton->setEnabled(!CloneMode);
    ui->TableButton->setEnabled(!CloneMode);
    ui->PropositionButton->setEnabled(!CloneMode);
    ui->LemmButton->setEnabled(!CloneMode);
    ui->CorollaryButton->setEnabled(!CloneMode);
    ui->AxiomButton->setEnabled(!CloneMode);
    ui->SectionExerciseButton->setEnabled(!CloneMode);
    ui->SectionSubjectButton->setEnabled(!CloneMode);
    ui->CombExerciseButton->setEnabled(!CloneMode);
    ui->CombSubjectButton->setEnabled(!CloneMode);
    ui->MethodButton->setEnabled(!CloneMode);
    ui->ExampleButton->setEnabled(!CloneMode);
    saveSelections = SqlFunctions::Get_String_From_Query("SELECT Value FROM Initial_Settings WHERE Setting = 'SaveNewFileSelections'",DataTex::DataTeX_Settings).toInt();

    qDebug()<<saveSelections;

    ui->DatabaseCombo->addItem(tr("Select a database..."));
    for (int i=0;i<DataTex::GlobalFilesDatabaseList.count();i++) {
        QSqlDatabase database = DataTex::GlobalFilesDatabaseList.values().at(i);
        if(database.databaseName()!=DataTex::CurrentDataBasePath){
            ui->DatabaseCombo->addItem(DataTex::GlobalFilesDatabaseListNames[QFileInfo(database.databaseName()).baseName()],database.databaseName());
        }
    }
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
    metadata = meta;
//    ImportedFileContent = (editMode || cloneMode) ? ClearMetadataFromContent(metadata["FileContent"]) : "";
    ImportedChaptersList = chapters;
    ImportedSectionList = sections;
    ImportedSubSectionList = subsections;
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_Names.append(fields.value(0).toString());
        Field_ids.append(fields.value(1).toString());
    }

    LoadFileTypes();

    TheoryView = new PdfViewer(this);
    ui->verticalLayout_5->addWidget(TheoryView);
    ui->splitter_2->setSizes(QList<int>({1,1, 400}));
    TheoryView->show();

    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase));
    ui->horizontalLayout_13->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->SaveSelectionsCheckBox->setEnabled(false);
    QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    ui->BuildBox->addItems(DataTex::LatexCommands.keys());
    for (int i=0;i<PreambleIds.count();i++) {
        ui->PreambleBox->addItem(PreambleNames.at(i),QVariant(PreambleIds.at(i)));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
//    ui->SingleSection->setEnabled(false);
//    ui->MultiSection->setEnabled(false);
    connect(ui->NewFileContentText,&QTextEdit::textChanged,this,[=](){
        if(!ui->BuildBox->isEnabled()){
            ui->BuildBox->setEnabled(true);
            ui->PreambleBox->setEnabled(true);
        }
    });
    ui->DatabaseCombo->setVisible(CloneMode);
    ui->DatabaseLabel->setVisible(CloneMode);
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
    if(EditMode){
        EditModeIsEnabled();
    }
//    if(CloneMode){
//        on_DatabaseCombo_activated();
//    }
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

    connect(ui->FieldTable->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        FieldsClicked();
    });
    connect(ui->FieldTable, &QListWidget::itemClicked,this,[&](){
        FieldsClicked();
    });
    connect(ui->Chapters,&QListWidget::itemClicked,this,[&](){
        ChaptersClicked();
    });
    connect(ui->Chapters->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ChaptersClicked();
    });
    connect(ui->Sections,&QListWidget::itemClicked,this,[&](){
        SectionClicked();
    });
    connect(ui->SubSections,&QListWidget::itemClicked,this,[&](){
        SubSectionClicked();
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
}

NewDatabaseFile::~NewDatabaseFile()
{
    delete ui;
}

void NewDatabaseFile::EditModeIsEnabled()
{
    QString filetype = SqlFunctions::Get_StringList_From_Query(QString("SELECT Id FROM FileTypes WHERE FileType = \"%1\"").arg(metadata["FileType"]),currentbase)[0];
    for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
        if(ui->FileTypeGroup->buttons().at(i)->property("Name") == filetype){
            ui->FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata["Field"],Qt::MatchExactly).at(0));
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
    int diff = metadata["Difficulty"].toInt();
    ui->DifficultySpin->setValue(diff);
    int preamble = ui->PreambleBox->findData(metadata["Preamble"]);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
    if ( preamble != -1 ) {
        ui->PreambleBox->setCurrentIndex(1);
    }
    int build = ui->BuildBox->findText(metadata["BuildCommand"]);
    if ( build != -1 ) {
        ui->BuildBox->setCurrentIndex(build);
    }
    ui->DescriptionLine->setText(metadata["FileDescription"]);
    ui->NewFileContentText->setText(DataTex::NewFileText(FileName,ImportedFileContent));
    ui->FilePathLine->setText(QFileInfo(metadata["Path"]).absolutePath()+QDir::separator());
    ui->FileNameLine->setText(QFileInfo(metadata["Path"]).fileName());
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

void NewDatabaseFile::loadImageFile(QString exoFile, PdfViewer *view)
{
    if (exoFile.isEmpty()) {
        return;}
    QString pdfFile = exoFile.replace(".tex",".pdf");
    if(QFileInfo::exists(pdfFile)){
        view->setCurrentDocument(pdfFile);
    }
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
        DataTex::loadImageFile(outName,TheoryView);
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
        DataTex::CreateTexFile(FilePath,0,"");
//        DataTex::BuildDocument();
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
        ui->NewFileContentText->clear();
        event->accept();
    }
}

void NewDatabaseFile::on_buttonBox_accepted()
{
    QString fileName = ui->FileNameLine->text().remove(".tex");
    QString Path = ui->FilePathLine->text();
    QDir dir(Path);
    if (!dir.exists())
        dir.mkpath(".");
    QString Field_Id = Selected_Field_ids.at(0);
    QString Solved = (NeedsSolution) ? tr("NO") : "-";
    QString FileContent = ui->NewFileContentText->toPlainText();
    mapIdsNames.insert("Id",fileName);
    mapIdsNames.insert("FileType",FileType);
    mapIdsNames.insert("Field",Field_Id);
    mapIdsNames.insert("Difficulty",QString::number(ui->DifficultySpin->value()));
    mapIdsNames.insert("Path",ui->FilePathLine->text()+ui->FileNameLine->text());
    QString filePath = mapIdsNames["Path"];
    mapIdsNames.insert("Date",QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm"));
    mapIdsNames.insert("Solved_Prooved",Solved);
    mapIdsNames.insert("FileContent",FileContent);
    QString Content = FileContent;
    QTextStream contentline(&Content);
    QStringList Line;
    while(!contentline.atEnd()){
        Line.append(contentline.readLine());
    }
    Content=Line.join("\\qt_endl");
    mapIdsNames.insert("Preamble",ui->PreambleBox->currentData().toString());
    mapIdsNames.insert("BuildCommand",ui->BuildBox->currentText());
    mapIdsNames.insert("FileDescription",ui->DescriptionLine->text());
    mapIdsNames.insert("MultiSection",QString::number(ui->stackedWidget->currentIndex()));
    if(ui->FilePathLine->text().isEmpty() || ui-> FileNameLine->text().isEmpty()){
        QMessageBox::warning(this,tr("Error"),tr("No file created yet."),QMessageBox::Ok);
        return;
    }
    //Latex file Metadata - Write new entry to database ---
    QSqlQuery writeExercise(currentbase);
    if(!EditMode || InsertMode){
        writeExercise.exec("INSERT INTO Database_Files (\""+mapIdsNames.keys().join("\",\"")+"\") VALUES (\""+mapIdsNames.values().join("\",\"")+"\")");
        foreach(QString Chapter,Selected_Chapters_ids){
            writeExercise.exec("INSERT INTO Chapters_per_File (File_Id,Chapter_Id) VALUES (\""+fileName+"\",\""+Chapter+"\")");
        }
        foreach(QString Section,Selected_Sections_ids){
            writeExercise.exec("INSERT INTO Sections_per_File (File_Id,Section_Id) VALUES (\""+fileName+"\",\""+Section+"\")");
        }
        foreach(QString SubSection,Selected_SubSections_ids){
            writeExercise.exec("INSERT INTO ExerciseTypes_per_File (File_Id,ExerciseType_Id) VALUES (\""+fileName+"\",\""+SubSection+"\")");
        }
    }
    else if(EditMode && !InsertMode){
        QStringList list;
        QString query ="UPDATE Database_Files SET ";
        list.clear();
        for (int i=0;i<mapIdsNames.keys().count();i++) {
            list.append("\""+mapIdsNames.keys()[i]+"\""+"="+"\""+mapIdsNames.values()[i]+"\"");
        }
        query += list.join(",")+" WHERE Id=\""+mapIdsNames["Id"]+"\"";
        writeExercise.exec(query);

        for(int i=0;i<Selected_Chapters_ids.count();i++){
            writeExercise.exec("UPDATE Chapters_per_File SET Chapter_Id = "+Selected_Chapters_ids[i]+",File_Id = "+fileName+" "
                               "WHERE (File_Id = "+FileName+" AND Chapter_Id = "+ImportedChaptersList[i]+")");
        }
        for(int i=0;i<Selected_Sections_ids.count();i++){
            writeExercise.exec("UPDATE Sections_per_File SET Section_Id = "+Selected_Sections_ids[i]+",File_Id = "+fileName+" "
                               "WHERE (File_Id = "+FileName+" AND Section_Id = "+ImportedSectionList[i]+")");
        }
        for(int i=0;i<Selected_SubSections_ids.count();i++){
            writeExercise.exec("UPDATE ExerciseTypes_per_File SET Chapter_Id = "+Selected_SubSections_ids[i]+",File_Id = "+fileName+" "
                               "WHERE (File_Id = "+FileName+" AND ExerciseType_Id = "+ImportedSubSectionList[i]+")");
        }
    }
    QSqlQuery insertTag(currentbase);
    tags = tagLine->GetTags();
    foreach(QString tag,tags){
        if(!tag.isEmpty()){
            insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
            insertTag.exec("INSERT OR IGNORE INTO Tags_per_File (Tag_Id,File_Id) VALUES (\""+tag+"\",\""+QFileInfo(filePath).baseName()+"\")");
        }
    }

    if(CloneMode){
        emit acceptClone(ui->DatabaseCombo->currentData().toString(),filePath,FileContent);
    }
    else {
        emit acceptSignal(filePath,/*mapIdsNames,SectionList,*/FileContent);
    }

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
            ui->FieldTable->setCurrentRow(rows);
            FieldsClicked();
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
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addChapter->property("Info").toString()+ui->FieldTable->currentItem()->text());
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddChapter(currentbase);
        AddChapter.prepare(QString("INSERT INTO Chapters (Id,Name,Field) VALUES(\"%1\",\"%2\",\"%3\")")
                      .arg(Line[1],Line[0],ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));

        if(AddChapter.exec()){
            ui->Chapters->addItem(Line[0]);
            ui->Chapters->item(ui->Chapters->count()-1)->setData(Qt::UserRole,Line[1]);
            ui->Chapters->setCurrentRow(ui->Chapters->count()-1);
            ChaptersClicked();
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
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addSection->property("Info").toString()+ui->Chapters->currentItem()->text());
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection(currentbase);
        AddSection.prepare(QString("INSERT INTO Sections (Id,Name,Field,Chapter) VALUES(\"%1\",\"%2\",\"%3\",\"%4\")")
                        .arg(Line[1],Line[0],ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                ui->Chapters->currentItem()->data(Qt::UserRole).toString()));
        if(AddSection.exec()){
            ui->Sections->addItem(Line[0]);
            ui->Sections->item(ui->Sections->count()-1)->setData(Qt::UserRole,Line[1]);
            ui->Sections->setCurrentRow(ui->Sections->count()-1);
            SectionClicked();
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
    RemoveSection.exec(QString("DELETE FROM Sections WHERE Id = \"%1\" AND Field = \"%2\"")
                       .arg(ui->Sections->currentItem()->data(Qt::UserRole).toString(),
                            ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    ui->Sections->takeItem(ui->Sections->currentRow());
    SectionClicked();
}

void NewDatabaseFile::on_addExType_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this,ui->addExType->property("Info").toString()+ui->Sections->currentItem()->text(),true);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection1(currentbase);
        QSqlQuery AddSection2(currentbase);
        AddSection1.prepare(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")").arg(Line[1],Line[0]));
        AddSection2.prepare(QString("INSERT OR IGNORE INTO \"Sections_Exercises\" (\"Exercise_Id\",\"Section_Id\") VALUES(\"%1\",\"%2\")")
                        .arg(Line[1],ui->Sections->currentItem()->data(Qt::UserRole).toString()));
        if(AddSection1.exec() && AddSection2.exec()){
            ui->SubSections->addItem(Line[0]);
            ui->SubSections->item(ui->SubSections->count()-1)->setData(Qt::UserRole,Line[1]);
            ui->SubSections->setCurrentRow(ui->SubSections->count()-1);
            SubSectionClicked();
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
        NewFileType.prepare(QString("INSERT OR IGNORE INTO \"FileTypes\" (\"Id\",\"FileType\",\"FolderName\",\"Solvable\") VALUES(:id,:name,:folder,:sol)"));
        NewFileType.bindValue(":id",data[0]);
        NewFileType.bindValue(":name",data[1]);
        NewFileType.bindValue(":folder",data[2]);
        NewFileType.bindValue(":sol",data[3]);
        NewFileType.exec();
        if(data[3]=="1"){
            NewFileType.prepare(QString("INSERT OR IGNORE INTO \"FileTypes\" (\"Id\",\"FileType\",\"FolderName\",\"Solvable\",\"BelongsTo\") VALUES(:id,:name,:folder,-1,:fid)"));
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
            RemoveExerciseType.exec(QString("DELETE FROM \"FileTypes\" WHERE \"Id\" = \"%1\"").arg(FileType));
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
    for (int item=0;item<Field_Names.count();item++ ) {
        ui->FieldTable->addItem(Field_Names.at(item));
        ui->FieldTable->item(item)->setData(Qt::UserRole,Field_ids.at(item));
    }
    ui->FilePathLine->clear();
    ui->FileNameLine->clear();
    ui->NewFileContentText->clear();

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
//    ui->SingleSection->setEnabled(CustomFileTypesList.contains(button));
//    ui->MultiSection->setEnabled(CustomFileTypesList.contains(button));
    ui->UseExerciseType->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    needsSubSection = FileType == "SectEx" || FileType == "SectSub" || FileType =="CombEx"
            || FileType == "CombSub" || ui->UseExerciseType->isChecked();
}

void NewDatabaseFile::on_SingleSection_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void NewDatabaseFile::on_MultiSection_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

QString NewDatabaseFile::ClearMetadataFromContent(QString Content)
{
    QStringList text;
//    QStringList preamble;
//    QString cleanContent = Content;
//    QTextStream PreStream(&Content);
//    while(!PreStream.atEnd()){
//        QString line = PreStream.readLine();
//        preamble<<line;
//        if(line.startsWith("\\begin{document}")){
//            break;
//        }
//    }
//    cleanContent.remove(preamble.join("\n"));
//    cleanContent.remove("\\begin{document}");
//    cleanContent.remove("\\end{document}");
//    cleanContent = cleanContent.trimmed();
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database File :")
                || line.startsWith("%@ Database source:")
                || line.startsWith("%# End of file")){
            continue;
        }
        text<<line;
    }
    Content=text.join("\n");
    return Content;
}

void NewDatabaseFile::on_DatabaseCombo_activated(int index)
{
    QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
    currentbase = DataTex::GlobalFilesDatabaseList[database];
    DataBase_Path = QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator();
    Field_Names.clear();
    Field_ids.clear();
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_Names.append(fields.value(0).toString());
        Field_ids.append(fields.value(1).toString());
    }
    bool clone = index>0;
    ui->DefinitionButton->setEnabled(clone);
    ui->TheoremButton->setEnabled(clone);
    ui->FigureButton->setEnabled(clone);
    ui->TableButton->setEnabled(clone);
    ui->PropositionButton->setEnabled(clone);
    ui->LemmButton->setEnabled(clone);
    ui->CorollaryButton->setEnabled(clone);
    ui->AxiomButton->setEnabled(clone);
    ui->SectionExerciseButton->setEnabled(clone);
    ui->SectionSubjectButton->setEnabled(clone);
    ui->CombExerciseButton->setEnabled(clone);
    ui->CombSubjectButton->setEnabled(clone);
    ui->MethodButton->setEnabled(clone);
    ui->ExampleButton->setEnabled(clone);
    LoadFileTypes();
    foreach(QAbstractButton * bt,CustomFileTypesList){
        bt->setEnabled(clone);
    }
}


void NewDatabaseFile::FieldsClicked()
{
    FieldsSelected.clear();
    Selected_Field_ids.clear();
    ui->FilterChapters->setEnabled(true);
    for (int i=0;i<ui->FieldTable->count();i++) {
        if(ui->FieldTable->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_Field_ids.append(ui->FieldTable->item(i)->data(Qt::UserRole).toString());
            FieldsSelected.append(ui->FieldTable->item(i)->text());
        }
    }

    Chapter_Names.clear();
    Chapter_ids.clear();
    QSqlQuery Chapters(currentbase);
    Chapters.exec(QString("SELECT Name,Id FROM Chapters WHERE Field IN (\"%1\")").arg(Selected_Field_ids.join("\",\"")));
    while(Chapters.next()){
        Chapter_Names.append(Chapters.value(0).toString());
        Chapter_ids.append(Chapters.value(1).toString());
    }

    ui->Chapters->clear();
    for (int item=0;item<Chapter_Names.count();item++ ) {
        ui->Chapters->addItem(Chapter_Names.at(item));
        ui->Chapters->item(item)->setData(Qt::UserRole,Chapter_ids.at(item));
//        ui->Chapters->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
//        ui->Chapters->item(item)->setCheckState(Qt::Checked);
    }
    Selected_Chapters_ids.clear();
    for(int i=0;i<ui->Chapters->count();++i){
        if(ui->Chapters->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_Chapters_ids.append(ui->Chapters->item(i)->data(Qt::UserRole).toString());
            ChaptersSelected.append(ui->Chapters->item(i)->text());
        }
    }
    QCompleter *completer = new QCompleter(Chapter_Names, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->FilterChapters->setCompleter(completer);

    Sections_Names.clear();
    Sections_ids.clear();
    QSqlQuery Sections(currentbase);
    Sections.exec(QString("SELECT Name,Id FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.join("\",\"")));
    while(Sections.next()){
        Sections_Names.append(Sections.value(0).toString());
        Sections_ids.append(Sections.value(1).toString());
    }

    ui->Sections->clear();
    for (int item=0;item<Sections_Names.count();item++ ) {
        ui->Sections->addItem(Sections_Names.at(item));
        ui->Sections->item(item)->setData(Qt::UserRole,Sections_ids.at(item));
//        ui->Sections->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
//        ui->Sections->item(item)->setCheckState(Qt::Checked);
    }
    Selected_Sections_ids.clear();
    for(int i=0;i<ui->Sections->count();++i){
        if(ui->Sections->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_Sections_ids.append(ui->Sections->item(i)->data(Qt::UserRole).toString());
            SectionsSelected.append(ui->Sections->item(i)->text());
        }
    }

    SubSections_Names.clear();
    SubSections_ids.clear();
    QSqlQuery SubSections(currentbase);
    SubSections.exec(QString("SELECT DISTINCT et.Name,et.Id "
                             "FROM Sections_Exercises se "
                             "LEFT JOIN Exercise_Types et ON et.Id = se.Exercise_Id "
                             "WHERE Section_Id IN (\"&1\")").arg(Selected_Sections_ids.join("\",\"")));
    while(SubSections.next()){
        SubSections_Names.append(SubSections.value(0).toString());
        SubSections_ids.append(SubSections.value(1).toString());
    }

    ui->SubSections->clear();
    for (int item=0;item<SubSections_Names.count();item++ ) {
        ui->SubSections->addItem(SubSections_Names.at(item));
        ui->SubSections->item(item)->setData(Qt::UserRole,SubSections_ids.at(item));
//        ui->Sections->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
//        ui->Sections->item(item)->setCheckState(Qt::Checked);
    }
    Selected_SubSections_ids.clear();
    for(int i=0;i<ui->SubSections->count();++i){
        if(ui->SubSections->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_SubSections_ids.append(ui->SubSections->item(i)->data(Qt::UserRole).toString());
            SubSectionsSelected.append(ui->SubSections->item(i)->text());
        }
    }
    DataTex::StretchColumnsToWidth(ui->ExerciseFileList);
    ui->ExerciseFileList->setColumnHidden(2,true);
    ui->addChapter->setEnabled(true);
    ui->removeChapter->setEnabled(true);
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.arg(Selected_Field_ids.join("|"),"","","",FileType));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    qDebug()<<SqlFunctions::UpdateTableFiles.arg(Selected_Field_ids.join("|"),"","","",FileType);
}

void NewDatabaseFile::ChaptersClicked()
{
    ChaptersSelected.clear();
    Selected_Chapters_ids.clear();
    ui->FilterSections->setEnabled(true);
    for(int i=0;i<ui->Chapters->count();++i){
        if(ui->Chapters->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_Chapters_ids.append(ui->Chapters->item(i)->data(Qt::UserRole).toString());
            ChaptersSelected.append(ui->Chapters->item(i)->text());
        }
    }
    Sections_Names.clear();
    Sections_ids.clear();
    QSqlQuery Sections(currentbase);
    Sections.exec(QString("SELECT Name,Id FROM Sections WHERE Chapter IN (\"%1\")").arg(Selected_Chapters_ids.join("\",\"")));
    while(Sections.next()){
        Sections_Names.append(Sections.value(0).toString());
        Sections_ids.append(Sections.value(1).toString());
    }
    ui->Sections->clear();
    for (int item=0;item<Sections_Names.count();item++ ) {
        ui->Sections->addItem(Sections_Names.at(item));
        ui->Sections->item(item)->setData(Qt::UserRole,Sections_ids.at(item));
//        ui->Sections->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
//        ui->Sections->item(item)->setCheckState(Qt::Checked);
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
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
}

void NewDatabaseFile::SectionClicked()
{
    SectionsSelected.clear();
    Selected_Sections_ids.clear();
    for(int i=0;i<ui->Sections->count();++i){
        if(ui->Sections->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_Sections_ids.append(ui->Sections->item(i)->data(Qt::UserRole).toString());
            SectionsSelected.append(ui->Sections->item(i)->text());
        }
    }
    if(needsSubSection){
        ui->FilterSubSections->setEnabled(true);
        SubSections_Names.clear();
        SubSections_ids.clear();
        QSqlQuery SubSections(currentbase);
        SubSections.exec(QString("SELECT DISTINCT e.Name,se.Exercise_Id FROM Sections_Exercises se JOIN Exercise_Types e ON e.Id=se.Exercise_Id WHERE se.Section_Id IN (\"%1\")").arg(Selected_Sections_ids.join("\",\"")));
        while(SubSections.next()){
            SubSections_Names.append(SubSections.value(0).toString());
            SubSections_ids.append(SubSections.value(1).toString());
        }
        ui->SubSections->clear();
        for (int item=0;item<SubSections_Names.count();item++ ) {
            ui->SubSections->addItem(SubSections_Names.at(item));
            ui->SubSections->item(item)->setData(Qt::UserRole,SubSections_ids.at(item));
    //        ui->Sections->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    //        ui->Sections->item(item)->setCheckState(Qt::Checked);
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
}

void NewDatabaseFile::SubSectionClicked()
{
    SubSectionsSelected.clear();
    Selected_SubSections_ids.clear();
    for(int i=0;i<ui->SubSections->count();++i){
        if(ui->SubSections->item(i)->isSelected()/*checkState() == Qt::Checked*/){
            Selected_SubSections_ids.append(ui->SubSections->item(i)->data(Qt::UserRole).toString());
            SubSectionsSelected.append(ui->SubSections->item(i)->text());
        }
    }
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(needsSubSection);
    ui->SaveSelectionsCheckBox->setEnabled(needsSubSection);
    tagLine->setEnabled(needsSubSection);
    foreach(QAbstractButton * button,CustomFileTypesList){
        ui->UseExerciseType->setEnabled(button->isChecked());
    }
    UpdateFileInfo();
}

void NewDatabaseFile::UpdateFileInfo()
{
    QString Fields =  FieldsSelected.join(" , ");
    QString Chapters = ChaptersSelected.join(" , ");
    QString Sections = SectionsSelected.join(" , ");
    QString SubSections = SubSectionsSelected.join(" , ");
    ui->FileInfo->setText("#### File Info<br />---------------<br />**Fields**<br />"+Fields+"<br />**Chapters**<br />"+Chapters+"<br />**Sections**<br />"+Sections+
                          "<br />**SubSections**<br />"+SubSections);
    NewFilePathAndId();

}

void NewDatabaseFile::NewFilePathAndId()
{
    QString folderName = SqlFunctions::Get_StringList_From_Query(
                SqlFunctions::FolderName.arg(FileType),currentbase).at(0);
    QString Fields = FieldsSelected.join("|");
    QString Chapters = ChaptersSelected.join("|");
    QString Sections = SectionsSelected.join("|");
    QString FieldId = Selected_Field_ids.join("");
    QString ChapterId = Selected_Chapters_ids.join("");
    QString SectionId = Selected_Sections_ids.join("");
    QString ExTypeId = Selected_SubSections_ids.join("");
    QString Path = DataBase_Path+Fields+QDir::separator()+Chapters+QDir::separator()+Sections+QDir::separator()+folderName+QDir::separator();

    QString fileId = (needsSubSection) ? FieldId+"-"+ChapterId+"-"+SectionId+"-"+ExTypeId+"-"+FileType
                                       : FieldId+"-"+ChapterId+"-"+SectionId+"-"+FileType;
    QStringList ExistingFiles = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT Id FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(fileId),currentbase);
    QRegExp file_index("[0-9]{1,}");
    int fileNumber = 1;
//    for (int i=0;i<ExistingFiles.count();i++) {
//        QString file = ExistingFiles[i];
//        file_index.indexIn(file);
//        QString number = file_index.capturedTexts().last();
//        if(number.toInt()!=i+2){
//            qDebug()<<number;
//            fileNumber = i+2;
//            break;
//        }
//    }
    while(ExistingFiles.contains(fileId+QString::number(fileNumber))){
        fileNumber++;
    }
    file_index.indexIn(QFileInfo(metadata["Path"]).baseName());
    QString number = file_index.capturedTexts().last();
    int filecount = (!EditMode) ? fileNumber : number.toInt();
    QString fileName = Path+fileId+QString::number(filecount)+".tex";
    ui->NewFileContentText->setText(DataTex::NewFileText(fileName,ImportedFileContent,currentbase));
    ui->FilePathLine->setText(Path);
    ui->FileNameLine->setText(fileId+QString::number(filecount)+".tex");
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
            button->setEnabled(!CloneMode);
            CustomFileTypesList.append(button);
            ui->FileTypeGroup->addButton(button);
            ui->gridLayout_10->addWidget(button,ui->gridLayout_10->rowCount(),0);
        }
    }
    for(int i=0;i<ui->FileTypeGroup->buttons().count();i++){
        connect(ui->FileTypeGroup->buttons().at(i), &QRadioButton::toggled, this,
                [=](){
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

    }
    if(!chapters.isEmpty()){
        foreach(QString chapter,chapters){
            foreach(QListWidgetItem * item,FindListItemByData(ui->Chapters,chapter)){
                item->setSelected(true);
                ChaptersClicked();
            }
        }
    }
    if(!sections.isEmpty()){
        foreach(QString section,sections){
            foreach(QListWidgetItem * item,FindListItemByData(ui->Sections,section)){
                item->setSelected(true);
                SectionClicked();
            }
        }
    }
    if(!subsections.isEmpty()){
        foreach(QString subsection,subsections){
            foreach(QListWidgetItem * item,FindListItemByData(ui->SubSections,subsection)){
                item->setSelected(true);
                SubSectionClicked();
            }
        }
    }
}

void NewDatabaseFile::SaveSettings()
{
    QSqlQuery SaveSelections(DataTex::DataTeX_Settings);
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+QString::number(ui->SaveSelectionsCheckBox->isChecked())+"' WHERE Setting = 'SaveNewFileSelections'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+FileType+"' WHERE Setting = 'NewDatabaseFile_CurrentFileType'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+ui->FieldTable->currentItem()->data(Qt::UserRole).toString()+"' WHERE Setting = 'NewDatabaseFile_CurrentField'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Chapters_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentChapter'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_Sections_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_CurrentSection'");
    SaveSelections.exec("UPDATE Initial_Settings SET Value = '"+Selected_SubSections_ids.join(",")+"' WHERE Setting = 'NewDatabaseFile_ExerciseType'");
}
