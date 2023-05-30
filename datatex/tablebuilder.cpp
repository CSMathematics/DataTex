#include "tablebuilder.h"
#include "ui_tablebuilder.h"

TableBuilder::TableBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableBuilder)
{
    ui->setupUi(this);
    TablePreview = new QPdfViewer(this);
    ui->splitter->addWidget(TablePreview);
    TablePreview->show();

    borderlist << QString("|") << QString("||") << QString("") << QString("@{}");
    alignlist << "c" << "l" << "r"
              << QString("<SEP>")  << "p{3cm}" << ">{\\raggedright\\arraybackslash}p{3cm}" << ">{\\centering\\arraybackslash}p{3cm}" << ">{\\raggedleft\\arraybackslash}p{3cm}"
              << QString("<SEP>")  << "m{3cm}" << ">{\\raggedright\\arraybackslash}m{3cm}" << ">{\\centering\\arraybackslash}m{3cm}" << ">{\\raggedleft\\arraybackslash}m{3cm}"
              << QString("<SEP>")  << "b{3cm}" << ">{\\raggedright\\arraybackslash}p{3cm}" << ">{\\centering\\arraybackslash}b{3cm}" << ">{\\raggedleft\\arraybackslash}b{3cm}";

    alignlistLabels << QString("c") << QString("l") << QString("r")
                    << QString("<SEP>") << QString("j p{}") << QString("l p{}") << QString("c p{}") << QString("r p{}")
                    << QString("<SEP>")  << QString("j m{}") << QString("l m{}") << QString("c m{}") << QString("r m{}")
                    << QString("<SEP>")  << QString("j b{}") << QString("l b{}") << QString("c b{}") << QString("r b{}");


    connect( ui->RowsSpinBox, SIGNAL(valueChanged(int)),this, SLOT(NewRows(int)));
    connect( ui->ColumnsSpinBox, SIGNAL(valueChanged(int)),this, SLOT(NewColumns(int)));

    TableColumnData column;
    column.alignment=0;
    column.leftborder=0;
    TableRowData row;
    row.topborder=true;
    row.merge=false;
    row.mergefrom=1;
    row.mergeto=1;
    columnDataList.clear();
    rowDataList.clear();
    for ( int j = 0; j < 99; ++j )
    {
        column.header=tr("Column %1").arg(QString::number(j+1));
        columnDataList.append(column);
        rowDataList.append(row);
    }
    ui->TableSourceCode->editor->setText(getLatexText());
    connect(ui->TableEnvComboBox,&QComboBox::textActivated,this,[&](const QString environment){
        ui->TableSourceCode->editor->setText(getLatexText());
    });

//    RowCommands = new QToolButton(this);
//    RowCommands->setPopupMode(QToolButton::MenuButtonPopup);
//    ui->horizontalLayout_2->addWidget(RowCommands);
//    RowCommands->setMenu(RowsMenu);
//    RowCommands->setIconSize(QSize(20,20));
//    RowCommands->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    RowCommands->setDefaultAction(PdfLatex);
}

TableBuilder::~TableBuilder()
{
    delete ui;
}

