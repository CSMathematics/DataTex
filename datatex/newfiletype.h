#ifndef NEWFILETYPE_H
#define NEWFILETYPE_H

#include <QDialog>

namespace Ui {
class NewFileType;
}

class NewFileType : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileType(QWidget *parent = nullptr);
    ~NewFileType();

private slots:
    void on_OkButton_accepted();

private:
    Ui::NewFileType *ui;

signals:
    void filedata(QStringList data);
};

#endif // NEWFILETYPE_H
