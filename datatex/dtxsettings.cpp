#include "dtxsettings.h"
#include <QSettings>

DTXSettings::DTXSettings()
{
    QSettings settings;
    settings.beginGroup("Application_Settings");
    saveLocation = settings.value("SaveLocation").toString();
}
