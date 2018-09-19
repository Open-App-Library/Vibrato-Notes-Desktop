#include "notes.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Notes w;
    w.show();

    return a.exec();
}
