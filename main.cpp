#include "renderview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RenderView w;
    w.show();

    return a.exec();
}
