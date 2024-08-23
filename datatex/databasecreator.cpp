#include "databasecreator.h"
#include "ui_databasecreator.h"
#include <QObject>


DatabaseFieldInfoWidget::DatabaseFieldInfoWidget(QWidget *parent, bool isBasicField) :
    QWidget(parent)
{
    isBasic = isBasicField;
    NameDescription = new QLineEdit(this) ;
    DataTypes = new QComboBox(this);
    DataTypes->addItems({"TEXT","INTEGER","BLOB","REAL","NUMERIC"});
    HLayout = new QHBoxLayout(this);
    VisibleInTable = new QCheckBox(this);
    VisibleInTable->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    if(isBasicField){
        Id = new QLabel(this);
        HLayout->addWidget(Id);
    }
    else{
        checkRemove = new QCheckBox(QString(),this);
        CustomId = new QLineEdit(this);
        CustomId->setMinimumWidth((int)NameDescription->width()*0.3);
        CustomId->setMaximumWidth((int)NameDescription->width()*0.4);
        CustomId->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
        HLayout->addWidget(checkRemove);
        HLayout->addWidget(CustomId);
    }
    HLayout->addWidget(NameDescription);
    HLayout->addWidget(DataTypes);
    HLayout->addWidget(VisibleInTable);
    setLayout(HLayout);
}

DatabaseFieldInfoWidget::~DatabaseFieldInfoWidget()
{
//    delete Id;
}

DTXDBFieldInfo DatabaseFieldInfoWidget::getFieldInfoList()
{
    DTXDBFieldInfo info;
    QString FieldId = (isBasic) ? Id->text() : CustomId->text() ;
    info.Id = FieldId;
    info.Name = NameDescription->text();
    info.isBasic = isBasic;
    info.DataType = DataTypes->currentText();
    info.isVisibleInTable = VisibleInTable->isChecked();//
    info.Index = NameDescription->property("index").toInt();//
    return info;
}

