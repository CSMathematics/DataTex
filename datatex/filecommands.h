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
#include "databasecreator.h"
#include "csvfunctions.h"

struct DTXPreamble;

enum struct CompileEngine {
    LaTeX           = 1<<0,
    PdfLaTeX        = 1<<1,
    XeLaTeX         = 1<<2,
    LuaLaTeX        = 1<<3,
    PythonTex       = 1<<4,
    Asymptote       = 1<<5,
    Metapost        = 1<<6,
    KnitR           = 1<<7,
    Lilypond        = 1<<8,
    Latexmk         = 1<<9,
    BibTeX          = 1<<10,
    Biber           = 1<<11,
    MakeIndex       = 1<<12,
    Xindy           = 1<<13,
    Xindex          = 1<<14,
    MakeGlossaries  = 1<<15,
    DVIPS           = 1<<16,
    DVI2PDF         = 1<<17,
    DVI2PNG         = 1<<18,
    PS2PDF          = 1<<19
};

Q_DECLARE_FLAGS (CompileEngines, CompileEngine)
Q_DECLARE_OPERATORS_FOR_FLAGS (CompileEngines)
Q_DECLARE_METATYPE(CompileEngine)


struct DTXIncludedFile
{
    QString Id;
    QString DatabaseId;
    DTXDatabaseType DatabaseType;

    inline bool operator==(const DTXIncludedFile &file2) const
    {
        return (Id==file2.Id &&
                DatabaseId==file2.DatabaseId &&
                DatabaseType == file2.DatabaseType);
    }
};

struct DTXField
{
    QString Id;
    QString Name;

    DTXField();
    DTXField(QString id, QSqlDatabase database);

    inline bool operator==(const DTXField &file2) const
    {
        bool isEqual;
        isEqual = Id == file2.Id && Name == file2.Name;
        return isEqual;
    }
};

struct DTXChapter
{
    QString id;
    QString name;
    QString fieldId;

    inline bool operator==(const DTXChapter &file2) const
    {
        bool isEqual;
        isEqual = id == file2.id && name == file2.name && fieldId == file2.fieldId;
        return isEqual;
    }

    DTXChapter();
    DTXChapter(QStringList list);
};

struct DTXChapters : QHash<QString,DTXChapter>
{
    DTXChapters();
    DTXChapters(QString id,QSqlDatabase database);
};

struct DTXSection
{
    QString id;
    QString name;
    QString chapterId;
    inline bool operator==(const DTXChapter &file2) const
    {
        bool isEqual;
        isEqual = id == file2.id && name == file2.name && fieldId == file2.fieldId;
        return isEqual;
    }

    DTXSection();
    DTXSection(QStringList list);
};

struct DTXSections : QHash<QString,DTXSection>
{
    DTXSections();
    DTXSections(QString id,QSqlDatabase database);

};

struct DTXSubSection
{
    QString id;
    QString name;
    QString section;
};

struct DTXSubSections : QHash<QString,DTXSubSection>
{
    DTXSubSections();
    DTXSubSections(QString id,QSqlDatabase database);
};

struct DTXFile {
    QString Id = QString();
    DTXFileType FileType;
    DTXField Field;
    DTXChapters Chapters;
    DTXSections Sections;
    DTXSubSections SubSections;
    int Difficulty = 0;
    QString Path = QString();
    QDateTime Date = QDateTime();
    DTXSolutionState Solved;
    QString Bibliography = QString();
    QString Content = QString();
    DTXIncludedFile Preamble;
    QString BuildCommand = QString();
    QString Description = QString();
    QStringList Tags = QStringList();
    QStringList Solutions = QStringList();
    QVariant misc = QVariant();
    DTXDatabaseInfo Database;
    QString DatabaseId = QString();

    QStringList getInfoList(QList<QStringList> List,int i);
    QStringList getChaptersNames();
    QStringList getChaptersIds();
    QStringList getSectionsNames();
    QStringList getSectionsIds();
    QStringList getSubSectionsNames();
    QStringList getSubSectionsIds();

    QList<QStringList> setRecordList(QString querytext,QSqlDatabase database);

