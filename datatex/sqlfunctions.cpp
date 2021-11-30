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

const QString SqlFunctions::Chapters_Query = "SELECT \"Chapters\".\"Name\",\"Chapters\".\"Id\""
                    "FROM \"Chapters\" JOIN \"Fields\""
                    "ON \"Fields\".\"Id\" = \"Chapters\".\"Field\""
                    "WHERE \"Fields\".\"Name\" = \"%1\";";
const QString SqlFunctions::Sections_Chapters_Query = "SELECT \"Sections\".\"Name\",\"Sections\".\"Id\""
                        "FROM \"Sections\" JOIN \"Chapters\""
                        "ON \"Sections\".\"Chapter\" = \"Chapters\".\"Id\""
                        "WHERE \"Chapters\".\"Name\" = \"%1\";";
const QString SqlFunctions::Sections_Query =
                        "SELECT \"Name\",\"Id\""
                        "FROM \"Sections\""
                        "WHERE \"Field\" = \"%1\";";
const QString SqlFunctions::Exercise_Types_Query =
                        "SELECT \"Exercise_Name\",\"Exercise_Id\" "
                        "FROM \"Sections_Exercises\" "
                        "WHERE \"Section_Id\" = \"%1\";";

const QString SqlFunctions::SelectCurrentDataBase =
                        "SELECT Value FROM Current_Database_Notes_Folder WHERE Setting = \"Current_DataBase\"";

const QString SqlFunctions::SelectCurrentNotesFolderBase =
                        "SELECT Value FROM Current_Database_Notes_Folder WHERE Setting = \"Current_Notes_Folder\"";

const QString SqlFunctions::GetCurrentDataBaseFields =
                        "SELECT \"Metadata_per_Database\".\"Metadata_Id\",\"Metadata\".\"Name\""
                        "FROM \"Metadata_per_Database\" JOIN \"Metadata\""
                        "ON \"Metadata_per_Database\".\"Metadata_Id\" = \"Metadata\".\"Id\""
                        "WHERE \"Database_FileName\" = \"%1\";";
const QString SqlFunctions::GetCurrentDataBaseBasicFields =
                        "SELECT \"Metadata_per_Database\".\"Metadata_Id\",\"Metadata\".\"Name\""
                        "FROM \"Metadata_per_Database\" JOIN \"Metadata\""
                        "ON \"Metadata_per_Database\".\"Metadata_Id\" = \"Metadata\".\"Id\""
                        "WHERE \"Database_FileName\" = \"%1\" AND \"Basic\"=1;";
const QString SqlFunctions::GetCurrentDataBaseOptionalFields =
                        "SELECT \"Metadata_per_Database\".\"Metadata_Id\",\"Metadata\".\"Name\""
                        "FROM \"Metadata_per_Database\" JOIN \"Metadata\""
                        "ON \"Metadata_per_Database\".\"Metadata_Id\" = \"Metadata\".\"Id\""
                        "WHERE \"Database_FileName\" = \"%1\" AND \"Basic\"=0;";

const QString SqlFunctions::GetCurrentDocDataBaseOptionalFields =
                        "SELECT \"DocMetadata_per_Database\".\"Metadata_Id\",\"DocMetadata\".\"Name\""
                        "FROM \"DocMetadata_per_Database\" JOIN \"DocMetadata\""
                        "ON \"DocMetadata_per_Database\".\"Metadata_Id\" = \"DocMetadata\".\"Id\""
                        "WHERE \"Database_FileName\" = \"%1\" AND \"Basic\"=0;";

const QString SqlFunctions::GetCurrentBibliographyFields =
                        "SELECT \"Bibliographic_Fields_per_Database\".\"Bibliographic_Field\",\"Bibliography\".\"Name\""
                        "FROM \"Bibliographic_Fields_per_Database\" JOIN \"Bibliography\""
                        "ON \"Bibliographic_Fields_per_Database\".\"Bibliographic_Field\" = \"Bibliography\".\"Id\""
                        "WHERE \"Database\" = \"%1\"";

