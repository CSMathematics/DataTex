#include "filecommands.h"
#include <QRegExp>

FileCommands::FileCommands()
{

}

DTXFile::DTXFile()
{

}

DTXFile::DTXFile(QString fileId,QSqlDatabase database){
    Id=fileId;
    FileType = DTXFileType(SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFileTypeInfo.arg(fileId),database));
    Field<<SqlFunctions::Get_Record_From_Query(SqlFunctions::GetFieldInfo.arg(fileId),database);
    Chapters=setRecordList(SqlFunctions::GetChapterInfo.arg(fileId),database);
    Sections=setRecordList(SqlFunctions::GetSectionInfo.arg(fileId),database);
    SubSections=setRecordList(SqlFunctions::GetSubsectionInfo.arg(fileId),database);
    QStringList meta = SqlFunctions::Get_Record_From_Query(QString(SqlFunctions::GetFileInfo.arg(fileId)),database);
    qDebug()<<SqlFunctions::GetFileInfo.arg(fileId);
    Difficulty = meta.at(0).toInt();
    Path = meta[1];
    Date = QDateTime::fromString(meta[2],"dd/M/yyyy hh:mm");
    Solved = (DTXSolutionState)meta.at(3).toInt();
    Bibliography = meta[4];
    Content = meta[5];
    Preamble.Id = meta[6];
    BuildCommand = meta[7];
    Description = meta[8];
    Database.Id = database.databaseName();
    DatabaseId = DataTex::GlobalDatabaseList.value(QFileInfo(database.databaseName()).baseName()).Description;
    Tags = SqlFunctions::Get_StringList_From_Query(QString("SELECT Tag_Id FROM "
                                                           "Tags_per_File WHERE File_Id = '%1'").arg(fileId),database);
    Solutions = SqlFunctions::Get_StringList_From_Query(QString("SELECT Solution_Id FROM "
                                                                "Solutions_per_File WHERE File_Id = '%1'").arg(fileId),database);
}

DTXFile::DTXFile(const DTXFile &fileinfo)
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
    DatabaseId = fileinfo.DatabaseId;
    Tags = fileinfo.Tags;
    Solutions = fileinfo.Solutions;
}

DTXFile::DTXFile(QString DTexPath)
{
    QFile newDBFile(DTexPath);
    newDBFile.open(QIODevice::ReadOnly);
    QTextStream data( &newDBFile );
    bool isBibliography = false;
    QString bibliography;
    bool isContent = false;
    QString content;
    while (!data.atEnd()) {
        QString Line = data.readLine();
        if(Line.startsWith("DBId=")){
            Database.Id = Line.remove("DBId=");
        }
        if(Line.startsWith("DBName=")){
            Database.Name = Line.remove("DBName=");
        }
        if(Line.startsWith("DBPath=")){
            Database.Path = Line.remove("DBPath=");
        }
        if(Line.startsWith("DBType=")){
            Database.Type = (DTXDatabaseType)Line.remove("DBType=").toInt();
        }
        if(Line.startsWith("Id=")){
            Line = Line.remove("Id=");
            Id = Line;
        }
        if(Line.startsWith("FileType=(")){
            Line = Line.remove("FileType=(");
            Line = Line.chopped(1);
            QStringList list = Line.split(",");
            FileType = DTXFileType(list);
        }
        if(Line.startsWith("Fields=(")){
            Line = Line.remove("Fields=(");
            Line = Line.chopped(1);
            //                    QStringList list = Line.split(")|(");
            Field = Line.split(",");
        }
        if(Line.startsWith("Chapters=")){
            Line = Line.remove("Chapters=");
            QStringList list = Line.split("|");
            for (QString chapter: list) {
                chapter.remove(0,1);
                chapter = chapter.chopped(1);
                Chapters.append(chapter.split(","));
            }
        }
        if(Line.startsWith("Sections=")){
            Line = Line.remove("Sections=");
            QStringList list = Line.split("|");
            for (QString section:list) {
                section.remove(0,1);
                section = section.chopped(1);
                Sections.append(section.split(","));
            }
        }
        if(Line.startsWith("SubSections=")){
            Line = Line.remove("SubSections=");
            QStringList list = Line.split("|");
            for (QString subsection: list){
                if(subsection.size()>2){
                    subsection.remove(0,1);
                    subsection = subsection.chopped(1);
                    SubSections.append(subsection.split(","));
                }
            }
        }
        if(Line.startsWith("Difficulty=")){
            Line = Line.remove("Difficulty=");
            Difficulty = Line.toInt();
        }
        if(Line.startsWith("Path=")){
            Line = Line.remove("Path=");
            Path = Line;
        }
        if(Line.startsWith("Date=")){
            Line = Line.remove("Date=");
            Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
        }
        if(Line.startsWith("Solved_Prooved=")){
            Line = Line.remove("Solved_Prooved=");
            Solved = (DTXSolutionState)Line.toInt();
        }
        if(Line.startsWith("Preamble=(")){
            Line = Line.remove("Preamble=(");
            Line = Line.chopped(1);
            QStringList list = Line.split(",");
            Preamble.Id = list.at(0);
            Preamble.DatabaseId = list.at(1);
            Preamble.DatabaseType = (DTXDatabaseType)QString(list.at(2)).toInt();

        }
        if(Line.startsWith("BuildCommand=")){
            Line = Line.remove("BuildCommand=");
            BuildCommand = Line;
        }
        if(Line.startsWith("FileDescription=")){
            Line = Line.remove("FileDescription=");
            Description = Line;
        }
        if(Line.startsWith("Custom_Tags=(")){
            Line = Line.remove("Custom_Tags=(");
            Line = Line.chopped(1);
            Tags = Line.split(",");
        }
        if(Line.startsWith("Solutions_Proofs=(")){
            Line = Line.remove("Solutions_Proofs=(");
            Line = Line.chopped(1);
            Solutions = Line.split(",");
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
            Bibliography = bibliography.remove("[File Content]");
            isContent = true;
        }
        if(isContent){
            content += Line+"\n";
        }
        Content = content.remove("[File Content]");
    }
    newDBFile.close();
}

