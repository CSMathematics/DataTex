#include "notesdocuments.h"
#include "ui_notesdocuments.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QCompleter>
#include <QTextStream>
#include <QComboBox>
#include <QString>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QDesktopServices>
#include <QList>
#include <QUrl>
#include <QDebug>
#include <QCloseEvent>
#include <QRadioButton>
#include <QButtonGroup>
#include "addfolder.h"
#include "sqlfunctions.h"
#include "addfiletoeditor.h"
#include <QFileSystemModel>


NotesDocuments::NotesDocuments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotesDocuments)
{
    radiogroup = new QButtonGroup;
    NotesPath = QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath()+QDir::separator();
    currentbase = DataTex::CurrentNotesFolderDataBase;
    currentbase_Exercises = DataTex::CurrentTexFilesDataBase;
    radioList.clear();
    ui->setupUi(this);
    Database_DocumentTableColumns = SqlFunctions::Get_StringList_From_Query("SELECT name FROM pragma_table_info('Documents')",DataTex::CurrentNotesFolderDataBase);
    QStringList horzHeaders;
    horzHeaders << tr("File type") << tr("Name")<<"Path";
//    DocumentTable->setHorizontalHeaderLabels(horzHeaders);
//    DocumentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
//    DocumentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    DocumentTable->setAlternatingRowColors(true);
//    DocumentTable->setStyleSheet("alternate-background-color: #e8e8e8");
//    DocumentTable->horizontalHeader()->setStretchLastSection(true);
    QStringList horzHeaders2;
    horzHeaders2 << tr("File Id") << tr("Section")<<tr("Exercise type");

    QStringList list;

//    ui->FoldersStructureView->setColumnCount(1);
    QStringList Headers;
    ui->FoldersStructureView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->FoldersStructureView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->FoldersStructureView->setAlternatingRowColors(true);
    ui->FoldersStructureView->setStyleSheet("alternate-background-color: #e8e8e8");
    Headers << tr("Folder name");


    ui->RemBasicButton->setEnabled(false);

    ui->EditBasicButton->setEnabled(false);

    ui->NameLine->setEnabled(false);
    ui->pathline->setEnabled(false);

    ui->InsertFilesCheckBox->setEnabled(false);


    QStringList BasicFolders;
    QSqlQuery fields(currentbase);
    fields.exec("SELECT \"Name\" FROM \"Basic_Folders\";");
    while(fields.next()){
        BasicFolders.append(fields.value(0).toString());
    }
//    for (int item=0;item<BasicFolders.count();item++ ) {
//        ui->FoldersStructureView->insertRow(item);
//        ui->FoldersStructureView->setItem(item,0, new QTableWidgetItem(BasicFolders.at(item)));
//    }
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
    view = new PdfViewer(this);
    ui->verticalLayout_5->addWidget(view);
    view->show();

    QFileSystemModel *model = new QFileSystemModel(this);
    model->setRootPath(QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath());
    QStringList filters;
    model->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    model->setNameFilterDisables(false);
    model->insertRow(0);
    ui->FoldersStructureView->setModel(model);
    QModelIndex index = model->index(QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath());
    ui->FoldersStructureView->setRootIndex(index);
    ui->FoldersStructureView->expand(index);
    ui->FoldersStructureView->scrollTo(index);
    ui->FoldersStructureView->setCurrentIndex(index);
    ui->FoldersStructureView->resizeColumnToContents(0);
    for (int i=1;i<model->columnCount();i++) {
        ui->FoldersStructureView->setColumnHidden(i,true);
    }

    DocumentTable = new ExtendedTableWidget(this);
    ui->verticalLayout_4->addWidget(DocumentTable);
    DocumentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    DocumentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DocumentTable->horizontalHeader()->setSectionsClickable(true);
    QSqlTableModel * DocumentModel = new QSqlTableModel(this,DataTex::CurrentNotesFolderDataBase);
    DocumentModel->setTable("Documents");
    DocumentModel->select();
    DocumentModel->setHeaderData(1, Qt::Horizontal, tr("Document type"));
    DocumentTable->setModel(DocumentModel);
    DocumentTable->show();
    int columns = DocumentTable->model()->columnCount();
    DocumentTable->generateFilters(columns,false);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,this, &NotesDocuments::DocumentTable_selectionChanged);
    connect(DocumentTable->filterHeader(), &FilterTableHeader::filterValues, this, &NotesDocuments::updateFilter);
    for (int c = 0; c < DocumentTable->horizontalHeader()->count()-1; ++c)
    {
        DocumentTable->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }

    for (int i=2;i<9;i++) {
        DocumentTable->setColumnHidden(i,true);
    }
    connect(ui->FoldersStructureView, &QAbstractItemView::clicked, this, &NotesDocuments::SavePath);
}

