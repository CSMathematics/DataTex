#include "solutionsdocument.h"
#include "ui_solutionsdocument.h"
#include <QFileInfo>

SolutionsDocument::SolutionsDocument(QWidget *parent, QString fileName,
                                     QString BuildCommand,
                                     QHash<QString,QStringList> SolutionsPerExercise,
                                     QHash<QString,QSqlDatabase> DatabasePerSolution,
                                     QMap<int, QString> exerciseOrder) :
    QDialog(parent),
    ui(new Ui::SolutionsDocument)
{
    ui->setupUi(this);
    SolutionDocumentName = fileName;
    SolutionDocumentName.replace(QFileInfo(fileName).baseName(),QFileInfo(fileName).baseName()+"-Solutions");
    SolutionsPerExercisePreview = SolutionsPerExercise;
    databasePerSolutionFile = DatabasePerSolution;
    fileOrder = exerciseOrder;
    CurrentBuildCommand = BuildCommand;
    docReady = false;
    ui->ContentLabel->setText(QFileInfo(SolutionDocumentName).baseName());
    qDebug()<<SolutionsPerExercisePreview;
    view = new PdfViewer(this);
    ui->verticalLayout_2->addWidget(view,1);
    view->show();
    DocView = new PdfViewer(this);
    ui->splitter_2->insertWidget(2,DocView);
    DocView->show();
    DataTex::loadImageFile(SolutionDocumentName,DocView);
    QMapIterator<int,QString> order(fileOrder);
    while (order.hasNext()) {
        order.next();
        QString exercise = order.value();
//        cout << order.key() << ": " << order.value() << Qt::endl;
//    }
//    foreach(QString exercise,SolutionsPerExercisePreview.keys()){
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0,exercise);
        item->setText(2,DataTex::GlobalFilesDatabaseListNames[QFileInfo(databasePerSolutionFile[exercise].databaseName()).baseName()]);
        ui->ExercisesInDocument->addTopLevelItem(item);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(0,Qt::Checked);
        for (int k=0;k<SolutionsPerExercisePreview[exercise].count();k++) {
            QString solution = QFileInfo(SolutionsPerExercisePreview[exercise][k]).baseName();
            QTreeWidgetItem * subitem = new QTreeWidgetItem();
            subitem->setText(0,solution);
            subitem->setText(1,SolutionsPerExercisePreview[exercise][k]);
            item->addChild(subitem);
            item->child(k)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            QStringList Content = SqlFunctions::Get_StringList_From_Query
                (QString("SELECT FileContent FROM Database_Files WHERE Id = \"%1\"").arg(solution),
                        databasePerSolutionFile[exercise]);
            if(!Content.at(0).isEmpty()){
                SolutionContent.insert(solution,Content.at(0));
            }
            else{
                SolutionContent.insert(solution,tr("The exercise ")+exercise+tr("has no solutions"));
            }
        }
//        Solutions[exercise].append(QFileInfo(SolutionsPerExercisePreview[exercise][0]).baseName());
    }

    connect(ui->ExercisesInDocument, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(ItemChecked(QTreeWidgetItem*,int)));
    for(int i=0;i<SolutionsPerExercisePreview.count();i++){
        ui->ExercisesInDocument->topLevelItem(i)->setText(1," ("+QString::number(ui->ExercisesInDocument->topLevelItem(i)->childCount())+" solutions)");
    }

    ui->ExercisesInDocument->header()->resizeSection(0,200);
    QString FileContent;
    if(QFileInfo::exists(SolutionDocumentName)){
        QFile file(SolutionDocumentName);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        FileContent = text.readAll();
        file.close();
        ui->SolutionsDocumentContent->setText(FileContent);
        QStringList solutionsNamesInDocument;
        QString path_rem = "%# Database File : ";
        QTextStream Line(&FileContent);
        while (!Line.atEnd()){
            QString LineText=Line.readLine();
            if(LineText.contains(path_rem))solutionsNamesInDocument.append(LineText.remove(path_rem));
        }
        for (int i = 0; i < ui->ExercisesInDocument->topLevelItemCount();i++) {
            for (int k = 0; k < ui->ExercisesInDocument->topLevelItem(i)->childCount();k++) {
                QString sol = ui->ExercisesInDocument->topLevelItem(i)->child(k)->text(0);
                if(solutionsNamesInDocument.contains(sol)){
                    ui->ExercisesInDocument->topLevelItem(i)->child(k)->setCheckState(0,Qt::Checked);
                }
                else{
                    ui->ExercisesInDocument->topLevelItem(i)->child(k)->setCheckState(0,Qt::Unchecked);
                }
            }
        }
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
        for (int i = 0; i < ui->ExercisesInDocument->topLevelItemCount();i++) {
            for (int k = 0; k < ui->ExercisesInDocument->topLevelItem(i)->childCount();k++) {
                if(k>0){
                    ui->ExercisesInDocument->topLevelItem(i)->child(k)->setCheckState(0,Qt::Unchecked);
                }
                else{
                    ui->ExercisesInDocument->topLevelItem(i)->child(k)->setCheckState(0,Qt::Checked);
                }
            }
        }
    }
    ui->splitter_2->setSizes(QList<int>{150,150,150});
    DocumentText();
    connect(ui->SaveButton,&QPushButton::clicked,this,&SolutionsDocument::SaveText);
    docReady = true;
}