QStringList DTXFile::getInfoList(QList<QStringList> List,int i){
    QStringList list;
    for (QStringList item:List) {
        if(!list.contains(item.at(i))){
            list.append(item.at(i));
        }
    }
    return list;
}

QStringList DTXFile::getChaptersNames(){return getInfoList(Chapters,1);}
QStringList DTXFile::getChaptersIds(){return getInfoList(Chapters,0);}
QStringList DTXFile::getSectionsNames(){return getInfoList(Sections,1);}
QStringList DTXFile::getSectionsIds(){return getInfoList(Sections,0);}
QStringList DTXFile::getSubSectionsNames(){return getInfoList(SubSections,1);}
QStringList DTXFile::getSubSectionsIds(){return getInfoList(SubSections,0);}

QList<QStringList> DTXFile::setRecordList(QString querytext,QSqlDatabase database){
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

void DTXFile::WriteDTexFile()
{
    QString path = Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
    text += "DBId="+Database.Id+"\n";
    text += "DBName"+Database.Name+"\n";
    text += "DBPath"+Database.Path+"\n";
    text += "DBType"+QString::number((int)Database.Type)+"\n\n";
    text += "[File Data]\n";
    text += "Id="+Id+"\n";
    text += "FileType=("+FileType.getListFromDTXFileType().join(",")+")\n";
    text += "Fields=("+Field.join(",")+")\n";
    QStringList c;
    for (QStringList chapter:Chapters) {
        c.append("("+chapter.join(",")+")");
    }
    text += "Chapters="+c.join("|")+"\n";
    QStringList s;
    for (QStringList section:Sections) {
        s.append("("+section.join(",")+")");
    }
    text += "Sections="+s.join("|")+"\n";
    QStringList ss;
    for (QStringList subsection:SubSections) {
        ss.append("("+subsection.join(",")+")");
    }
    text += "SubSections="+ss.join("|")+"\n";
    text += "Difficulty="+QString::number(Difficulty)+"\n";
    text += "Path="+Path+"\n";
    text += "Date="+Date.toString("dd/M/yyyy hh:mm")+"\n";
    text += "Solved_Prooved="+QString::number((int)Solved)+"\n";
    text += "Preamble=("+Preamble.Id+","+Preamble.DatabaseId+","+
            QString::number((int)Preamble.DatabaseType)+")\n";
    text += "BuildCommand="+BuildCommand+"\n";
    text += "FileDescription="+Description+"\n";
    text += "Custom_Tags=("+Tags.join(",")+")\n";
    text += "Solutions_Proofs=("+Solutions.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += Bibliography+"\n";
    text += "[File Content]\n";
    text += Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
}


DTXDocument::DTXDocument()
{

}

DTXDocument::DTXDocument(QString docId,QSqlDatabase database){
    Id=docId;
    QStringList meta = SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM Documents WHERE Id = '%1'").arg(docId),database);
    Title = meta[1];
    Type = meta[2];
    BasicFolder = meta[3];
    SubFolder = meta[4];
    SubsubFolder = meta[5];
    Path = meta[6];
    Date = QDateTime::fromString(meta[7],"dd/M/yyyy hh:mm");
    Content = meta[8];
//    Preamble = meta[9];
    BuildCommand = meta[10];
    NeedsUpdate = (meta[11] == "1") ? true : false;
    Bibliography = meta[12];
    Description = meta[13];
    SolutionDocument = meta[14];
//    Database = database;
    DatabaseName = DataTex::GlobalDatabaseList.value(QFileInfo(database.databaseName()).baseName()).Description;
    Tags = QStringList();
    FilesIncluded = QList<DTXIncludedFile>();
    BibEntries = QStringList();
    misc = QStringList();
}

DTXDocument::DTXDocument(const DTXDocument &docinfo){
    Id= docinfo.Id;
    Title = docinfo.Title;
    Type = docinfo.Type;
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

DTXDocument::DTXDocument(QString DTexPath)
{
    QFile newDBFile(DTexPath);
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
            DatabaseName = list[1];
            //            Database = DataTex::GlobalDatabaseList.value(list[0]).Database;
        }
        if(Line.startsWith("Id=")){
            Line = Line.remove("Id=");
            Id = Line;
        }
        if(Line.startsWith("Title=")){
            Line = Line.remove("Title=");
            Title = Line;
        }
        if(Line.startsWith("Type=")){
            Line = Line.remove("Type=");
            Type = Line;
        }
        if(Line.startsWith("BasicFolder=")){
            Line = Line.remove("BasicFolder=");
            BasicFolder = Line;
        }
        if(Line.startsWith("SubFolder=")){
            Line = Line.remove("SubFolder=");
            Line = Line.chopped(1);
            SubFolder = Line;

        }
        if(Line.startsWith("SubsubFolder=")){
            Line = Line.remove("SubsubFolder=");
            Line = Line.chopped(1);
            SubsubFolder = Line;

        }
        if(Line.startsWith("Path=")){
            Line = Line.remove("Path=");
            Path = Line;
        }
        if(Line.startsWith("Date=")){
            Line = Line.remove("Date=");
            Date = QDateTime::fromString(Line,"dd/M/yyyy hh:mm");
        }
        if(Line.startsWith("Preamble=")){
            Line = Line.remove("Preamble=");
//            Preamble = Line;
        }
        if(Line.startsWith("BuildCommand=")){
            Line = Line.remove("BuildCommand=");
            BuildCommand = Line;
        }
        if(Line.startsWith("NeedsUpdate=")){
            Line = Line.remove("NeedsUpdate=");
            NeedsUpdate = Line.toInt();
        }
        if(Line.startsWith("Description=")){
            Line = Line.remove("Description=");
            Description = Line;
        }
        if(Line.startsWith("SolutionDocument=(")){
            Line = Line.remove("SolutionDocument=");
            SolutionDocument = Line;
        }
        if(Line.startsWith("CustomTags=(")){
            Line = Line.remove("CustomTags=(");
            Line = Line.chopped(1);
            Tags = Line.split(",");
        }
        //        if(Line.startsWith("FilesIncluded=(")){
        //            Line = Line.remove("FilesIncluded=(");
        //            Line = Line.chopped(1);
        //            FilesIncluded = Line.split(",");
        //        }
        if(Line.startsWith("BibEntries=(")){
            Line = Line.remove("BibEntries=(");
            Line = Line.chopped(1);
            BibEntries = Line.split(",");
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
            Bibliography = bibliography.remove("[File Content]");
            isContent = true;
        }
        if(isContent){
            content += Line+"\n";
        }
        Content = content.remove("[File Content]");
    }
    newDBFile.close();
}

