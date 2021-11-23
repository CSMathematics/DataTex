#ifndef ADDFILETOEDITOR_H
#define ADDFILETOEDITOR_H

#include <QDialog>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QFile>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include "sqlfunctions.h"
#include "datatex.h"
#include "ExtendedTableWidget.h"
#include "pdfviewer.h"
//#include <QtPdf>
//#include <QtPdfWidgets>
//#include "qpdfviewer.h"

namespace Ui {
class AddFileToEditor;
}

class AddFileToEditor : public QDialog
{
    Q_OBJECT

public:
    explicit AddFileToEditor(QWidget *parent = nullptr, QString currentTexFile = QString(), QString BuildCommand = QString());
    ~AddFileToEditor();

public slots:

private slots:
    void on_Okbutton_rejected();
    void closeEvent (QCloseEvent *event);
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_Okbutton_accepted();
    void on_Rebuild_clicked();
    void FilesTable_selectionchanged();
    void LoadDatabaseFiles(QSqlDatabase database,QString query);
    void updateFilter(QStringList values);
    QList<QStringList> getDatabaseFields(QSqlDatabase database);
    void on_checkBox_clicked(bool checked);
    void SaveText();
    void on_FilesDatabasesCombo_currentIndexChanged(int index);
    void on_DocumentContent_cursorPositionChanged();
    void on_Save_clicked();
    void on_DocumentContent_textChanged();

private:
    Ui::AddFileToEditor *ui;
    QSqlDatabase currentbase;
    QSqlTableModel * tableModel;
    QString PreviewFile;
    QString FileType;
    QString ExSubType;
    QString SolutionFileType;
    QString ComFileType;
    QList<QCheckBox *> checklist;
    QString CurrentDatabaseFile;
    ExtendedTableWidget * FilesTable;
    PdfViewer *view;

    PdfViewer *DocView;
    QStringList Database_FileTableFields;
    QStringList Database_FileTableFieldNames;
//    QList<QSqlDatabase> datalist;
    QString CurrentBuildCommand;
    QStringList ExercisesInsideDocument;
    QStringList DatabasesInsideDocument;
    QList<QList<int>> positions;
    QString DatabaseFileContent;

signals:

    void files(QStringList paths);
};

#endif // ADDFILETOEDITOR_H
