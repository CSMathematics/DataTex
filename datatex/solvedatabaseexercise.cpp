#include "solvedatabaseexercise.h"
#include "ui_solvedatabaseexercise.h"
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QDesktopServices>
#include <QDebug>
#include <QDirIterator>
#include <QVector>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QProcess>
#include <QAbstractButton>
#include <QCheckBox>
#include "sqlfunctions.h"
#include "datatex.h"

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

   connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
   view = new QPdfViewer(this);
   view->setMinimumWidth(700);
   ui->verticalLayout->addWidget(view);
   view->show();

   viewSolution = new PdfViewer(this);
   ui->gridLayout_10->addWidget(viewSolution);
   viewSolution->show();

//   ui->TexPage->setChecked(true);
   ui->tabWidget->setCornerWidget(ui->ToolBar,Qt::TopRightCorner);
   ui->splitter_2->setSizes(QList<int>({150, 1}));

   connect(ui->SolutionContent,&QTextEdit::textChanged,this,[=](){ui->SaveContent->setEnabled(true);});

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
}

SolveDatabaseExercise::~SolveDatabaseExercise()
{
    delete ui;
    delete view;
}

QString SolveDatabaseExercise::getFileType()
{
    QString filetype;
    for (int i=0;i<ui->buttonGroup->buttons().count();i++ ) {
        if(ui->buttonGroup->buttons().at(i)->isChecked()==true){
            filetype = ui->buttonGroup->buttons().at(i)->text();
        }
    }
    return filetype;
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
        QString fileType = btn->property("Name").toString();
        QSqlQuery sectionList(currentbase);
        QStringList Section_Names;
        QStringList Section_Ids;
        if(ui->ExeciseRadioButton->isChecked() || ui->SubjectRadioButton->isChecked()){
        sectionList.prepare(btn->property("Query").toString().
                            arg(ui->FieldsList->currentItem()->data(Qt::UserRole).toString(),fileType));
        }
        else{sectionList.prepare(btn->property("Query").toString()
                                 .arg(fileType,ui->FieldsList->currentIndex().data(Qt::UserRole).toString()));}
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
    else{tableQuery.prepare(QString(SqlFunctions::ShowSolvedAndUnSolvedCombExercises).
                            arg(radiobutton->property("Name").toString(),ui->SectionList->currentItem()->text()));
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
    for (int c = 0; c < ExerciseTable->horizontalHeader()->count()-1; ++c)
    {
        ExerciseTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
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
        ExerciseTable->model()->setHeaderData(2,Qt::Horizontal,tr("Solved"),Qt::DisplayRole);
    }
    else{
        ExerciseTable->model()->setHeaderData(0,Qt::Horizontal,tr("Id"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(1,Qt::Horizontal,tr("Sections"),Qt::DisplayRole);
        ExerciseTable->model()->setHeaderData(2,Qt::Horizontal,tr("Solved"),Qt::DisplayRole);
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
    qDebug()<<values;
}

void SolveDatabaseExercise::ExerciseTable_SelectionChanged()
{
    if(ExerciseTable->currentIndex().row()>-1){
        Exercise.clear();
        Solutions.clear();
        SolutionsCount = 0;
        int row = ExerciseTable->currentIndex().row();
        Exercise = ExerciseTable->model()->data(ExerciseTable->model()->index(row,3)).toString();

        //Προσωρινό μέχρι να έχουν όλες οι ασκήσεις Preamble
        QString pr = ExerciseTable->model()->data(ExerciseTable->model()->index(row,4)).toString();
        if(!pr.isNull()){
            DataTex::CurrentPreamble = pr;
        }
        else {
            DataTex::CurrentPreamble = "Askhseis";
        }

        CurrentBuildCommand = ExerciseTable->model()->data(ExerciseTable->model()->index(row,5)).toString();
        QString Solution = Exercise;
        QStringList Ids;
        QStringList solIds;
        QStringList folders;
        QStringList solfolders;
        Ids<<"SectEx"<<"SectSub"<<"CombEx"<<"CombSub";
        solIds<<"SolSE"<<"SolSS"<<"SolCE"<<"SolCS";
        for (int i=0;i<Ids.count();i++ ) {
            QString folderName = SqlFunctions::Get_StringList_From_Query(
                        SqlFunctions::FolderName.arg(Ids.at(i)),currentbase).at(0);
            folders.append(folderName);
            QString solfolderName = SqlFunctions::Get_StringList_From_Query(
                        SqlFunctions::FolderName.arg(solIds.at(i)),currentbase).at(0);
            solfolders.append(solfolderName);
            Solution.replace("-"+Ids.at(i),"-"+solIds.at(i));
            Solution.replace(folders.at(i),solfolders.at(i));
        }
        NewSolution = Solution;
        QDirIterator list(QFileInfo(Solution).absolutePath(),QStringList() << QFileInfo(Solution).baseName()+"*.tex", QDir::Files, QDirIterator::Subdirectories);
        while (list.hasNext()){
            Solutions.append(list.next());
            SolutionsCount++;
        }
        qDebug()<<Solutions;
//        ui->SolutionLabel->setText("Solution of file : "+ExerciseTable->model()->data(ExerciseTable->model()->index(row,0)).toString());
        DataTex::loadImageFile(Exercise,view);
        ui->SolutionsCombo->clear();
        for (int i=0;i<Solutions.count();i++) {
            ui->SolutionsCombo->addItem(QFileInfo(Solutions[i]).baseName(),QVariant(Solutions[i]));
        }
//        if(ExerciseTable->model()->data(ExerciseTable->model()->index(row,2)).toString()=="YES") /* && exists */{
//            ui->CreateSolutionButton->setText(tr("Open solution"));
//            DataTex::loadImageFile(Solution,viewSolution);
//        }
//        else{
//            ui->CreateSolutionButton->setText(tr("Create solution"));
//        }
    }
}

void SolveDatabaseExercise::CreateSolution(QString filetype)
{
    QStringList Sections;
    NewSolution.replace(".tex","-"+QString::number(SolutionsCount+1)+".tex");
    if(ExerciseTable->model()->rowCount()>0){
        Sections<<ui->SectionList->currentItem()->data(Qt::UserRole).toString().split(",");
        QSqlQuery solved(currentbase);
        solved.exec(SqlFunctions::UpdateSolution.arg(QFileInfo(Exercise).baseName()));
        QSqlQuery RowValues(currentbase);
        QStringList valuesList;
        RowValues.exec(SqlFunctions::SelestExerciseRow.arg(QFileInfo(Exercise).baseName()));
        while (RowValues.next())
        {
            QSqlRecord record = RowValues.record();
            for(int i=0; i < record.count(); i++)
            {
                valuesList << record.value(i).toString();
            }
        }
        QSqlQuery Columns(currentbase);
        QStringList ColumnNames;
        Columns.exec("SELECT \"name\" FROM pragma_table_info(\'Database_Files\');");
        while (Columns.next()){ColumnNames.append(Columns.value(0).toString());}
        QMap<QString,QString> mapValues;
        for(int i=0;i<ColumnNames.count();i++){
            mapValues.insert(ColumnNames.at(i),valuesList.at(i));
        }
        mapValues.insert("Date",QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm"));
        mapValues.insert("FileType",filetype);
        mapValues.insert("Path",NewSolution);
        mapValues.insert("Solved","-");
        mapValues.insert("Id",QFileInfo(NewSolution).baseName());
        mapValues.remove("Section");
        mapValues.remove("File_Content");

        QDir path(QFileInfo(NewSolution).absolutePath());
        if (!path.exists()){path.mkpath(".");}
        ui->SolutionContent->setText(DataTex::NewFileText(NewSolution));

        //Latex file Metadata - Write new entry to database ---
        QString meta_Ids = mapValues.keys().join("\",\"");
        QString meta_Values = mapValues.values().join("\",\"");
        QStringList WriteValues;
        foreach(QString section,Sections){
            WriteValues.append("(\""+meta_Values+"\",\""+section+"\",\""+FileContent+"\")");
        }
        //-------------------
        QSqlQuery writeExercise(DataTex::CurrentTexFilesDataBase);
        writeExercise.exec("INSERT INTO \"Database_Files\" (\""+meta_Ids+"\",\"Section\",\"FileContent\") VALUES "+WriteValues.join(","));
        emit solution(NewSolution,/*mapValues,Sections,*/ui->SolutionContent->toPlainText());
        ui->SolutionsCombo->addItem(QFileInfo(NewSolution).baseName(),QVariant(NewSolution));
        ui->SolutionsCombo->setCurrentText(QFileInfo(NewSolution).baseName());
    }
}

void SolveDatabaseExercise::on_RecompileButton_clicked()
{
    QString file = ui->SolutionsCombo->currentData().toString();
    SaveText();
    DataTex::CreateTexFile(file);
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],file,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(file);
    DataTex::loadImageFile(file,viewSolution);
//    qDebug()<<file<<DataTex::LatexCommands[CurrentBuildCommand]<<CurrentBuildCommand;
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
    ui->SaveContent->setEnabled(false);
//    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(ui->SolutionsCombo->currentData().toString(),FileContent);
}

void SolveDatabaseExercise::on_CloseButton_clicked(QAbstractButton *button)
{
    accept();
}

//void SolveDatabaseExercise::on_TexPage_clicked(bool checked)
//{
//    if(checked){
//        ui->PdfPage->setChecked(!checked);
//        ui->stackedWidget->setCurrentIndex(0);
//    }
//}

//void SolveDatabaseExercise::on_PdfPage_clicked(bool checked)
//{
//    if(checked){
//        ui->TexPage->setChecked(!checked);
//        ui->stackedWidget->setCurrentIndex(1);
//    }
//}

void SolveDatabaseExercise::on_NewSolutionButton_clicked()
{
    CreateSolution(SolutionType);
}

void SolveDatabaseExercise::on_DeleteCurrentSolution_clicked()
{
    QCheckBox *cb = new QCheckBox(tr("Delete File"));
    QMessageBox msgbox;
    msgbox.setText(tr("Do you want to remove \n the file %1").arg(ui->SolutionsCombo->currentData().toString()));
    msgbox.setIcon(QMessageBox::Icon::Question);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.addButton(QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.setCheckBox(cb);
    if (msgbox.exec() == QMessageBox::Ok) {
        QSqlQuery deleteQuery(DataTex::CurrentTexFilesDataBase);
//        deleteQuery.exec(QString("DELETE FROM \"Database_Files\" WHERE \"Id\" = \"%1\"").arg(Solution));
     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"/*+QFileInfo(Solution).absolutePath()*/));}
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
    DataTex::loadImageFile(ui->SolutionsCombo->currentData().toString(),viewSolution);
}

void SolveDatabaseExercise::on_SaveContent_clicked()
{
    SaveText();
}
