#include "filecommands.h"

FileCommands::FileCommands()
{

}

DBFileInfo::DBFileInfo()
{

}

DBFileInfo::DBFileInfo(QString fileId,QSqlDatabase database){
    Id=fileId;
    FileType<<SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFileTypeInfo.arg(fileId),database);
    Field<<SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFieldInfo.arg(fileId),database);
    Chapters=setRecordList(SqlFunctions::GetChapterInfo.arg(fileId),database);
    Sections=setRecordList(SqlFunctions::GetSectionInfo.arg(fileId),database);
    SubSections=setRecordList(SqlFunctions::GetSubsectionInfo.arg(fileId),database);
    QStringList meta = SqlFunctions::Get_Record_From_Query(QString(SqlFunctions::GetFileInfo.arg(fileId)),database);
    Difficulty = meta.at(0).toInt();
    Path = meta[1];
    Date = QDateTime::fromString(meta[2],"dd/M/yyyy hh:mm");
    Solved = (meta[3] == "1") ? true : false;
    Bibliography = meta[4];
    Content = meta[5];
    Preamble = meta[6];
    BuildCommand = meta[7];
    Description = meta[8];
    Database = database;
    DatabaseName = DataTex::GlobalFilesDatabaseListNames[QFileInfo(database.databaseName()).baseName()];
    Tags = SqlFunctions::Get_StringList_From_Query(QString("SELECT Tag_Id FROM "
                                                           "Tags_per_File WHERE File_Id = '%1'").arg(fileId),database);
    Solutions = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Id FROM "
                                                                "Solutions_per_File WHERE File_Id = '%1'").arg(fileId),database);
}

DBFileInfo::DBFileInfo(const DBFileInfo &fileinfo)
{
    Id= fileinfo.Id;
    FileType = fileinfo.FileType;
    Field = fileinfo.Field;
    Chapters = fileinfo.Chapters;
    Sections = fileinfo.Sections;
    SubSections = fileinfo.SubSections;
    Difficulty = fileinfo.Difficulty;
    Path = fileinfo.Path;
    Date = fileinfo.Date;
    Solved = fileinfo.Solved;
    Bibliography = fileinfo.Bibliography;
    Content = fileinfo.Content;
    Preamble = fileinfo.Preamble;
    BuildCommand = fileinfo.BuildCommand;
    Description = fileinfo.Description;
    Database = fileinfo.Database;
    DatabaseName = fileinfo.DatabaseName;
    Tags = fileinfo.Tags;
    Solutions = fileinfo.Solutions;
}

QStringList DBFileInfo::getInfoList(QList<QStringList> List,int i){
    QStringList list;
    foreach (QStringList item,List) {
        if(!list.contains(item.at(i))){
            list.append(item.at(i));
        }
    }
    return list;
}

QStringList DBFileInfo::getChaptersNames(){return getInfoList(Chapters,1);}
QStringList DBFileInfo::getChaptersIds(){return getInfoList(Chapters,0);}
QStringList DBFileInfo::getSectionsNames(){return getInfoList(Sections,1);}
QStringList DBFileInfo::getSectionsIds(){return getInfoList(Sections,0);}
QStringList DBFileInfo::getSubSectionsNames(){return getInfoList(SubSections,1);}
QStringList DBFileInfo::getSubSectionsIds(){return getInfoList(SubSections,0);}