NotesDocuments::~NotesDocuments()
{
    delete ui;
    delete view;
}

void NotesDocuments::updateFilter(QStringList values)
{
    SqlFunctions::FilterDatabaseDocuments.clear();
    DataTex::FilterDocuments(Database_DocumentTableColumns);
    int columns = Database_DocumentTableColumns.count();
    for (int i=0;i<columns;i++) {
        SqlFunctions::FilterDatabaseDocuments.replace("replace"+Database_DocumentTableColumns.at(i),values.at(i));
    }
//        DocumentsTable->setColumnHidden(columns,true);
    DataTex::updateTableView(DocumentTable,SqlFunctions::FilterDatabaseDocuments,DataTex::CurrentNotesFolderDataBase,this);
    connect(DocumentTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NotesDocuments::DocumentTable_selectionChanged);
    DataTex::LoadTableHeaders(DocumentTable,Database_DocumentTableColumns);
    qDebug()<<SqlFunctions::FilterDatabaseDocuments<<"\n"<<values;
}

void NotesDocuments::DocumentTable_selectionChanged()
{
    QItemSelectionModel *select = DocumentTable->selectionModel();
    int row = -1;
    if(select->hasSelection()){ //check if has selection
        row = select->selectedRows().at(0).row();
    }
    DocumentFileName = DocumentTable->model()->data(DocumentTable->model()->index(row,0)).toString();
    DocFileType = DocumentTable->model()->data(DocumentTable->model()->index(row,1)).toString();
    DocumentFilePath = DocumentTable->model()->data(DocumentTable->model()->index(row,5)).toString();

    DataTex::loadImageFile(DocumentFilePath,view);
}

void NotesDocuments::on_AddBasicButton_clicked()
{
//    int rows = ui->FoldersStructureView->rowCount();
//    QString folder = QFileDialog::getExistingDirectory(this, tr("Select folder"),NotesPath);
//    QString BasicFolder = folder.remove(NotesPath);
//    if (folder.isEmpty()) return;
//    QStringList folders;
//    for (int i=0;i<rows;i++) {
//        folders.append(ui->FoldersStructureView->item(i,0)->text());
//    }
//    if(folders.contains(folder.remove(NotesPath))){
//        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(BasicFolder));
//    return;}
//    ui->FoldersStructureView->insertRow(rows);
//    ui->FoldersStructureView->setItem(rows,0, new QTableWidgetItem(folder.remove(NotesPath)));
//    QSqlQuery AddQuery(currentbase);
//    AddQuery.prepare(QString("INSERT INTO \"Basic_Folders\" (\"Name\") VALUES(:basic);").arg(BasicFolder));
//    AddQuery.bindValue(":basic",BasicFolder);
//    AddQuery.exec();
}

//void NotesDocuments::on_FoldersStructureView_itemSelectionChanged()
//{
//    ui->RemBasicButton->setEnabled(true);
//    ui->EditBasicButton->setEnabled(true);

//    QStringList HeadersSub;

//    HeadersSub << tr("Subfolder name");
//    ui->SubFolderTable->setHorizontalHeaderLabels(HeadersSub);
//    if(ui->FoldersStructureView->rowCount()>0 && ui->FoldersStructureView->currentRow()!=-1){
//        int row = ui->FoldersStructureView->currentRow();
//        QString baseFolder= ui->FoldersStructureView->item(row,0)->text();
//        QStringList SubFolders;
//        QSqlQuery fields(currentbase);
//        fields.prepare(QString("SELECT \"Sub_Id\" FROM \"SubFolders_per_Basic\" WHERE \"Basic_Id\" = :basic;"));
//        fields.bindValue(":basic",baseFolder);
//        fields.exec();
//        while(fields.next()){
//            SubFolders.append(fields.value(0).toString());
//        }
//        for (int item=0;item<SubFolders.count();item++ ) {
//            ui->SubFolderTable->insertRow(item);
//            ui->SubFolderTable->setItem(item,0, new QTableWidgetItem(SubFolders.at(item)));
//        }
//}

