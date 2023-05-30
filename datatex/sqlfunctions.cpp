#include "sqlfunctions.h"


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
                        "LEFT JOIN Exercise_Types et ON et.Id = se.Exercise_Id "
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
                        "SELECT Id,Name "
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
                        "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                        "LEFT JOIN Exercise_Types e ON e.Id = epf.ExerciseType_Id "
                        "WHERE e.Id <> '-' "
                        "GROUP BY e.Name "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountFiles_by_FileType =
                        "SELECT FileTypes.FileType AS 'File type',COUNT(*) AS 'Number' "
                        "FROM DataBase_Files JOIN FileTypes "
                        "ON FileTypes.Id = DataBase_Files.FileType "
                        "GROUP BY DataBase_Files.FileType "
                        "ORDER BY 2 DESC;";

const QString SqlFunctions::CountDocs_by_DocType =
                        "SELECT Document_Type,COUNT(Id) FROM Documents "
                        "GROUP BY Document_Type "
                        "ORDER BY 2 DESC";

const QString SqlFunctions::CountDocs_by_BasicFolder =
                        "SELECT Basic_Folder,COUNT(Id) FROM Documents "
                        "GROUP BY Basic_Folder "
                        "ORDER BY 2 DESC";

const QString SqlFunctions::CountDocs_by_SubFolder =
                        "SELECT SubFolder,COUNT(Id) FROM Documents "
                        "GROUP BY SubFolder "
                        "ORDER BY 2 DESC";

const QString SqlFunctions::CountDocs_by_SubsubFolder =
                        "SELECT SubsubFolder,COUNT(Id) FROM Documents "
                        "GROUP BY SubsubFolder "
                        "ORDER BY 2 DESC";

const QString SqlFunctions::CountBib_by_DocType =
                        "SELECT d.Name,COUNT(Citation_Key) FROM Bibliography b "
                        "JOIN DocumentTypes d ON d.Id = b.Document_Type "
                        "GROUP BY Document_Type "
                        "ORDER BY 2 DESC ";

const QString SqlFunctions::CountBib_by_Author =
                        "SELECT a.FullName, COUNT(BibEntry_Id) FROM Authors_per_BibEntry apb "
                        "JOIN Authors a ON a.FullName = apb.FullName "
                        "GROUP BY a.FullName "
                        "ORDER BY 2 DESC ";

const QString SqlFunctions::CountBib_by_Publisher =
                        "SELECT publisher,COUNT(Citation_Key) FROM Bibliography "
                        "GROUP BY publisher "
                        "ORDER BY 2 DESC ";

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
        "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
        "GROUP by df.Id "
        "HAVING df.Field REGEXP \"%1\" "
        "AND (cpf.Chapter_Id REGEXP \"%2\" OR cpf.Chapter_Id ISNULL) "
        "AND spf.Section_Id REGEXP \"%3\" "
        "AND (epf.ExerciseType_Id REGEXP \"%4\" OR epf.ExerciseType_Id ISNULL) "
        "AND ft.Id = '%5'";

//const QString SqlFunctions::GetSectionsCombFiles =
//                                "SELECT DISTINCT replace(group_concat(s.Name),',','|'),group_concat(s.Id) "
//                                "FROM Database_Files df "
//                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                                "JOIN (SELECT * FROM Sections s ORDER BY s.Id) s ON s.Id = spf.Section_Id "
//                                "WHERE FileType = \"%2\" AND df.Field = \"%1\" "
//                                "GROUP BY df.Id;";

//const QString SqlFunctions::ShowSolvedAndUnSolvedExercises  =
//                                "SELECT df.Id AS 'Id',%1,df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
//                                "FROM Database_Files df "
//                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                                "JOIN Sections s ON s.Id = spf.Section_Id "
//                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
//                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
//                                "WHERE df.FileType = \"%2\" AND spf.Section_Id = \"%3\" "
//                                "ORDER BY df.Id;";

//const QString SqlFunctions::ShowSolvedAndUnSolvedExercisesFiltered  =
//                                "SELECT df.Id AS 'Id',%1,df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
//                                "FROM Database_Files df "
//                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                                "JOIN Sections s ON s.Id = spf.Section_Id "
//                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
//                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
//                                "WHERE df.FileType = \"%2\" AND spf.Section_Id = \"%3\" "
//                                "AND df.Id LIKE \"%%4%\" "
//                                "AND et.Name LIKE \"%%5%\" "
//                                "AND Solved_Prooved LIKE \"%%6%\" "
//                                "ORDER BY df.Id;";

