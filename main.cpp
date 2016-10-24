#include "reflowcontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReflowController w;
    w.show();

    return a.exec();
}
