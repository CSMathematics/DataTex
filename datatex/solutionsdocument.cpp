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
    SolutionDocumentName = fileName;
    SolutionDocumentName.replace(QFileInfo(fileName).baseName(),QFileInfo(fileName).baseName()+"-Solutions");
    SolutionsPerExercisePreview = SolutionsPerExercise;
    CurrentBuildCommand = BuildCommand;
    ui->ExercisesInDocument->setColumnCount(4);
    ui->ExercisesInDocument->setColumnHidden(2,true);
    ui->ExercisesInDocument->setColumnHidden(3,true);
    ui->ExercisesInDocument->horizontalHeader()->setStretchLastSection(true);
    for (int i=0;i<Exercises.count();i++ ) {
        ui->ExercisesInDocument->insertRow(i);
        ui->ExercisesInDocument->setItem(i, 0 , new QTableWidgetItem(QFileInfo(Exercises[i]).baseName()));
        ui->ExercisesInDocument->setItem(i, 2 , new QTableWidgetItem(Exercises[i]));
        QListWidget * list = new QListWidget(this);
        for(int k=0;k<SolutionsPerExercise[i].count();k++){
            QString solution = QFileInfo(SolutionsPerExercise[i].at(k)).baseName();
            list->addItem(solution);
            list->item(k)->setFlags(list->item(k)->flags() | Qt::ItemIsUserCheckable);
            list->item(k)->setCheckState(Qt::Unchecked);
            QStringList Content = SqlFunctions::Get_StringList_From_Query
                    (QString("SELECT FileContent FROM Database_Files WHERE Id = \"%1\"").arg(solution),DataTex::CurrentTexFilesDataBase);
            SolutionContent.insert(solution,Content.at(0));
        }
        list->item(0)->setCheckState(Qt::Checked);
        list->sortItems();
        Solutions[QFileInfo(Exercises[i]).baseName()].append(list->item(0)->text());
        connect(list, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(ItemChecked(QListWidgetItem*)));
        ui->ExercisesInDocument->setCellWidget(i,1,list);

        int items = SolutionsPerExercise[i].count();
        int height = list->visualItemRect(list->item(0)).height();
        list->setSizeAdjustPolicy(QListWidget::AdjustToContents);
        ui->ExercisesInDocument->setRowHeight(i,items*height+height/2);
    }
    DocumentText();
    ui->ContentLabel->setText(QFileInfo(SolutionDocumentName).baseName());

    view = new PdfViewer(this);
//    view->setMinimumWidth(620);
    ui->verticalLayout_2->addWidget(view,1);
    view->show();
    DocView = new QPdfViewer(this);
    ui->splitter_2->insertWidget(2,DocView);
//    ui->verticalLayout_3->addWidget(DocView,1);
    DocView->show();

    QString FileContent = ui->SolutionsDocumentContent->toPlainText();
    if(!QFileInfo::exists(SolutionDocumentName)){
        QFile file(SolutionDocumentName);
        file.resize(0);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream Content(&file);
        Content << FileContent;
        file.close();
    }
    DataTex::loadImageFile(SolutionDocumentName,DocView);
}

void SolutionsDocument::ItemChecked(QListWidgetItem * item)
{
    QString CurrentExercise = ui->ExercisesInDocument->item(ui->ExercisesInDocument->currentRow(),0)->text();
    if(item->checkState() == Qt::Checked){
        if(!Solutions[CurrentExercise].contains(item->text())){
            Solutions[CurrentExercise].append(item->text());
            Solutions[CurrentExercise].sort();
        }
    }
    else{
        Solutions[CurrentExercise].removeAll(item->text());
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
    QString text;
    text = "%# Database Document : "+QFileInfo(SolutionDocumentName).baseName()+"-----------------\n";
    text += "%@ Document type: Solutions....(αλλαγή)\n";
    text += "%#--------------------------------------------------\n\n";
    for(int i=0;i<ui->ExercisesInDocument->rowCount();i++){
        QString Exercise = ui->ExercisesInDocument->item(i,0)->text();
        for (int k=0;k<Solutions[Exercise].count();k++) {
            QString solution = Solutions[Exercise][k];
            text += SolutionContent[solution]+"\n\n";
        }
    }

    ui->SolutionsDocumentContent->setText(text);
}


void SolutionsDocument::on_ExercisesInDocument_itemClicked(QTableWidgetItem *item)
{
    ui->SolutionCombo->clear();
    int row = item->row();
    QStringList solutions = SolutionsPerExercisePreview[row];
    for (int i=0;i<solutions.count();i++ ) {
        ui->SolutionCombo->addItem(QFileInfo(solutions[i]).baseName(),QVariant(solutions[i]));
    }
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
