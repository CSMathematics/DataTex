#include "graphicsbuilder.h"
#include "ui_graphicsbuilder.h"

GraphicsBuilder::GraphicsBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicsBuilder)
{
    ui->setupUi(this);
    QList<QStringList> pst_packages;// = SqlFunctions::GetRecordList("SELECT * FROM Pstricks_Packages WHERE rowid>4",DataTex::DataTeX_Settings);
    for (int i = 0; i < pst_packages.count(); ++i) {
        QTreeWidgetItem * item = new QTreeWidgetItem(pst_packages[i]);
        ui->PstTree->topLevelItem(3)->addChild(item);
    }
    for(QAbstractButton * bt:ui->GraphicsButtonGroup->buttons()){
        int page = abs(ui->GraphicsButtonGroup->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            ui->stackedWidget->setCurrentIndex(page);
        });
    }
    GraphicsView = new QPdfViewer(this);
    ui->splitter->addWidget(GraphicsView);
    GraphicsView->show();
//    GraphicsView->hideToolBar();

    ui->TikzButton->setChecked(true);
    ui->FigureSourceCode->toolBar->Save->setVisible(false);
}

GraphicsBuilder::~GraphicsBuilder()
{
    delete ui;
}

void GraphicsBuilder::autoCompile()
{
    delete ui;
}