DatabaseCreator::DatabaseCreator(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::DatabaseCreator)
{
    ui->setupUi(this);
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/datatex.png"));

    QStringList templates = {"FilesDBTemplate","DocumentsDBTemplate",
                             "BibliographyDBTemplate",
                             "TablesDBTemplate","FiguresDBTemplate",
                             "CommandsDBTemplate","PreamblesDBTemplate",
                             "PackagesDBTemplate","ClassesDBTemplate"};

    for (int index = 0;index<ui->DatabaseTypeCombo->count();index++) {
        ui->DatabaseTypeCombo->setItemData(index,templates.at(index));
    }
    NewDatabase.Type = 0;
    DBTemplateFileName = "FilesDBTemplate";
    connect(ui->DatabaseTypeCombo,QOverload<int>::of(&QComboBox::activated),this,[=](int index){
        NewDatabase.Type = index;
        DBTemplateFileName = ui->DatabaseTypeCombo->itemData(index).toString();
        // qDebug()<<":/databases/"+DBTemplateFileName+".db";
    });
    DatabaseList = SqlFunctions::Get_StringList_From_Query("SELECT FileName FROM Databases",DataTex::DataTeX_Settings);

    connect(ui->UsePrefix,&QCheckBox::toggled,this,[=](bool checked){
        NewDatabase.UsePrefix = (checked == !ui->prefix->text().isEmpty());
        ui->prefix->setEnabled(checked);
        emit ui->FilesPage->completeChanged();
    });
    connect(ui->prefix,&QLineEdit::textChanged,this,[=](QString text){
        NewDatabase.UsePrefix = (ui->UsePrefix->isChecked() == !text.isEmpty());
        emit ui->FilesPage->completeChanged();
    });
    connect(ui->encryptBox,&QCheckBox::toggled,this,[=](bool checked){
        NewDatabase.Encrypt = (checked == !ui->userName->text().isEmpty()) &&
                          (!ui->userName->text().isEmpty() == !ui->passwordLine->text().isEmpty());
        emit ui->FilesPage->completeChanged();
        ui->keyGenerator->setEnabled(checked);
        ui->userName->setEnabled(checked);
        ui->passwordLine->setEnabled(checked);
    });
    connect(ui->keyGenerator,&QPushButton::clicked,this,[=](){
        KeyGenerator * key = new KeyGenerator(this);
        connect(key,&KeyGenerator::password,this,[=](QString password){
            ui->passwordLine->setText(password);
        });
        key->show();
        key->activateWindow();
    });
    connect(ui->userName,&QLineEdit::textChanged,this,[=](QString text){
        NewDatabase.Encrypt = (ui->encryptBox->isChecked() == !text.isEmpty()) &&
                          (!text.isEmpty() == !ui->passwordLine->text().isEmpty());
        emit ui->FilesPage->completeChanged();
    });
    connect(ui->passwordLine,&QLineEdit::textChanged,this,[=](QString text){
        NewDatabase.Encrypt = (ui->encryptBox->isChecked() != ui->userName->text().isEmpty()) &&
                          (ui->userName->text().isEmpty() != !text.isEmpty());
        emit ui->FilesPage->completeChanged();
    });

    ui->DatabasePathButton->setFocus();
    QFont font = ui->CheckDatabaseName->font();
    font.setPointSize(11);
    font.setBold(true);
    ui->CheckDatabaseName->setFont(font);
    ui->CheckDatabaseName->setStyleSheet("QLabel {color : red; }");

    connect(ui->DatabasePathButton,&QPushButton::pressed,this,[&](){
        QString path = QFileDialog::getExistingDirectory(this,tr("Select a folder for your new database"),QDir::homePath());
        ui->DatabasePath->setText(path);
    });
    connect(ui->DatabaseFileName,&QLineEdit::textChanged,this, &DatabaseCreator::CheckDatabase);

    connect(ui->DatabaseFileName,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->DatabaseName,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->DatabasePath,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->prefix,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->userName,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->passwordLine,&QLineEdit::textChanged,this, &DatabaseCreator::isPageReady);
    connect(ui->UsePrefix,&QCheckBox::toggled,this, &DatabaseCreator::isPageReady);
    connect(ui->encryptBox,&QCheckBox::toggled,this, &DatabaseCreator::isPageReady);

    connect(ui->DatabaseFileName,&QLineEdit::textChanged,this, [&](QString text){NewDatabase.BaseName = text;});
    connect(ui->DatabaseName,&QLineEdit::textChanged,this, [&](QString text){NewDatabase.Description = text;});
    connect(ui->DatabasePath,&QLineEdit::textChanged,this, [&](QString text){NewDatabase.Path = text;});
    connect(ui->UsePrefix,&QCheckBox::toggled,this, [&](bool checked){NewDatabase.UsePrefix = checked;});
    connect(ui->encryptBox,&QCheckBox::toggled,this,[&](bool checked){NewDatabase.Encrypt = checked;});
    connect(ui->prefix,&QLineEdit::textChanged,this, [&](QString text){NewDatabase.Prefix = text;});
    connect(ui->userName,&QLineEdit::textChanged,this, [&](QString text){NewDatabase.Username = text;});
    connect(ui->passwordLine,&QLineEdit::textChanged,this,[&](QString text){NewDatabase.Password = text;});
    connect(ui->keyGenerator,&QPushButton::clicked,this,[=](){
        KeyGenerator * key = new KeyGenerator(this);
        connect(key,&KeyGenerator::password,this,[=](QString password){
            ui->passwordLine->setText(password);
        });
        key->show();
        key->activateWindow();
    });
    connect(this,&QWizard::currentIdChanged,this,[=](int page){
        if(page==1){
            if(DatabaseInfoWidgetsList.count()>0)
                return;
            QList<QStringList> Info = SqlFunctions::GetRecordList(QString("SELECT Id,Name,DataType,VisibleInTable "
                                                                          "FROM Metadata WHERE Basic=1 AND DatabaseType = %1;").arg(QString::number(NewDatabase.Type)),DataTex::DataTeX_Settings);
            int index = 0;
            for(QStringList list : qAsConst(Info)){
                DatabaseFieldInfoWidget * Field = new DatabaseFieldInfoWidget(this,true);
                Field->Id->setText(list[0]);
                Field->NameDescription->setText(list[1]);
                Field->DataTypes->setCurrentText(list[2]);
                Field->VisibleInTable->setChecked(list[3].toInt());
                Field->VisibleInTable->setEnabled(false);
                ui->verticalLayout_2->addWidget(Field);
                Field->NameDescription->setProperty("index",index);
                DatabaseInfoWidgetsList.append(Field);
                index++;
            }
        }
        else if(page==2){
            ui->splitter->setSizes(QList<int>({1,200}));
            NewDatabase.DBFieldInfoList.clear();
            for(DatabaseFieldInfoWidget * field : qAsConst(DatabaseInfoWidgetsList)){
                NewDatabase.DBFieldInfoList.append(field->getFieldInfoList()); //Όταν δημιουργείται η βάση να αποθηκεύονται τα DTXDBFiledInfo σε List
               // qDebug()<<field->getFieldInfoList().Id;
            }

            QFile file(":/databases/FileTypesData.json");

            if (!file.open(QFile::ReadOnly | QFile::Text))
                return;

            const QJsonObject &json(QJsonDocument::fromJson(file.readAll()).object());
            file.close();

            const QJsonArray filedata = json["FileTypes"].toArray();
            int row = 0;
            FileTypeGroup.setExclusive(false);
            foreach (const QJsonValue &info, filedata) {
                DTXFileType filetype;
                const QJsonObject &json(info.toObject());
                filetype.Id = json["Id"].toString().toUtf8().data();
                filetype.Name = tr(json["Name"].toString().toUtf8().data());
                filetype.FolderName = tr(json["Folder"].toString().toUtf8().data());
                filetype.Solvable = json["Solvable"].toVariant().value<DTXSolutionState>();
                filetype.BelongsTo = json["BelongsTo"].toString().toUtf8().data();
                filetype.DBType = json["DatabaseType"].toVariant().value<DTXDatabaseType>();
                filetype.BuiltIn = json["BuiltIn"].toBool();
                filetype.Description = json["Description"].toString().toUtf8().data();
                FileTypeList.append(filetype);
                if(filetype.DBType == NewDatabase.Type && filetype.Solvable != DTXSolutionState::Solution){
                    QCheckBox * item = new QCheckBox(filetype.Name,this);
                    QLabel * description = new QLabel(filetype.Description,this);
                    description->setWordWrap(true);
                    item->setProperty("data",QVariant::fromValue(filetype));
                    item->setCheckState(Qt::Unchecked);
                    ui->gridLayout_8->addWidget(item,row,0);
                    ui->gridLayout_8->addWidget(description,row,1);
                    row++;
                    FileTypeGroup.addButton(item);
                }
            }
            QSpacerItem *spacer = new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
            ui->gridLayout_8->addItem(spacer,row,0);
            QJsonArray templates = json["Templates"].toArray();
            foreach (const QJsonValue &temp, templates) {
                const QJsonObject &jstmpl(temp.toObject());
                QStringList keys = jstmpl.keys();
                for(const QString &key : qAsConst(keys)){
                    QRadioButton * topic = new QRadioButton(key,this);
                    ui->verticalLayout->addWidget(topic);
                    const QJsonArray item = jstmpl.value(key).toArray();
                    QStringList filetypes;
                    for(const QJsonValue &ft : item){
                        filetypes.append(ft.toString());
                    }
                    connect(topic,&QRadioButton::toggled,this,[=](bool checked){
                        for(QAbstractButton * bt : FileTypeGroup.buttons()){
                            QString id = bt->property("data").value<DTXFileType>().Id;
                            bt->setChecked(filetypes.contains(id));
                        }
                    });
                }
                QSpacerItem *spacer = new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding);
                ui->verticalLayout->addItem(spacer);
            }
        }
        else if(page==3){
            int i=-1;
            for(QAbstractButton * checkBox : FileTypeGroup.buttons()){
                if(checkBox->isChecked()){
                    i++;
                    DTXFileType filetype = checkBox->property("data").value<DTXFileType>();
                    NewDatabase.FileTypes.append(filetype);
                    // qDebug()<<NewDatabase.FileTypes[i].Name;
                }
            }
        }
    });
    connect(ui->AddButton,&QPushButton::clicked,this,[=](){
        NewFileType * newFileType = new NewFileType(this,(DTXDatabaseType)NewDatabase.Type);
        connect(newFileType,&NewFileType::filedata,this,[=](DTXFileType filetype){
            QCheckBox * newButton = new QCheckBox(filetype.Name,this);
            QPushButton * deleteFileType = new QPushButton(QIcon::fromTheme("EditDelete"),QString(),this);
            QHBoxLayout * hlayout = new QHBoxLayout(this);
            hlayout->setContentsMargins(0,0,0,0);
            QWidget * widget = new QWidget(this);
            widget->setLayout(hlayout);
            hlayout->addWidget(newButton);
            hlayout->addWidget(deleteFileType);
            newButton->setProperty("filetype",QVariant::fromValue(filetype));
            FileTypeList.append(filetype);
            FileTypeGroup.addButton(newButton);

//            QSqlQuery NewFileType(NewDatabase.Database);
//            NewFileType.prepare(QString("INSERT OR IGNORE INTO FileTypes (Id,Name,FolderName,Solvable,Description) "
//                                        "VALUES(:id,:name,:folder,:sol,:desc)"));
//            NewFileType.bindValue(":id",filetype.Id);
//            NewFileType.bindValue(":name",filetype.Name);
//            NewFileType.bindValue(":folder",filetype.FolderName);
//            NewFileType.bindValue(":sol",filetype.Solvable);
//            NewFileType.bindValue(":desc",filetype.Description);
//            NewFileType.exec();
//            if(filetype.Solvable == DTXSolutionState::Solvable){
//                DTXFileType filetypeSol;
//                filetypeSol.Id = filetype.Id+"-Sol";
//                filetypeSol.Name = filetype.Name+tr(" - Solution");
//                filetypeSol.FolderName = filetype.FolderName+tr(" - Solutions");
//                filetypeSol.Solvable = DTXSolutionState::Solution;
//                filetypeSol.BelongsTo = filetype.Id;
//                filetypeSol.Description = tr("Solutions of : ")+filetype.Name;
//                filetypeSol.DBType = (DTXDatabaseType)NewDatabase.Type;
//                filetypeSol.BuiltIn = false;
//                FileTypeList.append(filetypeSol);
//                NewFileType.prepare(QString("INSERT OR IGNORE INTO FileTypes "
//                                            "(Id,Name,FolderName,Solvable,BelongsTo,Description) "
//                                            "VALUES(:id,:name,:folder,-1,:fid,:desc)"));
//                NewFileType.bindValue(":id",filetypeSol.Id);
//                NewFileType.bindValue(":name",filetypeSol.Name);
//                NewFileType.bindValue(":folder",filetypeSol.FolderName);
//                NewFileType.bindValue(":fid",filetype.Id);
//                NewFileType.bindValue(":desc",filetypeSol.Description);
//                NewFileType.exec();
//            }

            ui->gridLayout_8->addWidget(widget,FileTypeList.count(),0,1,1,Qt::AlignLeft);
            QLabel * description = new QLabel(filetype.Description,this);
            description->setWordWrap(true);
            ui->gridLayout_8->addWidget(description,FileTypeList.count(),1,1,1,Qt::AlignLeft);
        });
        newFileType->show();
        newFileType->activateWindow();
    });
    connect(ui->AddField,&QPushButton::clicked,this,[&](){
        DatabaseFieldInfoWidget * Field = new DatabaseFieldInfoWidget(this,false);
        DatabaseInfoWidgetsList.append(Field);
        ui->verticalLayout_2->addWidget(Field);
    });
    connect(ui->RemoveField,&QPushButton::clicked,this,[&](){
        for(DatabaseFieldInfoWidget * field : qAsConst(DatabaseInfoWidgetsList)){
            if(!field->isBasic){
                if(field->checkRemove->isChecked()){
                    DatabaseInfoWidgetsList.removeOne(field);
                    ui->verticalLayout_2->removeWidget(field);
                    delete field;
                }
            }
        }
    });
}