QList<QStringList> DBFileInfo::setRecordList(QString querytext,QSqlDatabase database){
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


void DBFileInfo::WriteDBFile(const DBFileInfo *file)
{
    QString path = file->Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
    text += "Database=("+QFileInfo(file->Database.databaseName()).baseName()+","+file->DatabaseName+",files_database)\n";
    text += "DatabasePath="+file->Database.databaseName()+"\n\n";
    text += "[File Data]\n";
    text += "Id="+file->Id+"\n";
    text += "FileType=("+file->FileType.join(",")+")\n";
    text += "Fields=("+file->Field.join(",")+")\n";
    QStringList c;
    foreach (QStringList chapter,file->Chapters) {
        c.append("("+chapter.join(",")+")");
    }
    text += "Chapters="+c.join("|")+"\n";
    QStringList s;
    foreach (QStringList section,file->Sections) {
        s.append("("+section.join(",")+")");
    }
    text += "Sections="+s.join("|")+"\n";
    QStringList ss;
    foreach (QStringList subsection,file->SubSections) {
        ss.append("("+subsection.join(",")+")");
    }
    text += "SubSections="+ss.join("|")+"\n";
    text += "Difficulty="+QString::number(file->Difficulty)+"\n";
    text += "Path="+file->Path+"\n";
    text += "Date="+file->Date.toString("dd/M/yyyy hh:mm")+"\n";
    text += "Solved_Prooved="+QString::number(file->Solved)+"\n";
    text += "Preamble="+file->Preamble+"\n";
    text += "BuildCommand="+file->BuildCommand+"\n";
    text += "FileDescription="+file->Description+"\n";
    text += "Custom_Tags=("+file->Tags.join(",")+")\n";
    text += "Solutions_Proofs=("+file->Solutions.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += file->Bibliography+"\n";
    text += "[File Content]\n";
    text += file->Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
}

void DBFileInfo::WriteDBFile(const DBFileInfo file)
{
    QString path = file.Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
    text += "Database=("+QFileInfo(file.Database.databaseName()).baseName()+","+file.DatabaseName+",files_database)\n";
    text += "DatabasePath="+file.Database.databaseName()+"\n\n";
    text += "[File Data]\n";
    text += "Id="+file.Id+"\n";
    text += "FileType=("+file.FileType.join(",")+")\n";
    text += "Fields=("+file.Field.join(",")+")\n";
    QStringList c;
    foreach (QStringList chapter,file.Chapters) {
        c.append("("+chapter.join(",")+")");
    }
    text += "Chapters="+c.join("|")+"\n";
    QStringList s;
    foreach (QStringList section,file.Sections) {
        s.append("("+section.join(",")+")");
    }
    text += "Sections="+s.join("|")+"\n";
    QStringList ss;
    foreach (QStringList subsection,file.SubSections) {
        ss.append("("+subsection.join(",")+")");
    }
    text += "SubSections="+ss.join("|")+"\n";
    text += "Difficulty="+QString::number(file.Difficulty)+"\n";
    text += "Path="+file.Path+"\n";
    text += "Date="+file.Date.toString("dd/M/yyyy hh:mm")+"\n";
    text += "Solved_Prooved="+QString::number(file.Solved)+"\n";
    text += "Preamble="+file.Preamble+"\n";
    text += "BuildCommand="+file.BuildCommand+"\n";
    text += "FileDescription="+file.Description+"\n";
    text += "Custom_Tags=("+file.Tags.join(",")+")\n";
    text += "Solutions_Proofs=("+file.Solutions.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += file.Bibliography+"\n";
    text += "[File Content]\n";
    text += file.Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
}

DBFileInfo * DBFileInfo::ReadDBFile(QString filePath)
{
    DBFileInfo *fileInfo = new DBFileInfo;
    QFile newDBFile(filePath);
    newDBFile.open(QIODevice::ReadOnly);
    QTextStream data( &newDBFile );
    bool isBibliography = false;
    QString bibliography;
    bool isContent = false;
    QString content;
    while (!data.atEnd()) {
        QString Line = data.readLine();
        if(Line.startsWith("Database=")){
            Line = Line.remove("Database=(");
            Line = Line.chopped(1);
            QStringList list = Line.split(",");
            fileInfo->DatabaseName = list[1];
            if(list[2]=="files_database"){
                fileInfo->Database = DataTex::GlobalFilesDatabaseList[list[0]];
            }
            else{
                fileInfo->Database = DataTex::GlobalDocsDatabaseList[list[0]];
            }
        }
        if(Line.startsWith("Id=")){
            Line = Line.remove("Id=");
            fileInfo->Id = Line;
        }
        if(Line.startsWith("FileType=(")){
            Line = Line.remove("FileType=(");
            Line = Line.chopped(1);
            fileInfo->FileType = Line.split(",");
        }
        if(Line.startsWith("Fields=(")){
            Line = Line.remove("Fields=(");
            Line = Line.chopped(1);
            //                    QStringList list = Line.split(")|(");
            fileInfo->Field = Line.split(",");
        }
        if(Line.startsWith("Chapters=")){
            Line = Line.remove("Chapters=");
            QStringList list = Line.split("|");
            foreach (QString chapter, list) {
                chapter.remove(0,1);
                chapter = chapter.chopped(1);
                fileInfo->Chapters.append(chapter.split(","));
            }
        }
        if(Line.startsWith("Sections=")){
            Line = Line.remove("Sections=");
            QStringList list = Line.split("|");
            foreach (QString section, list) {
                section.remove(0,1);
                section = section.chopped(1);
                fileInfo->Sections.append(section.split(","));
            }
        }
        if(Line.startsWith("SubSections=")){
            Line = Line.remove("SubSections=");
            QStringList list = Line.split("|");
            foreach (QString subsection, list){
                if(subsection.size()>2){
                    subsection.remove(0,1);
                    subsection = subsection.chopped(1);
                    fileInfo->SubSections.append(subsection.split(","));
                }
            }
        }
        if(Line.startsWith("Difficulty=")){
            Line = Line.remove("Difficulty=");
            fileInfo->Difficulty = Line.toInt();
        }
        if(Line.startsWith("Path=")){
            Line = Line.remove("Path=");
            fileInfo->Path = Line;
        }
        if(Line.startsWith("Date=")){
            Line = Line.remove("Date=");
            fileInfo->Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
        }
        if(Line.startsWith("Solved_Prooved=")){
            Line = Line.remove("Solved_Prooved=");
            fileInfo->Solved = Line.toInt();
        }
        if(Line.startsWith("Preamble=")){
            Line = Line.remove("Preamble=");
            fileInfo->Preamble = Line;
        }
        if(Line.startsWith("BuildCommand=")){
            Line = Line.remove("BuildCommand=");
            fileInfo->BuildCommand = Line;
        }
        if(Line.startsWith("FileDescription=")){
            Line = Line.remove("FileDescription=");
            fileInfo->Description = Line;
        }
        if(Line.startsWith("Custom_Tags=(")){
            Line = Line.remove("Custom_Tags=(");
            Line = Line.chopped(1);
            fileInfo->Tags = Line.split(",");
        }
        if(Line.startsWith("Solutions_Proofs=(")){
            Line = Line.remove("Solutions_Proofs=(");
            Line = Line.chopped(1);
            fileInfo->Solutions = Line.split(",");
        }

        if(Line.startsWith("[Bibliography]")){
            isBibliography = true;
        }
        if(isBibliography){
            bibliography += Line+"\n";
        }
        if(Line.startsWith("[File Content]")){
            isBibliography = false;
            bibliography.remove("[Bibliography]");
            fileInfo->Bibliography = bibliography.remove("[File Content]");
            isContent = true;
        }
        if(isContent){
            content += Line+"\n";
        }
        fileInfo->Content = content.remove("[File Content]");
    }
    newDBFile.close();
    qDebug()<<fileInfo->Id<<"\n"<<fileInfo->FileType<<"\n"
             <<fileInfo->Chapters<<"\n"<<fileInfo->Bibliography<<"\n"
             <<fileInfo->Content;
    return fileInfo;
}


DBDocInfo::DBDocInfo()
{

}

DBDocInfo::DBDocInfo(QString docId,QSqlDatabase database){
    Id=docId;
    QStringList meta = SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM Documents WHERE Id = '%1'").arg(docId),database);
    Title = meta[1];
    DocType = meta[2];
    BasicFolder = meta[3];
    SubFolder<<meta[4]<<BasicFolder;
    SubsubFolder<<meta[5]<<SubFolder;
    Path = meta[6];
    Date = QDateTime::fromString(meta[7],"dd/M/yyyy hh:mm");
    Content = meta[8];
    Preamble = meta[9];
    BuildCommand = meta[10];
    NeedsUpdate = (meta[11] == "1") ? true : false;
    Bibliography = meta[12];
    Description = meta[13];
    SolutionDocument = meta[14];
    Database = database;
    DatabaseName = DataTex::GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()];
    Tags = QStringList();
    FilesIncluded = QStringList();
    BibEntries = QStringList();
    misc = QStringList();
}

