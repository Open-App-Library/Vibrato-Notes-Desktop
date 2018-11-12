#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFontDatabase>

#include "mainwindow.h"
#include "meta/info/appinfo.h"
#include "meta/info/appconfig.h"

int main(int argc, char *argv[])
{
    QIcon::setThemeName("breeze-qownnotes");

    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

		QFontDatabase::addApplicationFont(":/fonts/Cantarell-Regular.ttf");
		QFont defaultFont("Cantarell", 10);
		QApplication::setFont(defaultFont);

    MainWindow w;
    w.setWindowTitle("Vibrato Notes");
    w.show();

    qDebug().nospace() << "Welcome to Vibrato Notes! v." << qPrintable( VERSION );
    qDebug()           << "User Config Location:" << config()->fileName();

    return a.exec();
}
