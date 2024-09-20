#include "dtxsettings.h"
#include <QSettings>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QString>

DTXSettings::DTXSettings()
{
    QSettings settings;
    settings.beginGroup("Application_Settings");
    saveLocation = settings.value("SaveLocation").toString();
}

QString DTXSettings::getCurrentPreambleContent(QString preambleId)
{
    QString content;
    QFile file("/home/spyros/.datatex/Preambles.json");
    file.open(QFile::ReadOnly | QFile::Text);
    const QJsonArray &json(QJsonDocument::fromJson(file.readAll()).array());
    file.close();
    for (const QJsonValue &value: json) {
        QString id = value["Id"].toString();
        if(id == preambleId){
            content = value["Preamble_Content"].toString();
            break;
        }
    }
    return content;
}

QList<QStringList> DTXSettings::getCurrentPreambleInfo()
{
    QList<QStringList> infoList;
    QStringList list;
    QFile file("/home/spyros/.datatex/Preambles.json");
    file.open(QFile::ReadOnly | QFile::Text);
    const QJsonArray &json(QJsonDocument::fromJson(file.readAll()).array());
    file.close();
    for (const QJsonValue &value: json) {
        list.append(value["Id"].toString());
        list.append(value["Name"].toString());
        infoList.append(list);
    }
    return infoList;
}

QStringList DTXSettings::getDatabasesIds()
{
    QStringList list;
    QFile file("/home/spyros/Έγγραφα/DataBases.json");
    file.open(QFile::ReadOnly | QFile::Text);
    const QJsonObject &json(QJsonDocument::fromJson(file.readAll()).object());
    file.close();
    const QJsonArray databases = json["Databases"].toArray();

    for (const QJsonValue &value: databases) {
        const QJsonObject &dbObject(value.toObject());
        QString id = dbObject["FileName"].toString();
        list.append(id);
    }
    return list;
}

QList<QStringList> DTXSettings::getDatabaseBasicMeta(int dbType)
{
    QList<QStringList> outputList;
    QFile file("/home/spyros/.datatex/Metadata.json");
    file.open(QFile::ReadOnly | QFile::Text);
    const QJsonArray &json(QJsonDocument::fromJson(file.readAll()).array());
    file.close();

    for (const QJsonValue &value: json) {
        const QJsonObject &metaObject(value.toObject());
        // qDebug()<<metaObject["DatabaseType"].toInt();
        if(metaObject["DatabaseType"].toInt() == dbType && metaObject["Basic"].toInt() == 1){
            QStringList list;
            list.append(metaObject["Id"].toString());
            list.append(metaObject["Name"].toString());
            list.append(metaObject["DataType"].toString());
            int i = metaObject["VisibleInTable"].toInt();
            list.append(QString::number(i));
            outputList.append(list);
        }
    }
    return outputList;
}

// QHash<int, DTXBuildCommand> DTXSettings::setDTXBuildCommands()
// {
//     QHash<int, DTXBuildCommand> hash;
//     QList<QStringList> list;// = SqlFunctions::GetRecordList("SELECT FLOOR(power(2,ROWID-1)),* FROM BuildCommands",DataTeX_Settings);
//     int index = 0;
//     for(const QStringList &item : list){
//         DTXBuildCommand Command;
//         Command.Id = item.at(0).toInt();
//         Command.Name = item.at(1);
//         Command.ConsoleCommand = item.at(2);
//         Command.Path = item.at(3);
//         Command.CommandArguments = QString(item.at(4)).split(",");
//         Command.Extention = item.at(5);
//         Command.CommandType = item.at(6);
//         // if(Command.CommandType == "Build"){
//         //     CompileMenu->actions().at(index)->setData(QVariant::fromValue(Command));
//         // }
//         // else if(Command.CommandType == "Convert"){
//         //     ConvertMenu->actions().at(index-16)->setData(QVariant::fromValue(Command));
//         // }
//         // QSqlQuery CommandsQuery(DataTeX_Settings);
//         // CommandsQuery.exec(QString("UPDATE BuildCommands SET Path = '%1' WHERE Name = '%2';").arg(Command.Path,Command.Name));
//         // DTXBuildCommands.insert(Command.Id,Command);
//         //        qDebug()<<(Command.CommandType == "Build")<<CompileMenu->actions().at(index)->data().value<DTXBuildCommand>().Name;
//         index++;
//     }
//     return hash;
// }


QString DTXSettings::findTexLiveBinFolder()
{

    QString texLiveBinFolder;

#ifdef Q_OS_WIN
    QStringList windowsPossiblePaths = {};
        texLiveBinFolder = "C:\\Program Files\\MiKTeX\\miktex\\bin";

        QFileInfo fileInfo(texLiveBinFolder);
        if (!fileInfo.exists() || !fileInfo.isDir()) {
            texLiveBinFolder = "C:\\texlive\\bin";
            fileInfo.setFile(texLiveBinFolder);
            if (!fileInfo.exists() || !fileInfo.isDir()) {
                return "";
            }
        }
#elif defined Q_OS_LINUX
    QStringList linuxPossiblePaths = {"/usr/share/texlive/bin","/usr/local/texlive/bin",
                                      "~/texlive/bin","~/.local/bin/texlive/bin",
                                      "/usr/local/texlive/2024/bin/",
                                      "/usr/local/texlive/2024/bin/x86_64-linux/"};


    for(const QString &path: linuxPossiblePaths){
        QFileInfo fileInfo(path);
        if (fileInfo.exists() && fileInfo.isDir()) {
            texLiveBinFolder = path;
            return texLiveBinFolder;
            qDebug()<<texLiveBinFolder;
        }
    }
#elif defined Q_OS_MACOS
    QStringList macosPossiblePaths = {};
    texLiveBinFolder = "/usr/local/texlive/bin";

    QFileInfo fileInfo(texLiveBinFolder);
    if (!fileInfo.exists() || !fileInfo.isDir()) {
        texLiveBinFolder = "/Library/TeX/texbin";
        fileInfo.setFile(texLiveBinFolder);
        if (!fileInfo.exists() || !fileInfo.isDir()) {
            return "";
        }
    }
#endif

    return texLiveBinFolder;
}
