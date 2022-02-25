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

NewDatabaseFile::NewDatabaseFile(QWidget *parent,QStringList meta,QStringList sections,bool editMode,QString fileName,bool cloneMode) :
    QDialog(parent),
    ui(new Ui::NewDatabaseFile)
{
    ui->setupUi(this);
    EditMode = editMode;
    CloneMode = cloneMode;
    FileName = fileName;
    ui->addButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
    ui->SectionExercisesChapterComboBox->setEnabled(false);
    ui->SectionExercisesSectionComboBox->setEnabled(false);
    ui->SectionExercisesExTypeComboBox->setEnabled(false);
    ui->NewFileContentText->setEnabled(false);
    ui->DefinitionButton->setProperty("Name","Def");
    ui->TheoremButton->setProperty("Name","Theor");
    ui->FigureButton->setProperty("Name","Fig");
    ui->TableButton->setProperty("Name","Tab");
    ui->SectionExerciseButton->setProperty("Name","SectEx");
    ui->SectionSubjectButton->setProperty("Name","SectSub");
    ui->CombExerciseButton->setProperty("Name","CombEx");
    ui->CombSubjectButton->setProperty("Name","CombSub");
    ui->MethodButton->setProperty("Name","Method");
    ui->ExampleButton->setProperty("Name","Example");
    ui->DefinitionButton->setEnabled(!CloneMode);
    ui->TheoremButton->setEnabled(!CloneMode);
    ui->FigureButton->setEnabled(!CloneMode);
    ui->TableButton->setEnabled(!CloneMode);
    ui->SectionExerciseButton->setEnabled(!CloneMode);
    ui->SectionSubjectButton->setEnabled(!CloneMode);
    ui->CombExerciseButton->setEnabled(!CloneMode);
    ui->CombSubjectButton->setEnabled(!CloneMode);
    ui->MethodButton->setEnabled(!CloneMode);
    ui->ExampleButton->setEnabled(!CloneMode);

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
    ImportedFileContent = (editMode || cloneMode) ? ClearMetadataFromContent(metadata[8]) : "";
    ImportedSectionList = sections;
    qDebug()<<ImportedSectionList;
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_Names.append(fields.value(0).toString());
        Field_ids.append(fields.value(1).toString());
    }

    QSqlQuery FileTypes(currentbase);
    FileTypes.exec("SELECT \"FileType\",\"Id\",\"FolderName\" FROM \"FileTypes\" WHERE Solvable >-1");
    int line = -1;
    ui->gridLayout_10->removeItem(ui->gridLayout_8);
    ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
    while(FileTypes.next()){
        line++;
        if(line>9){
            QRadioButton * button = new QRadioButton(FileTypes.value(0).toString(),this);
            button->setProperty("Name",FileTypes.value(1).toString());
            button->setEnabled(!CloneMode);
            CustomFileTypesList.append(button);
            ui->FileTypeGroup->addButton(button);
            ui->gridLayout_10->addWidget(button,ui->gridLayout_10->rowCount(),0);
        }
    }
    ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
    ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
    ui->removeFileType->setEnabled(false);

    for(int i=0;i<ui->FileTypeGroup->buttons().count();i++){
        connect(ui->FileTypeGroup->buttons().at(i), &QRadioButton::toggled, this,
                [=](){
            FileTypeClicked();
        });
    }

    TheoryView = new PdfViewer(this);
    ui->verticalLayout_5->addWidget(TheoryView);
    ui->splitter_2->setSizes(QList<int>({1,1, 400}));
    TheoryView->show();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    ui->BuildBox->addItems(DataTex::LatexCommands.keys());
    for (int i=0;i<PreambleIds.count();i++) {
        ui->PreambleBox->addItem(PreambleNames.at(i),QVariant(PreambleIds.at(i)));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    ui->SingleSection->setEnabled(false);
    ui->MultiSection->setEnabled(false);
    connect(ui->NewFileContentText,&QTextEdit::textChanged,this,[=](){
        if(!ui->BuildBox->isEnabled()){
            ui->BuildBox->setEnabled(true);
            ui->PreambleBox->setEnabled(true);
        }
    });
    ui->DatabaseCombo->setVisible(CloneMode);
    ui->DatabaseLabel->setVisible(CloneMode);
    if(EditMode){
        EditModeIsEnabled();
    }
//    if(CloneMode){
//        on_DatabaseCombo_activated();
//    }
    ui->splitter_2->setSizes(QList<int>({150, 300,350}));
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));

    ui->UseExerciseType->setEnabled(false);
    connect(ui->UseExerciseType,&QCheckBox::clicked,this,[=](bool checked){
        ui->SectionExercisesExTypeComboBox->setEnabled(checked);
        ui->addExType->setEnabled(checked);
        ui->removeExType->setEnabled(checked);
        ui->SectionExercisesExTypeComboBox->clear();
        QList<QStringList> data = SqlFunctions::ComboList_Double_List(
                    SqlFunctions::Exercise_Types_Query.arg(ui->SectionExercisesSectionComboBox->currentData().toString()),currentbase);
        QCompleter *completer = new QCompleter(data[0], this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->SectionExercisesExTypeComboBox->setCompleter(completer);
        for (int item=0;item<data[0].count() ;item++ ) {
            ui->SectionExercisesExTypeComboBox->addItem(data[0].at(item),QVariant(data[1].at(item)));
        }
        ui->SectionExercisesExTypeComboBox->setCurrentIndex(-1);
        if(ui->FieldTable->currentRow()>-1){
            updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                            arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                                ui->SectionExercisesChapterComboBox->currentData().toString(),
                                ui->SectionExercisesSectionComboBox->currentData().toString(),"",FileType));
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    });
}

