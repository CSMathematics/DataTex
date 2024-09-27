#include "bibentry.h"
#include "ui_bibentry.h"
#include <QDebug>
#include <QTabWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "qsqlquery.h"
#include "adddatabasefield.h"
#include "datatex.h"
#include "sqlfunctions.h"
#include "addfolder.h"


BibEntry::BibEntry(QWidget *parent, bool EditMode, bool ImportMode, QHash<QString,QString> values) :
    QDialog(parent),
    ui(new Ui::BibEntry)
{
    ui->setupUi(this);
    isEditMode = EditMode;
    isImportMode = ImportMode;
    editValues = values;
    for (int i=0;i<DataTex::DocTypesIds.count();i++) {
        ui->DocumentTypeCombo->addItem(DataTex::DocTypesNames[i],QVariant(DataTex::DocTypesIds[i]));
    }
    OptBibFields;// = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Bibliography_Fields WHERE Basic = '0'",DataTex::Bibliography_Settings);
    citationkeys;// = SqlFunctions::Get_StringList_From_Query("SELECT Citation_Key FROM Bibliography",DataTex::Bibliography_Settings);
    connect(ui->AddBibDocTypeButton,&QPushButton::clicked,this,[&](){
        AddDatabaseField * newData = new AddDatabaseField(this);
        connect(newData,&AddDatabaseField::newline,this,[=](QStringList Line){
            // QSqlQuery AddField(DataTex::Bibliography_Settings);
            // AddField.exec(QString("INSERT OR IGNORE INTO DocumentTypes (Id,Name,Basic) "
            //                       "VALUES (\"%1\",\"%2\",\"%3\")").arg(Line[1],Line[0],"'0'"));
            // ui->DocumentTypeCombo->addItem(Line[1],QVariant("@"+Line[0]));
        });
    });
    QRegularExpression pk("[0-9]{4}");
    QRegularExpressionValidator * validator = new QRegularExpressionValidator(pk);
    ui->YearLine->setValidator(validator);
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,[&](int i){
        if(i==3){BibSourceCode();}
    });
    connect(ui->DocumentTypeCombo,SIGNAL(activated(int)),this,SLOT(BibSourceCode()));
    connect(ui->TitleLine,&QLineEdit::textChanged,this,[&](){
        BibSourceCode();
    });

    ui->AuthorsCombo->addItem(tr("Select an author"));
    ui->EditorCombo->addItem(tr("Select an editor"));
    ui->TranslatorCombo->addItem(tr("Select a translator"));

    authors;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Authors",DataTex::Bibliography_Settings);
    editors;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Editors",DataTex::Bibliography_Settings);
    translators;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Translators",DataTex::Bibliography_Settings);
    ui->AuthorsCombo->addItems(authors);
    ui->EditorCombo->addItems(editors);
    ui->TranslatorCombo->addItems(translators);

    ui->AddAuthorButton->setEnabled(false);
    ui->AddEditorButton->setEnabled(false);
    ui->AddTranslatorButton->setEnabled(false);
    ui->RemoveAuthorButton->setEnabled(false);
    ui->RemoveEditorButton->setEnabled(false);
    ui->RemoveTranslatorButton->setEnabled(false);
    ui->DeleteAuthorButton->setEnabled(false);
    ui->DeleteEditorButton->setEnabled(false);
    ui->DeleteTranslatorButton->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->NewAuthorButton->setEnabled(false);
    ui->NewEditorButton->setEnabled(false);
    ui->NewTranslatorButton->setEnabled(false);
    BibEntryExists(ui->CitationKeyLine->text());

    connect(ui->CitationKeyLine,&QLineEdit::textChanged,this,[&](QString text){
        BibSourceCode();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty() && !text.isNull()
                                                                && !citationkeys.contains(text));
        ui->warningLabel->setText((citationkeys.contains(text)) ? tr("Entry \"%1\" aldeady exists").arg(text) : "");
        BibEntryExists(text);
    });
    connect(ui->AuthorsCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->AddAuthorButton->setEnabled(!ui->AuthorsCombo->currentText().isEmpty()
                                        && !ui->AuthorsCombo->currentText().isNull()
                                        && (ui->AuthorsCombo->currentIndex())>0
                                        && !DataTex::GetListWidgetItems(ui->AuthorsList).contains(text));
        ui->DeleteAuthorButton->setEnabled(ui->AuthorsCombo->currentIndex()>0);
    });
    connect(ui->EditorCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->AddEditorButton->setEnabled(!ui->EditorCombo->currentText().isEmpty()
                                        && !ui->EditorCombo->currentText().isNull()
                                        && (ui->EditorCombo->currentIndex())>0
                                        && !DataTex::GetListWidgetItems(ui->EditorsList).contains(text));
        ui->DeleteEditorButton->setEnabled(ui->EditorCombo->currentIndex()>0);
    });
    connect(ui->TranslatorCombo,&QComboBox::currentTextChanged,this,[=](QString text){
        ui->AddTranslatorButton->setEnabled(!ui->TranslatorCombo->currentText().isEmpty()
                                            && !ui->TranslatorCombo->currentText().isNull()
                                            && (ui->TranslatorCombo->currentIndex())>0
                                            && !DataTex::GetListWidgetItems(ui->TranslatorList).contains(text));
        ui->DeleteTranslatorButton->setEnabled(ui->TranslatorCombo->currentIndex()>0);
    });

    connect(ui->NewAuthorLine,&QLineEdit::textChanged,this,[=](QString text){
        ui->NewAuthorButton->setEnabled(!authors.contains(text) && !text.isEmpty());
        ui->AuthorWarningLabel->setText((authors.contains(text)) ? tr("Author \"%1\" already exists.").arg(text) : "");
    });
    connect(ui->NewEditorLine,&QLineEdit::textChanged,this,[=](QString text){
        ui->NewEditorButton->setEnabled(!editors.contains(text) && !text.isEmpty());
        ui->EditorWarningLabel->setText((editors.contains(text)) ? tr("Editor \"%1\" already exists.").arg(text) : "");
    });
    connect(ui->NewTranslatorLine,&QLineEdit::textChanged,this,[=](QString text){
        ui->NewTranslatorButton->setEnabled(!translators.contains(text) && !text.isEmpty());
        ui->TranslatorWarningLabel->setText((translators.contains(text)) ? tr("Translator \"%1\" already exists.").arg(text) : "");
    });

    connect(ui->AuthorsList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->RemoveAuthorButton->setEnabled(ui->AuthorsList->selectionModel()->hasSelection());
    });
    connect(ui->EditorsList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->RemoveEditorButton->setEnabled(ui->EditorsList->selectionModel()->hasSelection());
    });
    connect(ui->TranslatorList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->RemoveTranslatorButton->setEnabled(ui->TranslatorList->selectionModel()->hasSelection());
    });


    connect(ui->AddAuthorButton,&QPushButton::clicked,this,[&](){
        ui->AuthorsList->addItem(ui->AuthorsCombo->currentText());
        ui->AddAuthorButton->setEnabled(false);
    });
    connect(ui->AddEditorButton,&QPushButton::clicked,this,[&](){
        ui->EditorsList->addItem(ui->EditorCombo->currentText());
        ui->AddEditorButton->setEnabled(false);
    });
    connect(ui->AddTranslatorButton,&QPushButton::clicked,this,[&](){
        ui->TranslatorList->addItem(ui->TranslatorCombo->currentText());
        ui->AddTranslatorButton->setEnabled(false);
    });

    connect(ui->RemoveAuthorButton,&QPushButton::clicked,this,[&](){
        ui->AuthorsList->takeItem(ui->AuthorsList->currentRow());
        ui->RemoveAuthorButton->setEnabled(ui->AuthorsList->count()>0);
    });
    connect(ui->RemoveEditorButton,&QPushButton::clicked,this,[&](){
        ui->EditorsList->takeItem(ui->EditorsList->currentRow());
        ui->RemoveEditorButton->setEnabled(ui->EditorsList->count()>0);
    });
    connect(ui->RemoveTranslatorButton,&QPushButton::clicked,this,[&](){
        ui->TranslatorList->takeItem(ui->TranslatorList->currentRow());
        ui->RemoveTranslatorButton->setEnabled(ui->TranslatorList->count()>0);
    });

    connect(ui->NewAuthorButton,&QPushButton::clicked,this,[=](){
        QString text = ui->NewAuthorLine->text();
        // QSqlQuery addAuthor(DataTex::Bibliography_Settings);
        // addAuthor.exec(QString("INSERT OR IGNORE INTO Authors (FullName) VALUES ('%1')").arg(text));
        // ui->AuthorsCombo->addItem(text);
        // ui->AuthorsCombo->setCurrentIndex(ui->AuthorsCombo->count()-1);
        // authors.append(text);
        // ui->NewAuthorButton->setEnabled(false);
    });
    connect(ui->NewEditorButton,&QPushButton::clicked,this,[=](){
        QString text = ui->NewEditorButton->text();
        // QSqlQuery addEditor(DataTex::Bibliography_Settings);
        // addEditor.exec(QString("INSERT OR IGNORE INTO Editors (FullName) VALUES ('%1')").arg(text));
        // ui->EditorCombo->addItem(text);
        // ui->EditorCombo->setCurrentIndex(ui->EditorCombo->count()-1);
        // editors.append(text);
        // ui->NewEditorButton->setEnabled(false);
    });
    connect(ui->NewTranslatorButton,&QPushButton::clicked,this,[=](){
        QString text = ui->NewTranslatorLine->text();
        // QSqlQuery addTranslator(DataTex::Bibliography_Settings);
        // addTranslator.exec(QString("INSERT OR IGNORE INTO Translators (FullName) VALUES ('%1')").arg(text));
        // ui->TranslatorCombo->addItem(text);
        // ui->TranslatorCombo->setCurrentIndex(ui->TranslatorCombo->count()-1);
        // translators.append(text);
        // ui->NewTranslatorButton->setEnabled(false);
    });

    connect(ui->DeleteAuthorButton,&QPushButton::clicked,this,[=](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Authors WHERE FullName = \"%1\"").arg(ui->AuthorsCombo->currentText()));
        // authors.removeAll(ui->AuthorsCombo->currentText());
        // ui->AuthorsCombo->removeItem(ui->AuthorsCombo->currentIndex());
    });
    connect(ui->DeleteEditorButton,&QPushButton::clicked,this,[&](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Editors WHERE FullName = \"%1\"").arg(ui->EditorCombo->currentText()));
        // editors.removeAll(ui->EditorCombo->currentText());
        // ui->EditorCombo->removeItem(ui->EditorCombo->currentIndex());
    });
    connect(ui->DeleteTranslatorButton,&QPushButton::clicked,this,[&](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Translators WHERE FullName = \"%1\"").arg(ui->TranslatorCombo->currentText()));
        // translators.removeAll(ui->TranslatorCombo->currentText());
        // ui->TranslatorCombo->removeItem(ui->TranslatorCombo->currentIndex());
    });
    ui->CustomFieldsTable->setColumnCount(2);
    ui->CustomFieldsTable->setHorizontalHeaderLabels({tr("Id"),tr("Value")});
    DataTex::StretchColumnsToWidth(ui->CustomFieldsTable);
    if(isEditMode){BibEditMode();}
    if(isImportMode){BibEditMode();}
    ui->buttonBox->setHidden(isImportMode);
}

