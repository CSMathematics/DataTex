#include "notesdocuments.h"
#include "ui_notesdocuments.h"
#include <QMessageBox>
#include <QCompleter>
#include <QDebug>
#include <QCloseEvent>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileSystemModel>
#include "addfolder.h"
#include "sqlfunctions.h"
#include "addfiletoeditor.h"
#include "csvfunctions.h"


NotesDocuments::NotesDocuments(QWidget *parent, bool editMode, QStringList metadata, QString fileName,
                               bool cloneMode, QString DocumentContent) :
    QDialog(parent),
    ui(new Ui::NotesDocuments)
{
    radiogroup = new QButtonGroup;
    currentbase = DataTex::CurrentNotesFolderDataBase;
    currentbase_Exercises = DataTex::CurrentTexFilesDataBase;
    NotesPath = QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath()+QDir::separator();
    DocContent = DocumentContent;
    CloneMode = cloneMode;
    DocumentPath = fileName;
    radioList.clear();
    ui->setupUi(this);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",currentbase);
    QStringList horzHeaders;
    horzHeaders << tr("File type") << tr("Name")<<"Path";
    QStringList horzHeaders2;
    horzHeaders2 << tr("File Id") << tr("Section")<<tr("Exercise type");
    QStringList Headers;
    ui->FoldersStructureView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->FoldersStructureView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Headers << tr("Folder name");
    ui->RemButton->setEnabled(false);
    ui->EditButton->setEnabled(false);
    ui->NameLine->setEnabled(false);
    ui->pathline->setEnabled(false);
    ui->OpenPath->setEnabled(false);
    ui->InsertFilesCheckBox->setEnabled(false);
    view = new PdfViewer(this);
    ui->verticalLayout_5->addWidget(view);
    view->show();

    DocumentTable = new ExtendedTableWidget(this);
    ui->verticalLayout_4->addWidget(DocumentTable);
    DocumentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentTable->horizontalHeader()->setSectionsClickable(true);
    if(!CloneMode){
        GetDocTypes();
        LoadFolderStructure();
        LoadDocTable();
    }

    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);
    ui->CreateSubFolder->setEnabled(false);
    ui->addButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
    level = -1;

    QStringList PreambleIds = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM Preambles ORDER BY ROWID",DataTex::DataTeX_Settings);
    QStringList PreambleNames = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM Preambles",DataTex::DataTeX_Settings);
    ui->BuildBox->addItems(DataTex::LatexCommands.keys());
    for (int i=0;i<PreambleIds.count();i++) {
        ui->PreambleBox->addItem(PreambleNames.at(i),QVariant(PreambleIds.at(i)));
    }
    ui->BuildBox->setCurrentText("PdfLaTeX");
    ui->BuildBox->setEnabled(false);
    ui->PreambleBox->setEnabled(false);
    ui->PreambleBox->setCurrentIndex(ui->PreambleBox->findData("Basic"));
    connect(ui->NameLine,&QLineEdit::textChanged,this,[=](){
        if(!ui->NameLine->text().isEmpty()){
            QString Content = "%# Database Document : "+ui->NameLine->text()+"-----------------\n";
            Content += "%@ Document type: "+DocumentType+"\n";
            Content += "%#--------------------------------------------------\n";
            Content += ClearContent(DocContent);
            ui->DocumentContent->setPlainText(Content);
        }
        else{
            ui->DocumentContent->clear();
        }
    });
