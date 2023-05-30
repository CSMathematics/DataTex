#ifndef DATATABLES_H
#define DATATABLES_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QFileInfo>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include "datatex.h"
#include "sqlfunctions.h"
#include "newfiletype.h"
#include "addfolder.h"
#include "adddatabasefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DataTables; }
QT_END_NAMESPACE

class DataTables : public QDialog
{
    Q_OBJECT

public:
    DataTables(QWidget *parent = nullptr);
    ~DataTables();

public slots:

    void EditField(QStringList Line);
    void EditChapter(QStringList Line);
    void EditSection(QStringList Line);
    void EditDocumentType(QString Line);
    void EditTag(QString Line);

private slots:
    void LoadLists();
    void LoadFields();
    void LoadDocumentTypes();
    void LoadFileTypes();
    void LoadTags();
//    void on_AddFieldButton_clicked();
    void on_RemFieldButton_clicked();
    void on_FieldTable_itemClicked(QTableWidgetItem *item);
    void on_ComboFields_ChapterTab_currentIndexChanged(int index);
    void on_ComboFields_SectionTab_currentIndexChanged(int index);
    void on_ComboChapters_SectionTab_currentIndexChanged(int index);
//    void on_AddChapterButton_clicked();
    void on_RemoveChapterButton_clicked();
    void on_ChapterTable_itemClicked(QTableWidgetItem *item);
//    void on_AddSectionButton_clicked();
    void on_SectionTable_itemClicked(QTableWidgetItem *item);
    void on_RemoveSectionButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_EditFieldButton_clicked();
    void on_EditChapterButton_clicked();
    void on_EditSectionButton_clicked();
    void on_AddDocumentTypeButton_clicked();
    void on_RemDocumentTypeButton_clicked();
    void on_EditDocumentTypeButton_clicked();
    void on_DocumentTypeTable_itemClicked(QTableWidgetItem *item);
    void on_AddTag_clicked();
    void on_RemTag_clicked();
    void on_EditTag_clicked();
    void on_TagTable_itemClicked(QTableWidgetItem *item);
    void AddField(QStringList Line);
    void AddChapter(QStringList Line);
    void AddSection(QStringList Line);
    void AddExerciseType(QStringList Line);
    void EditExerciseType(QStringList Line);
    void AddDocumentType(QString Line);
    void AddTag(QString Line);
    void on_ComboFields_ExerciseTypeTab_currentIndexChanged(int index);
    void on_ComboChapters_ExerciseTypeTab_currentIndexChanged(int index);
    void on_ComboSections_ExerciseTypeTab_currentIndexChanged(int index);
//    void on_AddExerciseTypeButton_clicked();
    void on_RemoveExerciseTypeButton_clicked();
    void on_EditExerciseTypeButton_clicked();
    void on_ExerciseTypeTable_itemClicked(QTableWidgetItem *item);
    void UpdateDatabaseMetadata(QString Id,QString DBField,QString oldId1,QString newId1,QString oldPath1,QString newPath1,QString oldPath2,QString newPath2);

    void on_AddFileTypeButton_clicked();

    void on_RemFileTypeButton_clicked();
    void EditFileType(QStringList Line);
    void on_EditFileTypeButton_clicked();

private:
    Ui::DataTables *ui;
    AddDatabaseField * newLine;
    addfolder * newFolder;
    QSqlDatabase currentbase;
    QSqlDatabase currentbase_Notes;

    QStringList FieldIds;
    QStringList FieldNames;
    QStringList ChapterIds;
    QStringList ChapterNames;
    QStringList SectionIds;
    QStringList SectionNames;
    QStringList ExTypeIds;
    QStringList ExTypeNames;
    QStringList FileTypeIds;
    QStringList FileTypeNames;
    QStringList FileTypeFolders;
    QStringList DocumentTypes;
    QStringList CustomTags;


signals:

    void addline(QStringList Line);
    void addfolder_signal(QString Line);
};
#endif // DATATABLES_H