    DTXFile();
    DTXFile(QString fileId,QSqlDatabase database);
    DTXFile(const DTXFile &fileinfo);
    DTXFile(QString DTexPath);
    inline bool operator==(const DTXFile &file2) const
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
                  DatabaseId == file2.DatabaseId && Tags == file2.Tags &&
                  Solutions == file2.Solutions;
        return isEqual;
    }

    void WriteDTexFile();

    void setChapters(QString id, QSqlDatabase database);
};
//Q_DECLARE_METATYPE(DTXFile)
Q_DECLARE_METATYPE(DTXFile*)

struct DTXDocument {
    QString Id = QString();
    QString Title = QString();
    QString Type = QString();//DTXFileType
    QString BasicFolder = QString();
    QString SubFolder =  QString();
    QString SubsubFolder = QString();
    QString Path = QString();
    QDateTime Date = QDateTime();
    QString Content = QString();
    DTXIncludedFile Preamble = DTXIncludedFile();
    QString BuildCommand = QString();
    bool NeedsUpdate = false;
    QString Bibliography = QString();//DTXBibliography
    QString Description = QString();
    QString SolutionDocument = QString();
    QStringList Tags = QStringList();
    QList<DTXIncludedFile> FilesIncluded = QList<DTXIncludedFile>();
    QStringList BibEntries = QStringList();
    QVariant misc = QVariant();
    DTXDatabaseInfo Database;//DTXDatabase
    QString DatabaseName = QString();//

    DTXDocument();
    DTXDocument(QString docId,QSqlDatabase database);
    DTXDocument(const DTXDocument &docinfo);
    DTXDocument(QString DTexPath);
    inline bool operator==(const DTXDocument &file2) const
    {
        bool isEqual;
        isEqual = Id == file2.Id && Title == file2.Title &&
                  Type == file2.Type && BasicFolder == file2.BasicFolder &&
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

    void WriteDTexFile();
};
//Q_DECLARE_METATYPE(DTXFile)
Q_DECLARE_METATYPE(DTXDocument*)

struct DTXBibliorgaphy
{
    QString CitationKey;
    QString Type;
    QString Title;
    QStringList Authors;
    QStringList Editors;
    QStringList Translators;
    QString Publisher;
    int Year;
    QString Month;
    QString ISBN;
    QString ISSN;
    int Pages;
    QString Series;
    QString Edition;
    QString Chapter;
    QString Number;
    QString Volume;
    QString Journal;
    QString Institution;
    QString School;
    QString Issue;
    QString Address;
    QString DOI;
    QString URL;
    QString Language;
    QString Location;
    QString Subtitle;
    QString Organization;
    QString Key;
    QString Abstract;
    QString CrossReference;
    QString Note;
    QList<DTXIncludedFile> Entries;
    QString Path;
    QDateTime Date;
    QString SourceCode;
    QFlags<CompileEngine> Build;
    DTXDatabaseInfo Database;
};
Q_DECLARE_METATYPE(DTXBibliorgaphy)

struct DTXBuildCommand
{
    int Id;
    QString Name;
    QString ConsoleCommand;
    QString Path;
    QStringList CommandArguments;
    QString Extention;
    QString CommandType;
};
Q_DECLARE_METATYPE(DTXBuildCommand)

struct DTXTeXCommand
{
    int Id;
    QString Name;
    DTXFileType CommandType;
    QString Path;
    QDateTime Date;
    QString Content;
    QStringList RequiredPackages;
    QString Description;
};
Q_DECLARE_METATYPE(DTXTeXCommand)

struct DTXTable
{
    QString Id;
    QString Name;
    DTXFileType Type;
    QString Environment;
    QString TableStyle;//Options
    int Dimentions[2];
    QString Path;
    QDateTime Date;
    QString Content;
    DTXIncludedFile Preamble;
    QFlags<CompileEngine> BuildCommand;
    QString Caption;
    QString Description;
    DTXDatabaseInfo Database;
};
Q_DECLARE_METATYPE(DTXTable)

struct DTXFigure
{
    QString Id;
    QString Name;
    DTXFileType PlotType;//2DPlot,3Dplot,BarChart,2DGeometry,
    QString Environment;//tikz,pstricks,...
    QString FigureStyle;//Options,tikzStyle...
    float Geometry[2];
    QString Path;
    QDateTime Date;
    QString Content;
    DTXIncludedFile Preamble;
    QFlags<CompileEngine> BuildCommand;
    QString Caption;
    QString Description;
    DTXDatabaseInfo Database;
};
Q_DECLARE_METATYPE(DTXFigure)

struct DTXPackage
{
    QString Id;
    QString Name;
    DTXFileType Type;
    QStringList Topics;
    QString Path;
    QDateTime Date;
    QString Content;
    QStringList Options;
    QString Description;
    QString Documentation;
    QStringList RequiredPackages;
    QList<DTXIncludedFile> IncludedCommandList;
    bool isBuiltIn;
    DTXDatabaseInfo Database;
};
Q_DECLARE_METATYPE(DTXPackage)

struct DTXGeometry
{
    float paperWidth;
    float paperHeight;
    float textPidth;
    float textHeight;
    float topMargin;
    float bottomMargin;
    float leftMargin;
    float rightMargin;
    float innerMargin;
    float outerMargin;
    float marginParWidth;
    float marginParSep;
    float headHeight;
    float headSep;
    float footSkip;
    float columnSep;
    float bindingOffset;
    bool centering;
    bool includeHead;
    bool includeFoot;
    bool includeMarPag;
    bool twoside;
    bool asymetric;
    bool twocolumn;
    bool reverseMarPar;
};

struct DTXPreamble
{
    QString Id;
    QString Name;
    DTXFileType Type;
    QFlags<CompileEngine> Engine;
    QString Path;
    QDateTime Date;
    QString Content;
    QString Class;
    QString PaperSize;
    int FontSize;
    QStringList Options;
    QStringList Languages;
    QString Encoding;
    DTXGeometry Geometry;
    QString Author;
    QString Title;
    QString Description;
    QList<DTXIncludedFile> CommandList;
    QList<DTXIncludedFile> PackageList;
    bool useBibliography;
    CompileEngine BibCompileEngine;
    bool makeIndex;
    bool makeGlossaries;
    bool hasTOC;
    bool hasLOT;
    bool hasLOF;
    DTXDatabaseInfo Database;

