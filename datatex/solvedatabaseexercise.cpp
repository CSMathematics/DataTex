#include "solvedatabaseexercise.h"
#include "ui_solvedatabaseexercise.h"
#include <math.h>


SolveDatabaseExercise::SolveDatabaseExercise(QWidget *parent, QString fileName) :
    QDialog(parent),
    ui(new Ui::SolveDatabaseExercise)
{
    ui->setupUi(this);
    currentbase = DataTex::CurrentFilesDataBase.Database;
    ExerciseTable = new ExtendedTableWidget(this);
    ui->gridLayout->addWidget(ExerciseTable,1,0);
    ExerciseTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ExerciseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ExerciseTable->horizontalHeader()->setSectionsClickable(true);
    ExerciseTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding);

    connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
    exercise = new LatexTextWidget(this);
    ui->verticalLayout_4->addWidget(exercise);
    exercise->show();
    view = new QPdfViewer(this);
    ui->verticalLayout_5->addWidget(view);
    view->show();

    viewSolution = new QPdfViewer(this);
    ui->gridLayout_10->addWidget(viewSolution);
    viewSolution->show();

    ui->SolutionContent->editor->setReadOnly(true);
    connect(ui->SolutionContent->editor,&QTextEdit::textChanged,this,[=](){
        ui->SolutionContent->toolBar->Save->setEnabled(true);
    });


    ui->splitter->setSizes(QList<int>({150,400}));
