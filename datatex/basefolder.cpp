#include "basefolder.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QWizardPage>
#include <QHeaderView>
#include "sqlfunctions.h"
#include "datatex.h"


QStringList BaseFolder::fields;
QStringList BaseFolder::bibfields;
QStringList BaseFolder::FieldTypesValues;
QStringList BaseFolder::BibTypesValues;
QString BaseFolder::DatabaseType;
QList<QCheckBox *> DataPage::newCheckList;
QList<QLineEdit *> DataPage::newlabelList;
QList<QLineEdit * >  DataPage::newFieldLineList;
QList<int> DataPage::addedIdList;
QList<int> DataPage::removedIdList;
//QList<QCheckBox *> BibliographyPage::newBibCheckList;
//QList<QLineEdit *> BibliographyPage::newBiblabelList;
//QList<QLineEdit * >  BibliographyPage::newBibLineList;
//QList<int> BibliographyPage::addedBibIdList;
//QList<int> BibliographyPage::removedBibIdList;
QList<QComboBox *> DataPage::newcomboList = QList<QComboBox *>();
//QList<QComboBox *> BibliographyPage::newcomboList = QList<QComboBox *>();
QStringList BaseFolder::Metadata;
//QStringList BaseFolder::BibData;
QStringList BaseFolder::MetadataNames;
//QStringList BaseFolder::BibDataNames;
QTableWidget * FinalPage::table2;
QRadioButton * BaseFolder::FilesDB;
QRadioButton * BaseFolder::DocsDB;
QString BaseFolder::Prefix;
QString BaseFolder::UserName;
QString BaseFolder::PassWord;

/* CODE BELONGS TO Gwenaël Cléon FROM THE PROGRAM TeXoMaker  */
BaseFolder::BaseFolder(QWidget *parent) :
    QWizard(parent)/*,
    ui(new Ui::BaseFolder)*/
{
    BaseFolder::fields.clear();
    BaseFolder::FieldTypesValues.clear();
    DataPage::newlabelList.clear();
    DataPage::newFieldLineList.clear();
    DataPage::newCheckList.clear();
    DataPage::newcomboList.clear();
//    BibliographyPage::newBibCheckList.clear();
//    BibliographyPage::newBibLineList.clear();
//    BibliographyPage::newBiblabelList.clear();
    BaseFolder::bibfields.clear();
    BaseFolder::BibTypesValues.clear();

    enum { Info, Data, Docs, Final};

    BaseFolder::DatabaseType = "Files";
    setPage(Info,new InfoPage);
    setPage(Data, new DataPage);
//    setPage(Bibliography,new BibliographyPage);
    setPage(Final,new FinalPage);
    setWindowTitle("New Database");
    QAbstractButton * backButton = button(QWizard::BackButton);
    connect(backButton, &QAbstractButton::pressed,this,[=](){back();});

}

void BaseFolder::back() const
{
    if(BaseFolder::DatabaseType == "Files" && currentId() == BaseFolder::Final){
        delete FinalPage::table2;
    }
}

InfoPage::InfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    QSqlQuery DatabaseListQuery(DataTex::DataTeX_Settings);
    DatabaseListQuery.exec("SELECT FileName FROM Databases");
    while (DatabaseListQuery.next()) {
        DatabaseList.append(DatabaseListQuery.value(0).toString());
    }
    setTitle("Create new Database");
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/datatex.png"));
    QLabel *label = new QLabel(tr("This wizard will help you create a new LaTeX database for DataTex."));
    label->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QHBoxLayout *hlayout2 = new QHBoxLayout(this);
    QHBoxLayout *hlayout3 = new QHBoxLayout(this);

    QHBoxLayout *prefixLayout = new QHBoxLayout(this);
    QHBoxLayout *userNameLayout = new QHBoxLayout(this);
    QHBoxLayout *passwordLayout = new QHBoxLayout(this);
    QLabel *user = new QLabel(tr("Username"),this);
    QLabel *password = new QLabel(tr("Password"),this);
    QCheckBox *usePrefixBox = new QCheckBox(tr("Use prefix for this database"),this);
    prefix = new QLineEdit(this);
    prefix->setEnabled(false);
    QCheckBox *encryptBox = new QCheckBox(tr("Encrypt this database"),this);
    QPushButton *keyGenerator = new QPushButton(QIcon::fromTheme("Key"),QString(),this);
    userName = new QLineEdit(this);
    passwordLine = new PasswordLineEdit(this);
    keyGenerator->setEnabled(false);
    userName->setEnabled(false);
    passwordLine->setEnabled(false);

    connect(usePrefixBox,&QCheckBox::toggled,this,[=](bool checked){
        usePrefix = checked == !prefix->text().isEmpty();
        prefix->setEnabled(checked);
        emit completeChanged();
    });
    connect(prefix,&QLineEdit::textChanged,this,[=](QString text){
        usePrefix = usePrefixBox->isChecked() == !text.isEmpty();
        emit completeChanged();
    });
    connect(encryptBox,&QCheckBox::toggled,this,[=](bool checked){
        encryptDatabase = (checked == !userName->text().isEmpty()) &&
                          (!userName->text().isEmpty() == !passwordLine->text().isEmpty());
        // qDebug()<<checked<<!userName->text().isEmpty()<<!passwordLine->text().isEmpty();
        // qDebug()<<QWizardPage::isComplete()<<usePrefix<<encryptDatabase;
        // qDebug()<<((checked == !userName->text().isEmpty()) == !passwordLine->text().isEmpty());
        // qDebug()<<(QWizardPage::isComplete() && usePrefix && encryptDatabase);
        emit completeChanged();
        keyGenerator->setEnabled(checked);
        userName->setEnabled(checked);
        passwordLine->setEnabled(checked);
    });
    connect(keyGenerator,&QPushButton::clicked,this,[=](){
        KeyGenerator * key = new KeyGenerator(this);
        connect(key,&KeyGenerator::password,this,[=](QString password){
            passwordLine->setText(password);
        });
        key->show();
        key->activateWindow();
    });
    connect(userName,&QLineEdit::textChanged,this,[=](QString text){
        encryptDatabase = (encryptBox->isChecked() == !text.isEmpty()) &&
                          (!text.isEmpty() == !passwordLine->text().isEmpty());
        emit completeChanged();
//        emit readyChanged(text.isEmpty());
    });
    connect(passwordLine,&QLineEdit::textChanged,this,[=](QString text){
        encryptDatabase = (encryptBox->isChecked() == !userName->text().isEmpty()) &&
                          (!userName->text().isEmpty() == !text.isEmpty());
//        emit readyChanged(text.isEmpty());
        emit completeChanged();
    });
