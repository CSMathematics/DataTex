#ifndef NEWFILETYPE_H
#define NEWFILETYPE_H

#include <QDialog>
#include "databasecreator.h"

namespace Ui {
class NewFileType;
}

class NewFileType : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileType(QWidget *parent = nullptr, DTXDatabaseType Type = DTXDatabaseType::FilesDB);
    ~NewFileType();
    static bool CreateNewDatabaseFileType(QSqlDatabase database, int DBType, DTXFileType filetype);

private slots:
    void on_OkButton_accepted();

private:
    Ui::NewFileType *ui;
    DTXFileType FileType;
    DTXDatabaseType DBType;

signals:
    void filedata(DTXFileType newFileType);
};

#endif // NEWFILETYPE_H
