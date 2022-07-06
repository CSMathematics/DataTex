#include "solvedatabaseexercise.h"
#include "ui_solvedatabaseexercise.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAbstractButton>
#include <QCheckBox>
#include "sqlfunctions.h"
#include "datatex.h"
#include "csvfunctions.h"

SolveDatabaseExercise::SolveDatabaseExercise(QWidget *parent, QStringList meta, QString sections) :
    QDialog(parent),
    ui(new Ui::SolveDatabaseExercise)
{
    ui->setupUi(this);
    metadata = meta;
    Sections = sections;
    currentbase = DataTex::CurrentTexFilesDataBase;
    QStringList Field_Names;
    QStringList Field_ids;
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        Field_Names.append(fields.value(0).toString());
        Field_ids.append(fields.value(1).toString());
    }
        for(int row=0;row<Field_Names.count();row++)
        {
            ui->FieldsList->addItem(Field_Names.at(row));
            ui->FieldsList->item(row)->setData(Qt::UserRole,Field_ids.at(row));
        }
    for (int i=0;i<ui->buttonGroup->buttons().count();i++ ) {
        connect(ui->buttonGroup->buttons().at(i), &QRadioButton::toggled, this, &SolveDatabaseExercise::on_FileType_checked);
    }

    for (int i=0;i<ui->buttonGroup->buttons().count();i++ ) {
        ui->buttonGroup->buttons().at(i)->setEnabled(false);
    }
    ui->ExeciseRadioButton->setProperty("Name","SectEx");
    ui->SubjectRadioButton->setProperty("Name","SectSub");
    ui->ComExerciseRadioButton->setProperty("Name","CombEx");
    ui->ComSubjectRadioButton->setProperty("Name","CombSub");
    ui->ExeciseRadioButton->setProperty("Solution","SolSE");
    ui->SubjectRadioButton->setProperty("Solution","SolSS");
    ui->ComExerciseRadioButton->setProperty("Solution","SolCE");
    ui->ComSubjectRadioButton->setProperty("Solution","SolCS");
    ui->ExeciseRadioButton->setProperty("Query",SqlFunctions::Section_List_contaning_Exercises);
    ui->ExeciseRadioButton->setProperty("header","\"et\".\"Name\" AS ");
    ui->ExeciseRadioButton->setProperty("title",tr("'Exercise type'"));
    ui->SubjectRadioButton->setProperty("Query",SqlFunctions::Section_List_contaning_Exercises);
    ui->SubjectRadioButton->setProperty("header","\"et\".\"Name\" AS ");
    ui->SubjectRadioButton->setProperty("title",tr("'Subject type'"));
    ui->ComExerciseRadioButton->setProperty("Query",SqlFunctions::GetSectionsCombFiles);
    ui->ComExerciseRadioButton->setProperty("header","group_concat(\"Sections\".\"Name\") AS ");
    ui->ComExerciseRadioButton->setProperty("title",tr("'Sections'"));
    ui->ComSubjectRadioButton->setProperty("Query",SqlFunctions::GetSectionsCombFiles);
    ui->ComSubjectRadioButton->setProperty("header","group_concat(\"Sections\".\"Name\") AS ");
    ui->ComSubjectRadioButton->setProperty("title",tr("'Sections'"));
    ExerciseTable = new ExtendedTableWidget(this);
    ui->gridLayout_3->addWidget(ExerciseTable,1,0);
    ExerciseTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ExerciseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ExerciseTable->horizontalHeader()->setSectionsClickable(true);

    ui->ExeciseRadioButton->setHidden(true);
    ui->SubjectRadioButton->setHidden(true);
    ui->ComExerciseRadioButton->setHidden(true);
    ui->ComSubjectRadioButton->setHidden(true);

   connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
   view = new PdfViewer(this);
   view->setMinimumWidth(700);
   ui->verticalLayout->addWidget(view);
   view->show();

   viewSolution = new PdfViewer(this);
   ui->gridLayout_10->addWidget(viewSolution);
   viewSolution->show();

   ui->SolutionContent->setReadOnly(true);
   ui->splitter_2->setSizes(QList<int>({150, 1}));

   connect(ui->SolutionContent,&QTextEdit::textChanged,this,[=](){ui->SaveContent->setEnabled(true);});
   QSqlQuery FileTypes(currentbase);
   FileTypes.exec("SELECT a.Id,b.Id,a.FolderName,b.FolderName,a.FileType FROM "
                  "(SELECT FileType,Id,FolderName FROM FileTypes WHERE Solvable = 1) as a "
                  "JOIN (SELECT FileType,Id,FolderName,BelongsTo FROM FileTypes WHERE Solvable = -1) as b "
                  "ON a.Id=b.BelongsTo");

   int line = -1;
   ui->gridLayout->removeItem(ui->verticalSpacer);
   while(FileTypes.next()){
       FileTypeSolIds.insert(FileTypes.value(0).toString(),FileTypes.value(1).toString());
       FileTypeFolders.insert(FileTypes.value(0).toString(),FileTypes.value(2).toString());
       FileTypeFoldersSol.insert(FileTypes.value(0).toString(),FileTypes.value(3).toString());
       line++;
//       if(line>3){
           QRadioButton * button = new QRadioButton(FileTypes.value(4).toString(),this);
           button->setProperty("Name",FileTypes.value(0).toString());
           CustomFileTypesList.append(button);
           ui->buttonGroup->addButton(button);
           ui->gridLayout->addWidget(button,ui->gridLayout->rowCount(),0);
           button->setEnabled(false);
           button->setProperty("Query",SqlFunctions::GetSectionsCombFiles);
           button->setProperty("header","\"et\".\"Name\" AS ");
           button->setProperty("title",FileTypes.value(1).toString());
           button->setProperty("Solution",FileTypes.value(1).toString());
           connect(button, &QRadioButton::toggled, this, &SolveDatabaseExercise::on_FileType_checked);
//       }
   }
   ui->gridLayout->addItem(ui->verticalSpacer,ui->gridLayout->rowCount(),0);

   if(!metadata.isEmpty()){
       ui->FieldsList->setCurrentItem(ui->FieldsList->findItems(metadata[1],Qt::MatchExactly).at(0));
       on_FieldsList_itemClicked(ui->FieldsList->findItems(metadata[1],Qt::MatchExactly).at(0));
       QString filetype = metadata.last();
       for (int i=0;i<ui->buttonGroup->buttons().count();i++ ) {
           if(filetype == ui->buttonGroup->buttons().at(i)->property("Name")){
               ui->buttonGroup->buttons().at(i)->setChecked(true);
           }
       }
       ui->SectionList->setCurrentItem(ui->SectionList->findItems(Sections,Qt::MatchExactly).at(0));
       on_SectionList_itemSelectionChanged();

       QAbstractItemModel * model = ExerciseTable->model();
       QSortFilterProxyModel proxy;
       proxy.setSourceModel(model);
       proxy.setFilterKeyColumn(0);
       proxy.setFilterFixedString(metadata[0]);
       QModelIndex matchingIndex = proxy.mapToSource(proxy.index(0,0));
       if(matchingIndex.isValid()){
           ExerciseTable->selectRow(matchingIndex.row());
           ExerciseTable_SelectionChanged();
       }
   }
   ui->splitter->setSizes(QList<int>({150,400}));
   ui->scrollArea->setMinimumWidth(200);
   bool hasSolution = ui->SolutionsCombo->count()>0;
   ui->DeleteCurrentSolution->setEnabled(hasSolution);
   ui->SaveContent->setEnabled(hasSolution);
   ui->RecompileButton->setEnabled(hasSolution);
   connect(ui->SolutionContent,&QTextEdit::textChanged,this,[=](){
       ui->SaveContent->setEnabled(true);
   });
}