//    connect(this,&InfoPage::readyChanged, this,[=](){
//        emit completeChanged();
//    });
    prefixLayout->addWidget(usePrefixBox);
    prefixLayout->addWidget(prefix);
    userNameLayout->addWidget(user);
    userNameLayout->addWidget(userName);
    passwordLayout->addWidget(password);
    passwordLayout->addWidget(keyGenerator);
    passwordLayout->addWidget(passwordLine);

    BaseFolder::FilesDB = new QRadioButton(tr("Files Database"));
    BaseFolder::DocsDB = new QRadioButton(tr("Document Database"));
    QHBoxLayout * RadioLH = new QHBoxLayout(this);
    BaseFolder::FilesDB->setIcon(QIcon::fromTheme("tex"));
    BaseFolder::DocsDB->setIcon(QIcon::fromTheme("pdf"));
    BaseFolder::FilesDB->setChecked(true);
    RadioLH->addWidget(BaseFolder::FilesDB);
    RadioLH->addWidget(BaseFolder::DocsDB);
    RadioLH->addStretch();
    QLabel * DatabasePathLabel = new QLabel(tr("DataBase Path"),this);
    DatabasePath = new QLineEdit(this);
    QLabel * DatabaseFileNameLabel = new QLabel(tr("DataBase file name"),this);
    DatabaseFileName = new QLineEdit(this);
    QLabel * DatabaseNameLabel = new QLabel(tr("DataBase name - Short Description"),this);
    DatabaseName = new QLineEdit(this);
    DatabasePathButton = new QPushButton(tr("Path"),this);
    DatabasePathButton->setFocus();
    layout->addWidget(label);
    hlayout->addWidget(DatabasePathLabel);
    hlayout->addWidget(DatabasePath);
    hlayout->addWidget(DatabasePathButton);
    layout->addLayout(hlayout);
    layout->addLayout(RadioLH);
    hlayout2->addWidget(DatabaseFileNameLabel);
    hlayout2->addWidget(DatabaseFileName);
    layout->addLayout(hlayout2);
    hlayout3->addWidget(DatabaseNameLabel);
    hlayout3->addWidget(DatabaseName);
    layout->addLayout(hlayout3);
    CheckDatabaseName = new QLabel(this);
    QFont font = CheckDatabaseName->font();
    font.setPointSize(11);
    font.setBold(true);
    CheckDatabaseName->setFont(font);
    CheckDatabaseName->setStyleSheet("QLabel {color : red; }");
    layout->addWidget(CheckDatabaseName);
    layout->addLayout(prefixLayout);
    layout->addWidget(encryptBox);
    layout->addLayout(userNameLayout);
    layout->addLayout(passwordLayout);
    setLayout(layout);
    registerField("DataΒaseFileName*", DatabaseFileName);
    registerField("DataBasePath*", DatabasePath);
    registerField("DataBaseName*", DatabaseName);
//    if(usePrefix){
//        registerField("UsePreFix*", prefix);
//    }
//    if(encryptDatabase){
//        registerField("UserName*", userName);
//        registerField("Password*", passwordLine);
//    }
    connect(DatabasePathButton,&QPushButton::pressed,this,&InfoPage::Database_Path);
    setMinimumHeight(400);
    setMinimumWidth(700);
    layout->addStretch();
    connect(DatabaseFileName,&QLineEdit::textChanged,this, &InfoPage::CheckDatabase);
}

bool InfoPage::isComplete() const
{
    return QWizardPage::isComplete() && usePrefix && encryptDatabase;
}

void InfoPage::CheckDatabase()
{
    if(DatabaseList.contains(DatabaseFileName->text())){
        CheckDatabaseName->setText(tr("The database %1 already exists.").arg(DatabaseFileName->text()));
        wizard()->button(QWizard::NextButton)->setEnabled(false);
    }
    else{
        CheckDatabaseName->clear();
    }
}

void InfoPage::Database_Path()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("Select a folder for your new database"),QDir::homePath());
    DatabasePath->setText(path);
}

int InfoPage::nextId() const
{
    if (BaseFolder::FilesDB->isChecked()) {
        BaseFolder::DatabaseType = "Files";
        return BaseFolder::Data;
    } else {
        BaseFolder::DatabaseType = "Documents";
    }
    if(usePrefix){
        BaseFolder::Prefix = prefix->text();
    }
    if(encryptDatabase){
        BaseFolder::UserName = userName->text();
        BaseFolder::PassWord = passwordLine->text();
    }
    return BaseFolder::Data;
}

