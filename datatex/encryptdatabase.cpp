#include "encryptdatabase.h"
#include "ui_encryptdatabase.h"
#include "datatex.h"
#include "keygenerator.h"

EncryptDatabase::EncryptDatabase(QWidget *parent,DTXDatabase database) :
    QDialog(parent),
    ui(new Ui::EncryptDatabase)
{
    ui->setupUi(this);
    QStringList DatabaseTypes = {tr("Files database"),tr("Documents database"),tr("Bibliography database"),
                                tr("Tables database"),tr("Figures database"),tr("Commands database"),
                                tr("Preambles database"),tr("Packages database"),tr("Classes database")};
    for(int i=0;i<DatabaseTypes.count();i++){
        ui->DBTypeCombo->addItem(DatabaseTypes[i],i);
    }
    for(DTXDatabase &db : DataTex::GlobalDatabaseList.values()){
        if(db.Type == database.Type){
            ui->DBNameCombo->addItem(db.Description,db.BaseName);
        }
    }

    ui->DBNameCombo->model()->sort(0);
    connect(ui->DBTypeCombo,QOverload<int>::of(&QComboBox::activated),this,[=](int index){
        ui->DBNameCombo->clear();
        for(DTXDatabase &db : DataTex::GlobalDatabaseList.values()){
            if(db.Type == index){
                ui->DBNameCombo->addItem(db.Description);
            }
        }
        ui->DBNameCombo->model()->sort(0);
    });
    ui->DBTypeCombo->setCurrentIndex(database.Type);
    ui->DBNameCombo->setCurrentText(database.Description);

//    if(database.Encrypt){
//        ui->UsernameLine->setText(database.Username);
//        ui->PasswordLine->setText(database.Password);
//    }
    connect(ui->SetKey,&QPushButton::clicked,this,[=](){
        KeyGenerator * key = new KeyGenerator(this);
        connect(key,&KeyGenerator::password,this,[=](QString password){
            ui->PasswordLine->setText(password);
        });
        key->show();
        key->activateWindow();
    });
    connect(ui->OkButton,&QDialogButtonBox::accepted,this,[&](){
        QString db = ui->DBNameCombo->currentData().toString();

        DataTex::GlobalDatabaseList[db].Username = ui->UsernameLine->text();
        const QByteArray password = ui->PasswordLine->text().toUtf8();
        QString HashedPassword = QCryptographicHash::hash(password,QCryptographicHash::Sha256);
        DataTex::GlobalDatabaseList[db].Password = HashedPassword;//Κρυπτογράφηση
        QSqlQuery fdb_encription(DataTex::DataTeX_Settings);
        fdb_encription.exec(QString("UPDATE DataBases SET UserName = '%1' WHERE FileName = '%2'")
                                .arg(ui->UsernameLine->text(),db));
        fdb_encription.exec(QString("UPDATE DataBases SET PassWord = '%1' WHERE FileName = '%2'")
                                .arg(HashedPassword,db));
        if(ui->LockDatabase->isChecked()){
            emit DBEncrypted(db);
        }
    });
}

EncryptDatabase::~EncryptDatabase()
{
    delete ui;
}
