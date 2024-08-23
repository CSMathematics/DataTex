#ifndef RIGHTCLICKMENU_H
#define RIGHTCLICKMENU_H

#include <QtWidgets>
#include <QWidget>
#include <QtCore>

class RightClickMenu : public QWidget
{
    Q_OBJECT
public:
    explicit RightClickMenu(QWidget *parent = nullptr,QTableView * Table = nullptr,int columnsHidden = 0);

private:
    QDialog * RightClick;
    QListWidget * FilesRightClickMenu;
    QVector<int> FilesTableHiddenColumns;

signals:

};

#endif // RIGHTCLICKMENU_H
