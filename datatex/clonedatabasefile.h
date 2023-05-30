#ifndef CLONEDATABASEFILE_H
#define CLONEDATABASEFILE_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSet>
#include <QWizard>
#include "datatex.h"
#include "sqlfunctions.h"
#include "qpdfviewer.h"
#include "newdatabasefile.h"
#include "combolist.h"
#include "rightclickmenu.h"
#include "filecommands.h"
#include "switch.h"
#include "style.h"

class DBFileInfo;
enum NewFileMode {
    BasicMode,ImportMode,EditMode,CloneModeOnlyContent,CloneModeContentAndMetadata
};

namespace Ui {
class CloneDatabaseFile;
}

class CloneDatabaseFile : public QDialog
{
    Q_OBJECT

public:
    explicit CloneDatabaseFile(QWidget *parent = nullptr);
    ~CloneDatabaseFile();


private slots:
    void on_Okbutton_rejected();
    void closeEvent (QCloseEvent *event);
    void AddFiles(int row);
    void on_removeButton_clicked();
    void RemoveFile(int row);
    void on_Okbutton_accepted();
    void FilesTable_selectionchanged();
    void LoadDatabaseFiles(QSqlDatabase database);
    void updateFilter(QStringList values);
//    void on_checkBox_clicked(bool checked);
    void on_FilesDatabasesCombo_activated(int index);
    int CountModelRows();
    void on_addEverything_clicked();
    void CreateCustomTagWidget(QSqlDatabase database);
    void on_SelectedFiles_itemClicked(QTreeWidgetItem *item, int column);
    void disableItems(QComboBox * combo, int excludeItems, bool checked);
    bool CheckMetadataResemblances(DBFileInfo * fileInfo,QSqlDatabase destinationDatabase);
    void ActionsWidget(QTreeWidgetItem * item, bool hassResemb);
    void ShowReseblences(DBFileInfo * fileInfo);

private:
    Ui::CloneDatabaseFile *ui;
    QSqlDatabase SourceDatabase;
    QSqlDatabase DestinationDatabase;
    QString SourceDatabaseName;
    QSqlTableModel * tableModel;
    QString PreviewFile;
    QString ExSubType;
    QString CurrentDatabaseFile;
    ExtendedTableWidget * FilesTable;
    QPdfViewer *view;
    QPdfViewer *FileView;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
    RightClickMenu * RightClick;
    TagsFilterWidget * filesTagLine = nullptr;
    QSortFilterProxyModel *FilesProxyModel;
    bool filesSorting;
    QSqlQueryModel * FilesModel;
    QString singlefile;
    QSet<QString> SelectedDatabaseList;
    QHash<QString,QSet<QString>> SelectedFileList;
    QList<DBFileInfo *> FileList;
    QList<DBFileInfo *> SolutionsList;
    QList<Switch *> CloneMetadataSwitches;
    QList<QPushButton *> MetadataResemblancesButtons;
//    int cloneMode = NewFileMode::CloneModeOnlyContent;
    bool hasSelection = false;
    bool isImportMode = true;
    int needsSubSection;

signals:
    void acceptSignal(QString Path);
    void sendResemblenceInfo(DBFileInfo fileInfo);
};
Q_DECLARE_METATYPE(QSqlDatabase)

#endif // CLONEDATABASEFILE_H