//    qDebug()<<ClearContent("%# Database Document : Epanalipsi_meleth_synarthshs-----------------\n"
//                          "%@ Document type: Ασκήσεις\n"
//                          "%#--------------------------------------------------\n"
//                          "\\documentclass[11pt,a4paper]{article}\n"
//                          "\\usepackage[utf8]{inputenc}\n"
//                          "\\usepackage{nimbusserif}\n"
//                          "\\usepackage[T1]{fontenc}\n"
//                          "\\begin{document}\n"
//                          "\\begin{center}\n"
//                          "{\\Large\n"
//                          "\\begin{enumerate}\n"
//                          "%# Database File : Analysh-KyrtothtaMonotAkrot-CombEx1----\n"
//                          "\\item Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, \n"
//                          "τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. \n"
//                          "Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.\n"
//                          "\\begin{alist}\n"
//                          "\\item $ f(x)=\frac{x}{x^2+1} $\n"
//                          "\\end{document}");
    tagLine = new TagsLineEditWidget(this,SqlFunctions::Get_StringList_From_Query("SELECT * FROM CustomTags",currentbase));
    ui->horizontalLayout->addWidget(tagLine);
    tags = tagLine->GetTags();
    tagLine->setEnabled(false);
    DataTex::StretchColumnsToWidth(DocumentTable);
    ui->DatabaseCombo->addItem(tr("Select a database..."));
    for (int i=0;i<DataTex::GlobalDocsDatabaseList.count();i++) {
        QSqlDatabase database = DataTex::GlobalDocsDatabaseList.values().at(i);
        if(database.databaseName()!=DataTex::CurrentNotesFolderPath){
            ui->DatabaseCombo->addItem(DataTex::GlobalDocsDatabaseListNames[QFileInfo(database.databaseName()).baseName()],database.databaseName());
        }
    }
    ui->DatabaseCombo->setVisible(CloneMode);
    ui->DatabaseLabel->setVisible(CloneMode);
}

NotesDocuments::~NotesDocuments()
{
    delete ui;
    delete view;
    foreach(QRadioButton * bt,radioList){
        delete bt;
    }
}

void NotesDocuments::updateFilter(QStringList values)
{
    SqlFunctions::FilterDatabaseDocuments.clear();
    DataTex::FilterDocuments(Database_DocumentTableColumns);
    int columns = Database_DocumentTableColumns.count();
    for (int i=0;i<columns;i++) {
        SqlFunctions::FilterDatabaseDocuments.replace("replace"+Database_DocumentTableColumns.at(i),values.at(i));
    }
    DataTex::updateTableView(DocumentTable,SqlFunctions::FilterDatabaseDocuments,currentbase,this);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::DocumentTable_selectionChanged);
    DataTex::LoadTableHeaders(DocumentTable,Database_DocumentTableColumns);
}

void NotesDocuments::DocumentTable_selectionChanged()
{
    QItemSelectionModel *select = DocumentTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){
        row = select->selectedRows().at(0).row();
    }
    DocumentFileName = DocumentTable->model()->data(DocumentTable->model()->index(row,0)).toString();
    DocFileType = DocumentTable->model()->data(DocumentTable->model()->index(row,1)).toString();
    DocumentFilePath = DocumentTable->model()->data(DocumentTable->model()->index(row,5)).toString();
    ui->tabWidget->setCurrentIndex(1);
    DataTex::loadImageFile(DocumentFilePath,view);
}

