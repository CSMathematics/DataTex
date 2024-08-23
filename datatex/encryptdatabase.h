#ifndef ENCRYPTDATABASE_H
#define ENCRYPTDATABASE_H

#include <QDialog>
#include "databasecreator.h"

namespace Ui {
class EncryptDatabase;
}

class EncryptDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit EncryptDatabase(QWidget *parent = nullptr, DTXDatabase database = DTXDatabase());
    ~EncryptDatabase();

private:
    Ui::EncryptDatabase *ui;

signals:
    void DBEncrypted(QString DBId);
};

#endif // ENCRYPTDATABASE_H