void DTXDocument::WriteDTexFile()
{
    QString path = Path;
    QString text = "DataTex V 1.0\n";
    text += "["+QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm")+"]\n\n";
    text += "[Database Data]\n";
//    text += "Database=("+QFileInfo(Database.databaseName()).baseName()+","+DatabaseName+",doc_database)\n";
//    text += "DatabasePath="+Database.databaseName()+"\n\n";
    text += "[File Data]\n";
    text += "Id="+Id+"\n";
    text += "Title="+Title+"\n";
    text += "Type="+Type+"\n";
    text += "BasicFolder="+BasicFolder+"\n";
    text += "SubFolder="+SubFolder+"\n";
    text += "SubsubFolder="+SubsubFolder+"\n";
    text += "Path="+Path+"\n";
    text += "Date="+Date.toString("dd/M/yyyy hh:mm")+"\n";
//    text += "Preamble="+Preamble+"\n";
    text += "BuildCommand="+BuildCommand+"\n";
    text += "NeedsUpdate="+QString::number(NeedsUpdate)+"\n";
    text += "Description="+Description+"\n";
    text += "SolutionDocument="+SolutionDocument+"\n";
    text += "Custom_Tags=("+Tags.join(",")+")\n";
    text += "BibEntries=("+BibEntries.join(",")+")\n\n";
    text += "[Bibliography]\n";
    text += Bibliography+"\n";
    text += "[Files Included]\n";
    int i=0;
    for(const DTXIncludedFile &file : FilesIncluded){
        i++;
        text += QString::number(i)+". : FileId="+file.Id+",Database="+file.DatabaseId+
                ",Database type="+QString::number(file.DatabaseType)+"\n";
    }
    text += "[Document Content]\n";
    text += Content;
    QFile newDBFile(path.replace(".tex",".dtex"));
    if (newDBFile.open(QFile::WriteOnly))
    {
        QTextStream data( &newDBFile );
        data << text;
    }
    newDBFile.close();
    qDebug()<<"dbf ok";
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
    qDebug()<<sheetFileContent;
    return sheetFile;
}