    DTXPreamble();
};
Q_DECLARE_METATYPE(DTXPreamble)

struct DTXClass
{
    QString Id;
    QString Name;
    DTXFileType Type;
    QFlags<CompileEngine> Engine;
    QString Path;
    QDateTime Date;
    QString Content;
    QString PaperSize;
    int FontSize;
    DTXGeometry Geometry;
    QStringList Options;
    QStringList Languages;
    QString Encoding;
    QString Description;
    QList<DTXIncludedFile> CommandList;
    QList<DTXIncludedFile> PackageList;
    DTXDatabaseInfo Database;
};
Q_DECLARE_METATYPE(DTXClass)

//---------------------------------------------------------

class FileCommands
{
public:
    FileCommands();
    static QString CreateTexFile(QString fullFilePath, bool addToPreamble, QString addStuffToPreamble);
    static void BuildDocument(DTXBuildCommand Command, QString fullFilePath);
    static void ClearOldFiles(QString fullFilePath);
    static void ShowPdfInViewer(QString File, QPdfViewer * view);
//    static void ShowPdfInViewer(QString exoFile, PdfViewer * view);
//    static void ShowPdfInViewer(QString exoFile, PdfViewerWidget * view);
    static QString NewFileText(QString fileName, QString FileContent);
    static QString NewFileText(QString fileName, QString FileContent,QSqlDatabase database);
    static QString NewFilePathAndId(DTXFile *info, bool needsSubSection);
    static QString ClearMetadataFromContent(QString Content);
    static QString ClearDocumentContent(QString Content);
    static QString GetPreamble(QString Content);
    static QHash<QString,QString> GetSingleFileIdFromContent(QString Content);
    static QHash<QString,QString> GetSingleFileIdFromFile(QString filePath);
    static QList<QHash<QString, QString>> GetFileIdsFromContent(QString Content);
    static DTXDatabaseInfo GetDatabaseTypeFromDTexFile(QString file);
    static void AddNewFileToDatabase(DTXFile * fileInfo,QSqlDatabase database);
    static void UpdateFileInfo(DTXFile * fileInfo,QSqlDatabase database);
    static DTXFile *CreateSolutionData(DTXFile * fileInfo,QSqlDatabase database);
    static QString CreateSolutionPath(DTXFile * fileInfo, QSqlDatabase database);
    static QString PrintSolutionState(DTXSolutionState state);
};

#endif // FILECOMMANDS_H
