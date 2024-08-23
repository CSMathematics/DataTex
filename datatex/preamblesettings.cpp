#include "preamblesettings.h"
#include "ui_preamblesettings.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QDesktopServices>
#include <QDebug>
#include <QModelIndex>
#include <QSqlQuery>
#include "sqlfunctions.h"
#include "datatex.h"
#include "QStandardItemModel"
#include <math.h>


DeleteItemButton::DeleteItemButton(QWidget * parent,QListWidgetItem * Item) :
    QWidget(parent)
{
    deleteButton = new QPushButton(this);
    QSpacerItem * spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addItem(spacer);
    layout->addWidget(deleteButton);
    setLayout(layout);

    deleteButton->setIcon(QIcon::fromTheme("EditDelete"));
    deleteButton->setStyleSheet("QPushButton {padding: 3px;border: 0px solid transparent;}"
                                "QPushButton:hover {border: 1px solid #d20000;padding: 3px;}\n"
                                "QPushButton:pressed {color: #ff0000;padding: 3px;background-color: #ff4000;}");
    deleteButton->setGeometry(0,0,20,20);
    deleteButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QObject::connect(deleteButton,&QPushButton::clicked,this,[=](){
        delete Item;
    });
}

DeleteItemButton::~DeleteItemButton()
{
    delete deleteButton;
}