void NotesDocuments::on_CreateBasicButton_clicked()
{
    newFolder = new addfolder(this);
    connect(newFolder,SIGNAL(grammhfolder(QString)),this,SLOT(Basic(QString)));
    newFolder->show();
    newFolder->activateWindow();
}

void NotesDocuments::Basic(QString newfolder)
{
    QFile BasF(BasicFolderFile);
    QStringList folders;
//    for (int i=0;i<ui->FoldersStructureView->rowCount();i++) {
//        folders.append(ui->FoldersStructureView->item(i,0)->text());
//    }
    if(folders.contains(newfolder)){
        QMessageBox::warning(this,tr("Attention"),tr("The folder %1 already exists").arg(newfolder));
    return;}

    QSqlQuery CreateQuery(currentbase);
    CreateQuery.prepare(QString("INSERT INTO \"Basic_Folders\" (\"Name\") VALUES(:folder);"));
    CreateQuery.bindValue(":folder",newfolder);
    CreateQuery.exec();
//    int i = ui->FoldersStructureView->rowCount();
//    ui->FoldersStructureView->insertRow(i);
//    ui->FoldersStructureView->setItem(i,0 , new QTableWidgetItem(newfolder));
    QDir dir(NotesPath+newfolder);
    if (!dir.exists())
        dir.mkpath(".");
}

void NotesDocuments::on_RemBasicButton_clicked()
{
//    int row = ui->FoldersStructureView->currentRow();
//    QCheckBox *cb = new QCheckBox("Μετάβαση στο φάκελο");
//        QMessageBox msgbox;
//        msgbox.setText(tr("The folder will be deleted from the database!\nDelete the folder manually in case it contains files.\nDo you wish to proceed?"));
//        msgbox.setIcon(QMessageBox::Icon::Question);
//        msgbox.addButton(QMessageBox::Ok);
//        msgbox.addButton(QMessageBox::Cancel);
//        msgbox.setDefaultButton(QMessageBox::Cancel);
//        msgbox.setCheckBox(cb);

//    if (msgbox.exec() == QMessageBox::Ok) {
//    ui->FoldersStructureView->removeRow(row);
//    QSqlQuery RemQuery(currentbase);
//    RemQuery.exec("PRAGMA foreign_keys = ON");
//    RemQuery.prepare(QString("DELETE FROM \"Basic_Folders\" WHERE \"Name\" = :basic;"));
//    RemQuery.bindValue(":basic",ui->FoldersStructureView->currentItem()->text());
//    RemQuery.exec();
//     if(cb->isChecked()==true){QDesktopServices::openUrl(QUrl("file:///"+NotesPath));}
//    }
}

