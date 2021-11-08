#include "newdatabasefile.h"
#include "ui_newdatabasefile.h"
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QRadioButton>
#include "adddatabasefield.h"
#include "sqlfunctions.h"
#include "datatex.h"

NewDatabaseFile::NewDatabaseFile(QWidget *parent,QStringList meta,QStringList sections,bool editMode) :
    QDialog(parent),
    ui(new Ui::NewDatabaseFile)
{
    ui->setupUi(this);
    EditMode = editMode;
    ui->addButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
    ui->SectionExercisesChapterComboBox->setEnabled(false);
    ui->SectionExercisesSectionComboBox->setEnabled(false);
    ui->SectionExercisesExTypeComboBox->setEnabled(false);
    ui->NewFileContentText->setEnabled(false);
    ui->DefinitionButton->setProperty("Name","Definition");
    ui->TheoremButton->setProperty("Name","Theorem");
    ui->FigureButton->setProperty("Name","Figure");
    ui->TableButton->setProperty("Name","Table");
    ui->SectionExerciseButton->setProperty("Name","SectEx");
    ui->SectionSubjectButton->setProperty("Name","SectSub");
    ui->CombExerciseButton->setProperty("Name","CombEx");
    ui->CombSubjectButton->setProperty("Name","CombSub");
    ui->MethodButton->setProperty("Name","Method");
    ui->ExampleButton->setProperty("Name","Example");
    ui->FieldTable->setEnabled(false);
    ui->addField->setEnabled(false);
    ui->addChapter->setEnabled(false);
    ui->addSection->setEnabled(false);
    ui->addExType->setEnabled(false);
    ui->removeField->setEnabled(false);
    ui->removeChapter->setEnabled(false);
    ui->removeSection->setEnabled(false);
    ui->removeExType->setEnabled(false);
    DataBase_Path = QFileInfo(DataTex::CurrentDataBasePath).absolutePath()+QDir::separator();
    currentbase = DataTex::CurrentTexFilesDataBase;
    metadata = meta;
    InportedSectionList = sections;
    QStringList Field_Names;
    QStringList Field_ids;
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_Names.append(fields.value(0).toString());
        Field_ids.append(fields.value(1).toString());
    }

    QSqlQuery FileTypes(currentbase);
    FileTypes.exec("SELECT \"FileType\",\"Id\",\"FolderName\" FROM \"FileTypes\"");
    int line = -1;
    ui->gridLayout_10->removeItem(ui->gridLayout_8);
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
    ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
    ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
    ui->removeFileType->setEnabled(false);

    for(int i=0;i<ui->FileTypeGroup->buttons().count();i++){
        connect(ui->FileTypeGroup->buttons().at(i), &QRadioButton::toggled, this,
                [=](){
            SectionList.clear();
            acceptNewFile = false;
            ui->BuildBox->setEnabled(false);
            ui->PreambleBox->setEnabled(false);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
            FileType = ui->FileTypeGroup->buttons().at(i)->property("Name").toString();
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

            QStringList Exercises = {"SectEx","SectSub","CombEx","CombSub"};
            if(Exercises.contains(FileType)){
                ui->DifficultySpin->setEnabled(true);
                ui->DifficultySpin->setMinimum(1);
                ui->DifficultySpin->setValue(1);
            }
            else{
                ui->DifficultySpin->setMinimum(0);
                ui->DifficultySpin->setValue(0);
                ui->DifficultySpin->setEnabled(false);
            }
            ui->removeFileType->setEnabled(CustomFileTypesList.contains(ui->FileTypeGroup->buttons().at(i)));
            ui->SingleSection->setEnabled(CustomFileTypesList.contains(ui->FileTypeGroup->buttons().at(i)));
            ui->MultiSection->setEnabled(CustomFileTypesList.contains(ui->FileTypeGroup->buttons().at(i)));
        });
    }

    TheoryView = new QPdfViewer(this);
    ui->verticalLayout_5->addWidget(TheoryView);
    ui->splitter_2->setSizes(QList<int>({1,1, 400}));
    TheoryView->show();
    acceptNewFile = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
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
    if(EditMode){
        EditModeIsEnabled();
    }
    ui->splitter_2->setSizes(QList<int>({150, 300,350}));
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
}

