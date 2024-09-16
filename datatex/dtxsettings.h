#ifndef DTXSETTINGS_H
#define DTXSETTINGS_H

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
};

#endif // DTXSETTINGS_H
