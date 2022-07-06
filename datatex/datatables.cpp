#include "datatables.h"
#include "ui_datatables.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <QTabWidget>
#include "datatex.h"
#include "sqlfunctions.h"
#include "newfiletype.h"


DataTables::DataTables(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataTables)
{
    ui->setupUi(this);
    currentbase = DataTex::CurrentTexFilesDataBase;
    currentbase_Notes = DataTex::CurrentNotesFolderDataBase;
    ui->FieldTable->setColumnCount(2);
    ui->FieldTable->setHorizontalHeaderLabels({tr("Field name"),tr("Primary key")});
    ui->FieldTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->AddFieldButton->setEnabled(false);
    ui->RemFieldButton->setEnabled(false);
    ui->RemoveChapterButton->setEnabled(false);
    ui->RemoveSectionButton->setEnabled(false);
    ui->RemFieldButton->setEnabled(false);
    ui->EditFieldButton->setEnabled(false);
    ui->EditChapterButton->setEnabled(false);
    ui->EditSectionButton->setEnabled(false);
    ui->RemoveExerciseTypeButton->setEnabled(false);
    ui->EditExerciseTypeButton->setEnabled(false);
    ui->RemDocumentTypeButton->setEnabled(false);
    ui->EditDocumentTypeButton->setEnabled(false);
    ui->RemTag->setEnabled(false);
    ui->EditTag->setEnabled(false);
    LoadLists();
    LoadFields();
    ui->DocumentTypeTable->setColumnCount(1);
    ui->DocumentTypeTable->setHorizontalHeaderLabels({tr("Document type")});
    ui->DocumentTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    LoadDocumentTypes();
    ui->TagTable->setColumnCount(1);
    ui->TagTable->setHorizontalHeaderLabels({tr("Tag name")});
    ui->TagTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->FieldTable->sortItems(0);
    ui->DocumentTypeTable->sortItems(0);
    ui->TagTable->sortItems(0);
    ui->AddChapterButton->setEnabled(false);
    ui->AddSectionButton->setEnabled(false);
    ui->AddExerciseTypeButton->setEnabled(false);
    ui->FileTypeTable->setColumnCount(3);
    ui->FileTypeTable->setHorizontalHeaderLabels({tr("Name"),tr("Id"),tr("Folder")});
    ui->FileTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    LoadFileTypes();
    LoadTags();
    foreach(QSqlDatabase database,DataTex::GlobalFilesDatabaseList){
        ui->FilesDBCombo->addItem(DataTex::GlobalFilesDatabaseListNames[QFileInfo(database.databaseName()).baseName()],QVariant(database.databaseName()));
    }
    ui->FilesDBCombo->setCurrentText(DataTex::GlobalFilesDatabaseListNames[QFileInfo(DataTex::CurrentDataBasePath).baseName()]);
    foreach(QSqlDatabase database,DataTex::GlobalDocsDatabaseList){
        ui->DocsDBCombo->addItem(DataTex::GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()],QVariant(database.databaseName()));
    }
    ui->DocsDBCombo->setCurrentText(DataTex::GlobalDocsDatabaseListNames[QFileInfo(DataTex::CurrentNotesFolderPath).baseName()]);
    connect(ui->FilesDBCombo,&QComboBox::textActivated,this,[=](){
        currentbase = DataTex::GlobalFilesDatabaseList[QFileInfo(ui->FilesDBCombo->currentData().toString()).baseName()];
        LoadLists();
        LoadFields();
        LoadFileTypes();
        LoadTags();
    });
    connect(ui->DocsDBCombo,&QComboBox::textActivated,this,[=](){
        currentbase_Notes = DataTex::GlobalDocsDatabaseList[QFileInfo(ui->DocsDBCombo->currentData().toString()).baseName()];
        LoadDocumentTypes();
    });

    connect(ui->FieldPKLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->FieldNameLine->text().isEmpty()
            && !ui->FieldNameLine->text().isNull() && !FieldNames.contains(ui->FieldNameLine->text())){
            ui->AddFieldButton->setEnabled(true);
            if(!FieldIds.contains(text)){
                ui->AddFieldButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddFieldButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Primary key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddFieldButton->setEnabled(false);
        }
    });
    connect(ui->FieldNameLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->FieldPKLine->text().isEmpty()
            && !ui->FieldPKLine->text().isNull() && !FieldIds.contains(ui->FieldPKLine->text())){
            ui->AddFieldButton->setEnabled(true);
            if(!FieldNames.contains(text)){
                ui->AddFieldButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddFieldButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Field name key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddFieldButton->setEnabled(false);
        }
    });
    connect(ui->AddFieldButton,&QPushButton::clicked,this,[&](){
        QStringList list = {ui->FieldNameLine->text(),ui->FieldPKLine->text()};
        AddField(list);
    });

    connect(ui->ChapterPKLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->ChapterNameLine->text().isEmpty()
            && !ui->ChapterNameLine->text().isNull() && !ChapterNames.contains(ui->ChapterNameLine->text())){
            ui->AddChapterButton->setEnabled(true);
            if(!ChapterIds.contains(text)){
                ui->AddChapterButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddChapterButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Primary key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddChapterButton->setEnabled(false);
        }
    });
    connect(ui->ChapterNameLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->ChapterPKLine->text().isEmpty()
            && !ui->ChapterPKLine->text().isNull() && !ChapterIds.contains(ui->ChapterPKLine->text())){
            ui->AddChapterButton->setEnabled(true);
            if(!ChapterNames.contains(text)){
                ui->AddChapterButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddChapterButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Chapter name key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddChapterButton->setEnabled(false);
        }
    });
    connect(ui->AddChapterButton,&QPushButton::clicked,this,[&](){
        QStringList list = {ui->ChapterNameLine->text(),ui->ChapterPKLine->text()};
        AddChapter(list);
    });

    connect(ui->SectionPKLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->SectionNameLine->text().isEmpty()
            && !ui->SectionNameLine->text().isNull() && !SectionNames.contains(ui->SectionNameLine->text())){
            ui->AddSectionButton->setEnabled(true);
            if(!SectionIds.contains(text)){
                ui->AddSectionButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddSectionButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Primary key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddSectionButton->setEnabled(false);
        }
    });
    connect(ui->SectionNameLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->SectionPKLine->text().isEmpty()
            && !ui->SectionPKLine->text().isNull() && !SectionIds.contains(ui->SectionPKLine->text())){
            ui->AddSectionButton->setEnabled(true);
            if(!SectionNames.contains(text)){
                ui->AddSectionButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddSectionButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Section name key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddSectionButton->setEnabled(false);
        }
    });
    connect(ui->AddSectionButton,&QPushButton::clicked,this,[&](){
        QStringList list = {ui->SectionNameLine->text(),ui->SectionPKLine->text()};
        AddSection(list);
    });

    connect(ui->ExerciseTypePKLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->ExerciseTypeNameLine->text().isEmpty()
            && !ui->ExerciseTypeNameLine->text().isNull() && !ExTypeNames.contains(ui->ExerciseTypeNameLine->text())){
            ui->AddExerciseTypeButton->setEnabled(true);
            if(!ExTypeIds.contains(text)){
                ui->AddExerciseTypeButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddExerciseTypeButton->setEnabled(false);
                ui->warrningLabel->setText(tr("Primary key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddExerciseTypeButton->setEnabled(false);
        }
    });
    connect(ui->ExerciseTypeNameLine,&QLineEdit::textChanged,this,[&](QString text){
        if(!text.isEmpty() && !text.isNull() && !ui->ExerciseTypePKLine->text().isEmpty()
            && !ui->ExerciseTypePKLine->text().isNull() && !ExTypeIds.contains(ui->ExerciseTypePKLine->text())){
            ui->AddExerciseTypeButton->setEnabled(true);
            if(!ExTypeNames.contains(text)){
                ui->AddExerciseTypeButton->setEnabled(true);
                ui->warrningLabel->clear();
            }
            else{
                ui->AddExerciseTypeButton->setEnabled(false);
                ui->warrningLabel->setText(tr("ExType name key %1 already exists.").arg(text));
            }
        }
        else{
            ui->AddExerciseTypeButton->setEnabled(false);
        }
    });
    connect(ui->AddExerciseTypeButton,&QPushButton::clicked,this,[&](){
        QStringList list = {ui->ExerciseTypeNameLine->text(),ui->ExerciseTypePKLine->text()};
        AddExerciseType(list);
    });
}