void TableBuilder::UpdateTable()
{
    int nrows = ui->RowsSpinBox->value();
    int ncols = ui->ColumnsSpinBox->value();

    QStringList headerList;
    for ( int j=0;j<ncols;j++)
    {
        QString tag;
//        tag=borderlist.at(columnDataList.at(j).leftborder);
//        tag+=alignlistLabels.at(columnDataList.at(j).alignment);
        tag+=columnDataList[j].header;
        tag+=" : ("+alignlistLabels.at(columnDataList.at(j).alignment)+")";
        if (j<ncols) headerList.append(tag);
    }
//    tag+=borderlist.at(ui->comboBoxEndBorder->currentIndex());
    ui->tableWidget->setHorizontalHeaderLabels(headerList);

    /*QColor selBlendColor=QColor("#0000FF");
QColor spancolor = selBlendColor.dark( 140 );
spancolor.setAlphaF( 0.2 );*/
    QTableWidgetItem *item, *new_item;
    QString content;
    for ( int i=0;i<nrows;i++)
    {
        for ( int j=0;j<ncols;j++)
        {
            item=ui->tableWidget->item(i,j);
            if (item)
            {
                content=item->text();
                new_item=new QTableWidgetItem();
                new_item->setText(content);
            }
            else
            {
                new_item=new QTableWidgetItem();
                new_item->setText("");
            }
            if (alignlist.at(columnDataList.at(j).alignment).contains("l")) new_item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            else if (alignlist.at(columnDataList.at(j).alignment).contains("r")) new_item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            else new_item->setTextAlignment(Qt::AlignCenter);
//            if (ui->tableWidget->columnSpan(i,j)>1) ui->tableWidget->setSpan(i,j,1,1);
            /*    if ((rowDataList.at(i).merge) && (rowDataList.at(i).mergeto>rowDataList.at(i).mergefrom)&& (j>=rowDataList.at(i).mergefrom-1) && (j<=rowDataList.at(i).mergeto-1))
     {
     new_item->setBackgroundColor(spancolor);
     }*/
            ui->tableWidget->setItem(i,j,new_item);
        }
//        if ((rowDataList.at(i).merge) && (rowDataList.at(i).mergeto>rowDataList.at(i).mergefrom))
//        {
//            ui->tableWidget->setSpan(i,rowDataList.at(i).mergefrom-1,1,rowDataList.at(i).mergeto-rowDataList.at(i).mergefrom+1);
//        }
    }
}


QString TableBuilder::getLatexText()
{
    QString placeholder;//(0x2022);

    int nrows = ui->RowsSpinBox->value();
    int ncols = ui->ColumnsSpinBox->value();
    QString environment = ui->TableEnvComboBox->currentText();
    QString text = "\\begin{"+environment+"}{";
    for ( int j = 0; j < ncols; j++) {
        text += borderlist.at(columnDataList.at(j).leftborder);
        text += alignlist.at(columnDataList.at(j).alignment);
    }
//    text += borderlist.at(ui->comboBoxEndBorder->currentIndex());
    text += "}\n";
    QTableWidgetItem *item = nullptr;
    for ( int i = 0; i < nrows; i++) {
        if (rowDataList.at(i).topborder) text += "\\hline\n";
//        if (ui->checkBoxMargin->isChecked()) text += "\\rule[-1ex]{0pt}{2.5ex} ";
        if (rowDataList.at(i).merge && (rowDataList.at(i).mergeto > rowDataList.at(i).mergefrom)) {
            QString el = "";
            for ( int j = 0; j < ncols; j++) {
                item = ui->tableWidget->item(i, j);
                QString itemText = (item) ? textToLatex(item->text()) : "";
                if (j == rowDataList.at(i).mergefrom - 1) {
                    el += itemText;
                    text += "\\multicolumn{";
                    text += QString::number(rowDataList.at(i).mergeto - rowDataList.at(i).mergefrom + 1);
                    text += "}{";
                    if ((j == 0) && (columnDataList.at(j).leftborder < 2)) text += borderlist.at(columnDataList.at(j).leftborder);
                    if (columnDataList.at(j).alignment < 3) text += alignlist.at(columnDataList.at(j).alignment);
                    else text += "c";
                    if (rowDataList.at(i).mergeto == ncols) text += borderlist.at(0/*ui->comboBoxEndBorder->currentIndex()*/);
                    else text += borderlist.at(columnDataList.at(rowDataList.at(i).mergeto).leftborder);
                    text += "}{";
                } else if (j == rowDataList.at(i).mergeto - 1) {
                    el += itemText;
                    if (el.isEmpty()) el = placeholder;
                    text += el + "}";
                    if (j < ncols - 1) text += " & ";
                    else text += " \\\\\n";
                } else if ((j > rowDataList.at(i).mergefrom - 1) && (j < rowDataList.at(i).mergeto - 1)) {
                    el += itemText;
                } else {
                    if (itemText.isEmpty()) {
                        itemText = placeholder;
                    }
                    text += itemText;
                    if (j < ncols - 1) text += " & ";
                    else text += " \\\\\n";
                }

            }
        } else {
            for ( int j = 0; j < ncols - 1; j++) {
                item = ui->tableWidget->item(i, j);
                QString itemText = (item) ? textToLatex(item->text()) : "";
                if (itemText.isEmpty()) {
                    itemText = placeholder;
                }
                text += itemText + " & ";
            }
            item = ui->tableWidget->item(i, ncols - 1);
            QString itemText = (item) ? textToLatex(item->text()) : "";
            if (itemText.isEmpty()) {
                itemText = placeholder;
            }
            text += itemText + " \\\\\n";
        }
    }
//    if (ui->checkBoxBorderBottom->isChecked())
//        text += "\\hline\n\\end{tabular}";
//    else
        text += "\\end{"+environment+"}";
    return text;
}