void NotesDocuments::on_CreateBasicFolder_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(Basic(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::Basic(QString newfolder)
{
    QString Path;
    QStringList list;
    if(level > 0){
        list = ui->pathline->text().split(QDir::separator());
        list.removeLast();
        list.removeLast();
        Path = list.join(QDir::separator());
        qDebug()<<level<<Path;
    }
    else {
        Path = QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath();
    }
    QDir dir(Path+QDir::separator()+newfolder);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    else{
        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
    }
}

void NotesDocuments::on_OkbuttonBoxFolders_accepted()
{
    if(ui->NameLine->isEnabled()==true){
        QString texname = ui->NameLine->text();
        QString name;
        if(texname.isEmpty()==false && isWhiteSpace(texname)==false){
            name = ui->pathline->text()+texname+".tex";
            QFileInfo NewDocument(name);
            QDir path = NewDocument.absoluteDir();
            if (!path.exists())
                path.mkpath(".");
            QString Content = ui->DocumentContent->toPlainText();
            QString pathpdf = ui->pathline->text();
            QString FilePath = pathpdf+texname+".tex";
            QString date = QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm");
            QStringList valuesList;
            valuesList << texname<<DocumentType<<BasicFolder<<SubFolder<<SubSubFolder<<FilePath<<date<<Content<<
                          ui->PreambleBox->currentData().toString()<<ui->BuildBox->currentText()<<"0"<<""<<"0";
            QString Values = "(\""+valuesList.join("\",\"")+"\")";
            QSqlQuery AddDocument(currentbase);
            AddDocument.exec(QString("INSERT OR IGNORE INTO \"Basic_Folders\" (\"Name\") VALUES (\"%1\")").arg(BasicFolder));
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders\" (Name) VALUES(:subfolder);"));
            AddDocument.bindValue(":subfolder",SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubFolders_per_Basic\" (\"Sub_Id\",\"Basic_Id\") VALUES(:sub,:basic);"));
            AddDocument.bindValue(":basic",BasicFolder);
            AddDocument.bindValue(":sub",SubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders\" (\"Name\") VALUES(:subsub);"));
            AddDocument.bindValue(":subsub",SubSubFolder);
            AddDocument.exec();
            AddDocument.prepare(QString("INSERT OR IGNORE INTO \"SubsubFolders_per_Sub_perBasic\" (\"Subsub_Id\",\"Sub_Id\",\"Basic_Id\") VALUES(:subsub,:sub,:basic);"));
            AddDocument.bindValue(":subsub",SubSubFolder);
            AddDocument.bindValue(":sub",SubFolder);
            AddDocument.bindValue(":basic",BasicFolder);
            AddDocument.exec();
            AddDocument.exec(QString("INSERT OR IGNORE INTO \"Documents\" "
                                    "(\"Id\",\"Document_Type\",\"Basic_Folder\",\"SubFolder\",\"SubsubFolder\",\"Path\",\"Date\",\"Content\",\"Preamble\",\"BuildCommand\",\"NeedsUpdate\",\"Bibliography\",\"UseBibliography\") "
                                    "VALUES %1;").arg(Values));
            qDebug()<<QString("INSERT OR IGNORE INTO \"Documents\" "
                              "(\"Id\",\"Document_Type\",\"Basic_Folder\",\"SubFolder\",\"SubsubFolder\",\"Path\",\"Date\",\"Content\",\"Preamble\",\"BuildCommand\",\"NeedsUpdate\",\"Bibliography\",\"UseBibliography\") "
                              "VALUES %1;").arg(Values);
            if(!CloneMode){
                emit createnewdocument(FilePath,Content);
            }
            else{
                QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
                emit clonedocument(FilePath,Content,DataTex::GlobalDocsDatabaseList[database]);
            }
            if(ui->InsertFilesCheckBox->isChecked()){
                emit insertfiles();
            }
            QString BibliographyFile = FilePath;
            BibliographyFile.replace(".tex",".bib");
            QFile bibfile(BibliographyFile);
            bibfile.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream BibContent(&bibfile);
            BibContent.flush();
            BibContent << "%# Bib File : "+texname+"\n\n%# End of bib file "+texname;
            bibfile.close();

            CsvFunctions::WriteDocDataToCSV(FilePath,currentbase);

            QSqlQuery insertTag(currentbase);
            tags = tagLine->GetTags();
            foreach(QString tag,tags){
                if(!tag.isEmpty()){
                    insertTag.exec("INSERT OR IGNORE INTO CustomTags (Tag) VALUES (\""+tag+"\")");
                    insertTag.exec("INSERT OR IGNORE INTO Tags_per_Document (Tag_Id,Document_Id) VALUES (\""+tag+"\",\""+QFileInfo(FilePath).baseName()+"\")");
                }
            }

            QString newDocPath = ui->pathline->text()+ui->NameLine->text()+".tex";
            if(CloneMode){QFile::copy(DocumentPath.left(DocumentPath.lastIndexOf("."))+".pdf",
                            newDocPath.left(newDocPath.lastIndexOf("."))+".pdf");}
            accept();
        }
        else if (texname.isEmpty()==true || isWhiteSpace(texname) ==true){
        QMessageBox::warning(this,tr("Error"),tr("Select a file name.\nThe name is "
                                           "empty or contains\nempty space or dot \".\"."),QMessageBox::Ok);}
    }
    else if(ui->NameLine->isEnabled()==false){accept();}
}

void NotesDocuments::on_OkbuttonBoxFolders_rejected()
{
    reject();
}

void NotesDocuments::Folder(bool checked)
{
    if(checked){
        ui->removeButton->setEnabled(true);
        QRadioButton *btn = static_cast<QRadioButton *>(sender());
        DocumentType = btn->text();
        ui->NameLine->setEnabled(true);
        ui->BuildBox->setEnabled(true);
        ui->PreambleBox->setEnabled(true);
        ui->tabWidget->setCurrentIndex(0);
    }
}

bool NotesDocuments::isWhiteSpace(const QString & str)
{
  return QRegExp("\t | \n | \v | \f | \r | |.").exactMatch(str);
}