NewDatabaseFile::~NewDatabaseFile()
{
    delete ui;
}

void NewDatabaseFile::EditModeIsEnabled()
{
    QStringList multisection = {"CombEx","CombSub"};
    QString filetype = metadata.last();
    for (int i=0;i<ui->FileTypeGroup->buttons().count();i++) {
        if(ui->FileTypeGroup->buttons().at(i)->property("Name") == filetype){
            ui->FileTypeGroup->buttons().at(i)->setChecked(true);
        }
    }
    ui->FieldTable->setCurrentItem(ui->FieldTable->findItems(metadata[1],Qt::MatchExactly).at(0));
    on_FieldTable_itemClicked(ui->FieldTable->findItems(metadata[1],Qt::MatchExactly).at(0));
    if(multisection.contains(filetype)){
        for(int i=0;i<InportedSectionList.count();i++){
            on_SectionList_itemDoubleClicked(ui->SectionList->findItems(InportedSectionList[i],Qt::MatchExactly).at(0));
        }
    }
    else{
        int chap = ui->SectionExercisesChapterComboBox->findText(metadata[2]);
        if ( chap != -1 ) {
            ui->SectionExercisesChapterComboBox->setCurrentIndex(chap);
            on_SectionExercisesChapterComboBox_currentIndexChanged(chap);
        }
        int sect = ui->SectionExercisesSectionComboBox->findText(InportedSectionList[0]);
        if ( sect != -1 ) {
            ui->SectionExercisesSectionComboBox->setCurrentIndex(sect);
            on_SectionExercisesSectionComboBox_currentIndexChanged(sect);
        }
        int extype = ui->SectionExercisesExTypeComboBox->findText(metadata[3]);
        if ( extype != -1 ) {
            ui->SectionExercisesExTypeComboBox->setCurrentIndex(extype);
            on_SectionExercisesExTypeComboBox_currentIndexChanged(extype);
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
    if (FileType == "CombSub" || FileType == "CombEx"){
        Path = DataBase_Path+Field+QDir::separator()+folderName+QDir::separator()+FolderName+QDir::separator();
    }
    else {
        Path = DataBase_Path+datalist.join(QDir::separator())+QDir::separator();
    }
    QString fileId;
    if (FileType == "CombSub" || FileType == "CombEx"){
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

void NewDatabaseFile::loadImageFile(QString exoFile, QPdfViewer *view)
{
    if (exoFile.isEmpty()) {
        return;}
    QString pdfFile = exoFile.replace(".tex",".pdf");
    if(QFileInfo::exists(pdfFile)){
        view->open(QUrl::fromLocalFile(pdfFile));
    }
}

void NewDatabaseFile::on_FieldTable_itemClicked(QListWidgetItem *item)
{
    ui->NewFileContentText->setEnabled(true);
    QString Id = item->data(Qt::UserRole).toString();
    if(FileType != "CombEx" && FileType != "CombSub"){
        ui->SectionExercisesChapterComboBox->setEnabled(true);
        ui->addChapter->setEnabled(true);
        ui->removeChapter->setEnabled(true);
        ui->SectionExercisesChapterComboBox->clear();
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
    }
    for (int c = 0; c < ui->ExerciseFileList->horizontalHeader()->count()-1; ++c)
    {
        ui->ExerciseFileList->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
    ui->ExerciseFileList->setColumnHidden(2,true);
    connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
    ui->SectionExercisesSectionComboBox->setEnabled(false);
    ui->addSection->setEnabled(false);
    ui->removeSection->setEnabled(false);
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

void NewDatabaseFile::on_SectionExercisesChapterComboBox_currentIndexChanged(int index)
{
    if(index>-1){
        ui->SectionExercisesSectionComboBox->setEnabled(true);
        ui->addSection->setEnabled(true);
        ui->removeSection->setEnabled(true);
        ui->SectionExercisesSectionComboBox->clear();
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
        connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
        ui->FileNameLine->clear();
        ui->FilePathLine->clear();
        ui->NewFileContentText->clear();
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->SectionExercisesExTypeComboBox->setEnabled(false);
    ui->addExType->setEnabled(false);
    ui->removeExType->setEnabled(false);
}

void NewDatabaseFile::on_SectionExercisesSectionComboBox_currentIndexChanged(int index)
{
    ui->SectionExercisesExTypeComboBox->clear();
    if(index>-1){
        if(FileType == "SectEx"){
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
            acceptNewFile = false;
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
        }
        else if(FileType == "SectSub"){
            ui->SectionExercisesExTypeComboBox->setEnabled(true);
            ui->addExType->setEnabled(true);
            ui->removeExType->setEnabled(true);
            ui->SectionExercisesExTypeComboBox->clear();
            QList<QStringList> data = SqlFunctions::ComboList_Double_List(
                        SqlFunctions::GetSubject_Types,currentbase);
            for (int item=0;item<data[0].count() ;item++ )
            {
                ui->SectionExercisesExTypeComboBox->addItem(data[0].at(item), QVariant(data[1].at(item)));
            }
            ui->SectionExercisesExTypeComboBox->setCurrentIndex(-1);
            acceptNewFile = false;
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
        }
        connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
        if(FileType != "SectEx" && FileType != "SectSub"){
            acceptNewFile = true;
            QString Path = NewFilePathAndId();
            ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
            ui->FileNameLine->setText(QFileInfo(Path).fileName());
            ui->NewFileContentText->setText(DataTex::NewFileText(Path));
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
        }
    }
}

void NewDatabaseFile::on_SectionExercisesExTypeComboBox_currentIndexChanged(int index)
{
    if(index!=-1){
        ui->SectionExerciseButton->setEnabled(true);
        updateTableView(ui->ExerciseFileList,SqlFunctions::UpdateTableFiles.
                        arg(ui->FieldTable->currentItem()->data(Qt::UserRole).toString(),
                            ui->SectionExercisesChapterComboBox->currentData().toString(),
                            ui->SectionExercisesSectionComboBox->currentData().toString(),
                            ui->SectionExercisesExTypeComboBox->currentData().toString(),FileType));
        connect(ui->ExerciseFileList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NewDatabaseFile::ExerciseFileList_selection_changed);
        acceptNewFile = true;
        QString Path = NewFilePathAndId();
        ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
        ui->FileNameLine->setText(QFileInfo(Path).fileName());
        ui->NewFileContentText->setText(DataTex::NewFileText(Path));
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
    }
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
        acceptNewFile = true;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
    }
    else if(SectionCount==0){
        ui->SectionList->removeItemWidget(widget);
        ui->SelectedSections->addItem(widget);
        ui->addButton->setEnabled(false);
        acceptNewFile = false;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
    }
    ui->SelectedSections->sortItems();
    QString Path = NewFilePathAndId();
    ui->FilePathLine->setText(QFileInfo(Path).absolutePath()+QDir::separator());
    ui->FileNameLine->setText(QFileInfo(Path).fileName());
    ui->NewFileContentText->setText(DataTex::NewFileText(Path));
}

void NewDatabaseFile::on_addButton_clicked()
{
    on_SectionList_itemDoubleClicked(ui->SectionList->currentItem());
    emit listChanged();
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
        ui->NewFileContentText->setText(DataTex::NewFileText(Path));
        ui->NewFileContentText->setText(DataTex::NewFileText(Path));
    }
    else if(SelectedSections==0){
        ui->SelectedSections->removeItemWidget(currentItem);
        ui->SectionList->addItem(currentItem);
        ui->SectionList->sortItems();
        ui->removeButton->setEnabled(false);
        ui->FilePathLine->clear();
        ui->FileNameLine->clear();
        ui->NewFileContentText->clear();
        acceptNewFile = false;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptNewFile);
    }
    emit listChanged();
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
        DataTex::CreateTexFile(FilePath);
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
    if(FileType != "CombEx" && FileType != "CombSub"){
        ChapterId = ui->SectionExercisesChapterComboBox->currentData().toString();
        SectionList.append(ui->SectionExercisesSectionComboBox->currentData().toString());
        if(!ui->SectionExercisesExTypeComboBox->currentText().isEmpty()){
            ExType = ui->SectionExercisesExTypeComboBox->currentData().toString();
        }
        else{
            ExType = "-";
        }
    }
    else if(FileType == "CombEx" || FileType == "CombSub"){
        ChapterId ="-";
        ExType = "-";
    }
    QStringList Exercises = {"SectEx","SectSub","CombEx","CombSub"};
    if(Exercises.contains(FileType)){
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
    Content.replace("\n","\\n");
    Text += "FileContent,"+Content+"\n";
    mapIdsNames.insert("Preamble",ui->PreambleBox->currentData().toString());
    Text += "Preamble,"+ui->PreambleBox->currentData().toString()+"\n";
    mapIdsNames.insert("BuildCommand",ui->BuildBox->currentText());
    Text += "BuildCommand,"+ui->BuildBox->currentText()+"\n";
    mapIdsNames.insert("FileDescription",ui->DescriptionLine->text());
    Text += "FileDescription,"+ui->DescriptionLine->text();

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
    QSqlQuery writeExercise(DataTex::CurrentTexFilesDataBase);
    writeExercise.exec("INSERT INTO \"Database_Files\" (\""+meta_Ids+"\",\"Section\") VALUES "+WriteValues.join(","));
    qDebug()<<mapIdsNames;
    //Create a backup csv file and store metadata
    QString csvFile = fileName;
    csvFile.replace(".tex",".csv");

    QFile CSV(csvFile);
    CSV.open (QIODevice::ReadWrite | QIODevice::Text);
    QTextStream CsvContent(&CSV);
    CsvContent << Text;
    CSV.close();

    //Bibliography entry--------------------------------------------------
    QSqlQuery Select_DataBase_Bibliography(DataTex::DataTeX_Settings);
    Select_DataBase_Bibliography.exec(QString(SqlFunctions::GetBibliographyFields)
                                           .arg(QFileInfo(DataTex::CurrentDataBasePath).baseName()));
    while(Select_DataBase_Bibliography.next()){
        Bibliography_Ids.append(Select_DataBase_Bibliography.value(0).toString());
    }
    Bibliography_Ids.removeAt(0);
    QString query = "INSERT INTO \"Bibliography\" (\"Citation_Key\",\""+Bibliography_Ids.join("\",\"")+"\") VALUES(\"%1\","
                    +QString("\"\",").repeated(Bibliography_Ids.count()-1)+"'')";
    QSqlQuery BibliographyQuery(currentbase);
    BibliographyQuery.exec(QString(query).arg(QFileInfo(fileName).baseName()));
    emit acceptSignal(fileName,/*mapIdsNames,SectionList,*/FileContent);
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
            ui->SectionExercisesChapterComboBox->setCurrentText(Line[0]);
            on_SectionExercisesChapterComboBox_currentIndexChanged(ui->SectionExercisesChapterComboBox->count()-1);
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
    on_SectionExercisesChapterComboBox_currentIndexChanged(0);
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
            ui->SectionExercisesSectionComboBox->setCurrentText(Line[0]);
            on_SectionExercisesSectionComboBox_currentIndexChanged(ui->SectionExercisesSectionComboBox->count()-1);
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
    on_SectionExercisesSectionComboBox_currentIndexChanged(0);
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
                ui->SectionExercisesExTypeComboBox->setCurrentText(Line[0]);
                on_SectionExercisesExTypeComboBox_currentIndexChanged(ui->SectionExercisesSectionComboBox->count()-1);
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
                on_SectionExercisesExTypeComboBox_currentIndexChanged(ui->SectionExercisesSectionComboBox->count()-1);
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
        on_SectionExercisesExTypeComboBox_currentIndexChanged(0);
    }
}

void NewDatabaseFile::on_addFileType_clicked()
{
    QStringList Ids = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM FileTypes",DataTex::CurrentTexFilesDataBase);
    QStringList Names = SqlFunctions::Get_StringList_From_Query("SELECT FileType FROM FileTypes",DataTex::CurrentTexFilesDataBase);
    QStringList Folders = SqlFunctions::Get_StringList_From_Query("SELECT FolderName FROM FileTypes",DataTex::CurrentTexFilesDataBase);

    QDialog * NameDialog = new QDialog(this);
    QDialogButtonBox * OkButton = new QDialogButtonBox(this);
    QLineEdit * IdLine = new QLineEdit(this);
    QLineEdit * NameLine = new QLineEdit(this);
    QLineEdit * FolderLine = new QLineEdit(this);
    QGridLayout * layout = new QGridLayout(this);
    QLabel * label1 = new QLabel(tr("Id"),this);
    QLabel * label2 = new QLabel(tr("Name"),this);
    QLabel * label3 = new QLabel(tr("Folder name"),this);
    QLabel * warning = new QLabel(this);
    OkButton->addButton(QDialogButtonBox::Ok);
    OkButton->addButton(QDialogButtonBox::Cancel);
    layout->addWidget(label1,0,0);
    layout->addWidget(label2,0,1);
    layout->addWidget(label3,2,0);
    layout->addWidget(IdLine,1,0);
    layout->addWidget(NameLine,1,1);
    layout->addWidget(FolderLine,3,0,1,2);
    layout->addWidget(warning,4,0);
    layout->addWidget(OkButton,4,1);
    NameDialog->setLayout(layout);
    NameDialog->resize(350,100);
    NameDialog->setWindowTitle("Select a name/desctription for tis database");
    connect(OkButton,&QDialogButtonBox::accepted,this,[=](){
        QStringList Line = {IdLine->text(),NameLine->text(),FolderLine->text()};
        if(!NameLine->text().isEmpty()){
            QRadioButton * newButton = new QRadioButton(Line[0],this);
            ui->gridLayout_10->removeItem(ui->gridLayout_8);
            ui->gridLayout_10->removeItem(ui->verticalSpacer_2);
            newButton->setProperty("Name",Line[1]);
            CustomFileTypesList.append(newButton);
            ui->FileTypeGroup->addButton(newButton);
            ui->gridLayout_10->addWidget(newButton,ui->gridLayout_10->rowCount(),0);
            ui->gridLayout_10->addItem(ui->gridLayout_8,ui->gridLayout_10->rowCount(),0);
            ui->gridLayout_10->addItem(ui->verticalSpacer_2,ui->gridLayout_10->rowCount(),0);
            QSqlQuery NewFileType(DataTex::CurrentTexFilesDataBase);
            NewFileType.prepare(QString("INSERT OR IGNORE INTO \"FileTypes\" (\"Id\",\"FileType\",\"FolderName\") VALUES(:id,:name,:folder)"));
            NewFileType.bindValue(":id",Line[0]);
            NewFileType.bindValue(":name",Line[1]);
            NewFileType.bindValue(":folder",Line[2]);
            NewFileType.exec();
            NameDialog->close();
        }
    });
    NameDialog->setMinimumWidth(350);
    connect(OkButton,SIGNAL(rejected()),NameDialog,SLOT(reject()));
    connect(NameLine,&QLineEdit::textChanged,NameDialog,[=](){
        if(Names.contains(NameLine->text())){
            warning->setText("This name already exists");
            OkButton->setEnabled(false);
        }
        else{
            warning->setText("");
            OkButton->setEnabled(true);
        }
    });
    connect(IdLine,&QLineEdit::textChanged,NameDialog,[=](){
        if(Ids.contains(IdLine->text())){
            warning->setText("This id already exists");
            OkButton->setEnabled(false);
        }
        else{
            warning->setText("");
            OkButton->setEnabled(true);
        }
    });
    connect(FolderLine,&QLineEdit::textChanged,NameDialog,[=](){
        if(Folders.contains(FolderLine->text())){
            warning->setText("This folder already exists");
            OkButton->setEnabled(false);
        }
        else{
            warning->setText("");
            OkButton->setEnabled(true);
        }
    });
    NameDialog->exec();
}


void NewDatabaseFile::on_removeFileType_clicked()
{

}

