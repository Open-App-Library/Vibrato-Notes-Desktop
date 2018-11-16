#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFontDatabase>

#include "mainwindow.h"
#include "meta/info/appinfo.h"
#include "meta/info/appconfig.h"

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	// Set Cross Platform Icon settings
	QIcon::setFallbackSearchPaths( QIcon::fallbackSearchPaths() << ":/vibrato/icons" );

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
