#ifndef FILECOMMANDS_H
#define FILECOMMANDS_H

#include <QtCore>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "qpdfviewer.h"
#include "datatex.h"
#include "clonedatabasefile.h"
#include "sqlfunctions.h"


//QT_BEGIN_NAMESPACE
//class DataTex;
//class SqlFunctions;
//QT_END_NAMESPACE

struct DBFileInfo {
    QString Id = QString();
    QStringList FileType = QStringList();
    QStringList Field = QStringList();
    QList<QStringList> Chapters;
    QList<QStringList> Sections;
    QList<QStringList> SubSections;
    int Difficulty = 0;
    QString Path = QString();
    QDateTime Date = QDateTime();
    bool Solved = false;
    QString Bibliography = QString();
    QString Content = QString();
    QString Preamble = QString();
    QString BuildCommand = QString();
    QString Description = QString();
    QStringList Tags = QStringList();
    QStringList Solutions = QStringList();
    QVariant misc = QVariant();
    //-------------------------------
    QSqlDatabase Database;
    QString DatabaseName = QString();

    QStringList getInfoList(QList<QStringList> List,int i);
//    {
//        QStringList list;
//        foreach (QStringList item,List) {
//            if(!list.contains(item.at(i))){
//                list.append(item.at(i));
//            }
//        }
//        return list;
//    }

    QStringList getChaptersNames();//{return getInfoList(Chapters,1);}
    QStringList getChaptersIds();//{return getInfoList(Chapters,0);}
    QStringList getSectionsNames();//{return getInfoList(Sections,1);}
    QStringList getSectionsIds();//{return getInfoList(Sections,0);}
    QStringList getSubSectionsNames();//{return getInfoList(SubSections,1);}
    QStringList getSubSectionsIds();//{return getInfoList(SubSections,0);}

    QList<QStringList> setRecordList(QString querytext,QSqlDatabase database);
//    {
//        QList<QStringList> list;
//        QSqlQuery query(database);
//        query.exec(querytext);
//        while(query.next()){
//            QStringList s;
//            for(int i=0; i < query.record().count(); i++)
//            {
//                s << query.record().value(i).toString();
//            }
//            list.append(s);
//        }
//        return list;
//    }

    DBFileInfo();//{}
    DBFileInfo(QString fileId,QSqlDatabase database);
//    {
//        Id=fileId;
//        FileType<<SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFileTypeInfo.arg(fileId),database);
//        Field<<SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFieldInfo.arg(fileId),database);
//        Chapters=setRecordList(SqlFunctions::GetChapterInfo.arg(fileId),database);
//        Sections=setRecordList(SqlFunctions::GetSectionInfo.arg(fileId),database);
//        SubSections=setRecordList(SqlFunctions::GetSubsectionInfo.arg(fileId),database);
//        QStringList meta = SqlFunctions::Get_Record_From_Query(QString(SqlFunctions::GetFileInfo.arg(fileId)),database);
//        Difficulty = meta.at(1).toInt();
//        Path = meta[1];
//        Date = QDateTime::fromString(meta[2],"dd/M/yyyy hh:mm");
//        Solved = (meta[3] == "1") ? true : false;
//        Bibliography = meta[4];
//        Content = meta[5];
//        Preamble = meta[6];
//        BuildCommand = meta[7];
//        Description = meta[8];
//        Database = database;
//        DatabaseName = DataTex::GlobalFilesDatabaseListNames[QFileInfo(database.databaseName()).baseName()];
//        Tags = QStringList();
//        Solutions = QStringList();
//    }

    DBFileInfo(const DBFileInfo &fileinfo);
//    {
//        Id= fileinfo.Id;
//        FileType = fileinfo.FileType;
//        Field = fileinfo.Field;
//        Chapters = fileinfo.Chapters;
//        Sections = fileinfo.Sections;
//        SubSections = fileinfo.SubSections;
//        Difficulty = fileinfo.Difficulty;
//        Path = fileinfo.Path;
//        Date = fileinfo.Date;
//        Solved = fileinfo.Solved;
//        Bibliography = fileinfo.Bibliography;
//        Content = fileinfo.Content;
//        Preamble = fileinfo.Preamble;
//        BuildCommand = fileinfo.BuildCommand;
//        Description = fileinfo.Description;
//        Database = fileinfo.Database;
//        DatabaseName = fileinfo.DatabaseName;
//        Tags = fileinfo.Tags;
//        Solutions = fileinfo.Solutions;
//    }

