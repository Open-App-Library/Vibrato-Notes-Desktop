#include "sqlmanager.h"
#include <QDir>
#include <helper-io.hpp>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

SQLManager::SQLManager(QObject *parent) : QObject(parent)
{
  // Determine an appropraite pathname for the sqlite3 database
  QDir data_dir = HelperIO::dataDir();
  m_location = data_dir.filePath("vibrato-db.sqlite3");

  // Open the database connection
  m_sqldb = QSqlDatabase::addDatabase("QSQLITE");
  m_sqldb.setDatabaseName(m_location);
  bool ok = m_sqldb.open();
  if (!ok) qFatal("Fatal error establishing a connection with Vibrato's sqlite3 database. :(");

  // Check if 'notes' table exists. If not, create it and import tutorial note.
  if ( !m_sqldb.tables().contains("notes") )
    qDebug("Import tutorial notes! Implement this");

  // Create any tables that are non-existent.
  //  QSqlQuery query =

  // Sync to cloud

  // load notes into database
}

void SQLManager::close() {
  m_sqldb.close();
}

QString SQLManager::location() const
{
  return m_location;
}

QSqlQuery SQLManager::basicQuery(QString query)
{
  QSqlQuery q;

  bool success = q.exec(query);
  if (!success)
    logSqlError( q.lastQuery() );

  return q;
}

bool SQLManager::realBasicQuery(QString query)
{
  return basicQuery(query).isActive();
}

QVector<QVariant> SQLManager::column(QString query)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return QVector<QVariant>();

  QVector<QVariant> list;

  while ( q.next() )
    list.append( q.value(0) );

  return list;
}

QVector<QVector<QVariant>> SQLManager::rows(QString query, QStringList tableLabels)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return QVector<QVector<QVariant>>(); // Return empty list

  QVector<QVector<QVariant>> table;

  while ( q.next() ) {
    QVector<QVariant> row;
    for (int i=0; i<tableLabels.length(); i++) {
      row.append( q.value(i) );
    }
    table.append( row );
  }

  return table;
}

bool SQLManager::runScript(QString fileName)
{
  QFile file(fileName);
  if ( !file.open(QIODevice::ReadOnly) ) {
    qWarning() << "Unable to load SQL file" << fileName;
    return false;
  }
  QSqlQuery query;
  return runScript(&file, &query);
}

bool SQLManager::runScript(QFile *file, QSqlQuery *query)
{
  QString contents = file->readAll();

  // Remove sql comments
  contents = contents.replace(QRegExp("--.+$"), "");

  QStringList lines = contents.split(";");

  for ( QString line : lines ) {
    line = line.trimmed();
    if ( line.isEmpty() ) continue;

    bool success = query->exec(line);
    if (!success) qDebug() << "ERROR!" << line << "(" << file->fileName() << ")";
  }

  return query->isActive();
}

void SQLManager::logSqlError(QSqlError error, bool fatal) {
  QString status = "SQL Error";
  if (fatal) status = "FATAL SQL Error";

  QString msg = QString("[%1] %2 %3").arg(status,
                                          error.driverText(),
                                          error.databaseText());

  // Print the message. Must convert it from QString to char*.
  if (fatal)
    qFatal("%s", msg.toLatin1().constData());
  else
    qWarning("%s", msg.toLatin1().constData());
}