DBDocInfo::DBDocInfo(const DBDocInfo &docinfo){
    Id= docinfo.Id;
    Title = docinfo.Title;
    DocType = docinfo.DocType;
    BasicFolder = docinfo.BasicFolder;
    SubFolder = docinfo.SubFolder;
    SubsubFolder = docinfo.SubsubFolder;
    Path = docinfo.Path;
    Date = docinfo.Date;
    Content = docinfo.Content;
    Preamble = docinfo.Preamble;
    BuildCommand = docinfo.BuildCommand;
    NeedsUpdate = docinfo.NeedsUpdate;
    Bibliography = docinfo.Bibliography;
    Description = docinfo.Description;
    SolutionDocument = docinfo.SolutionDocument;
    Tags = docinfo.Tags;
    FilesIncluded = docinfo.FilesIncluded;
    BibEntries = docinfo.BibEntries;
    misc = docinfo.misc;
    //-------------------------------
    Database = docinfo.Database;
    DatabaseName = docinfo.DatabaseName;
}

void DBDocInfo::WriteDBFile(const DBDocInfo *document)
{
    QString path = document->Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
    text += "Database=("+QFileInfo(document->Database.databaseName()).baseName()+","+document->DatabaseName+",doc_database)\n";
    text += "DatabasePath="+document->Database.databaseName()+"\n\n";
    text += "[File Data]\n";
    text += "Id="+document->Id+"\n";
    text += "Title="+document->Title+"\n";
    text += "DocType="+document->DocType+"\n";
    text += "BasicFolder="+document->BasicFolder+"\n";
    text += "SubFolder=("+document->SubFolder.join(",")+")\n";
    text += "SubsubFolder=("+document->SubsubFolder.join(",")+")\n";
    text += "Path="+document->Path+"\n";
    text += "Date="+document->Date.toString("dd/M/yyyy hh:mm")+"\n";
    text += "Preamble="+document->Preamble+"\n";
    text += "BuildCommand="+document->BuildCommand+"\n";
    text += "NeedsUpdate="+QString::number(document->NeedsUpdate)+"\n";
    text += "Description="+document->Description+"\n";
    text += "SolutionDocument="+document->SolutionDocument+"\n";
    text += "Custom_Tags=("+document->Tags.join(",")+")\n";
    text += "FilesIncluded=("+document->FilesIncluded.join(",")+")\n";
    text += "BibEntries=("+document->BibEntries.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += document->Bibliography+"\n";
    text += "[Document Content]\n";
    text += document->Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
    qDebug()<<"dbf ok";
}

