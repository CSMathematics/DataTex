#ifndef NOTESDOCUMENTS_H
#define NOTESDOCUMENTS_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QComboBox>
#include <QString>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QDesktopServices>
#include <QList>
#include <QUrl>
#include <QDesktopServices>
#include <QGridLayout>
#include <QDebug>
#include <QModelIndex>
#include <algorithm>
#include <QtSql>
#include <QSqlDatabase>
#include <QRadioButton>
#include "addfolder.h"
#include "datatex.h"
#include "ExtendedTableWidget.h"
#include "pdfviewer.h"
#include "switch.h"

namespace Ui {
class NotesDocuments;
}

class NotesDocuments : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDocuments(QWidget *parent = nullptr);
    ~NotesDocuments();

public slots:
    
    void EditBasicForders_DataTex(QString line);

private slots:

    bool isWhiteSpace(const QString & str);
    void Folder(bool checked);
    void on_CreateBasicFolder_clicked();
    void on_OkbuttonBoxFolders_accepted();
    void on_OkbuttonBoxFolders_rejected();
    QString SavePath(const QModelIndex &index);
    void Basic(QString newfolder);
    void on_FoldersStructureView_clicked(const QModelIndex &index);
    void DocumentTable_selectionChanged();
    void updateFilter(QStringList values);
    void NewSubFolder(QString newfolder);
    void on_CreateSubFolder_clicked();
    void on_RemButton_clicked();
    void on_EditButton_clicked();
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void AddDocType(QString docType);

    void on_OpenPath_clicked();

    void on_pushButton_clicked();

private:
    Ui::NotesDocuments *ui;
    addfolder * newFolder;
    QSqlDatabase currentbase;
    QSqlDatabase currentbase_Exercises;
    QButtonGroup * radiogroup;
    QList<QRadioButton * >  radioList;
    QString BasicFolderFile;
    QString NotesPath;

    QStringList Exer_List;
    QStringList Solutions_List;
    QStringList Solved_List;
    QStringList Unsolved_List;
    PdfViewer * view;
    ExtendedTableWidget * DocumentTable;
    QStringList Database_DocumentTableColumns;
    QString DocumentFileName;
    QString DocumentType;
    QString DocFileType;
    QString BasicFolder;
    QString SubFolder;
    QString SubSubFolder;
    QString DocumentFilePath;
    int level;

signals:
    void pathline(QString line);
    void createnewdocument(QString file,QString content);
    void insertfiles();
    void nolines();
    void OpenSolutionFile(QString SolutionsTexFile);
};

#endif // NOTESDOCUMENTS_H