void NotesDocuments::EditBasicForders_DataTex(QString Line)
{
//    QString BasicFolder = ui->FoldersStructureView->currentItem()->text();
//    int row = ui->FoldersStructureView->currentRow();
//        QStringList folders;
//        for (int i=0;i<ui->FoldersStructureView->rowCount();i++) {
//            folders.append(ui->FoldersStructureView->item(i,0)->text());
//        }
//        if(folders.contains(Line)){
//            QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(Line));
//        return;}
//        ui->FoldersStructureView->item(row,0)->setText(QString(Line));
//        QFile::rename(NotesPath+BasicFolder+QDir::separator(),NotesPath+Line+QDir::separator());
//        QSqlQuery EditQuery(currentbase);
//        EditQuery.exec("PRAGMA foreign_keys = ON");
//        EditQuery.prepare(QString("UPDATE \"Basic_Folders\" SET \"Name\" = :newname WHERE \"Name\" = :name;"));
//        EditQuery.bindValue(":newname",Line);
//        EditQuery.bindValue(":name",BasicFolder);
//        EditQuery.exec();
//        EditQuery.prepare(QString("UPDATE \"Documents\" SET \"Path\" = replace(\"Path\",:basic,:line) WHERE \"Basic_Folder\" = :line;"));
//        EditQuery.bindValue(":line",Line);
//        EditQuery.bindValue(":basic",BasicFolder);
//        EditQuery.exec();
//        emit pathline(Line);
}

//void NotesDocuments::on_EditBasicButton_clicked()
//{
//    QString BasicFolder = ui->FoldersStructureView->currentItem()->text();
//    newFolder = new addfolder(this);
//    newFolder->EditFolder(BasicFolder);
//    connect(this,SIGNAL(pathline(QString)),newFolder,SLOT(EditFolder(QString)));
//    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(EditBasicFolders_DataTex(QString)));
//    newFolder->show();
//    newFolder->activateWindow();
//}

QString NotesDocuments::SavePath(const QModelIndex &index)
{
    ui->pathline->setEnabled(true);
    ui->OpenPath->setEnabled(true);
    ui->InsertFilesCheckBox->setEnabled(true);
    QModelIndex ix = index;
    QStringList list = {ix.data().toString()};
    QString path;
    int depth = 0;
    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
      list.prepend(ix.data().toString());
      depth++;
    }
    path = list.join(QDir::separator())+QDir::separator();
    path.replace("//","/");//Προσωρινό
    QString trimmedPath = path;
    trimmedPath.remove(QFileInfo(NotesPath).absolutePath()+"/");//Προσωρινό το '/'
    QStringList Folders = trimmedPath.split(QDir::separator());
    QStringList Values = {BasicFolder,SubFolder,SubSubFolder};
    BasicFolder = Folders[0];
    if(Folders.count()>1){
        if(!Folders[1].isNull() && !Folders[1].isEmpty()){
            SubFolder = Folders[1];
        }
        else{
            SubFolder = tr("No folder");
            SubSubFolder = tr("No folder");
        }
    }
    if(Folders.count()>2){
        if(!Folders[2].isNull() && !Folders[2].isEmpty()){
            SubSubFolder = Folders[2];
        }
        else{
            SubSubFolder = tr("No folder");
        }
    }
    qDebug()<<BasicFolder<<SubFolder<<SubSubFolder;
    ui->pathline->setText(path);
    return path;
}

void NotesDocuments::on_FoldersStructureView_clicked(const QModelIndex &index)
{
    ui->CreateSubFolder->setEnabled(true);
    ui->EditButton->setEnabled(true);
    ui->addButton->setEnabled(true);
    ui->RemButton->setEnabled(true);
    if(radioList.count()>0){
        if(!radioList[0]->isEnabled()){
            for (int i=0;i<radioList.count();i++) {
                radioList.at(i)->setEnabled(true);
            }
        }
    }
    QModelIndex ix = index;
    int depth = 0;
    while ( ix.parent().isValid() )
    {
      ix = ix.parent();
      depth++;
    }
    int result;
    QDir dir(NotesPath);
    for(result=0;dir.cdUp();++result){}
    level = depth-result;
    qDebug()<<depth-result<<index;
    tagLine->setEnabled(true);
}


