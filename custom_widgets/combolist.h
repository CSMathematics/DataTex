#ifndef COMBOLIST_H
#define COMBOLIST_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSql>
#include <QTextStream>
#include <QFile>
#include <QtCore>
#include <QDebug>
#include <QtWidgets>
#include <QSettings>
#include <QComboBox>
#include <QList>

class ComboList : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboList(QWidget *parent = nullptr);
    ~ComboList();
    void setItemModel(QList<QStringList> list, int i = 1);
    void addItemToModel(QString text,QStringList list);
    void disableItems(bool checked, int excludeItems = 0);
    QList<QStringList> datalist;

private:
    QStandardItemModel *itemModel;

private slots:
    void itemCheck(QModelIndex a, QModelIndex b);

    QModelIndexList checkedIndexes()const;
    QModelIndexList uncheckedIndexes()const;
    bool allChecked()const;
    bool noneChecked()const;
    void paintEvent(QPaintEvent *);


signals:
    void dataChanged(QModelIndex, QModelIndex, QVector<int>);

};

#endif // COMBOLIST_H