void DBDocInfo::WriteDBFile(const DBDocInfo document)
{
    QString path = document.Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
    text += "Database=("+QFileInfo(document.Database.databaseName()).baseName()+","+document.DatabaseName+",doc_database)\n";
    text += "DatabasePath="+document.Database.databaseName()+"\n\n";
    text += "[File Data]\n";
    text += "Id="+document.Id+"\n";
    text += "Title="+document.Title+"\n";
    text += "DocType="+document.DocType+"\n";
    text += "BasicFolder="+document.BasicFolder+"\n";
    text += "SubFolder=("+document.SubFolder.join(",")+")\n";
    text += "SubsubFolder=("+document.SubsubFolder.join(",")+")\n";
    text += "Path="+document.Path+"\n";
    text += "Date="+document.Date.toString("dd/M/yyyy hh:mm")+"\n";
    text += "Preamble="+document.Preamble+"\n";
    text += "BuildCommand="+document.BuildCommand+"\n";
    text += "NeedsUpdate="+QString::number(document.NeedsUpdate)+"\n";
    text += "Description="+document.Description+"\n";
    text += "SolutionDocument="+document.SolutionDocument+"\n";
    text += "Custom_Tags=("+document.Tags.join(",")+")\n";
    text += "FilesIncluded=("+document.FilesIncluded.join(",")+")\n";
    text += "BibEntries=("+document.BibEntries.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += document.Bibliography+"\n";
    text += "[Document Content]\n";
    text += document.Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
    qDebug()<<"dbf ok";
}