//    ui->scrollArea->setMinimumWidth(200);
    bool hasSolution = ui->SolutionsCombo->count()>0;
    ui->DeleteCurrentSolution->setEnabled(hasSolution);
    ui->SolutionContent->toolBar->Save->setEnabled(hasSolution);

    connect(ui->SolutionContent->editor,&QTextEdit::textChanged,this,[=](){
        ui->SolutionContent->toolBar->Save->setEnabled(true);
    });
    exer_Toolbar = new FileToolBar(exercise);
    ui->horizontalLayout->insertWidget(1,exer_Toolbar);
    connect(exer_Toolbar->ShowTex,&QAction::triggered,this,[&](){ui->stackedWidget_2->setCurrentIndex(0);});
    connect(exer_Toolbar->ShowPdf,&QAction::triggered,this,[&](){ui->stackedWidget_2->setCurrentIndex(1);});
    connect(exer_Toolbar->BuildFile,&QAction::triggered,this,[&](){RebuildExercise();});
    exer_Toolbar->show();

    sol_Toolbar = new FileToolBar(ui->SolutionContent);
    ui->horizontalLayout_4->insertWidget(1,sol_Toolbar);
    connect(sol_Toolbar->ShowTex,&QAction::triggered,this,[&](){ui->stackedWidget->setCurrentIndex(0);});
    connect(sol_Toolbar->ShowPdf,&QAction::triggered,this,[&](){ui->stackedWidget->setCurrentIndex(1);});
    connect(sol_Toolbar->BuildFile,&QAction::triggered,this,[&](){RebuildSolution();});
    sol_Toolbar->show();
    sol_Toolbar->BuildFile->setEnabled(hasSolution);

    connect(ui->SplitTexPdf,&QPushButton::toggled,this,[&](bool checked){
        ui->splitter->setOrientation(static_cast<Qt::Orientation>(2-checked));
        ui->splitter_2->setSizes(QList<int>({(1-checked)*width()/2,(1+checked)*width()/2}));
        ui->splitter_2->setHandleWidth(4*checked);
        ExerciseTable->setHidden(checked);
    });
    ui->splitter->setSizes(QList<int>({height()/2,height()/2}));
    ui->SolutionReady->setEnabled(false);
    ui->SolutionNotReady->setEnabled(false);
    connect(ui->SolutionReady,&QPushButton::toggled,this,[&](bool checked){
        QSqlQuery setSolutionState(currentbase);
        int solutionState = (checked) ? (int)DTXSolutionState::SolutionComplete : (int)DTXSolutionState::SolutionIncomplete;
        setSolutionState.exec(QString("UPDATE Database_Files SET Solved_Prooved = '%1' WHERE Id = '%2'").arg(QString::number(solutionState),ui->SolutionsCombo->currentText()));

        int i = 0;
        for(const QString &solution : qAsConst(Solutions)){
            i += SqlFunctions::Get_String_From_Query(
                     QString("SELECT Solved_Prooved FROM Database_Files WHERE Id = '%1'").arg(QFileInfo(solution).baseName()),currentbase).toInt();
        }
        int fileState = (i==(Solutions.count()*(int)DTXSolutionState::SolutionComplete)) ? (int)DTXSolutionState::Solved : (int)DTXSolutionState::HasIncompleteSolutions;
        qDebug()<<i<<fileState<<(Solutions.count()*(int)DTXSolutionState::SolutionComplete);
        setSolutionState.exec(QString("UPDATE Database_Files SET Solved_Prooved = '%1' WHERE Id = '%2'").arg(QString::number(fileState),QFileInfo(Exercise).baseName()));
        ui->SolutionReady->setChecked(checked);
        ui->SolutionNotReady->setChecked(!checked);
    });

    Database_FileTableFields = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentFilesDataBase.Database);
    QStringList Database_HeaderNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM BackUp WHERE Table_Id = 'Metadata'",DataTex::CurrentFilesDataBase.Database);
    QSqlQueryModel * Model = new QSqlQueryModel(this);
    QSqlQuery tableQuery(currentbase);


    QString query = SqlFunctions::ShowAllFiles_SELECT;
    query += QString("WHERE (ft.Solvable = %1 OR ft.Solvable = %2 OR ft.Solvable = %3) "
                     "GROUP BY df.Id ORDER BY df.rowid;").arg(QString::number((int)DTXSolutionState::Solvable),
                      QString::number((int)/*(DTXSolutionState::Solvable | */DTXSolutionState::Solved),
                      QString::number((int)/*(DTXSolutionState::Solvable | */DTXSolutionState::UnSolved),
                      QString::number((int)/*(DTXSolutionState::Solvable | */DTXSolutionState::HasIncompleteSolutions));
    tableQuery.exec(query);
    qDebug()<<query;
    Model->setQuery(tableQuery);
    ExerciseTable->setModel(Model);
    ExerciseTable->show();
    ExerciseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ExerciseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ExerciseTable->setAlternatingRowColors(true);
    DataTex::LoadTableHeaders(ExerciseTable,Database_HeaderNames);
    ExerciseTable->setSelectionMode( QAbstractItemView::SingleSelection );
    int columns = ExerciseTable->model()->columnCount();
    ExerciseTable->generateFilters(columns,false);
    connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
    connect(ExerciseTable->filterHeader(), &FilterTableHeader::filterValues, this, &SolveDatabaseExercise::updateFilter);
    if(!fileName.isEmpty()){
        DataTex::SelectNewFileInModel(ExerciseTable,fileName);
    }
    connect(ui->EnableSortingFiles,&QPushButton::toggled,this,[&](bool checked){
        filesSorting = checked;
        updateFilter(ExerciseTable->filterHeader()->getFilterValues());
        ExerciseTable->filterHeader()->adjustPositions();
    });
    connect(ui->ShowSolved,&QPushButton::toggled,this,[&](bool checked){
        if(checked){
            ExerciseTable->filterHeader()->setFilter(9,QString::number(checked));
            updateFilter(ExerciseTable->filterHeader()->getFilterValues());
            ui->ShowUnsolved->setChecked(false);
        }
        else{
            ExerciseTable->filterHeader()->clearFilters();
        }
    });
    connect(ui->ShowUnsolved,&QPushButton::toggled,this,[&](bool checked){
        if(checked){
            ExerciseTable->filterHeader()->setFilter(9,QString::number(!checked));
            updateFilter(ExerciseTable->filterHeader()->getFilterValues());
            ui->ShowSolved->setChecked(false);
        }
        else{
            ExerciseTable->filterHeader()->clearFilters();
        }
    });
    connect(ui->SolutionContent->toolBar->Save,&QAction::triggered,this,[&](){
//        DTXFile fileInfo = DTXFile(ui->SolutionsCombo->currentText(),currentbase);
//        DTXFile::WriteDTexFile(fileInfo);
//        delete fileInfo;
    });
}

