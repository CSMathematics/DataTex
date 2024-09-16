#include "dtxsettings.h"
#include <QSettings>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

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