void SolutionsDocument::ItemChecked(QTreeWidgetItem * item,int column = 0)
{
    QString CurrentExercise;

    if(item->parent()){
    CurrentExercise = item->parent()->text(0);
        if(item->checkState(0) == Qt::Checked){
            if(!Solutions[CurrentExercise].contains(item->text(0))){
                Solutions[CurrentExercise].append(item->text(0));
                Solutions[CurrentExercise].sort();
            }
        }
        else{
            Solutions[CurrentExercise].removeAll(item->text(0));
        }
    }
    else if(!item->parent() && docReady){
        CurrentExercise = item->text(0);
        if(item->checkState(0) == Qt::Checked){
            for (int i=0;i<item->childCount();i++) {
                item->child(i)->setCheckState(0,Qt::Checked);
                if(!Solutions[CurrentExercise].contains(item->child(i)->text(0))){
                    Solutions[CurrentExercise].append(item->child(i)->text(0));
                    Solutions[CurrentExercise].sort();
                }
            }
        }
        else{
            for (int i=0;i<item->childCount();i++) {
                item->child(i)->setCheckState(0,Qt::Unchecked);
                Solutions[CurrentExercise].removeAll(item->child(i)->text(0));
            }
        }
    }
    ui->SolutionsDocumentContent->clear();
    DocumentText();
}

SolutionsDocument::~SolutionsDocument()
{
    delete ui;
}

void SolutionsDocument::DocumentText()
{
    QString FileContent;
    FileContent = "%# Database Document : "+QFileInfo(SolutionDocumentName).baseName()+"-----------------\n";
    FileContent += "%@ Document type: Solutions\n";
    FileContent += "%#--------------------------------------------------\n\n";
    for(int i=0;i<ui->ExercisesInDocument->topLevelItemCount();i++){
        QString Exercise = ui->ExercisesInDocument->topLevelItem(i)->text(0);
        for (int k=0;k<Solutions[Exercise].count();k++) {
            QString solution = Solutions[Exercise][k];
            FileContent += SolutionContent[solution]+"\n\n";
        }
    }
    ui->SolutionsDocumentContent->setText(FileContent);
}

void SolutionsDocument::on_BuildButton_clicked()
{
    SaveText();
    DataTex::CreateTexFile(SolutionDocumentName,0,"");
    DataTex::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],SolutionDocumentName
                           ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    DataTex::ClearOldFiles(SolutionDocumentName);
    DataTex::loadImageFile(SolutionDocumentName,DocView);
}

void SolutionsDocument::SaveText()
{
    QString FileContent = ui->SolutionsDocumentContent->toPlainText();
    QFile file(SolutionDocumentName);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.resize(0);
    QTextStream Content(&file);
    Content << FileContent;
    file.close();
    ui->SaveButton->setEnabled(false);
//    UndoTex->setEnabled(false);
    DataTex::SaveContentToDatabase(SolutionDocumentName,FileContent);
}

void SolutionsDocument::on_ExercisesInDocument_clicked(const QModelIndex &index)
{
    int solIndex = 0;
    int exIndex = -1;
    if(!index.parent().isValid()){
        exIndex = index.row();
    }
    else{
        solIndex = index.row();
        exIndex = index.parent().row();
        DataTex::loadImageFile(ui->ExercisesInDocument->topLevelItem(exIndex)->child(solIndex)->text(1),view);
    }
    CurrentExercise = ui->ExercisesInDocument->topLevelItem(exIndex)->text(0);
}