PreambleSettings::PreambleSettings(QWidget *parent,QString preamble_content) :
    QDialog(parent),
    ui(new Ui::PreambleSettings)
{
    ui->setupUi(this);
    CTANPackages = QSqlDatabase::addDatabase("QSQLITE","CTANPackages");
    CTANPackages.setDatabaseName(DataTex::datatexpath+"CTANDatabase.db");
    CTANPackages.open();
    PreambleContent = preamble_content;
    foreach(QAbstractButton * bt,ui->TabButtonGroup->buttons()){
        int page = abs(ui->TabButtonGroup->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            ui->stackedWidget->setCurrentIndex(page);
        });
    }

    foreach(QAbstractButton * bt,ui->OddEvenHeaderGroup->buttons()){
        int page = abs(ui->OddEvenHeaderGroup->id(bt))-2;
        connect(bt,&QAbstractButton::clicked,this,[=](){
            ui->stackedWidget_2->setCurrentIndex(page);
        });
    }

    ui->splitter->setSizes(QList<int>({300,300}));
    ui->PreambleContentWidget->editor->setText("\\documentclass[a4paper,10pt]{article}\n");
    QTextCursor cursor = ui->PreambleContentWidget->editor->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->PreambleContentWidget->editor->setTextCursor(cursor);
    ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,&PreambleSettings::EnableAccept);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,&PreambleSettings::EnableAccept);
    ui->PackageList->setCurrentIndex(ui->PackageList->model()->index(0,0));

    QStringList babelLanguages = {"afrikaans","azerbaijani","basque","breton","bulgarian",
                                  "catalan",
                                  "croatian",
                                  "czech",
                                  "danish",
                                  "dutch",
                                  "english","USenglish", "american", "UKenglish", "british", "canadian", "australian", "newzealand",
                                  "esperanto",
                                  "estonian",
                                  "finnish",
                                  "french", "francais", "canadien", "acadian",
                                  "galician",
                                  "austrian", "german", "germanb", "ngerman", "naustrian",
                                  "greek", "polutonikogreek",
                                  "hebrew",
                                  "icelandic",
                                  "indonesian",
                                  "interlingua",
                                  "irish",
                                  "italian",
                                  "latin",
                                  "lowersorbian",
                                  "malay", "melayu",
                                  "samin",
                                  "norsk", "nynorsk",
                                  "polish",
                                  "portuguese", "brazilian",
                                  "romanian",
                                  "russian",
                                  "scottish",
                                  "spanish",
                                  "slovak",
                                  "slovene",
                                  "swedish",
                                  "serbian",
                                  "turkish",
                                  "ukrainian",
                                  "uppersorbian",
                                  "welsh"};
    QStandardItemModel languagesModel(babelLanguages.count(), 1);
    for (int r = 0; r < babelLanguages.count(); ++r)
    {
        ui->BabelList->addItem(babelLanguages[r]);
        ui->BabelList->item(r)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        ui->BabelList->item(r)->setCheckState(Qt::Unchecked);
    }

    connect(ui->BabelList, &QListWidget::itemClicked,this,[&](QListWidgetItem *item){
        if(item->checkState()==Qt::Checked){
            Languages.append(item->text());
        }
        else{
            Languages.removeAll(item->text());
        }
    });

    QStringList encodings = {"none","latin1","latin2","latin3","latin5","utf8","utf8x","ascii","decmulti",
                             "cp850","cp852","cp437","cp437de","cp865","applemac","next","ansinew",
                             "cp1252","cp1250"};
    ui->InputCombo->addItems(encodings);
    QStringList fontEncodings = {"OT1","OT2","OT3","OT4","OT5","OT6","T1","T2A","T2B","T2C","T3","T4","T5","T6"
                                 ,"T7","TS1","TS3","X2","OML","OMS","OMX","C..","E..","L..","LY1","LV1","LGR","PD1","PU","U","NONE"};
    ui->FontEncCombo->addItems(fontEncodings);
    QStringList polyglossia = {"afrikaans", "danish","hungarian", "marathi","slovenian",
                               "albanian", "divehi", "icelandic", "mongolian","sorbian",
                               "amharic","dutch","interlingua","nko","spanish",
                               "arabic","english","italian","norwegian","swedish",
                               "armenian","esperanto","japanese","occitan","syriac",
                               "asturian","estonian","kannada","persian","tamil",
                               "basque","finnish","khmer","piedmontese","telugu",
                               "belarusian","french","korean","polish","thai",
                               "bengali","friulian","kurdish","portuguese","tibetan",
                               "bosnian","gaelic","lao","romanian","turkish",
                               "breton","galician","latin","romansh","turkmen",
                               "bulgarian","georgian","latvian","russian","ukrainian",
                               "catalan","german","lithuanian","sami","urdu",
                               "coptic","greek","macedonian","sanskrit","uyghur",
                               "croatian","hebrew","malay","serbian","vietnamese"};
    connect(ui->UseTemplate,&QCheckBox::toggled,this,[&](bool checked){
        ui->stackedWidget->setCurrentIndex(checked);
    });
    ui->InputCombo->setEnabled(false);
    ui->FontEncCombo->setEnabled(false);
    connect(ui->InputCheck,&QCheckBox::toggled,this,[&](bool checked){
        ui->InputCombo->setEnabled(checked);
    });
    connect(ui->FontCheck,&QCheckBox::toggled,this,[&](bool checked){
        ui->FontEncCombo->setEnabled(checked);
    });

    getClass();
    connect(ui->ClassCombo,&QComboBox::currentTextChanged,this,[&](QString docclass){
        ClassLine.replace(docClass,docclass);
        QString text = ui->PreambleContentWidget->editor->toPlainText();
        text.replace(temp_ClassLine,ClassLine);
        ui->PreambleContentWidget->editor->setText(text);
        getClass();
    });
    connect(ui->FontSizeCombo,&QComboBox::currentTextChanged,this,[&](QString fontsize){
        ClassLine.replace(classOptions,ui->PaperSizeCombo->currentText()+","+fontsize);
        QString text = ui->PreambleContentWidget->editor->toPlainText();
        text.replace(temp_ClassLine,ClassLine);
        ui->PreambleContentWidget->editor->setText(text);
        getClass();
    });
    connect(ui->PaperSizeCombo,&QComboBox::currentTextChanged,this,[&](QString papersize){
        ClassLine.replace(classOptions,papersize+","+ui->FontSizeCombo->currentText());
        QString text = ui->PreambleContentWidget->editor->toPlainText();
        text.replace(temp_ClassLine,ClassLine);
        ui->PreambleContentWidget->editor->setText(text);
        getClass();
    });

    ui->TemplateTree->setColumnHidden(1,true);
    QSqlQuery getTemplates(DataTex::DataTeX_Settings);
    getTemplates.exec("SELECT Name,Preamble_Content,BuiltIn FROM Preambles");
    while(getTemplates.next()){
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0,getTemplates.value(0).toString());
        item->setText(1,getTemplates.value(1).toString());
        ui->TemplateTree->topLevelItem(1-getTemplates.value(2).toInt())->addChild(item);
    }
    ui->TemplateTree->expandAll();
    connect(ui->TemplateTree,&QTreeWidget::itemClicked,this,[&](QTreeWidgetItem * item){
        QMessageBox::StandardButton resBtn =
            QMessageBox::warning(this,tr(""),tr("The preamble content will be lost.\n Do you want to continue?"),
            QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if(resBtn == QMessageBox::Yes){
            ui->PreambleContentWidget->editor->setText(item->text(1));
        }
        else{
            return;
        }
    });
    ui->splitter_2->setSizes(QList<int>{0.7*size().width(),0.3*size().width()});
    ui->PreambleContentWidget->toolBar->Save->setVisible(false);
    if(!PreambleContent.isEmpty() /*&& notexists*/){
        ui->PreambleContentWidget->editor->setText(PreambleContent);
    }

    QList<QStringList> Topics = SqlFunctions::GetRecordList("SELECT TopicId,count(TopicId) FROM TopicsPerPackage "
                                                                 "GROUP BY 1 ORDER BY 2 DESC,1",CTANPackages);
    QStandardItemModel * TopicsModel = new QStandardItemModel;
    for(QStringList topic : Topics){
        QStandardItem *it = new QStandardItem(QString("%1").arg(topic.at(0)));
        TopicsModel->appendRow(it);
        // qDebug()<<it->text();
    }
    QSqlQuery query(CTANPackages);
    query.exec("SELECT * FROM DBView "/*"SELECT p.*,group_concat(tp.TopicId,','),group_concat(t.increasing_number-1),max(c) AS Topics FROM Packages p "
                "LEFT JOIN TopicsPerPackage tp ON p.Id = tp.PackageId "
                "LEFT JOIN (SELECT TopicId,count(TopicId) AS c,ROW_NUMBER() OVER (ORDER BY count(TopicId) DESC) AS increasing_number "
                "FROM TopicsPerPackage GROUP BY 1 ORDER BY 2 DESC) "
                "AS t ON t.TopicId = tp.TopicId "
                "GROUP BY p.Id HAVING Topics NOT NULL ORDER BY c DESC"*/);
    while(query.next()){
        QStringList indexes;
        CTANPackage package;
        package.Id = query.value(0).toString();
        package.Name = query.value(1).toString();
        package.ShortDescription = query.value(2).toString();
        package.Version = query.value(3).toString();
        package.Date = query.value(4).toString();
        package.CTANLink = query.value(14).toString();
        package.Miktex = query.value(5).toString();
        package.Texlive = query.value(5).toString();
        package.Topics = query.value(18).toString().split(",");
        indexes = query.value(19).toString().split(",");
        for (QString row : indexes) {
            QStandardItem *item = new QStandardItem;
            item->setText(package.Name);
            item->setData(QVariant::fromValue(package),Qt::UserRole);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            item->setCheckState(Qt::Unchecked);
            TopicsModel->item(row.toInt()-1,0)->appendRow(item);
            if(row.toInt()==TopicsModel->rowCount()){
                qDebug()<<(row.toInt()==TopicsModel->rowCount());
            }
        }
        // TopicsModel->item(query.at(),0)->sortChildren(0);
    }

    filter = new CustomFilterModel;
    TopicsModel->setHeaderData(0,Qt::Horizontal,tr("Topics - Packages"),Qt::DisplayRole);
    filter->setSourceModel(TopicsModel);
    ui->TopicList->setModel(filter);
    connect(ui->PackageFilter,&QLineEdit::textEdited,this,[=](QString text){
        filter->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,QRegExp::FixedString));
        filter->setFilterKeyColumn(0);
    });

    connect(ui->TopicList->model(),&QAbstractItemModel::dataChanged,this,[=](QModelIndex a, QModelIndex b)mutable{
        QString PackageName = a.data(Qt::DisplayRole).toString();
        if(a.data(Qt::CheckStateRole).toBool()){
            QTreeWidgetItem * newitem = new QTreeWidgetItem();
            newitem->setText(0,PackageName);
//            newitem->setData(0,Qt::UserRole,PackageDescription(a.data(Qt::UserRole).value<CTANPackage>()));
            newitem->setData(0,Qt::UserRole,QVariant::fromValue(QList<int>({a.parent().row(),a.row()})));

            QTreeWidgetItemIterator it(ui->PackageList);
            while (*it) {
                if ((*it)->text(0) == PackageName){
                    QSignalBlocker SignalBlocker(ui->TopicList->model());
                    qDebug()<<"Already checked";
                    ui->TopicList->model()->setData(a, Qt::Unchecked, Qt::CheckStateRole);
                    SignalBlocker.unblock();
                    return;
                }
                ++it;
            }
            ui->PackageList->addTopLevelItem(newitem);
            QList<QStringList> list = SqlFunctions::GetRecordList(QString("SELECT Option "
                                                "FROM PackageOptions WHERE PackageId = '%1'").arg(a.data(Qt::DisplayRole).toString()),CTANPackages);
            for(QStringList option : list){
                QTreeWidgetItem * child = new QTreeWidgetItem();
                child->setText(0,option[0]);
                child->setFlags(child->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
                child->setCheckState(0,Qt::Unchecked);
                ui->PackageList->topLevelItem(ui->PackageList->indexOfTopLevelItem(newitem))->addChild(child);
            }
        }
        else{
//            emit itemDeleted(packageName);
            delete ui->PackageList->findItems(a.data().toString(),Qt::MatchExactly,0).at(0);
        }
        ui->PackageList->resizeColumnToContents(0);
    });

    connect(ui->TopicList->selectionModel(),&QItemSelectionModel::selectionChanged,
            this,[&](QItemSelection selected, QItemSelection deselected){
        if(!selected.isEmpty() && selected.constFirst().parent().isValid()){
            QString text = PackageDescription(selected.indexes().at(0).data(Qt::UserRole).value<CTANPackage>());
            ui->PackageDescription->setMarkdown(text);
        }
    });

    int packagesChecked = 0;
    connect(ui->PackageList,&QTreeWidget::itemChanged,this,[=](QTreeWidgetItem * item,int column = 0)mutable{
        if(item->parent()){
            if(item->checkState(0) == Qt::Checked){
                SelectedOptionsPerPackage[item->parent()->text(0)].append(item->text(0));
            }
            else{
                SelectedOptionsPerPackage[item->parent()->text(0)].removeOne(item->text(0));
            }
        }
        else{
            packagesChecked += 2*static_cast<bool>(item->checkState(0))-1;
            ui->CleanButton->setEnabled(packagesChecked>0);
        }
        qDebug()<<packagesChecked;
    });

    connect(ui->PackageList->selectionModel(),&QItemSelectionModel::selectionChanged,this,[&](QItemSelection selected, QItemSelection deselected){
        if(!selected.isEmpty() && !selected.constFirst().parent().isValid()){
            QString text = selected.indexes().at(0).data(Qt::UserRole).toString();
            ui->PackageDescription->setMarkdown(text);
        }
    });
    ui->PackageList->setColumnWidth(0,500);
    connect(ui->PreviewPreamble,&QPushButton::clicked,this,[=](){PreviewPreamble();});

    connect(ui->CleanButton,&QPushButton::clicked,this,[=](){
        ui->PackageList->clear();
        SelectedOptionsPerPackage.clear();
        for(int row = 0;row < ui->TopicList->model()->rowCount();row++){
            QModelIndex index = ui->TopicList->model()->index(row,0).child(row,0);
            if(index.parent().isValid()){
                ui->TopicList->model()->setData(index, Qt::Unchecked, Qt::CheckStateRole);
            }
        }
    });

    connect(ui->SetHeightCheckBox,&QCheckBox::clicked,this,[=](bool checked){
        ui->HeightSpinBox->setEnabled(checked);
        ui->HeightUnitCombo->setEnabled(checked || ui->WidthSpinBox->isEnabled());
    });
    connect(ui->SetWidthCheckBox,&QCheckBox::clicked,this,[=](bool checked){
        ui->WidthSpinBox->setEnabled(checked);
        ui->HeightUnitCombo->setEnabled(checked || ui->HeightSpinBox->isEnabled());
    });
    connect(ui->SetMarginsCheckBox,&QGroupBox::clicked,this,[=](bool checked){
        ui->TopMarginSpinBox->setEnabled(checked);
        ui->BottomMarginSpinBox->setEnabled(checked);
        ui->LeftMarginSpinBox->setEnabled(checked);
        ui->RightMarginSpinBox->setEnabled(checked);
        ui->LockAnalogyButton->setEnabled(checked);
        ui->TopMarginUnitCombo->setEnabled(checked);
    });
    connect(ui->MoveUpButton,&QPushButton::clicked,this,[&](){
        int row = ui->PackageList->currentIndex().row();
        QTreeWidgetItem * item = ui->PackageList->takeTopLevelItem(row);
//        DeleteItemButton * widget = (DeleteItemButton *)ui->PackageList->itemWidget(item,1);
        ui->PackageList->insertTopLevelItem(row-1,item);
//        ui->PackageList->setItemWidget(item,1,widget);
        ui->PackageList->setCurrentItem(item);
    });
    ui->PackageList->header()->setStretchLastSection(true);
    connect(ui->DeletePackageButton,&QPushButton::clicked,this,[=](){
        for(QTreeWidgetItem * item : ui->PackageList->selectedItems()){
            QList<int> index = item->data(0,Qt::UserRole).value<QList<int>>();
//            if(index.parent().isValid()){
            QModelIndex idx = ui->TopicList->model()->index(index[0],0).child(index[1],0);
                ui->TopicList->model()->setData(idx, Qt::Unchecked, Qt::CheckStateRole);
//            delete item;
            qDebug()<<index<<idx;
//            }
        }
    });

    connect(ui->ChooseColor,&QPushButton::clicked,this,[=](){
        QColorDialog * cd = new QColorDialog(this);
        connect(cd,&QColorDialog::colorSelected,this,[=](QColor color){
//            ui->ChooseColor->setStyleSheet("background-color: "+color.name());
//            ui->ChooseColor->setProperty("color",QVariant::fromValue(color));

            QPixmap pixmap(20,20);
//            QColor color = ui->ChooseColor->property("color").value<QColor>();
            pixmap.fill(QColor(color.name()));
            QIcon Icon(pixmap);
            bool exists = false;
            QString colorName = ui->ColorNameLine->text();
            for(int row = 0;row<ui->ColorList->count();row++){
                if(ui->ColorList->item(row)->text()==colorName ||
                    color==ui->ColorList->item(row)->data(Qt::UserRole).value<QColor>()){
                    exists = true;
                    break;
                }
            }
            if(!exists){
                QListWidgetItem * item = new QListWidgetItem(Icon,colorName);
                item->setData(Qt::UserRole,QVariant::fromValue(color));
                item->setData(Qt::WhatsThisRole,ui->ColorTypeComboBox->currentText());
                ui->ColorList->addItem(item);
                DeleteItemButton * bt = new DeleteItemButton(this,item);
                ui->ColorList->setItemWidget(item,bt);
            }
        });
        cd->show();
        cd->activateWindow();
    });
    QRegExp reg("[A-Za-z0-9]{1,}");
    QRegExpValidator *validator = new QRegExpValidator(reg, this);
    ui->ColorNameLine->setValidator(validator);
    connect(ui->ColorNameLine,&QLineEdit::textChanged,this,[=](QString text){
        ui->ChooseColor->setEnabled(!text.isEmpty());
    });

    QTreeWidgetItemIterator iterator(ui->XcolorOptionsTree);
    while (*iterator) {
        if ((*iterator)->parent()){
            (*iterator)->setFlags((*iterator)->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            (*iterator)->setCheckState(0,Qt::Unchecked);
        }
        ++iterator;
    }
}

PreambleSettings::~PreambleSettings()
{
    delete ui;
}

QString PreambleSettings::PackageDescription(CTANPackage package)
{
    QString text = QString("- Name : %1\n"
                           "- Description : %2\n"
                           "- Long Description : %3\n"
                           "- Version : %4\n"
                           "- Topics : %5\n"
                           "- License : %6\n"
                           "- CTAN Link : %7").arg(package.Name,package.ShortDescription,package.Descriptions.at(0),
                            package.Version,package.Topics.join(","),package.Licence,package.CTANLink);
    return text;
}

void PreambleSettings::EditLine_DataTex(QStringList Line)
{
    ui->NameLine->setText(Line[0]);
    ui->CodeLine->setText(Line[1]);
}


QStringList PreambleSettings::on_OkButton_accepted()
{
    QStringList Line = {ui->NameLine->text(),ui->CodeLine->text(),ui->PreambleContentWidget->editor->toPlainText()};
    emit newpreamblesignal(Line);
    accept();
    return Line;
}

void PreambleSettings::on_OkButton_rejected()
{
    reject();
}

void PreambleSettings::on_AddButton_clicked()
{
    QString package = ui->PackageList->currentIndex().data().toString();
    ui->PreambleContentWidget->editor->insertPlainText("\\usepackage{"+package+"}\n");
}

void PreambleSettings::on_RemoveButton_clicked()
{
    QTextCursor cursor = ui->PreambleContentWidget->editor->textCursor();
    int row = cursor.blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, row);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    cursor.deleteChar();
    ui->PreambleContentWidget->editor->setTextCursor(cursor);
}

