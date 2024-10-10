#ifndef NEWDATABASEFILE_H
#define NEWDATABASEFILE_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include "qpdfviewer.h"
#include <QListWidget>
#include <QTableView>
#include <QRadioButton>
#include <QSqlError>
#include "tagslineeditwidget.h"
#include "adddatabasefield.h"
#include "sqlfunctions.h"
#include "clonedatabasefile.h"
#include "datatex.h"
#include "newfiletype.h"

struct DTXFile;
namespace Ui {
class NewDatabaseFile;
}

class NewDatabaseFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewDatabaseFile(QWidget *parent = nullptr, DTXFile *fileinfo = nullptr, int mode = 0 /*bool editMode = false,
                             bool cloneMode = false, bool importMode = false,bool cloneMetadata = false*/);
    ~NewDatabaseFile();
    void hideButton(bool setHidden);
    void setFinishButton(bool isLast);
    DTXFile * getFileInfo();
    void CloneModeIsEnabled(DTXFile * fileInfo,bool cloneMetadata);

private:
    Ui::NewDatabaseFile *ui;
    QSqlDatabase currentbase;
    QString DataBase_Path;
    QPdfViewer * TheoryView;
    QStringList ImportedChaptersList;
    QStringList ImportedSectionList;
    QStringList ImportedSubSectionList;
    DTXFileType FileType;
    QStringList Bibliography_Ids;
    int Mode;
    QString FileName;
    QString FilePath;
    DTXFile * metadata = nullptr;
    QList<QAbstractButton *> CustomFileTypesList;
    QStringList Field_Names;
    QStringList Field_ids;
    QString ImportedFileContent;
    QList<QStringList> FieldList;
    QSet<QString> Selected_Field_ids;
    QSet<QString> Selected_Field_names;
    QSet<QString> Selected_Chapters_ids;
    QSet<QString> Selected_Chapters_names;
    QSet<QString> Selected_Sections_ids;
    QSet<QString> Selected_Sections_names;
    QSet<QString> Selected_SubSections_ids;
    QSet<QString> Selected_SubSections_names;
    // QStringList Sections_Names;
    // QStringList Sections_ids;
    // QStringList SubSections_Names;
    // QStringList SubSections_ids;
    QString currentField;
    QString currentChapter;
    QString currentSection;
    QString currentSubSection;
//    bool needsSubSection;
    TagsLineEditWidget * tagLine;
    QVector<QString> tags;
    bool saveSelections;
    QString CurrentFileContent;
    int index = 0;
    enum move{Back,Next};
    QButtonGroup * FileTypeGroup = nullptr;

private slots:

    void updateTableView(QTableView * table,QString QueryText);
    void ExerciseFileList_selection_changed();
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
    void CloneModeIsEnabled(int cloneMode);

    void on_addFileType_clicked();
    void on_removeFileType_clicked();
    void FileTypeClicked();
    void FieldsClicked(QListWidgetItem *item);
    void ChaptersClicked(QListWidgetItem * item);
    void SectionClicked(QListWidgetItem *item);
    void SubSectionClicked(QListWidgetItem *item);
    void UpdateFileInfo();
    QList<QListWidgetItem *> FindListItemByData(QListWidget *list,QString text);
    void LoadFileTypes();
    void InitialSettings();
    void SaveSettings();
    QList<QStringList> GetDataFromSelectionList(QListWidget *list);
    void setDBFileInfo();
    void on_NextButton_clicked();
    void on_BackButton_clicked();
    void reset();

signals:
    void acceptSignal(QString fileName);
    void acceptClone();
    void sendFileInfo(int page,DTXFile * fileInfo,int move);
};

#endif // NEWDATABASEFILE_H