SolveDatabaseExercise::~SolveDatabaseExercise()
{
    delete ui;
    delete view;
}

void SolveDatabaseExercise::on_FileType_checked(bool checked)
{
    ExerciseFiles.clear();
    ui->SectionList->clear();
    if (checked){
        QSqlQueryModel * emptyModel = new QSqlQueryModel(this);
        QSqlQuery emptytable(currentbase);
        emptytable.exec(SqlFunctions::EmptyTable);
        emptyModel->setQuery(emptytable);
        ExerciseTable->setModel(emptyModel);
        ExerciseTable->show();

        QRadioButton *btn = static_cast<QRadioButton *>(sender());
        radiobutton = btn;
        SolutionType = btn->property("Solution").toString();
        FileType = btn->property("Name").toString();
        QSqlQuery sectionList(currentbase);
        QStringList Section_Names;
        QStringList Section_Ids;
//        if(ui->ExeciseRadioButton->isChecked() || ui->SubjectRadioButton->isChecked()){
        sectionList.prepare(btn->property("Query").toString().
                            arg(ui->FieldsList->currentItem()->data(Qt::UserRole).toString(),FileType));
//        }
//        else{sectionList.prepare(btn->property("Query").toString()
//                                 .arg(FileType,ui->FieldsList->currentIndex().data(Qt::UserRole).toString()));}
        sectionList.exec();
        while (sectionList.next()) {
            Section_Names.append(sectionList.value(0).toString());
            Section_Ids.append(sectionList.value(1).toString());
        }
        for(int row=0;row<Section_Names.count();row++)
        {
            ui->SectionList->addItem(Section_Names.at(row));
            ui->SectionList->item(row)->setData(Qt::UserRole,Section_Ids.at(row));
        }
    }
}

