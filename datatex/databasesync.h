#ifndef DATABASESYNC_H
#define DATABASESYNC_H

#include <QDialog>
#include <QTreeWidget>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include "switch.h"

namespace Ui {
class DatabaseSync;
}

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
    bool CheckMetadata(QString file, QSqlDatabase database);
    QList<QStringList> CompareLists(QStringList list1, QStringList list2);
    void ExclusiveSwitches(Switch * sw1,Switch * sw2);
    void SyncMetadataToCsvFile(QString file);
    void SyncMetadataToDatabase(QString file);
    void on_StartSync_clicked();
    void ShowProgress(int files, int total);

private:
    Ui::DatabaseSync *ui;
//    QHash<QString,QSqlDatabase> datalist;
    QHash<QString,QString> ContentsFromFiles;
    QHash<QString,QString> ContentsFromDatabase;
    QSqlDatabase currentBase;
    QString Table;
    QString rem;
    QString ContentType;
    QString DataTable;
    int TotalFiles;
    int FilesFound;
    bool filesync = false;
    bool metacheck = false;
    bool missfiles = false;
    QStringList Database_FileTableFields;
    QHash<QString,QStringList> DifferencesInCSV;
    QHash<QString,QStringList> DifferencesInDatabase;
    QHash<QString,QStringList> DatabaseFieldsWithDifferences;
    QHash<QString,QString> TexFilesMissing;
    QHash<QString,QString> PdfFilesMissing;
    QHash<QString,QString> CsvFilesMissing;
    QHash<QString,QStringList> BuildComPreamble;
    QStringList Database_FileTableFieldNames;
    QHash<QString,bool> SyncContentToDatabase;
    QHash<QString,bool> SyncMetadataToFile;
    QHash<QString,bool> CreateNewTexFile;
    QHash<QString,bool> CreateNewPdfFile;
    QHash<QString,bool> CreateNewCsvFile;
    QFont FoundFont;
    QBrush redBrush;    

signals:
    void progress(int files,int total);
};

#endif // DATABASESYNC_H
