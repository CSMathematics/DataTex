#ifndef PREAMBLESETTINGS_H
#define PREAMBLESETTINGS_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include "latexeditorwidget.h"
#include "switch.h"

namespace Ui {
class PreambleSettings;
}

class PreambleSettings : public QDialog
{
    Q_OBJECT

public:
    explicit PreambleSettings(QWidget *parent = nullptr, QString PreambleContent = QString());
    ~PreambleSettings();

public slots:

    void EditLine_DataTex(QStringList Line);

private slots:

    QStringList on_OkButton_accepted();
    void PackageList_SelectionChanged();
    void on_OkButton_rejected();
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void on_PreambleContentWidget_cursorPositionChanged();
    void EnableAccept();
    void getClass();
    bool getLanguage();

private:
    Ui::PreambleSettings *ui;
    QStringList Packages;
    QStringList Descriptions;
    QSortFilterProxyModel * filter;
    QStringListModel * model;
    QHash<QString,QString> MapDescriptions;
//    LatexTextWidget * PreambleContentWidget;
    QString ClassLine;
    QString temp_ClassLine;
    QString docClass;
    QString classOptions;
    QString LanguageLine;
    QString temp_LanguageLine;
    QStringList Languages;

signals:

    void newpreamblesignal(QStringList);
};

#endif // PREAMBLESETTINGS_H