    inline bool operator==(const DBFileInfo &file2) const
    {
        bool isEqual;
        isEqual = Id == file2.Id && FileType == file2.FileType &&
                  Field == file2.Field && Chapters == file2.Chapters &&
                  Sections == file2.Sections && SubSections == file2.SubSections &&
                  Difficulty == file2.Difficulty && Path == file2.Path &&
                  Date == file2.Date && Solved == file2.Solved &&
                  Bibliography == file2.Bibliography && Content == file2.Content &&
                  Preamble == file2.Preamble && BuildCommand == file2.BuildCommand &&
                  Description == file2.Description && misc == file2.misc &&
                  DatabaseName == file2.DatabaseName && Tags == file2.Tags &&
                  Solutions == file2.Solutions;
        return isEqual;
    }

    static void WriteDBFile(const DBFileInfo *file);
//    {
//        QString path = file->Path;
//        QString text = "DataTex V 1.0\n";
//        text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
//        text += "[Database Data]\n";
//        text += "Database=("+QFileInfo(file->Database.databaseName()).baseName()+","+file->DatabaseName+",files_database)\n";
//        text += "DatabasePath="+file->Database.databaseName()+"\n\n";
//        text += "[File Data]\n";
//        text += "Id="+file->Id+"\n";
//        text += "FileType=("+file->FileType.join(",")+")\n";
//        text += "Fields=("+file->Field.join(",")+")\n";
//        QStringList c;
//        foreach (QStringList chapter,file->Chapters) {
//            c.append("("+chapter.join(",")+")");
//        }
//        text += "Chapters="+c.join("|")+"\n";
//        QStringList s;
//        foreach (QStringList section,file->Sections) {
//            s.append("("+section.join(",")+")");
//        }
//        text += "Sections="+s.join("|")+"\n";
//        QStringList ss;
//        foreach (QStringList subsection,file->SubSections) {
//            ss.append("("+subsection.join(",")+")");
//        }
//        text += "SubSections="+ss.join("|")+"\n";
//        text += "Difficulty="+QString::number(file->Difficulty)+"\n";
//        text += "Path="+file->Path+"\n";
//        text += "Date="+file->Date.toString("dd/M/yyyy hh:mm")+"\n";
//        text += "Solved_Prooved="+QString::number(file->Solved)+"\n";
//        text += "Preamble="+file->Preamble+"\n";
//        text += "BuildCommand="+file->BuildCommand+"\n";
//        text += "FileDescription="+file->Description+"\n";
//        text += "Custom_Tags=("+file->Tags.join(",")+")\n";
//        text += "Solutions_Proofs=("+file->Solutions.join(",")+")\n\n";
//        text += "[Bibliography]\n";
//        text += file->Bibliography+"\n";
//        text += "[File Content]\n";
//        text += file->Content;
//        QFile newDBFile(path.replace(".tex",".dtex"));
//        if (newDBFile.open(QFile::WriteOnly))
//        {
//            QTextStream data( &newDBFile );
//            data << text;
//        }
//        newDBFile.close();
//    }

