#include "mainwidget.h"
#include <QApplication>
#include <face_detection.h>
using namespace seeta;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();

    return a.exec();
}
