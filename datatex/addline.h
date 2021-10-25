#ifndef ADDLINE_H
#define ADDLINE_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QStringListModel>

namespace Ui {
class AddLine;
}

class AddLine : public QDialog
{
    Q_OBJECT

public:
    explicit AddLine(QWidget *parent = nullptr);
    ~AddLine();

public slots:

    void EditLine_DataTex(QStringList Line);

private slots:

    QStringList on_OkButton_accepted();
    void PackageList_SelectionChanged();
    void on_OkButton_rejected();
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void on_textEdit_cursorPositionChanged();
    void EnableAccept();

private:
    Ui::AddLine *ui;
    QStringList Packages;
    QStringList Descriptions;
    QSortFilterProxyModel * filter;
    QStringListModel * model;
    QMap<QString,QString> MapDescriptions;

signals:

    void grammhline(QStringList);//---
};

#endif // ADDLINE_H