BibEntry::~BibEntry()
{
    delete ui;
}

void BibEntry::BibEditMode()
{
    ui->CitationKeyLine->setText(editValues["Citation_Key"]);
    ui->CitationKeyLine->setEnabled(!isEditMode);
    ui->DocumentTypeCombo->setCurrentText(editValues["Document_Type"]);
    ui->TitleLine->setText(editValues["title"]);
    if(!editValues["author"].isEmpty() && !editValues["author"].isNull()){
        ui->AuthorsList->addItems(editValues["author"].split(tr(" and ")));
    }
    if(!editValues["editor"].isEmpty() && !editValues["editor"].isNull()){
        ui->EditorsList->addItems(editValues["editor"].split(tr(" and ")));
    }
    if(!editValues["translator"].isEmpty() && !editValues["translator"].isNull()){
        ui->TranslatorList->addItems(editValues["translator"].split(tr(" and ")));
    }
    ui->PublisherLine->setText(editValues["publisher"]);
    ui->YearLine->setText(editValues["year"]);
    ui->MonthLine->setText(editValues["month"]);
    ui->ISBNLine->setText(editValues["isbn"]);
    ui->ISSNLine->setText(editValues["issn"]);
    ui->PagesLine->setText(editValues["pages"]);
    ui->SeriesLine->setText(editValues["series"]);
    ui->EditionLine->setText(editValues["edition"]);
    ui->ChapterLine->setText(editValues["chapter"]);
    ui->NumberLine->setText(editValues["number"]);
    ui->VolumeLine->setText(editValues["volume"]);
    ui->JournalLine->setText(editValues["journal"]);
    ui->InstitutionLine->setText(editValues["institution"]);
    ui->SchoolLine->setText(editValues["school"]);
    ui->IssueLine->setText(editValues["issue"]);
    ui->AddressLine->setText(editValues["address"]);
    ui->DOILine->setText(editValues["doi"]);
    ui->URLLine->setText(editValues["url"]);
    ui->LanguageLine->setText(editValues["language"]);
    ui->LocationLine->setText(editValues["location"]);
    ui->SubtitleLine->setText(editValues["subtitle"]);
    ui->OrganizationLine->setText(editValues["organization"]);
    ui->KeyLine->setText(editValues["key"]);
    ui->AbstractTextEdit->setText(editValues["abstract"]);
    ui->CrossReferenceCombo->setCurrentText(editValues["crossref"]);
    ui->NoteLine->setText(editValues["note"]);

    int i=-1;
    for(QString text:OptBibFields){
        if(!editValues[text].isEmpty() && ! editValues[text].isNull()){
            i++;
            ui->CustomFieldsTable->insertRow(i);
            ui->CustomFieldsTable->setItem(i,0 , new QTableWidgetItem(text));
            ui->CustomFieldsTable->setItem(i,1 , new QTableWidgetItem(editValues[text]));
        }
    }
}

