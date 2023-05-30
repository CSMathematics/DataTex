#include "datatex.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTranslator>
#include <QSettings>
#include <qtsingleapplication.h>
#include <QLocalSocket>
#include <QLocalServer>


int main(int argc, char *argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setOrganizationName("CSMathematics");
    QCoreApplication::setApplicationName("DataTex");

    QtSingleApplication a(argc, argv);

    QPixmap pixmap(":/images/datatex-splash.png");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    if(!a.isRunning()){
        splash.show();
        QTimer::singleShot(4000, &splash, &QWidget::close);
        Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splash.showMessage(QObject::tr("loading modules..."), topRight, Qt::black);
        splash.showMessage(QObject::tr("Establishing Connections..."), topRight, Qt::black);
    }

    QSettings settings;
    QString theme = settings.value("Application_Settings/Theme").toString();
    QString language = settings.value("Application_Settings/Language").toString();

    QFile styleFile(":/qdarkstyle/"+theme+"/"+theme+".qss");
    if(styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
    QTranslator translator;
    translator.load(":/languages/DataTex_"+language+".qm");
    QCoreApplication::installTranslator(&translator);

    DataTex *w = new DataTex(nullptr);
    QObject::connect(&a, SIGNAL(messageReceived(QString)),
                     w, SLOT(onOtherInstanceMessage(QString)));
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