void NotesDocuments::on_OkbuttonBoxFolders_accepted()
{
    if(ui->NameLine->isEnabled()==true){
        QString texname = ui->NameLine->text();
        QString name;
//        int row1 = ui->FoldersStructureView->currentRow();
//        int row2 = ui->SubFolderTable->currentRow();
//        QString BasicFolder = ui->FoldersStructureView->item(row1,0)->text();
//        QString SubFolder = ui->SubFolderTable->item(row2,0)->text();
//        QString SubsubFolfer;
//        if(ui->SubsubCheck->isChecked()){
//            SubsubFolfer = "\""+ui->SubsubFolderTable->currentItem()->text()+"\"";
//        }
//        else{SubsubFolfer = "NULL";}
        if(texname.isEmpty()==false && isWhiteSpace(texname)==false){
            name = ui->pathline->text()+"DTX-"+texname+".tex";
            QFileInfo OnomaArxeiou(name);
            QDir path = OnomaArxeiou.absoluteDir();
        if (!path.exists())
            path.mkpath(".");
        QString pathpdf = ui->pathline->text();
        QString FilePath = pathpdf+"DTX-"+texname+".tex";
        QString date = QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm");
        QStringList valuesList;
        valuesList << "DTX-"+texname<<DocumentType<<BasicFolder<<SubFolder<<SubSubFolder<<FilePath<<date;
        qDebug()<<valuesList;
        QString Values = "(\""+valuesList.join("\",\"")+"\")";
        qDebug()<<Values;
        QSqlQuery AddDocument(currentbase);
        AddDocument.exec(QString("INSERT OR IGNORE INTO \"Documents\" "
"(\"Id\",\"Document_Type\",\"Basic_Folder\",\"SubFolder\",\"SubsubFolder\",\"Path\",\"Date\") "
"VALUES %1;").arg(Values));
        emit openpdf(FilePath);
        if(ui->InsertFilesCheckBox->isChecked()){
            emit insertfiles();}
        accept();}
        else if (texname.isEmpty()==true || isWhiteSpace(texname) ==true) {
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
        QRadioButton *btn = static_cast<QRadioButton *>(sender());
        DocumentType = btn->text();
        ui->NameLine->setEnabled(true);
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

void NotesDocuments::on_EditBasicButton_clicked()
{
//    QString BasicFolder = ui->FoldersStructureView->currentItem()->text();
//    newFolder = new addfolder(this);
//    newFolder->EditFolder(BasicFolder);
//    connect(this,SIGNAL(pathline(QString)),newFolder,SLOT(EditFolder(QString)));
//    connect(newFolder,SIGNAL(grammhfolder(QString)),this,SLOT(EditBasicFolders_DataTex(QString)));
//    newFolder->show();
//    newFolder->activateWindow();
}

void NotesDocuments::on_OpenFolderButton_clicked()
{
//    int row = DocumentTable->currentRow();
//    QString path = DocumentTable->item(row,2)->text();
//    QFileInfo Path(path);
//    QString Folder = Path.absolutePath();
//    QDesktopServices::openUrl(QUrl("file:///"+Folder));
}

QString NotesDocuments::SavePath(const QModelIndex &index)
{
    ui->pathline->setEnabled(true);
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
    trimmedPath.remove(QFileInfo(DataTex::CurrentNotesFolderPath).absolutePath()+"/");//Προσωρινό το '/'
    QStringList Folders = trimmedPath.split(QDir::separator());
    QStringList Values = {BasicFolder,SubFolder,SubSubFolder};
    BasicFolder = Folders[0];
    if(Folders.count()>1){SubFolder = Folders[1];}
    if(Folders.count()>2){SubSubFolder = Folders[2];}
    qDebug()<<BasicFolder<<SubFolder<<SubSubFolder;
    ui->pathline->setText(path);
    return path;
}

void NotesDocuments::on_DocumentTable_itemSelectionChanged()
{
//    int row = DocumentTable->currentRow();
//    ui->OpenPdfButton->setEnabled(true);
//    ui->OpenTexButton->setEnabled(true);
//    ui->OpenFolderButton->setEnabled(true);
//    QString TexFilePath = DocumentTable->item(row,2)->text();
//    QFile TexFile(TexFilePath);
//    QStringList filepaths;
//    QString path_rem = "%# Database File : ";
//    TexFile.open (QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream Line(&TexFile);
//    while (!Line.atEnd()){
//    QString GrammhText=Line.readLine();
//    if(GrammhText.contains(path_rem))filepaths.append(GrammhText.remove(path_rem).remove("----"));}
//    TexFile.close();
//    qDebug()<<filepaths;
//    QString files = "(\""+filepaths.join("\",\"")+"\")";

//    QSqlQueryModel * Files = new QSqlQueryModel(this);
//    QSqlQuery FilesQuery(currentbase_Exercises);
//    FilesQuery.exec(QString("SELECT DISTINCT \"df\".\"Id\",\"ft\".\"FileType\","
//                            "IIF(\"df\".\"FileType\" IN ('CombEx','SolCE','CombSub','SolCS','HintCE','HintCS'),"
//                            "replace(group_concat(\"s\".\"Name\"),',','-'),\"s\".\"Name\") AS 'Section',"
//                            "\"se\".\"Exercise_Name\","
//                            "\"Path\",\"Solved\",\"df\".\"FileType\" "
//                            "FROM \"Database_Files\" \"df\" "
//                            "JOIN \"FileTypes\" \"ft\" ON \"ft\".\"Id\" = \"df\".\"FileType\" "
//                            "JOIN \"Sections\" \"s\" ON \"s\".\"Id\" = \"df\".\"Section\""
//                            "LEFT JOIN \"Sections_Exercises\" \"se\" ON \"se\".\"Exercise_Id\" = \"df\".\"ExerciseType\" "
//                            "WHERE \"df\".\"Id\" IN %1"
//                            "GROUP BY \"df\".\"Id\""
//                            "ORDER BY \"df\".rowid;").arg(files));
//    Files->setQuery(FilesQuery);
//    ui->TexFileTable->setModel(Files);
//    ui->TexFileTable->show();
//    Exer_List.clear();
//    Solutions_List.clear();
//    Solved_List.clear();
//    Unsolved_List.clear();
//    Hint_List.clear();
//    UnHinted_List.clear();
//    QStringList Ids;
//    QStringList solIds;
//    QStringList hintIds;
//    QStringList folders;
//    QStringList solfolders;
//    Ids<<"SectEx"<<"SectSub"<<"CombEx"<<"CombSub";
//    solIds<<"SolSE"<<"SolSS"<<"SolCE"<<"SolCS";
//    hintIds<<"HintSE"<<"HintSS"<<"HintCE"<<"HintCS";
//    for (int i =0;i<filepaths.count();i++){
//        QString FileType = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,6)).toString();
//        if(Ids.contains(FileType)){
//            QString exercise = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,4)).toString();
//            QString solved = exercise;
////            QString Solution_Exists = ui->TexFileTable->model()->data(ui->TexFileTable->model()->index(i,5)).toString();
//            QString Hint = exercise;
//            for (int i=0;i<Ids.count();i++ ) {
//                QString folderName = SqlFunctions::Get_StringList_From_Query(
//                            SqlFunctions::FolderName.arg(Ids.at(i)),currentbase_Exercises).at(0);
//                folders.append(folderName);
//                QString solfolderName = SqlFunctions::Get_StringList_From_Query(
//                            SqlFunctions::FolderName.arg(solIds.at(i)),currentbase_Exercises).at(0);
//                solfolders.append(solfolderName);
//                solved.replace("-"+Ids.at(i),"-"+solIds.at(i));
//                solved.replace(folders.at(i),solfolders.at(i));
//                Hint.replace("-"+Ids.at(i),"-"+hintIds.at(i));
//                Hint.replace(folders.at(i),solfolders.at(i));
//            }
//            Exer_List.append(exercise);
//            Solutions_List.append(solved);
//            Hint_List.append(Hint);
//        }
//    }
//    for (int i=0;i<Exer_List.count();i++) {
//        QFile pathfile;
//        if(!pathfile.exists(Solutions_List.at(i))){
//            Unsolved_List.append(Exer_List.at(i));}
//        if(pathfile.exists(Solutions_List.at(i))){
//            Solved_List.append(Solutions_List.at(i));}
//        if(!pathfile.exists(Hint_List.at(i))){
//            UnHinted_List.append(Exer_List.at(i));}
//        if(pathfile.exists(Hint_List.at(i))){
//            Hinted_List.append(Hint_List.at(i));}
//    }
//    if(Exer_List.size()>0){ui->SolutionTexButton->setEnabled(true);}
//    else if(Exer_List.size()==0){ui->SolutionTexButton->setEnabled(false);}

//    QString solutionsname = QFileInfo(TexFilePath).baseName()+"_Solutions";
//    QString SolutionsPdfFile = QFileInfo(TexFilePath).absolutePath()+QDir::separator()+solutionsname+".pdf";
//    QString PdfFile = QFileInfo(TexFilePath).absolutePath()+QDir::separator()+QFileInfo(TexFilePath).baseName()+".pdf";
//    QFile file;
//    if(!file.exists(PdfFile)){
//        ui->OpenPdfButton->setEnabled(false);
//    }
//    QFile solfile;
//    if(!solfile.exists(SolutionsPdfFile)){
//        ui->SolutionPdfButton->setEnabled(false);
//    }
//    else{ui->SolutionPdfButton->setEnabled(true);}
//    if(TexFilePath.contains("_Solutions")){
//        ui->SolutionPdfButton->setEnabled(false);
//        ui->SolutionTexButton->setEnabled(false);
//    }
}

//QString NotesDocuments::getPreamble(QString fileName)
//{
//    QString Content;
//    QFile File(fileName);
//    File.open (QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream Line(&File);
//    while (!Line.atEnd()){
//        QString GrammhText=Line.readLine();
//        Content.append(GrammhText+"\n");
//        if(GrammhText.contains("\\begin{document}")){break;}
//        }

//    File.close();
//    return Content;
//}

void NotesDocuments::on_FoldersStructureView_clicked(const QModelIndex &index)
{
    if(!radioList[0]->isEnabled()){
        for (int i=0;i<radioList.count();i++) {
            radioList.at(i)->setEnabled(true);
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
    QDir dir(DataTex::CurrentNotesFolderPath);
    for(result=0;dir.cdUp();++result){}
    qDebug()<<depth-result;
    DocumentTable->setFilter(depth-result+2,index.data().toString());
}