const QString SqlFunctions::Fields_Query =
                        "SELECT \"Name\",\"Id\""
                        "FROM \"Fields\""
                        "WHERE \"Name\" <> \"-\";";

const QString SqlFunctions::SelectExercises =
                        "SELECT \"Id\" , \"Path\""
                        "FROM \"DataBase_Files\""
                        "WHERE \"FileType\" = \"%1\" AND \"Field\" = \"%2\" "
                        "AND \"Section\" = \"%3\" AND \"ExerciseType\" = \"%4\""
                        "ORDER BY \"Id\";";

const QString SqlFunctions::CountFiles_by_Field =
                        "SELECT \"Fields\".\"Name\" AS 'Field',COUNT(*) AS 'Number' "
                        "FROM \"DataBase_Files\" JOIN \"Fields\""
                        "ON \"Fields\".\"Id\" = \"DataBase_Files\".\"Field\""
                        "GROUP BY \"DataBase_Files\".\"Field\""
                        "ORDER BY 2 DESC;";
const QString SqlFunctions::CountFiles_by_Chapter =
                        "SELECT \"Chapters\".\"Name\" AS 'Chapter',COUNT(*) AS 'Number'"
                        "FROM \"DataBase_Files\" JOIN \"Chapters\""
                        "ON \"Chapters\".\"Id\" = \"DataBase_Files\".\"Chapter\""
                        "WHERE \"Chapters\".\"Id\" NOTNULL "
                        "GROUP BY \"DataBase_Files\".\"Chapter\""
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_Section =
                        "SELECT \"Sections\".\"Name\" AS 'Section',COUNT(*) AS 'Number'"
                        "FROM \"DataBase_Files\" JOIN \"Sections\""
                        "ON \"Sections\".\"Id\" = \"DataBase_Files\".\"Section\""
                        "WHERE \"Sections\".\"Id\" NOTNULL "
                        "GROUP BY \"DataBase_Files\".\"Section\""
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_ExerciseType =
                        "SELECT \"Exercise_Types\".\"Name\" AS 'Exercise type',COUNT(*) AS 'Number'"
                        "FROM \"DataBase_Files\" JOIN \"Exercise_Types\""
                        "ON \"Exercise_Types\".\"Id\" = \"DataBase_Files\".\"ExerciseType\""
                        "WHERE \"Exercise_Types\".\"Id\" NOTNULL "
                        "GROUP BY \"DataBase_Files\".\"ExerciseType\""
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_FileType =
                        "SELECT \"FileTypes\".\"FileType\" AS 'File type',COUNT(*) AS 'Number'"
                        "FROM \"DataBase_Files\" JOIN \"FileTypes\""
                        "ON \"FileTypes\".\"Id\" = \"DataBase_Files\".\"FileType\""
                        "GROUP BY \"DataBase_Files\".\"FileType\""
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::Section_List_contaning_Exercises =
                        "SELECT DISTINCT \"s\".\"Name\",\"s\".\"Id\" "
                                "FROM \"DataBase_Files\" \"df\" JOIN \"Sections\" \"s\" "
                                "ON \"s\".\"Id\" = \"df\".\"Section\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"df\".\"FileType\" = \"%2\" "
                                "ORDER BY \"s\".\"Name\";";

const QString SqlFunctions::GetName =
                                "SELECT \"Name\""
                                "FROM \"Fields\""
                                "WHERE \"Id\" = \"%1\";";

const QString SqlFunctions::FolderName =
                                "SELECT \"FolderName\""
                                "FROM \"FileTypes\""
                                "WHERE \"Id\" = \"%1\";";
const QString SqlFunctions::TheoryFiles =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('Def','Theor','Tab','Fig');";
const QString SqlFunctions::UpdateTableFiles =
                                "SELECT \"df\".\"Id\" AS 'Name',\"df\".\"Date\",\"df\".\"Path\" "
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\" "
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\" "
                                "WHERE \"df\".\"Field\" LIKE \"%%1%\" "
                                "AND (\"df\".\"Chapter\" LIKE \"%%2%\" OR \"df\".\"Chapter\" ISNULL) "
                                "AND \"df\".\"Section\" LIKE \"%%3%\" "
                                "AND (\"df\".\"ExerciseType\" LIKE \"%%4%\" OR \"df\".\"ExerciseType\" ISNULL) "
                                "AND \"ft\".\"Id\" = '%5'";
