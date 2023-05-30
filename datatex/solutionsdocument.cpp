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
    DocumentName = fileName;
    SolutionDocumentName = fileName;
    SolutionDocumentName.replace(QFileInfo(fileName).baseName(),QFileInfo(fileName).baseName()+"-Solutions");
    SolutionsPerExercisePreview = SolutionsPerExercise;
    databasePerSolutionFile = DatabasePerSolution;
    fileOrder = exerciseOrder;
    CurrentBuildCommand = BuildCommand;
    docReady = false;
    ui->ContentLabel->setText(QFileInfo(SolutionDocumentName).baseName());
    qDebug()<<SolutionsPerExercisePreview;
    view = new QPdfViewer(this);
    ui->verticalLayout_2->addWidget(view,1);
    view->show();
    DocView = new QPdfViewer(this);
    ui->verticalLayout_6->addWidget(DocView);
    DocView->show();
    SolutionsView = new QPdfViewer(this);
    ui->verticalLayout_3->addWidget(SolutionsView);
    SolutionsView->show();
    FileCommands::ShowPdfInViewer(SolutionDocumentName,SolutionsView);
    QMapIterator<int,QString> order(fileOrder);
    while (order.hasNext()) {
        order.next();
        QString exercise = order.value();
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
            item->child(k)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable |Qt::ItemIsSelectable);
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
    }

    connect(ui->ExercisesInDocument, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(ItemChecked(QTreeWidgetItem*,int)));
    for(int i=0;i<SolutionsPerExercisePreview.count();i++){
        ui->ExercisesInDocument->topLevelItem(i)->setText(1," ("+QString::number(ui->ExercisesInDocument->topLevelItem(i)->childCount())+" solutions)");
    }

    ui->DocumentContent->editor->setDocumentTitle(fileName);
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    DocFileContent = text.readAll();
    file.close();
    ui->DocumentContent->editor->setText(DocFileContent);
    FileCommands::ShowPdfInViewer(fileName,DocView);

    QString FileContent;
    if(QFileInfo::exists(SolutionDocumentName)){
        QFile file(SolutionDocumentName);
        file.open(QIODevice::ReadOnly);
        QTextStream text(&file);
        FileContent = text.readAll();
        file.close();
        qDebug()<<FileContent;
        ui->SolutionsDocumentContent->editor->setText(FileContent);
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
        FileContent = ui->SolutionsDocumentContent->editor->toPlainText();
        QFile file(SolutionDocumentName);
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
    int width = ui->splitter_2->geometry().width();
    ui->splitter_2->setSizes(QList<int>{width/3,width/3,width/3});
    connect(ui->ShowTex,&QPushButton::toggled,this,[&](bool checked){
        ui->stackedWidget_2->setCurrentIndex(!checked);
    });
    connect(ui->ShowTexSolution,&QPushButton::toggled,this,[&](bool checked){
        ui->stackedWidget->setCurrentIndex(!checked);
    });
    ui->ShowTex->setChecked(true);
    ui->ShowTexSolution->setChecked(true);
    ui->TopToolBar->setChecked(true);
    ui->SideToolBar->setChecked(true);
    ui->TopToolBarSol->setChecked(true);
    ui->SideToolBarSol->setChecked(true);
    connect(ui->TopToolBar,&QPushButton::toggled,this,[&](bool checked){
        ui->DocumentContent->toolBar->setVisible(checked);
    });
    connect(ui->SideToolBar,&QPushButton::toggled,this,[&](bool checked){
        ui->DocumentContent->mathBar->setVisible(checked);
    });
    connect(ui->TopToolBarSol,&QPushButton::toggled,this,[&](bool checked){
        ui->SolutionsDocumentContent->toolBar->setVisible(checked);
    });
    connect(ui->SideToolBarSol,&QPushButton::toggled,this,[&](bool checked){
        ui->SolutionsDocumentContent->mathBar->setVisible(checked);
    });
    ui->ShowDocument->setChecked(true);
    connect(ui->DocumentContent->toolBar->Save,&QAction::triggered,this,[&](){
        if(ui->ShowDocument->isChecked()){
            DocFileContent = ui->DocumentContent->editor->toPlainText();
        }
    });
    connect(ui->ShowDocument,&QPushButton::toggled,this,[=](bool checked){
        ui->stackedWidget->setCurrentIndex(!checked);
        ui->stackedWidget_2->setCurrentIndex(0);
        ui->ShowPdf->setEnabled(checked);
        ui->ShowTexSolution->setEnabled(checked);
        ui->TopToolBarSol->setEnabled(checked);
        ui->SideToolBarSol->setEnabled(checked);
        ui->BuildButtonDoc->setEnabled(checked);
        if(checked){
            ui->DocumentContent->editor->setText(DocFileContent);
            ui->DocumentContent->editor->setDocumentTitle(fileName);
        }
        else{
            ui->DocumentContent->editor->setText(FileContent);
            ui->DocumentContent->editor->setDocumentTitle(SolutionDocumentName);
        }
    });
    DocumentText();
    ui->DocumentContent->editor->setDocumentTitle(fileName);
    ui->DocumentContent->toolBar->Save->setEnabled(false);
    ui->SolutionsDocumentContent->toolBar->Save->setEnabled(false);
    ui->SolutionsDocumentContent->editor->setDocumentTitle(SolutionDocumentName);
    docReady = true;
    ui->ExercisesInDocument->expandAll();
    connect(ui->QuickSolution,&QPushButton::clicked,this,[&](){
        for (int i = 0; i < ui->ExercisesInDocument->topLevelItemCount(); ++i) {
            ui->ExercisesInDocument->topLevelItem(i)->child(0)->setCheckState(0,Qt::Checked);
        }
    });
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
    if(ui->ShowDocument->isChecked()){
        ui->DocumentContent->editor->setText(DocFileContent);
        ui->DocumentContent->editor->setDocumentTitle(DocumentName);
        ui->SolutionsDocumentContent->editor->setText(FileContent);
        ui->SolutionsDocumentContent->editor->setDocumentTitle(SolutionDocumentName);
    }
    else{
        ui->DocumentContent->editor->setText(FileContent);
        ui->DocumentContent->editor->setDocumentTitle(SolutionDocumentName);
    }
}

void SolutionsDocument::on_BuildButton_clicked()
{
    ui->SolutionsDocumentContent->toolBar->Save->trigger();
    FileCommands::CreateTexFile(SolutionDocumentName,0,"");
    qDebug()<<SolutionDocumentName;
    FileCommands::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],SolutionDocumentName
                           ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    FileCommands::ClearOldFiles(SolutionDocumentName);
    FileCommands::ShowPdfInViewer(SolutionDocumentName,SolutionsView);
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
        FileCommands::ShowPdfInViewer(ui->ExercisesInDocument->topLevelItem(exIndex)->child(solIndex)->text(1),view);
    }
    CurrentExercise = ui->ExercisesInDocument->topLevelItem(exIndex)->text(0);
}

void SolutionsDocument::on_BuildButtonDoc_clicked()
{
    ui->DocumentContent->toolBar->Save->trigger();
    FileCommands::CreateTexFile(DocumentName,0,"");
    FileCommands::BuildDocument(DataTex::LatexCommands[CurrentBuildCommand],DocumentName
                           ,DataTex::LatexCommandsArguments[CurrentBuildCommand],".tex");
    FileCommands::ClearOldFiles(DocumentName);
    FileCommands::ShowPdfInViewer(DocumentName,DocView);
}

void SolutionsDocument::findSolution()
{

}
