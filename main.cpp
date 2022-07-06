#include "datatex.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTranslator>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QPixmap pixmap(":/images/datatex-splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    DataTex w;

//    QFile file("/home/spyros/Qt Projects/StudentDatabase/breezelight.qss");
//    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
//        {
//            a.setStyleSheet(file.readAll());
//            file.close();
//        }
    QTranslator translator;
    translator.load(":/languages/DataTex_el_GR.qm");
//    QCoreApplication::removeTranslator(&translator);
    QCoreApplication::installTranslator(&translator);
    //    qApp->removeTranslator(&translator);

    w.show();
    splash->finish(&w);
    delete splash;
    return a.exec();
}
