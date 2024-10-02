#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QWizard>
#include <QSqlDatabase>
#include <QtWidgets>


QT_BEGIN_NAMESPACE
struct DTXFileType;
struct DTXDatabase;
QT_END_NAMESPACE

namespace Ui {
class DatabaseCreator;
}

enum struct DTXSolutionState : int
{
    //File and FileType flags
    Solvable                = 1 <<0,
    NotSolvable             = 1 <<1,
    Solution                = 1 <<2,
    //Solvable file flags
    Solved                  = 1 <<3,
    UnSolved                = 1 <<4,
    HasIncompleteSolutions  = 1 <<5,
    //Solution flags
    SolutionComplete        = 1 <<6,
    SolutionIncomplete      = 1 <<7
};
Q_DECLARE_FLAGS (SolutionStates, DTXSolutionState)
Q_DECLARE_OPERATORS_FOR_FLAGS (SolutionStates)
Q_DECLARE_METATYPE(DTXSolutionState)

enum DTXDatabaseType { FilesDB,
                       DocumentsDB,
                       BibliographyDB,
                       TablesDB,
                       FiguresDB,
                       CommandsDB,
                       PreamblesDB,
                       PackagesDB,
                       ClassesDB,
                       NoType
};
Q_DECLARE_METATYPE(DTXDatabaseType)

struct DTXDBFieldInfo
{
    QString Id;
    QString Name;
    bool isBasic;
    QString DataType;
    bool isVisibleInTable;
    int Index;

    inline bool operator==(const DTXDBFieldInfo &file2) const
    {
        return (Id==file2.Id &&
                Name==file2.Name &&
                isBasic == file2.isBasic &&
                DataType == file2.DataType);
    }
};
Q_DECLARE_METATYPE(DTXDBFieldInfo)

struct DTXDatabase
{
    QSqlDatabase Database;
    QString BaseName;
    QString Description;
    int Type = NoType;
    QString Path;
    bool UsePrefix = false;
    QString Prefix;
    bool Encrypt = false;
    QString Username;
    QString Password;
    QList<DTXDBFieldInfo> DBFieldInfoList;
    QList<DTXFileType> FileTypes;
    bool IsConnected;

    inline bool operator==(const DTXDatabase &DTXDB2) const
    {
        bool isEqual;
        isEqual = BaseName == DTXDB2.BaseName &&
                  Description == DTXDB2.Description && Type == DTXDB2.Type &&
                  Path == DTXDB2.Path &&
                  UsePrefix == DTXDB2.UsePrefix && Prefix == DTXDB2.Prefix &&
                  Encrypt == DTXDB2.Encrypt && Username == DTXDB2.Username &&
                  Password == DTXDB2.Password && DBFieldInfoList == DTXDB2.DBFieldInfoList;
        return isEqual;
    }

    QStringList getIdsList()
    {
        QStringList column;
        for(const DTXDBFieldInfo &fieldInfo : qAsConst(DBFieldInfoList)){
            column.append(fieldInfo.Id);
        }
        return column;
    }

    QStringList getNamesList()
    {
        QStringList column;
        for(const DTXDBFieldInfo &fieldInfo : qAsConst(DBFieldInfoList)){
            column.append(fieldInfo.Name);
        }
        return column;
    }

    QList<int> basicVisibleFieldIndexes()
    {
        QList<int> list;
        int index = 0;
        for(const DTXDBFieldInfo &fieldInfo : qAsConst(DBFieldInfoList)){
            if(fieldInfo.isBasic && fieldInfo.isVisibleInTable){
                list.append(index);
            }
            index++;
        }
        return list;
    }

    QStringList customFieldIds()
    {
        QStringList list;
        for(const DTXDBFieldInfo &fieldInfo : qAsConst(DBFieldInfoList)){
            if(!fieldInfo.isBasic && fieldInfo.isVisibleInTable){
                list.append(fieldInfo.Id);
            }
        }
        return list;
    }
};
Q_DECLARE_METATYPE(DTXDatabase)

struct DTXFileType
{
    QString Id;
    QString Name;
    QString FolderName;
    DTXSolutionState Solvable;
    QString BelongsTo;
    QString Description;
    bool BuiltIn;
    DTXDatabaseType DBType;

    inline bool operator==(const DTXFileType &file2) const
    {
        return (Id==file2.Id &&
                Name==file2.Name &&
                FolderName == file2.FolderName);
    }

    DTXFileType();
    DTXFileType(QStringList list);

    QStringList getListFromDTXFileType()
    {
        return {Id,Name,FolderName,QString::number((int)Solvable),BelongsTo,Description};
    }
};
Q_DECLARE_METATYPE(DTXFileType)

struct DTXDatabaseInfo
{
    QString Id;
    QString Name;
    QString Path;
    DTXDatabaseType Type;
    DTXDatabase getDTXDatabase();
};


class DatabaseFieldInfoWidget : public QWidget
{
    Q_OBJECT

public:
    QLabel * Id;
    QLineEdit * CustomId;
    QLineEdit * NameDescription;
    QComboBox * DataTypes;
    QHBoxLayout * HLayout;
    QCheckBox * VisibleInTable;
    QCheckBox * checkRemove;
    bool isBasic;
    explicit DatabaseFieldInfoWidget(QWidget *parent = nullptr,bool isBasicField = true);
    ~DatabaseFieldInfoWidget();
    DTXDBFieldInfo getFieldInfoList();
private:

};

class DatabaseCreator : public QWizard
{
    Q_OBJECT

public:
    explicit DatabaseCreator(QWidget *parent = nullptr);
    ~DatabaseCreator();
    DTXDatabase NewDatabase;


private slots:
    void on_DatabaseCreator_accepted();
    void CheckDatabase();
    void isPageReady();
    bool ItemHasTopic(QListWidgetItem *item);
    QStringList JsonArrayToList(QJsonArray fields){
        QStringList list;
        for(const QJsonValue &field : fields){
            QString value = field.toString();
            list.append(value);
        }
        return list;
    }
    QString getDatabaseTypeName(int type);

private:
    Ui::DatabaseCreator *ui;
    QStringList DatabaseList;
    bool isNextReady = false;
    QList<DatabaseFieldInfoWidget *> DatabaseInfoWidgetsList;
    QList<DTXFileType> FileTypeList;
    QButtonGroup FileTypeGroup;
    QString DBTemplateFileName;

signals:
    void newbase(DTXDatabase DTXDB);
};

#endif // DATABASECREATOR_H
