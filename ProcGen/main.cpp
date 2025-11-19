#include "ProcGen.h"
#include <QtWidgets/QApplication>

#include "QSettingsSingleton.h"
QSettings* QSettingsSingleton::instance = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ProcGen window;
    window.showMaximized();
    return app.exec();
}