const QString SqlFunctions::TheoryFiles_Chapter =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('Def','Theor','Tab','Fig')"
                                "AND \"df\".\"Chapter\" = \"%2\";";
const QString SqlFunctions::TheoryFiles_Chapter_Section =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('Def','Theor','Tab','Fig')"
                                "AND \"df\".\"Chapter\" = \"%2\" AND df.Section = \"%3\";";
const QString SqlFunctions::ExerciseFiles =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('SectEx','SectSub','Method','Example');";
const QString SqlFunctions::ExerciseFiles_Chapters =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('SectEx','SectSub','Method','Example')"
                                "AND \"df\".\"Chapter\" = \"%2\";";
const QString SqlFunctions::ExerciseFiles_Chapters_Sections =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('SectEx','SectSub','Method','Example')"
                                "AND \"df\".\"Chapter\" = \"%2\" AND \"df\".\"Section\" = \"%3\";";
const QString SqlFunctions::ExerciseFiles_Chapters_Sections_ExSubType =
                                "SELECT \"ft\".\"FileType\" AS 'File type', \"df\".\"Id\" AS 'Name',\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"ft\".\"Id\" IN ('SectEx','SectSub','Method','Example')"
                                "AND \"df\".\"Chapter\" = \"%2\" AND \"df\".\"Section\" = \"%3\" AND \"df\".\"ExerciseType\" = \"%5\""
                                "AND \"df\".\"FileType\" = \"%4\";";

//const QString SqlFunctions::GetSubject_Types =
//                                "SELECT \"Name\",\"Id\""
//                                "FROM \"Subject_Types\";";
const QString SqlFunctions::GetCombFiles =
                                "SELECT DISTINCT \"Id\" AS 'Name',\"Path\""
                                "FROM \"Database_Files\" "
                                "WHERE \"FileType\" = \"%1\" AND section IN (%2);";
const QString SqlFunctions::GetTheoryFiles =
                                "SELECT \"df\".\"Id\", \"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\" AND \"df\".\"FileType\" = \"%2\""
                                "AND \"df\".\"Chapter\" = \"%3\" AND \"df\".\"Section\" = \"%4\"";
const QString SqlFunctions::AddTheoryFilesToEditor =
                                "SELECT \"ft\".\"FileType\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE df.Path = \"%1\"";
const QString SqlFunctions::AddExerciseFilesToEditor =
                                "SELECT \"df\".\"Id\",\"df\".\"Path\""
                                "FROM \"Database_Files\" \"df\" JOIN \"FileTypes\" \"ft\""
                                "ON \"df\".\"FileType\" = \"ft\".\"Id\""
                                "WHERE \"df\".\"Field\" = \"%1\""
                                "AND \"df\".\"Chapter\" = \"%2\" "
                                "AND \"df\".\"Section\" = \"%3\" "
                                "AND \"df\".\"ExerciseType\" = \"%4\""
                                "AND \"df\".\"FileType\" = \"%5\";";
const QString SqlFunctions::GetFileTypeIdfromCheckBox =
                                "SELECT \"Id\""
                                "FROM \"FileTypes\""
                                "WHERE \"FolderName\" = \"%1\"";
const QString SqlFunctions::GetSectionsCombFiles =
                                "SELECT DISTINCT replace(group_concat(\"s\".\"Name\"),',','-'),group_concat(\"s\".\"Id\")"
                                "FROM \"Database_Files\" \"df\" JOIN \"Sections\" \"s\""
                                "ON \"df\".\"Section\" = \"s\".\"Id\""
                                "WHERE \"FileType\" = \"%2\" AND \"df\".\"Field\" = \"%1\""
                                "GROUP BY \"df\".\"Id\";";
const QString SqlFunctions::AddCombFilesToList =
                                "SELECT DISTINCT \"Id\",\"Path\""
                                "FROM \"Database_Files\""
                                "WHERE Section IN (%1) AND FileType = \"%2\""
                                "GROUP BY \"Id\""
                                "HAVING COUNT(\"Id\") = %3";