SolveDatabaseExercise::~SolveDatabaseExercise()
{
    delete ui;
    delete view;
    delete exercise;
    delete viewSolution;
    delete ExerciseTable;
    delete exer_Toolbar;
    delete sol_Toolbar;
}

void SolveDatabaseExercise::updateFilter(QStringList values)
{
    SqlFunctions::FilterTable(Database_FileTableFields,values,{"ft.Solvable > 0"});
    DataTex::updateTableView(ExerciseTable,SqlFunctions::FilesTable_UpdateQuery,currentbase,this);
    connect(ExerciseTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SolveDatabaseExercise::ExerciseTable_SelectionChanged);
//    DataTex::LoadTableHeaders(FilesTable,Database_FileTableFieldNames);
}

void SolveDatabaseExercise::ExerciseTable_SelectionChanged()
{
    if(ExerciseTable->currentIndex().row()>-1){
        Exercise.clear();
        int row = ExerciseTable->currentIndex().row();
//        QString FileType = ExerciseTable->model()->data(ExerciseTable->model()->index(row,ExerciseTable->model()->columnCount()-3)).toString();
        Exercise = ExerciseTable->model()->data(ExerciseTable->model()->index(row,7)).toString();
        DataTex::CurrentPreamble = ExerciseTable->model()->data(ExerciseTable->model()->index(row,12)).toString();
        CurrentBuildCommand = ExerciseTable->model()->data(ExerciseTable->model()->index(row,13)).toString();
//        QString tempFile; = Exercise;
//        tempFile.replace("-"+FileType,"-"+FileTypeSolIds[FileType]);
//        tempFile.replace(FileTypeFolders[FileType],FileTypeFoldersSol[FileType]);
////        qDebug()<<tempFile<<FileType;
//        QDirIterator list(QFileInfo(tempFile).absolutePath(),QStringList() << QFileInfo(tempFile).baseName()+"-*.tex", QDir::Files);
//        while (list.hasNext()){
//            Solutions.append(list.next());
//        }
        Solutions = SqlFunctions::Get_StringList_From_Query(QString("SELECT d.Path FROM Solutions_per_File s "
                                                                   "JOIN Database_Files d ON s.Solution_Id = d.Id "
                                                                   "WHERE File_Id = '%1'").arg(QFileInfo(Exercise).baseName()),currentbase);
        FileCommands::ShowPdfInViewer(Exercise,view);
        QFile file(Exercise);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        text.flush();
        QString Content = text.readAll();
        file.close();
        exercise->editor->setText(Content);
        exercise->editor->setDocumentTitle(Exercise);
        ui->SolutionsCombo->clear();
        for (int i=0;i<Solutions.count();i++) {
            ui->SolutionsCombo->addItem(QFileInfo(Solutions[i]).baseName(),QVariant(Solutions[i]));
        }
        ui->SolutionsCombo->model()->sort(Qt::AscendingOrder);
        ui->DeleteCurrentSolution->setEnabled(ui->SolutionsCombo->count()>0);
        ui->SolutionContent->toolBar->Save->setEnabled(false);
        ui->SolutionContent->editor->setDocumentTitle(ui->SolutionsCombo->currentData().toString());
        ui->SolutionReady->setEnabled(true);
        ui->SolutionNotReady->setEnabled(true);
    }
}

