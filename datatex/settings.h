#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QAction>
#include <QToolButton>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QDesktopServices>
#include <QDebug>
#include <QCloseEvent>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSettings>
#include <QColorDialog>
#include <QCryptographicHash>
#include "basefolder.h"
#include "datatex.h"
#include "sqlfunctions.h"
#include "preamblesettings.h"
//#include "simplecrypt.h"
#include "basefolder.h"
#include "notefolder.h"
#include "datatex.h"
#include "keygenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

class BaseFolder;
class Settings : public QDialog
{
    Q_OBJECT

public:
    Settings(QWidget *parent = nullptr);
    ~Settings();

public slots:

    void CreateBaseFolder(QString path, QString FolderName, QString fileName);
    void CreateNoteFolder(QString path, QString FolderName, QString FileName);
    void SelectLanguage(QString language);

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
    void AddPreamble(QStringList preamble);
    void LoadTables(QString database);
    void LoadDocTables(QString database);
    void on_ListOfSettings_currentRowChanged(int currentRow);
    void on_AddDocDatabaseButton_clicked();
    void on_UseDocDatabasePrefix_clicked(bool checked);
    void on_UseDatabasePrefix_clicked(bool checked);
    void on_EncryptDatabase_toggled(bool checked);
    void on_OpenSaveLocation_clicked();
    void on_OpenPdfLatexPath_clicked();
    void on_OpenLatexPath_clicked();
    void on_OpenXeLatexPath_clicked();
    void on_OpenLuaLatexPath_clicked();
    void on_OpenPythontexPath_clicked();
    void on_OpenBibtexPath_clicked();
    void on_OpenAsymptotePath_clicked();
    void on_commandColorButton_clicked();
    void on_KeyGeneratorFDB_clicked();
    void on_KeyGeneratorDDB_clicked();

    void on_EncryptDocDatabase_toggled(bool checked);

private:
    Ui::Settings *ui;
    BaseFolder * newbasefolder;
    BaseFolder * newnotefolder;
    QSqlDatabase currentbase_Exercises;
    QString DataBase_Path;
    QString currentLanguage;
    QString currentTheme;
//    QTranslator translator;

    QString prog;
    QString base;
    QString note;

signals:
    void iconsize(int size);
    void selectfont(QString font);
    void selectEditorFont(QFont font);
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
#endif // SETTINGS_H
