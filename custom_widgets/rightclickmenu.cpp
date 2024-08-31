#include "rightclickmenu.h"

RightClickMenu::RightClickMenu(QWidget *parent,QTableView * Table,int columnsHidden)
    : QWidget{parent}
{
    Table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    RightClick = new QDialog(this);
    QVBoxLayout * layout = new QVBoxLayout;
    FilesRightClickMenu = new QListWidget(RightClick);
    layout->addWidget(FilesRightClickMenu);
    layout->setContentsMargins(0,0,0,0);
    RightClick->setLayout(layout);
    RightClick->hide();
    FilesTableHiddenColumns.clear();
    FilesRightClickMenu->clear();
    FilesRightClickMenu->addItem(tr("Select all"));
    FilesRightClickMenu->item(0)->setData(Qt::UserRole,-1);
    FilesRightClickMenu->item(0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    FilesRightClickMenu->item(0)->setCheckState(Qt::Checked);
    for (int i = 0; i < Table->model()->columnCount()-columnsHidden;i++) {
        FilesTableHiddenColumns.push_back(i);
        FilesRightClickMenu->addItem(Table->model()->headerData(i, Qt::Horizontal,Qt::DisplayRole).toString());
        FilesRightClickMenu->item(i+1)->setData(Qt::UserRole,i);
        FilesRightClickMenu->item(i+1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        FilesRightClickMenu->item(i+1)->setCheckState(Qt::Checked);
    }
    connect(Table->horizontalHeader(),&QHeaderView::customContextMenuRequested, this,[=](QPoint point){
        QPoint globalpos=Table->mapToGlobal(point);
        RightClick->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        RightClick->setGeometry(QRect(globalpos, QSize(200,300)));
        RightClick->exec();
    });
    connect(FilesRightClickMenu, &QListWidget::itemClicked,this,[=](QListWidgetItem *item){
        int row = item->data(Qt::UserRole).toInt();
        if(row>-1){
            if(item->checkState()==Qt::Checked){
                FilesTableHiddenColumns.push_back(row);
            }
            else{
                FilesTableHiddenColumns.removeAll(row);
            }
            Table->setColumnHidden(row,!item->checkState());
        }
        else{
            for (int i = 0; i < Table->model()->columnCount()-columnsHidden; i++) {
                FilesRightClickMenu->item(i+1)->setCheckState(item->checkState());
                Table->setColumnHidden(i,!item->checkState());
            }
        }
    });
    //---
}
