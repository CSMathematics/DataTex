#ifndef BACKUP_H
#define BACKUP_H

#include <QDialog>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class BackUp;
}

class BackUp : public QDialog
{
    Q_OBJECT

public:
    explicit BackUp(QWidget *parent = nullptr);
    ~BackUp();

private slots:
    void on_BackUpFilesButton_clicked();
    void on_OpenDatabasesTreeWidget_itemSelectionChanged();
    void on_SelectPath_clicked();
    void CreateTexFiles();
    void CopyFiles(QStringList &list, QString folder);

private:
    Ui::BackUp *ui;
    QString destination;
    QString folderName;
    QString BackUpPath;
    bool isBackUp;
    bool LatexFilesSelected;
    bool PdfSelected;
    bool CsvSelected;
    bool DBFileSelected;
    bool DatabaseSelected;
    bool UpdatePathSelected;
    bool KeepFolderStructure;
    bool AddDateToName;
    bool hasSelection;
    bool newDatabaseUpdated;
    QString databaseName;
    QString databasePath;
    QString date;
    QStringList TexFiles;
    QStringList PdfFiles;
    QStringList CsvFiles;
    QSqlDatabase currentBase;
    QString Table;
    QString rem;
    QString ContentType;
    QString DataTable;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
    QHash<QString,QString> ContentsFromDatabase;
    QHash<QString,QStringList> BuildComPreamble;
};

#endif // BACKUP_H