NewDatabaseFile::~NewDatabaseFile()
{
    delete ui;
}

void NewDatabaseFile::EditModeIsEnabled()
{
    QString filetype = metadata.last();
    int IsMultiSection = metadata.at(metadata.size()-2).toInt();
    qDebug()<<IsMultiSection;
    for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
        if(ui->FileTypeGroup->buttons().at(i)->property("Name") == filetype){
            ui->FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata[1],Qt::MatchExactly).at(0));
    on_FieldTable_itemClicked(ui->FieldTable->findItems(metadata[1],Qt::MatchExactly).at(0));
    if(IsMultiSection){
        for(int i=0;i<ImportedSectionList.count();i++){
            on_SectionList_itemDoubleClicked(ui->SectionList->findItems(ImportedSectionList[i],Qt::MatchExactly).at(0));
        }
    }
    else{
        int chap = ui->SectionExercisesChapterComboBox->findText(metadata[2]);
        if ( chap != -1 ) {
            ui->SectionExercisesChapterComboBox->setCurrentIndex(chap);
            on_SectionExercisesChapterComboBox_activated(chap);
        }
        int sect = ui->SectionExercisesSectionComboBox->findText(ImportedSectionList[0]);
        if ( sect != -1 ) {
            ui->SectionExercisesSectionComboBox->setCurrentIndex(sect);
            on_SectionExercisesSectionComboBox_activated(sect);
        }
        int extype = ui->SectionExercisesExTypeComboBox->findText(metadata[3]);
        if ( extype != -1 ) {
            ui->SectionExercisesExTypeComboBox->setCurrentIndex(extype);
            on_SectionExercisesExTypeComboBox_activated(extype);
        }
    }
    int diff = metadata[4].toInt();
    ui->DifficultySpin->setValue(diff);
    int preamble = ui->PreambleBox->findData(metadata[5]);//Να γίνει findText όταν αλλάξω στον πίνακα Database_Files τη στήλη Preamble να δείχνει την τιμή.
    if ( preamble != -1 ) {
        ui->PreambleBox->setCurrentIndex(preamble);
    }
    int build = ui->SectionExercisesExTypeComboBox->findText(metadata[6]);
    if ( build != -1 ) {
        ui->BuildBox->setCurrentIndex(build);
    }
    ui->DescriptionLine->setText(metadata[7]);
    ui->NewFileContentText->setText(metadata[8]);
    QString file = NewFilePathAndId();
    ui->FilePathLine->setText(QFileInfo(file).absolutePath());
    ui->FileNameLine->setText(FileName+".tex");
}

