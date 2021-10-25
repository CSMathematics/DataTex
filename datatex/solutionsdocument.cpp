#include "solutionsdocument.h"
#include "ui_solutionsdocument.h"
#include <QFileInfo>

SolutionsDocument::SolutionsDocument(QWidget *parent, QString fileName,
                                     QString BuildCommand,
                                     QStringList Exercises,
                                     QList<QStringList> SolutionsPerExercise) :
    QDialog(parent),
    ui(new Ui::SolutionsDocument)
{
    ui->setupUi(this);
    ExercisesInDocument = Exercises;
    SolutionDocumentName = fileName;
    SolutionDocumentName.replace(QFileInfo(fileName).baseName(),QFileInfo(fileName).baseName()+"-Solutions");
    SolutionsPerExercisePreview = SolutionsPerExercise;
    CurrentBuildCommand = BuildCommand;
    ui->ExercisesInDocument->setColumnCount(4);
    ui->ExercisesInDocument->setColumnHidden(2,true);
    ui->ExercisesInDocument->setColumnHidden(3,true);

    QString FileContent;
    if(QFileInfo::exists(SolutionDocumentName)){
        QFile file(SolutionDocumentName);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        FileContent = text.readAll();
        file.close();
        ui->SolutionsDocumentContent->setText(FileContent);
    }
    else {
        DocumentText();
        FileContent = ui->SolutionsDocumentContent->toPlainText();
        QFile file(SolutionDocumentName);
        file.resize(0);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream Content(&file);
        Content << FileContent;
        file.close();
    }
    ui->ContentLabel->setText(QFileInfo(SolutionDocumentName).baseName());

    view = new PdfViewer(this);
    ui->verticalLayout_2->addWidget(view,1);
    view->show();
    DocView = new QPdfViewer(this);
    ui->splitter_2->insertWidget(2,DocView);
    DocView->show();
    DataTex::loadImageFile(SolutionDocumentName,DocView);

    for(int i=0;i<Exercises.count();i++){
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0,QFileInfo(Exercises[i]).baseName());
        ui->ExercisesInDocument->addTopLevelItem(item);
        for (int k=0;k<SolutionsPerExercise[i].count();k++) {
            QTreeWidgetItem * subitem = new QTreeWidgetItem();
            subitem->setText(0,QFileInfo(SolutionsPerExercise[i].at(k)).baseName());
            ui->ExercisesInDocument->topLevelItem(i)->addChild(subitem);
        }
    }
    for(int i=0;i<Exercises.count();i++){
        ui->ExercisesInDocument->topLevelItem(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        ui->ExercisesInDocument->topLevelItem(i)->setCheckState(0,Qt::Checked);
        ui->ExercisesInDocument->topLevelItem(i)->child(0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        ui->ExercisesInDocument->topLevelItem(i)->child(0)->setCheckState(0,Qt::Checked);
        for (int k=1;k<SolutionsPerExercise[i].count();k++) {
            ui->ExercisesInDocument->topLevelItem(i)->child(k)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            ui->ExercisesInDocument->topLevelItem(i)->child(k)->setCheckState(0,Qt::Unchecked);
        }
    }

    for (int i=0;i<Exercises.count();i++ ) {
        if(SolutionsPerExercise[i].count()>0){
            for(int k=0;k<SolutionsPerExercise[i].count();k++){
                QString solution = QFileInfo(SolutionsPerExercise[i].at(k)).baseName();
                QStringList Content = SqlFunctions::Get_StringList_From_Query
                        (QString("SELECT FileContent FROM Database_Files WHERE Id = \"%1\"").arg(solution),DataTex::CurrentTexFilesDataBase);
                SolutionContent.insert(solution,Content.at(0));
            }
            Solutions[QFileInfo(Exercises[i]).baseName()].append(QFileInfo(SolutionsPerExercisePreview[i][0]).baseName());
        }
    }

    QTreeWidgetItem * subitem = new QTreeWidgetItem();
    subitem->setText(0,"New");
    ui->ExercisesInDocument->topLevelItem(1)->addChild(subitem);
    connect(ui->ExercisesInDocument, SIGNAL(itemClicked(QTreeWidgetItem*,int)),this, SLOT(ItemChecked(QTreeWidgetItem*,int)));
    qDebug()<<Solutions;
    for(int i=0;i<Exercises.count();i++){
        ui->ExercisesInDocument->topLevelItem(i)->setText(0,ui->ExercisesInDocument->topLevelItem(i)->
             text(0)+" ("+QString::number(ui->ExercisesInDocument->topLevelItem(i)->childCount())+" solutions)");
    }
}

void SolutionsDocument::ItemChecked(QTreeWidgetItem * item,int column = 0)
{
    QString CurrentExercise = item->parent()->text(0);
    if(item->checkState(0) == Qt::Checked){
        if(!Solutions[CurrentExercise].contains(item->text(0))){
            Solutions[CurrentExercise].append(item->text(0));
            Solutions[CurrentExercise].sort();
        }
    }
    else{
        Solutions[CurrentExercise].removeAll(item->text(0));
    }
    ui->SolutionsDocumentContent->clear();
    DocumentText();
}

SolutionsDocument::~SolutionsDocument()
{
    delete ui;
}


//void SolutionsDocument::on_pushButton_clicked()
//{
//    for (int i=0;i<Solutions.count() ;i++ ) {
////        qDebug()<<SolutionContent[Solutions[i]];
//    }
//    qDebug()<<Solutions;
//}

void SolutionsDocument::DocumentText()
{
    QString FileContent;
    FileContent = "%# Database Document : "+QFileInfo(SolutionDocumentName).baseName()+"-----------------\n";
    FileContent += "%@ Document type: Solutions....(αλλαγή)\n";
    FileContent += "%#--------------------------------------------------\n\n";
    for(int i=0;i<ExercisesInDocument.count();i++){
        QString Exercise = ui->ExercisesInDocument->topLevelItem(i)->text(0);
        for (int k=0;k<Solutions[Exercise].count();k++) {
            QString solution = Solutions[Exercise][k];
            FileContent += SolutionContent[solution]+"\n\n";
        }
    }
    ui->SolutionsDocumentContent->setText(FileContent);
}

void SolutionsDocument::on_SolutionCombo_currentIndexChanged(int index)
{
    DataTex::loadImageFile(ui->SolutionCombo->currentData().toString(),view);
}


void SolutionsDocument::on_BuildButton_clicked()
{
    SaveText();
    DataTex::CreateTexFile(SolutionDocumentName);
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],SolutionDocumentName
                           ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(SolutionDocumentName);
    DataTex::loadImageFile(SolutionDocumentName,DocView);
}

void SolutionsDocument::SaveText()
{
    QString FileContent = ui->SolutionsDocumentContent->toPlainText();
    QFile file(SolutionDocumentName);
    file.resize(0);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream Content(&file);
    Content << FileContent;
    file.close();
    ui->SaveButton->setEnabled(false);
//    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(SolutionDocumentName,FileContent);
}

void SolutionsDocument::on_ExercisesInDocument_clicked(const QModelIndex &index)
{
    ui->SolutionCombo->clear();
    QModelIndex ix = index;

    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
      depth++;
    }

    if(depth == 0){
        exIndex = index.row();
    }
    else{
        solIndex = index.row();
        exIndex = index.parent().row();
    }
//    qDebug()<<"Depth is = "<<depth<<"Current Exercise is = "<<exIndex<<"Solution number = "<<solIndex;

    QStringList solutions = SolutionsPerExercisePreview[exIndex];
    for (int i=0;i<solutions.count();i++ ) {
        ui->SolutionCombo->addItem(QFileInfo(solutions[i]).baseName(),QVariant(solutions[i]));
    }
    if(depth == 1){
        ui->SolutionCombo->setCurrentIndex(solIndex);
    }
}

