#include "datatex.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QPixmap pixmap(":/images/datatex-splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    DataTex w;
    w.show();
    splash->finish(&w);
    delete splash;
    return a.exec();
}