void PreambleSettings::on_PreambleContentWidget_cursorPositionChanged()
{
    QTextCursor cursor = ui->PreambleContentWidget->editor->textCursor();
    int row = cursor.blockNumber();
}

void PreambleSettings::EnableAccept()
{
    if(!ui->CodeLine->text().isEmpty() && !ui->NameLine->text().isEmpty()){
        ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else {ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);}
}

void PreambleSettings::getClass()
{
    ClassLine.clear();
    temp_ClassLine.clear();
    docClass.clear();
    classOptions.clear();
    QString text = ui->PreambleContentWidget->editor->toPlainText();
    QTextStream stream(&text);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.contains("\\documentclass")){
            ClassLine = line;
            temp_ClassLine = line;
            line.remove("\\documentclass[");
            int i=-1;
            QChar c;
            while(i<line.length()){
                i++;
                c = line[i];
                if(c!="]"){
                    classOptions.append(c);
                }
                else{
                    break;
                }
            }
            line.remove(classOptions);
            line.remove("]");
            docClass = line.remove(QRegularExpression("[{}]"));
            break;
        }
    }
}

void PreambleSettings::PreviewPreamble()
{
    PreambleContent = "\\documentclass";
    PreambleContent += "["+ui->PaperSizeCombo->currentText()+","+ui->FontSizeCombo->currentText()+"]";
    PreambleContent += "{"+ui->ClassCombo->currentText()+"}\n";
    if(ui->LanguageGroup->isChecked()){
        if(ui->BabelRadioButton->isChecked()){
            PreambleContent += "\\usepackage["+Languages.join(",")+"]{babel}\n";
        }
        else{

        }
    }
    if(ui->ColorGroupBox->isChecked()){
        PreambleContent += "\\usepackage{xcolor}\n";
        for(int i=0;i<ui->ColorList->count();i++){
            QColor color = ui->ColorList->item(i)->data(Qt::UserRole).value<QColor>();
            QString type = ui->ColorList->item(i)->data(Qt::WhatsThisRole).toString();
            PreambleContent += "\\definecolor{"+ui->ColorList->item(i)->text()+
                               "}{"+type+"}{"+
                               getColorValuesForPreamble(type,color)+"}\n";
        }
    }
    QStringList packagesWithoutSelectedOptions;
    for(int i = 0;i<ui->PackageList->topLevelItemCount();i++){
        QTreeWidgetItem * item = ui->PackageList->topLevelItem(i);
        //            CTANPackage package = item->data(0,Qt::UserRole).value<CTANPackage>();
        if(SelectedOptionsPerPackage.value(item->text(0)).isEmpty()){
            packagesWithoutSelectedOptions.append(item->text(0));
        }
        else{
            PreambleContent += "\\usepackage["+SelectedOptionsPerPackage.value(item->text(0)).join(",")+"]{"+item->text(0)+"}\n";
        }
    }
    PreambleContent += "\\usepackage{"+packagesWithoutSelectedOptions.join(",")+"}\n";
    ui->PreambleContentWidget->editor->setText(PreambleContent);
}