DBDocInfo * DBDocInfo::ReadDBFile(QString docPath)
{
    DBDocInfo *docInfo = new DBDocInfo;
    QFile newDBFile(docPath);
    newDBFile.open(QIODevice::ReadOnly);
    QTextStream data( &newDBFile );
    bool isBibliography = false;
    QString bibliography;
    bool isContent = false;
    QString content;
    while (!data.atEnd()) {
        QString Line = data.readLine();
        if(Line.startsWith("Database=")){
            Line = Line.remove("Database=(");
            Line = Line.chopped(1);
            QStringList list = Line.split(",");
            docInfo->DatabaseName = list[1];
            docInfo->Database = DataTex::GlobalDocsDatabaseList[list[0]];
        }
        if(Line.startsWith("Id=")){
            Line = Line.remove("Id=");
            docInfo->Id = Line;
        }
        if(Line.startsWith("Title=")){
            Line = Line.remove("Title=");
            docInfo->Title = Line;
        }
        if(Line.startsWith("DocType=")){
            Line = Line.remove("DocType=");
            docInfo->DocType = Line;
        }
        if(Line.startsWith("BasicFolder=")){
            Line = Line.remove("BasicFolder=");
            docInfo->BasicFolder = Line;
        }
        if(Line.startsWith("SubFolder=")){
            Line = Line.remove("SubFolder=(");
            Line = Line.chopped(1);
            docInfo->SubFolder = Line.split(",");

        }
        if(Line.startsWith("SubsubFolder=")){
            Line = Line.remove("SubsubFolder=(");
            Line = Line.chopped(1);
            docInfo->SubsubFolder = Line.split(",");

        }
        if(Line.startsWith("Path=")){
            Line = Line.remove("Path=");
            docInfo->Path = Line;
        }
        if(Line.startsWith("Date=")){
            Line = Line.remove("Date=");
            docInfo->Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
        }
        if(Line.startsWith("Preamble=")){
            Line = Line.remove("Preamble=");
            docInfo->Preamble = Line;
        }
        if(Line.startsWith("BuildCommand=")){
            Line = Line.remove("BuildCommand=");
            docInfo->BuildCommand = Line;
        }
        if(Line.startsWith("NeedsUpdate=")){
            Line = Line.remove("NeedsUpdate=");
            docInfo->NeedsUpdate = Line.toInt();
        }
        if(Line.startsWith("Description=")){
            Line = Line.remove("Description=");
            docInfo->Description = Line;
        }
        if(Line.startsWith("SolutionDocument=(")){
            Line = Line.remove("SolutionDocument=");
            docInfo->SolutionDocument = Line;
        }
        if(Line.startsWith("CustomTags=(")){
            Line = Line.remove("CustomTags=(");
            Line = Line.chopped(1);
            docInfo->Tags = Line.split(",");
        }
        if(Line.startsWith("FilesIncluded=(")){
            Line = Line.remove("FilesIncluded=(");
            Line = Line.chopped(1);
            docInfo->FilesIncluded = Line.split(",");
        }
        if(Line.startsWith("BibEntries=(")){
            Line = Line.remove("BibEntries=(");
            Line = Line.chopped(1);
            docInfo->BibEntries = Line.split(",");
        }
        if(Line.startsWith("[Bibliography]")){
            isBibliography = true;
        }
        if(isBibliography){
            bibliography += Line+"\n";
        }
        if(Line.startsWith("[File Content]")){
            isBibliography = false;
            bibliography.remove("[Bibliography]");
            docInfo->Bibliography = bibliography.remove("[File Content]");
            isContent = true;
        }
        if(isContent){
            content += Line+"\n";
        }
        docInfo->Content = content.remove("[File Content]");
    }
    newDBFile.close();
    qDebug()<<docInfo->Id<<"\n"<<docInfo->SubFolder<<"\n"
             <<docInfo->SubsubFolder<<"\n"<<docInfo->Bibliography<<"\n"
             <<docInfo->Content;
    return docInfo;
}


QString FileCommands::CreateTexFile(QString fullFilePath,bool addToPreamble,QString addStuffToPreamble)
{
    QString outputDir = QFileInfo(fullFilePath).absolutePath();
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QString realContent = QString();

    QString sheetFileContent = DataTex::CurrentPreamble_Content+ "\n";
    sheetFileContent += (addToPreamble) ? addStuffToPreamble : QString();
    sheetFileContent += "\n\\begin{document}\n";
    QFile Databasefile(fullFilePath);
    Databasefile.open(QIODevice::ReadOnly | QFile::Text);
    QTextStream exoStream(&Databasefile);
    exoStream.flush();
    realContent = exoStream.readAll();
    sheetFileContent += "\n"+realContent+"\n";
    sheetFileContent += "\n \\end{document}";

    QString sheetFile = outputDir + QDir::separator() + outputFile +"-preview.tex";
    QFile file(sheetFile);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);
    out.flush();
    out << sheetFileContent;
    file.close();
    return sheetFile;
}