QString NewDatabaseFile::NewFilePathAndId()
{
    SectionList.clear();
    QString folderName = SqlFunctions::Get_StringList_From_Query(
                SqlFunctions::FolderName.arg(FileType),currentbase).at(0);
    QString Field;
    QString Chapter;
    QString Section;
    QString ExType;
    QString FieldId;
    QString ChapterId;
    QString SectionId;
    QString ExTypeId;
    QString Path;
    QStringList sectionslist;
    if(ui->FieldTable->currentRow()>-1){
        Field = ui->FieldTable->currentItem()->text();
        FieldId = ui->FieldTable->currentItem()->data(Qt::UserRole).toString();
    }
    if(ui->SectionExercisesChapterComboBox->currentIndex()>-1){
        Chapter = ui->SectionExercisesChapterComboBox->currentText();
        ChapterId = ui->SectionExercisesChapterComboBox->currentData().toString();
    }
    if(ui->SectionExercisesSectionComboBox->currentIndex()>-1){
        Section = ui->SectionExercisesSectionComboBox->currentText();
        SectionId = ui->SectionExercisesSectionComboBox->currentData().toString();
    }
    QStringList datalist = {Field,Chapter,Section,folderName};
    QStringList IdList = {FieldId,ChapterId,SectionId};
    if(!ui->SectionExercisesExTypeComboBox->currentText().isEmpty()){
        ExType = ui->SectionExercisesExTypeComboBox->currentText();
        ExTypeId = ui->SectionExercisesExTypeComboBox->currentData().toString();
        datalist.append(ExType);
        IdList.append(ExTypeId);
    }
    for (int i=0;i<ui->SelectedSections->count();i++ ) {
        sectionslist.append(ui->SelectedSections->item(i)->text());
        SectionList.append(ui->SelectedSections->item(i)->data(Qt::UserRole).toString());
    }
    QString FolderName;
    QString SectionsId;
    if(ui->SelectedSections->count()>0){
        FolderName = sectionslist.join("-");
        SectionsId = SectionList.join("");
    }
    QString fileName;
    if (ui->stackedWidget->currentIndex()==1){
        Path = DataBase_Path+Field+QDir::separator()+folderName+QDir::separator()+FolderName+QDir::separator();
    }
    else {
        Path = DataBase_Path+datalist.join(QDir::separator())+QDir::separator();
    }
    QString fileId;
    if (ui->stackedWidget->currentIndex()==1){
        fileId = FieldId+"-"+SectionsId+"-"+FileType;
    }
    else {
        fileId = IdList.join("-")+"-"+FileType;
    }
    QString count = SqlFunctions::Get_StringList_From_Query(
                QString("SELECT COUNT(Id) FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(fileId),currentbase).at(0);
    int filecount = 0;
    if(count == "0"){
        filecount = 1;
    }
    else{
        filecount = count.toInt()+1;
    }
    fileName = Path+"DTX-"+fileId+QString::number(filecount)+".tex";
    return fileName;
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

void NewDatabaseFile::on_FieldTable_itemClicked(QListWidgetItem *item)
{
    ui->NewFileContentText->setEnabled(true);
    QString Id = item->data(Qt::UserRole).toString();
    if(ui->stackedWidget->currentIndex()==0){
        ui->SectionExercisesChapterComboBox->setEnabled(true);
        ui->addChapter->setEnabled(true);
        ui->removeChapter->setEnabled(true);
        ui->SectionExercisesChapterComboBox->clear();
        ui->SectionExercisesSectionComboBox->clear();
        ui->SectionExercisesExTypeComboBox->clear();
        QList<QStringList> data =
                SqlFunctions::ComboList_Single(SqlFunctions::Chapters_Query,currentbase,item->text());
        QCompleter *completer = new QCompleter(data[0], this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->SectionExercisesChapterComboBox->setCompleter(completer);
        for (int item=0;item<data[0].count() ;item++ ) {
            ui->SectionExercisesChapterComboBox->addItem(data[0].at(item),QVariant(data[1].at(item)));
        }
        ui->SectionExercisesChapterComboBox->setCurrentIndex(-1);
        if(ui->FieldTable->currentRow()>-1){
            updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.arg(Id,"","","",FileType));
        }
    }
    else{
        ui->SectionList->clear();
        QList<QStringList> data =
                SqlFunctions::ComboList_Single(SqlFunctions::Sections_Query,currentbase,
                                               ui->FieldTable->currentItem()->data(Qt::UserRole).toString());
        for (int item=0;item<data[0].count() ;item++ ) {
            ui->SectionList->addItem(data[0].at(item));
            ui->SectionList->item(item)->setData(Qt::UserRole,data[1].at(item));
        }
        ui->SectionList->setCurrentRow(-1);
        connect(ui->FilterAvailableSections,&QLineEdit::textChanged,this,[=](QString text){
            for (int k1 = 0; k1 < ui->SectionList->count(); ++k1)
                {
                if (ui->SectionList->item(k1)->text().startsWith(text,Qt::CaseInsensitive))
                {
                     ui->SectionList->item(k1)->setHidden(false);
                }
                else
                {
                     ui->SectionList->item(k1)->setHidden(true);
                }
            }
        });
    }
    for (int c = 0; c < ui->ExerciseFileList->horizontalHeader()->count()-1; ++c)
    {
        ui->ExerciseFileList->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
    ui->ExerciseFileList->setColumnHidden(2,true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
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

void NewDatabaseFile::on_SectionExercisesChapterComboBox_activated(int index)
{
        ui->SectionExercisesSectionComboBox->setEnabled(true);
        ui->addSection->setEnabled(true);
        ui->removeSection->setEnabled(true);
        ui->SectionExercisesSectionComboBox->clear();
        ui->SectionExercisesExTypeComboBox->clear();
        QList<QStringList> data =
                SqlFunctions::ComboList_Single(SqlFunctions::Sections_Chapters_Query,currentbase,
                                               ui->SectionExercisesChapterComboBox->currentText());
        QCompleter *completer = new QCompleter(data[0], this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->SectionExercisesSectionComboBox->setCompleter(completer);
        for (int item=0;item<data[0].count() ;item++ ) {
            ui->SectionExercisesSectionComboBox->addItem(data[0].at(item),QVariant(data[1].at(item)));
        }
        ui->SectionExercisesSectionComboBox->setCurrentIndex(-1);
        if(ui->FieldTable->currentRow()>-1){
            updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                            arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                                ui->SectionExercisesChapterComboBox->currentData().toString(),"","",FileType));
        }
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
}

void NewDatabaseFile::on_SectionExercisesSectionComboBox_activated(int index)
{
    ui->SectionExercisesExTypeComboBox->clear();
        if(FileType == "SectEx" || FileType == "SectSub"){
            ui->SectionExercisesExTypeComboBox->setEnabled(true);
            ui->addExType->setEnabled(true);
            ui->removeExType->setEnabled(true);
            ui->SectionExercisesExTypeComboBox->clear();
            QList<QStringList> data = SqlFunctions::ComboList_Double_List(
                        SqlFunctions::Exercise_Types_Query.arg(ui->SectionExercisesSectionComboBox->currentData().toString()),currentbase);
            QCompleter *completer = new QCompleter(data[0], this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            ui->SectionExercisesExTypeComboBox->setCompleter(completer);
            for (int item=0;item<data[0].count() ;item++ ) {
                ui->SectionExercisesExTypeComboBox->addItem(data[0].at(item),QVariant(data[1].at(item)));
            }
            ui->SectionExercisesExTypeComboBox->setCurrentIndex(-1);
            if(ui->FieldTable->currentRow()>-1){
                updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                                arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                                    ui->SectionExercisesChapterComboBox->currentData().toString(),
                                    ui->SectionExercisesSectionComboBox->currentData().toString(),"",FileType));
            }
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
        connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
        if(FileType != "SectEx" && FileType != "SectSub"){
            QString Path = NewFilePathAndId();
            ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
            ui->FileNameLine->setText(QFileInfo(Path).fileName());
            ui->NewFileContentText->setText(DataTex::NewFileText(Path,ImportedFileContent,currentbase));
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            foreach(QAbstractButton * button,CustomFileTypesList)
            if(button->isChecked()){
                ui->UseExerciseType->setEnabled(true);
            }
        }
}

void NewDatabaseFile::on_SectionExercisesExTypeComboBox_activated(int index)
{
    updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                    arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                        ui->SectionExercisesChapterComboBox->currentData().toString(),
                        ui->SectionExercisesSectionComboBox->currentData().toString(),
                        ui->SectionExercisesExTypeComboBox->currentData().toString(),FileType));
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    QString Path = NewFilePathAndId();
    ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
    ui->FileNameLine->setText(QFileInfo(Path).fileName());
    ui->NewFileContentText->setText(DataTex::NewFileText(Path,ImportedFileContent,currentbase));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void NewDatabaseFile::on_SectionList_itemClicked(QListWidgetItem *item)
{
    ui->addButton->setEnabled(true);
}

void NewDatabaseFile::on_SectionList_itemDoubleClicked(QListWidgetItem *item)
{
    int row = ui->SectionList->row(item);
    int SelectedSections = ui->SelectedSections->count();
    int SectionCount = ui->SectionList->count();
    SectionCount--;
    SelectedSections++;
    QListWidgetItem *widget = ui->SectionList->takeItem(row);
    if(SectionCount!=0){
        ui->SectionList->removeItemWidget(widget);
        ui->SelectedSections->addItem(widget);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else if(SectionCount==0){
        ui->SectionList->removeItemWidget(widget);
        ui->SelectedSections->addItem(widget);
        ui->addButton->setEnabled(false);
    }
    ui->SelectedSections->sortItems();
    QString Path = NewFilePathAndId();
    ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
    ui->FileNameLine->setText(QFileInfo(Path).fileName());
    ui->NewFileContentText->setText(DataTex::NewFileText(Path,ImportedFileContent,currentbase));
}

void NewDatabaseFile::on_addButton_clicked()
{
    on_SectionList_itemDoubleClicked(ui->SectionList->currentItem());
}

void NewDatabaseFile::on_removeButton_clicked()
{
    int row = ui->SelectedSections->currentRow();
    int SelectedSections = ui->SelectedSections->count();
    SelectedSections--;
    QListWidgetItem * currentItem = ui->SelectedSections->takeItem(row);
    ui->SelectedSections->sortItems();
    if(SelectedSections!=0){
        ui->SelectedSections->removeItemWidget(currentItem);
        ui->SectionList->addItem(currentItem);
        ui->SectionList->sortItems();
        QString Path = NewFilePathAndId();
        ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
        ui->FileNameLine->setText(QFileInfo(Path).fileName());
        ui->NewFileContentText->setText(DataTex::NewFileText(Path,ImportedFileContent,currentbase));
        ui->NewFileContentText->setText(DataTex::NewFileText(Path,ImportedFileContent,currentbase));
    }
    else if(SelectedSections==0){
        ui->SelectedSections->removeItemWidget(currentItem);
        ui->SectionList->addItem(currentItem);
        ui->SectionList->sortItems();
        ui->removeButton->setEnabled(false);
        ui->FilePathLine->clear();
        ui->FileNameLine->clear();
        ui->NewFileContentText->clear();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void NewDatabaseFile::on_SelectedSections_itemClicked(QListWidgetItem *item)
{
    ui->removeButton->setEnabled(true);
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
    SectionList.clear();
    QString Path = QFileInfo(NewFilePathAndId()).absolutePath()+QDir::separator();
    QDir dir(Path);
    if (!dir.exists())
        dir.mkpath(".");
    QString Field_Id = ui->FieldTable->currentItem()->data(Qt::UserRole).toString();
    QString ChapterId;
    QString ExType;
    QString Solved;
    if(ui->stackedWidget->currentIndex()==0){
        ChapterId = ui->SectionExercisesChapterComboBox->currentData().toString();
        SectionList.append(ui->SectionExercisesSectionComboBox->currentData().toString());
        if(!ui->SectionExercisesExTypeComboBox->currentText().isEmpty()){
            ExType = ui->SectionExercisesExTypeComboBox->currentData().toString();
        }
        else{
            ExType = "-";
        }
    }
    else if(ui->stackedWidget->currentIndex()==1){
        ChapterId ="-";
        ExType = "-";
    }

    if(NeedsSolution){
        Solved = tr("NO");
    }
    else{
        Solved = "-";
    }
    QString FileContent = ui->NewFileContentText->toPlainText();
    QStringList SectionsToCsv = SectionList;
    QString JoinedSections = SectionsToCsv.join(",");
    QString Text;
    mapIdsNames.insert("Id",QFileInfo(ui->FileNameLine->text()).baseName());
    Text = "Id,"+QFileInfo(ui->FileNameLine->text()).baseName()+"\n";
    mapIdsNames.insert("FileType",FileType);
    Text += "FileType,"+FileType+"\n";
    mapIdsNames.insert("Field",Field_Id);
    Text += "Field,"+Field_Id+"\n";
    mapIdsNames.insert("Chapter",ChapterId);
    Text += "Chapter,"+ChapterId+"\n";
    Text += "Section,"+JoinedSections+"\n";
    mapIdsNames.insert("ExerciseType",ExType);
    Text += "ExerciseType,"+ExType+"\n";
    mapIdsNames.insert("Difficulty",QString::number(ui->DifficultySpin->value()));
    Text += "Difficulty,"+QString::number(ui->DifficultySpin->value())+"\n";
    mapIdsNames.insert("Path",ui->FilePathLine->text()+ui->FileNameLine->text());
    QString fileName = mapIdsNames["Path"];
    Text += "Path,"+fileName+"\n";
    mapIdsNames.insert("Date",QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm"));
    Text += "Date,"+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"\n";
    mapIdsNames.insert("Solved",Solved);
    Text += "Solved,"+Solved+"\n";
    mapIdsNames.insert("Bibliography","@article{"+mapIdsNames["Id"]+",}");//Προσωρινό το @article
    Text += "Bibliography,@article{"+mapIdsNames["Id"]+",}\n";
    mapIdsNames.insert("FileContent",FileContent);
    QString Content = FileContent;
    QTextStream contentline(&Content);
    QStringList Line;
    while(!contentline.atEnd()){
        Line.append(contentline.readLine());
    }
    Content=Line.join("\\qt_endl");
    Text += "FileContent,"+Content+"\n";
    mapIdsNames.insert("Preamble",ui->PreambleBox->currentData().toString());
    Text += "Preamble,"+ui->PreambleBox->currentData().toString()+"\n";
    mapIdsNames.insert("BuildCommand",ui->BuildBox->currentText());
    Text += "BuildCommand,"+ui->BuildBox->currentText()+"\n";
    mapIdsNames.insert("FileDescription",ui->DescriptionLine->text());
    Text += "FileDescription,"+ui->DescriptionLine->text()+"\n";
    mapIdsNames.insert("MultiSection",QString::number(ui->stackedWidget->currentIndex()));
    Text += "MultiSection,"+QString::number(ui->stackedWidget->currentIndex());
    if(ui->FilePathLine->text().isEmpty() || ui-> FileNameLine->text().isEmpty()){
        QMessageBox::warning(this,tr("Error"),tr("No file created yet."),QMessageBox::Ok);
        return;
    }
    //Latex file Metadata - Write new entry to database ---
    QString meta_Ids = mapIdsNames.keys().join("\",\"");
    QString meta_Values = mapIdsNames.values().join("\",\"");
    QStringList WriteValues;
    foreach(QString section,SectionList){
        WriteValues.append("(\""+meta_Values+"\",\""+section+"\")");
    }
    //-------------------
    QSqlQuery writeExercise(currentbase);
    if(!EditMode){
        writeExercise.exec("INSERT INTO \"Database_Files\" (\""+meta_Ids+"\",\"Section\") VALUES "+WriteValues.join(","));
    }
    else{
//        writeExercise.exec("INSERT INTO \"Database_Files\" (\""+meta_Ids+"\",\"Section\") VALUES "+WriteValues.join(","));
        QStringList list;
        foreach(QString section,SectionList){
            QString query ="UPDATE \"Database_Files\" SET ";
            list.clear();
            for (int i=0;i<mapIdsNames.keys().count();i++) {
                list.append("\""+mapIdsNames.keys()[i]+"\""+"="+"\""+mapIdsNames.values()[i]+"\"");
            }
            list.append("\"Section\"=\""+section+"\"");
            query += list.join(",")+" WHERE \"Id\"=\""+mapIdsNames["Id"]+"\" AND \"Section\"=\""+section+"\"";
            writeExercise.exec(query);
        }
    }
    //Create a backup csv file and store metadata
    QString csvFile = fileName;
    csvFile.replace(".tex",".csv");
    QFile CSV(csvFile);
    CSV.open (QIODevice::ReadWrite | QIODevice::Text);
    QTextStream CsvContent(&CSV);
    CsvContent << Text;
    CSV.close();

    if(CloneMode){
        emit acceptClone(ui->DatabaseCombo->currentData().toString(),fileName,FileContent);
    }
    else {
        emit acceptSignal(fileName,/*mapIdsNames,SectionList,*/FileContent);
    }
    accept();
}

void NewDatabaseFile::on_buttonBox_rejected()
{
    reject();
}

void NewDatabaseFile::on_addField_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddField(currentbase);
        AddField.prepare(QString("INSERT INTO \"Fields\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")").arg(Line[1],Line[0]));
        if(AddField.exec()){
            int rows = ui->FieldTable->count();
            ui->FieldTable->addItem(Line[0]);
            ui->FieldTable->item(rows)->setData(Qt::UserRole,Line[1]);
            ui->FieldTable->setCurrentRow(rows);
            on_FieldTable_itemClicked(ui->FieldTable->itemAt(rows,0));
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
    AddDatabaseField * newData = new AddDatabaseField(this);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddChapter(currentbase);
        AddChapter.prepare(QString("INSERT INTO \"Chapters\" (\"Id\",\"Name\",\"Field\") VALUES(\"%1\",\"%2\",\"%3\")")
                      .arg(Line[1],Line[0],ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));

        if(AddChapter.exec()){
            ui->SectionExercisesChapterComboBox->addItem(Line[0],QVariant(Line[1]));
            ui->SectionExercisesChapterComboBox->setCurrentIndex(ui->SectionExercisesChapterComboBox->count()-1);
            on_SectionExercisesChapterComboBox_activated(ui->SectionExercisesChapterComboBox->count()-1);
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
    RemoveField.exec(QString("DELETE FROM \"Fields\" WHERE \"Id\" = \"%1\"").arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    delete ui->FieldTable->takeItem(ui->FieldTable->currentRow());
    ui->FieldTable->setCurrentRow(0);
    on_FieldTable_itemClicked(ui->FieldTable->item(0));
}


void NewDatabaseFile::on_removeChapter_clicked()
{
    QSqlQuery RemoveChapter(currentbase);
    RemoveChapter.exec("PRAGMA foreign_keys = ON");
    RemoveChapter.exec(QString("DELETE FROM \"Chapters\" WHERE \"Id\" = \"%1\" AND \"Field\" = \"%2\"")
                     .arg(ui->SectionExercisesChapterComboBox->currentData().toString()
                          ,ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    ui->SectionExercisesChapterComboBox->removeItem(ui->SectionExercisesChapterComboBox->currentIndex());
    ui->SectionExercisesChapterComboBox->setCurrentIndex(0);
    on_SectionExercisesChapterComboBox_activated(0);
}


void NewDatabaseFile::on_addSection_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        QSqlQuery AddSection(currentbase);
        AddSection.prepare(QString("INSERT INTO \"Sections\" (\"Id\",\"Name\",\"Field\",\"Chapter\") VALUES(\"%1\",\"%2\",\"%3\",\"%4\")")
                        .arg(Line[1],Line[0],ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                ui->SectionExercisesChapterComboBox->currentData().toString()));
        if(AddSection.exec()){
            ui->SectionExercisesSectionComboBox->addItem(Line[0],QVariant(Line[1]));
            ui->SectionExercisesSectionComboBox->setCurrentIndex(ui->SectionExercisesSectionComboBox->count()-1);
            on_SectionExercisesSectionComboBox_activated(ui->SectionExercisesSectionComboBox->count()-1);
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
    RemoveSection.exec(QString("DELETE FROM \"Sections\" WHERE \"Id\" = \"%1\" AND \"Field\" = \"%2\"")
                       .arg(ui->SectionExercisesSectionComboBox->currentData().toString(),
                            ui->FieldTable->currentItem()->data(Qt::UserRole).toString()));
    ui->SectionExercisesSectionComboBox->removeItem(ui->SectionExercisesChapterComboBox->currentIndex());
    ui->SectionExercisesSectionComboBox->setCurrentIndex(0);
    on_SectionExercisesSectionComboBox_activated(0);
}

void NewDatabaseFile::on_addExType_clicked()
{
    AddDatabaseField * newData = new AddDatabaseField(this);
    connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
        if(FileType == "SectEx"){
            QSqlQuery AddSection1(currentbase);
            QSqlQuery AddSection2(currentbase);
            AddSection1.prepare(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")").arg(Line[1],Line[0]));
            AddSection2.prepare(QString("INSERT OR IGNORE INTO \"Sections_Exercises\" (\"Exercise_Id\",\"Exercise_Name\",\"Section_Id\") VALUES(\"%1\",\"%2\",\"%3\")")
                            .arg(Line[1],Line[0],ui->SectionExercisesSectionComboBox->currentData().toString()));
            if(AddSection1.exec() && AddSection2.exec()){
                ui->SectionExercisesExTypeComboBox->addItem(Line[0],QVariant(Line[1]));
                ui->SectionExercisesExTypeComboBox->setCurrentIndex(ui->SectionExercisesExTypeComboBox->count()-1);
                on_SectionExercisesExTypeComboBox_activated(ui->SectionExercisesSectionComboBox->count()-1);
            }
            else{
                QMessageBox::warning(this,tr("Error"),AddSection2.lastError().text(),QMessageBox::Ok);
            }
        }
        else if(FileType == "SectSub"){
            QSqlQuery AddSubjectType(currentbase);
            AddSubjectType.exec("PRAGMA foreign_keys = ON");
            AddSubjectType.prepare(QString("INSERT INTO \"Subject_Types\" (\"Id\",\"Name\") VALUES(:id,:name)"));
            AddSubjectType.bindValue(":id",Line[1]);
            AddSubjectType.bindValue(":name",Line[0]);
            if(AddSubjectType.exec()){
                QSqlQuery AddExType(currentbase);
                AddExType.exec("PRAGMA foreign_keys = ON");
                AddExType.prepare(QString("INSERT INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(:id,:name)"));
                AddExType.bindValue(":id",Line[1]);
                AddExType.bindValue(":name",Line[0]);
                AddExType.exec();
                ui->SectionExercisesExTypeComboBox->addItem(Line[0],QVariant(Line[1]));
                ui->SectionExercisesExTypeComboBox->setCurrentText(Line[0]);
                on_SectionExercisesExTypeComboBox_activated(ui->SectionExercisesSectionComboBox->count()-1);
            }
            else{
                QMessageBox::warning(this,tr("Error"),AddSubjectType.lastError().text(),QMessageBox::Ok);
            }
        }
    });
    newData->show();
    newData->activateWindow();
}

void NewDatabaseFile::on_removeExType_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete exercise type",tr("The exercise type %1 will be deleted!\nDo you wish to proceed?").arg(ui->SectionExercisesExTypeComboBox->currentData().toString()),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        if(FileType == "SectEx"){
            QSqlQuery RemoveExerciseType(currentbase);
            RemoveExerciseType.exec(QString("DELETE FROM \"Sections_Exercises\" WHERE \"Exercise_Id\" = \"%1\" AND \"Section_Id\" = \"%2\"")
                             .arg(ui->SectionExercisesExTypeComboBox->currentData().toString(),
                                  ui->SectionExercisesSectionComboBox->currentData().toString()));
        }
        else if(FileType == "SectSub"){
            QSqlQuery RemoveDocumentType(currentbase);
            RemoveDocumentType.exec("PRAGMA foreign_keys = ON");
            RemoveDocumentType.exec(QString("DELETE FROM \"Subject_Types\" WHERE \"Name\" = \"%1\"")
                            .arg(ui->SectionExercisesExTypeComboBox->currentText()));
        }
        ui->SectionExercisesExTypeComboBox->removeItem(ui->SectionExercisesChapterComboBox->currentIndex());
        ui->SectionExercisesExTypeComboBox->setCurrentIndex(0);
        on_SectionExercisesExTypeComboBox_activated(0);
    }
}

void NewDatabaseFile::on_addFileType_clicked()
{
    NewFileType * newFile = new NewFileType(this);
    connect(newFile,&NewFileType::filedata,this,[=](QStringList data){
        QRadioButton * newButton = new QRadioButton(data[1],this);
        ui->gridLayout_10->removeItem(ui->gridLayout_8);
        ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
        newButton->setProperty("Name",data[0]);
        CustomFileTypesList.append(newButton);
        ui->FileTypeGroup->addButton(newButton);
        connect(newButton, &QRadioButton::toggled, this,
                [=](){
            FileTypeClicked();
        });
        ui->gridLayout_10->addWidget(newButton,ui->gridLayout_10->rowCount(),0);
        ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
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
    SectionList.clear();
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    FileType = button->property("Name").toString();
    ui->FieldTable->setEnabled(true);
    ui->addField->setEnabled(true);
    ui->addChapter->setEnabled(false);
    ui->addSection->setEnabled(false);
    ui->addExType->setEnabled(false);
    ui->removeField->setEnabled(true);
    ui->removeChapter->setEnabled(false);
    ui->removeSection->setEnabled(false);
    ui->removeExType->setEnabled(false);
    ui->FieldTable->clear();
    ui->SectionExercisesChapterComboBox->clear();
    ui->SectionExercisesSectionComboBox->clear();
    ui->SectionExercisesExTypeComboBox->clear();
    ui->SectionExercisesChapterComboBox->setEnabled(false);
    ui->SectionExercisesSectionComboBox->setEnabled(false);
    ui->SectionExercisesExTypeComboBox->setEnabled(false);
    for (int item=0;item<Field_Names.count();item++ ) {
        ui->FieldTable->addItem(Field_Names.at(item));
        ui->FieldTable->item(item)->setData(Qt::UserRole,Field_ids.at(item));
    }
    ui->FilePathLine->clear();
    ui->FileNameLine->clear();
    ui->NewFileContentText->clear();
    ui->SectionList->clear();
    ui->SelectedSections->clear();
    if(FileType == "CombEx" || FileType == "CombSub"){
        ui->stackedWidget->setCurrentIndex(1);
    }
    else{
        ui->stackedWidget->setCurrentIndex(0);
    }
    ui->SingleSection->setEnabled(false);
    ui->MultiSection->setEnabled(false);

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
    ui->SingleSection->setEnabled(CustomFileTypesList.contains(button));
    ui->MultiSection->setEnabled(CustomFileTypesList.contains(button));
    ui->UseExerciseType->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
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
    ui->SectionExerciseButton->setEnabled(clone);
    ui->SectionSubjectButton->setEnabled(clone);
    ui->CombExerciseButton->setEnabled(clone);
    ui->CombSubjectButton->setEnabled(clone);
    ui->MethodButton->setEnabled(clone);
    ui->ExampleButton->setEnabled(clone);
    foreach(QAbstractButton * bt,CustomFileTypesList){
        bt->setEnabled(clone);
    }
}