    static void WriteDBFile(const DBFileInfo file);
//    {
//        QString path = file.Path;
//        QString text = "DataTex V 1.0\n";
//        text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
//        text += "[Database Data]\n";
//        text += "Database=("+QFileInfo(file.Database.databaseName()).baseName()+","+file.DatabaseName+",files_database)\n";
//        text += "DatabasePath="+file.Database.databaseName()+"\n\n";
//        text += "[File Data]\n";
//        text += "Id="+file.Id+"\n";
//        text += "FileType=("+file.FileType.join(",")+")\n";
//        text += "Fields=("+file.Field.join(",")+")\n";
//        QStringList c;
//        foreach (QStringList chapter,file.Chapters) {
//            c.append("("+chapter.join(",")+")");
//        }
//        text += "Chapters="+c.join("|")+"\n";
//        QStringList s;
//        foreach (QStringList section,file.Sections) {
//            s.append("("+section.join(",")+")");
//        }
//        text += "Sections="+s.join("|")+"\n";
//        QStringList ss;
//        foreach (QStringList subsection,file.SubSections) {
//            ss.append("("+subsection.join(",")+")");
//        }
//        text += "SubSections="+ss.join("|")+"\n";
//        text += "Difficulty="+QString::number(file.Difficulty)+"\n";
//        text += "Path="+file.Path+"\n";
//        text += "Date="+file.Date.toString("dd/M/yyyy hh:mm")+"\n";
//        text += "Solved_Prooved="+QString::number(file.Solved)+"\n";
//        text += "Preamble="+file.Preamble+"\n";
//        text += "BuildCommand="+file.BuildCommand+"\n";
//        text += "FileDescription="+file.Description+"\n";
//        text += "Custom_Tags=("+file.Tags.join(",")+")\n";
//        text += "Solutions_Proofs=("+file.Solutions.join(",")+")\n\n";
//        text += "[Bibliography]\n";
//        text += file.Bibliography+"\n";
//        text += "[File Content]\n";
//        text += file.Content;
//        QFile newDBFile(path.replace(".tex",".dtex"));
//        if (newDBFile.open(QFile::WriteOnly))
//        {
//            QTextStream data( &newDBFile );
//            data << text;
//        }
//        newDBFile.close();
//    }

    static DBFileInfo * ReadDBFile(QString filePath);
//    {
//        DBFileInfo *fileInfo = new DBFileInfo;
//        QFile newDBFile(filePath);
//        newDBFile.open(QIODevice::ReadOnly);
//        QTextStream data( &newDBFile );
//        bool isBibliography = false;
//        QString bibliography;
//        bool isContent = false;
//        QString content;
//        while (!data.atEnd()) {
//            QString Line = data.readLine();
//            if(Line.startsWith("Database=")){
//                Line = Line.remove("Database=(");
//                Line = Line.chopped(1);
//                QStringList list = Line.split(",");
//                fileInfo->DatabaseName = list[1];
//                if(list[2]=="files_database"){
//                    fileInfo->Database = DataTex::GlobalFilesDatabaseList[list[0]];
//                }
//                else{
//                    fileInfo->Database = DataTex::GlobalDocsDatabaseList[list[0]];
//                }
//            }
//            if(Line.startsWith("Id=")){
//                Line = Line.remove("Id=");
//                fileInfo->Id = Line;
//            }
//            if(Line.startsWith("FileType=(")){
//                Line = Line.remove("FileType=(");
//                Line = Line.chopped(1);
//                fileInfo->FileType = Line.split(",");
//            }
//            if(Line.startsWith("Fields=(")){
//                Line = Line.remove("Fields=(");
//                Line = Line.chopped(1);
//                //                    QStringList list = Line.split(")|(");
//                fileInfo->Field = Line.split(",");
//            }
//            if(Line.startsWith("Chapters=")){
//                Line = Line.remove("Chapters=");
//                QStringList list = Line.split("|");
//                foreach (QString chapter, list) {
//                    chapter.remove(0,1);
//                    chapter = chapter.chopped(1);
//                    fileInfo->Chapters.append(chapter.split(","));
//                }
//            }
//            if(Line.startsWith("Sections=")){
//                Line = Line.remove("Sections=");
//                QStringList list = Line.split("|");
//                foreach (QString section, list) {
//                    section.remove(0,1);
//                    section = section.chopped(1);
//                    fileInfo->Sections.append(section.split(","));
//                }
//            }
//            if(Line.startsWith("SubSections=")){
//                Line = Line.remove("SubSections=");
//                QStringList list = Line.split("|");
//                foreach (QString subsection, list){
//                    if(subsection.size()>2){
//                        subsection.remove(0,1);
//                        subsection = subsection.chopped(1);
//                        fileInfo->SubSections.append(subsection.split(","));
//                    }
//                }
//            }
//            if(Line.startsWith("Difficulty=")){
//                Line = Line.remove("Difficulty=");
//                fileInfo->Difficulty = Line.toInt();
//            }
//            if(Line.startsWith("Path=")){
//                Line = Line.remove("Path=");
//                fileInfo->Path = Line;
//            }
//            if(Line.startsWith("Date=")){
//                Line = Line.remove("Date=");
//                fileInfo->Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
//            }
//            if(Line.startsWith("Solved_Prooved=")){
//                Line = Line.remove("Solved_Prooved=");
//                fileInfo->Solved = Line.toInt();
//            }
//            if(Line.startsWith("Preamble=")){
//                Line = Line.remove("Preamble=");
//                fileInfo->Preamble = Line;
//            }
//            if(Line.startsWith("BuildCommand=")){
//                Line = Line.remove("BuildCommand=");
//                fileInfo->BuildCommand = Line;
//            }
//            if(Line.startsWith("FileDescription=")){
//                Line = Line.remove("FileDescription=");
//                fileInfo->Description = Line;
//            }
//            if(Line.startsWith("Custom_Tags=(")){
//                Line = Line.remove("Custom_Tags=(");
//                Line = Line.chopped(1);
//                fileInfo->Tags = Line.split(",");
//            }
//            if(Line.startsWith("Solutions_Proofs=(")){
//                Line = Line.remove("Solutions_Proofs=(");
//                Line = Line.chopped(1);
//                fileInfo->Solutions = Line.split(",");
//            }

//            if(Line.startsWith("[Bibliography]")){
//                isBibliography = true;
//            }
//            if(isBibliography){
//                bibliography += Line+"\n";
//            }
//            if(Line.startsWith("[File Content]")){
//                isBibliography = false;
//                bibliography.remove("[Bibliography]");
//                fileInfo->Bibliography = bibliography.remove("[File Content]");
//                isContent = true;
//            }
//            if(isContent){
//                content += Line+"\n";
//            }
//            fileInfo->Content = content.remove("[File Content]");
//        }
//        newDBFile.close();
//        qDebug()<<fileInfo->Id<<"\n"<<fileInfo->FileType<<"\n"
//                 <<fileInfo->Chapters<<"\n"<<fileInfo->Bibliography<<"\n"
//                 <<fileInfo->Content;
//        return fileInfo;
//    }
};
//Q_DECLARE_METATYPE(DBFileInfo)
Q_DECLARE_METATYPE(DBFileInfo*)

