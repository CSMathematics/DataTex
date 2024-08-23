#include "newfiletype.h"
#include "ui_newfiletype.h"
#include "datatex.h"
#include "sqlfunctions.h"

NewFileType::NewFileType(QWidget *parent,DTXDatabaseType Type) :
    QDialog(parent),
    ui(new Ui::NewFileType)
{
    ui->setupUi(this);
    DBType = Type;
    QStringList Ids = SqlFunctions::Get_StringList_From_Query("SELECT Id FROM FileTypes",DataTex::CurrentFilesDataBase.Database);
    QStringList Names = SqlFunctions::Get_StringList_From_Query("SELECT Name FROM FileTypes",DataTex::CurrentFilesDataBase.Database);
    QStringList Folders = SqlFunctions::Get_StringList_From_Query("SELECT FolderName FROM FileTypes",DataTex::CurrentFilesDataBase.Database);
    setWindowTitle("Select a name/desctription for tis database");
    QRegExp pk("[A-Za-z0-9]{1,}");
    QRegExpValidator * validator = new QRegExpValidator( pk, this );
    ui->IdLine->setValidator(validator);
    connect(ui->NameLine,&QLineEdit::textChanged,this,[=](){
        if(Names.contains(ui->NameLine->text())){
            ui->warning->setText("This name already exists");
            ui->OkButton->setEnabled(false);
        }
        else{
            ui->warning->setText("");
            ui->OkButton->setEnabled(true);
        }
    });
    connect(ui->IdLine,&QLineEdit::textChanged,this,[=](QString text){
        if(Ids.contains(text)){
            ui->warning->setText(tr("This id already exists"));
            ui->OkButton->setEnabled(false);
        }
        else{
            if(text.length()>7){
                ui->warning->setText(tr("Use short primary keys\n(<8 characters) for short file names."));
            }
            else{
                ui->warning->clear();
            }
            ui->OkButton->setEnabled(true);
        }
    });
    connect(ui->FolderLine,&QLineEdit::textChanged,this,[=](){
        if(Folders.contains(ui->FolderLine->text())){
            ui->warning->setText(tr("This folder already exists"));
            ui->OkButton->setEnabled(false);
        }
        else{
            ui->warning->setText("");
            ui->OkButton->setEnabled(true);
        }
    });
}

NewFileType::~NewFileType()
{
    delete ui;
}

void NewFileType::on_OkButton_accepted()
{
    FileType.Id = ui->IdLine->text();
    FileType.Name = ui->NameLine->text();
    FileType.FolderName = ui->FolderLine->text();
    FileType.Solvable = (ui->NeedsSolution->isChecked()) ? DTXSolutionState::Solvable : DTXSolutionState::NotSolvable;
    FileType.BelongsTo = QString();
    FileType.Description = ui->DescriptionLine->text();
    FileType.BuiltIn = false;
    FileType.DBType = DBType;
    emit filedata(FileType);
}

bool NewFileType::CreateNewDatabaseFileType(QSqlDatabase database,int DBType, DTXFileType filetype)
{
    QStringList tables = {"FileTypes","Document_Types","BibTypes","","","","","",""};
    QSqlQuery NewFileType(database);
    NewFileType.prepare(QString("INSERT OR IGNORE INTO %1 (Id,Name,FolderName,Solvable,BelongsTo,Description) "
                                "VALUES(\"%2\",\"%3\",\"%4\",\"%5\",\"%6\")").arg(tables[DBType],filetype.Id,filetype.Name,
                                 filetype.FolderName,QString::number((int)filetype.Solvable),filetype.Description));
    qDebug()<<QString("INSERT OR IGNORE INTO %1 (Id,Name,FolderName,Solvable,Description) "
                        "VALUES (\"%2\",\"%3\",\"%4\",\"%5\",\"%6\")").arg(tables[DBType],filetype.Id,filetype.Name,
                         filetype.FolderName,QString::number((int)filetype.Solvable),filetype.Description);
    qDebug()<<(filetype.Solvable == DTXSolutionState::Solvable);
    if(NewFileType.exec()){
        if(filetype.Solvable == DTXSolutionState::Solvable){
            DTXFileType filetypeSol;
            filetypeSol.Id = filetype.Id+"-Sol";
            filetypeSol.Name = filetype.Name+tr(" - Solution");
            filetypeSol.FolderName = filetype.FolderName+tr(" - Solutions");
            filetypeSol.Solvable = DTXSolutionState::Solution;
            filetypeSol.BelongsTo = filetype.Id;
            filetypeSol.Description = tr("Solutions of : ")+filetype.Name;
            filetypeSol.BuiltIn = false;
            filetypeSol.DBType = (DTXDatabaseType)DBType;

            NewFileType.prepare(QString("INSERT OR IGNORE INTO %1 (Id,Name,FolderName,Solvable,BelongsTo,Description) "
                                        "VALUES(\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\")")
                                    .arg(tables[DBType],filetypeSol.Id,filetypeSol.Name,
                                     filetypeSol.FolderName,QString::number((int)filetypeSol.Solvable)
                                    ,filetypeSol.BelongsTo,filetypeSol.Description));
            NewFileType.exec();
        }
        return true;
    }
    else{
        qDebug()<<NewFileType.lastQuery();
        return false;
    }
}

