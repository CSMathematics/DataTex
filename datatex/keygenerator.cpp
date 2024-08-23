#include "keygenerator.h"
#include "ui_keygenerator.h"
#include <math.h>
#include <QDebug>

KeyGenerator::KeyGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyGenerator)
{
    ui->setupUi(this);
    KeyLength = ui->KeyLength->value();
    ui->IncludeCapitals->setProperty("charlist","ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    ui->IncludeSmall->setProperty("charlist","abcdefghijklmnopqrstuvwxyz");
    ui->IncludeNumbers->setProperty("charlist","0123456789");
    ui->IncludeSymbols->setProperty("charlist","!@#$%^&*()_+=-[]{}`~<>?");

    connect(ui->IncludeCapitals,&QAbstractButton::clicked,this,[=](){UpdateKey();});
    connect(ui->IncludeSmall,&QAbstractButton::clicked,this,[=](){UpdateKey();});
    connect(ui->IncludeNumbers,&QAbstractButton::clicked,this,[=](){UpdateKey();});
    connect(ui->IncludeSymbols,&QAbstractButton::clicked,this,[=](){UpdateKey();});
    connect(ui->KeyLength,QOverload<int>::of(&QSpinBox::valueChanged),this,[&](int value){
        KeyLength = value;
        UpdateKey();
    });
    connect(ui->Update,&QPushButton::clicked,this,[&](){
        UpdateKey();
    });
    connect(ui->Copy,&QPushButton::clicked,this,[&](){
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(ui->Code->toPlainText());
    });
    connect(ui->keyLengthSlider,&QSlider::valueChanged,this,[&](int value){
        ui->KeyLength->setValue(value);
        UpdateKey();
    });
}

KeyGenerator::~KeyGenerator()
{
    delete ui;
}

void KeyGenerator::UpdateKey()
{
    CharList.clear();
    foreach (QAbstractButton * button, ui->buttonGroup->buttons()) {
        if(button->isChecked()){
            CharList.append(button->property("charlist").toString());
        }
    }
    QString key;
    if(CharList.length()>0){
        ui->Code->setEnabled(true);
        ui->Update->setEnabled(true);
        ui->Copy->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        for(int i=0; i<KeyLength; ++i)
        {
            quint32 length = QRandomGenerator::global()->generate();
            int index = length % CharList.length();
            QChar nextChar = CharList.at(index);
            key.append(nextChar);
        }
        double entropy = key.length() * log(CharList.length())/log(2);
        QString strength;
        QColor color;
        if(entropy<=30){strength = tr("Bad");color = QColor(225,0,0);}
        else if (entropy>30 && entropy <= 60){strength = tr("Weak");color = QColor(255,150,0);}
        else if (entropy>60 && entropy <= 90){strength = tr("Reasonable");color = QColor(230,230,0);}
        else if (entropy>90 && entropy <= 120){strength = tr("Strong");color = QColor(55,220,0);}
        else if(entropy>120){strength = tr("Excellent");color = QColor(25,125,0);}
        ui->Code->setText(key);
        ui->SafeLevel->setText(strength+" : "+QString::number(entropy,'f', 2)+"bits");
        ui->SafeLevel->setStyleSheet(QString("color : %1").arg(color.name()));
    }
    else{
        ui->Code->setEnabled(false);
        ui->Update->setEnabled(false);
        ui->Copy->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void KeyGenerator::on_buttonBox_accepted()
{
    emit password(ui->Code->toPlainText());
    accept();
}