DataPage::DataPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Database fields");
        QLabel *label = new QLabel(tr("Below you can see the basic fields of the Database files table."
                                   "Select a name for each field of your new database."
                                  "Add new custom fields by selecting an id, a name and a type."));
        label->setWordWrap(true);
        QVBoxLayout *layout2 = new QVBoxLayout;
        layout2->addWidget(label);
        setLayout(layout2);
        DataLayout = new QGridLayout;
        QWidget *client = new QWidget;
        QScrollArea *scrollArea = new QScrollArea;
        layout2->addWidget(scrollArea);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(client);
        client->setLayout(DataLayout);
        FieldTypes <<"TEXT"<<"INTEGER"<<"BLOB"<<"REAL"<<"NUMERIC";
        QFrame* hFrame = new QFrame;
        hFrame->setFrameShape(QFrame::HLine);
        DataLayout->addWidget(hFrame,0,0,1,3);
        DataLayout->addWidget(new QLabel(tr("Field id")),1,0);
        DataLayout->addWidget(new QLabel(tr("Field mame")),1,1);
        DataLayout->addWidget(new QLabel(tr("Data type")),1,2);
        DataLayout->addWidget(hFrame,2,0,1,3);
        AddFieldButton = new QPushButton(this);
        RemoveFieldButton = new QPushButton(this);
        AddFieldButton->setIcon(QIcon::fromTheme("list-add"));
        RemoveFieldButton->setIcon(QIcon::fromTheme("list-remove"));
        CheckFieldId = new QLabel(this);
        QHBoxLayout *layout3 = new QHBoxLayout;
        layout2->addLayout(layout3);
        layout3->addWidget(AddFieldButton);
        layout3->addWidget(RemoveFieldButton);
        layout3->addWidget(CheckFieldId);
        layout3->addStretch();
        setMinimumHeight(400);
        setMinimumWidth(700);
        connect(AddFieldButton,SIGNAL(clicked()),this,SLOT(AddField()));
        connect(RemoveFieldButton,SIGNAL(clicked()),this,SLOT(RemoveField()));
        optfield = -1;
}

void DataPage::initializePage()
{
    BasicDataBaseFields.clear();
    BasicDataBaseValues.clear();
    if(labelList.count()>0){
        for (int i=0;i<labelList.count();i++) {
            delete labelList[i];
            delete lineList[i];
            delete basiccomboList[i];
        }
    }
    if(newlabelList.count()>0){
        for (int i=0;i<newlabelList.count();i++) {
            delete newlabelList[i];
            delete newFieldLineList[i];
            delete newcomboList[i];
            delete newCheckList[i];
        }
    }
    labelList.clear();
    lineList.clear();
    basiccomboList.clear();
    BaseFolder::fields.clear();
    BaseFolder::FieldTypesValues.clear();
    QString Table;
    if (BaseFolder::FilesDB->isChecked()) {
        Table = "Metadata";
    } else {
        Table = "DocMetadata";
    }

    QSqlQuery Select_DataBase_Metadata(DataTex::DataTeX_Settings);
    Select_DataBase_Metadata.exec(QString("SELECT Id,Name,DataType FROM "+Table+" WHERE Basic=1;"));
    while(Select_DataBase_Metadata.next()){
        BasicDataBaseFields.append(Select_DataBase_Metadata.value(0).toString());
        BasicDataBaseValues.append(Select_DataBase_Metadata.value(1).toString());
    }

    for (int i=0;i<BasicDataBaseFields.size();i++) {
        QLabel * label = new QLabel(BasicDataBaseFields.at(i),this);
        QLineEdit * line = new QLineEdit(this);
        QComboBox * combo = new QComboBox(this);
        labelList.append(label);
        lineList.append(line);
        line->setMinimumWidth(400);
        combo->addItems(FieldTypes);
        combo->setEnabled(false);
        basiccomboList.append(combo);
        labelList.at(i)->setBuddy(lineList.at(i));
        DataLayout->addWidget(basiccomboList.at(i),i+3,2);
        BaseFolder::fields.append(label->text());
        QString rlabel = BaseFolder::DatabaseType+label->text();
        registerField(rlabel, line);
        QString rcombo = BaseFolder::DatabaseType+"Combo"+label->text();
        BaseFolder::FieldTypesValues.append(rcombo);
        registerField(rcombo, combo,"currentText", "currentTextChanged");
        DataLayout->addWidget(labelList.at(i),i+3,0);
        DataLayout->addWidget(lineList.at(i),i+3,1);
        lineList.at(i)->setText(BasicDataBaseValues.at(i));
    }
    if(BaseFolder::DatabaseType == "Files"){
        basiccomboList.at(6)->setCurrentIndex(1);
    }
    else{
        basiccomboList.at(10)->setCurrentIndex(1);
    }
}

void DataPage::AddField()
{
    optfield++;
    DataPage::addedIdList.append(optfield);
    int basicFields = BasicDataBaseFields.count();
    int newlabels = DataPage::newlabelList.count();
    QLineEdit * line1 = new QLineEdit(this);
    QLineEdit * line2 = new QLineEdit(this);
    QComboBox * combo = new QComboBox(this);
    QHBoxLayout *hlayout = new QHBoxLayout;
    QCheckBox * check = new QCheckBox(this);
    line1->setPlaceholderText(tr("New field"));
    line2->setPlaceholderText(tr("Type the field's name"));
    line1->setClearButtonEnabled(true);
    line2->setClearButtonEnabled(true);
    DataLayout->addLayout(hlayout,basicFields+newlabels+3,0);
    hlayout->addWidget(check);
    hlayout->addWidget(line1);
    DataLayout->addWidget(line2,basicFields+newlabels+3,1);
    DataLayout->addWidget(combo,basicFields+newlabels+3,2);
    combo->addItems(FieldTypes);
    QString rlabel = "optField_"+QString::number(optfield)+"*";
    registerField(rlabel, line1);
    QString rlabel2 = "optValue_"+QString::number(optfield)+"*";
    registerField(rlabel2, line2);
    QString rcombo = "optCombo_"+QString::number(optfield);
    BaseFolder::FieldTypesValues.append(rcombo);
    registerField(rcombo, combo,"currentText",SIGNAL(currentIndexChanged(QString)));
    newlabelList.append(line1);
    newFieldLineList.append(line2);
    newCheckList.append(check);
    newcomboList.append(combo);
    checkGroup->addButton(check);
    checkGroup->setExclusive(false);
    check->setProperty("Id",optfield);
    for (int i=0;i<newlabelList.count();i++ ) {
        connect(newlabelList.at(i), &QLineEdit::textChanged, this, &DataPage::CheckNewField);
        connect(newlabelList.at(i), &QLineEdit::textChanged, this, &DataPage::CheckNext);
        connect(newFieldLineList.at(i), &QLineEdit::textChanged, this, &DataPage::CheckNext);
    }
    wizard()->button(QWizard::NextButton)->setEnabled(false);
}