void SolveDatabaseExercise::on_FieldsList_itemClicked(QListWidgetItem *item)
{
    for (int i=0;i<ui->buttonGroup->buttons().count();i++ ) {
        ui->buttonGroup->buttons().at(i)->setEnabled(true);
    }
    if(ui->buttonGroup->checkedButton() != nullptr)
    {
       ui->buttonGroup->setExclusive(false);
       ui->buttonGroup->checkedButton()->setChecked(false);
       ui->buttonGroup->setExclusive(true);
    }
    QSqlQueryModel * emptyModel = new QSqlQueryModel(this);
    QSqlQuery emptytable(currentbase);
    emptytable.exec(SqlFunctions::EmptyTable);
    emptyModel->setQuery(emptytable);
    ExerciseTable->setModel(emptyModel);
    ExerciseTable->show();
}

void SolveDatabaseExercise::on_SectionList_itemSelectionChanged()
{
    QString title = radiobutton->property("header").toString()+radiobutton->property("title").toString();
    QSqlQueryModel * Model = new QSqlQueryModel(this);
    QSqlQuery tableQuery(currentbase);
    if(ui->ExeciseRadioButton->isChecked() || ui->SubjectRadioButton->isChecked()){
        tableQuery.prepare(QString(SqlFunctions::ShowSolvedAndUnSolvedExercises).
                           arg(title,radiobutton->property("Name").toString(),
                           ui->SectionList->currentItem()->data(Qt::UserRole).toString()));
    }
    else{
        QStringList secQuery;
        foreach (QString section,ui->SectionList->currentItem()->text().split("|")) {
            secQuery.append("sec LIKE \"%"+section+"%\" ");
        }
        tableQuery.prepare(QString(SqlFunctions::ShowSolvedAndUnSolvedCombExercises).
                            arg(radiobutton->property("Name").toString(),secQuery.join(" AND ")));
    }
    tableQuery.exec();
    Model->setQuery(tableQuery);
    ExerciseTable->setModel(Model);
    ExerciseTable->show();
    ExerciseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ExerciseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ExerciseTable->setAlternatingRowColors(true);
    LoadTableHeaders();
//    ExerciseTable->setStyleSheet("alternate-background-color: #e8e8e8");

    DataTex::StretchColumnsToWidth(ExerciseTable);
    ExerciseTable->setColumnHidden(3,true);
    ExerciseTable->setColumnHidden(4,true);
    ExerciseTable->setColumnHidden(5,true);
    ExerciseTable->setSelectionMode( QAbstractItemView::SingleSelection );
    int columns = ExerciseTable->model()->columnCount();
    ExerciseTable->generateFilters(columns,false);
    connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
    connect(ExerciseTable->filterHeader(), &FilterTableHeader::filterValues, this, &SolveDatabaseExercise::updateFilter);
}

