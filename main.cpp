#include "applicgaragewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApplicGarageWindow w;
    w.show();
    return a.exec();
}