QString PreambleSettings::getColorValuesForPreamble(QString ColorType, QColor color)
{
    QString colorValues;
    if(ColorType=="rgb"){
        colorValues = QString::number(color.toRgb().redF())+","+
                      QString::number(color.toRgb().greenF())+","+
                      QString::number(color.toRgb().blueF());
    }
    else if(ColorType=="RGB"){
        colorValues = QString::number(color.toRgb().red())+","+
                      QString::number(color.toRgb().green())+","+
                      QString::number(color.toRgb().blue());
    }
    else if(ColorType=="cmyk"){
        colorValues = QString::number(color.toCmyk().cyanF())+","+
                      QString::number(color.toCmyk().magentaF())+","+
                      QString::number(color.toCmyk().yellowF())+","+
                      QString::number(color.toCmyk().blackF());
    }
    else if(ColorType=="cmy"){
        colorValues = QString::number(color.toCmyk().cyanF())+","+
                      QString::number(color.toCmyk().magentaF())+","+
                      QString::number(color.toCmyk().yellowF());
    }
    else if(ColorType=="HTML"){
        colorValues = color.name().remove("#").toUpper();
    }
    else if(ColorType=="hsb"){
        colorValues = QString::number(color.toHsv().hueF())+","+
                      QString::number(color.toHsv().saturationF())+","+
                      QString::number(color.toHsv().valueF());
    }
    else if(ColorType=="Hsb"){
        colorValues = QString::number(color.hue())+","+
                      QString::number(color.saturationF())+","+
                      QString::number(color.valueF());
    }
    else if(ColorType=="HSB"){
        colorValues = QString::number(color.hue())+","+
                      QString::number(color.saturation())+","+
                      QString::number(color.value());
    }
    else if(ColorType=="gray"){
        colorValues = QString::number(color.blackF());
    }
    else if(ColorType=="Gray"){
        colorValues = QString::number(color.black());
    }
    return colorValues;
}