void BibEntry::on_buttonBox_accepted()
{
    bibValues.clear();
    bibValues = getBibValues();

    if(!isEditMode){
        InsertValues(bibValues);
    }

    else{
        // QSqlQuery editBibEntry(DataTex::Bibliography_Settings);
        // QStringList list;
        // for(QString text:bibValues.keys()){
        //     list.append(text+"='"+bibValues[text]+"'");
        // }
        // editBibEntry.exec(QString("UPDATE Bibliography SET "+list.join(",")+
        //                           " WHERE Citation_Key = '"+ui->CitationKeyLine->text()+"'"));
    }
    accept();
}

QString BibEntry::BibSourceCode()
{
    ui->SourceCodeText->clear();
    QString sourceCode = ui->DocumentTypeCombo->currentData().toString()+"{"+
            ui->CitationKeyLine->text()+",\n";
    if(!ui->TitleLine->text().isEmpty() && !ui->TitleLine->text().isNull()){
            sourceCode += "\t title = {"+ui->TitleLine->text()+"}"+",\n";}
    if((!ui->AuthorsList->count())==0){
        sourceCode += "\t author = {"+DataTex::GetListWidgetItems(ui->AuthorsList).join(" and ")+"}"+",\n";}
    if((!ui->EditorsList->count())==0){
        sourceCode += "\t editor = {"+DataTex::GetListWidgetItems(ui->EditorsList).join(" and ")+"}"+",\n";}
    if((!ui->TranslatorList->count())==0){
        sourceCode += "\t translator = {"+DataTex::GetListWidgetItems(ui->TranslatorList).join(" and ")+"}"+",\n";}

    if(!ui->PublisherLine->text().isEmpty() && !ui->PublisherLine->text().isNull()){
            sourceCode += "\t publisher = {"+ui->PublisherLine->text()+"}"+",\n";}
    if(!ui->YearLine->text().isEmpty() && !ui->YearLine->text().isNull()){
            sourceCode += "\t year = {"+ui->YearLine->text()+"}"+",\n";}
    if(!ui->MonthLine->text().isEmpty() && !ui->MonthLine->text().isNull()){
            sourceCode += "\t month = {"+ui->MonthLine->text()+"}"+",\n";}
    if(!ui->ISBNLine->text().isEmpty() && !ui->ISBNLine->text().isNull() && ui->ISBNLine->hasAcceptableInput()){
            sourceCode += "\t isbn = {"+ui->ISBNLine->text()+"}"+",\n";}
    if(!ui->ISSNLine->text().isEmpty() && !ui->ISSNLine->text().isNull() && ui->ISSNLine->hasAcceptableInput()){
            sourceCode += "\t isbn = {"+ui->ISSNLine->text()+"}"+",\n";}
    if(!ui->PagesLine->text().isEmpty() && !ui->PagesLine->text().isNull()){
            sourceCode += "\t pages = {"+ui->PagesLine->text()+"}"+",\n";}
    if(!ui->SeriesLine->text().isEmpty() && !ui->SeriesLine->text().isNull()){
            sourceCode += "\t series = {"+ui->SeriesLine->text()+"}"+",\n";}
    if(!ui->EditionLine->text().isEmpty() && !ui->EditionLine->text().isNull()){
            sourceCode += "\t edition = {"+ui->EditionLine->text()+"}"+",\n";}
    if(!ui->ChapterLine->text().isEmpty() && !ui->ChapterLine->text().isNull()){
            sourceCode += "\t chapter = {"+ui->ChapterLine->text()+"}"+",\n";}
    if(!ui->NumberLine->text().isEmpty() && !ui->NumberLine->text().isNull()){
            sourceCode += "\t number = {"+ui->NumberLine->text()+"}"+",\n";}
    if(!ui->VolumeLine->text().isEmpty() && !ui->VolumeLine->text().isNull()){
            sourceCode += "\t volume = {"+ui->VolumeLine->text()+"}"+",\n";}
    if(!ui->JournalLine->text().isEmpty() && !ui->JournalLine->text().isNull()){
            sourceCode += "\t journal = {"+ui->JournalLine->text()+"}"+",\n";}
    if(!ui->InstitutionLine->text().isEmpty() && !ui->InstitutionLine->text().isNull()){
            sourceCode += "\t institution = {"+ui->InstitutionLine->text()+"}"+",\n";}
    if(!ui->SchoolLine->text().isEmpty() && !ui->SchoolLine->text().isNull()){
            sourceCode += "\t school = {"+ui->SchoolLine->text()+"}"+",\n";}
    if(!ui->IssueLine->text().isEmpty() && !ui->IssueLine->text().isNull()){
            sourceCode += "\t issue = {"+ui->IssueLine->text()+"}"+",\n";}
    if(!ui->AddressLine->text().isEmpty() && !ui->AddressLine->text().isNull()){
            sourceCode += "\t address = {"+ui->AddressLine->text()+"}"+",\n";}
    if(!ui->DOILine->text().isEmpty() && !ui->DOILine->text().isNull()){
            sourceCode += "\t doi = {"+ui->DOILine->text()+"}"+",\n";}
    if(!ui->URLLine->text().isEmpty() && !ui->URLLine->text().isNull()){
            sourceCode += "\t url = {"+ui->URLLine->text()+"}"+",\n";}
    if(!ui->LanguageLine->text().isEmpty() && !ui->LanguageLine->text().isNull()){
            sourceCode += "\t language = {"+ui->LanguageLine->text()+"}"+",\n";}
    if(!ui->LocationLine->text().isEmpty() && !ui->LocationLine->text().isNull()){
            sourceCode += "\t location = {"+ui->LocationLine->text()+"}"+",\n";}
    if(!ui->SubtitleLine->text().isEmpty() && !ui->SubtitleLine->text().isNull()){
            sourceCode += "\t subtitle = {"+ui->SubtitleLine->text()+"}"+",\n";}
    if(!ui->OrganizationLine->text().isEmpty() && !ui->OrganizationLine->text().isNull()){
            sourceCode += "\t organization = {"+ui->OrganizationLine->text()+"}"+",\n";}
    if(!ui->KeyLine->text().isEmpty() && !ui->KeyLine->text().isNull()){
            sourceCode += "\t key = {"+ui->KeyLine->text()+"}"+",\n";}
    if(!ui->AbstractTextEdit->toPlainText().isEmpty() && !ui->AbstractTextEdit->toPlainText().isNull()){
            sourceCode += "\t abstract = {"+ui->AbstractTextEdit->toPlainText()+"}"+",\n";}
    if(!ui->CrossReferenceCombo->currentText().isEmpty() && !ui->CrossReferenceCombo->currentText().isNull()){
            sourceCode += "\t crossref = {"+ui->CrossReferenceCombo->currentText()+"}"+",\n";}
    if(!ui->NoteLine->text().isEmpty() && !ui->NoteLine->text().isNull()){
            sourceCode += "\t note = {"+ui->NoteLine->text()+"}"+",\n";}
    sourceCode += "}\n\n";
    ui->SourceCodeText->setText(sourceCode);
    return sourceCode;
}