DatabaseCreator::~DatabaseCreator()
{
    delete ui;
}

void DatabaseCreator::CheckDatabase()
{
    if(DatabaseList.contains(ui->DatabaseFileName->text())){
        ui->CheckDatabaseName->setText(tr("The database %1 already exists.").arg(ui->DatabaseFileName->text()));
        button(QWizard::NextButton)->setEnabled(false);
    }
    else{
        ui->CheckDatabaseName->clear();
    }
}

void DatabaseCreator::isPageReady()
{
    switch(currentId()){
    case 0:
        isNextReady = (NewDatabase.Type > -1);
        break;
    case 1:
        isNextReady = (!ui->DatabaseFileName->text().isEmpty() &&
                       !ui->DatabasePath->text().isEmpty() &&
                       !ui->DatabaseName->text().isEmpty() &&
                       ui->UsePrefix->isChecked() == !ui->prefix->text().isEmpty() &&
                       ui->encryptBox->isChecked() == (!ui->userName->text().isEmpty()
                        && !ui->passwordLine->text().isEmpty()));
        break;
    }
    button(QWizard::NextButton)->setEnabled(isNextReady);
}

bool DatabaseCreator::ItemHasTopic(QListWidgetItem * item)
{
//    item->data(Qt::UserRole)
}

