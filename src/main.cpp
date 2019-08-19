#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFontDatabase>

#include "mainwindow.h"
#include "meta/info/appinfo.h"
#include "meta/info/appconfig.h"
#include <helper-io.hpp>

int main(int argc, char *argv[])
{
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication a(argc, argv);
    QIcon::setThemeName("vibrato-default");

  // Set Cross Platform Icon settings
  QIcon::setFallbackSearchPaths( QIcon::fallbackSearchPaths() << ":/icons/vibrato-default/icons" );

#if defined(TARGET_OS_MAC)
#else
    QFontDatabase::addApplicationFont(":/fonts/Cantarell-Regular.ttf");
    QFont defaultFont("Cantarell", 10);
    QApplication::setFont(defaultFont);
#endif

  MainWindow w;
  w.setWindowTitle("Vibrato Notes");
  w.show();

  qDebug().nospace() << "Welcome to Vibrato Notes! v." << qPrintable( VERSION );
  qDebug()           << "User Config Location:" << config()->fileName();
  qDebug()           << "Data Location:" <<   HelperIO::dataDir().absolutePath();

  return a.exec();
}
