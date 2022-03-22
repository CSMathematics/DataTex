#ifndef NEWDATABASEFILE_H
#define NEWDATABASEFILE_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include "pdfviewer.h"
#include <QListWidget>
#include <QTableView>
#include <QRadioButton>

namespace Ui {
class NewDatabaseFile;
}

class NewDatabaseFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewDatabaseFile(QWidget *parent = nullptr, QStringList meta = QStringList()
            , QStringList sections = QStringList(), bool editMode = false, QString fileName = QString(), bool cloneMode = false);
    ~NewDatabaseFile();
    static QString ClearMetadataFromContent(QString Content);

private:
    Ui::NewDatabaseFile *ui;

    QSqlDatabase currentbase;
    QString DataBase_Path;
    PdfViewer * TheoryView;
    QHash<QString,QString> mapIdsNames;
    QStringList SectionList;
    QStringList ImportedSectionList;
    QString FileType;
    QStringList Bibliography_Ids;
    bool EditMode;
    bool CloneMode;
    QString FileName;
    QStringList metadata;
    QList<QAbstractButton *> CustomFileTypesList;
    int NeedsSolution;
    QStringList Field_Names;
    QStringList Field_ids;
    QString ImportedFileContent;

private slots:

    void on_FieldTable_itemClicked(QListWidgetItem *item);
    void updateTableView(QTableView * table,QString QueryText);
    void ExerciseFileList_selection_changed();
    void loadImageFile(QString exoFile, PdfViewer *view);
    void on_SectionExercisesChapterComboBox_activated(int index);
    void on_SectionExercisesSectionComboBox_activated(int index);
    void on_SectionExercisesExTypeComboBox_activated(int index);
    QString NewFilePathAndId();
    void on_SectionList_itemClicked(QListWidgetItem *item);
    void on_SectionList_itemDoubleClicked(QListWidgetItem *item);
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_SelectedSections_itemClicked(QListWidgetItem *item);
    void on_ExerciseFileList_doubleClicked(const QModelIndex &index);
    void closeEvent (QCloseEvent *event);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_addChapter_clicked();
    void on_addField_clicked();
    void on_removeField_clicked();
    void on_removeChapter_clicked();
    void on_addSection_clicked();
    void on_removeSection_clicked();
    void on_addExType_clicked();
    void on_removeExType_clicked();
    void EditModeIsEnabled();
    void on_addFileType_clicked();
    void on_removeFileType_clicked();
    void FileTypeClicked();
    void on_SingleSection_clicked();
    void on_MultiSection_clicked();


    void on_DatabaseCombo_activated(int index);

signals:
    void acceptSignal(QString fileName,/* QMap<QString,QString> metadata,QStringList SectionList,*/QString FileContent);
    void acceptClone(QString Database,QString fileName,QString FileContent);
};

#endif // NEWDATABASEFILE_H
