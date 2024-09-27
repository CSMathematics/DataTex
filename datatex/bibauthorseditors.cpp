#include "bibauthorseditors.h"
#include "ui_bibauthorseditors.h"
#include "qsqlquery.h"
#include "datatex.h"
#include "addfolder.h"
#include "sqlfunctions.h"

BibAuthorsEditors::BibAuthorsEditors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibAuthorsEditors)
{
    ui->setupUi(this);
    QStringList authors;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Authors",DataTex::Bibliography_Settings);
    QStringList editors;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Editors",DataTex::Bibliography_Settings);
    QStringList translators;// = SqlFunctions::Get_StringList_From_Query("SELECT * FROM Translators",DataTex::Bibliography_Settings);
    ui->AuthorsList->addItems(authors);
    ui->EditorsList->addItems(editors);
    ui->TranslatorList->addItems(translators);
    ui->DeleteAuthorButton->setEnabled(false);
    ui->DeleteEditorButton->setEnabled(false);
    ui->DeleteTranslatorButton->setEnabled(false);
    ui->NewAuthorButton->setEnabled(false);
    ui->NewEditorButton->setEnabled(false);
    ui->NewTranslatorButton->setEnabled(false);
    connect(ui->NewAuthorLine,&QLineEdit::textChanged,this,[=](QString text){
        bool pk = authors.contains(text);
        if(pk){
            ui->warningLabel->setText(tr("Author \"%1\" already exists.").arg(text));
        }
        else{
            ui->warningLabel->clear();
        }
        ui->NewAuthorButton->setEnabled(!ui->NewAuthorLine->text().isEmpty() && !pk);
    });
    connect(ui->NewEditorLine,&QLineEdit::textChanged,this,[=](QString text){
        bool pk = editors.contains(text);
        if(pk){
            ui->warningLabel->setText(tr("Editor \"%1\" already exists.").arg(text));
        }
        else{
            ui->warningLabel->clear();
        }
        ui->NewEditorButton->setEnabled(!ui->NewEditorLine->text().isEmpty() && !pk);
    });
    connect(ui->NewTranslatorLine,&QLineEdit::textChanged,this,[=](QString text){
        bool pk = translators.contains(text);
        if(pk){
            ui->warningLabel->setText(tr("Translator \"%1\" already exists.").arg(text));
        }
        else{
            ui->warningLabel->clear();
        }
        ui->NewTranslatorButton->setEnabled(!ui->NewTranslatorLine->text().isEmpty() && !pk);
    });
    connect(ui->NewAuthorButton,&QPushButton::clicked,this,[=](){
        QString text = ui->NewAuthorLine->text();
        // QSqlQuery addAuthor(DataTex::Bibliography_Settings);
        // addAuthor.exec(QString("INSERT OR IGNORE INTO Authors (FullName) VALUES ('%1')").arg(text));
        // if(!authors.contains(text)){
        //     ui->AuthorsList->addItem(text);
        //     ui->AuthorsList->setCurrentRow(ui->AuthorsList->count()-1);
        // }
    });
    connect(ui->NewEditorButton,&QPushButton::clicked,this,[=](){
        // QSqlQuery addEditor(DataTex::Bibliography_Settings);
        // QString text = ui->NewEditorLine->text();
        // addEditor.exec(QString("INSERT OR IGNORE INTO Editors (FullName) VALUES ('%1')").arg(text));
        // if(!editors.contains(text)){
        //     ui->EditorsList->addItem(text);
        //     ui->EditorsList->setCurrentRow(ui->EditorsList->count()-1);
        // }
    });
    connect(ui->NewTranslatorButton,&QPushButton::clicked,this,[=](){
        QString text = ui->NewTranslatorLine->text();
        // QSqlQuery addTranslator(DataTex::Bibliography_Settings);
        // addTranslator.exec(QString("INSERT OR IGNORE INTO Translators (FullName) VALUES ('%1')").arg(text));
        // if(!translators.contains(text)){
        //     ui->TranslatorList->addItem(text);
        //     ui->TranslatorList->setCurrentRow(ui->TranslatorList->count()-1);
        // }
    });

    connect(ui->AuthorsList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->DeleteAuthorButton->setEnabled(ui->AuthorsList->selectionModel()->hasSelection());
    });
    connect(ui->EditorsList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->DeleteEditorButton->setEnabled(ui->EditorsList->selectionModel()->hasSelection());
    });
    connect(ui->TranslatorList->selectionModel(), &QItemSelectionModel::selectionChanged,this,[&](){
        ui->DeleteTranslatorButton->setEnabled(ui->TranslatorList->selectionModel()->hasSelection());
    });
    connect(ui->DeleteAuthorButton,&QPushButton::clicked,this,[&](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Authors WHERE FullName = \"%1\"").arg(ui->AuthorsList->currentItem()->text()));
        // ui->AuthorsList->takeItem(ui->AuthorsList->currentRow());
    });
    connect(ui->DeleteEditorButton,&QPushButton::clicked,this,[&](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Editors WHERE FullName = \"%1\"").arg(ui->EditorsList->currentItem()->text()));
        // ui->EditorsList->takeItem(ui->EditorsList->currentRow());
    });
    connect(ui->DeleteTranslatorButton,&QPushButton::clicked,this,[&](){
        // QSqlQuery deleteQuery(DataTex::Bibliography_Settings);
        // deleteQuery.exec("PRAGMA foreign_keys = ON");
        // deleteQuery.exec(QString("DELETE FROM Translators WHERE FullName = \"%1\"").arg(ui->TranslatorList->currentItem()->text()));
        // ui->TranslatorList->takeItem(ui->TranslatorList->currentRow());
    });
}

BibAuthorsEditors::~BibAuthorsEditors()
{
    delete ui;
}
