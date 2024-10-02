#include "dtxsettings.h"
#include <QSettings>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QString>
#include <QDate>
#include "math.h"

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

QHash<int, DTXBuildCommand> DTXSettings::setDTXBuildCommands()
{
    QHash<int, DTXBuildCommand> hash;
    QFile file("/home/spyros/.datatex/BuildCommands.json");
    file.open(QFile::ReadOnly | QFile::Text);
    const QJsonArray &json(QJsonDocument::fromJson(file.readAll()).array());
    file.close();
    int commandId = 0;
    for (const QJsonValue &value: json) {
        const QJsonObject &buildCommandObject(value.toObject());
        DTXBuildCommand Command;
        Command.Id = pow(2,commandId);
        Command.Name = buildCommandObject["Name"].toString();
        Command.ConsoleCommand = buildCommandObject["ConsoleCommand"].toString();
        Command.Path = buildCommandObject["Path"].toString();
        Command.CommandArguments = buildCommandObject["Arguments"].toString().split(",");
        Command.Extention = buildCommandObject["Extention"].toString();
        Command.CommandType = buildCommandObject["CommandType"].toString();
        commandId++;
        // qDebug()<<Command.CommandType;
        hash.insert(Command.Id,Command);
    }
    return hash;
}


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
    QStringList linuxPossiblePaths = {"/usr/share/texlive/bin/","/usr/local/texlive/bin/",
                                      "~/texlive/bin/","~/.local/bin/texlive/bin/"};

    QDate date = QDate::currentDate();
    for(int year = date.year(); year>2008; year--){
        linuxPossiblePaths.append(QString("/usr/local/texlive/%1/bin/x86_64-linux/").arg(QString::number(year)));
        linuxPossiblePaths.append(QString("/usr/local/texlive/%1/bin/").arg(QString::number(year)));
    }
    for(const QString &path: linuxPossiblePaths){
        QFileInfo fileInfo(path+"pdflatex");
        if (fileInfo.exists() && !fileInfo.isDir()) {
            texLiveBinFolder = path;
            return texLiveBinFolder;
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