//const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercises =
//                                "SELECT df.Id AS 'Id',replace(group_concat(DISTINCT s.Name),',','|') AS sec, "
//                                "df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
//                                "FROM Database_Files df "
//                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                                "JOIN Sections s ON s.Id = spf.Section_Id "
//                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
//                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
//                                "WHERE df.FileType = \"%1\" "
//                                "GROUP BY df.Id "
//                                "HAVING %2 "
//                                "ORDER BY df.Id;";

//const QString SqlFunctions::ShowSolvedAndUnSolvedCombExercisesFiltered =
//                                "SELECT df.Id AS 'Id',replace(group_concat(DISTINCT s.Name),',','|') AS 'sec', "
//                                "df.Solved_Prooved AS 'Solved',Path,Preamble,BuildCommand "
//                                "FROM Database_Files df "
//                                "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                                "JOIN Sections s ON s.Id = spf.Section_Id "
//                                "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
//                                "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
//                                "WHERE df.FileType = \"%1\" "
//                                "AND df.Id LIKE \"%%3%\" "
//                                "AND Solved_Prooved LIKE \"%%5%\" "
//                                "GROUP BY df.Id "
//                                "HAVING (sec LIKE \"%%2%\" AND sec LIKE \"%%4%\") "
//                                "ORDER BY df.Id;";

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
//                            "SELECT DISTINCT df.Id, \"%2\" AS \"Database source\" ,ft.FileType, "
//                            "replace(group_concat(DISTINCT s.Name),',','|') AS 'Section', "
//                            "replace(group_concat(DISTINCT et.Name),',','|'),Path,Solved_Prooved,df.FileType, "
//                            "replace(group_concat(DISTINCT bpf.Bib_Id),',','|'),\"%3\" AS \"Database\" "
//                            "FROM Database_Files df "
//                            "JOIN FileTypes ft ON ft.Id = df.FileType "
//                            "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
//                            "JOIN Sections s ON s.Id = spf.Section_Id "
//                            "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
//                            "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
//                            "LEFT JOIN Sections_Exercises se ON se.Exercise_Id = et.Id "
//                            "LEFT JOIN Bib_Entries_per_File bpf ON bpf.File_Id = df.Id "
//                            "WHERE df.Id IN %1 "
//                            "GROUP BY df.Id ";
    "SELECT DISTINCT df.Id, \"%2\" AS \"Database source\",ft.FileType, "
    "replace(group_concat(DISTINCT s.Name),',','|') AS 'Section', "
    "replace(group_concat(DISTINCT et.Name),',','|'),Path,Solved_Prooved,df.FileType, "
    "replace(group_concat(DISTINCT bpf.Bib_Id),',','|'),\"%3\" AS \"Database\" "
    "FROM \"%3\".Database_Files df "
    "JOIN \"%3\".FileTypes ft ON ft.Id = df.FileType "
    "JOIN \"%3\".Sections_per_File spf ON spf.File_Id = df.Id "
    "JOIN \"%3\".Sections s ON s.Id = spf.Section_Id "
    "LEFT JOIN \"%3\".ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
    "LEFT JOIN \"%3\".Exercise_Types et ON et.Id = epf.ExerciseType_Id "
    "LEFT JOIN \"%3\".Sections_Exercises se ON se.Exercise_Id = et.Id "
    "LEFT JOIN \"%3\".Bib_Entries_per_File bpf ON bpf.File_Id = df.Id "
    "WHERE df.Id IN %1 "
    "GROUP BY df.Id ";


const QString SqlFunctions::ShowFilesInADocument_DifferentDatabase =
                            "SELECT DISTINCT df.Id, \"%2\" AS \"Database source\",ft.FileType, "
                            "replace(group_concat(DISTINCT s.Name),',','|') AS 'Section', "
                            "replace(group_concat(DISTINCT et.Name),',','|'),Path,Solved_Prooved,df.FileType, "
                            "replace(group_concat(DISTINCT bpf.Bib_Id),',','|'),\"%3\" AS \"Database\" "
                            "FROM \"%3\".Database_Files df "
                            "JOIN \"%3\".FileTypes ft ON ft.Id = df.FileType "
                            "JOIN \"%3\".Sections_per_File spf ON spf.File_Id = df.Id "
                            "JOIN \"%3\".Sections s ON s.Id = spf.Section_Id "
                            "LEFT JOIN \"%3\".ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                            "LEFT JOIN \"%3\".Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                            "LEFT JOIN \"%3\".Sections_Exercises se ON se.Exercise_Id = et.Id "
                            "LEFT JOIN \"%3\".Bib_Entries_per_File bpf ON bpf.File_Id = df.Id "
                            "WHERE df.Id IN %1 "
                            "GROUP BY df.Id ";

