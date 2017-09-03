#include "sudo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sudo *w=new sudo();
    w->show();

    return a.exec();
}
