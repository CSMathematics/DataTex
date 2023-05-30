#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <QDialog>
#include <QToolButton>
#include "qpdfviewer.h"

struct TableColumnData
{
    int alignment;
    int leftborder;
    QString header;
};

struct TableRowData
{
    bool topborder;
    bool merge;
    int mergefrom;
    int mergeto;
};

namespace Ui {
class TableBuilder;
}

class TableBuilder : public QDialog
{
    Q_OBJECT

public:
    explicit TableBuilder(QWidget *parent = nullptr);
    ~TableBuilder();

private:
    Ui::TableBuilder *ui;
    QPdfViewer *TablePreview;
    QList<TableColumnData> columnDataList;
    QList<TableRowData> rowDataList;
    QStringList alignlist;
    QStringList alignlistLabels;
    QStringList borderlist;

private slots:
    void UpdateTable();
    QString getLatexText();
    static QStringList getRequiredPackages(const QString &text);
    void NewRows(int num);
    void NewColumns(int num);
    void updateSpanStatus(bool enabled);
    void applytoAllColumns();
    void applytoAllLines();
    void updateColSettings();
    void updateRowSettings();
    void showColSettings(int column);
    void showRowSettings(int row);
    void showColRowSettings(int row,int column);
    QString textToLatex(const QString &text);
};

#endif // TABLEBUILDER_H