void FileCommands::BuildDocument(DTXBuildCommand Command,QString fullFilePath)
{
    QString outputDir = QFileInfo(fullFilePath).absolutePath();
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QProcess compileProcess;

#ifndef Q_OS_WIN
    QStringList env = QProcess::systemEnvironment();
    int j = 0;//env.indexOf(QRegExp("^PATH=(.*)"));
    int limit = env.at(j).indexOf("=");
    QString value = env.at(j).right(env.at(j).size()-limit-1).trimmed();
    value = "PATH=" + value + ":" + QFileInfo(Command.Path).path() + ":";
    env.replace(j,value);
    compileProcess.setEnvironment(env);
#endif
    compileProcess.setWorkingDirectory(outputDir);
    QString newTexFile = outputFile +"-preview"+Command.Extention;
    QString oldpdf = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +".pdf";
    if (QFileInfo::exists(oldpdf)){QFile(oldpdf).remove();}

    //#ifdef Q_OS_WIN
    //#else
    //#endif
//    Command.CommandArguments << qPrintable(newTexFile);
    compileProcess.start(Command.Path,QStringList()<<Command.CommandArguments<< qPrintable(newTexFile));
    compileProcess.waitForFinished(-1);
    QString errorOutput = QString(compileProcess.readAllStandardOutput());
    qDebug()<<Command.Path<<QStringList()<<Command.CommandArguments<< qPrintable(newTexFile);
}

void FileCommands::ClearOldFiles(QString fullFilePath)
{
    QStringList extensions;
    QString outputFile = QFileInfo(fullFilePath).baseName();
    QString newTexFile = QFileInfo(fullFilePath).absolutePath()+QDir::separator()+outputFile +"-preview.tex";
    extensions << ".log" << ".aux" << ".tex" << "-old.pdf" << ".out"<<".run.xml"<<".bcf";
    QString trashFile;
    for (QString ext:extensions)
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
    text = "%# File Id : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ FilesDB Id : "+QFileInfo(DataTex::CurrentFilesDataBase.Path).baseName()+"\n";
    text += FileContent+"\n";
    text += "%# End of file "+QFileInfo(fileName).baseName();
    return text;
}

