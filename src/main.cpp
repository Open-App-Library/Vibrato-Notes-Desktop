#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <string>

#include "mainwindow.h"
#include "meta/info/appinfo.h"
#include "meta/info/appconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("Vibrato Notes");
    w.show();

    qDebug().nospace() << "Welcome to Vibrato Notes! v." << qPrintable( VERSION );
    qDebug()           << "User Config Location:" << config()->fileName();

    QStringList fallback_icon_search_paths;
    fallback_icon_search_paths << ":icons/breeze-qownnotes/16x16";
    QIcon::setFallbackSearchPaths( fallback_icon_search_paths );

    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    return a.exec();
}