void SolveDatabaseExercise::CreateSolution()
{
    ui->DeleteCurrentSolution->setEnabled(true);
    sol_Toolbar->BuildFile->setEnabled(true);
//    QString file = tempFile;
//    file.remove(".tex");
//    int i = 1;
//    while(QFileInfo::exists(file+"-"+QString::number(i)+".tex")){
//        i++;
//    }
//    NewSolution = file+"-"+QString::number(i)+".tex";
    QString ExerciseName = QFileInfo(Exercise).baseName();
//    QString SolutionName = QFileInfo(NewSolution).baseName();
    if(ExerciseTable->model()->rowCount()>0){
        DTXFile * solInfo = new DTXFile(ExerciseName,currentbase);//
        qDebug()<<"Askhsh "<<ExerciseName;
        solInfo = FileCommands::CreateSolutionData(solInfo,currentbase);
        FileCommands::AddNewFileToDatabase(solInfo,currentbase);
        QSqlQuery solved(currentbase);
        solved.exec(QString("INSERT INTO Solutions_per_File (Solution_Id,Solution_Path,File_Id) VALUES ('%1', '%2','%3');").arg(solInfo->Id,solInfo->Path,ExerciseName));
        solved.exec(SqlFunctions::UpdateSolution.arg(ExerciseName));
//        QSqlQuery writeExercise(DataTex::CurrentFilesDataBase.Database);
//        QStringList ColumnNames = SqlFunctions::Get_StringList_From_Query("SELECT \"name\" FROM pragma_table_info(\'Database_Files\');",currentbase);
//        QStringList NewValues = ColumnNames;
//        NewValues[0] = "\""+SolutionName+"\"";
//        NewValues[1] = "\""+FileTypeSolIds[FileType]+"\"";
//        NewValues[4] = "\""+NewSolution+"\"";
//        NewValues[5] = "\""+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"\"";
//        NewValues[6] = "\"0\"";
//        NewValues[8] = "\""+FileCommands::NewFileText(NewSolution,"")+"\"";
//        writeExercise.exec(QString("INSERT INTO Database_Files ("+ColumnNames.join(",")+") "
//                           "SELECT "+NewValues.join(",")+" FROM Database_Files "
//                           "WHERE Id = '%1'").arg(ExerciseName));
//        qDebug()<<QString("INSERT INTO Database_Files ("+ColumnNames.join(",")+") "
//                                                                                     "SELECT "+NewValues.join(",")+" FROM Database_Files "
//                                                    "WHERE Id = '%1'").arg(ExerciseName);
//        QDir path(QFileInfo(NewSolution).absolutePath());
//        if (!path.exists()){path.mkpath(".");}
        ui->SolutionContent->editor->setText(FileCommands::NewFileText(solInfo->Path,""));

//        writeExercise.exec(QString("INSERT INTO Chapters_per_File (File_Id,Chapter_Id) SELECT \"%1\",Chapter_Id FROM Chapters_per_File "
//                                   "WHERE File_Id = '%2' ").arg(SolutionName,(ExerciseName)));
//        writeExercise.exec(QString("INSERT INTO Sections_per_File (File_Id,Section_Id) SELECT \"%1\",Section_Id FROM Sections_per_File "
//                                   "WHERE File_Id = '%2' ").arg(SolutionName,(ExerciseName)));
//        writeExercise.exec(QString("INSERT INTO ExerciseTypes_per_File (File_Id,ExerciseType_Id) SELECT \"%1\",ExerciseType_Id FROM ExerciseTypes_per_File "
//                                   "WHERE File_Id = '%2' ").arg(SolutionName,(ExerciseName)));

        emit solution(solInfo->Path,ui->SolutionContent->editor->toPlainText());
        ui->SolutionsCombo->addItem(solInfo->Id,QVariant(solInfo->Path));
        ui->SolutionsCombo->setCurrentText(solInfo->Id);
        ui->SolutionContent->editor->setDocumentTitle(solInfo->Path);

        solInfo->WriteDTexFile();
        CsvFunctions::WriteDataToCSV(solInfo->Path,currentbase);
        delete solInfo;
        ui->SolutionReady->setEnabled(true);
    }
}

