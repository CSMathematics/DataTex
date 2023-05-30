#ifndef CSVFUNCTIONS_H
#define CSVFUNCTIONS_H
//#include <QtCore>
#include <QHash>
#include <QSqlQuery>
#include <QSqlDatabase>


class CsvFunctions
{
public:
//    CsvFunctions();
    static void WriteDataToCSV(QString fileName, QSqlDatabase database);
    static void WriteDocDataToCSV(QString fileName, QSqlDatabase database);
    static QHash<QString,QString> ReadCsv(QString fileName);
    static QString getFile(QString file, QString databasepath);
    static QString getDocument(QString file, QString databasepath);

private:
};

#endif // CSVFUNCTIONS_H