void DataPage::CheckNewField(QString text)
{
    QSet<QString> NoDuplicates;
    FieldList.clear();
    NoDuplicates.clear();
    for (int i=0;i<newlabelList.count();i++) {
        FieldList.append(newlabelList.at(i)->text());
        NoDuplicates.insert(newlabelList.at(i)->text());
    }
    if(BaseFolder::fields.contains(text) || NoDuplicates.count()<newlabelList.count()){
        next = false;
        CheckFieldId->setText(tr("The field %1 already exists.").arg(text));
        wizard()->button(QWizard::NextButton)->setEnabled(false);
    }
    else{
        next = true;
        CheckFieldId->clear();
    }
}

void DataPage::CheckNext()
{
    for (int i=0;i<newlabelList.count();i++) {
        if(newlabelList.at(i)->text().isEmpty() || newFieldLineList.at(i)->text().isEmpty() || next==false){
            wizard()->button(QWizard::NextButton)->setEnabled(false);
        }
        else{wizard()->button(QWizard::NextButton)->setEnabled(true);}
    }
}

void DataPage::RemoveField()
{
    QList<int> CheckIdList;
    for (int i=DataPage::newCheckList.count()-1;i>-1;i--) {
        if(DataPage::newCheckList.at(i)->isChecked()){
            CheckIdList.append(i);
            DataPage::removedIdList.append(DataPage::newCheckList.at(i)->property("Id").toInt());
        }
    }
    for(int i:CheckIdList) {
            DataLayout->removeWidget(DataPage::newlabelList.at(i));
            DataLayout->removeWidget(DataPage::newFieldLineList.at(i));
            DataLayout->removeWidget(DataPage::newCheckList.at(i));
            DataLayout->removeWidget(newcomboList.at(i));
            delete DataPage::newCheckList.takeAt(i);
            delete newlabelList.takeAt(i);
            delete DataPage::newFieldLineList.takeAt(i);
            delete newcomboList.takeAt(i);
        }
}

//int DataPage::nextId() const
//{
//    if (BaseFolder::DatabaseType == "Files") {
//        return BaseFolder::Bibliography;
//    } else {
//        return BaseFolder::Final;
//    }
//}

//BibliographyPage::BibliographyPage(QWidget *parent)
//    : QWizardPage(parent)
//{
//    setTitle("Bibliography fields");
//        QLabel *label = new QLabel("Select...");
//        label->setWordWrap(true);
//        QVBoxLayout *layout2 = new QVBoxLayout;
//        layout2->addWidget(label);
//        setLayout(layout2);
//        CheckFieldId = new QLabel(this);
//        BibliographyLayout = new QGridLayout;
//        QWidget *client = new QWidget;
//        QScrollArea *scrollArea = new QScrollArea;
//        layout2->addWidget(scrollArea);
//        scrollArea->setWidgetResizable(true);
//        scrollArea->setWidget(client);
//        client->setLayout(BibliographyLayout);
//        FieldTypes <<"TEXT"<<"INTEGER"<<"BLOB"<<"REAL"<<"NUMERIC";
//        QSqlQuery Select_DataBase_Metadata(DataTex::DataTeX_Settings);
//        Select_DataBase_Metadata.exec(QString("SELECT Id,Name FROM Bibliography WHERE Basic=1"));
//        while(Select_DataBase_Metadata.next()){
//            BasicBibliographyFields.append(Select_DataBase_Metadata.value(0).toString());
//            BasicBibliographyValues.append(Select_DataBase_Metadata.value(1).toString());
//        }
//        QFrame* hFrame = new QFrame;
//        hFrame->setFrameShape(QFrame::HLine);
//        BibliographyLayout->addWidget(hFrame,0,0,1,3);
//        BibliographyLayout->addWidget(new QLabel(tr("Πεδίο")),1,0);
//        BibliographyLayout->addWidget(new QLabel(tr("Όνομα πεδίου")),1,1);
//        BibliographyLayout->addWidget(new QLabel(tr("Τύπος")),1,2);
//        BibliographyLayout->addWidget(hFrame,2,0,1,3);

