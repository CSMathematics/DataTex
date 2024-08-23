#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QDialog>
#include <QRandomGenerator>
#include <QClipboard>

namespace Ui {
class KeyGenerator;
}

class KeyGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit KeyGenerator(QWidget *parent = nullptr);
    ~KeyGenerator();

private:
    Ui::KeyGenerator *ui;
    QString CharList;
    int KeyLength;

    int capCount;
    int smallCount;
    int numCount;
    int symCount;

    void UpdateKey();

signals:
    QString password(QString key);
private slots:
    void on_buttonBox_accepted();
};

#endif // KEYGENERATOR_H
