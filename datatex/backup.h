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

private:
    Ui::BackUp *ui;
    QString destination;
    bool isBackUp;
    bool LatexFilesSelected;
    bool PdfSelected;
    bool CsvSelected;
    bool DBFileSelected;
    bool DatabaseSelected;
    bool hasSelection;
    QString basenamebackup;
    QString basepathbackup;
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
