#ifndef DATABASESYNC_H
#define DATABASESYNC_H

#include <QDialog>
#include <QTreeWidget>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QHash>
#include <iostream>
#include "switch.h"

namespace Ui {
class DatabaseSync;
}

enum SyncFlags {SyncContentToDatabase = 1 <<0,
                 SyncContentToFile    = 1 <<1,
                 MirrorFromDatabase   = 1 <<2,
                 MirrorFromDTex       = 1 <<3,
                 MirrorFromCSV        = 1 <<4,
                 CreateTex            = 1 <<5,
                 CreatePdf            = 1 <<6,
                 CreateCSV            = 1 <<7,
                 CreateDTex           = 1 <<8,
                 DeleteEntry          = 1 <<9

};
Q_DECLARE_FLAGS(Options, SyncFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Options)

struct FileScanResults
{
    QString Id;
    QString Path;
    QString BuildCommand;
    QString Preamble;
    QString ContentFromFile;
    QString ContentFromDatabase;
    QFlags<SyncFlags> SyncContentFlag;
    QFlags<SyncFlags> SyncMetadataFlag;
    bool CreateMissingFile;
    QFlags<SyncFlags> CreateMissingFileFlag;
    QHash<QString,QString> DifferencesInCSV;
    QHash<QString,QString> DifferencesInDatabase;
    QHash<QString,QString> DifferencesInDTex;
    bool SyncFile = true;

//    QHash<QString,QString> TexFilesMissing;
//    QHash<QString,QString> PdfFilesMissing;
//    QHash<QString,QString> CsvFilesMissing;
//    QHash<QString,QString> DTexFilesMissing;
//    QHash<QString,QStringList> BuildComPreamble;
//    QStringList Database_FileTableFieldNames;
//    QHash<QString,bool> SyncContentToDatabase;
//    QHash<QString,bool> SyncMetadataToFile;
//    QHash<QString,bool> CreateNewTexFile;
//    QHash<QString,bool> CreateNewPdfFile;
//    QHash<QString,bool> CreateNewCsvFile;
//    QHash<QString,bool> CreateNewDTexFile;
};

class DatabaseSync : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSync(QWidget *parent = nullptr);
    ~DatabaseSync();

private slots:
    void on_OpenDatabasesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_ResultsTreeWidget_itemSelectionChanged();
    void on_ScanFiles_clicked();
    void on_SyncCheck_clicked(bool checked);
    void on_MetaCheck_clicked(bool checked);
    void on_MissingCheck_clicked(bool checked);
    bool CheckMetadata(QString result, QSqlDatabase database);
    QList<QHash<QString,QString>> CompareLists(QHash<QString, QString> list1, QHash<QString, QString> list2);
//    void ExclusiveSwitches(Switch * sw1,Switch * sw2);
    void SyncMetadataToDatabase(QString file);
    void *AddItem(FileScanResults result, int flag, int level1, int level2=-1);
    void Sync(FileScanResults results);
    void on_StartSync_clicked();
    void ShowProgress(int files, int total);

private:
    enum {ContentDifferences,Metadata,MissingFiles};
    QHash<QString,FileScanResults> ResultsList;
    Ui::DatabaseSync *ui;
    QHash<QString,QString> ContentsFromFiles;
    QHash<QString,QString> ContentsFromDatabase;
    QSqlDatabase currentBase;
    QString Table;
    QString rem;
    QString ContentType;
    QString DataTable;
    int TotalFiles;
    int FilesFound;
    int IndexInList;
    bool filesync = false;
    bool metacheck = false;
    bool missfiles = false;
    QHash<QString,QString> Database_FileTableFields;
    QHash<QString,QHash<QString,QString>> DifferencesInCSV;
    QHash<QString,QHash<QString,QString>> DifferencesInDatabase;
    QHash<QString,QHash<QString,QString>> DifferencesInDTex;
    QHash<QString,QStringList> DatabaseFieldsWithDifferences;
    QHash<QString,QString> TexFilesMissing;
    QHash<QString,QString> PdfFilesMissing;
    QHash<QString,QString> CsvFilesMissing;
    QHash<QString,QString> DTexFilesMissing;
    QHash<QString,QStringList> BuildComPreamble;
    QStringList Database_FileTableFieldNames;
//    QHash<QString,bool> SyncContentToDatabase;
//    QHash<QString,bool> SyncMetadataToFile;
//    QHash<QString,bool> CreateNewTexFile;
//    QHash<QString,bool> CreateNewPdfFile;
//    QHash<QString,bool> CreateNewCsvFile;
//    QHash<QString,bool> CreateNewDTexFile;
    QFont FoundFont;
    QBrush redBrush;    

signals:
    void progress(int files,int total);
    QFlags<SyncFlags> actionChanged(QFlags<SyncFlags> flag);
    bool createFile(bool createfile);
};
Q_DECLARE_METATYPE(SyncFlags)
#endif // DATABASESYNC_H
