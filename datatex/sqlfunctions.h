#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

#include<QStringList>
#include<QSql>
#include<QSqlDatabase>
#include <QSqlQueryModel>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QtSql/QSql>
#include <QSqlError>
#include <QSqlRecord>
#include "datatex.h"

class SqlFunctions
{
public:
    SqlFunctions();

    static const QString Fields_Query;
    static const QString Chapters_Query;
    static const QString Sections_Chapters_Query;
    static const QString Sections_Query;
    static const QString Exercise_Types_Query;
    static const QString CreateFieldsTablesQuery;
    static const QString CreateClassTablesQuery;
    static const QString CreateChaptersTablesQuery;
    static const QString CreateSectionsTablesQuery;
    static const QString CreateExerciseTypesTablesQuery;
    static const QString CreateDatabaseFilesTablesQuery;
    static const QString CreateSectionExercisesTablesQuery;
    static const QString CreateFileTypesTablesQuery;
    static const QString CreateSubjectTypesTablesQuery;
    static const QString SelectCurrentDataBase;
    static const QString SelectCurrentNotesFolderBase;
    static const QString GetCurrentDataBaseFields;
    static const QString GetCurrentDataBaseBasicFields;
    static const QString GetCurrentDataBaseOptionalFields;
    static const QString GetCurrentDocDataBaseOptionalFields;
    static const QString GetCurrentBibliographyFields;
    static const QString CountFiles_by_Field;
    static const QString CountFiles_by_Chapter;
    static const QString CountFiles_by_Section;
    static const QString CountFiles_by_ExerciseType;
    static const QString CountFiles_by_FileType;
    static const QString CountDocs_by_DocType;
    static const QString CountDocs_by_BasicFolder;
    static const QString CountDocs_by_SubFolder;
    static const QString CountDocs_by_SubsubFolder;
    static const QString CountBib_by_DocType;
    static const QString CountBib_by_Author;
    static const QString CountBib_by_Publisher;
    static const QString Section_List_contaning_Exercises;
    static const QString FolderName;
    static const QString UpdateTableFiles;
    static const QString EmptyTable;
    static const QString UpdateSolution;
    static const QString SelestExerciseRow;
    static const QString GetDocumentTypes;
    static QString ShowAllFiles_SELECT;
    static QString ShowAllDatabaseFiles;
    static QString ShowDocuments;
    static QString FilterDatabaseDocuments;
    static const QString GetPreamble;
    static const QString GetPreamble_Content;
    static const QString GetBibliographyFields;
    static const QString GetColumnNames;
    static const QString GetLatexCommands;
    static const QString ShowFilesInADocument;
    static const QString ShowFilesInADocument_DifferentDatabase;
    static QString FilesTable_UpdateQuery;
    static QString ShowBibliographyEntries;
    static QString FilterBibliographyEntries;

    static QString GetFieldInfo;
    static QString GetFileTypeInfo;
    static QString GetChapterInfo;
    static QString GetSectionInfo;
    static QString GetSubsectionInfo;
    static QString GetFileInfo;
    static QString GetSolutionState;

public slots:

    static QList<QStringList> ComboList_Single(QString queryString,QSqlDatabase database,QString Arg1);
    static QList<QStringList> ComboList_Double_List(QString queryString,QSqlDatabase database);
    static QList<QStringList> ComboList_Exercises(QString queryString,QSqlDatabase database,
                                                  QString Arg1,QString Arg2,QString Arg3,QString Arg4);
    static const QString GetCurrentDataBase(QSqlDatabase database, QString databaseQuery);
    static QStringList Get_StringList_From_Query(QString queryString,QSqlDatabase database);
    static QString Get_String_From_Query(QString queryString,QSqlDatabase database);
    static QStringList Get_Record_From_Query(QString queryString,QSqlDatabase database);
    static QList<QStringList> GetRecordList(QString querytext,QSqlDatabase database);
    static int ExecuteSqlScriptFile(QSqlDatabase & database, const QString & fileName);
    static QString MultiUpdate(QStringList entries, QStringList values, QString Table , QString column_1, QString column_2);
    static QHash<QString,QString> ReadRow(QString QueryText,QSqlDatabase & database);
    static void ShowAllFiles(QStringList fields);
    static void FilterTable(QStringList fields, QStringList values, QStringList additionalFilters = QStringList());

private:

};

#endif // SQLFUNCTIONS_H
