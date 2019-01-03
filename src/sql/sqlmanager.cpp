#include "sqlmanager.h"
#include <QDebug>
#include <QDir>
#include <helper-io.hpp>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVariant>

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

ObjectList SQLManager::column(QString query, int column)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return ObjectList();

  ObjectList list;

  while ( q.next() )
    list.append( q.value(column) );

  return list;
}

Map SQLManager::row(QSqlQuery query, QStringList tableLabels) {
  ArrayOfMaps arr = rows(query, tableLabels);
  qDebug() << arr;
  Map m;
  if ( arr.length() > 0)
    return arr[0];
  else
    return m;
}

Map SQLManager::row(QString queryString, QStringList tableLabels) {
  QSqlQuery q;
  q.exec(queryString);
  return row(q, tableLabels);
}

ArrayOfMaps SQLManager::rows(QSqlQuery query, QStringList tableColumns)
{
  if (!query.isActive())
    return ArrayOfMaps(); // Return empty list

  ArrayOfMaps table;

  while ( query.next() ) {
    QMap<QString, QVariant> row;
    for (int i=0; i<tableColumns.length(); i++) {
      row[tableColumns[i]] = query.value(i);
    }
    table.append(row);
  }

  return table;
}

ArrayOfMaps SQLManager::rows(QString query, QStringList tableColumns)
{
  QSqlQuery q = basicQuery(query);
  return rows(q, tableColumns);
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
    if (!success)
      logSqlError(query->lastError());
  }

  return query->isActive();
}

bool SQLManager::logSqlError(QSqlError error, bool fatal) {
  if ( !error.isValid() )
    return true;
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

  return false;
}

QStringList SQLManager::noteColumns() const
{
  return m_noteColumns;
}

QVector<Note*> SQLManager::Notes() {
  QVector<Note*> notes;

  QString queryString = QString("SELECT %1 FROM notes").arg(noteColumns().join(", "));
  ArrayOfMaps rawNotes = rows(queryString, noteColumns());

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

QVector<Notebook*> SQLManager::Notebooks() {

}

QVector<Tag*> SQLManager::tags() {

}

bool SQLManager::addNote(Note *note, bool getNewID)
{
  QStringList noteCols = noteColumns();
  QStringList columnPlaceholders;

  if (!getNewID) noteCols.removeOne("id");

  for (QString col : noteCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notes (%1) "
                                "VALUES (%2)").arg(noteCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
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

  q.exec();

  // Set the ID
  int newID = q.lastInsertId().toInt();
  if ( getNewID )
    note->setId( newID );
  if ( newID != note->id() )
    qWarning() << "Note" << note->title() << "has an id of" << note->id() << "however it was just inserted to the database as id" << newID;

  // Set the tags
  QSqlQuery tagQ;
  for ( int tagID : note->tags() ) {
    tagQ.prepare("insert into notes_tags (note, tag) values "
              "(:noteID, :tagID)");
    tagQ.bindValue(":noteID", note->id());
    tagQ.bindValue(":tagID", tagID);
    tagQ.exec();
    logSqlError(tagQ.lastError());
  }

  // Print error if there is one. Return true if no error.
  return logSqlError(q.lastError());
}

bool SQLManager::updateNoteToDB(Note* note) {
  ///
  // Update the note
  ///
  QSqlTableModel model;
  model.setTable("notes");
  model.setFilter( QString("id = %1").arg(note->id()) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Note SQLite3 Warning!] Found" << model.rowCount() << "of" << note->title();

  QSqlRecord noteInDB = model.record(0);

  noteInDB.setValue("sync_id", note->syncId());
  noteInDB.setValue("title", note->title());
  noteInDB.setValue("text", note->text());
  noteInDB.setValue("date_created", note->date_created());
  noteInDB.setValue("date_modified", note->date_modified());
  noteInDB.setValue("favorited", note->favorited());
  noteInDB.setValue("notebook", note->notebook());
  noteInDB.setValue("trashed", note->trashed());

  model.setRecord(0, noteInDB);

  ///
  // Update the note's tags
  ///
  QString noteTagsQuery
    = QString("select note, tag from notes_tags where note =%1").arg(note->id());
  ObjectList curTagObjects = column(noteTagsQuery, 1);
  QVector<int> curTagIDs;
  for (QVariant obj : curTagObjects)
    curTagIDs.append(obj.toInt());

  // First, loop through note's tags and add to db if needded
  for (int id : note->tags()) {
    if (!curTagIDs.contains(id))
      addTagToNote(note->id(), id);
  }

  // Then, loop through db tags and remove ones that are not needed
  for (int id : curTagIDs) {
    if (!note->tags().contains(id))
      removeTagFromNote(note->id(), id);
  }
  return logSqlError(model.lastError());
}

bool SQLManager::updateNoteFromDB(Note* note) {
  QSqlQuery query;
  QString queryString
    = QString("select %1 from notes where id = :id").arg(noteColumns().join(", "));
  query.prepare( queryString );
  query.bindValue(":id", note->id());
  query.exec();
  Map noteRow = row(query, noteColumns());

  note->setSyncId        ( noteRow["sync_id"].toInt() );
  note->setId            ( noteRow["id"].toInt() );
  note->setTitle         ( noteRow["title"].toString() );
  note->setText          ( noteRow["text"].toString() );
  note->setDate_created  ( noteRow["date_created"].toDateTime() );
  note->setDate_modified ( noteRow["date_modified"].toDateTime() );
  note->setFavorited     ( noteRow["favorited"].toBool() );
  note->setNotebook      ( noteRow["notebook"].toInt() );
  note->setTrashed       ( noteRow["trashed"].toBool() );

  QSqlTableModel tags;
  tags.setTable("notes_tags");
  tags.setFilter( QString("note = %1").arg(note->id()) );
  tags.select();

  QVector<int> tagList;

  for (int i=0; i<tags.rowCount(); i++)
    tagList.append( tags.record(i).value("tag").toInt() );
  note->setTags(tagList);

  return
    logSqlError(query.lastError()) &&
    logSqlError(tags.lastError());
}

bool SQLManager::tagExists(int noteID, int tagID) {
  QSqlTableModel model;
  model.setTable("notes_tags");
  model.setFilter( QString("note = %1 and tag = %2").arg(noteID, tagID) );
  model.select();
  return model.rowCount() > 0;
}

bool SQLManager::addTagToNote(int noteID, int tagID, bool skip_duplicate_check) {
  if ( !skip_duplicate_check && tagExists(noteID, tagID) )
    return true;
  QSqlQuery q;
  q.prepare("INSERT INTO notes_tags (note, tag) VALUES "
            "(:noteID, :tagID)");
  q.bindValue(":noteID", noteID);
  q.bindValue(":tagID", tagID);
  q.exec();
  return logSqlError(q.lastError());
}

bool SQLManager::removeTagFromNote(int noteID, int tagID) {
  QSqlQuery q;
  q.prepare("DELETE FROM notes_tags WHERE "
            "note = :noteID and tag = :tagID");
  q.bindValue(":noteID", noteID);
  q.bindValue(":tagID", tagID);
  q.exec();
  return logSqlError(q.lastError());
}
