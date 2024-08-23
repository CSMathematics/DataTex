#ifndef EDITHISTORY_H
#define EDITHISTORY_H

#include <QDialog>
#include "sqlfunctions.h"
#include "datatex.h"
#include "qpdfview.h"
#include "latexeditor.h"
#include "filecommands.h"

namespace Ui {
class EditHistory;
}

class EditHistory : public QDialog
{
    Q_OBJECT

public:
    explicit EditHistory(QWidget *parent = nullptr, QString filePath = QString(),
                         QString buildCommand = QString(), QString currentContent = QString(),
                         QStringList metadata = QStringList(), bool isDocument = false);
    ~EditHistory();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::EditHistory *ui;
    QString DatabaseFile;
    int currentrow;
    QString CurrentBuildCommand;
    QString Content;
    QString CurrentContent;
    QPdfViewer * TempFileView;
    QPdfViewer * CurrentFileView;
    QString temp_file;
    QStringList Metadata;
    QSqlDatabase currentdatabase;
};

#endif // EDITHISTORY_H