struct DBDocInfo {
    QString Id = QString();
    QString Title = QString();
    QString DocType = QString();
    QString BasicFolder = QString();
    QStringList SubFolder=  QStringList();
    QStringList SubsubFolder = QStringList();
    QString Path = QString();
    QDateTime Date = QDateTime();
    QString Content = QString();
    QString Preamble = QString();
    QString BuildCommand = QString();
    bool NeedsUpdate = false;
    QString Bibliography = QString();
    QString Description = QString();
    QString SolutionDocument = QString();
    QStringList Tags = QStringList();
    QStringList FilesIncluded = QStringList();
    QStringList BibEntries = QStringList();
    QVariant misc = QVariant();
    //-------------------------------
    QSqlDatabase Database;
    QString DatabaseName = QString();

//    QStringList getInfoList(QList<QStringList> List,int i){
//        QStringList list;
//        foreach (QStringList item,List) {
//            list.append(item.at(i));
//        }
//        return list;
//    }

    DBDocInfo();//{}
    DBDocInfo(QString docId,QSqlDatabase database);
//    {
//        Id=docId;
//        QStringList meta = SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM Documents WHERE Id = '%1'").arg(docId),database);
//        Title = meta[1];
//        DocType = meta[2];
//        BasicFolder = meta[3];
//        SubFolder<<meta[4]<<BasicFolder;
//        SubsubFolder<<meta[5]<<SubFolder;
//        Path = meta[6];
//        Date = QDateTime::fromString(meta[7],"dd/M/yyyy hh:mm");
//        Content = meta[8];
//        Preamble = meta[9];
//        BuildCommand = meta[10];
//        NeedsUpdate = (meta[11] == "1") ? true : false;
//        Bibliography = meta[12];
//        Description = meta[13];
//        SolutionDocument = meta[14];
//        Database = database;
//        DatabaseName = DataTex::GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()];
//        Tags = QStringList();
//        FilesIncluded = QStringList();
//        BibEntries = QStringList();
//        misc = QStringList();
//    }