QHash<QString, QString> BibEntry::getBibValues()
{
    QHash<QString, QString> values;
    values.insert("Citation_Key",ui->CitationKeyLine->text());
    values.insert("Document_Type",ui->DocumentTypeCombo->currentData().toString());
    values.insert("title",ui->TitleLine->text());
    values.insert("publisher",ui->PublisherLine->text());
    values.insert("year",ui->YearLine->text());
    values.insert("month",ui->MonthLine->text());
    values.insert("isbn",ui->ISBNLine->text());
    values.insert("issn",ui->ISSNLine->text());
    values.insert("pages",ui->PagesLine->text());
    values.insert("series",ui->SeriesLine->text());
    values.insert("edition",ui->EditionLine->text());
    values.insert("chapter",ui->ChapterLine->text());
    values.insert("number",ui->NumberLine->text());
    values.insert("volume",ui->VolumeLine->text());
    values.insert("journal",ui->JournalLine->text());
    values.insert("institution",ui->InstitutionLine->text());
    values.insert("school",ui->SchoolLine->text());
    values.insert("issue",ui->IssueLine->text());
    values.insert("address",ui->AddressLine->text());
    values.insert("doi",ui->DOILine->text());
    values.insert("url",ui->URLLine->text());
    values.insert("language",ui->LanguageLine->text());
    values.insert("location",ui->LocationLine->text());
    values.insert("subtitle",ui->SubtitleLine->text());
    values.insert("organization",ui->OrganizationLine->text());
    values.insert("key",ui->KeyLine->text());
    values.insert("abstract",ui->AbstractTextEdit->toPlainText());
    values.insert("crossref",ui->CrossReferenceCombo->currentText());
    values.insert("note",ui->NoteLine->text());

    for (int i=0;i<ui->CustomFieldsTable->rowCount() ;i++ ) {
        values.insert(ui->CustomFieldsTable->item(i,0)->text(),ui->CustomFieldsTable->item(i,1)->text());
    }
    return values;
}

