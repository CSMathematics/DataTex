#include "sqlfunctions.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QComboBox>
#include <QString>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QDesktopServices>
#include <QList>
#include <QUrl>
#include <QDesktopServices>
#include <QGridLayout>
#include <QDebug>
#include <QModelIndex>
#include <algorithm>
#include <QCloseEvent>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QRadioButton>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include "datatex.h"

const QString SqlFunctions::Chapters_Query = "SELECT Chapters.Name,Chapters.Id "
                    "FROM Chapters JOIN Fields "
                    "ON Fields.Id = Chapters.Field "
                    "WHERE Fields.Name = \"%1\";";
const QString SqlFunctions::Sections_Chapters_Query = "SELECT Sections.Name,Sections.Id "
                        "FROM Sections JOIN Chapters "
                        "ON Sections.Chapter = Chapters.Id "
                        "WHERE Chapters.Name = \"%1\";";
const QString SqlFunctions::Sections_Query =
                        "SELECT Name,Id "
                        "FROM Sections "
                        "WHERE Field = \"%1\";";
const QString SqlFunctions::Exercise_Types_Query =
                        "SELECT DISTINCT et.Name,et.Id "
                        "FROM Sections_Exercises se "
                        "JOIN Exercise_Types et ON et.Id = se.Exercise_Id "
                        "WHERE Section_Id = \"%1\";";

const QString SqlFunctions::SelectCurrentDataBase =
                        "SELECT Value FROM Current_Database_Notes_Folder WHERE Setting = \"Current_DataBase\"";

const QString SqlFunctions::SelectCurrentNotesFolderBase =
                        "SELECT Value FROM Current_Database_Notes_Folder WHERE Setting = \"Current_Notes_Folder\"";

const QString SqlFunctions::GetCurrentDataBaseFields =
                        "SELECT Metadata_per_Database.Metadata_Id,Metadata.Name "
                        "FROM Metadata_per_Database JOIN Metadata "
                        "ON Metadata_per_Database.Metadata_Id = Metadata.Id "
                        "WHERE Database_FileName = \"%1\";";
const QString SqlFunctions::GetCurrentDataBaseBasicFields =
                        "SELECT Metadata_per_Database.Metadata_Id,Metadata.Name "
                        "FROM Metadata_per_Database JOIN Metadata "
                        "ON Metadata_per_Database.Metadata_Id = Metadata.Id "
                        "WHERE Database_FileName = \"%1\" AND Basic=1;";
const QString SqlFunctions::GetCurrentDataBaseOptionalFields =
                        "SELECT Metadata_per_Database.Metadata_Id,Metadata.Name "
                        "FROM Metadata_per_Database JOIN Metadata "
                        "ON Metadata_per_Database.Metadata_Id = Metadata.Id "
                        "WHERE Database_FileName = \"%1\" AND Basic=0;";

const QString SqlFunctions::GetCurrentDocDataBaseOptionalFields =
                        "SELECT DocMetadata_per_Database.Metadata_Id,DocMetadata.Name "
                        "FROM DocMetadata_per_Database JOIN DocMetadata "
                        "ON DocMetadata_per_Database.Metadata_Id = DocMetadata.Id "
                        "WHERE Database_FileName = \"%1\" AND Basic=0;";

const QString SqlFunctions::GetCurrentBibliographyFields =
                        "SELECT Bibliographic_Fields_per_Database.Bibliographic_Field,Bibliography.Name "
                        "FROM Bibliographic_Fields_per_Database JOIN Bibliography "
                        "ON Bibliographic_Fields_per_Database.Bibliographic_Field = Bibliography.Id "
                        "WHERE Database = \"%1\"";

const QString SqlFunctions::Fields_Query =
                        "SELECT Name,Id "
                        "FROM Fields "
                        "WHERE Name <> \"-\";";

const QString SqlFunctions::CountFiles_by_Field =
                        "SELECT Fields.Name AS 'Field',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files JOIN Fields "
                        "ON Fields.Id = DataBase_Files.Field "
                        "GROUP BY DataBase_Files.Field "
                        "ORDER BY 2 DESC;";