    DBDocInfo(const DBDocInfo &docinfo);
//    {
//        Id= docinfo.Id;
//        Title = docinfo.Title;
//        DocType = docinfo.DocType;
//        BasicFolder = docinfo.BasicFolder;
//        SubFolder = docinfo.SubFolder;
//        SubsubFolder = docinfo.SubsubFolder;
//        Path = docinfo.Path;
//        Date = docinfo.Date;
//        Content = docinfo.Content;
//        Preamble = docinfo.Preamble;
//        BuildCommand = docinfo.BuildCommand;
//        NeedsUpdate = docinfo.NeedsUpdate;
//        Bibliography = docinfo.Bibliography;
//        Description = docinfo.Description;
//        SolutionDocument = docinfo.SolutionDocument;
//        Tags = docinfo.Tags;
//        FilesIncluded = docinfo.FilesIncluded;
//        BibEntries = docinfo.BibEntries;
//        misc = docinfo.misc;
//        //-------------------------------
//        Database = docinfo.Database;
//        DatabaseName = docinfo.DatabaseName;
//    }

    inline bool operator==(const DBDocInfo &file2) const
    {
        bool isEqual;
        isEqual = Id == file2.Id && Title == file2.Title &&
                  DocType == file2.DocType && BasicFolder == file2.BasicFolder &&
                  SubFolder == file2.SubFolder && SubsubFolder == file2.SubsubFolder &&
                  Path == file2.Path && Date == file2.Date &&
                  Content == file2.Content &&
                  Preamble == file2.Preamble && BuildCommand == file2.BuildCommand &&
                  NeedsUpdate == file2.NeedsUpdate && Bibliography == file2.Bibliography &&
                  Description == file2.Description && SolutionDocument == file2.SolutionDocument &&
                  Tags == file2.Tags && FilesIncluded == file2.FilesIncluded &&
                  BibEntries == file2.BibEntries && misc == file2.misc &&
                  DatabaseName == file2.DatabaseName;
        return isEqual;
    }

    static void WriteDBFile(const DBDocInfo *document);
//    {
//        QString path = document->Path;
//        QString text = "DataTex V 1.0\n";
//        text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
//        text += "[Database Data]\n";
//        text += "Database=("+QFileInfo(document->Database.databaseName()).baseName()+","+document->DatabaseName+",doc_database)\n";
//        text += "DatabasePath="+document->Database.databaseName()+"\n\n";
//        text += "[File Data]\n";
//        text += "Id="+document->Id+"\n";
//        text += "Title="+document->Title+"\n";
//        text += "DocType="+document->DocType+"\n";
//        text += "BasicFolder="+document->BasicFolder+"\n";
//        text += "SubFolder=("+document->SubFolder.join(",")+")\n";
//        text += "SubsubFolder=("+document->SubsubFolder.join(",")+")\n";
//        text += "Path="+document->Path+"\n";
//        text += "Date="+document->Date.toString("dd/M/yyyy hh:mm")+"\n";
//        text += "Preamble="+document->Preamble+"\n";
//        text += "BuildCommand="+document->BuildCommand+"\n";
//        text += "NeedsUpdate="+QString::number(document->NeedsUpdate)+"\n";
//        text += "Description="+document->Description+"\n";
//        text += "SolutionDocument="+document->SolutionDocument+"\n";
//        text += "Custom_Tags=("+document->Tags.join(",")+")\n";
//        text += "FilesIncluded=("+document->FilesIncluded.join(",")+")\n";
//        text += "BibEntries=("+document->BibEntries.join(",")+")\n\n";
//        text += "[Bibliography]\n";
//        text += document->Bibliography+"\n";
//        text += "[Document Content]\n";
//        text += document->Content;
//        QFile newDBFile(path.replace(".tex",".dtex"));
//        if (newDBFile.open(QFile::WriteOnly))
//        {
//            QTextStream data( &newDBFile );
//            data << text;
//        }
//        newDBFile.close();
//        qDebug()<<"dbf ok";
//    }

