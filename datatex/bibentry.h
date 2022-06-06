#ifndef BIBENTRY_H
#define BIBENTRY_H

#include <QDialog>
#include <QHash>

namespace Ui {
class BibEntry;
}

class BibEntry : public QDialog
{
    Q_OBJECT

public:
    explicit BibEntry(QWidget *parent = nullptr, bool EditMode = false,bool InsertMode = false,
                      QHash<QString, QString> values = QHash<QString, QString>());
    ~BibEntry();
    QHash<QString, QString> getBibValues();
    void InsertValues(QHash<QString,QString> values);
    bool BibEntryExists(QString text);

private:
    Ui::BibEntry *ui;
    QStringList OptBibFields;
    QHash<QString,QString> bibValues;
    bool isEditMode;
    bool isInsertMode;
    QHash<QString, QString> editValues;
    QStringList citationkeys;
    QStringList authors;
    QStringList editors;
    QStringList translators;

private slots:
    void on_buttonBox_accepted();
    QString BibSourceCode();
    void BibEditMode();

signals:
    void citationExists(bool exists);
};

#endif // BIBENTRY_H
