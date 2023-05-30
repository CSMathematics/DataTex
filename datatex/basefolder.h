#ifndef BASEFOLDER_H
#define BASEFOLDER_H

#include <QDialog>
#include <QHash>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QComboBox>
#include <QLayout>
#include <QtSql>
#include <QSqlDatabase>
#include <QWizard>
#include <QWizardPage>
#include <QCheckBox>
#include <QTableWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QRadioButton>
#include "keygenerator.h"
#include "settings.h"

namespace Ui {
class BaseFolder;
}
class PasswordLineEdit;
class BaseFolder : public  QWizard
{
    Q_OBJECT

public:
    explicit BaseFolder(QWidget *parent = nullptr);
    ~BaseFolder();

    void accept() override;

    static QStringList fields;
    static QStringList bibfields;
    static QStringList FieldTypesValues;
    static QStringList BibTypesValues;
    static QStringList Metadata;
    static QStringList MetadataNames;
    static QString DatabaseType;
    enum { Info, Data, Docs, Final};
    static QRadioButton * FilesDB;
    static QRadioButton * DocsDB;
    static QString Prefix;
    static QString UserName;
    static QString PassWord;

private slots:
    void back() const;

public slots:

signals:

    void newbase(QString Path,QString FolderName,QString FileName,QString DatabaseType);
    void newdocbase(QString Path,QString FolderName,QString FileName);

private:
    Ui::BaseFolder *ui;
};

class InfoPage : public QWizardPage
{
    Q_OBJECT

public:
    InfoPage(QWidget *parent = nullptr);

private:
    QLabel * CheckDatabaseName;
    QLineEdit * DatabasePath;
    QLineEdit * DatabaseFileName;
    QLineEdit * DatabaseName;
    QPushButton * DatabasePathButton;
    QStringList DatabaseList;
    QString DatabaseType;
    QString Table;
    QLineEdit * prefix;
    QLineEdit *userName;
    PasswordLineEdit *passwordLine;
    bool usePrefix = true;
    bool encryptDatabase = true;

private slots:
    void Database_Path();
    void CheckDatabase();
    int nextId() const;
//    void SetDatabaseType();
    bool isComplete() const;
signals:
    bool readyChanged(bool isReady);
};

class DataPage : public QWizardPage
{
    Q_OBJECT

public:
    DataPage(QWidget *parent = nullptr);

    static QList<QCheckBox *> newCheckList;
    static QList<QLineEdit *> newlabelList;
    static QList<QLineEdit * >  newFieldLineList;
    static QList<int> addedIdList;
    static QList<int> removedIdList;
    static QList<QComboBox *> newcomboList;
    void initializePage() override;
    ~DataPage();

private:
    QStringList BasicDataBaseFields;
    QStringList BasicDataBaseValues;
    QStringList OptionalDataBaseFields;
    QStringList OptionalDataBaseValues;
    QList<QLabel *> labelList;

    QList<QLineEdit * >  lineList;
    QList<QComboBox *> basiccomboList = QList<QComboBox *>();

    QList<QHBoxLayout *> hLayoutList;
    QButtonGroup *editorGroup;
    QButtonGroup * checkGroup = new QButtonGroup();
    QPushButton * AddFieldButton;
    QPushButton * RemoveFieldButton;

    QGridLayout *DataLayout;
    QStringList FieldTypes;
    int optfield;
    QStringList FieldList;
    QLabel * CheckFieldId;
    bool next;

private slots:
    void AddField();
    void RemoveField();
    void CheckNewField(QString text);
    void CheckNext();
//    int nextId() const override;
};

//class BibliographyPage : public QWizardPage
//{
//    Q_OBJECT

//public:
//    BibliographyPage(QWidget *parent = nullptr);

//    static QList<QCheckBox *> newBibCheckList;
//    static QList<QLineEdit *> newBiblabelList;
//    static QList<QLineEdit * >  newBibLineList;
//    static QList<int> addedBibIdList;
//    static QList<int> removedBibIdList;
//    static QList<QComboBox *> newcomboList;

////    void initializePage() override;

//    ~BibliographyPage();

//private:
//    QStringList BasicBibliographyFields;
//    QStringList BasicBibliographyValues;
//    QGridLayout * BibliographyLayout;
//    QPushButton * AddBibTexButton;
//    QPushButton * RemoveBibTexButton;
//    QStringList FieldTypes;
//    int optfield;
//    QList<QLabel *> labelList;
//    QList<QLineEdit * >  lineList;
//    QList<QComboBox *> basiccomboList = QList<QComboBox *>();

//    QList<QHBoxLayout *> hLayoutList;
//    QButtonGroup *editorGroup;
//    QButtonGroup * checkGroup = new QButtonGroup();
//    QStringList FieldList;
//    QLabel * CheckFieldId;
//    bool next;

//private slots:
//    void AddBibField();
//    void RemoveBibField();
//    void CheckNewBibField(QString text);
//    void CheckNext();
//};

class FinalPage : public QWizardPage
{
    Q_OBJECT

public:
    FinalPage(QWidget *parent = nullptr);
    void initializePage() override;
    static QTableWidget * table2;
    ~FinalPage();

private:
    QStringList BasicDataBaseFields;
    QList<QLabel *> labelList;
    QVBoxLayout *layout;
    QGridLayout * hlayout;
    QTableWidget * table;
    QLabel *PathLabel;
    QLabel *FileNameLabel;
    QLabel *NameLabel;
    QLabel *label3;
//    QLabel * label4;
    QLabel * label5;

private slots:

};

#endif // BASEFOLDER_H
