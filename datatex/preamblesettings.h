#ifndef PREAMBLESETTINGS_H
#define PREAMBLESETTINGS_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QRegularExpression>
#include "latexeditorwidget.h"
#include "switch.h"

class DeleteItemButton : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteItemButton(QWidget *parent = nullptr, QListWidgetItem *Item = nullptr);
    ~DeleteItemButton();

private:
    QPushButton *deleteButton;

signals:
    void itemDeleted();
};

namespace Ui {
class PreambleSettings;
}

class CustomFilterModel : public QSortFilterProxyModel {
    Q_OBJECT

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        // Apply the filter only to child items
        if (sourceParent.isValid()) {
            return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
        }
        // Parent items are always displayed
       return true;

        QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);

        if (sourceModel()->hasChildren(sourceIndex)) {
            // Check if any of the children are accepted
            int childCount = sourceModel()->rowCount(sourceIndex);
            for (int i = 0; i < childCount; i++) {
                QModelIndex childIndex = sourceModel()->index(i, 0, sourceIndex);
                if (filterAcceptsRow(childIndex.row(), childIndex.parent()))
                    return true;
            }
        }

        return false;
    }

protected:
    QVariant data(const QModelIndex &index, int role) const override {
        if (role == Qt::DisplayRole) {
            if (index.parent().isValid()) {
                // For child items, return the base class behavior
                return QSortFilterProxyModel::data(index, role);
            } else {
                // For parent items, display the child count
                QModelIndex sourceIndex = mapToSource(index);
                int childCount = 0;
                for (int row = 0; row < sourceModel()->rowCount(sourceIndex); ++row) {
                    QModelIndex childIndex = sourceModel()->index(row, 0, sourceIndex);
                    if (filterAcceptsRow(row, sourceIndex)) {
                        childCount++;
                    }
                }
                return sourceIndex.data().toString()+ "    (" + QString::number(childCount)+ "/"+ QString::number(sourceModel()->rowCount(sourceIndex)) + " packages)";
            }
        }

        // For other roles, use the base class behavior
        return QSortFilterProxyModel::data(index, role);
    }
};


struct CTANPackage
{
    QString Id;
    QString Name;
    QString ShortDescription;
    QStringList Topics;
    QString Version;
    QString Date;
    QString CTANLink;
    QStringList SelectedOptions;
    QStringList Authors;
    QStringList Copyright;
    QString Licence;
    QString Miktex;
    QString Texlive;
    QStringList Descriptions;
    QStringList Documentations;
    QVector<int> index;


    // void clear(){
    //     name.clear();
    //     shortDescription.clear();
    //     longDescription.clear();
    //     Topics.clear();
    //     Licence.clear();
    //     Version.clear();
    //     CTANLink.clear();
    // }
};
Q_DECLARE_METATYPE(CTANPackage)

class PreambleSettings : public QDialog
{
    Q_OBJECT

public:
    explicit PreambleSettings(QWidget *parent = nullptr, QString preamble_content = QString());
    ~PreambleSettings();

public slots:

    void EditLine_DataTex(QStringList Line);

private slots:

    QStringList on_OkButton_accepted();
    QString PackageDescription(CTANPackage package);
    void on_OkButton_rejected();
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void on_PreambleContentWidget_cursorPositionChanged();
    void EnableAccept();
    void getClass();
    void PreviewPreamble();
    QString getColorValuesForPreamble(QString ColorType,QColor color);

private:
    Ui::PreambleSettings *ui;
    QString PreambleContent;
    CustomFilterModel * filter;
    QStringListModel * model;
    QHash<QString,QStringList> SelectedOptionsPerPackage;
    QList<QVector<int>> checkedIndexes;
    QString ClassLine;
    QString temp_ClassLine;
    QString docClass;
    QString classOptions;
    QStringList Languages;
    QSqlDatabase CTANPackages;

signals:
    void newpreamblesignal(QStringList);
    void itemDeleted();
};

#endif // PREAMBLESETTINGS_H