    static void WriteDBFile(const DBDocInfo document);
//    {
//        QString path = document.Path;
//        QString text = "DataTex V 1.0\n";
//        text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
//        text += "[Database Data]\n";
//        text += "Database=("+QFileInfo(document.Database.databaseName()).baseName()+","+document.DatabaseName+",doc_database)\n";
//        text += "DatabasePath="+document.Database.databaseName()+"\n\n";
//        text += "[File Data]\n";
//        text += "Id="+document.Id+"\n";
//        text += "Title="+document.Title+"\n";
//        text += "DocType="+document.DocType+"\n";
//        text += "BasicFolder="+document.BasicFolder+"\n";
//        text += "SubFolder=("+document.SubFolder.join(",")+")\n";
//        text += "SubsubFolder=("+document.SubsubFolder.join(",")+")\n";
//        text += "Path="+document.Path+"\n";
//        text += "Date="+document.Date.toString("dd/M/yyyy hh:mm")+"\n";
//        text += "Preamble="+document.Preamble+"\n";
//        text += "BuildCommand="+document.BuildCommand+"\n";
//        text += "NeedsUpdate="+QString::number(document.NeedsUpdate)+"\n";
//        text += "Description="+document.Description+"\n";
//        text += "SolutionDocument="+document.SolutionDocument+"\n";
//        text += "Custom_Tags=("+document.Tags.join(",")+")\n";
//        text += "FilesIncluded=("+document.FilesIncluded.join(",")+")\n";
//        text += "BibEntries=("+document.BibEntries.join(",")+")\n\n";
//        text += "[Bibliography]\n";
//        text += document.Bibliography+"\n";
//        text += "[Document Content]\n";
//        text += document.Content;
//        QFile newDBFile(path.replace(".tex",".dtex"));
//        if (newDBFile.open(QFile::WriteOnly))
//        {
//            QTextStream data( &newDBFile );
//            data << text;
//        }
//        newDBFile.close();
//        qDebug()<<"dbf ok";
//    }