//        for (int i=0;i<BasicBibliographyFields.size();i++) {
//            QLabel * label = new QLabel(BasicBibliographyFields.at(i),this);
//            QLineEdit * line = new QLineEdit(this);
//            QComboBox * combo = new QComboBox(this);
//            labelList.append(label);
//            lineList.append(line);
//            line->setMinimumWidth(400);
//            combo->addItems(FieldTypes);
//            combo->setEnabled(false);
//            basiccomboList.append(combo);
//            labelList.at(i)->setBuddy(lineList.at(i));
//            BibliographyLayout->addWidget(basiccomboList.at(i),i+3,2);
//            BaseFolder::bibfields.append(label->text());
//            QString rlabel = "Bib_"+label->text();
//            registerField(rlabel, line);
//            QString rcombo = "Bib_Combo"+label->text();
//            BaseFolder::BibTypesValues.append(rcombo);
//            registerField(rcombo, combo,"currentText", "currentTextChanged");
//            BibliographyLayout->addWidget(labelList.at(i),i+3,0);
//            BibliographyLayout->addWidget(lineList.at(i),i+3,1);
//            lineList.at(i)->setText(BasicBibliographyValues.at(i));
//        }
//        basiccomboList.at(6)->setCurrentIndex(1);
//        basiccomboList.at(7)->setCurrentIndex(1);
//        basiccomboList.at(10)->setCurrentIndex(1);
//        AddBibTexButton = new QPushButton(this);
//        RemoveBibTexButton = new QPushButton(this);
//        AddBibTexButton->setIcon(QIcon::fromTheme("list-add"));
//        RemoveBibTexButton->setIcon(QIcon::fromTheme("list-remove"));
//        QHBoxLayout *layout3 = new QHBoxLayout;
//        layout2->addLayout(layout3);
//        layout3->addWidget(AddBibTexButton);
//        layout3->addWidget(RemoveBibTexButton);
//        layout3->addWidget(CheckFieldId);
//        layout3->addStretch();
//        setMinimumHeight(400);
//        setMinimumWidth(700);
//        connect(AddBibTexButton,SIGNAL(clicked()),this,SLOT(AddBibField()));
//        connect(RemoveBibTexButton,SIGNAL(clicked()),this,SLOT(RemoveBibField()));
//        optfield = -1;
//}

//void BibliographyPage::AddBibField()
//{
//    optfield++;
//    BibliographyPage::addedBibIdList.append(optfield);
//    int basicFields = BasicBibliographyFields.count();
//    int newlabels = BibliographyPage::newBiblabelList.count();
//    QLineEdit * line1 = new QLineEdit(this);
//    QLineEdit * line2 = new QLineEdit(this);
//    QComboBox * combo = new QComboBox(this);
//    QHBoxLayout *hlayout = new QHBoxLayout;
//    QCheckBox * check = new QCheckBox(this);
//    line1->setPlaceholderText(tr("New field"));
//    line2->setPlaceholderText(tr("Type the field's name"));
//    line1->setClearButtonEnabled(true);
//    line2->setClearButtonEnabled(true);
//    BibliographyLayout->addLayout(hlayout,basicFields+newlabels+3,0);
//    hlayout->addWidget(check);
//    hlayout->addWidget(line1);
//    BibliographyLayout->addWidget(line2,basicFields+newlabels+3,1);
//    BibliographyLayout->addWidget(combo,basicFields+newlabels+3,2);
//    combo->addItems(FieldTypes);
//    QString rlabel = "optBibField_"+QString::number(optfield)+"*";
//    registerField(rlabel, line1);
//    QString rlabel2 = "optBibValue_"+QString::number(optfield)+"*";
//    registerField(rlabel2, line2);
//    QString rcombo = "optBibCombo_"+QString::number(optfield);
//    BaseFolder::BibTypesValues.append(rcombo);
//    registerField(rcombo, combo,"currentText",SIGNAL(currentIndexChanged(QString)));
//    newBiblabelList.append(line1);
//    newBibLineList.append(line2);
//    newBibCheckList.append(check);
//    newcomboList.append(combo);
//    checkGroup->addButton(check);
//    checkGroup->setExclusive(false);
//    check->setProperty("Id",optfield);
//    for (int i=0;i<newBiblabelList.count();i++ ) {
//        connect(newBiblabelList.at(i), &QLineEdit::textChanged, this, &BibliographyPage::CheckNewBibField);
//        connect(newBiblabelList.at(i), &QLineEdit::textChanged, this, &BibliographyPage::CheckNext);
//        connect(newBibLineList.at(i), &QLineEdit::textChanged, this, &BibliographyPage::CheckNext);
//    }
//    wizard()->button(QWizard::NextButton)->setEnabled(false);
//}

//void BibliographyPage::CheckNewBibField(QString text)
//{
//    QSet<QString> NoDuplicates;
//    FieldList.clear();
//    NoDuplicates.clear();
//    for (int i=0;i<newBiblabelList.count();i++) {
//        FieldList.append(newBiblabelList.at(i)->text());
//        NoDuplicates.insert(newBiblabelList.at(i)->text());
//    }
//    if(BaseFolder::bibfields.contains(text) || NoDuplicates.count()<newBiblabelList.count()){
//        next = false;
//        CheckFieldId->setText(tr("The field %1 already exists.").arg(text));
//        wizard()->button(QWizard::NextButton)->setEnabled(false);
//    }
//    else{
//        next = true;
//        CheckFieldId->clear();
//    }
//}

//void BibliographyPage::CheckNext()
//{
//    for (int i=0;i<newBiblabelList.count();i++) {
//        if(newBiblabelList.at(i)->text().isEmpty() || newBibLineList.at(i)->text().isEmpty() || next==false){
//            wizard()->button(QWizard::NextButton)->setEnabled(false);
//        }
//        else{wizard()->button(QWizard::NextButton)->setEnabled(true);}
//    }
//}

//void BibliographyPage::RemoveBibField()
//{
//    QList<int> CheckIdList;
//    for (int i=BibliographyPage::newBibCheckList.count()-1;i>-1;i--) {
//        if(BibliographyPage::newBibCheckList.at(i)->isChecked()){
//            CheckIdList.append(i);
//            BibliographyPage::removedBibIdList.append(BibliographyPage::newBibCheckList.at(i)->property("Id").toInt());
//        }
//    }
//    foreach(int i,CheckIdList) {
//            BibliographyLayout->removeWidget(BibliographyPage::newBiblabelList.at(i));
//            BibliographyLayout->removeWidget(BibliographyPage::newBibLineList.at(i));
//            BibliographyLayout->removeWidget(BibliographyPage::newBibCheckList.at(i));
//            BibliographyLayout->removeWidget(newcomboList.at(i));
//            delete BibliographyPage::newBibCheckList.takeAt(i);
//            delete BibliographyPage::newBiblabelList.takeAt(i);
//            delete BibliographyPage::newBibLineList.takeAt(i);
//            delete newcomboList.at(i);
//        }
//}