void SolveDatabaseExercise::LoadTableHeaders()
{
    if(ui->ExeciseRadioButton->isChecked() || ui->SubjectRadioButton->isChecked()){
        ExerciseTable->model()->setHeaderData(0,Qt::Horizontal,tr("Id"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(1,Qt::Horizontal,tr("Exercise type"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(2,Qt::Horizontal,tr("Solved_Prooved"),Qt::DisplayRole);
    }
    else{
        ExerciseTable->model()->setHeaderData(0,Qt::Horizontal,tr("Id"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(1,Qt::Horizontal,tr("Sections"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(2,Qt::Horizontal,tr("Solved_Prooved"),Qt::DisplayRole);
    }
}

void SolveDatabaseExercise::updateFilter(QStringList values)
{
    QString title = radiobutton->property("header").toString()+radiobutton->property("title").toString();
    QString tableQuery;
    if(ui->ExeciseRadioButton->isChecked() || ui->SubjectRadioButton->isChecked()){
    tableQuery = QString(SqlFunctions::ShowSolvedAndUnSolvedExercisesFiltered).
                       arg(title,radiobutton->property("Name").toString(),
                       ui->SectionList->currentItem()->data(Qt::UserRole).toString(),values[0],values[1],values[2]);
    }
    else{tableQuery = QString(SqlFunctions::ShowSolvedAndUnSolvedCombExercisesFiltered).
                arg(radiobutton->property("Name").toString(),ui->SectionList->currentItem()->text(),values[0],values[1],values[2]);}
    DataTex::updateTableView(ExerciseTable,tableQuery,currentbase,this);
    connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
//    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void SolveDatabaseExercise::ExerciseTable_SelectionChanged()
{
    if(ExerciseTable->currentIndex().row()>-1){
        Exercise.clear();
        Solutions.clear();

        int row = ExerciseTable->currentIndex().row();
        Exercise = ExerciseTable->model()->data(ExerciseTable->model()->index(row,3)).toString();

        DataTex::CurrentPreamble = ExerciseTable->model()->data(ExerciseTable->model()->index(row,4)).toString();
        CurrentBuildCommand = ExerciseTable->model()->data(ExerciseTable->model()->index(row,5)).toString();
        tempFile = Exercise;
        tempFile.replace("-"+FileType,"-"+FileTypeSolIds[FileType]);
        tempFile.replace(FileTypeFolders[FileType],FileTypeFoldersSol[FileType]);
        NewSolution = tempFile;
        QDirIterator list(QFileInfo(tempFile).absolutePath(),QStringList() << QFileInfo(tempFile).baseName()+QRegExp("\\d+").cap(1)+"-*.tex", QDir::Files, QDirIterator::Subdirectories);
        while (list.hasNext()){
            Solutions.append(list.next());
        }
//        ui->SolutionLabel->setText("Solution of file : "+ExerciseTable->model()->data(ExerciseTable->model()->index(row,0)).toString());
        DataTex::loadImageFile(Exercise,view);
        ui->SolutionsCombo->clear();
        for (int i=0;i<Solutions.count();i++) {
            ui->SolutionsCombo->addItem(QFileInfo(Solutions[i]).baseName(),QVariant(Solutions[i]));
        }
        ui->SolutionsCombo->model()->sort(Qt::AscendingOrder);
        ui->DeleteCurrentSolution->setEnabled(ui->SolutionsCombo->count()>0);
        ui->RecompileButton->setEnabled(ui->SolutionsCombo->count()>0);
        ui->SaveContent->setEnabled(false);
    }
}

void SolveDatabaseExercise::CreateSolution(QString filetype)
{
    ui->DeleteCurrentSolution->setEnabled(true);
    ui->RecompileButton->setEnabled(true);
//    QStringList Sections;
    QString file = tempFile;
//    QRegExp rx("\\d+\\b(.tex)\\b");
//    if (rx.indexIn(file) != -1) {
        file.remove(".tex"/*rx.cap(0)*/);
//    }
    int i = 1;
    while(QFileInfo::exists(file+"-"+QString::number(i)+".tex")){
        i++;
    }
    NewSolution = file+"-"+QString::number(i)+".tex";
    if(ExerciseTable->model()->rowCount()>0){
//        Sections<<ui->SectionList->currentItem()->data(Qt::UserRole).toString().split(",");
        QSqlQuery solved(currentbase);
        solved.exec(QString("INSERT INTO Solutions_per_File (Solution_Id,Solution_Path,File_Id) VALUES ('%1', '%2','%3');").arg(QFileInfo(NewSolution).baseName(),NewSolution,QFileInfo(Exercise).baseName()));
        solved.exec(SqlFunctions::UpdateSolution.arg(QFileInfo(Exercise).baseName()));
        QSqlQuery writeExercise(DataTex::CurrentTexFilesDataBase);
        QStringList ColumnNames = SqlFunctions::Get_StringList_From_Query("SELECT \"name\" FROM pragma_table_info(\'Database_Files\');",currentbase);
        QStringList NewValues = ColumnNames;
        NewValues[0] = "\""+QFileInfo(NewSolution).baseName()+"\"";
        NewValues[1] = "\""+filetype+"\"";
        NewValues[4] = "\""+NewSolution+"\"";
        NewValues[5] = "\""+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"\"";
        NewValues[6] = "\"-\"";
        writeExercise.exec(QString("INSERT INTO Database_Files ("+ColumnNames.join(",")+") "
                           "SELECT "+NewValues.join(",")+" FROM Database_Files "
                           "WHERE Id = '%1'").arg(QFileInfo(Exercise).baseName()));
        QDir path(QFileInfo(NewSolution).absolutePath());
        if (!path.exists()){path.mkpath(".");}
        ui->SolutionContent->setText(DataTex::NewFileText(NewSolution,""));

        writeExercise.exec(QString("INSERT INTO Chapters_per_File (File_Id,Chapter_Id) SELECT \"%1\",Chapter_Id FROM Chapters_per_File "
                                   "WHERE File_Id = '%2' ").arg(QFileInfo(NewSolution).baseName(),(QFileInfo(Exercise).baseName())));
        writeExercise.exec(QString("INSERT INTO Sections_per_File (File_Id,Section_Id) SELECT \"%1\",Section_Id FROM Sections_per_File "
                                   "WHERE File_Id = '%2' ").arg(QFileInfo(NewSolution).baseName(),(QFileInfo(Exercise).baseName())));
        writeExercise.exec(QString("INSERT INTO ExerciseTypes_per_File (File_Id,ExerciseType_Id) SELECT \"%1\",ExerciseType_Id FROM ExerciseTypes_per_File "
                                   "WHERE File_Id = '%2' ").arg(QFileInfo(NewSolution).baseName(),(QFileInfo(Exercise).baseName())));

        emit solution(NewSolution,ui->SolutionContent->toPlainText());
        ui->SolutionsCombo->addItem(QFileInfo(NewSolution).baseName(),QVariant(NewSolution));
        ui->SolutionsCombo->setCurrentText(QFileInfo(NewSolution).baseName());
    }
}

void SolveDatabaseExercise::on_RecompileButton_clicked()
{
    QString file = ui->SolutionsCombo->currentData().toString();
    SaveText();
    DataTex::CreateTexFile(file,0,"");
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],file,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(file);
    DataTex::loadImageFile(file,viewSolution);
}

void SolveDatabaseExercise::SaveText()
{
    FileContent = ui->SolutionContent->toPlainText();
    QFile file(ui->SolutionsCombo->currentData().toString());
    file.resize(0);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream Grammh(&file);
    Grammh << FileContent;
    file.close();
    DataTex::SaveContentToDatabase(ui->SolutionsCombo->currentData().toString(),FileContent);
    CsvFunctions::WriteDataToCSV(NewSolution,DataTex::CurrentTexFilesDataBase);
    ui->SaveContent->setEnabled(false);
//    UndoTex->setEnabled(false);
}

void SolveDatabaseExercise::on_CloseButton_clicked()
{
    if(ui->SaveContent->isEnabled()){
        QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Error"),tr("There are unsaved changes in\nthe current solution.\n"
                                                          "Do you want to save the content?"),
                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        };
        SaveText();
    }
    emit on_close(ui->SolutionsCombo->currentText());
    accept();
}

void SolveDatabaseExercise::on_NewSolutionButton_clicked()
{
    ui->SolutionContent->setReadOnly(false);
    CreateSolution(SolutionType);
}

void SolveDatabaseExercise::on_DeleteCurrentSolution_clicked()
{
    QCheckBox *cb = new QCheckBox(tr("Delete File"));
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the file %1").arg(ui->SolutionsCombo->currentText()));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(DataTex::CurrentTexFilesDataBase);
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM Database_Files WHERE Id = \"%1\"").arg(ui->SolutionsCombo->currentText()));
        deleteQuery.exec(SqlFunctions::UpdateSolution.arg(QFileInfo(Exercise).baseName()));
        if(cb->isChecked()==true){
            QFile file(ui->SolutionsCombo->currentData().toString());
            file.remove();
            //QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(ui->SolutionsCombo->currentData().toString()).absolutePath()));}
        }
        ui->SolutionsCombo->removeItem(ui->SolutionsCombo->currentIndex());
    }
}

void SolveDatabaseExercise::on_SolutionsCombo_currentIndexChanged(int index)
{
    QFile file(ui->SolutionsCombo->currentData().toString());
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    ui->SolutionContent->setText(text.readAll());
    file.close();
    ui->SaveContent->setEnabled(false);
    DataTex::loadImageFile(ui->SolutionsCombo->currentData().toString(),viewSolution);
//    if(ui->SaveContent->isEnabled()){
//        QMessageBox::StandardButton resBtn =
//                QMessageBox::warning( this,tr("Error"),tr("There are unsaved changes in\nthe current solution.\n"
//                                                          "Do you want to save the content?"),
//                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
//        if (resBtn == QMessageBox::No) {
//            return;
//        };
//        SaveText();
//    }
}

void SolveDatabaseExercise::on_SaveContent_clicked()
{
    SaveText();
}


void SolveDatabaseExercise::on_ShowTexButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void SolveDatabaseExercise::on_ShowPdfButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void SolveDatabaseExercise::closeEvent (QCloseEvent *event)
{
    on_CloseButton_clicked();
}
