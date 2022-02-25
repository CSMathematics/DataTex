#ifndef PATHS_H
#define PATHS_H

#include <QDialog>
#include <QAction>
#include <QLineEdit>
#include <QToolButton>
#include "basefolder.h"
#include "notefolder.h"
#include "datatex.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Paths; }
QT_END_NAMESPACE

class Paths : public QDialog
{
    Q_OBJECT

public:
    Paths(QWidget *parent = nullptr,QString path = QString());
    ~Paths();

public slots:

    void CreateBaseFolder(QString path, QString FolderName, QString fileName);
    void CreateNoteFolder(QString path, QString FolderName, QString FileName);

private slots:
    void on_BaseButton_clicked();
    void on_buttonBox_accepted();
    void on_NoteButton_clicked();
    void on_buttonBox_rejected();
    void on_ComboBaseList_currentIndexChanged(int index);
    void on_DeleteBase_clicked();
    void on_ComboNote_currentIndexChanged(int index);
    void on_DeleteFilesBase_clicked();
    void on_AddBase_clicked();
    void on_PreambleCombo_currentIndexChanged(const QString &arg1);
    void on_AddPreambleButton_clicked();
    void on_RemovePreambleButton_clicked();
    void on_OpenSettingsButton_clicked();
    void AddPreamble(QStringList preamble);
    void LoadTables(QString database);
    void LoadDocTables(QString database);
    void on_ListOfSettings_currentRowChanged(int currentRow);
    void on_AddDocDatabaseButton_clicked();
    void on_EncryptDocDatabase_clicked(bool checked);
    void on_UseDocDatabasePrefix_clicked(bool checked);
    void on_UseDatabasePrefix_clicked(bool checked);
    void on_EncryptDatabase_clicked(bool checked);
    void on_OpenSaveLocation_clicked();
    void on_OpenPdfLatexPath_clicked();
    void on_OpenLatexPath_clicked();
    void on_OpenXeLatexPath_clicked();
    void on_OpenLuaLatexPath_clicked();
    void on_OpenPythontexPath_clicked();
    void on_OpenBibtexPath_clicked();
    void on_OpenAsymptotePath_clicked();

private:
    Ui::Paths *ui;
    BaseFolder * newbasefolder;
    BaseFolder * newnotefolder;
    QSqlDatabase currentbase_Exercises;
    QString DataBase_Path;

    QString prog;
    QString base;
    QString note;

signals:
    void iconsize(int size);
    void selectfont(QString font);
};

class PasswordLineEdit: public QLineEdit
{
public:
    PasswordLineEdit(QWidget *parent=nullptr);
private slots:
protected:
    void onReleased();
    void onPressed();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private:
    QToolButton *button;
};
#endif // PATHS_H
