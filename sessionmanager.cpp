#include "sessionmanager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>

SessionManager& SessionManager::instance()
{
    static SessionManager instance;
    return instance;
}

SessionManager::SessionManager()
{
    // Initialize databases or other members if necessary
    DataTeX_Settings = QSqlDatabase::addDatabase("QSQLITE", "DataTexSettings");
    Bibliography_Settings = QSqlDatabase::addDatabase("QSQLITE", "BibSettings");
}

SessionManager::~SessionManager()
{
}

void SessionManager::updateTableView(QTableView * table,QString QueryText,QSqlDatabase Database,QObject * parent)
{
    if(table->model()) table->model()->deleteLater();
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(table);
    QSqlQueryModel * model = new QSqlQueryModel(proxyModel);
    QSqlQuery query(Database);
    query.exec(QueryText);
    model->setQuery(query);
    proxyModel->setSourceModel(model);
    table->setModel(proxyModel);
    table->show();
    table->setSortingEnabled(true);
}

void SessionManager::LoadTableHeaders(QTableView * table,QStringList list)
{
    for (int i=0;i<list.count();i++) {
        table->model()->setHeaderData(i,Qt::Horizontal,list.at(i),Qt::DisplayRole);
    }
}

void SessionManager::StretchColumns(QTableView * Table, float stretchFactor)
{
    QFont myFont = Table->horizontalHeader()->font();
    QFontMetrics fm(myFont);
    for (int c = 0; c < Table->horizontalHeader()->count(); ++c)
    {
        QString str = Table->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Table->setColumnWidth(c,width);
    }
    Table->horizontalHeader()->setMinimumSectionSize(100);
}

void SessionManager::StretchColumns(QTreeView * Tree, float stretchFactor)
{
    QFont myFont = Tree->header()->font();
    QFontMetrics fm(myFont);
    for (int c = 0; c < Tree->model()->columnCount(); ++c)
    {
        QString str = Tree->model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString();
        int width=fm.horizontalAdvance(str)*stretchFactor;
        Tree->setColumnWidth(c,width);
    }
    Tree->header()->setMinimumSectionSize(100);
}

void SessionManager::StretchColumnsToWidth(QTableView * table)
{
    for (int c = 0; c < table->horizontalHeader()->count()-1; ++c)
    {
        table->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

QStringList SessionManager::GetListWidgetItems(QListWidget * list)
{
    QStringList items;
    for(int i=0;i<list->count();i++){
        items.append(list->item(i)->text());
    }
    return items;
}

void SessionManager::DBBackUp(QString database, QString dest_path)
{
    QFile file(database);
//    QProcess::execute("chmod",{"777",datatexpath});
    if (QFile::exists(dest_path))
    {
        QFile::remove(dest_path);
    }
    file.copy(dest_path);
//    QProcess::execute("chmod",{"555",datatexpath});
}

void SessionManager::runQuery_Root(QString queryText,QSqlDatabase database)
{
    /* Make dir writable for DataTex_Settings
     * QProcess::execute("chmod",{"777",datatexpath});
     * run query for DT and Bib databases
     * QProcess::execute("chmod",{"555",datatexpath});
     *  and back readable again */
//    QProcess::execute("chmod",{"777",datatexpath});
    QSqlQuery query(database);
    query.exec(queryText);
//    QProcess::execute("chmod",{"555",datatexpath});
}

QString SessionManager::getDataTexPath()
{
    return datatexpath;
}

bool SessionManager::SelectNewFileInModel(QTableView * table,QString newFile)
{
    QAbstractItemModel * m = table->model();
    QModelIndex ix = table->currentIndex();
    while (table->model()->canFetchMore(ix))
           table->model()->fetchMore(ix);
    QModelIndexList matchList = m->match(m->index(0,0), Qt::DisplayRole,
                               newFile, -1,  Qt::MatchFlags(Qt::MatchContains|Qt::MatchWrap));

    bool fileFound = matchList.count()>=1;
    if(matchList.count()>=1){
        table->setCurrentIndex(matchList.first());
        table->scrollTo(matchList.first());
    }
    return fileFound;
}

void SessionManager::FunctionInProgress()
{
    QMessageBox msgBox;
    msgBox.setText("This function will soon be completed.");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}