void BibEntry::InsertValues(QHash<QString,QString> values)
{
    // QSqlQuery writeBibEntry(DataTex::Bibliography_Settings);
    // writeBibEntry.exec(QString("INSERT INTO Bibliography ("+values.keys().join(",")+") VALUES (\""+
    //                            values.values().join("\",\"")+"\")"));
    // if(ui->AuthorsList->count()>0){
    //     for(QString author:DataTex::GetListWidgetItems(ui->AuthorsList)){
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Authors (FullName) "
    //                                    "VALUES (\""+author+"\")"));
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Authors_per_BibEntry (FullName,BibEntry_Id) "
    //                                    "VALUES (\""+author+"\",\""+ui->CitationKeyLine->text()+"\")"));
    //     }
    // }
    // if(ui->EditorsList->count()>0){
    //     for(QString editor:DataTex::GetListWidgetItems(ui->EditorsList)){
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Editors (FullName) "
    //                                    "VALUES (\""+editor+"\")"));
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Editors_per_BibEntry (FullName,BibEntry_Id) "
    //                                    "VALUES (\""+editor+"\",\""+ui->CitationKeyLine->text()+"\")"));
    //     }
    // }
    // if(ui->TranslatorList->count()>0){
    //     for(QString translator:DataTex::GetListWidgetItems(ui->TranslatorList)){
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Translators (FullName) "
    //                                    "VALUES (\""+translator+"\")"));
    //         writeBibEntry.exec(QString("INSERT OR IGNORE INTO Translators_per_BibEntry (FullName,BibEntry_Id) "
    //                                    "VALUES (\""+translator+"\",\""+ui->CitationKeyLine->text()+"\")"));
    //     }
    // }
    // QSqlQuery writeSourceCode(DataTex::Bibliography_Settings);
    // writeSourceCode.exec("INSERT INTO EntrySourceCode (BibId,SourceCode) VALUES "
    //                      "(\""+ui->CitationKeyLine->text()+"\",\""+BibSourceCode()+"\")");
}

bool BibEntry::BibEntryExists(QString text)
{
    emit citationExists(citationkeys.contains(text));
    return citationkeys.contains(text);
}