const QString SqlFunctions::ShowSolvedAndUnSolvedExercises  =
                                "SELECT \"df\".\"Id\" AS 'Id',%1,\"df\".\"Solved\" AS 'Solved',\"Path\",\"Preamble\",\"BuildCommand\""
                                "FROM \"Database_Files\" \"df\" JOIN \"Exercise_Types\" \"et\""
                                "ON \"df\".\"ExerciseType\" = \"et\".\"Id\""
                                "WHERE \"df\".\"FileType\" = \"%2\" AND \"df\".\"Section\" = \"%3\""
                                "ORDER BY \"df\".\"Id\";";

const QString SqlFunctions::ShowSolvedAndUnSolvedExercisesFiltered  =
                                "SELECT \"df\".\"Id\" AS 'Id',%1,\"df\".\"Solved\" AS 'Solved',\"Path\",\"Preamble\",\"BuildCommand\""
                                "FROM \"Database_Files\" \"df\" JOIN \"Exercise_Types\" \"et\""
                                "ON \"df\".\"ExerciseType\" = \"et\".\"Id\""
                                "WHERE \"df\".\"FileType\" = \"%2\" AND \"df\".\"Section\" = \"%3\""
                                "AND \"df\".\"Id\" LIKE \"%%4%\""
                                "AND \"et\".\"Name\" LIKE \"%%5%\""
                                "AND \"Solved\" LIKE \"%%6%\""
                                "ORDER BY \"df\".\"Id\";";
//FilteredComb

const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercises =
                                "SELECT \"df\".\"Id\" AS 'Id',replace(group_concat(\"s\".\"Name\"),',','-') "
                                "AS 'Sections',\"df\".\"Solved\" AS 'Solved',\"Path\",\"Preamble\",\"BuildCommand\""
                                "FROM \"Database_Files\" \"df\" JOIN \"Sections\" \"s\""
                                "ON \"df\".\"Section\" = \"s\".\"Id\""
                                "WHERE \"df\".\"FileType\" = \"%1\""
                                "GROUP BY \"df\".\"Id\""
                                "HAVING replace(group_concat(\"s\".\"Name\"),',','-') = \"%2\""
                                "ORDER BY \"df\".\"Id\";";

const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercisesFiltered =
                                "SELECT \"df\".\"Id\" AS 'Id',replace(group_concat(\"s\".\"Name\"),',','-') "
                                "AS sec,\"df\".\"Solved\" AS 'Solved',\"Path\",\"Preamble\",\"BuildCommand\""
                                "FROM \"Database_Files\" \"df\" JOIN \"Sections\" \"s\""
                                "ON \"df\".\"Section\" = \"s\".\"Id\""
                                "WHERE \"df\".\"FileType\" = \"%1\""
                                "AND \"df\".\"Id\" LIKE \"%%3%\""
                                "AND \"Solved\" LIKE \"%%5%\""
                                "GROUP BY \"df\".\"Id\""
                                "HAVING (sec = \"%2\""
                                "AND sec LIKE \"%%4%\")"
                                "ORDER BY \"df\".\"Id\";";

const QString SqlFunctions::EmptyTable = "SELECT * FROM \'Database_Files\" WHERE 1=0;";

const QString SqlFunctions::UpdateSolution =
                                "UPDATE \"Database_Files\""
                                "SET \"Solved\" = 'YES'"
                                "WHERE \"Id\" = '%1'";

const QString SqlFunctions::SelestExerciseRow = "SELECT *, replace(group_concat(\"Section\"),',','-') AS 'Section' FROM \"Database_Files\" WHERE \"Id\" = '%1';";

const QString SqlFunctions::GetDocumentTypes = "SELECT \"Name\" FROM \"Document_Types\" ORDER BY rowid;";

QString SqlFunctions::ShowAllDatabaseFiles;

QString SqlFunctions::FilesTable_UpdateQuery;

const QString SqlFunctions::GetPreamble = "SELECT \"Value\""
                                "FROM \"Initial_Settings\""
                                "WHERE \"Setting\" = 'Current_Preamble'";