FinalPage::FinalPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Database fields"));
    NameLabel = new QLabel(this);
    PathLabel = new QLabel(this);
    FileNameLabel = new QLabel(this);
    label3 = new QLabel(tr("Database Fields"));
//    label4 = new QLabel("Bibliography Fields");
    label5 = new QLabel(this);
    QFont Labels;
    Labels.setBold(true);
    NameLabel->setFont(Labels);
    FileNameLabel->setFont(Labels);
    PathLabel->setFont(Labels);
    label5->setFont(Labels);
    QFrame * line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    hlayout = new QGridLayout;
    table = new QTableWidget(this);

    layout = new QVBoxLayout;
    layout->addWidget(FileNameLabel);
    layout->addWidget(NameLabel);
    layout->addWidget(PathLabel);
    layout->addWidget(label5);
    layout->addWidget(line);
    hlayout->addWidget(label3,0,0);
//    hlayout->addWidget(label4,0,1);
    layout->addLayout(hlayout);
    table->setColumnCount(3);
    QStringList horzHeaders;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
//    table->setStyleSheet("alternate-background-color: #e8e8e8");
    horzHeaders << "Id"<<"Name"<<"Type";
    table->setHorizontalHeaderLabels(horzHeaders);
    NameLabel->setWordWrap(true);
    PathLabel->setWordWrap(true);
    label3->setWordWrap(true);
    setLayout(layout);
}

void FinalPage::initializePage()
{
    BaseFolder::Metadata.clear();
//    BaseFolder::BibData.clear();
    BaseFolder::MetadataNames.clear();
//    BaseFolder::BibDataNames.clear();

    label5->setText(tr("Database type : ")+BaseFolder::DatabaseType);
    NameLabel->setText(tr("Description : ")+ field("DataBaseName").toString());
    FileNameLabel->setText(tr("File name : ")+ field("DataΒaseFileName").toString());
        PathLabel->setText(tr("Path : ")+field("DataBasePath").toString()
                       +QDir::separator()+field("DataBaseName").toString()
                       +QDir::separator()+field("DataΒaseFileName").toString()+".db");

    QStringList Names;
    table->setRowCount(0);
    for (int i=0;i<BaseFolder::fields.count();i++ ) {
        table->insertRow(i);
        table->setItem(i,0 , new QTableWidgetItem(BaseFolder::fields.at(i)));
        table->setItem(i,1 , new QTableWidgetItem(field(BaseFolder::DatabaseType+BaseFolder::fields.at(i)).toString()));
        table->setItem(i,2 , new QTableWidgetItem(field(BaseFolder::FieldTypesValues.at(i)).toString()));
    }
    QList<int> list;
    for(int i:DataPage::addedIdList){
        if(!DataPage::removedIdList.contains(i))
            list.append(i);
    }
    for (int i=0;i<list.count();i++ ) {
        table->insertRow(BaseFolder::fields.count()+i);
        table->setItem(BaseFolder::fields.count()+i,0 , new QTableWidgetItem(field("optField_"+QString::number(list.at(i))).toString()));
        table->setItem(BaseFolder::fields.count()+i,1 , new QTableWidgetItem(field("optValue_"+QString::number(list.at(i))).toString()));
        table->setItem(BaseFolder::fields.count()+i,2 , new QTableWidgetItem(field("optCombo_"+QString::number(list.at(i))).toString()));
    }

    DataTex::StretchColumnsToWidth(table);

    for (int i=0;i<table->rowCount() ;i++ ) {
        BaseFolder::Metadata.append(table->item(i,0)->text());
        BaseFolder::MetadataNames.append(table->item(i,1)->text());
    }

//    if(/*wizard()->hasVisitedPage(BaseFolder::Bibliography) && */BaseFolder::DatabaseType == "Files"){
//        table2 = new QTableWidget(this);
//        table2->setColumnCount(3);
//        table2->setSelectionBehavior(QAbstractItemView::SelectRows);
//        table2->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        table2->setAlternatingRowColors(true);
//        table2->setStyleSheet("alternate-background-color: #e8e8e8");
//        table2->setHorizontalHeaderLabels({"Id","Name","Type"});
        hlayout->addWidget(table,1,0);
//        hlayout->addWidget(table2,1,1);
//        table2->setRowCount(0);
//        for (int i=0;i<BaseFolder::bibfields.count();i++ ) {
//            table2->insertRow(i);
//            table2->setItem(i,0 , new QTableWidgetItem(BaseFolder::bibfields.at(i)));
//            table2->setItem(i,1 , new QTableWidgetItem(field("Bib_"+BaseFolder::bibfields.at(i)).toString()));
//            table2->setItem(i,2 , new QTableWidgetItem(field(BaseFolder::BibTypesValues.at(i)).toString()));
//        }
//        QList<int> biblist;
//        foreach(int i,BibliographyPage::addedBibIdList){
//            if(!BibliographyPage::removedBibIdList.contains(i))
//                biblist.append(i);
//        }
//        for (int i=0;i<biblist.count();i++ ) {
//            table2->insertRow(BaseFolder::bibfields.count()+i);
//            table2->setItem(BaseFolder::bibfields.count()+i,0 , new QTableWidgetItem(field("optBibField_"+QString::number(biblist.at(i))).toString()));
//            table2->setItem(BaseFolder::bibfields.count()+i,1 , new QTableWidgetItem(field("optBibValue_"+QString::number(biblist.at(i))).toString()));
//            table2->setItem(BaseFolder::bibfields.count()+i,2 , new QTableWidgetItem(field("optBibCombo_"+QString::number(biblist.at(i))).toString()));
//        }

//        for (int c = 0; c < table2->horizontalHeader()->count()-1; ++c)
//        {
//            table2->horizontalHeader()->setSectionResizeMode(
//                c, QHeaderView::Stretch);
//        }
//        for (int i=0;i<table2->rowCount() ;i++ ) {
//            BaseFolder::BibData.append(table2->item(i,0)->text());
//            BaseFolder::BibDataNames.append(table2->item(i,1)->text());
//        }
//    }
//    else{
//        hlayout->addWidget(table,1,0,1,2);
//        label4->setHidden(true);
//    }
}