DataTables::~DataTables()
{
    delete ui;
}

void DataTables::LoadLists()
{
    FieldIds.clear();
    FieldNames.clear();
    ChapterIds.clear();
    ChapterNames.clear();
    SectionIds.clear();
    SectionNames.clear();
    ExTypeIds.clear();
    ExTypeNames.clear();
    FileTypeIds.clear();
    FileTypeNames.clear();
    FileTypeFolders.clear();
    CustomTags.clear();
    FieldIds.append(SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Fields",DataTex::CurrentTexFilesDataBase));
    FieldNames.append(SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Fields",DataTex::CurrentTexFilesDataBase));
    ChapterIds.append(SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Chapters",DataTex::CurrentTexFilesDataBase));
    ChapterNames.append(SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Chapters",DataTex::CurrentTexFilesDataBase));
    SectionIds.append(SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Sections",DataTex::CurrentTexFilesDataBase));
    SectionNames.append(SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Sections",DataTex::CurrentTexFilesDataBase));
    ExTypeIds.append(SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Exercise_Types WHERE id <> '-'",DataTex::CurrentTexFilesDataBase));
    ExTypeNames.append(SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Exercise_Types WHERE id <> '-'",DataTex::CurrentTexFilesDataBase));
    FileTypeIds.append(SqlFunctions::Get_StringList_From_Query("SELECT Id FROM FileTypes",DataTex::CurrentTexFilesDataBase));
    FileTypeNames.append(SqlFunctions::Get_StringList_From_Query("SELECT FileType FROM FileTypes",DataTex::CurrentTexFilesDataBase));
    FileTypeFolders.append(SqlFunctions::Get_StringList_From_Query("SELECT FolderName FROM FileTypes",DataTex::CurrentTexFilesDataBase));
}

void DataTables::LoadFields()
{
    int item=-1;
    ui->FieldTable->setRowCount(0);
    ui->ComboFields_ChapterTab->clear();
    ui->ComboFields_SectionTab->clear();
    ui->ComboFields_ExerciseTypeTab->clear();
    ui->ComboFields_ExerciseTypeTab->clear();
    ui->ComboFields_ChapterTab->clear();
    ui->ComboFields_SectionTab->clear();
    QSqlQuery fields(currentbase);
    fields.exec(SqlFunctions::Fields_Query);
    while(fields.next()){
        item++;
        ui->FieldTable->insertRow(item);
        ui->FieldTable->setItem(item,0 , new QTableWidgetItem(fields.value(0).toString()));
        ui->FieldTable->setItem(item,1 , new QTableWidgetItem(fields.value(1).toString()));
        ui->ComboFields_ChapterTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
        ui->ComboFields_SectionTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
        ui->ComboFields_ExerciseTypeTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
        ui->ComboFields_ExerciseTypeTab->setCurrentIndex(-1);
        ui->ComboFields_ChapterTab->setCurrentIndex(-1);
        ui->ComboFields_SectionTab->setCurrentIndex(-1);
    }
}

void DataTables::LoadDocumentTypes()
{
    int row=-1;
    ui->DocumentTypeTable->setRowCount(0);
    QSqlQuery DocumentType(currentbase_Notes);
    DocumentType.exec(SqlFunctions::GetDocumentTypes);
    DocumentTypes.clear();
    while(DocumentType.next()){
        row++;
        ui->DocumentTypeTable->insertRow(row);
        ui->DocumentTypeTable->setItem(row,0 , new QTableWidgetItem(DocumentType.value(0).toString()));
        DocumentTypes.append(DocumentType.value(0).toString());
    }
}

void DataTables::LoadFileTypes()
{
    QSqlQuery FileTypes(currentbase);
    FileTypes.exec("SELECT \"FileType\",\"Id\",\"FolderName\" FROM \"FileTypes\"");
    int i=-1;
    ui->FileTypeTable->setRowCount(0);
    while(FileTypes.next()){
        i++;
        ui->FileTypeTable->insertRow(i);
        ui->FileTypeTable->setItem(i,0 , new QTableWidgetItem(FileTypes.value(0).toString()));
        ui->FileTypeTable->setItem(i,1 , new QTableWidgetItem(FileTypes.value(1).toString()));
        ui->FileTypeTable->setItem(i,2 , new QTableWidgetItem(FileTypes.value(2).toString()));
    }
}

void DataTables::LoadTags()
{
    QSqlQuery Tags(currentbase);
    Tags.exec("SELECT Tag FROM CustomTags");
    int k=-1;
    ui->TagTable->setRowCount(0);
    while(Tags.next()){
        k++;
        ui->TagTable->insertRow(k);
        ui->TagTable->setItem(k,0 , new QTableWidgetItem(Tags.value(0).toString()));
    }
}

void DataTables::AddField(QStringList Line)
{
    QSqlQuery AddField(currentbase);
    AddField.prepare(QString("INSERT INTO Fields (Id,Name) VALUES(\"%1\",\"%2\")")
                  .arg(Line[1],Line[0]));
    int i = ui->FieldTable->rowCount();
    if(AddField.exec()){
        ui->FieldTable->insertRow(i);
        ui->FieldTable->setItem(i,0 , new QTableWidgetItem(Line[0]));
        ui->FieldTable->setItem(i,1 , new QTableWidgetItem(Line[1]));
        ui->ComboFields_ChapterTab->addItem(Line[0],QVariant(Line[1]));
        ui->ComboFields_ChapterTab->setCurrentIndex(-1);
        ui->ComboFields_SectionTab->addItem(Line[0],QVariant(Line[1]));
        ui->ComboFields_SectionTab->setCurrentIndex(-1);
        ui->ComboFields_ExerciseTypeTab->addItem(Line[0],QVariant(Line[1]));
        ui->ComboFields_ExerciseTypeTab->setCurrentIndex(-1);
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddField.lastError().text(),QMessageBox::Ok);
    }
    ui->FieldNameLine->clear();
    ui->FieldPKLine->clear();

}

void DataTables::on_RemFieldButton_clicked()
{
    int row = ui->FieldTable->currentRow();
    QString item = ui->FieldTable->item(row, 1)->text();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Delete field"),tr("The field %1 will be deleted!\n Possible data loss.\nDo you wish to proceed?")
                                    .arg(item),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        QSqlQuery RemoveField(currentbase);
        RemoveField.exec("PRAGMA foreign_keys = ON");
        RemoveField.exec(QString("DELETE FROM Fields WHERE Id = \"%1\"").arg(item));
        ui->FieldTable->removeRow(row);
        ui->ComboFields_ChapterTab->removeItem(row);
        ui->ComboFields_SectionTab->removeItem(row);
        ui->ComboFields_ExerciseTypeTab->removeItem(row);
    }
}

void DataTables::on_FieldTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemFieldButton->setEnabled(true);
    ui->EditFieldButton->setEnabled(true);
}

void DataTables::on_ComboFields_ChapterTab_currentIndexChanged(int index)
{
    QString chapter = ui->ComboFields_ChapterTab->currentText();
    ui->ChapterTable->setRowCount(0);
    ui->ChapterTable->setColumnCount(2);
    QStringList horzHeaders;
    horzHeaders << tr("Chapter name") << tr("Primary key");
    ui->ChapterTable->setHorizontalHeaderLabels(horzHeaders);
    ui->ChapterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QSqlQuery ChaptersQuery(currentbase);
    int i=-1;
    ChaptersQuery.exec(SqlFunctions::Chapters_Query.arg(chapter));
    while(ChaptersQuery.next()){
        i++;
         ui->ChapterTable->insertRow(i);
         ui->ChapterTable->setItem(i,0 , new QTableWidgetItem(ChaptersQuery.value(0).toString()));
         ui->ChapterTable->setItem(i,1 , new QTableWidgetItem(ChaptersQuery.value(1).toString()));
    }
    ui->ChapterTable->sortItems(0);
    if(index>-1){ui->AddChapterButton->setEnabled(true);}
}

void DataTables::on_ComboFields_SectionTab_currentIndexChanged(int index)
{
    ui->SectionTable->setRowCount(0);
    ui->ComboChapters_SectionTab->clear();
    QList<QStringList> data =
            SqlFunctions::ComboList_Single(SqlFunctions::Chapters_Query,currentbase,
                                           ui->ComboFields_SectionTab->currentText());
    for (int item=0;item<data[0].count() ;item++ ) {
        ui->ComboChapters_SectionTab->addItem(data[0].at(item),QVariant(data[1].at(item)));
    }
    ui->ComboChapters_SectionTab->setCurrentIndex(-1);
    ui->AddSectionButton->setEnabled(false);
}

void DataTables::on_ComboChapters_SectionTab_currentIndexChanged(int index)
{
    ui->SectionTable->setRowCount(0);
    ui->SectionTable->setColumnCount(2);
    QString section = ui->ComboChapters_SectionTab->currentText();
    QStringList horzHeaders;
    horzHeaders << tr("Section name") << tr("Primary key");
    ui->SectionTable->setHorizontalHeaderLabels(horzHeaders);
    ui->SectionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int i=-1;
     QSqlQuery SectionsQuery(currentbase);
     SectionsQuery.exec(SqlFunctions::Sections_Chapters_Query.arg(section));
     while (SectionsQuery.next()){
         i++;
        ui->SectionTable->insertRow(i);
        ui->SectionTable->setItem(i,0 , new QTableWidgetItem(SectionsQuery.value(0).toString()));
        ui->SectionTable->setItem(i,1 , new QTableWidgetItem(SectionsQuery.value(1).toString()));
    }
     ui->SectionTable->sortItems(0);
     if(index>-1){ui->AddSectionButton->setEnabled(true);}
}

void DataTables::AddChapter(QStringList Line)
{
    QString field = ui->ComboFields_ChapterTab->currentData().toString();
    QSqlQuery AddChapter(currentbase);
    AddChapter.prepare(QString("INSERT INTO Chapters (Id,Name,Field) VALUES(\"%1\",\"%2\",\"%3\")")
                  .arg(Line[1],Line[0],field));
    int i = ui->ChapterTable->rowCount();
    if(AddChapter.exec()){
        ui->ChapterTable->insertRow(i);
        ui->ChapterTable->setItem(i,0 , new QTableWidgetItem(Line[0]));
        ui->ChapterTable->setItem(i,1 , new QTableWidgetItem(Line[1]));
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddChapter.lastError().text(),QMessageBox::Ok);
    }
    ui->ChapterNameLine->clear();
    ui->ChapterPKLine->clear();
    ui->ComboFields_SectionTab->setCurrentIndex(-1);
}

void DataTables::on_RemoveChapterButton_clicked()
{
    int row = ui->ChapterTable->currentRow();
    QString ChapterId = ui->ChapterTable->item(row, 1)->text();
    QString FieldId = ui->ComboFields_ChapterTab->currentData().toString();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Delete chapter"),tr("The chapter %1 will be deleted!\nPossible data loss.\nDo you wish to proceed?")
                                      .arg(ChapterId),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        QSqlQuery RemoveChapter(currentbase);
        RemoveChapter.exec("PRAGMA foreign_keys = ON");
        RemoveChapter.exec(QString("DELETE FROM Chapters WHERE Id = \"%1\" AND Field = \"%2\"")
                         .arg(ChapterId,FieldId));
        ui->ChapterTable->removeRow(row);
    }
}

void DataTables::on_ChapterTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemoveChapterButton->setEnabled(true);
    ui->EditChapterButton->setEnabled(true);
}


void DataTables::AddSection(QStringList Line)
{
    QString FieldId = ui->ComboFields_SectionTab->currentData().toString();
    QString ChapterId = ui->ComboChapters_SectionTab->currentData().toString();
    QSqlQuery AddSection(currentbase);
    AddSection.prepare(QString("INSERT INTO Sections (Id,Name,Field,Chapter) VALUES(\"%1\",\"%2\",\"%3\",\"%4\")")
                    .arg(Line[1],Line[0],FieldId,ChapterId));
        int i = ui->SectionTable->rowCount();
    if(AddSection.exec()){
    ui->SectionTable->insertRow(i);
    ui->SectionTable->setItem(i,0 , new QTableWidgetItem(Line[0]));
    ui->SectionTable->setItem(i,1 , new QTableWidgetItem(Line[1]));
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddSection.lastError().text(),QMessageBox::Ok);
    }
    ui->SectionNameLine->clear();
    ui->SectionPKLine->clear();
}

//void DataTables::on_AddSectionButton_clicked()
//{
//    newLine = new AddDatabaseField(this);
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(AddSection(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
//}

void DataTables::on_SectionTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemoveSectionButton->setEnabled(true);
    ui->EditSectionButton->setEnabled(true);
}

void DataTables::on_RemoveSectionButton_clicked()
{
    int row = ui->SectionTable->currentRow();
    QString code = ui->SectionTable->item(row, 1)->text();
    QString field = ui->ComboFields_SectionTab->currentData().toString();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 tr("Delete section"),tr("The section %1 will be deleted!\nPossible data loss.\nDo you wish to proceed?")
                                     .arg(code),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        QSqlQuery RemoveSection(currentbase);
        RemoveSection.exec("PRAGMA foreign_keys = ON");
        RemoveSection.exec(QString("DELETE FROM Sections WHERE Id = \"%1\" AND Field = \"%2\"")
                           .arg(code,field));
        ui->SectionTable->removeRow(row);
    }
}

void DataTables::on_buttonBox_accepted()
{
    accept();
}

void DataTables::on_buttonBox_rejected()
{
    reject();
}

void DataTables::on_EditFieldButton_clicked()
{
    DataTex::FunctionInProgress();
//    int row = ui->FieldTable->currentRow();
//    QStringList line;
//    line.append(ui->FieldTable->item(row, 0)->text());
//    line.append(ui->FieldTable->item(row, 1)->text());
//    newLine = new AddDatabaseField(this);
//    newLine->EditLine_DataTex(line);
//    connect(this,SIGNAL(addline(QStringList)),newLine,SLOT(EditLine_DataTex(QStringList)));
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(EditField(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
}

void DataTables::EditField(QStringList Line)
{
    int row = ui->FieldTable->currentRow();
    QString FieldName = ui->FieldTable->item(row, 0)->text();
    QString FieldId = ui->FieldTable->item(row, 1)->text();
    QSqlQuery EditField(currentbase);
    EditField.exec("PRAGMA foreign_keys = ON");
    EditField.prepare(QString("UPDATE Fields SET Id = \"%1\",Name = \"%2\" WHERE Id = \"%3\" AND Name = \"%4\"")
                   .arg(Line[1],Line[0],FieldId,FieldName));
    if(EditField.exec()){
        ui->FieldTable->item(row,0)->setText(QString(Line[0]));
        ui->FieldTable->item(row,1)->setText(QString(Line[1]));
        ui->ComboFields_ChapterTab->clear();
        ui->ComboFields_SectionTab->clear();

        UpdateDatabaseMetadata("Field",Line[1],FieldId+"-",Line[1]+"-",
                FieldId+"-",Line[1]+"-",
                "/"+FieldName+"/","/"+Line[0]+"/");

        QSqlQuery fields(currentbase);
        fields.exec(SqlFunctions::Fields_Query);
        while(fields.next()){
            ui->ComboFields_ChapterTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
            ui->ComboFields_SectionTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
            ui->ComboFields_ExerciseTypeTab->addItem(fields.value(0).toString(), QVariant(fields.value(1).toString()));
            ui->ComboFields_ChapterTab->setCurrentIndex(-1);
            ui->ComboFields_SectionTab->setCurrentIndex(-1);
            ui->ComboFields_ExerciseTypeTab->setCurrentIndex(-1);
        }
        emit addline(Line);
        }
    else{
        QMessageBox::warning(this,tr("Error"),EditField.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::UpdateDatabaseMetadata(QString Id, QString DBField, QString oldId1, QString newId1, QString oldPath1, QString newPath1, QString oldPath2, QString newPath2)
{
    QStringList Paths;
    QStringList Ids;
    QSqlQuery EditFileMeta(currentbase);
    EditFileMeta.exec(QString("SELECT Id,Path FROM Database_Files WHERE \"%1\" = \"%2\"").arg(DBField,Id));
    while(EditFileMeta.next()){
        Ids.append(EditFileMeta.value(0).toString());
        Paths.append(EditFileMeta.value(1).toString());
    }
    for(int i=0;i<Ids.count();i++){
        QString id = Ids.at(i);
        QString oldId = id;
        QString path = Paths.at(i);
        QString oldpath = path;
        id.replace(oldId1,newId1);
        path.replace(oldPath1,newPath1);
        path.replace(oldPath2,newPath2);
        QSqlQuery Replace(currentbase);
        Replace.exec(QString("UPDATE Database_Files SET Id = \"%1\" WHERE Id = \"%2\"")
                       .arg(id,oldId));
        Replace.exec(QString("UPDATE Database_Files SET Path = \"%1\" WHERE Id = \"%2\"")
                       .arg(path,id));
        QDir dir(QFileInfo(path).absolutePath()); if (!dir.exists()){dir.mkpath(".");}
        QFile file(oldpath);
        file.rename(oldpath,path);
        QFile newfile(path);
        newfile.open(QIODevice::ReadWrite);
        QTextStream content(&newfile);
        QString text = content.readAll();
        text.replace("%# Database File : "+oldId,"%# Database File : "+id);
        text.replace("%# End of file "+oldId,"%# End of file "+id);
        newfile.resize(0);
        content << text;
        newfile.close();
    }
}

void DataTables::on_EditChapterButton_clicked()
{
    DataTex::FunctionInProgress();
//    int row = ui->ChapterTable->currentRow();
//    QStringList line;
//    line.append(ui->ChapterTable->item(row, 0)->text());
//    line.append(ui->ChapterTable->item(row, 1)->text());
//    newLine = new AddDatabaseField(this);
//    newLine->EditLine_DataTex(line);
//    connect(this,SIGNAL(addline(QStringList)),newLine,SLOT(EditLine_DataTex(QStringList)));
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(EditChapter(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
}

void DataTables::EditChapter(QStringList Line)
{
    int row = ui->ChapterTable->currentRow();
    QString ChapterId = ui->ChapterTable->item(row, 1)->text();
    QString ChapterName = ui->ChapterTable->item(row, 0)->text();
    QSqlQuery EditChapter(currentbase);
    EditChapter.exec("PRAGMA foreign_keys = ON");
    EditChapter.prepare(QString("UPDATE \"Chapters\" SET \"Id\" = \"%1\",\"Name\" = \"%2\" WHERE \"Id\" = \"%3\"")
                     .arg(Line[1],Line[0],ChapterId));
    if(EditChapter.exec()){
        ui->ChapterTable->item(row,0)->setText(QString(Line[0]));
        ui->ChapterTable->item(row,1)->setText(QString(Line[1]));
        ui->ComboChapters_SectionTab->clear();
        QSqlQuery ChaptersQuery(currentbase);
        ChaptersQuery.exec(SqlFunctions::Chapters_Query.arg(ChapterId));
        while(ChaptersQuery.next()){
            ui->ComboChapters_SectionTab->addItem(ChaptersQuery.value(0).toString(), QVariant(ChaptersQuery.value(1).toString()));
            ui->ComboChapters_ExerciseTypeTab->addItem(ChaptersQuery.value(0).toString(), QVariant(ChaptersQuery.value(1).toString()));
        }
        ui->ComboChapters_SectionTab->setCurrentIndex(-1);
        ui->ComboChapters_ExerciseTypeTab ->setCurrentIndex(-1);

        UpdateDatabaseMetadata("Chapter",Line[1],"-"+ChapterId+"-","-"+Line[1]+"-",
                "-"+ChapterId+"-","-"+Line[1]+"-",
                "","");

        emit addline(Line);
    }
    else{
        QMessageBox::warning(this,tr("Error"),EditChapter.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_EditSectionButton_clicked()
{
    DataTex::FunctionInProgress();
//    int row = ui->SectionTable->currentRow();
//    QStringList line;
//    line.append(ui->SectionTable->item(row, 0)->text());
//    line.append(ui->SectionTable->item(row, 1)->text());
//    newLine = new AddDatabaseField(this);
//    newLine->EditLine_DataTex(line);
//    connect(this,SIGNAL(addline(QStringList)),newLine,SLOT(EditLine_DataTex(QStringList)));
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(EditSection(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
}

void DataTables::EditSection(QStringList Line)
{
    int row = ui->SectionTable->currentRow();
    QString SectionId = ui->SectionTable->item(row, 1)->text();
    QString SectionName = ui->SectionTable->item(row, 0)->text();
    QSqlQuery EditSection(currentbase);
    EditSection.exec("PRAGMA foreign_keys = ON");
    EditSection.prepare(QString("UPDATE \"Sections\" SET \"Id\" = \"%1\",\"Name\" = \"%2\" WHERE \"Id\" = \"%3\"")
                     .arg(Line[1],Line[0],SectionId));
    if(EditSection.exec()){
        ui->SectionTable->item(row,0)->setText(QString(Line[0]));
        ui->SectionTable->item(row,1)->setText(QString(Line[1]));
        QSqlQuery SectionsQuery(currentbase);
        SectionsQuery.exec(SqlFunctions::Sections_Chapters_Query.arg(SectionId));
        while(SectionsQuery.next()){
            ui->ComboSections_ExerciseTypeTab->addItem(SectionsQuery.value(0).toString(), QVariant(SectionsQuery.value(1).toString()));
        }
        ui->ComboSections_ExerciseTypeTab ->setCurrentIndex(-1);

        UpdateDatabaseMetadata("Section",Line[1],SectionId,Line[1],
                SectionId,Line[1],
                SectionName,Line[0]);

        emit addline(Line);
    }
    else{
        QMessageBox::warning(this,tr("Error"),EditSection.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_AddDocumentTypeButton_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(AddDocumentType(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void DataTables::AddDocumentType(QString Line)
{
    QSqlQuery AddDocumentType(currentbase_Notes);
    AddDocumentType.exec("PRAGMA foreign_keys = ON");
    AddDocumentType.prepare(QString("INSERT INTO \"Document_Types\" (\"Name\") VALUES(\"%1\")")
                    .arg(Line));
    int i = ui->DocumentTypeTable->rowCount();
    if(AddDocumentType.exec()){
        ui->DocumentTypeTable->insertRow(i);
        ui->DocumentTypeTable->setItem(i,0 , new QTableWidgetItem(Line));
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddDocumentType.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_RemDocumentTypeButton_clicked()
{
    int row = ui->DocumentTypeTable->currentRow();
    QString DocumentType = ui->DocumentTypeTable->item(row, 0)->text();
    QSqlQuery RemoveDocumentType(currentbase_Notes);
    QMessageBox::StandardButton resBtn = QMessageBox::question(this,
                 tr("Delete document type"),tr("The document type %1 will be deleted!\nPossible data loss.\nDo you wish to prceed?")
                                    .arg(DocumentType),QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        RemoveDocumentType.exec("PRAGMA foreign_keys = ON");
        RemoveDocumentType.exec(QString("DELETE FROM \"Document_Types\" WHERE \"Name\" = \"%1\"")
                        .arg(DocumentType));
        ui->DocumentTypeTable->removeRow(row);
    }
}

void DataTables::on_EditDocumentTypeButton_clicked()
{
    DataTex::FunctionInProgress();
//    QString eidos = ui->DocumentTypeTable->currentItem()->text();
//    newFolder = new addfolder(this);
//    newFolder->EditFolder(eidos);
//    connect(this,SIGNAL(addfolder_signal(QString)),newFolder,SLOT(EditFolder(QString)));
//    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(EditDocumentType(QString)));
//    newFolder->show();
//    newFolder->activateWindow();
}

void DataTables::EditDocumentType(QString Line)
{
    int row = ui->DocumentTypeTable->currentRow();
    QString DocumentType = ui->DocumentTypeTable->item(row, 0)->text();
    ui->DocumentTypeTable->item(row,0)->setText(QString(Line));
    QSqlQuery EditDocumentType(currentbase_Notes);
    if(EditDocumentType.exec()){
        EditDocumentType.exec("PRAGMA foreign_keys = ON");
        EditDocumentType.prepare(QString("UPDATE \"Document_Types\" SET \"Name\" = \"%1\" WHERE \"Name\" = \"%2\"")
                         .arg(Line,DocumentType));
        emit addfolder_signal(Line);
    }
    else{
        QMessageBox::warning(this,tr("Error"),EditDocumentType.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_DocumentTypeTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemDocumentTypeButton->setEnabled(true);
    ui->EditDocumentTypeButton->setEnabled(true);
}

void DataTables::on_AddTag_clicked()//23/9
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(AddTag(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void DataTables::AddTag(QString Line)
{
    QSqlQuery AddTag(currentbase);
    AddTag.exec("PRAGMA foreign_keys = ON");
    AddTag.prepare(QString("INSERT OR IGNORE INTO CustomTags (Tag) VALUES(:id)"));
    AddTag.bindValue(":id",Line);
    int i = ui->TagTable->rowCount();
    if(AddTag.exec()){
        ui->TagTable->insertRow(i);
        ui->TagTable->setItem(i,0 , new QTableWidgetItem(Line));
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddTag.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_RemTag_clicked()
{
    int row = ui->TagTable->currentRow();
    QString Tag = ui->TagTable->item(row, 0)->text();
    QSqlQuery RemoveDocumentType(currentbase);
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete subject type",tr("The tag %1 will be deleted!\nPossible data loss.\nDo you wish to proceed?")
                                        .arg(Tag)
                                        ,QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        RemoveDocumentType.exec("PRAGMA foreign_keys = ON");
        RemoveDocumentType.exec(QString("DELETE FROM CustomTags WHERE Tag = \"%1\"")
                        .arg(Tag));
        ui->TagTable->removeRow(row);
    }
}

void DataTables::on_EditTag_clicked()
{
    newFolder = new addfolder(this);
    newFolder->EditFolder(ui->TagTable->item(ui->TagTable->currentRow(), 0)->text());
//    connect(this,SIGNAL(addfolder_signal(QString)),newFolder,SLOT(EditFolder(QString)));
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(EditTag(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void DataTables::EditTag(QString Line)
{
    int row = ui->TagTable->currentRow();
    QString Tag = ui->TagTable->item(row, 0)->text();
    ui->TagTable->item(row,0)->setText(Line);
    QSqlQuery EditTag(currentbase);
    EditTag.exec("PRAGMA foreign_keys = ON");
    EditTag.exec(QString("UPDATE CustomTags SET Tag = \"%1\" WHERE Tag = \"%2\"").arg(Line,Tag));
    emit addfolder_signal(Line);
}

void DataTables::on_TagTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemTag->setEnabled(true);
    ui->EditTag->setEnabled(true);
}

void DataTables::on_ComboFields_ExerciseTypeTab_currentIndexChanged(int index)
{
    ui->ExerciseTypeTable->setRowCount(0);
    ui->ComboChapters_ExerciseTypeTab->clear();
    QList<QStringList> data =
            SqlFunctions::ComboList_Single(SqlFunctions::Chapters_Query,currentbase,
                                           ui->ComboFields_ExerciseTypeTab->currentText());
    for (int item=0;item<data[0].count() ;item++ ) {
        ui->ComboChapters_ExerciseTypeTab->addItem(data[0].at(item),QVariant(data[1].at(item)));
    }
    ui->ComboChapters_ExerciseTypeTab->setCurrentIndex(-1);
    ui->AddExerciseTypeButton->setEnabled(false);
}

void DataTables::on_ComboChapters_ExerciseTypeTab_currentIndexChanged(int index)
{
    ui->ExerciseTypeTable->setRowCount(0);
    ui->ComboSections_ExerciseTypeTab->clear();
    QList<QStringList> data =
            SqlFunctions::ComboList_Single(SqlFunctions::Sections_Chapters_Query,currentbase,
                                           ui->ComboChapters_ExerciseTypeTab->currentText());
    for (int item=0;item<data[0].count() ;item++ ) {
        ui->ComboSections_ExerciseTypeTab->addItem(data[0].at(item),QVariant(data[1].at(item)));
    }
    ui->ComboSections_ExerciseTypeTab->setCurrentIndex(-1);
    ui->AddExerciseTypeButton->setEnabled(false);
}

void DataTables::on_ComboSections_ExerciseTypeTab_currentIndexChanged(int index)
{
    ui->ExerciseTypeTable->setRowCount(0);
    ui->ExerciseTypeTable->setColumnCount(2);
    QString section = ui->ComboSections_ExerciseTypeTab->currentData().toString();
    QStringList horzHeaders;
    horzHeaders << tr("Exercise type") << tr("Primary key");
    ui->ExerciseTypeTable->setHorizontalHeaderLabels(horzHeaders);
    ui->ExerciseTypeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int i=-1;
    QSqlQuery ExerciseTypes(currentbase);
    ExerciseTypes.exec(SqlFunctions::Exercise_Types_Query.arg(section));
    while(ExerciseTypes.next()){
        i++;
        ui->ExerciseTypeTable->insertRow(i);
        ui->ExerciseTypeTable->setItem(i,0 , new QTableWidgetItem(ExerciseTypes.value(0).toString()));
        ui->ExerciseTypeTable->setItem(i,1 , new QTableWidgetItem(ExerciseTypes.value(1).toString()));
    }
    ui->ExerciseTypeTable->sortItems(0);
    if(index>-1){ui->AddExerciseTypeButton->setEnabled(true);}
}

void DataTables::AddExerciseType(QStringList Line)
{
    QString section = ui->ComboSections_ExerciseTypeTab->currentData().toString();
    QSqlQuery AddSection1(currentbase);
    QSqlQuery AddSection2(currentbase);
    AddSection1.prepare(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")")
                    .arg(Line[1],Line[0]));
    AddSection2.prepare(QString("INSERT OR IGNORE INTO \"Sections_Exercises\" (\"Exercise_Id\",\"Section_Id\") VALUES(\"%1\",\"%2\")")
                    .arg(Line[1],section));
    int i = ui->ExerciseTypeTable->rowCount();
    if(AddSection1.exec() && AddSection2.exec()){
        ui->ExerciseTypeTable->insertRow(i);
        ui->ExerciseTypeTable->setItem(i,0 , new QTableWidgetItem(Line[0]));
        ui->ExerciseTypeTable->setItem(i,1 , new QTableWidgetItem(Line[1]));
    }
    else{
        QMessageBox::warning(this,tr("Error"),AddSection2.lastError().text(),QMessageBox::Ok);
    }
    ui->ExerciseTypeNameLine->clear();
    ui->ExerciseTypePKLine->clear();
}

void DataTables::on_RemoveExerciseTypeButton_clicked()
{
    int row = ui->ExerciseTypeTable->currentRow();
    QString ExerciseType = ui->ExerciseTypeTable->item(row, 1)->text();
    QString SectionId = ui->ComboSections_ExerciseTypeTab->currentData().toString();
    QSqlQuery RemoveExerciseType(currentbase);
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete exercise type",tr("The exercise type %1 will be deleted!\nDo you wish to proceed?").arg(ExerciseType),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
    RemoveExerciseType.exec(QString("DELETE FROM \"Sections_Exercises\" WHERE \"Exercise_Id\" = \"%1\" AND \"Section_Id\" = \"%2\"")
                     .arg(ExerciseType,SectionId));
        ui->ExerciseTypeTable->removeRow(row);
    }
}

void DataTables::on_EditExerciseTypeButton_clicked()
{
    DataTex::FunctionInProgress();
//    int row = ui->ExerciseTypeTable->currentRow();
//    QStringList line;
//    line.append(ui->ExerciseTypeTable->item(row, 0)->text());
//    line.append(ui->ExerciseTypeTable->item(row, 1)->text());
//    newLine = new AddDatabaseField(this);
//    newLine->EditLine_DataTex(line);
//    connect(this,SIGNAL(addline(QStringList)),newLine,SLOT(EditLine_DataTex(QStringList)));
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(EditExerciseType(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
}

void DataTables::EditExerciseType(QStringList Line)
{
    QString section = ui->ComboSections_ExerciseTypeTab->currentData().toString();
    int row = ui->ExerciseTypeTable->currentRow();
    QString ExerciseTypeId = ui->ExerciseTypeTable->item(row, 1)->text();
    QSqlQuery EditExerciseType1(currentbase);
    QSqlQuery EditExerciseType2(currentbase);
    EditExerciseType1.exec(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")")
                     .arg(Line[1],Line[0]));
    EditExerciseType2.prepare(QString("UPDATE \"Sections_Exercises\" SET \"Exercise_Id\" = \"%1\""
                             " WHERE \"Exercise_Id\" = \"%2\" "
                             "AND \"Section_Id\" = \"%3\"")
                     .arg(Line[1],ExerciseTypeId,section));
    if(EditExerciseType2.exec()){
        ui->ExerciseTypeTable->item(row,0)->setText(QString(Line[0]));
        ui->ExerciseTypeTable->item(row,1)->setText(QString(Line[1]));
        emit addline(Line);
    }
    else{
        QMessageBox::warning(this,tr("Error"),EditExerciseType1.lastError().text()+EditExerciseType2.lastError().text(),QMessageBox::Ok);
    }
}

void DataTables::on_ExerciseTypeTable_itemClicked(QTableWidgetItem *item)
{
    ui->RemoveExerciseTypeButton->setEnabled(true);
    ui->EditExerciseTypeButton->setEnabled(true);
}

void DataTables::on_AddFileTypeButton_clicked()
{
    QStringList Ids = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM FileTypes",currentbase);
    QStringList Names = SqlFunctions::Get_StringList_From_Query("SELECT FileType FROM FileTypes",currentbase);
    QStringList Folders = SqlFunctions::Get_StringList_From_Query("SELECT FolderName FROM FileTypes",currentbase);

    NewFileType * newFile = new NewFileType(this);
    connect(newFile,&NewFileType::filedata,this,[=](QStringList data){
        QSqlQuery NewFileType(currentbase);
        NewFileType.prepare(QString("INSERT OR IGNORE INTO \"FileTypes\" (\"Id\",\"FileType\",\"FolderName\",\"Solvable\") VALUES(:id,:name,:folder,:sol)"));
        NewFileType.bindValue(":id",data[0]);
        NewFileType.bindValue(":name",data[1]);
        NewFileType.bindValue(":folder",data[2]);
        NewFileType.bindValue(":sol",data[3]);
        NewFileType.exec();
        if(data[3]=="1"){
            NewFileType.prepare(QString("INSERT OR IGNORE INTO \"FileTypes\" (\"Id\",\"FileType\",\"FolderName\",\"Solvable\") VALUES(:id,:name,:folder,-1,:fid)"));
            NewFileType.bindValue(":id",data[0]+"-Sol");
            NewFileType.bindValue(":name",data[1]+tr(" - Solution"));
            NewFileType.bindValue(":folder",data[2]+tr(" - Solutions"));
            NewFileType.bindValue(":fid",data[0]);
        }
        int i = ui->FileTypeTable->rowCount();
        if(NewFileType.exec()){
            ui->FileTypeTable->insertRow(i);
            ui->FileTypeTable->setItem(i,0 , new QTableWidgetItem(data[0]));
            ui->FileTypeTable->setItem(i,1 , new QTableWidgetItem(data[1]));
            ui->FileTypeTable->setItem(i,2 , new QTableWidgetItem(data[2]));
        }
        else{
            QMessageBox::warning(this,tr("Error"),NewFileType.lastError().text(),QMessageBox::Ok);
        }
        newFile->close();
    });
    newFile->exec();
}


void DataTables::on_RemFileTypeButton_clicked()
{
    int row = ui->FileTypeTable->currentRow();
    QString FileType = ui->FileTypeTable->item(row, 1)->text();
    QSqlQuery RemoveFileType(currentbase);
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,
                 "Delete file type",tr("The file type %1 will be deleted!\nDo you wish to proceed?").arg(FileType),
                  QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        RemoveFileType.exec(QString("DELETE FROM \"FileTypes\" WHERE \"Id\" = \"%1\"")
                         .arg(FileType));
        ui->FileTypeTable->removeRow(row);
    }
}


void DataTables::on_EditFileTypeButton_clicked()
{
    DataTex::FunctionInProgress();
//    int row = ui->ExerciseTypeTable->currentRow();
//    QStringList line;
//    line.append(ui->ExerciseTypeTable->item(row, 0)->text());
//    line.append(ui->ExerciseTypeTable->item(row, 1)->text());
//    newLine = new AddDatabaseField(this);
//    newLine->EditLine_DataTex(line);
//    connect(this,SIGNAL(addline(QStringList)),newLine,SLOT(EditLine_DataTex(QStringList)));
//    connect(newLine,SIGNAL(newline(QStringList)),this,SLOT(EditFileType(QStringList)));
//    newLine->show();
//    newLine->activateWindow();
}

void DataTables::EditFileType(QStringList Line)
{
//    QString section = ui->ComboSections_ExerciseTypeTab->currentData().toString();
//    int row = ui->ExerciseTypeTable->currentRow();
//    QString ExerciseTypeId = ui->ExerciseTypeTable->item(row, 1)->text();
//    QSqlQuery EditExerciseType1(currentbase);
//    QSqlQuery EditExerciseType2(currentbase);
//    EditExerciseType1.exec(QString("INSERT OR IGNORE INTO \"Exercise_Types\" (\"Id\",\"Name\") VALUES(\"%1\",\"%2\")")
//                     .arg(Line[1],Line[0]));
//    EditExerciseType2.prepare(QString("UPDATE \"Sections_Exercises\" SET \"Exercise_Id\" = \"%1\""
//                             " WHERE \"Exercise_Id\" = \"%2\" "
//                             "AND \"Section_Id\" = \"%3\"")
//                     .arg(Line[1],ExerciseTypeId,section));
//    if(EditExerciseType2.exec()){
//        ui->ExerciseTypeTable->item(row,0)->setText(QString(Line[0]));
//        ui->ExerciseTypeTable->item(row,1)->setText(QString(Line[1]));
//        emit addline(Line);
//    }
//    else{
//        QMessageBox::warning(this,tr("Error"),EditExerciseType1.lastError().text()+EditExerciseType2.lastError().text(),QMessageBox::Ok);
//    }
}

