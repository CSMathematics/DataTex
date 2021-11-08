#ifndef NEWDATABASEFILE_H
#define NEWDATABASEFILE_H

#include <QDialog>
#include <QtSql/QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include "qpdfviewer.h"
#include <QListWidget>
#include <QTableView>
#include <QRadioButton>

namespace Ui {
class NewDatabaseFile;
}

class NewDatabaseFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewDatabaseFile(QWidget *parent = nullptr, QStringList meta = QStringList()
            , QStringList sections = QStringList(), bool editMode = false);
    ~NewDatabaseFile();

private:
    Ui::NewDatabaseFile *ui;

    QSqlDatabase currentbase;
    QString DataBase_Path;
    QPdfViewer * TheoryView;
    QHash<QString,QString> mapIdsNames;
    QStringList SectionList;
    QStringList InportedSectionList;
    QString FileType;
    bool acceptNewFile;
    QStringList Bibliography_Ids;
    bool EditMode;
    QStringList metadata;
    QList<QAbstractButton *> CustomFileTypesList;

private slots:

    void on_FieldTable_itemClicked(QListWidgetItem *item);
    void updateTableView(QTableView * table,QString QueryText);
    void ExerciseFileList_selection_changed();
    void loadImageFile(QString exoFile, QPdfViewer *view);
    void on_SectionExercisesChapterComboBox_currentIndexChanged(int index);
    void on_SectionExercisesSectionComboBox_currentIndexChanged(int index);
    void on_SectionExercisesExTypeComboBox_currentIndexChanged(int index);
    QString NewFilePathAndId();
    void on_SectionList_itemClicked(QListWidgetItem *item);
    void on_SectionList_itemDoubleClicked(QListWidgetItem *item);
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_SelectedSections_itemClicked(QListWidgetItem *item);
    void on_ExerciseFileList_doubleClicked(const QModelIndex &index);
    void closeEvent (QCloseEvent *event);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_addChapter_clicked();
    void on_addField_clicked();
    void on_removeField_clicked();
    void on_removeChapter_clicked();
    void on_addSection_clicked();
    void on_removeSection_clicked();
    void on_addExType_clicked();
    void on_removeExType_clicked();
    void EditModeIsEnabled();

    void on_addFileType_clicked();

    void on_removeFileType_clicked();

signals:

    void acceptSignal(QString fileName,/* QMap<QString,QString> metadata,QStringList SectionList,*/QString FileContent);

    void listChanged();
};

#endif // NEWDATABASEFILE_H