QString SqlFunctions::ShowDocuments = "SELECT d.*,replace(group_concat(DISTINCT t.Tag_Id),',','|') AS Custom_Tags "
                                      "FROM Documents d "
                                      "LEFT JOIN Tags_per_Document t ON t.Document_Id = d.Id "
                                      "GROUP BY d.Id "
                                      "ORDER BY d.rowid";

QString SqlFunctions::FilterDatabaseDocuments;

QString SqlFunctions::ShowBibliographyEntries =
        "SELECT b.Citation_Key,d.Name,b.title,replace(group_concat(DISTINCT apb.FullName),',',' and ') AuthorConcat, "
        "replace(group_concat(DISTINCT epb.FullName),',',' and ') EditorConcat, "
        "replace(group_concat(DISTINCT tpb.FullName),',',' and ') TranslatorConcat, "
        " otherBibFields "
        " FROM Bibliography b "
        "LEFT JOIN Authors_per_BibEntry apb ON b.Citation_Key = apb.BibEntry_Id "
        "LEFT JOIN Editors_per_BibEntry epb ON b.Citation_Key = epb.BibEntry_Id "
        "LEFT JOIN Translators_per_BibEntry tpb ON b.Citation_Key = tpb.BibEntry_Id "
        "JOIN DocumentTypes d ON d.Id = b.Document_Type "
        "GROUP BY Citation_Key "
        "ORDER BY b.ROWID";

QString SqlFunctions::FilterBibliographyEntries;

QString SqlFunctions::GetFileTypeInfo = "SELECT f.* FROM FileTypes f "
                                        "LEFT JOIN Database_Files d ON f.Id = d.FileType "
                                        "WHERE d.Id = '%1'";

QString SqlFunctions::GetFieldInfo = "SELECT f.Id,f.Name FROM Fields f "
                                     "LEFT JOIN Database_Files d ON f.Id = d.Field "
                                     "WHERE d.Id = '%1'";

QString SqlFunctions::GetChapterInfo = "SELECT c.* FROM Chapters c "
                                       "LEFT JOIN Chapters_per_File cf on c.Id = cf.Chapter_Id "
                                       "WHERE cf.File_Id = '%1'";

QString SqlFunctions::GetSectionInfo = "SELECT s.Id,s.Name,s.Chapter FROM Sections s "
                                       "LEFT JOIN Sections_per_File sf on s.Id = sf.Section_Id "
                                       "WHERE sf.File_Id = '%1'";

QString SqlFunctions::GetSubsectionInfo = "SELECT et.*,sf.Section_Id "
                                          "FROM ExerciseTypes_per_File ef "
                                          "JOIN Exercise_Types et ON ef.ExerciseType_Id = et.Id "
                                          "JOIN Sections_per_File sf ON sf.File_Id = ef.File_Id "
                                          "WHERE ef.File_Id = '%1'";

QString SqlFunctions::GetFileInfo = "SELECT d.Difficulty,d.Path,d.Date,d.Solved_Prooved,d.Bibliography, "
                                    "d.FileContent,d.Preamble,d.BuildCommand,d.FileDescription "
                                    "FROM Database_Files d "
                                    "WHERE Id = '%1'";

QString SqlFunctions::GetSolutionState = "SELECT sum(Solved_Prooved) FROM Solutions_per_File sf "
                                         "JOIN Database_Files d ON d.Id = sf.Solution_Id "
                                         "WHERE File_Id = '%1'";

QString SqlFunctions::ShowAllFiles_SELECT;

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

QString SqlFunctions::Get_String_From_Query(QString queryString,QSqlDatabase database)
{
    QString text;
    QString queryText = QString(queryString);
    QSqlQuery query(database);
    query.exec(queryText);
    while(query.next()){
        if(!query.value(0).toString().isEmpty() && !query.value(0).toString().isNull()){
            text = query.value(0).toString();
        }
    }
    return text;
}

QStringList SqlFunctions::Get_Record_From_Query(QString queryString,QSqlDatabase database)
{
    QSqlQuery query(database);
    query.exec(queryString);
    QStringList recordlist;
    while(query.next()){
        QSqlRecord record = query.record();
        for(int i=0; i < record.count(); i++)
        {
            recordlist << record.value(i).toString();
        }
    }
    return recordlist;
}

