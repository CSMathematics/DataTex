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
#include "tagslineeditwidget.h"

namespace Ui {
class NewDatabaseFile;
}

class NewDatabaseFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewDatabaseFile(QWidget *parent = nullptr, QHash<QString,QString> meta = QHash<QString,QString>()
            , QStringList chapters = QStringList(), QStringList sections = QStringList(),
              QStringList subsections = QStringList(), bool editMode = false,
              QString filePath = QString(), bool cloneMode = false);
    ~NewDatabaseFile();
    static QString ClearMetadataFromContent(QString Content);

private:
    Ui::NewDatabaseFile *ui;

    QSqlDatabase currentbase;
    QString DataBase_Path;
    PdfViewer * TheoryView;
    QHash<QString,QString> mapIdsNames;
    QStringList ImportedChaptersList;
    QStringList ImportedSectionList;
    QStringList ImportedSubSectionList;
    QString FileType;
    QStringList Bibliography_Ids;
    bool EditMode;
    bool CloneMode;
    QString FileName;
    QString FilePath;
    QHash<QString,QString> metadata;
    QList<QAbstractButton *> CustomFileTypesList;
    int NeedsSolution;
    QStringList Field_Names;
    QStringList Field_ids;
    QString ImportedFileContent;
    QStringList Selected_Field_ids;
    QStringList Selected_Chapters_ids;
    QStringList Selected_Sections_ids;
    QStringList Selected_SubSections_ids;
    QStringList Chapter_Names;
    QStringList Chapter_ids;
    QStringList Sections_Names;
    QStringList Sections_ids;
    QStringList SubSections_Names;
    QStringList SubSections_ids;
    QStringList FieldsSelected;
    QStringList ChaptersSelected;
    QStringList SectionsSelected;
    QStringList SubSectionsSelected;
    bool needsSubSection;
    TagsLineEditWidget * tagLine;
    QVector<QString> tags;

private slots:

    void updateTableView(QTableView * table,QString QueryText);
    void ExerciseFileList_selection_changed();
    void loadImageFile(QString exoFile, PdfViewer *view);
    void NewFilePathAndId();
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
    void FieldsClicked();
    void ChaptersClicked();
    void SectionClicked();
    void UpdateFileInfo();
    void SubSectionClicked();
    QList<QListWidgetItem *> FindListItemByData(QListWidget *list,QString text);
//    void NewFilePathAndId_2();

signals:
    void acceptSignal(QString fileName,/* QHash<QString,QString> metadata,QStringList SectionList,*/QString FileContent);
    void acceptClone(QString Database,QString fileName,QString FileContent);
};

#endif // NEWDATABASEFILE_H