const QString SqlFunctions::CountFiles_by_Chapter =
                        "SELECT c.Name AS 'Chapter',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files df "
                        "JOIN Chapters_per_File cpf ON cpf.File_Id = df.Id "
                        "JOIN Chapters c ON c.Id = cpf.Chapter_Id "
                        "GROUP BY c.Name "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_Section =
                        "SELECT s.Name AS 'Chapter',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files df "
                        "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                        "JOIN Sections s ON s.Id = spf.Section_Id "
                        "GROUP BY s.Name "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_ExerciseType =
                        "SELECT e.Name AS 'Chapter',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files df "
                        "JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                        "JOIN Exercise_Types e ON e.Id = epf.ExerciseType_Id "
                        "WHERE e.Id <> '-' "
                        "GROUP BY e.Name "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_FileType =
                        "SELECT FileTypes.FileType AS 'File type',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files JOIN FileTypes "
                        "ON FileTypes.Id = DataBase_Files.FileType "
                        "GROUP BY DataBase_Files.FileType "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::Section_List_contaning_Exercises =
                        "SELECT DISTINCT s.Name,s.Id "
                        "FROM DataBase_Files df "
                        "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                        "JOIN Sections s ON s.Id = spf.Section_Id "
                        "WHERE df.Field = \"%1\" AND df.FileType = \"%2\" "
                        "ORDER BY s.Name;";

const QString SqlFunctions::FolderName =
                                "SELECT FolderName "
                                "FROM FileTypes "
                                "WHERE Id = \"%1\";";

const QString SqlFunctions::UpdateTableFiles =
                                "SELECT df.Id AS 'Name',df.Date,df.Path  "
        "FROM Database_Files df "
        "JOIN FileTypes ft ON df.FileType = ft.Id "
        "JOIN Chapters_per_File cpf ON cpf.File_Id = df.Id "
        "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
        "JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
        "GROUP by df.Id "
        "HAVING df.Field REGEXP \"%1\" "
        "AND (cpf.Chapter_Id REGEXP \"%2\" OR cpf.Chapter_Id ISNULL) "
        "AND spf.Section_Id REGEXP \"%3\" "
        "AND (epf.ExerciseType_Id REGEXP \"%4\" OR epf.ExerciseType_Id ISNULL) "
        "AND ft.Id = '%5'";

const QString SqlFunctions::GetSectionsCombFiles =
                                "SELECT DISTINCT replace(group_concat(s.Name),',','|'),group_concat(s.Id) "
                                "FROM Database_Files df "
                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                "JOIN (SELECT * FROM Sections s ORDER BY s.Id) s ON s.Id = spf.Section_Id "
                                "WHERE FileType = \"%2\" AND df.Field = \"%1\" "
                                "GROUP BY df.Id;";

const QString SqlFunctions::ShowSolvedAndUnSolvedExercises  =
                                "SELECT df.Id AS 'Id',%1,df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
                                "FROM Database_Files df "
                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                "JOIN Sections s ON s.Id = spf.Section_Id "
                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                                "WHERE df.FileType = \"%2\" AND spf.Section_Id = \"%3\" "
                                "ORDER BY df.Id;";

const QString SqlFunctions::ShowSolvedAndUnSolvedExercisesFiltered  =
                                "SELECT df.Id AS 'Id',%1,df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
                                "FROM Database_Files df "
                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                "JOIN Sections s ON s.Id = spf.Section_Id "
                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                                "WHERE df.FileType = \"%2\" AND spf.Section_Id = \"%3\" "
                                "AND df.Id LIKE \"%%4%\" "
                                "AND et.Name LIKE \"%%5%\" "
                                "AND Solved_Prooved LIKE \"%%6%\" "
                                "ORDER BY df.Id;";
//FilteredComb

const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercises =
                                "SELECT df.Id AS 'Id',replace(group_concat(DISTINCT s.Name),',','|') AS sec, "
                                "df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
                                "FROM Database_Files df "
                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                "JOIN Sections s ON s.Id = spf.Section_Id "
                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                                "WHERE df.FileType = \"%1\" "
                                "GROUP BY df.Id "
                                "HAVING %2 "
                                "ORDER BY df.Id;";

const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercisesFiltered =
                                "SELECT df.Id AS 'Id',replace(group_concat(DISTINCT s.Name),',','|') AS 'sec', "
                                "df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
                                "FROM Database_Files df "
                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                "JOIN Sections s ON s.Id = spf.Section_Id "
                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                                "WHERE df.FileType = \"%1\" "
                                "AND df.Id LIKE \"%%3%\" "
                                "AND Solved_Prooved LIKE \"%%5%\" "
                                "GROUP BY df.Id "
                                "HAVING (sec LIKE \"%%2%\" AND sec LIKE \"%%4%\") "
                                "ORDER BY df.Id;";

const QString SqlFunctions::EmptyTable = "SELECT * FROM \'Database_Files\" WHERE 1=0;";

