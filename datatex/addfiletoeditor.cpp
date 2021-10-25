#include "addfiletoeditor.h"
#include "ui_addfiletoeditor.h"
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QDirIterator>
#include <QVector>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QProcess>
#include <QTextBlock>
#include <QTextCursor>
#include "datatex.h"



AddFileToEditor::AddFileToEditor(QWidget *parent,QString currentTexFile, QString BuildCommand) :
    QDialog(parent),
    ui(new Ui::AddFileToEditor)
{
    ui->setupUi(this);

    CurrentDatabaseFile = currentTexFile;
    currentbase = DataTex::CurrentTexFilesDataBase;
    datalist.append(DataTex::CurrentTexFilesDataBase);
    CurrentBuildCommand = BuildCommand;
    qDebug()<<datalist;

    QStringList ListOfDatabasesPaths =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Path FROM Databases"),DataTex::DataTeX_Settings);
    QStringList ListOfDatabasesNames =
            SqlFunctions::Get_StringList_From_Query(QString("SELECT Name FROM Databases"),DataTex::DataTeX_Settings);
    int current = 0;
    for (int i=0;i<ListOfDatabasesPaths.count();i++) {
            if(ListOfDatabasesPaths.at(i)!=DataTex::CurrentDataBasePath){
                QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE",QFileInfo(ListOfDatabasesPaths.at(i)).baseName());
                database.setDatabaseName(ListOfDatabasesPaths.at(i));
                database.open();
                datalist.append(database);
                qDebug()<<datalist;
            }
        ui->FilesDatabasesCombo->addItem(ListOfDatabasesNames.at(i),QVariant(ListOfDatabasesPaths.at(i)));
        if(ListOfDatabasesPaths.at(i) == DataTex::CurrentDataBasePath){current = i;}
    }
    ui->FilesDatabasesCombo->setCurrentIndex(current);
//    currentbase = datalist.at(ui->FilesDatabasesCombo->currentIndex());
    ui->removeButton->setEnabled(false);
    ui->UpButton->setEnabled(false);
    ui->DownButton->setEnabled(false);
    ui->Save->setEnabled(false);
    ui->CurrentFileLabel->setText(tr("Insert files to document : ")+QFileInfo(CurrentDatabaseFile).fileName());

    view = new QPdfViewer(this);
    view->setMinimumWidth(620);
    ui->gridLayout_17->addWidget(view,1,0);
    view->show();
    DocView = new PdfViewer(this);
    ui->verticalLayout_4->addWidget(DocView);
    DocView->show();

    FilesTable = new ExtendedTableWidget(this);
    ui->gridLayout_16->addWidget(FilesTable,3,0);
    FilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    FilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    FilesTable->horizontalHeader()->setSectionsClickable(true);

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    Database_FileTableFieldNames = SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",DataTex::CurrentTexFilesDataBase);
    LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);

    ui->FilesDatabasesCombo->setEnabled(false);
    ui->splitter->setSizes(QList<int>({1,200}));
    ui->splitter_2->setSizes(QList<int>({1,300}));

    QString FileContent;
    QFile file(CurrentDatabaseFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
    ui->DocumentContent->setText(FileContent);

    connect(FilesTable,&QTableView::doubleClicked,this,[=](){on_addButton_clicked();});
    ui->tabWidget->setCornerWidget(ui->ToolBar);
    DataTex::loadImageFile(CurrentDatabaseFile,DocView);
    ui->Save->setEnabled(false);
    connect(ui->DocumentContent, &QTextEdit::textChanged, this,[=](){ui->Save->setEnabled(true);});
}

AddFileToEditor::~AddFileToEditor()
{
    delete ui;
    delete view;
    delete DocView;
    for (int i=0;i<datalist.count();i++) {
        if(datalist[i].databaseName()!=DataTex::CurrentDataBasePath){
            datalist[i].close();
        }
    }
}

QList<QStringList> AddFileToEditor::getDatabaseFields(QSqlDatabase database)
{
    QList<QStringList> list;
    list.append(SqlFunctions::Get_StringList_From_Query("SELECT \"Id\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",database));
    list.append(SqlFunctions::Get_StringList_From_Query("SELECT \"Name\" FROM \"BackUp\" WHERE \"Table_Id\" = 'Metadata'",database));
    return list;
}