/*!
 * Return a list of packages the given latex table code depends upon.
 */
QStringList TableBuilder::getRequiredPackages(const QString &text)
{
    QStringList requiredPackages;
    if (text.contains("arraybackslash")) {
        requiredPackages << "array";
    }
    return requiredPackages;
}

void TableBuilder::NewRows(int num)
{
    ui->tableWidget->setRowCount( num );
//    ui->spinBoxNumLi->setRange(1,num);
    UpdateTable();
}

void TableBuilder::NewColumns(int num)
{
    ui->tableWidget->setColumnCount( num );
//    ui->spinBoxNumCol->setRange(1,num);
    UpdateTable();
}

void TableBuilder::updateSpanStatus(bool enabled)
{
//    ui->spinBoxSpanFrom->setEnabled(enabled);
//    ui->spinBoxSpanTo->setEnabled(enabled);
    UpdateTable();
}

void TableBuilder::applytoAllColumns()
{
//    TableColumnData col;
//    col.alignment=ui->comboBoxColAl->currentIndex();
//    col.leftborder=ui->comboLeftBorder->currentIndex();
//    for ( int i = 0; i < 99; ++i )
//    {
//        columnDataList.replace(i,col);
//    }
    UpdateTable();
}

void TableBuilder::applytoAllLines()
{
//    TableRowData li;
//    li.topborder=ui->checkBoxBorderTop->isChecked();
//    li.merge=ui->checkBoxSpan->isChecked();
//    li.mergefrom=ui->spinBoxSpanFrom->value();
//    li.mergeto=ui->spinBoxSpanTo->value();
//    if (li.mergefrom>li.mergeto)
//    {
//        li.mergefrom=1;
//        li.mergeto=1;
//    }
//    for ( int i = 0; i < 99; ++i )
//    {
//        rowDataList.replace(i,li);
//    }
    UpdateTable();
}

void TableBuilder::updateColSettings()
{
//    int i=ui->spinBoxNumCol->value()-1;
//    TableColumnData col;
//    col.alignment=ui->comboBoxColAl->currentIndex();
//    col.leftborder=ui->comboLeftBorder->currentIndex();
//    columnDataList.replace(i,col);
    UpdateTable();
    //qDebug() << "change" << i << columnDataList.at(i).alignment << columnDataList.at(i).leftborder;
}

void TableBuilder::updateRowSettings()
{
//    int i=ui->spinBoxNumLi->value()-1;
//    TableRowData li;
//    li.topborder=ui->checkBoxBorderTop->isChecked();
//    li.merge=ui->checkBoxSpan->isChecked();
//    li.mergefrom=ui->spinBoxSpanFrom->value();
//    li.mergeto=ui->spinBoxSpanTo->value();
//    if (li.mergefrom>li.mergeto)
//    {
//        li.mergefrom=1;
//        li.mergeto=1;
//    }
//    rowDataList.replace(i,li);
//    UpdateTable();
}

