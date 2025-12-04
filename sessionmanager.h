#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QHash>
#include <QStringList>
#include <QTranslator>
#include <QTableView>
#include <QTreeView>
#include <QListWidget>
#include <QSqlDatabase>
#include "databasecreator.h"
#include "filecommands.h"

class ExtendedTableWidget;

class SessionManager
{
public:
    static SessionManager& instance();

    void updateTableView(QTableView * table, QString QueryText, QSqlDatabase Database, QObject *parent);
    void LoadTableHeaders(QTableView * table, QStringList list);
    void StretchColumns(QTableView * Table,float stretchFactor);
    void StretchColumns(QTreeView * Tree,float stretchFactor);
    void StretchColumnsToWidth(QTableView *table);
    QStringList GetListWidgetItems(QListWidget * list);
    void DBBackUp(QString database,QString dest_path);
    void runQuery_Root(QString queryText, QSqlDatabase database);
    QString getDataTexPath();
    bool SelectNewFileInModel(QTableView *table, QString newFile);
    void FunctionInProgress();

    DTXDatabase CurrentFilesDataBase;
    DTXDatabase CurrentDocumentsDataBase;
    DTXDatabase CurrentBibliographyDataBase;
    DTXDatabase CurrentTablesDataBase;
    DTXDatabase CurrentFiguresDataBase;
    DTXDatabase CurrentCommandsDataBase;
    DTXDatabase CurrentPreamblesDataBase;
    DTXDatabase CurrentPackagesDataBase;
    DTXDatabase CurrentClassesDataBase;
    DTXDatabase CurrentDTXDataBase;

    QSqlDatabase DataTeX_Settings;
    QSqlDatabase Bibliography_Settings;

    QHash<QString,DTXDatabase> GlobalDatabaseList;

    QString CurrentPreamble;
    QString CurrentPreamble_Content;
    QStringList DocTypesIds;
    QStringList DocTypesNames;
    QString PdfLatex_Command;
    QString Latex_Command;
    QString XeLatex_Command;
    QString LuaLatex_Command;
    QString Pythontex_Command;
    QString Bibtex_Command;
    QString Asymptote_Command;
    QString RunCommand;
    QHash<QString,QString> BuildCommands;
    QHash<int,DTXBuildCommand> DTXBuildCommands;
    QString GlobalSaveLocation;
    QString TexLivePath;
    QHash<QString,QStringList> Optional_DocMetadata_Ids;
    QHash<QString,QStringList> Optional_DocMetadata_Names;
    QTranslator translator;
    QString currentlanguage;
    QString datatexpath;
    QStringList SVG_IconPaths;

private:
    SessionManager();
    ~SessionManager();
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;
};

#endif // SESSIONMANAGER_H
