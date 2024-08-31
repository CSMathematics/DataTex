#include "combolist.h"

ComboList::ComboList(QWidget *parent)
    : QComboBox{parent}
{
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
}

void ComboList::setItemModel(QList<QStringList> list,int i)
{
    itemModel = new QStandardItemModel(list.count(), 1);
    for (int r = 0; r < list.count(); ++r)
    {
        QStandardItem* item = new QStandardItem(list.at(r)[i]);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setData(list.at(r), Qt::UserRole);
        itemModel->setItem(r, 0, item);
    }
    setModel(itemModel);
    connect(this->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)), this, SLOT(itemCheck(QModelIndex, QModelIndex)));
}

void ComboList::itemCheck(QModelIndex a, QModelIndex b)
{
    QModelIndex cycleIndex;
    if (a.data(Qt::CheckStateRole).toBool())
    {
        cycleIndex = model()->index(a.row(), 0);
        model()->setData(cycleIndex, Qt::Checked, Qt::CheckStateRole);
        datalist.append(a.data(Qt::UserRole).toStringList());
    }
    else{
        cycleIndex = model()->index(a.row(), 0);
        model()->setData(cycleIndex, Qt::Unchecked, Qt::CheckStateRole);
        datalist.removeAll(a.data(Qt::UserRole).toStringList());
    }
}

void ComboList::addItemToModel(QString text, QStringList list)
{
    QFont bold;
    bold.setBold(true);
    QStandardItem* item = new QStandardItem(QString(text + " (Default value)"));
    item->setFont(bold);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    item->setCheckState(Qt::Unchecked);
    item->setData(QVariant::fromValue(list), Qt::UserRole);
    itemModel->insertRow(0,item);
    setItemData(0, QBrush(QColor(110,190,90)), Qt::BackgroundRole);
}

void ComboList::disableItems(bool checked,int excludeItems)
{
    for(int i=excludeItems;i<itemModel->rowCount();i++){
        itemModel->item(i)->setFlags(checked ? itemModel->item(i)->flags() & ~Qt::ItemIsEnabled
                                             : itemModel->item(i)->flags() | Qt::ItemIsEnabled);
    }
}

ComboList::~ComboList()
{
    delete itemModel;
}

QModelIndexList ComboList::checkedIndexes()const
{
    return model()->match(model()->index(0, 0), Qt::CheckStateRole, Qt::Checked, -1, Qt::MatchRecursive);
}

// returns a list with all the unchecked indexes
QModelIndexList ComboList::uncheckedIndexes()const
{
    return model()->match(model()->index(0, 0), Qt::CheckStateRole, Qt::Unchecked, -1, Qt::MatchRecursive);
}

//return true if all the items are checked
bool ComboList::allChecked()const
{
    return (uncheckedIndexes().count() == 0);
}

//return true if all the items are unchecked
bool ComboList::noneChecked()const
{
    return (checkedIndexes().count() == 0);
}

void ComboList::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    this->initStyleOption(&opt);
    if (noneChecked()){
        opt.currentText = tr("None");//Καμία επιλογή
    }
    //some are checked, some are not
    else{
        QStringList textlist = QStringList();
        for (int i = 0; i < datalist.count(); ++i) {
            textlist.append(datalist[i][1]);
        }
        opt.currentText = textlist.join("|");
    }

    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