void FileCommands::BuildDocument(QString CompileCommand,QString fullFilePath,QStringList args,QString ext)
{
    QString outputDir = QFileInfo(fullFilePath).absolutePath();
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QProcess compileProcess;

#ifndef Q_OS_WIN
    QStringList env = QProcess::systemEnvironment();
    int j = env.indexOf(QRegExp("^PATH=(.*)"));
    int limit = env.at(j).indexOf("=");
    QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
    value = "PATH=" + value + ":" + QFileInfo(CompileCommand).path() + ":";
    env.replace(j,value);
    compileProcess.setEnvironment(env);
#endif
    compileProcess.setWorkingDirectory(outputDir);
    QString newTexFile = /*QFileInfo(fullFilePath).absolutePath()+QDir::separator()+*/outputFile +"-preview"+ext;
    QString oldpdf = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +".pdf";
    if (QFileInfo::exists(oldpdf)){QFile(oldpdf).remove();}

    //#ifdef Q_OS_WIN
    //#else
    //#endif
    args << qPrintable(newTexFile);
    compileProcess.start(CompileCommand,args);
    compileProcess.waitForFinished(-1);
    QString errorOutput = QString(compileProcess.readAllStandardOutput());
}

void FileCommands::ClearOldFiles(QString fullFilePath)
{
    QStringList extensions;
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QString newTexFile = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +"-preview.tex";
    extensions << ".log" << ".aux" << ".tex" << "-old.pdf" << ".out"<<".run.xml"<<".bcf";
    QString trashFile;
    foreach (QString ext,extensions)
    {
        trashFile = QFileInfo(fullFilePath).path() + QDir::separator() + QFileInfo(fullFilePath).baseName() + "-preview" + ext;
        if (QFileInfo::exists(trashFile)) QFile(trashFile).remove();
    }
    QString PdfOutput = QFileInfo(fullFilePath).path() + QDir::separator()+ QFileInfo(fullFilePath).baseName()+".pdf";
    QString pdffile = newTexFile.replace(".tex",".pdf");
    QFile renamePdf(pdffile);
    renamePdf.rename(pdffile,PdfOutput);
}

//Προσωρινό
void FileCommands::ShowPdfInViewer(QString exoFile, QPdfViewer *view)
{
    if (exoFile.isEmpty()) {
        return;}
    QString pdfFile = "file://"+exoFile.replace(".tex",".pdf");
    if(QFileInfo::exists(QUrl(pdfFile).toLocalFile())){
        view->open(QUrl(pdfFile));
    }
    else{
        view->open(QUrl("file://"+DataTex::datatexpath+"No_Pdf.pdf"));
    }
}

QString FileCommands::NewFileText(QString fileName,QString FileContent)
{
    QString text;
    text = "%# Database File : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ Database source: "+QFileInfo(DataTex::CurrentDataBasePath).baseName()+"\n";
    text += FileContent+"\n";
    text += "%# End of file "+QFileInfo(fileName).baseName();
    return text;
}

QString FileCommands::NewFileText(QString fileName,QString FileContent,QSqlDatabase database)
{
    QString text;
    text = "%# Database File : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ Database source: "+QFileInfo(database.databaseName()).baseName()+"\n";
    text += FileContent+"\n";
    text += "%# End of file "+QFileInfo(fileName).baseName();
    return text;
}

QString FileCommands::ClearMetadataFromContent(QString Content)
{
    QStringList text;
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database File :")
            || line.startsWith("%@ Database source:")
            || line.startsWith("%# End of file")){
            continue;
        }
        text<<line;
    }
    Content=text.join("\n");
    return Content;
}

QString FileCommands::ClearDocumentContent(QString Content)
{
    QStringList text;
    QString cleanContent;// = Content;
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database Document :")
            || line.startsWith("%@ Document type:")
            || line.startsWith("%#--------------------------------------------------")){
            continue;
        }
        text<<line;
    }
    cleanContent=text.join("\n");
    cleanContent.remove("\\begin{document}");
    cleanContent.remove("\\end{document}");
    return cleanContent;
}

QString FileCommands::GetPreamble(QString Content)
{
    QString preamble;
    QTextStream PreStream(&Content);
    while(!PreStream.atEnd()){
        QString line = PreStream.readLine();
        if(line.contains("\\begin{document}")){
            break;
        }
        preamble += line+"\n";
    }
    return preamble;
}

QHash<QString,QString> FileCommands::GetSingleFileIdFromContent(QString Content)
{
    QHash<QString,QString> FileId;
    QString Id;
    QString Database;
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database File :")){
            Id = line.remove("%# Database File :").trimmed();
        }
        if(line.startsWith("%@ Database source:")){
            Database = line.remove("%@ Database source:").trimmed();
            break;
        }
        FileId.insert(Id,Database);
    }
    return FileId;
}

