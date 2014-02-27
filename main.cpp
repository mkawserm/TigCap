#include "tigcap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TigCap w;
    w.show();

    return a.exec();
}