FinalPage::~FinalPage()
{
    delete table;
}

BaseFolder::~BaseFolder()
{
//    delete ui;
}

DataPage::~DataPage()
{

}

//BibliographyPage::~BibliographyPage()
//{

//}

void BaseFolder::accept()
{
    QString path = field("DataBasePath").toString();
    QString folderName = field("DataBaseName").toString();
    QString baseFileName = field("DataΒaseFileName").toString();
    QDir basedir(path+QDir::separator()+folderName+QDir::separator());
    if(!basedir.exists()){basedir.mkpath(".");}
    QString FullPath = path+QDir::separator()+folderName+QDir::separator()+baseFileName+".db";
    QSqlDatabase newdatabaseFile;
    newdatabaseFile = QSqlDatabase::addDatabase("QSQLITE","newbase");
    newdatabaseFile.setDatabaseName(FullPath);
    newdatabaseFile.open();

    if(BaseFolder::DatabaseType == "Files"){
        SqlFunctions::ExecuteSqlScriptFile(newdatabaseFile,":/databases/FilesDatabase.sql");
        QSqlQuery FiletypesQuery(newdatabaseFile);
        QStringList Filetypes = {"Def","Theor","ThPrf","Prop","PrPrf","Lem","LemPr","Cor","CorPr","Ax","Fig","Tab",
                                 "SectEx","SolSE","SectSub",
                                 "SolSS","Method","Example","CombEx","SolCE","CombSub","SolCS"};
        QStringList FiletypesNames = {tr("Definition"),tr("Theorem"),tr("Theorem proof")
                                      ,tr("Proposition"),tr("Proposition proof"),tr("Lemma"),tr("Lemma proof")
                                      ,tr("Corollary"),tr("Corollary proof"),tr("Axiom"),tr("Figure"),tr("Table")
                                      ,tr("Section exercise"),tr("Exercise solution"),
                                      tr("Exam Subject"),tr("Subject solution"),
                                      tr("Method"),tr("Example"),tr("MultiSection exercise"),
                                      tr("MultiSection exercise solution"),
                                      tr("MultiSection subject"),tr("MultiSection subject solution")};
        QStringList FolderNames = {tr("Definitions"),tr("Theorems"),tr("Theorem proofs")
                                   ,tr("Propositions"),tr("Proposition proofs"),tr("Lemmas"),tr("Lemma proofs"),
                                   tr("Corollaries"),tr("Corollary proofs"),tr("Axioms"),tr("Figures"),tr("Tables")
                                   ,tr("Exercises"),tr("Exercise solutions"),tr("Exam Subjects"),tr("Subjects solutions"),
                                   tr("Methods"),tr("Examples"),tr("MultiSec exercises"),tr("MultiSec exercise solutions"),tr("MultiSection subjects"),
                                   tr("MultiSection subj. solutions")};
        QList<int> Solvable = {0,1,-1,1,-1,1,-1,1,-1,0,0,0,1,-1,1,-1,0,0,1,-1,1,-1};
        QStringList BelongsTo = {QString(),QString(),"Theor",QString(),"Prop",QString(),"Lem",QString(),"Cor",QString(),
                                 QString(),QString(),QString(),"SectEx",QString(),"SectSub",QString(),QString(),QString(),
                                 "CombEx",QString(),"CombSub"};
        for (int i=0;i<Filetypes.count();i++ ) {
            QString Query = "INSERT INTO FileTypes (Id, Name, FolderName,Solvable,BelongsTo) VALUES "
                            "(\""+Filetypes.at(i)+"\", \""+FiletypesNames.at(i)+"\", \""+FolderNames.at(i)+"\",\""+QString::number(Solvable.at(i))+"\",\""+BelongsTo.at(i)+"\")";
            FiletypesQuery.exec(Query);
        }
        FiletypesQuery.exec("INSERT INTO Exercise_Types (Id, Name) VALUES (\"-\",\"-\")");
    }
    else{
        SqlFunctions::ExecuteSqlScriptFile(newdatabaseFile,":/databases/NotesDatabase.sql");
    }

    QSqlQuery AddExtraMetadata(newdatabaseFile);
    if(DataPage::newlabelList.count()>0){
        QString Table;
        if(BaseFolder::DatabaseType == "Files"){
            Table = "Database_Files";
        }
        else{
            Table = "Documents";
        }
        for (int i=0;i<DataPage::newlabelList.count();i++ ) {
            QString query = "ALTER TABLE \""+Table+"\" ADD \""+DataPage::newlabelList.at(i)->text()+"\" "+DataPage::newcomboList.at(i)->currentText();
            AddExtraMetadata.exec(query);
        }
    }
    if(BaseFolder::DatabaseType == "Files"){
//        if(BibliographyPage::newBiblabelList.count()>0){
//            for (int i=0;i<BibliographyPage::newBiblabelList.count();i++ ) {
//                QString query = "ALTER TABLE Bibliography ADD \""+BibliographyPage::newBiblabelList.at(i)->text()+"\" "+BibliographyPage::newcomboList.at(i)->currentText();
//                AddExtraMetadata.exec(query);
//            }
//        }
        AddExtraMetadata.exec("ALTER TABLE Bibliography ADD UseBibliography TEXT");
    }
        QSqlQuery BackUp1(newdatabaseFile);
        QString BackUpMetadata = "INSERT INTO BackUp (Table_Id,Id,Name) VALUES ";
        QStringList BackUpMeta_Query;
        for (int i=0;i<Metadata.count();i++ ) {
            BackUpMeta_Query.append("(\"Metadata\",\""+Metadata.at(i)+"\",\""+MetadataNames.at(i)+"\")");
        }
        BackUpMetadata +=BackUpMeta_Query.join(",");
        BackUp1.exec(BackUpMetadata);
//    if(BaseFolder::DatabaseType == "Files"){
//        QString BackUpBib = "INSERT INTO BackUp (Table_Id,Id,Name) VALUES ";
//        QStringList BackUpBib_Query;
//        for (int i=0;i<BibData.count();i++ ) {
//            BackUpBib_Query.append("(\"Bibliography\",\""+BibData.at(i)+"\",\""
//                    +BibDataNames.at(i)+"\")");
//        }
//        BackUpBib +=BackUpBib_Query.join(",");
//        BackUp1.exec(BackUpBib);
//    }
    newdatabaseFile.close();
    QSqlQuery AddNewDatabase(DataTex::DataTeX_Settings);
    QString Table;
    QString SettingsTable;
    QString MetaTable;
    if(BaseFolder::DatabaseType == "Files"){
        Table = "DataBases";
        SettingsTable = "Metadata";
        MetaTable = "Metadata_per_Database";
    }
    else if(BaseFolder::DatabaseType == "Documents"){
        Table = "DataBases";
        SettingsTable = "DocMetadata";
        MetaTable = "DocMetadata_per_Database";
    }
    const QByteArray pass = BaseFolder::PassWord.toUtf8();
    QString password = QCryptographicHash::hash(pass,QCryptographicHash::Sha256);
    AddNewDatabase.exec(
    QString("INSERT INTO \""+Table+"\" (FileName, Name, Path,Prefix,UserName,PassWord) "
                                           "VALUES (\"%1\", \"%2\", \"%3\",\"%4\",\"%5\",\"%6\");")
            .arg(baseFileName,folderName,FullPath,BaseFolder::Prefix,BaseFolder::UserName,password));

    QString MetadataQuery_1 = "INSERT OR IGNORE INTO \""+SettingsTable+"\" (Id,Name,Basic) VALUES ";
    QStringList MetadataEntries_1;
    if(DataPage::newlabelList.count()>0){
        for (int i=0;i<DataPage::newlabelList.count();i++ ) {
            MetadataEntries_1.append("(\""+DataPage::newlabelList.at(i)->text()+"\",\""
                    +DataPage::newFieldLineList.at(i)->text()+"\",\"0\")");
        }
    }
    MetadataQuery_1 +=MetadataEntries_1.join(",");
    QSqlQuery Metadata_1(DataTex::DataTeX_Settings);
    Metadata_1.exec(MetadataQuery_1);

//    if(BaseFolder::DatabaseType == "Files"){
//        QString BibQuery_1 = "INSERT OR IGNORE INTO Bibliography (Id,Name,Basic) VALUES ";
//        QStringList BibEntries_1;

//        if(BibliographyPage::newBiblabelList.count()>0){
//            for (int i=0;i<BibliographyPage::newBiblabelList.count();i++ ) {
//                BibEntries_1.append("(\""+BibliographyPage::newBiblabelList.at(i)->text()+"\",\""
//                        +BibliographyPage::newBibLineList.at(i)->text()+"\",\"0\")");
//            }
//        }
//        BibQuery_1 +=BibEntries_1.join(",");
//        QSqlQuery Bibliography_1(DataTex::DataTeX_Settings);
//        Bibliography_1.exec(BibQuery_1);

//        QString BibQuery_2 = "INSERT INTO Bibliographic_Fields_per_Database (Database,Bibliographic_Field) VALUES ";
//        QStringList BibEntries_2;
//        for (int i=0;i<bibfields.count();i++ ) {
//            BibEntries_2.append("(\""+baseFileName+"\",\""+bibfields.at(i)+"\")");
//        }
//        for (int i=0;i<BibliographyPage::newBiblabelList.count();i++ ) {
//            BibEntries_2.append("(\""+baseFileName+"\",\""+BibliographyPage::newBiblabelList.at(i)->text()+"\")");
//        }
//        BibQuery_2 +=BibEntries_2.join(",");
//        QSqlQuery Bibliography_2(DataTex::DataTeX_Settings);
//        Bibliography_2.exec(BibQuery_2);
//    }

    QString MetadataQuery_2 =
            "INSERT INTO \""+MetaTable+"\" (Database_FileName,Metadata_Id) VALUES ";
    QStringList MetadataEntries_2;
    for (int i=0;i<fields.count();i++ ) {
        MetadataEntries_2.append("(\""+baseFileName+"\",\""+fields.at(i)+"\")");
    }
    for (int i=0;i<DataPage::newlabelList.count();i++ ) {
        MetadataEntries_2.append("(\""+baseFileName+"\",\""+DataPage::newlabelList.at(i)->text()+"\")");
    }
    MetadataQuery_2 +=MetadataEntries_2.join(",");
    QSqlQuery Metadata_2(DataTex::DataTeX_Settings);
    Metadata_2.exec(MetadataQuery_2);
    emit newbase(path,folderName,baseFileName,BaseFolder::DatabaseType);
    QDialog::accept();
}