QList<QStringList> SqlFunctions::GetRecordList(QString querytext,QSqlDatabase database)
{
    QList<QStringList> list;
    QSqlQuery query(database);
    query.exec(querytext);
    while(query.next()){
        QStringList s;
        for(int i=0; i < query.record().count(); i++)
        {
            s << query.record().value(i).toString();
        }
        list.append(s);
    }
    return list;
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

void SqlFunctions::ShowAllFiles(QStringList fields)
{
    ShowAllFiles_SELECT = "SELECT df.Id ,ft.FileType "
            ",f.Name , replace(group_concat(DISTINCT c.Name),',','|') , "
            "replace(group_concat(DISTINCT s.Name),',','|') "
            ",replace(group_concat(DISTINCT et.Name),',','|') ,";
    QStringList Select;
    QStringList DataFields;
    for (int i=6;i<fields.count();i++) {
        Select.append("\""+fields.at(i)+"\"");
        DataFields.append("(\"df\".\""+fields.at(i)+"\" LIKE "+"\"%replace"+fields.at(i)+
                          "%\" OR \"df\".\""+fields.at(i)+"\" ISNULL)");
    }
    ShowAllFiles_SELECT += Select.join(",")+",df.FileType,ft.Solvable,"
                                "replace(group_concat(DISTINCT t.Tag_Id),',','|')  ";
    ShowAllFiles_SELECT += "FROM Database_Files df "
             "JOIN FileTypes ft ON ft.Id = df.FileType "
             "JOIN Fields f ON f.Id = df.Field "
             "JOIN Chapters_per_File cpf ON cpf.File_Id=df.Id "
             "JOIN Chapters c ON c.Id = cpf.Chapter_Id "
             "JOIN Sections_per_File spf ON spf.File_Id = df.Id "
             "JOIN Sections s ON s.Id = spf.Section_Id  "
             "LEFT JOIN Sections_Exercises se ON se.Section_Id = s.Id "
             "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
             "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
             "LEFT JOIN Tags_per_File t ON t.File_Id = df.Id ";
    SqlFunctions::FilesTable_UpdateQuery = SqlFunctions::ShowAllFiles_SELECT;
    SqlFunctions::ShowAllDatabaseFiles = ShowAllFiles_SELECT+"GROUP BY df.Id "
                                          "ORDER BY df.rowid;";
}

void SqlFunctions::FilterTable(QStringList fields,QStringList values,QStringList additionalFilters)
{
    SqlFunctions::FilesTable_UpdateQuery.clear();
    ShowAllFiles(fields);
    for(int i= 0;i<values.count();i++){
        if((!values.at(i).isEmpty() && !values.at(i).isNull() && values.at(i)!="") || !additionalFilters.isEmpty()){
            SqlFunctions::FilesTable_UpdateQuery += " WHERE ";
            break;
        }
    }
    QStringList DataFields = QStringList();
    if(!values.at(0).isEmpty() && !values.at(0).isNull()){
        DataFields.append(" df.Id LIKE \"%"+values.at(0)+"%\" ");
    }
    if(!values.at(1).isEmpty() && !values.at(1).isNull()){
        DataFields.append(" ft.FileType LIKE \"%"+values.at(1)+"%\" ");
    }
    if(!values.at(2).isEmpty() && !values.at(2).isNull()){
        DataFields.append(" f.Name LIKE \"%"+values.at(2)+"%\" ");
    }
    if(!values.at(3).isEmpty() && !values.at(3).isNull()){
        DataFields.append(" c.Name LIKE \"%"+values.at(3)+"%\" ");
    }
    if(!values.at(4).isEmpty() && !values.at(4).isNull()){
        DataFields.append(" s.Name LIKE \"%"+values.at(4)+"%\" ");
    }
    if(!values.at(5).isEmpty() && !values.at(5).isNull()){
        DataFields.append(" et.Name LIKE \"%"+values.at(5)+"%\" ");
    }

    for (int i=6;i<fields.count();i++) {
        if(!values.at(i).isEmpty() && !values.at(i).isNull())
            DataFields.append(" (\"df\".\""+fields.at(i)+"\" LIKE "+"\"%"+values.at(i)+"%\") ");
    }

    int columns = fields.count();
    if(!values.at(columns+2).isNull() && !values.at(columns+2).isEmpty()){
        QStringList tags = values.at(columns+2).split(",");
        DataFields.append(" t.Tag_Id IN (\""+tags.join("\",\"")+"\") ");
    }
    DataFields.append(additionalFilters);
//    DataFields.replaceInStrings("Solved_Prooved","CASE WHEN (Solved_Prooved = 1 AND Solvable >0) THEN \"solved\" WHEN (Solved_Prooved = 0 AND Solvable >0) THEN \"unsolved\" ELSE \"-\" END AS sl");
    SqlFunctions::FilesTable_UpdateQuery += DataFields.join(" AND ");
//    if(!additionalFilter.isEmpty()){
//        SqlFunctions::FilesTable_UpdateQuery += additionalFilter;
//    }
    SqlFunctions::FilesTable_UpdateQuery += " GROUP BY df.Id ORDER BY df.rowid;";
    qDebug()<<FilesTable_UpdateQuery;
}
