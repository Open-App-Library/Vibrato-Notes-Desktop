#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <string>

#include "mainwindow.h"
#include "appinfo.h"
#include "appconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug().nospace() << "Welcome to Vibrato Notes! v." << qPrintable( VERSION );
    qDebug()           << "User Config Location:" << config()->fileName();

    return a.exec();
}
