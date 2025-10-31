#include "ProcGen.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ProcGen window;
    window.show();
    return app.exec();
}