void NotesDocuments::on_CreateSubFolder_clicked()
{
    addfolder * newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(NewSubFolder(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::NewSubFolder(QString newfolder)
{
    QDir dir(ui->pathline->text()+newfolder);
    if (!dir.exists()){
        dir.mkpath(".");
        QModelIndex in = ui->FoldersStructureView->currentIndex();
        ui->FoldersStructureView->expand(in);
    }
    else{
        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
    }
}


void NotesDocuments::on_RemButton_clicked()
{
    DataTex::FunctionInProgress();
//    QCheckBox *cb = new QCheckBox("Μετάβαση στο φάκελο");
//    QMessageBox msgbox;
//    msgbox.setText(tr("The folder will be deleted from the database!\nDelete the folder manually in case it contains files.\nDo you wish to proceed?"));
//    msgbox.setIcon(QMessageBox::Icon::Question);
//    msgbox.addButton(QMessageBox::Ok);
//    msgbox.addButton(QMessageBox::Cancel);
//    msgbox.setDefaultButton(QMessageBox::Cancel);
//    msgbox.setCheckBox(cb);
//    if (msgbox.exec() == QMessageBox::Ok) {
//        QString DeleteQuery;
//        QSqlQuery RemQuery(currentbase);
//        RemQuery.exec("PRAGMA foreign_keys = ON");
//        switch (level) {
//        case 0:
//            DeleteQuery = QString("DELETE FROM \"Basic_Folders\" WHERE \"Name\" = \"%1\";").arg(BasicFolder);
//            break;
//        case 1:
//            DeleteQuery = QString("DELETE FROM \"SubFolders_per_Basic\" WHERE \"Sub_Id\" = \"%1\";").arg(SubFolder);
//            break;
//        case 2:
//            DeleteQuery = QString("DELETE FROM \"SubsubFolders_per_Sub_perBasic\" WHERE \"Subsub_Id\" = \"%1\";").arg(SubSubFolder);
//            break;
//        }
//        RemQuery.exec(DeleteQuery);
//         if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+NotesPath));}
//    }
}

void NotesDocuments::on_EditButton_clicked()
{
    DataTex::FunctionInProgress();
}

void NotesDocuments::on_addButton_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(newSingleEntry(QString)),this,SLOT(AddDocType(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::AddDocType(QString docType)
{
    int rows = ui->gridLayout_4->rowCount();
    int widgets = ui->gridLayout_4->count();
    QSqlQuery AddDoc(currentbase);
    AddDoc.prepare(QString("INSERT INTO Document_Types (Name) VALUES (\"%1\")").arg(docType));
    if(AddDoc.exec()){
        QRadioButton * radio = new QRadioButton(docType);
        radioList.append(radio);
        radiogroup->addButton(radio);
        ui->gridLayout_4->addWidget(radio,rows-1+(widgets+1)%2,widgets%2);
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    else{
        QMessageBox::warning(this,tr("Error"),tr("The document type %1 already exists.").arg(docType),QMessageBox::Ok);
    }
}

void NotesDocuments::on_removeButton_clicked()
{
    foreach(QRadioButton * bt,radioList){
        if(bt->isChecked()){
            radiogroup->removeButton(bt);
            radioList.removeAll(bt);
            QSqlQuery removeFileType(currentbase);
            removeFileType.exec(QString("DELETE FROM Document_Types WHERE Name = \"%1\"").arg(bt->text()));
            delete bt;
            ui->removeButton->setEnabled(false);
        }
    }
}

void NotesDocuments::on_OpenPath_clicked()
{
    QDesktopServices::openUrl(QUrl("file:///"+ui->pathline->text()));
}


//void NotesDocuments::on_pushButton_clicked()
//{
//    QSortFilterProxyModel proxy;
//    QAbstractItemModel * model = ui->FoldersStructureView->model();
//    QModelIndex indexToSelect = model->index(0, 0).child(0,0).child(0,0).child(0,0).child(0,0);
////      for (int k = 0; k < model->rowCount(); ++k) {
////        auto currentIndex = model->index(k, 0);
////        if (currentIndex.data(Qt::DisplayRole) == "Bar") {
////          indexToSelect = currentIndex;
////        }
////      }
//    ui->FoldersStructureView->selectionModel()->setCurrentIndex(indexToSelect,QItemSelectionModel::Select);
//    SavePath(indexToSelect);
//    on_FoldersStructureView_clicked(indexToSelect);
//    qDebug() << indexToSelect;
//}

QString NotesDocuments::ClearContent(QString Content)
{
    QStringList text;
//    QStringList preamble;
    QString cleanContent;// = Content;
//    QTextStream PreStream(&Content);
//    while(!PreStream.atEnd()){
//        QString line = PreStream.readLine();
//        if(line.startsWith("\\begin{document}")){
//            break;
//        }
//        preamble<<line;
//    }
//    cleanContent.remove(preamble.join("\n"));
//    cleanContent.remove("\\begin{document}");
//    cleanContent.remove("\\end{document}");
//    cleanContent = cleanContent.trimmed();
    QTextStream stream(&Content);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        if(line.startsWith("%# Database Document :")
                || line.startsWith("%@ Document type:")
                || line.startsWith("%#--------------------------------------------------")){
            continue;
        }
        text<<line;
    }
    cleanContent=text.join("\n");
    return cleanContent;
}

void NotesDocuments::on_DatabaseCombo_activated(int index)
{
    qDebug()<<DataTex::GlobalDocsDatabaseList;
    QString database = QFileInfo(ui->DatabaseCombo->currentData().toString()).baseName();
    currentbase = DataTex::GlobalDocsDatabaseList[database];
    NotesPath = QFileInfo(currentbase.databaseName()).absolutePath()+QDir::separator();
    GetDocTypes();
    LoadFolderStructure();
    LoadDocTable();
    qDebug()<<DataTex::GlobalDocsDatabaseList;
}

void NotesDocuments::GetDocTypes()
{
    radioList.clear();
    foreach(QAbstractButton *button, radiogroup->buttons()) {
        ui->gridLayout_4->removeWidget(button);
        radiogroup->removeButton(button);
        delete button;
    }

    QStringList FileTypeList;
    QSqlQuery DocTypes(currentbase);
    DocTypes.exec(SqlFunctions::GetDocumentTypes);
    while(DocTypes.next()){
        FileTypeList.append(DocTypes.value(0).toString());
    }
    double items = FileTypeList.count();
    int half = ceil(items/2);
    for (int i=0;i<half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i),i,0);
        radioList.at(i)->setEnabled(false);
        radiogroup->addButton(radioList.at(i));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    for (int i=0;i<items-half;i++) {
        QRadioButton * radio = new QRadioButton(FileTypeList.at(i+half));
        radioList.append(radio);
        ui->gridLayout_4->addWidget(radioList.at(i+half),i,1);
        radioList.at(i+half)->setEnabled(false);
        radiogroup->addButton(radioList.at(i+half));
        connect(radio, &QRadioButton::toggled, this, &NotesDocuments::Folder);
    }
    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);

}

void NotesDocuments::LoadFolderStructure()
{
    QFileSystemModel *model = new QFileSystemModel(this);
    model->setRootPath(QFileInfo(currentbase.databaseName()).absolutePath());
    model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    model->setNameFilterDisables(false);
    model->insertRow(0);
    ui->FoldersStructureView->setModel(model);
    QModelIndex index = model->index(QFileInfo(currentbase.databaseName()).absolutePath());
    ui->FoldersStructureView->setRootIndex(index);
    ui->FoldersStructureView->expand(index);
    ui->FoldersStructureView->scrollTo(index);
    ui->FoldersStructureView->setCurrentIndex(index);
    ui->FoldersStructureView->resizeColumnToContents(0);
    for (int i=1;i<model->columnCount();i++) {
        ui->FoldersStructureView->setColumnHidden(i,true);
    }
}

void NotesDocuments::LoadDocTable()
{
    QSqlTableModel * DocumentModel = new QSqlTableModel(this,currentbase);
    DocumentModel->setTable("Documents");
    DocumentModel->select();
    DocumentModel->setHeaderData(1, Qt::Horizontal, tr("Document type"));
    DocumentTable->setModel(DocumentModel);
    DocumentTable->show();
    int columns = DocumentTable->model()->columnCount();
    DocumentTable->generateFilters(columns,false);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NotesDocuments::DocumentTable_selectionChanged);
    connect(DocumentTable->filterHeader(), &FilterTableHeader::filterValues, this, &NotesDocuments::updateFilter);
    DataTex::StretchColumns(DocumentTable,1.5);
    for (int i=2;i<DocumentTable->model()->columnCount();i++) {
        DocumentTable->setColumnHidden(i,true);
    }
}
