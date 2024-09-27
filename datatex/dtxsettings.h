#ifndef DTXSETTINGS_H
#define DTXSETTINGS_H

#include "filecommands.h"
#include <QObject>
#include <QJsonDocument>

class DTXSettings
{
public:
    DTXSettings();

    QString saveLocation;

public slots:
    QString getCurrentPreambleContent(QString preambleId);
    QStringList getDatabasesIds();
    QList<QStringList> getDatabaseBasicMeta(int dbType);
    QList<QStringList> getCurrentPreambleInfo();
    QHash<int, DTXBuildCommand> setDTXBuildCommands();
    QString findTexLiveBinFolder();
};

#endif // DTXSETTINGS_H