QHash<QString,QString> FileCommands::GetSingleFileIdFromFile(QString filePath)
{
    QHash<QString,QString> FileId;
    QString Id;
    QString Database;
    QFile File(filePath);
    File.open (QIODevice::ReadOnly);
    QTextStream Content(&File);
    while (!Content.atEnd()){
        QString line=Content.readLine();
        if(line.startsWith("%# Database File :")){
            Id = line.remove("%# Database File :").trimmed();
        }
        if(line.startsWith("%@ Database source:")){
            Database = line.remove("%@ Database source:").trimmed();
            break;
        }
    }
    FileId.insert(Id,Database);
//    qDebug()<<FileId;
    File.close();
    return FileId;
}

QList<QHash<QString,QString>> FileCommands::GetFileIdsFromContent(QString Content)
{
    QList<QHash<QString,QString>> List;
    QHash<QString,QString> FileId;
    QString Id;
    QString Database;
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database File :")){
            Id = line.remove("%# Database File :").trimmed();
        }
        if(line.startsWith("%@ Database source:")){
            Database = line.remove("%@ Database source:").trimmed();
        }
        FileId.insert(Id,Database);
        List.append(FileId);
    }
    return List;
}

QString FileCommands::NewFilePathAndId(DBFileInfo *info,bool needsSubSection)
{
    QString FieldId = info->Field[0];
    QString Fields = info->Field[1];
    QString Chapters = info->getChaptersNames().join("|");
    QString Sections = info->getSectionsNames().join("|");
    QString ChapterId = info->getChaptersIds().join("");
    QString SectionId = info->getSectionsIds().join("");
    QString ExTypeId = info->getSubSectionsIds().join("");
    QString Path = QFileInfo(DataTex::CurrentDataBasePath).absolutePath()+QDir::separator()+Fields+QDir::separator()+Chapters+QDir::separator()+Sections+QDir::separator()+info->FileType[2]+QDir::separator();
    QString prefix = SqlFunctions::Get_String_From_Query(QString("SELECT Prefix FROM DataBases WHERE FileName = '%1'").arg(QFileInfo(DataTex::CurrentDataBasePath).baseName()),DataTex::DataTeX_Settings);
    prefix = (!prefix.isEmpty() && !prefix.isNull()) ? prefix+"-" : QString() ;
    QString fileId = (needsSubSection) ? prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+ExTypeId+"-"+info->FileType[0]
                                       : prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+info->FileType[0];
    QStringList ExistingFiles = SqlFunctions::Get_StringList_From_Query(
        QString("SELECT Id FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(fileId),DataTex::CurrentTexFilesDataBase);
    int fileNumber = 1;
    while(ExistingFiles.contains(fileId+QString::number(fileNumber))){
        fileNumber++;
    }
    return Path+fileId+QString::number(fileNumber)+".tex";
}

QStringList FileCommands::GetDatabaseTypeFromDTexFile(QString file)
{
    QStringList data;
    QFile File(file);
    File.open(QIODevice::ReadOnly);
    QTextStream in( &File );
    while (!in.atEnd()) {
        QString Line = in.readLine();
        if(Line.startsWith("Database=")){
            Line = Line.remove("Database=(");
            Line = Line.chopped(1);
            data = Line.split(",");
        }
        if(Line.startsWith("DatabasePath=")){
            Line = Line.remove("DatabasePath=");
            data.append(Line.split(","));
        }
    }
    File.close();
    return data;
}

void FileCommands::AddNewFileToDatabase(DBFileInfo * fileInfo,QSqlDatabase database)
{
    QString Path = QFileInfo(fileInfo->Path).absolutePath();
    QDir dir(Path);
    if (!dir.exists()) dir.mkpath(".");

    QString Content;
    QTextStream contentline(&fileInfo->Content);
    QStringList Line;
    while(!contentline.atEnd()){
        Line.append(contentline.readLine());
    }
    Content=Line.join("\\qt_endl");

    // Write new entry to database
    QSqlQuery writeExercise(database);
    writeExercise.exec("INSERT INTO Database_Files (Id,FileType,Field,Difficulty,Path,Date,Solved_Prooved,"
                       "FileContent,Preamble,BuildCommand,FileDescription) "
                       "VALUES (\""+fileInfo->Id+"\",\""+fileInfo->FileType[0]+"\",\""+fileInfo->Field[0]+
                       "\",\""+QString::number(fileInfo->Difficulty)+"\",\""+fileInfo->Path+
                       "\",\""+fileInfo->Date.toString("dd/M/yyyy hh:mm")+
                       "\",\""+QString::number(fileInfo->Solved)+"\",\""+fileInfo->Content+"\",\""+fileInfo->Preamble+
                       "\",\""+fileInfo->BuildCommand+"\",\""+fileInfo->Description+"\")");
    QString fileName = fileInfo->Id;
    foreach(QString Chapter,fileInfo->getChaptersIds()){
            writeExercise.exec("INSERT INTO Chapters_per_File (File_Id,Chapter_Id) VALUES (\""+fileName+"\",\""+Chapter+"\")");
    }
    foreach(QString Section,fileInfo->getSectionsIds()){
            writeExercise.exec("INSERT INTO Sections_per_File (File_Id,Section_Id) VALUES (\""+fileName+"\",\""+Section+"\")");
    }
    foreach(QString SubSection,fileInfo->getSubSectionsIds()){
            writeExercise.exec("INSERT INTO ExerciseTypes_per_File (File_Id,ExerciseType_Id) VALUES (\""+fileName+"\",\""+SubSection+"\")");
    }

    // Create new tex file and write content
    QFile file(fileInfo->Path);
    file.open(QIODevice::ReadWrite);
    QTextStream writeContent(&file);
    writeContent.flush();
    writeContent << fileInfo->Content;
    file.close();

    // Write metadata to dtex and csv files
    CsvFunctions::WriteDataToCSV(fileInfo->Path,DataTex::CurrentTexFilesDataBase);
    DBFileInfo::WriteDBFile(fileInfo);

//    QSqlQuery insertTag(database);
//    tags = tagLine->GetTags();
//    foreach(QString tag,tags){
//        if(!tag.isEmpty()){
//            insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
//            insertTag.exec("INSERT OR IGNORE INTO Tags_per_File (Tag_Id,File_Id) VALUES (\""+tag+"\",\""+fileName+"\")");
//        }
//    }
}

void FileCommands::UpdateFileInfo(DBFileInfo * fileInfo,QSqlDatabase database)
{
    QString fileName = fileInfo->Id;
    QSqlQuery writeExercise(database);
    QString Path = QFileInfo(fileInfo->Path).absolutePath();
    QDir dir(Path);
    if (!dir.exists()) dir.mkpath(".");
    QString query ="UPDATE Database_Files SET "
                    "Path=\""+fileInfo->Path+"\","
                    "Field='"+fileInfo->Field[0]+"',"
                    "FileDescription=\""+fileInfo->Description+"\","
                    "Preamble='"+fileInfo->Preamble+"',"
                    "BuildCommand='"+fileInfo->BuildCommand+"',"
                    "FileContent=\""+fileInfo->Content+"\","
                    "FileType='"+fileInfo->FileType[0]+"',"
                    "Id='"+fileName+"',"
                    "Difficulty='"+QString::number(fileInfo->Difficulty)+"',"
                    "Date=\""+fileInfo->Date.toString("dd/M/yyyy hh:mm")+"\" "
                    "WHERE Id='"+fileName+"';";//Παλιό Id
    writeExercise.exec("PRAGMA foreign_keys = ON");
    writeExercise.exec(query);
}

DBFileInfo * FileCommands::CreateSolutionData(DBFileInfo * fileInfo,QSqlDatabase database)
{
    QStringList info = SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM  FileTypes WHERE BelongsTo = '%1'").arg(fileInfo->FileType[0]),database);
    QString Path = fileInfo->Path;
    Path.replace("-"+fileInfo->FileType[0],"-"+info[0]);
    Path.replace(fileInfo->FileType[2],info[2]);
    Path.remove(".tex");
    int i = 1;
    while(QFileInfo::exists(Path+"-"+QString::number(i)+".tex")){
            i++;
    }
    fileInfo->Path = Path+"-"+QString::number(i)+".tex";
    fileInfo->Id = QFileInfo(fileInfo->Path).baseName();
    fileInfo->FileType = info;
    fileInfo->Date = QDateTime::currentDateTime();
    fileInfo->Solved = 0;
    fileInfo->Content = NewFileText(fileInfo->Path,"",database);
    return fileInfo;
}
