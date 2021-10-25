#ifndef UPDATEDOCUMENTCONTENT_H
#define UPDATEDOCUMENTCONTENT_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQueryModel>

namespace Ui {
class UpdateDocumentContent;
}

class UpdateDocumentContent : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDocumentContent(QWidget *parent = nullptr, QString Document = QString(),
                                   QStringList files = QStringList(), QStringList databases = QStringList());
    ~UpdateDocumentContent();

private:
    Ui::UpdateDocumentContent *ui;
    QString CurrentDocument;
    QStringList FilesInDocument;
    QList<QSqlDatabase> DatabasesInDocument;
    QMap<QString,QSqlDatabase> DatabasePerFile;
    QMap<QString,QString> FilesContentsFormFile;
    QMap<QString,QString> FilesContentsFormDocument;

private slots:
    void GetFileContentsFromFile();
    void GetFileContentsFromDocument();
    void FilesTable_selection_changed();
};

#endif // UPDATEDOCUMENTCONTENT_H