const QString SqlFunctions::UpdateSolution =
                                "UPDATE Database_Files SET Solved_Prooved = "
                                "CASE "
                                "WHEN (SELECT count(Solution_Id) "
                                "FROM Solutions_per_File "
                                "WHERE File_Id = \"%1\") "
                                "THEN 'YES' "
                                "ELSE 'NO' "
                                "END "
                                "WHERE Id = \"%1\"";

const QString SqlFunctions::SelestExerciseRow = "SELECT df.*, replace(group_concat(s.Id),',','|') AS 'Section' "
                                                "FROM Database_Files df "
                                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                                                "JOIN Sections s ON s.Id = spf.Section_Id "
                                                "WHERE df.Id = '%1'";

const QString SqlFunctions::GetDocumentTypes = "SELECT Name FROM Document_Types ORDER BY rowid;";

QString SqlFunctions::ShowAllDatabaseFiles;

QString SqlFunctions::FilesTable_UpdateQuery;

const QString SqlFunctions::GetPreamble = "SELECT Value "
                                "FROM Initial_Settings "
                                "WHERE Setting = 'Current_Preamble'";

const QString SqlFunctions::GetPreamble_Content = "SELECT Preamble_Content FROM Preambles WHERE Id = \"%1\";";

const QString SqlFunctions::GetBibliographyFields =
                                "SELECT Bibliographic_Field,Name "
                                "FROM Bibliographic_Fields_per_Database bf "
                                "JOIN Bibliography b "
                                "ON bf.Bibliographic_Field = b.Id "
                                "WHERE bf.Database = \"%1\";";
const QString SqlFunctions::GetColumnNames = "SELECT name FROM pragma_table_info('%1') JOIN (SELECT COUNT(*) FROM %1);";

const QString SqlFunctions::GetLatexCommands = "UPDATE Initial_Settings SET Value = :pdf WHERE Setting = 'Pdflatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :latex WHERE Setting = 'Latex_Command'; "
                                "UPDATE Initial_Settings SET Value = :xelatex WHERE Setting = 'Xelatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :lua WHERE Setting = 'Lualatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :python WHERE Setting = 'Pythontex_Command';";

const QString SqlFunctions::ShowFilesInADocument =
                            "SELECT DISTINCT df.Id, \"%2\" AS \"Database source\" ,ft.FileType, "
                            "replace(group_concat(DISTINCT s.Name),',','|') AS 'Section', "
                            "replace(group_concat(DISTINCT et.Name),',','|'),Path,Solved_Prooved,df.FileType, "
                            "replace(group_concat(DISTINCT bpf.Bib_Id),',','|') "
                            "FROM Database_Files df "
                            "JOIN FileTypes ft ON ft.Id = df.FileType "
                            "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
                            "JOIN Sections s ON s.Id = spf.Section_Id "
                            "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                            "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                            "LEFT JOIN Sections_Exercises se ON se.Exercise_Id = et.Id "
                            "LEFT JOIN Bib_Entries_per_File bpf ON bpf.File_Id = df.Id "
                            "WHERE df.Id IN %1 "
                            "GROUP BY df.Id "/*
                            "ORDER BY \"df\".rowid "*/;

const QString SqlFunctions::ShowFilesInADocument_DifferentDatabase =
                            "SELECT DISTINCT df.Id, \"%2\" AS \"Database source\",ft.FileType, "
                            "replace(group_concat(DISTINCT s.Name),',','|') AS 'Section', "
                            "replace(group_concat(DISTINCT et.Name),',','|'), "
                            "Path,Solved_Prooved,df.FileType, "
                            "replace(group_concat(DISTINCT bpf.Bib_Id),',','|') "
                            "FROM \"%2\".Database_Files df "
                            "JOIN \"%2\".FileTypes ft ON ft.Id = df.FileType "
                            "JOIN \"%2\".Sections_per_File spf ON spf.File_Id = df.Id "
                            "JOIN \"%2\".Sections s ON s.Id = spf.Section_Id "
                            "LEFT JOIN \"%2\".ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                            "LEFT JOIN \"%2\".Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                            "LEFT JOIN \"%2\".Sections_Exercises se ON se.Exercise_Id = et.Id "
                            "LEFT JOIN \"%2\".Bib_Entries_per_File bpf ON bpf.File_Id = df.Id "
                            "WHERE df.Id IN %1 "
                            "GROUP BY df.Id ";
QString SqlFunctions::FilterDatabaseDocuments;