void SolveDatabaseExercise::RebuildSolution()
{
    QString file = ui->SolutionsCombo->currentData().toString();
    ui->SolutionContent->toolBar->Save->trigger();
    FileCommands::CreateTexFile(file,0,"");
    FileCommands::BuildDocument(DataTex::DTXBuildCommands[(int)CompileEngine::PdfLaTeX],file);//CurrentBuildCommand
    FileCommands::ClearOldFiles(file);
    FileCommands::ShowPdfInViewer(file,viewSolution);
}

void SolveDatabaseExercise::on_CloseButton_clicked()
{
    if(ui->SolutionContent->toolBar->Save->isEnabled()){
        QMessageBox::StandardButton resBtn =
                QMessageBox::warning( this,tr("Error"),tr("There are unsaved changes in\nthe current solution.\n"
                                                          "Do you want to save the content?"),
                                      QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if (resBtn == QMessageBox::No) {
            return;
        };
        ui->SolutionContent->toolBar->Save->trigger();
    }
    emit on_close(ui->SolutionsCombo->currentText());
    accept();
}

void SolveDatabaseExercise::on_NewSolutionButton_clicked()
{
    ui->SolutionContent->editor->setReadOnly(false);
    CreateSolution();
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
        QSqlQuery deleteQuery(DataTex::CurrentFilesDataBase.Database);
        deleteQuery.exec("PRAGMA foreign_keys = ON");
        deleteQuery.exec(QString("DELETE FROM Database_Files WHERE Id = \"%1\"").arg(ui->SolutionsCombo->currentText()));
        deleteQuery.exec(SqlFunctions::UpdateSolution.arg(QFileInfo(Exercise).baseName()));
        if(cb->isChecked()==true){
            QFile file(ui->SolutionsCombo->currentData().toString());
            file.remove();
        }
        ui->SolutionsCombo->removeItem(ui->SolutionsCombo->currentIndex());
    }
}

void SolveDatabaseExercise::on_SolutionsCombo_currentTextChanged(QString id)
{
    QString solution = ui->SolutionsCombo->currentData().toString();
    ui->SolutionContent->editor->setDocumentTitle(solution);
    QFile file(solution);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.flush();
    ui->SolutionContent->editor->setText(text.readAll());
    file.close();
    ui->SolutionContent->toolBar->Save->setEnabled(false);
    if(ui->SolutionsCombo->count()>0){
        FileCommands::ShowPdfInViewer(solution,viewSolution);
    }
    else{
        FileCommands::ShowPdfInViewer("",viewSolution);
    }
//    qDebug()<<"sol "<<solution;
    int s = SqlFunctions::Get_String_From_Query(QString("SELECT Solved_Prooved FROM Database_Files WHERE Id = '%1'").arg(id),currentbase).toInt();
//    qDebug()<<1-s%2;
    ui->SolutionReady->setChecked(1-s%2);
    ui->SolutionNotReady->setChecked(s%2);
}

void SolveDatabaseExercise::closeEvent (QCloseEvent *event)
{
    on_CloseButton_clicked();
}

void SolveDatabaseExercise::RebuildExercise()
{
    QString file = exercise->editor->documentTitle();
    exercise->toolBar->Save->trigger();
    FileCommands::CreateTexFile(file,0,"");
    FileCommands::BuildDocument(DataTex::DTXBuildCommands[(int)CompileEngine::PdfLaTeX],file);//CurrentBuildCommand
    FileCommands::ClearOldFiles(file);
    FileCommands::ShowPdfInViewer(file,view);
}