    static DBDocInfo * ReadDBFile(QString docPath);
//    {
//        DBDocInfo *docInfo = new DBDocInfo;
//        QFile newDBFile(docPath);
//        newDBFile.open(QIODevice::ReadOnly);
//        QTextStream data( &newDBFile );
//        bool isBibliography = false;
//        QString bibliography;
//        bool isContent = false;
//        QString content;
//        while (!data.atEnd()) {
//            QString Line = data.readLine();
//            if(Line.startsWith("Database=")){
//                Line = Line.remove("Database=(");
//                Line = Line.chopped(1);
//                QStringList list = Line.split(",");
//                docInfo->DatabaseName = list[1];
//                docInfo->Database = DataTex::GlobalDocsDatabaseList[list[0]];
//            }
//            if(Line.startsWith("Id=")){
//                Line = Line.remove("Id=");
//                docInfo->Id = Line;
//            }
//            if(Line.startsWith("Title=")){
//                Line = Line.remove("Title=");
//                docInfo->Title = Line;
//            }
//            if(Line.startsWith("DocType=")){
//                Line = Line.remove("DocType=");
//                docInfo->DocType = Line;
//            }
//            if(Line.startsWith("BasicFolder=")){
//                Line = Line.remove("BasicFolder=");
//                docInfo->BasicFolder = Line;
//            }
//            if(Line.startsWith("SubFolder=")){
//                Line = Line.remove("SubFolder=(");
//                Line = Line.chopped(1);
//                docInfo->SubFolder = Line.split(",");

//            }
//            if(Line.startsWith("SubsubFolder=")){
//                Line = Line.remove("SubsubFolder=(");
//                Line = Line.chopped(1);
//                docInfo->SubsubFolder = Line.split(",");

//            }
//            if(Line.startsWith("Path=")){
//                Line = Line.remove("Path=");
//                docInfo->Path = Line;
//            }
//            if(Line.startsWith("Date=")){
//                Line = Line.remove("Date=");
//                docInfo->Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
//            }
//            if(Line.startsWith("Preamble=")){
//                Line = Line.remove("Preamble=");
//                docInfo->Preamble = Line;
//            }
//            if(Line.startsWith("BuildCommand=")){
//                Line = Line.remove("BuildCommand=");
//                docInfo->BuildCommand = Line;
//            }
//            if(Line.startsWith("NeedsUpdate=")){
//                Line = Line.remove("NeedsUpdate=");
//                docInfo->NeedsUpdate = Line.toInt();
//            }
//            if(Line.startsWith("Description=")){
//                Line = Line.remove("Description=");
//                docInfo->Description = Line;
//            }
//            if(Line.startsWith("SolutionDocument=(")){
//                Line = Line.remove("SolutionDocument=");
//                docInfo->SolutionDocument = Line;
//            }
//            if(Line.startsWith("CustomTags=(")){
//                Line = Line.remove("CustomTags=(");
//                Line = Line.chopped(1);
//                docInfo->Tags = Line.split(",");
//            }
//            if(Line.startsWith("FilesIncluded=(")){
//                Line = Line.remove("FilesIncluded=(");
//                Line = Line.chopped(1);
//                docInfo->FilesIncluded = Line.split(",");
//            }
//            if(Line.startsWith("BibEntries=(")){
//                Line = Line.remove("BibEntries=(");
//                Line = Line.chopped(1);
//                docInfo->BibEntries = Line.split(",");
//            }
//            if(Line.startsWith("[Bibliography]")){
//                isBibliography = true;
//            }
//            if(isBibliography){
//                bibliography += Line+"\n";
//            }
//            if(Line.startsWith("[File Content]")){
//                isBibliography = false;
//                bibliography.remove("[Bibliography]");
//                docInfo->Bibliography = bibliography.remove("[File Content]");
//                isContent = true;
//            }
//            if(isContent){
//                content += Line+"\n";
//            }
//            docInfo->Content = content.remove("[File Content]");
//        }
//        newDBFile.close();
//        qDebug()<<docInfo->Id<<"\n"<<docInfo->SubFolder<<"\n"
//                 <<docInfo->SubsubFolder<<"\n"<<docInfo->Bibliography<<"\n"
//                 <<docInfo->Content;
//        return docInfo;
//    }
};
//Q_DECLARE_METATYPE(DBFileInfo)
Q_DECLARE_METATYPE(DBDocInfo*)

class FileCommands
{
public:
    FileCommands();
    static QString CreateTexFile(QString fullFilePath, bool addToPreamble, QString addStuffToPreamble);
    static void BuildDocument(QString CompileCommand, QString fullFilePath, QStringList args, QString ext);
    static void ClearOldFiles(QString fullFilePath);
    static void ShowPdfInViewer(QString File, QPdfViewer * view);
//    static void ShowPdfInViewer(QString exoFile, PdfViewer * view);
//    static void ShowPdfInViewer(QString exoFile, PdfViewerWidget * view);
    static QString NewFileText(QString fileName, QString FileContent);
    static QString NewFileText(QString fileName, QString FileContent,QSqlDatabase database);
    static QString NewFilePathAndId(DBFileInfo *info, bool needsSubSection);
    static QString ClearMetadataFromContent(QString Content);
    static QString ClearDocumentContent(QString Content);
    static QString GetPreamble(QString Content);
    static QHash<QString,QString> GetSingleFileIdFromContent(QString Content);
    static QHash<QString,QString> GetSingleFileIdFromFile(QString filePath);
    static QList<QHash<QString, QString>> GetFileIdsFromContent(QString Content);
    static QStringList GetDatabaseTypeFromDTexFile(QString file);
    static void AddNewFileToDatabase(DBFileInfo * fileInfo,QSqlDatabase database);
    static void UpdateFileInfo(DBFileInfo * fileInfo,QSqlDatabase database);
    static DBFileInfo *CreateSolutionData(DBFileInfo * fileInfo,QSqlDatabase database);
};

#endif // FILECOMMANDS_H
