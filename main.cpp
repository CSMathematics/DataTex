#include "datatex.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTranslator>
#include <QSettings>
#include <qtsingleapplication.h> //第三方库，确保某一时刻只有一个该软件在执行。
#include <QLocalSocket>
#include <QLocalServer>



int main(int argc, char *argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);//使用ini格式的文件来配置软件参数
    QCoreApplication::setOrganizationName("CSMathematics");//确定软件组织
    QCoreApplication::setApplicationName("DataTex");//确定软件名称

    QtSingleApplication a(argc, argv);//生成单例程序

    QPixmap pixmap(":/images/datatex-splash.png");//创建位图
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);//将刚才的位图作为启动动画，并将其放置在最上层。
     qDebug()<<"23";
    if(!a.isRunning()){ // 未启动程序的情况下，执行以后操作
          qDebug()<<"25";
        // splash.show(); //显示启动图
           qDebug()<<"27";
        QTimer::singleShot(4000, &splash, &QWidget::close);// 4s后关闭启动动画
            qDebug()<<"29";
           Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splash.showMessage(QObject::tr("loading modules..."), topRight, Qt::black);//启动页面右上角黑色字体显示启动信息
        splash.showMessage(QObject::tr("Establishing Connections..."), topRight, Qt::black);
    }
    qDebug()<<"31";
    QSettings settings;//创建/更新配置文件，将其读入内存
    QString theme = settings.value("Application_Settings/Theme").toString();
    QString language = settings.value("Application_Settings/Language").toString();
    qDebug()<<"34";
    QFile styleFile(":/qdarkstyle/"+theme+"/"+theme+".qss");
    qDebug()<<"36";
    if(styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(styleFile.readAll());//读取qss配置并关闭文件
        styleFile.close();
    }
    qDebug()<<"41";
    QTranslator translator;//根据配置文件的语言设定，载入相关语言的翻译
    translator.load(":/languages/DataTex_"+language+".qm");
    QCoreApplication::installTranslator(&translator);
qDebug()<<"45";
    DataTex *w = new DataTex(nullptr); // 初始化软件主体
qDebug()<<"47";
    QObject::connect(&a, SIGNAL(messageReceived(QString)),
                     w, SLOT(onOtherInstanceMessage(QString))); //将软件主体与单例程序主体连接。
    if(a.isRunning()){
        a.sendMessage(QCoreApplication::arguments().last());
        return 0;
    }
    w->initialize(QCoreApplication::arguments().last());
    w->show();
//    splash->finish(w);
//    delete splash;
    return a.exec();
}
