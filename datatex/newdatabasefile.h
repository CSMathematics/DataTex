#ifndef NEWDATABASEFILE_H
#define NEWDATABASEFILE_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QListWidget>
#include <QTableView>
#include <QRadioButton>
#include <QSqlError>
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
    QString currentField;
    QString currentChapter;
    QString currentSection;
    QString currentSubSection;
    // TagsLineEditWidget * tagLine;
    QVector<QString> tags;
    bool saveSelections;
    QString CurrentFileContent;
    int index = 0;
    enum move{Back,Next};
    QButtonGroup * FileTypeGroup = nullptr;

private slots:

    void updateTableView(QTableView * table,QString QueryText);
    void NewFilePathAndId();
    void closeEvent (QCloseEvent *event);
    void buttonBox_accepted();
    void buttonBox_rejected();
    void addChapter_clicked();
    void addField_clicked();
    void removeField_clicked();
    void removeChapter_clicked();
    void addSection_clicked();
    void removeSection_clicked();
    void addSubSection_clicked();
    void removeSubSection_clicked();
    void EditModeIsEnabled();
    void CloneModeIsEnabled(int cloneMode);
    void addFileType_clicked();
    void removeFileType_clicked();
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
    void NextButton_clicked();
    void BackButton_clicked();
    void reset();

signals:
    void acceptSignal(QString fileName);
    void acceptClone();
    void sendFileInfo(int page,DTXFile * fileInfo,int move);
};

#endif // NEWDATABASEFILE_H
