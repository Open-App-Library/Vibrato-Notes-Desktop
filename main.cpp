#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <string>

#include "mainwindow.h"
#include "appinfo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QSettings config(COMPANY, CONFIG);
    QSettings meta_config(COMPANY, CONFIG_META);

    if (meta_config.contains("last_opened_window_size")) {
        w.restoreGeometry(meta_config.value("last_opened_window_size").toByteArray());
    }

    qDebug().nospace() << "Welcome to Vibrato Notes! v." << qPrintable( VERSION );
    qDebug()           << "User Config Location:" << config.fileName();

    return a.exec();
}