void TableBuilder::showColSettings(int column)
{
    int i=column-1;
    if (i>=99) return;
//    disconnect(ui->comboBoxColAl, SIGNAL(currentIndexChanged(int)),this, SLOT(updateColSettings()));
//    disconnect(ui->comboLeftBorder, SIGNAL(currentIndexChanged(int)),this, SLOT(updateColSettings()));
//    ui->comboBoxColAl->setCurrentIndex(columnDataList.at(i).alignment);
//    ui->comboLeftBorder->setCurrentIndex(columnDataList.at(i).leftborder);
//    connect(ui->comboBoxColAl, SIGNAL(currentIndexChanged(int)),this, SLOT(updateColSettings()));
//    connect(ui->comboLeftBorder, SIGNAL(currentIndexChanged(int)),this, SLOT(updateColSettings()));
    UpdateTable();
}

void TableBuilder::showRowSettings(int row)
{
    int i=row-1;
    if (i>=99) return;
//    disconnect(ui->checkBoxBorderTop, SIGNAL(toggled(bool)),this, SLOT(updateRowSettings()));
//    disconnect(ui->checkBoxSpan, SIGNAL(toggled(bool)),this, SLOT(updateRowSettings()));
//    disconnect(ui->spinBoxSpanFrom, SIGNAL(valueChanged(int)),this, SLOT(updateRowSettings()));
//    disconnect(ui->spinBoxSpanTo, SIGNAL(valueChanged(int)),this, SLOT(updateRowSettings()));
//    ui->checkBoxBorderTop->setChecked(rowDataList.at(i).topborder);
//    ui->checkBoxSpan->setChecked(rowDataList.at(i).merge);
//    ui->spinBoxSpanFrom->setValue(rowDataList.at(i).mergefrom);
//    ui->spinBoxSpanTo->setValue(rowDataList.at(i).mergeto);
//    connect(ui->checkBoxBorderTop, SIGNAL(toggled(bool)),this, SLOT(updateRowSettings()));
//    connect(ui->checkBoxSpan, SIGNAL(toggled(bool)),this, SLOT(updateRowSettings()));
//    connect(ui->spinBoxSpanFrom, SIGNAL(valueChanged(int)),this, SLOT(updateRowSettings()));
//    connect(ui->spinBoxSpanTo, SIGNAL(valueChanged(int)),this, SLOT(updateRowSettings()));
    UpdateTable();
}

void TableBuilder::showColRowSettings(int row,int column)
{
//    ui->spinBoxNumLi->setValue(row+1);
//    ui->spinBoxNumCol->setValue(column+1);
}

QString TableBuilder::textToLatex(const QString &text)
{
    QList<QPair<QString, QString> > replaceList;
    // replacements for resevered characters according to
    // http://en.wikibooks.org/wiki/LaTeX/Basics#Reserved_Characters
    replaceList.append(QPair<QString, QString> ("\\", "\\textbackslash{}"));
    replaceList.append(QPair<QString, QString> ("{", "\\{"));
    replaceList.append(QPair<QString, QString> ("}", "\\}"));
    replaceList.append(QPair<QString, QString> ("#", "\\#"));
    replaceList.append(QPair<QString, QString> ("$", "\\$"));
    replaceList.append(QPair<QString, QString> ("%", "\\%"));
    replaceList.append(QPair<QString, QString> ("&", "\\&"));
    replaceList.append(QPair<QString, QString> ("~", "\\~{}"));
    replaceList.append(QPair<QString, QString> ("_", "\\_"));
    replaceList.append(QPair<QString, QString> ("^", "\\^{}"));

    QString result = text;
    for (QList<QPair<QString, QString> >::const_iterator it = replaceList.begin(); it != replaceList.end(); ++it)
        result.replace(it->first, it->second);

    return result;
}
