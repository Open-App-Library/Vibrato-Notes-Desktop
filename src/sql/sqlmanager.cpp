#include "sqlmanager.h"
#include <QDir>
#include <helper-io.hpp>

SQLManager::SQLManager(QObject *parent) : QObject(parent)
{
  // Create the
  QDir data_dir = HelperIO::dataDir();
  m_location = data_dir.filePath("vibrato-db.sql");
  m_sqldb = QSqlDatabase::addDatabase("sqlite");
  m_sqldb.setDatabaseName(m_location);
  bool ok = m_sqldb.open();
  // Die if can't connect
  if (!ok) qFatal("Fatal error establishing a connection with Vibrato's sqlite3 database. :(");
}

void SQLManager::close() {
  m_sqldb.close();
}

QString SQLManager::location() const
{
  return m_location;
}