void DatabaseCreator::on_DatabaseCreator_accepted()
{
    QString FullPath = NewDatabase.Path+QDir::separator()+NewDatabase.Description+QDir::separator()+NewDatabase.BaseName+".db";
    QSqlQuery AddNewDatabase(DataTex::DataTeX_Settings);
    if(ui->encryptBox->isChecked()){
        const QByteArray pass = NewDatabase.Password.toUtf8();
        NewDatabase.Password = QCryptographicHash::hash(pass,QCryptographicHash::Sha256);
        AddNewDatabase.exec(
            QString("INSERT INTO \"DataBases\" (FileName, Name,Type, Path,Prefix,UserName,PassWord,IsConnected) "
                    "VALUES (\"%1\", \"%2\", \"%3\",\"%4\",\"%5\",\"%6\",\"%7\",1);")
                .arg(NewDatabase.BaseName,NewDatabase.Description,QString::number(NewDatabase.Type)
                     ,FullPath,NewDatabase.Prefix,NewDatabase.Username,NewDatabase.Password));
    }
    else{
        NewDatabase.Password = QString();
        AddNewDatabase.exec(
            QString("INSERT INTO \"DataBases\" (FileName, Name,Type,Path,Prefix,IsConnected) "
                    "VALUES (\"%1\", \"%2\", \"%3\",\"%4\",\"%5\",1);")
                .arg(NewDatabase.BaseName,NewDatabase.Description,QString::number(NewDatabase.Type)
                     ,FullPath,NewDatabase.Prefix));
    }

    QSqlQuery Metadata_1(DataTex::DataTeX_Settings);
    QSqlQuery Metadata_2(DataTex::DataTeX_Settings);
    for (const DTXDBFieldInfo &info : qAsConst(NewDatabase.DBFieldInfoList)) {
        // qDebug()<<info.isBasic;
        // if(!widget->isBasic){
        QString MetadataQuery_1 = "INSERT OR IGNORE INTO Metadata (Id,DatabaseType,Name,Basic,DataType,VisibleInTable) VALUES ";
        MetadataQuery_1 += "(\""+info.Id+"\",'"+QString::number(NewDatabase.Type)+"',\""
                           +info.Name+"\",'0','"+info.DataType+"','1')";
        // qDebug()<<MetadataQuery_1;
        Metadata_1.exec(MetadataQuery_1);
        // }

        QString MetadataQuery_2 =
            "INSERT INTO Metadata_per_Database (Database_FileName,DatabaseType,Metadata_Id,Metadata_Name) VALUES ";
        MetadataQuery_2 += "(\""+NewDatabase.BaseName+"\",\""+
                           QString::number(NewDatabase.Type)+"\",\""+
                           info.Id+"\",\""+
                           info.Name+"\")";
        Metadata_2.exec(MetadataQuery_2);
    }

    QDir basedir(NewDatabase.Path+QDir::separator()+NewDatabase.Description+QDir::separator());
    if(!basedir.exists()){basedir.mkpath(".");}
    QSqlDatabase newdatabaseFile;
    newdatabaseFile = QSqlDatabase::addDatabase("QSQLITE","newTempDatabase");
    if(!QFileInfo::exists(FullPath)){
        QFile DB(":/databases/"+DBTemplateFileName+".db");
        DB.copy(FullPath);
        QFile(FullPath).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
    newdatabaseFile.setDatabaseName(FullPath);
    // NewDatabase.Database = newdatabaseFile;
    newdatabaseFile.open();

    for (const DTXFileType &filetype : qAsConst(NewDatabase.FileTypes)){
        NewFileType::CreateNewDatabaseFileType(newdatabaseFile,NewDatabase.Type,filetype);
    }

    QSqlQuery AddExtraMetadata(newdatabaseFile);
        QString Table;
        switch (NewDatabase.Type) {
        case DTXDatabaseType::FilesDB:
            Table = "Database_Files";
            break;
        case DTXDatabaseType::DocumentsDB:
            Table = "Documents";
            break;
        case DTXDatabaseType::BibliographyDB:

            break;
        case DTXDatabaseType::TablesDB:

            break;
        case DTXDatabaseType::FiguresDB:

            break;
        case DTXDatabaseType::CommandsDB:

            break;
        case DTXDatabaseType::PreamblesDB:

            break;
        case DTXDatabaseType::PackagesDB:

            break;
        case DTXDatabaseType::ClassesDB:

            break;
        }
        QStringList additionalColumns;

        for (DatabaseFieldInfoWidget * field : qAsConst(DatabaseInfoWidgetsList)) {
            if(!field->isBasic){
                QString query = "ALTER TABLE \""+Table+"\" ADD COLUMN \""+field->CustomId->text()+"\" "+field->DataTypes->currentText();
                // qDebug()<<query;
                AddExtraMetadata.exec(query);
                additionalColumns.append(field->NameDescription->text());
            }
        }
        if(additionalColumns.count()>0){
            QString query;
            QFile file(":/databases/FilesDBView.sql");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream queryStream(&file);
            query = queryStream.readAll();
            file.close();

            query.replace("<additional_columns>",additionalColumns.join(","));
            qDebug()<<"View query = "<<query;
            AddExtraMetadata.exec(query);
        }

    QSqlQuery BackUp(newdatabaseFile);
    QString BackUpMetadata = "INSERT INTO BackUp (Id,Name,Basic,DataType,VisibleInTable) VALUES ";
    QStringList BackUpMeta_Query;
    for (const DTXDBFieldInfo &info : qAsConst(NewDatabase.DBFieldInfoList)) {
        BackUpMeta_Query.append("(\""+info.Id+"\",\""+info.Name+"\",\""
                                +QString::number(info.isBasic)+"\",\""+info.DataType+"\",\""
                                +QString::number(info.isVisibleInTable)+"\")");
    }
    BackUpMetadata +=BackUpMeta_Query.join(",");
    BackUp.exec(BackUpMetadata);

    QSqlQuery WriteFileTypes(newdatabaseFile);
    for (const DTXFileType &filetype : qAsConst(NewDatabase.FileTypes)) {
        WriteFileTypes.exec("INSERT INTO FileTypes (Id,Name,FolderName,Solvable,BelongsTo,Description) VALUES (\""
                            +filetype.Id+"\",\""+filetype.Name+"\",\""+filetype.FolderName+"\",\""
                            +QString::number((int)filetype.Solvable)+"\",\""+filetype.BelongsTo+"\",\""
                            +filetype.Description+"\")");
    }

    newdatabaseFile.close();

    emit newbase(NewDatabase);
//    QDialog::accept();
}

DTXFileType::DTXFileType(){}

DTXFileType::DTXFileType(QStringList list)
{
    Id = list.at(0);
    Name = list.at(1);
    FolderName = list.at(2);
    Solvable = (DTXSolutionState)QString(list.at(3)).toInt();
    BelongsTo = list.at(4);
    // Description = list.at(5);
}

DTXDatabase DTXDatabaseInfo::getDTXDatabase()
{
    if(DataTex::GlobalDatabaseList.value(Id).Type == Type){
        return DataTex::GlobalDatabaseList.value(Id);
    }
}
