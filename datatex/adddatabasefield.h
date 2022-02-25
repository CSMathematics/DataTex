#ifndef ADDDATABASEFIELD_H
#define ADDDATABASEFIELD_H

#include <QDialog>

namespace Ui {
class AddDatabaseField;
}

class AddDatabaseField : public QDialog
{
    Q_OBJECT

public:
    explicit AddDatabaseField(QWidget *parent = nullptr);
    ~AddDatabaseField();

public slots:
    void EditLine_DataTex(QStringList Line);

private:
    Ui::AddDatabaseField *ui;

private slots:
    QStringList on_OkButton_accepted();
    void on_OkButton_rejected();
    void EnableAccept();

signals:

    void newline(QStringList);//---
};

#endif // ADDDATABASEFIELD_H