void AddFileToEditor::LoadDatabaseFiles(QSqlDatabase database,QString query)
{
    QSqlQueryModel * DatabaseModel = new QSqlQueryModel(this);
    QSqlQuery DatabaseQuery(database);
    DatabaseQuery.exec(query);
    DatabaseModel->setQuery(DatabaseQuery);
    FilesTable->setModel(DatabaseModel);
    FilesTable->show();
    int columns = FilesTable->model()->columnCount();
    FilesTable->generateFilters(columns,false);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &AddFileToEditor::FilesTable_selectionchanged);
    connect(FilesTable->filterHeader(), &FilterTableHeader::filterValues, this, &AddFileToEditor::updateFilter);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void AddFileToEditor::updateFilter(/*size_t column, const QString& value*/QStringList values)
{
    SqlFunctions::FilesTable_UpdateQuery.clear();
    DataTex::FilterTables_Queries(Database_FileTableFields);
    int columns = Database_FileTableFields.count();
    for (int i=0;i<columns;i++) {
        SqlFunctions::FilesTable_UpdateQuery.replace("replace"+Database_FileTableFields.at(i),values.at(i));
    }
    FilesTable->setColumnHidden(columns,true);
    DataTex::updateTableView(FilesTable,SqlFunctions::FilesTable_UpdateQuery,currentbase,this);
    connect(FilesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AddFileToEditor::FilesTable_selectionchanged);
    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

//void AddFileToEditor::on_FieldListTheory_itemClicked(QListWidgetItem *item)
//{
//    ui->DefinitionsBox->setEnabled(true);
//    ui->TheoremsBox->setEnabled(true);
//    ui->TablesBox->setEnabled(true);
//    ui->FiguresBox->setEnabled(true);
//    ui->FileList->clear();
//    if(ui->buttonGroup->checkedButton() != nullptr)
//    {
//       ui->buttonGroup->setExclusive(false);
//       ui->buttonGroup->checkedButton()->setChecked(false);
//       ui->buttonGroup->setExclusive(true);
//    }
//}

//void AddFileToEditor::on_ComboChapterTheory_currentIndexChanged(int index)
//{
//    ui->comboSectionTheory->clear();
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Single(SqlFunctions::Sections_Chapters_Query,currentbase,
//                                           ui->ComboChapterTheory->currentText());
//    for (int item=0;item<data[0].count() ;item++ ) {
//    ui->comboSectionTheory->addItem(data[0].at(item),QVariant(data[1].at(item)));
//    }
//    ui->comboSectionTheory->setCurrentIndex(-1);
//}

//void AddFileToEditor::on_comboSectionTheory_currentIndexChanged(int index)
//{
//    ui->FileList->clear();
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Double_List(SqlFunctions::GetTheoryFiles
//            .arg(ui->FieldListTheory->currentItem()->data(Qt::UserRole).toString(),FileType
//                 ,ui->ComboChapterTheory->currentData().toString(),
//                 ui->comboSectionTheory->currentData().toString()),currentbase);
//    for(int row=0;row<data[0].count();row++)
//    {
//        ui->FileList->addItem(data[0].at(row));
//        ui->FileList->item(row)->setData(Qt::UserRole,data[1].at(row));
//    }
//}

//void AddFileToEditor::on_TheoryCheck_checked(bool checked)
//{
//    QCheckBox *btn = static_cast<QCheckBox *>(sender());
//    FileType = btn->property("Name").toString();
//    ui->ComboChapterTheory->clear();
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Single(SqlFunctions::Chapters_Query,currentbase,
//                                           ui->FieldListTheory->currentItem()->text());
//    for (int item=0;item<data[0].count() ;item++ ) {
//        ui->ComboChapterTheory->addItem(data[0].at(item),QVariant(data[1].at(item)));
//    }
//    ui->ComboChapterTheory->setCurrentIndex(-1);
//}

//void AddFileToEditor::on_FileList_itemSelectionChanged()
//{
//    PreviewFile.clear();
//    QVariant data = ui->FileList->currentItem()->data(Qt::UserRole);
//    QString fullFilePath = data.toString();
//    QString file = fullFilePath;
//    QString pdffile = file.replace(".tex",".pdf");
//    PreviewFile = fullFilePath;
//    if(!QFileInfo::exists(pdffile)){DataTex::CreateTexFile(fullFilePath);DataTex::loadImageFile(fullFilePath,view);}
//    else {DataTex::loadImageFile(fullFilePath,view);}
//}

void AddFileToEditor::FilesTable_selectionchanged()
{
    PreviewFile.clear();
    int row  = FilesTable->selectionModel()->currentIndex().row();
    QVariant data = FilesTable->model()->data(FilesTable->model()->index(row,7));
    QString fullFilePath = data.toString();
    QString file = fullFilePath;
    QString pdffile = file.replace(".tex",".pdf");
    PreviewFile = fullFilePath;
    if(!QFileInfo::exists(pdffile)){
        DataTex::CreateTexFile(fullFilePath);

        DataTex::loadImageFile(fullFilePath,view);
    }
    else {DataTex::loadImageFile(fullFilePath,view);}
}

//void AddFileToEditor::on_FieldListExercises_itemClicked(QListWidgetItem *item)
//{
//    ui->radioButton->setEnabled(true);
//    ui->radioButton_2->setEnabled(true);
//    ui->radioButton->setChecked(true);
//    for (int i=0;i<checklist.count() ;i++ ) {
//        checklist.at(i)->setEnabled(true);
//    }
//    if(ui->buttonGroup_2->checkedButton() != 0)
//    {ui->buttonGroup_2->setExclusive(false);
//    ui->buttonGroup_2->checkedButton()->setChecked(false);
//    ui->buttonGroup_2->setExclusive(true);}
//}

//void AddFileToEditor::on_radioButton_toggled(bool checked)
//{
//    ui->FileList->clear();
//    if(ui->buttonGroup_2->checkedButton() != 0)
//    {
//       ui->buttonGroup_2->setExclusive(false);
//       ui->buttonGroup_2->checkedButton()->setChecked(false);
//       ui->buttonGroup_2->setExclusive(true);
//    }
//    ui->ComboExTypeExercises->clear();
//    if(checked){
//        ExSubType = "SectEx";
//        SolutionFileType = "SolSE";
//        ui->ExercisesBox->setText(tr("Exercises"));
//        ui->ExerciseSolutionsBox->setText(tr("Exercise solutions"));
//        ui->label_10->setText(tr("Exercise type"));
//    }
//    else{
//        ExSubType = "SectSub";
//        SolutionFileType = "SolSS";
//        ui->ExercisesBox->setText("Subjects");
//        ui->label_10->setText("Subject type");
//    ui->ExerciseSolutionsBox->setText("Subject solutions");}
//    ui->ExercisesBox->setProperty("Name",ExSubType);
//    ui->ExerciseSolutionsBox->setProperty("Name",SolutionFileType);
//}

//void AddFileToEditor::on_CheckBox_checked(bool checked)
//{
//    if(checked){
//        ui->ComboChapterExercises->setEnabled(checked);
//        ui->ComboChapterExercises->clear();
//            QCheckBox *btn = static_cast<QCheckBox *>(sender());
//            FileType = btn->property("Name").toString();
////                    SqlFunctions::Get_StringList_From_Query(
////                        SqlFunctions::GetFileTypeIdfromCheckBox.arg(btn->text()),currentbase).at(0);
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Single(SqlFunctions::Chapters_Query,currentbase,
//                                           ui->FieldListExercises->currentItem()->text());
//    for (int item=0;item<data[0].count() ;item++ ) {
//        ui->ComboChapterExercises->addItem(data[0].at(item),QVariant(data[1].at(item)));
//    }
//    ui->ComboChapterExercises->setCurrentIndex(-1);
//    ui->ComboSectionExercises->setEnabled(false);
//    ui->ComboExTypeExercises->setEnabled(false);
//    }
//}

//void AddFileToEditor::on_ComboChapterExercises_currentIndexChanged(int index)
//{
//    ui->ComboSectionExercises->setEnabled(true);
//    ui->ComboSectionExercises->setCurrentIndex(-1);
//    ui->ComboSectionExercises->clear();
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Single(SqlFunctions::Sections_Chapters_Query,currentbase,
//                                           ui->ComboChapterExercises->currentText());
//    for (int item=0;item<data[0].count() ;item++ ) {
//        ui->ComboSectionExercises->addItem(data[0].at(item),QVariant(data[1].at(item)));
//    }
//    ui->ComboSectionExercises->setCurrentIndex(-1);
//    ui->ComboExTypeExercises->setEnabled(false);

//}

//void AddFileToEditor::on_ComboSectionExercises_currentIndexChanged(int index)
//{
//    ui->label_10->setEnabled(true);
//    ui->ComboExTypeExercises->setEnabled(true);
//    ui->ComboExTypeExercises->clear();
//    if(ui->radioButton->isChecked()){}

//    QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                SqlFunctions::Exercise_Types_Query.arg(ui->ComboSectionExercises->currentData().toString()),currentbase);
//    for (int item=0;item<data[0].count() ;item++ ) {
//        ui->ComboExTypeExercises->addItem(data[0].at(item),QVariant(data[1].at(item)));
//    }
//    ui->FileList->clear();
//    if(ui->MethodsBox->isChecked() || ui->ExamplesBox->isChecked()){
//        ui->ComboExTypeExercises->setEnabled(false);
//        ui->ComboExTypeExercises->clear();
//    }
//    ui->ComboExTypeExercises->setCurrentIndex(-1);
//}

//void AddFileToEditor::on_ComboExTypeExercises_currentIndexChanged(int index)
//{
//    ui->FileList->clear();
//    QList<QStringList> data =
//            SqlFunctions::ComboList_Double_List(SqlFunctions::AddExerciseFilesToEditor
//            .arg(ui->FieldListExercises->currentItem()->data(Qt::UserRole).toString())
//            .arg(ui->ComboChapterExercises->currentData().toString())
//            .arg(ui->ComboSectionExercises->currentData().toString())
//            .arg(ui->ComboExTypeExercises->currentData().toString())
//            .arg(FileType),currentbase);
//    for(int row=0;row<data[0].count();row++)
//    {
//        ui->FileList->addItem(data[0].at(row));
//        ui->FileList->item(row)->setData(Qt::UserRole,data[1].at(row));
//    }
//}

void AddFileToEditor::on_Okbutton_rejected()
{
    reject();
}

void AddFileToEditor::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Close window"),tr("Do you want to close the window?"),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

//void AddFileToEditor::on_FileList_itemDoubleClicked(QListWidgetItem *item)
//{
//    int rows = ui->SelectedFiles->rowCount();
//    rows++;
//    if(rows>0 && rows!=1){
//        ui->removeButton->setEnabled(true);
//        ui->UpButton->setEnabled(true);
//        ui->DownButton->setEnabled(true);
//    }
//    else if(rows==1){
//        ui->removeButton->setEnabled(true);
//        ui->UpButton->setEnabled(false);
//        ui->DownButton->setEnabled(false);
//    }
//    QString fullFilePath = ui->FileList->currentItem()->data(Qt::UserRole).toString();
//    QString Id = item->text();
//    QString FileType = SqlFunctions::Get_StringList_From_Query(SqlFunctions::AddTheoryFilesToEditor.arg(fullFilePath),currentbase).at(0);
//    int row = ui->SelectedFiles->rowCount();
//    ui->SelectedFiles->insertRow(row);
//    ui->SelectedFiles->setItem(row,1 , new QTableWidgetItem(Id));
//    ui->SelectedFiles->setItem(row,0 , new QTableWidgetItem(FileType));
//    ui->SelectedFiles->setItem(row,2 , new QTableWidgetItem(fullFilePath));
//    ui->SelectedFiles->selectRow(row);
//}

//void AddFileToEditor::on_FieldListCombExercises_itemClicked(QListWidgetItem *item)
//{
//    ui->SectionsList->clear();
//    ui->CombSubjectsBox->setEnabled(true);
//    ui->CombSubjectSolutionsBox->setEnabled(true);
//    ui->CombExercisesBox->setEnabled(true);
//    ui->CombExerciseSolutionsBox->setEnabled(true);
//    if(ui->buttonGroup_4->checkedButton() != 0)
//    {
//       ui->buttonGroup_4->setExclusive(false);
//       ui->buttonGroup_4->checkedButton()->setChecked(false);
//       ui->buttonGroup_4->setExclusive(true);
//    }
//    ui->FileList->clear();
//}

//void AddFileToEditor::on_ComboCombSubjectType_currentIndexChanged(int index)
//{
//    ui->SectionsList->clear();
//    QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                SqlFunctions::GetSectionsCombFiles
//                .arg(ComFileType,ui->FieldListCombExercises->currentItem()->data(Qt::UserRole).toString()),currentbase);
//    for(int i=0;i<data[0].count();i++){
//        ui->SectionsList->addItem(data[0].at(i));
//        ui->SectionsList->item(i)->setData(Qt::UserRole,data[1].at(i));
//    }
//}

//void AddFileToEditor::on_SectionsList_itemClicked(QListWidgetItem *item)
//{
//    ui->FileList->clear();
//    QStringList SectionList = item->data(Qt::UserRole).toString().split(",");
//    QString Sections = "\""+SectionList.join("\",\"")+"\"";
//    QString SectionsLength = QString::number(SectionList.length());
//    QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                SqlFunctions::AddCombFilesToList.arg(Sections,ComFileType,SectionsLength),currentbase);
//    for(int row=0;row<data[0].count();row++)
//    {
//        ui->FileList->addItem(data[0].at(row));
//        ui->FileList->item(row)->setData(Qt::UserRole,data[1].at(row));
//    }
//}

//void AddFileToEditor::on_CombSubjectsBox_clicked(bool checked)
//{
//    if(checked){
//        ComFileType = ui->CombSubjectsBox->property("Name").toString();
//    ui->ComboCombSubjectType->setEnabled(true);
//    QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                SqlFunctions::GetSubject_Types,currentbase);
//    for (int item=0;item<data[0].count() ;item++ )
//        {
//            ui->ComboCombSubjectType->addItem(data[0].at(item), QVariant(data[1].at(item)));
//        }
//    ui->ComboCombSubjectType->setCurrentIndex(-1);
//    ui->SectionsList->clear();
//    }
//}

//void AddFileToEditor::on_CombSubjectSolutionsBox_clicked(bool checked)
//{
//    if(checked){
//        ComFileType = ui->CombSubjectSolutionsBox->property("Name").toString();
//    ui->ComboCombSubjectType->setEnabled(true);
//    QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                SqlFunctions::GetSubject_Types,currentbase);
//    for (int item=0;item<data[0].count() ;item++ )
//        {
//            ui->ComboCombSubjectType->addItem(data[0].at(item), QVariant(data[1].at(item)));
//        }
//    ui->ComboCombSubjectType->setCurrentIndex(-1);
//    ui->SectionsList->clear();
//    }
//}

//void AddFileToEditor::on_CombExercisesBox_clicked(bool checked)
//{
//    if(checked){
//        ComFileType = ui->CombExercisesBox->property("Name").toString();
//        ui->ComboCombSubjectType->clear();
//        ui->SectionsList->clear();
//        ui->ComboCombSubjectType->setEnabled(false);
//        QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                    SqlFunctions::GetSectionsCombFiles
//                    .arg(ComFileType,ui->FieldListCombExercises->currentItem()->data(Qt::UserRole).toString()),currentbase);
//        for(int i=0;i<data[0].count();i++){
//            ui->SectionsList->addItem(data[0].at(i));
//            ui->SectionsList->item(i)->setData(Qt::UserRole,data[1].at(i));
//            }
//    }
//}

//void AddFileToEditor::on_CombExerciseSolutionsBox_clicked(bool checked)
//{
//    if(checked){
//        ComFileType = ui->CombExerciseSolutionsBox->property("Name").toString();
//        ui->ComboCombSubjectType->clear();
//        ui->SectionsList->clear();
//        ui->ComboCombSubjectType->setEnabled(false);
//        QList<QStringList> data = SqlFunctions::ComboList_Double_List(
//                    SqlFunctions::GetSectionsCombFiles
//                    .arg(ComFileType,ui->FieldListCombExercises->currentItem()->data(Qt::UserRole).toString()),currentbase);
//        for(int i=0;i<data[0].count();i++){
//            ui->SectionsList->addItem(data[0].at(0));
//            ui->SectionsList->item(i)->setData(Qt::UserRole,data[1].at(i));
//            }
//    }
//}

void AddFileToEditor::on_addButton_clicked()
{
    QTextCursor prev_cursor = ui->DocumentContent->textCursor();
//    myTextEdit->insertPlainText (myString);
    QString FileContent;
    int row = FilesTable->currentIndex().row();
    QFile file(FilesTable->model()->data(FilesTable->model()->index(row,7)).toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    FileContent = text.readAll();
    file.close();
//    int line = prev_cursor.position();
//    ui->DocumentContent->moveCursor (QTextCursor::End);
    ui->DocumentContent->insertPlainText(FileContent);
//    prev_cursor.setVerticalMovementX(2);
    ui->DocumentContent->setTextCursor(prev_cursor);
//    on_FileList_itemDoubleClicked(item);
}

void AddFileToEditor::on_removeButton_clicked()
{
        ui->removeButton->setEnabled(false);
        QString Content = ui->DocumentContent->toPlainText();
        Content.remove(DatabaseFileContent);
        ui->DocumentContent->setText(Content);
//        ui->UpButton->setEnabled(false);
//        ui->DownButton->setEnabled(false);
//        DataTex::loadImageFile("",view);
//    }
}

void AddFileToEditor::on_Okbutton_accepted()
{
//    int rows = ui->SelectedFiles->rowCount();
    QStringList paths;
//    for (int i=0;i<rows;i++) {
//        QString path = ui->SelectedFiles->item(i,2)->text();
//        paths.append(path);
//    }
//    emit files(paths);
    QString content = ui->DocumentContent->toPlainText();
    QTextStream textstream(&content);
    while (!textstream.atEnd()){
        QString LineText=textstream.readLine();
        if(LineText.contains("%# Database File : ")){ExercisesInsideDocument.append(LineText.remove("%# Database File : "));}
        if(LineText.contains("%@ Database source: ")){DatabasesInsideDocument.append(LineText.remove("%@ Database source: "));}
    }
    QStringList valuesQuery;
    for (int i = 0;i<ExercisesInsideDocument.count();i++ ) {
        valuesQuery.append("(\""+QFileInfo(CurrentDatabaseFile).baseName()+"\",\""+ExercisesInsideDocument[i]+"\",\""+DatabasesInsideDocument[i]+"\")");
    }
    QSqlQuery writeExercisesPerDatabase(DataTex::CurrentNotesFolderDataBase);
    writeExercisesPerDatabase.exec("INSERT OR IGNORE INTO Files_per_Document (\"Document_Id\",\"File_Id\",\"Files_Database_Source\") VALUES "+valuesQuery.join(","));
    writeExercisesPerDatabase.exec("DELETE FROM Files_per_Document WHERE Document_Id = \"DTX-test_24_9\" AND File_Id NOT IN (\""+ExercisesInsideDocument.join("\",\"")+"\")");
    SaveText();
    accept();
}

void AddFileToEditor::moveup(bool up)
{
//    Q_ASSERT(ui->SelectedFiles->selectedItems().count() > 0);
//    const int sourceRow = ui->SelectedFiles->row(ui->SelectedFiles->selectedItems().at(0));
//    const int destRow = (up ? sourceRow-1 : sourceRow+1);
//    Q_ASSERT(destRow >= 0 && destRow < ui->SelectedFiles->rowCount());

//    // take whole rows
//    QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
//    QList<QTableWidgetItem*> destItems = takeRow(destRow);

//    // set back in reverse order
//    setRow(sourceRow, destItems);
//    setRow(destRow, sourceItems);
//    ui->SelectedFiles->selectRow(destRow);
}

void AddFileToEditor::movedown(bool down)
{
//    Q_ASSERT(ui->SelectedFiles->selectedItems().count() > 0);
//    const int sourceRow = ui->SelectedFiles->row(ui->SelectedFiles->selectedItems().at(0));
//    const int destRow = (down ? sourceRow+1 : sourceRow-1);
//    Q_ASSERT(destRow >= 0 && destRow < ui->SelectedFiles->rowCount());

//    // take whole rows
//    QList<QTableWidgetItem*> sourceItems = takeRow(sourceRow);
//    QList<QTableWidgetItem*> destItems = takeRow(destRow);

//    // set back in reverse order
//    setRow(sourceRow, destItems);
//    setRow(destRow, sourceItems);
//    ui->SelectedFiles->selectRow(destRow);
}

QList<QTableWidgetItem*> AddFileToEditor::takeRow(int row)
{
//    QList<QTableWidgetItem*> rowItems;
//    for (int col = 0; col < ui->SelectedFiles->columnCount(); ++col)
//    {
//        rowItems << ui->SelectedFiles->takeItem(row, col);
//    }
//    return rowItems;
}

void AddFileToEditor::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
//    for (int col = 0; col < ui->SelectedFiles->columnCount(); ++col)
//    {
//        ui->SelectedFiles->setItem(row, col, rowItems.at(col));
//    }
}

void AddFileToEditor::on_UpButton_clicked()
{
    moveup(true);
}

void AddFileToEditor::on_DownButton_clicked()
{
    movedown(true);
}

void AddFileToEditor::SaveText()
{
    QString FileContent = ui->DocumentContent->toPlainText();
    QFile file(CurrentDatabaseFile);
    file.resize(0);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream ContentStream(&file);
    ContentStream << FileContent;
    file.close();
    ui->Save->setEnabled(false);
//    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(CurrentDatabaseFile,FileContent);
}

void AddFileToEditor::on_Rebuild_clicked()
{
    SaveText();
    DataTex::CreateTexFile(CurrentDatabaseFile);
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],CurrentDatabaseFile,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(CurrentDatabaseFile);
    DataTex::loadImageFile(CurrentDatabaseFile,DocView);
}

void AddFileToEditor::on_checkBox_clicked(bool checked)
{
    if(checked){
        LoadDatabaseFiles(currentbase,SqlFunctions::ShowAllDatabaseFiles);
        ui->FilesDatabasesCombo->setEnabled(checked);
//        qDebug()<<datalist.at(ui->FilesDatabasesCombo->currentIndex());
    }
    else {
        ui->FilesDatabasesCombo->setEnabled(checked);
        LoadDatabaseFiles(DataTex::CurrentTexFilesDataBase,SqlFunctions::ShowAllDatabaseFiles);
    }
}

void AddFileToEditor::on_FilesDatabasesCombo_currentIndexChanged(int index)
{
    if(index>-1 && ui->checkBox->isChecked()){
        for (int i=0;i<datalist.count();i++) {
            if(datalist[i].databaseName()==ui->FilesDatabasesCombo->currentData().toString()){
                index = i;
            }
        }
        currentbase = datalist.at(index);
        LoadDatabaseFiles(currentbase,SqlFunctions::ShowAllDatabaseFiles);
    }
//    qDebug()<<datalist;
}

void AddFileToEditor::on_DocumentContent_cursorPositionChanged()
{
    int start = -1;
    int end = -1;
    int lineNumber = -1;
    LatexTextEdit::clearFormat(ui->DocumentContent);
    positions.clear();
    DatabaseFileContent.clear();
    QString FileContent = ui->DocumentContent->toPlainText();
    QTextStream parse(&FileContent);
    while(!parse.atEnd()){
        lineNumber++;
        QString line = parse.readLine();
        if(line.startsWith("%# Database File")){
            start = lineNumber;
        }
        if(line.startsWith("%# End of file")){
            end = lineNumber;
            positions.append({start,end});
        }
    }
    int line = ui->DocumentContent->textCursor().blockNumber();
    int fileIndex =-1;
    for (int k=0;k<positions.count();k++) {
        if(line>=positions[k][0] && line <=positions[k][1]){
            fileIndex = k;
            for (int i = positions[fileIndex][0];i<positions[fileIndex][1]+1;i++) {
                QTextBlock block = ui->DocumentContent->document()->findBlockByNumber(i);
                QTextCursor cursor(block);
                QTextBlockFormat blockFormat = cursor.blockFormat();
                blockFormat.setBackground(QColor(240,245,245));
                cursor.setBlockFormat(blockFormat);
                DatabaseFileContent.append(block.text()+"\n");
            }
            ui->removeButton->setEnabled(true);
            qDebug()<<DatabaseFileContent;
            break;
        }
        else{
            LatexTextEdit::clearFormat(ui->DocumentContent);
            ui->removeButton->setEnabled(false);
        }
    }
//    LatexTextEdit::SetEditorReadOnly(ui->DocumentContent);
}

void AddFileToEditor::on_Save_clicked()
{
    SaveText();
    ui->Save->setEnabled(false);
}

void AddFileToEditor::on_DocumentContent_textChanged()
{
    ui->Save->setEnabled(true);
}