const QString SqlFunctions::GetPreamble_Content = "SELECT \"Preamble_Content\" FROM \"Preambles\" WHERE \"Id\" = \"%1\";";

const QString SqlFunctions::GetBibliographyFields =
                                "SELECT \"Bibliographic_Field\",\"Name\" "
                                "FROM \"Bibliographic_Fields_per_Database\" \"bf\" "
                                "JOIN \"Bibliography\" \"b\" "
                                "ON \"bf\".\"Bibliographic_Field\" = \"b\".\"Id\" "
                                "WHERE \"bf\".\"Database\" = \"%1\";";
const QString SqlFunctions::GetColumnNames = "SELECT \"name\" FROM pragma_table_info('%1') JOIN (SELECT COUNT(*) FROM %1);";

const QString SqlFunctions::GetLatexCommands = "UPDATE Initial_Settings SET Value = :pdf WHERE Setting = 'Pdflatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :latex WHERE Setting = 'Latex_Command'; "
                                "UPDATE Initial_Settings SET Value = :xelatex WHERE Setting = 'Xelatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :lua WHERE Setting = 'Lualatex_Command'; "
                                "UPDATE Initial_Settings SET Value = :python WHERE Setting = 'Pythontex_Command';";

const QString SqlFunctions::ShowFilesInADocument =
                            "SELECT DISTINCT \"df\".\"Id\", \"%2\" AS \"Database source\" ,\"ft\".\"FileType\","
                            "replace(group_concat(DISTINCT \"s\".\"Name\"),',','-') AS 'Section',"
                            "\"se\".\"Exercise_Name\","
                            "\"Path\",\"Solved\",\"df\".\"FileType\",\"df\".\"Bibliography\" "
                            "FROM \"Database_Files\" \"df\" "
                            "JOIN \"FileTypes\" \"ft\" ON \"ft\".\"Id\" = \"df\".\"FileType\" "
                            "JOIN \"Sections\" \"s\" ON \"s\".\"Id\" = \"df\".\"Section\""
                            "LEFT JOIN \"Sections_Exercises\" \"se\" ON \"se\".\"Exercise_Id\" = \"df\".\"ExerciseType\" "
                            "WHERE \"df\".\"Id\" IN %1"
                            "GROUP BY \"df\".\"Id\" "/*
                            "ORDER BY \"df\".rowid "*/;

const QString SqlFunctions::ShowFilesInADocument_DifferentDatabase =
                            "SELECT DISTINCT \"df\".\"Id\", \"%2\" AS \"Database source\",\"ft\".\"FileType\","
                            "replace(group_concat(DISTINCT \"s\".\"Name\"),',','-') AS 'Section',"
                            "\"se\".\"Exercise_Name\","
                            "\"Path\",\"Solved\",\"df\".\"FileType\",\"df\".\"Bibliography\" "
                            "FROM \"%2\".\"Database_Files\" \"df\" "
                            "JOIN \"%2\".\"FileTypes\" \"ft\" ON \"ft\".\"Id\" = \"df\".\"FileType\" "
                            "JOIN \"%2\".\"Sections\" \"s\" ON \"s\".\"Id\" = \"df\".\"Section\""
                            "LEFT JOIN \"%2\".\"Sections_Exercises\" \"se\" ON \"se\".\"Exercise_Id\" = \"df\".\"ExerciseType\" "
                            "WHERE \"df\".\"Id\" IN %1"
                            "GROUP BY \"df\".\"Id\" ";
QString SqlFunctions::FilterDatabaseDocuments;

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
    QString query = "UPDATE ";
    query += Table;
    query += QString(" SET %2 = CASE %1 ").arg(column_1,column_2);
    for(int i=0;i<settings.count();i++){
        query += QString(" WHEN '%1' THEN '%2' ").arg(settings.at(i),values.at(i));
    }
    query += " ELSE ";
    query += column_2;
    query += " END ";
    query += " WHERE ";
    query += column_1;
    query += " IN (\'"+settings.join("\',\'")+"\')";
    return query;
}

