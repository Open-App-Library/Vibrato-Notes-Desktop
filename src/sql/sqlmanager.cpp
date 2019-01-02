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

QVector<QVariant> SQLManager::column(QString query, int column)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return QVector<QVariant>();

  QVector<QVariant> list;

  while ( q.next() )
    list.append( q.value(column) );

  return list;
}

ArrayOfMaps SQLManager::rows(QString query, QStringList tableColumns)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return ArrayOfMaps(); // Return empty list

  ArrayOfMaps table;

  while ( q.next() ) {
    QMap<QString, QVariant> row;
    for (int i=0; i<tableColumns.length(); i++) {
      row[tableColumns[i]] = q.value(i);
    }
    table.append(row);
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

  // Remove sql comments and
  // Remove unnecesary blank lines
  contents = contents.replace(QRegExp("--[^\\n]+\\n"), "\n");
  contents = contents.replace(QRegExp("\\n\\n+"), "\n");

  QStringList queryList = contents.split(";");

  for ( QString line : queryList ) {
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

QStringList SQLManager::noteColumns() const
{
  return m_noteColumns;
}

QVector<Note*> SQLManager::Notes() {
  QVector<Note*> notes;

  ArrayOfMaps rawNotes = rows("SELECT * FROM notes", noteColumns());

  for ( int i=0; i<rawNotes.length(); i++ ) {
    QMap<QString, QVariant> noteObject = rawNotes[i];

    int sync_id             = noteObject["sync_id"].toInt();
    int id                  = noteObject["id"].toInt();
    QString title           = noteObject["title"].toString();
    QString text            = noteObject["text"].toString();
    QDateTime date_created  = noteObject["date_created"].toDateTime();
    QDateTime date_modified = noteObject["date_modified"].toDateTime();
    bool favorited          = noteObject["favorited"].toBool();
    int notebook            = noteObject["notebook"].toInt();
    bool trashed            = noteObject["trashed"].toInt();

    // Parse a tags array
    QVector<int> tags;
    QString tagVariantsQuery = QString("select tag from notes_tags where note = %1").arg(id);
    for ( QVariant tagVariant : column(tagVariantsQuery) )
      tags.append( tagVariant.toInt() );

    Note *note = new Note(sync_id,
                          id,
                          title,
                          text,
                          date_created,
                          date_modified,
                          favorited,
                          notebook,
                          tags,
                          trashed);
    notes.append(note);

    qDebug() << "ADDED NOTE" << sync_id << id << title << text << date_created << date_modified << favorited << notebook << tags << trashed;
  }
  return notes;
}

QVector<Note*> SQLManager::Notebooks() {

}

QVector<Note*> SQLManager::tags() {

}
#include <iostream>
void SQLManager::addNoteToDatabase(Note *note, bool getNewID)
{
  QSqlQuery q;

  QStringList noteCols = noteColumns();
  QStringList columnPlaceholders;

  if (!getNewID) noteCols.removeAll("id");

  for (QString col : noteCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notes (%1) "
                                "VALUES (%2)").arg(noteCols.join(", "),
                                                   columnPlaceholders.join(", "));


  q.prepare(queryString);

  q.bindValue(":sync_id"       , note->syncId());
  if (getNewID) q.bindValue(":id", note->id());
  q.bindValue(":title"         , note->title());
  q.bindValue(":text"          , note->text());
  q.bindValue(":date_created"  , note->date_created());
  q.bindValue(":date_modified" , note->date_modified());
  q.bindValue(":favorited"     , note->favorited());
  q.bindValue(":notebook"      , note->notebook());
  q.bindValue(":trashed"       , note->trashed());

  qDebug() << "TEST" << q.executedQuery();

  q.exec();
  qDebug() << q.lastError();
}
