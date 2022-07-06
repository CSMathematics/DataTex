#include "csvfunctions.h"
#include <QSqlQueryModel>

#include "datatex.h"



CsvFunctions::CsvFunctions()
{

}

void CsvFunctions::WriteDataToCSV(QString filePath,QSqlDatabase database)
{
    QString text = getFile(QFileInfo(filePath).baseName(),database.databaseName());
    QSqlQueryModel * model = new QSqlQueryModel;
    model->setQuery(text,database);
//    qDebug()<<QString(text).arg(QFileInfo(filePath).baseName());
    QFile f(filePath.replace(".tex",".csv"));
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        QStringList strList;

        for( int c = 0; c < model->columnCount(); ++c )
        {
            strList.clear();
            for( int r = 0; r < model->rowCount(); ++r )
            {

                QVariant data = model->index(r, c).data();
                if(c==FileData::FileContent){
                    QString text = data.toString();
                    QTextStream contentline(&text);
                    QStringList Line;
                    while(!contentline.atEnd()){
                        Line.append(contentline.readLine());
                    }
                    text=Line.join("\\qt_endl");
                    strList << model->headerData(c,Qt::Horizontal).toString()+","+"\""+text+"\"";
                }
                else{
                    strList << model->headerData(c,Qt::Horizontal).toString()+","+"\""+data.toString()+"\"";
                }
            }
            data << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

void CsvFunctions::WriteDocDataToCSV(QString filePath, QSqlDatabase database)
{
    QString text = getDocument(QFileInfo(filePath).baseName(),database.databaseName());
    QSqlQueryModel * model = new QSqlQueryModel;
    model->setQuery(text,database);
    //    qDebug()<<QString(text).arg(QFileInfo(filePath).baseName());
    QFile f(filePath.replace(".tex",".csv"));
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        QStringList strList;

        for( int c = 0; c < model->columnCount(); ++c )
        {
            strList.clear();
            for( int r = 0; r < model->rowCount(); ++r )
            {

                QVariant data = model->index(r, c).data();
                if(c==DocData::DocContent || c==DocData::DocBibliography){
                    QString text = data.toString();
                    QTextStream contentline(&text);
                    QStringList Line;
                    while(!contentline.atEnd()){
                        Line.append(contentline.readLine());
                    }
                    text=Line.join("\\qt_endl");
                    strList << model->headerData(c,Qt::Horizontal).toString()+","+text;
                }
                else{
                    strList << model->headerData(c,Qt::Horizontal).toString()+","+data.toString();
                }
            }
            data << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

QHash<QString,QString> CsvFunctions::ReadCsv(QString fileName)
{
    QString csvFile = fileName;
    QHash<QString,QString> metadata;
    csvFile.replace(".tex",".csv");
    QFile CSV(csvFile);
    if(CSV.open (QIODevice::ReadOnly)){
        QTextStream Content(&CSV);
        int line = -1;
        QString value;
        while (!Content.atEnd()){
            line++;
            QString LineText=Content.readLine();
            QStringList fields = LineText.split(",");
            if(fields[0] == "FileContent"){
                value = fields.mid(1).join(",");
                value.replace("\\qt_endl","\n");
            }
            else{
                value = fields.mid(1).join(",");
            }
            if(value.startsWith("\"")){
                value.remove(0,1);
            }
            if(value.endsWith("\"")){
                value.chop(1);
            }
            metadata.insert(fields[0],value);
        }
        CSV.close();
    }
    return metadata;
}

QString CsvFunctions::getFile(QString file,QString databasepath)
{
    QString text;
    text = "SELECT df.Id ,ft.FileType AS FileType,f.Name AS Field, replace(group_concat(DISTINCT c.Name),',','|') AS Chapter, "
                   "replace(group_concat(DISTINCT s.Name),',','|') AS Section,replace(group_concat(DISTINCT et.Name),',','|') AS ExerciseType,Difficulty,Path, "
                   "Date,Solved_Prooved,replace(group_concat(DISTINCT bpf.Bib_Id),',','|') AS Bibliography,FileContent,Preamble,BuildCommand,FileDescription ";
            if(DataTex::Optional_Metadata_Ids[QFileInfo(databasepath).baseName()].count()>0)
            {text += ","+DataTex::Optional_Metadata_Ids[QFileInfo(databasepath).baseName()].join(",");}
            text += ", replace(group_concat(DISTINCT t.Tag_Id),',','|') AS Custom_Tags";
            text +=" FROM Database_Files df JOIN FileTypes ft ON ft.Id = df.FileType JOIN Fields f ON f.Id = df.Field "
                   "JOIN Chapters_per_File cpf ON cpf.File_Id=df.Id "
                   "JOIN Chapters c ON c.Id = cpf.Chapter_Id "
                   "JOIN Sections_per_File spf  ON spf.File_Id = df.Id "
                   "JOIN Sections s ON s.Id = spf.Section_Id  "
                   "LEFT JOIN Sections_Exercises se ON se.Section_Id = s.Id "
                   "LEFT JOIN Bib_Entries_per_File bpf ON df.Id = bpf.File_Id "
                   "LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id "
                   "LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id "
                   "LEFT JOIN Tags_per_File t ON t.File_Id = df.Id "
                   "WHERE df.Id = '"+file+"' "
                   "GROUP BY df.Id "
                   "ORDER BY df.rowid";
            return text;
}

QString CsvFunctions::getDocument(QString file,QString databasepath)
{
    QString text;
    text = "SELECT d.Id,d.Document_Type,d.Basic_Folder,d.SubFolder,d.SubsubFolder,d.Path,d.Date,d.Content,d.Preamble, "
           "d.BuildCommand,d.NeedsUpdate,d.Bibliography,d.UseBibliography ";
    if(DataTex::Optional_DocMetadata_Ids[QFileInfo(databasepath).baseName()].count()>0)
    {text += ","+DataTex::Optional_DocMetadata_Ids[QFileInfo(databasepath).baseName()].join(",");}
    text += ",replace(group_concat(DISTINCT fd.File_Id || '<<' || fd.Files_Database_Source || '>>'),',','|') AS Files_in_Document,"
            "replace(group_concat(DISTINCT bd.Bib_Id),',','|') AS Bib_Entries,"
            " replace(group_concat(DISTINCT t.Tag_Id),',','|') AS Custom_Tags ";
    text += "FROM Documents d "
            "LEFT JOIN BibEntries_per_Document bd ON bd.Document_Id = d.Id "
            "LEFT JOIN Files_per_Document fd ON fd.Document_Id = d.Id "
            "LEFT JOIN Tags_per_Document t ON t.Document_Id = d.Id "
            "WHERE d.Id = '"+file+"' "
            "GROUP BY d.Id "
            "ORDER BY d.rowid";
    return text;
}
