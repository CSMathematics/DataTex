#ifndef ADDDATABASEFIELD_H
#define ADDDATABASEFIELD_H

#include <QDialog>
#include <QHash>
#include <QAction>

namespace Ui {
class AddDatabaseField;
}

class AddDatabaseField : public QDialog
{
    Q_OBJECT

public:
    explicit AddDatabaseField(QWidget *parent = nullptr, QString Info = QString(), bool isSubSection = false);
    ~AddDatabaseField();

public slots:
    void EditLine_DataTex(QStringList Line);

private:
    Ui::AddDatabaseField *ui;
    QHash<QString,QString> MapSubSecIds;
    QHash<QString,QString> MapSubSecNames;
    QAction *idAction;
    QAction *nameAction;
    bool IsSubSection;

private slots:
    QStringList on_OkButton_accepted();
    void on_OkButton_rejected();
    void EnableAccept();

signals:

    void newline(QStringList);//---
};

#endif // ADDDATABASEFIELD_H