QString FileCommands::NewFileText(QString fileName,QString FileContent,QSqlDatabase database)
{
    QString text;
    text = "%# File Id : "+QFileInfo(fileName).baseName()+"\n";
    text += "%@ FilesDB Id : "+QFileInfo(database.databaseName()).baseName()+"\n";
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
        if(line.startsWith("%# File Id :")
            || line.startsWith("%@ FilesDB Id :")
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
        if(line.startsWith("%# Document Id :")
            || line.startsWith("%@ DocumentsDB Id :")
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
        if(line.startsWith("%# File Id :")){
            Id = line.remove("%# File Id :").trimmed();
        }
        if(line.startsWith("%@ FilesDB Id :")){
            Database = line.remove("%@ FilesDB Id :").trimmed();
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
        if(line.startsWith("%# File Id :")){
            Id = line.remove("%# File Id :").trimmed();
        }
        if(line.startsWith("%@ FilesDB Id :")){
            Database = line.remove("%@ FilesDB Id :").trimmed();
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
        if(line.startsWith("%# File Id :")){
            Id = line.remove("%# File Id :").trimmed();
        }
        if(line.startsWith("%@ FilesDB Id :")){
            Database = line.remove("%@ FilesDB Id :").trimmed();
        }
        FileId.insert(Id,Database);
        List.append(FileId);
    }
    return List;
}

QString FileCommands::NewFilePathAndId(DTXFile *info,bool needsSubSection)
{
    QString FieldId = info->Field[0];
    QString Fields = info->Field[1];
    QString Chapters = info->getChaptersNames().join("|");
    QString Sections = info->getSectionsNames().join("|");
    QString ChapterId = info->getChaptersIds().join("");
    QString SectionId = info->getSectionsIds().join("");
    QString ExTypeId = info->getSubSectionsIds().join("");
    QString Path = QFileInfo(DataTex::CurrentFilesDataBase.Path).absolutePath()+QDir::separator()+Fields+QDir::separator()+Chapters+QDir::separator()+Sections+QDir::separator()+info->FileType.FolderName+QDir::separator();
    QString prefix;// = SqlFunctions::Get_String_From_Query(QString("SELECT Prefix FROM DataBases WHERE FileName = '%1'").arg(QFileInfo(DataTex::CurrentFilesDataBase.Path).baseName()),DataTex::DataTeX_Settings);
    prefix = (!prefix.isEmpty() && !prefix.isNull()) ? prefix+"-" : QString() ;
    QString fileId = (needsSubSection) ? prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+ExTypeId+"-"+info->FileType.Id
                                       : prefix+FieldId+"-"+ChapterId+"-"+SectionId+"-"+info->FileType.Id;
    QStringList ExistingFiles = SqlFunctions::Get_StringList_From_Query(
        QString("SELECT Id FROM Database_Files WHERE Id LIKE \"%%1%\"").arg(fileId),DataTex::CurrentFilesDataBase.Database);
    int fileNumber = 1;
    while(ExistingFiles.contains(fileId+QString::number(fileNumber))){
        fileNumber++;
    }
    return Path+fileId+QString::number(fileNumber)+".tex";
}

DTXDatabaseInfo FileCommands::GetDatabaseTypeFromDTexFile(QString file)
{
    DTXDatabaseInfo data;
    QFile File(file);
    File.open(QIODevice::ReadOnly);
    QTextStream in( &File );
    while (!in.atEnd()) {
        QString Line = in.readLine();
        if(Line.startsWith("DBId=")){
            data.Id = Line.remove("DBId=");
        }
        if(Line.startsWith("DBName=")){
            data.Name = Line.remove("DBName=");
        }
        if(Line.startsWith("DBPath=")){
            data.Path = Line.remove("DBPath=");
        }
        if(Line.startsWith("DBType=")){
            data.Type = (DTXDatabaseType)Line.remove("DBType=").toInt();
        }
    }
    File.close();
    return data;
}

void FileCommands::AddNewFileToDatabase(DTXFile * fileInfo,QSqlDatabase database)
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
                       "VALUES (\""+fileInfo->Id+"\",\""+fileInfo->FileType.Id+"\",\""+fileInfo->Field[0]+
                       "\",\""+QString::number(fileInfo->Difficulty)+"\",\""+fileInfo->Path+
                       "\",\""+fileInfo->Date.toString("dd/M/yyyy hh:mm")+
                       "\",\""+QString::number((int)fileInfo->Solved)+"\",\""+fileInfo->Content+"\",\""+fileInfo->Preamble.Id+
                       "\",\""+fileInfo->BuildCommand+"\",\""+fileInfo->Description+"\")");
    QString fileName = fileInfo->Id;
    for(QString Chapter:fileInfo->getChaptersIds()){
            writeExercise.exec("INSERT INTO Chapters_per_File (File_Id,Chapter_Id) VALUES (\""+fileName+"\",\""+Chapter+"\")");
    }
    for(QString Section:fileInfo->getSectionsIds()){
            writeExercise.exec("INSERT INTO Sections_per_File (File_Id,Section_Id) VALUES (\""+fileName+"\",\""+Section+"\")");
    }
    for(QString SubSection:fileInfo->getSubSectionsIds()){
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
    CsvFunctions::WriteDataToCSV(fileInfo->Path,DataTex::CurrentFilesDataBase.Database);
    fileInfo->WriteDTexFile();

//    QSqlQuery insertTag(database);
//    tags = tagLine->GetTags();
//    foreach(QString tag,tags){
//        if(!tag.isEmpty()){
//            insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
//            insertTag.exec("INSERT OR IGNORE INTO Tags_per_File (Tag_Id,File_Id) VALUES (\""+tag+"\",\""+fileName+"\")");
//        }
//    }
}

void FileCommands::UpdateFileInfo(DTXFile * fileInfo,QSqlDatabase database)
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
                    "Preamble='"+fileInfo->Preamble.Id+"',"
                    "BuildCommand='"+fileInfo->BuildCommand+"',"
                    "FileContent=\""+fileInfo->Content+"\","
                    "FileType='"+fileInfo->FileType.Id+"',"
                    "Id='"+fileName+"',"
                    "Difficulty='"+QString::number(fileInfo->Difficulty)+"',"
                    "Date=\""+fileInfo->Date.toString("dd/M/yyyy hh:mm")+"\" "
                    "WHERE Id='"+fileName+"';";//Παλιό Id
    writeExercise.exec("PRAGMA foreign_keys = ON");
    writeExercise.exec(query);
}

DTXFile * FileCommands::CreateSolutionData(DTXFile * fileInfo,QSqlDatabase database)
{
    QStringList info =/*= fileInfo->FileType.getListFromDTXFileType();*/
        SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM  FileTypes WHERE BelongsTo = '%1'").arg(fileInfo->FileType.Id),database);
    QString Path = fileInfo->Path;
    qDebug()<<info;
    Path.replace("-"+fileInfo->FileType.Id,"-"+info[0]);
    Path.replace(fileInfo->FileType.FolderName,info[2]);
    Path.remove(".tex");
    int i = 1;
    while(QFileInfo::exists(Path+"-"+QString::number(i)+".tex")){
            i++;
    }
    fileInfo->Path = Path+"-"+QString::number(i)+".tex";
    fileInfo->Id = QFileInfo(fileInfo->Path).baseName();
//    fileInfo->FileType = info;
    fileInfo->Date = QDateTime::currentDateTime();
    fileInfo->Solved = DTXSolutionState::Solution;
    fileInfo->Content = NewFileText(fileInfo->Path,"",database);
    return fileInfo;
}

QString FileCommands::CreateSolutionPath(DTXFile * fileInfo,QSqlDatabase database)
{
    QStringList info = SqlFunctions::Get_Record_From_Query(QString("SELECT * FROM  FileTypes WHERE BelongsTo = '%1'").arg(fileInfo->FileType.Id),database);
    QString Path = fileInfo->Path;
    Path.replace("-"+fileInfo->FileType.Id,"-"+info[0]);
    Path.replace(fileInfo->FileType.FolderName,info[2]);
    Path.remove(".tex");
    int i = 1;
    while(QFileInfo::exists(Path+"-"+QString::number(i)+".tex")){
            i++;
    }
    return Path+"-"+QString::number(i)+".tex";
}

QString FileCommands::PrintSolutionState(DTXSolutionState state)
{
    QString output;
    switch (state) {
    case DTXSolutionState::Solvable:
            output = QObject::tr("Solvable");
            break;
    case DTXSolutionState::NotSolvable:
            output = QObject::tr("Not solvable");
            break;
    case DTXSolutionState::Solution:
            output = QObject::tr("Solution");
            break;
    case DTXSolutionState::Solved:
            output = QObject::tr("Solved");
            break;
    case DTXSolutionState::UnSolved:
            output = QObject::tr("Unsolved");
            break;
    case DTXSolutionState::HasIncompleteSolutions:
            output = QObject::tr("Has incomplete solutions");
            break;
    case DTXSolutionState::SolutionComplete:
            output = QObject::tr("Solution is completed");
            break;
    case DTXSolutionState::SolutionIncomplete:
            output = QObject::tr("Solution is not completed");
            break;
    }
    return output;
}
