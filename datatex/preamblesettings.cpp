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
#include <QtCore5Compat/QRegExp>


PreambleSettings::PreambleSettings(QWidget *parent,QString PreambleContent) :
    QDialog(parent),
    ui(new Ui::PreambleSettings)
{
    ui->setupUi(this);
    Packages = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Texlive_Packages",DataTex::DataTeX_Settings);
    Descriptions = SqlFunctions::Get_StringList_From_Query("SELECT Description FROM Texlive_Packages",DataTex::DataTeX_Settings);
    filter = new QSortFilterProxyModel(this);
    model = new QStringListModel(Packages,this);
    filter->setSourceModel(model);
    ui->PackageList->setModel(filter);
    for (int i=0;i<Packages.count();i++) {
        MapDescriptions.insert(Packages[i],Descriptions[i]);
    }
    connect(ui->PackageFilter,&QLineEdit::textChanged,this,[=](QString text){

        // filter->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,
        //                                             QRegExp::FixedString));

        filter->setFilterFixedString(text);
        filter->setFilterKeyColumn(0);
    });
    connect(ui->PackageList->selectionModel(), &QItemSelectionModel::selectionChanged,this, &PreambleSettings::PackageList_SelectionChanged);
    ui->splitter->setSizes(QList<int>({1, 200}));
    ui->PreambleContentWidget->editor->setText("\\documentclass[a4paper,10pt]{<Select document type>}\n");
    QTextCursor cursor = ui->PreambleContentWidget->editor->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->PreambleContentWidget->editor->setTextCursor(cursor);
    ui->OkButton->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->NameLine,&QLineEdit::textChanged,this,&PreambleSettings::EnableAccept);
    connect(ui->CodeLine,&QLineEdit::textChanged,this,&PreambleSettings::EnableAccept);
    ui->PackageList->setCurrentIndex(ui->PackageList->model()->index(0,0));

    QStringList babelLanguages = {"arabic","czech","danish","dutch","english","finnish","french",
                                  "ngerman","naustrian","greek","icelandic","italian","magyar","polish","portuguese",
                                  "russian","slovak","spanish","swedish"};
    QStandardItemModel languagesModel(babelLanguages.count(), 1);
    for (int r = 0; r < babelLanguages.count(); ++r)
    {
        ui->BabelList->addItem(babelLanguages[r]);
        ui->BabelList->item(r)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        ui->BabelList->item(r)->setCheckState(Qt::Unchecked);
    }

    connect(ui->BabelList, &QListWidget::itemClicked,this,[&](QListWidgetItem *item){
        getLanguage();
        if(item->checkState()==Qt::Checked){
            Languages.append(item->text());
        }
        else{
            Languages.removeAll(item->text());
        }
        LanguageLine = "\\usepackage["+Languages.join(",")+"]{babel}";
        QString text = ui->PreambleContentWidget->editor->toPlainText();
        text.replace(temp_LanguageLine,LanguageLine);
        ui->PreambleContentWidget->editor->setText(text);
        getLanguage();
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
    ui->BabelList->setEnabled(false);
    connect(ui->InputCheck,&QCheckBox::toggled,this,[&](bool checked){
        ui->InputCombo->setEnabled(checked);
    });
    connect(ui->FontCheck,&QCheckBox::toggled,this,[&](bool checked){
        ui->FontEncCombo->setEnabled(checked);
    });
    connect(ui->BabelPolyglossiaCheckBox,&QCheckBox::toggled,this,[&](bool checked){
        ui->BabelList->setEnabled(checked);
        if(!getLanguage()){
            ui->PreambleContentWidget->editor->insertPlainText("\\usepackage["+Languages.join(",")+"]{babel}\n");
        }
        else{
            QString text = ui->PreambleContentWidget->editor->toPlainText();
            text.remove(temp_LanguageLine);
            ui->PreambleContentWidget->editor->setText(text);
        }
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

    ui->TamplateTree->setColumnHidden(1,true);
    QSqlQuery getTemplates(DataTex::DataTeX_Settings);
    getTemplates.exec("SELECT Name,Preamble_Content,BuiltIn FROM Preambles");
    while(getTemplates.next()){
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0,getTemplates.value(0).toString());
        item->setText(1,getTemplates.value(1).toString());
        ui->TamplateTree->topLevelItem(1-getTemplates.value(2).toInt())->addChild(item);
    }
    ui->TamplateTree->expandAll();
    connect(ui->TamplateTree,&QTreeWidget::itemClicked,this,[&](QTreeWidgetItem * item){
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
    // ui->splitter->setSizes(QList<int>{0.25*size().width(),0.75*size().width()});
    ui->splitter->setSizes(QList<int>{qFloor(0.25*size().width()),qFloor(0.75*size().width())});

    ui->PreambleContentWidget->toolBar->Save->setVisible(false);
    if(!PreambleContent.isEmpty() /*&& notexists*/){
        ui->PreambleContentWidget->editor->setText(PreambleContent);
    }
}

PreambleSettings::~PreambleSettings()
{
    delete ui;
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

void PreambleSettings::PackageList_SelectionChanged()
{
    QItemSelectionModel *select = ui->PackageList->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    if(row>-1){
        ui->PackageDescription->setText(MapDescriptions[ui->PackageList->model()->index(row,0).data(Qt::DisplayRole).toString()]);
    }
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
                // if(c!="]"){
                if(c!=']'){
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

bool PreambleSettings::getLanguage()
{
    bool useLang;
    temp_LanguageLine.clear();
    QString text = ui->PreambleContentWidget->editor->toPlainText();
    QTextStream stream(&text);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        useLang = line.contains("{babel}");
        if(useLang){
            temp_LanguageLine = line;
            break;
        }
    }
    return useLang;
}