QString SqlFunctions::ShowBibliographyEntries = "SELECT b.*,replace(group_concat(DISTINCT apb.FullName),',',' and ') AuthorConcat, "
        "replace(group_concat(DISTINCT epb.FullName),',',' and ') EditorConcat, "
        "replace(group_concat(DISTINCT tpb.FullName),',',' and ') TranslatorConcat "
        "FROM Bibliography b "
        "LEFT JOIN Authors_per_BibEntry apb ON b.Citation_Key = apb.BibEntry_Id "
        "LEFT JOIN Editors_per_BibEntry epb ON b.Citation_Key = epb.BibEntry_Id "
        "LEFT JOIN Translators_per_BibEntry tpb ON b.Citation_Key = tpb.BibEntry_Id "
        "GROUP BY Citation_Key ORDER BY b.ROWID";

QString SqlFunctions::FilterBibliographyEntries;

SqlFunctions::SqlFunctions()
{

}

QStringList SqlFunctions::Get_StringList_From_Query(QString queryString,QSqlDatabase database)
{
    QStringList List;
    QString queryText = QString(queryString);
    QSqlQuery query(database);
    query.exec(queryText);
    while(query.next()){
        List.append(query.value(0).toString());
    }
    return List;
}

QList<QStringList> SqlFunctions::ComboList_Single(QString queryString,QSqlDatabase database,QString Arg)
{
    QList<QStringList> data;
    QStringList Name_List;
    QStringList Id_List;
    QString queryText = QString(queryString).arg(Arg);
    QSqlQuery query(database);
    query.exec(queryText);
    while(query.next()){
        Name_List.append(query.value(0).toString());
        Id_List.append(query.value(1).toString());
    }
    data <<Name_List<<Id_List;
    return data;
}

QList<QStringList> SqlFunctions::ComboList_Double_List(QString queryString,QSqlDatabase database)
{
    QList<QStringList> data;
    QStringList Name_List;
    QStringList Id_List;
    QString queryText = QString(queryString);
    QSqlQuery query(database);
    query.exec(queryText);
    while(query.next()){
        Name_List.append(query.value(0).toString());
        Id_List.append(query.value(1).toString());
    }
    data <<Name_List<<Id_List;
    return data;
}

QList<QStringList> SqlFunctions::ComboList_Exercises(QString queryString,QSqlDatabase database,
                                                     QString Arg1,QString Arg2,QString Arg3,QString Arg4)
{
    QList<QStringList> data;
    QStringList Name_List;
    QStringList Id_List;
    QString queryText = QString(queryString).arg(Arg1).arg(Arg2).arg(Arg3).arg(Arg4);
    QSqlQuery query(database);
    query.exec(queryText);
    while(query.next()){
        Name_List.append(query.value(0).toString());
        Id_List.append(query.value(1).toString());
    }
    data <<Name_List<<Id_List;
    return data;
}

const QString SqlFunctions::GetCurrentDataBase(QSqlDatabase database, QString databaseQuery)
{
    QString Base_Path;
    QSqlQuery Select_DataBase_Path(database);
    Select_DataBase_Path.exec(databaseQuery);
    while(Select_DataBase_Path.next()){
    Base_Path = Select_DataBase_Path.value(0).toString();}
    return Base_Path;
}

int SqlFunctions::ExecuteSqlScriptFile(QSqlDatabase & database, const QString & fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return  0;

    QTextStream in(&file);
    QString sql = in.readAll();
    QStringList sqlStatements = sql.split("end_of_query", QString::SkipEmptyParts);
    int successCount = 0;

    foreach(const QString& statement, sqlStatements)
    {
        if (statement.trimmed() != "")
        {
            QSqlQuery query(database);
            if (query.exec(statement))
                successCount++;
            else
                qDebug() << "Failed:" << statement << "\nReason:" << query.lastError();
        }
    }
    return successCount;
}

QString SqlFunctions::MultiUpdate(QStringList settings, QStringList values, QString Table, QString column_1, QString column_2)
{

    return "query";
}

QHash<QString,QString> SqlFunctions::ReadRow(QString QueryText,QSqlDatabase & database)
{
    QHash<QString,QString> list;
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery DatabaseQuery(database);
    DatabaseQuery.exec(QueryText/*CsvFunctions::getFile("alg-eq-e1-Def1")*/);
    model->setQuery(DatabaseQuery);
    for(int i = 0; i < model->columnCount(); i++)
    {
      list.insert(model->headerData(i, Qt::Horizontal).toString(),
                  model->data(model->index(0,i)).toString());
    }
    return list;
}
