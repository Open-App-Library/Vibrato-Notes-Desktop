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
    m_shouldImportTutorialNotes = true;

  // Create any tables that are non-existent.
  runScript(":sql/create.sql");

  if ( m_shouldImportTutorialNotes )
    importTutorialNotes();
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

VariantList SQLManager::column(QString query, int column)
{
  QSqlQuery q = basicQuery(query);
  if (!q.isActive())
    return VariantList();

  VariantList list;

  while ( q.next() )
    list.append( q.value(column) );

  return list;
}

Map SQLManager::row(QSqlQuery query, QStringList tableLabels) {
  MapVector arr = rows(query, tableLabels);
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

MapVector SQLManager::rows(QSqlQuery query, QStringList tableColumns)
{
  if (!query.isActive())
    return MapVector(); // Return empty list

  MapVector table;

  while ( query.next() ) {
    QMap<QString, QVariant> row;
    for (int i=0; i<tableColumns.length(); i++) {
      row[tableColumns[i]] = query.value(i);
    }
    table.append(row);
  }

  return table;
}

MapVector SQLManager::rows(QString query, QStringList tableColumns)
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

QStringList SQLManager::notebookColumns() const
{
  return m_notebookColumns;
}

QStringList SQLManager::tagColumns() const {
  return m_tagColumns;
}

QVector<Note*> SQLManager::notes() {
  QVector<Note*> notes;

  QString queryString = QString("SELECT %1 FROM notes").arg(noteColumns().join(", "));
  MapVector rawNotes = rows(queryString, noteColumns());

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

  }
  return notes;
}

QVector<Notebook*> SQLManager::notebooks() {
  return m_getNotebooks();
}

QVector<Notebook*> SQLManager::m_getNotebooks(Notebook *parent) {
  QVector<Notebook*> notebooks;
  QSqlQuery q;

  int parentID=-1;
  if ( parent != nullptr) parentID = parent->id();



  QString queryString =
    QString("select %1 from notebooks where parent = :id").arg( notebookColumns().join(", ") );
  q.prepare(queryString);
  q.bindValue(":id", parentID);
  q.exec();
  MapVector notebookResults = rows(q, notebookColumns());

  for (Map n : notebookResults) {
    Notebook *notebook = new Notebook(n["sync_id"].toInt(),
                                      n["id"].toInt(),
                                      n["title"].toString(),
                                      parent);
    notebook->setChildren(m_getNotebooks(notebook));
    notebooks.append(notebook);
  }

  return notebooks;
}

QVector<Tag*> SQLManager::tags() {
  QVector<Tag*> tags;
  QString queryString =
    QString("select %1 from tags").arg( tagColumns().join(", ") );
  MapVector tagResults = rows(queryString, tagColumns());

  for ( Map tagMap : tagResults) {
    Tag *t = new Tag(tagMap["sync_id"].toInt(),
                     tagMap["id"].toInt(),
                     tagMap["title"].toString());
    tags.append(t);
  }

  return tags;
}

bool SQLManager::addNote(Note *note, bool getNewID)
{
  QStringList noteCols = noteColumns();
  QStringList columnPlaceholders;

  noteCols.removeOne("sync_id");
  if (getNewID) noteCols.removeOne("id");

  qDebug() << noteCols;

  for (QString col : noteCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notes (%1) "
                                "VALUES (%2)").arg(noteCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  if (!getNewID) q.bindValue(":id", note->id());
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
  VariantList curTagObjects = column(noteTagsQuery, 1);
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

  if (!logSqlError(query.lastError()) )
    return false;

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

  return logSqlError(tags.lastError());
}

bool SQLManager::deleteNote(Note* note) {
  QSqlQuery q;
  q.prepare("DELETE FROM notes WHERE id = :id");
  q.bindValue(":id", note->id());
  q.exec();
  return logSqlError(q.lastError());
}

bool SQLManager::addNotebook(Notebook* notebook, bool getNewID) {
  QStringList notebookCols = notebookColumns();
  QStringList columnPlaceholders;

  notebookCols.removeOne("sync_id");
  if (getNewID) notebookCols.removeOne("id");

  for (QString col : notebookCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notebooks (%1) "
                                "VALUES (%2)").arg(notebookCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  int parentID = -1;
  if ( notebook->parent() != nullptr )
    parentID = notebook->parent()->id();

  if (!getNewID) q.bindValue(":id" , notebook->id());
  q.bindValue(":title"            , notebook->title());
  q.bindValue(":parent"           , parentID);

  q.exec();

  // Set the ID
  int newID = q.lastInsertId().toInt();
  qDebug() << getNewID << newID;
  if ( getNewID )
    notebook->setId( newID );
  if ( newID != notebook->id() )
    qWarning() << "Notebook" << notebook->title() << "has an id of" << notebook->id() << "however it was just inserted to the database as id" << newID;

  // Print error if there is one. Return true if no error.
  return logSqlError(q.lastError());
}

bool SQLManager::updateNotebookToDB(Notebook* notebook) {
  QSqlTableModel model;
  model.setTable("notebooks");
  model.setFilter( QString("id = %1").arg(notebook->id()) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Notebook SQLite3 Warning!] Found" << model.rowCount() << "of" << notebook->title();

  QSqlRecord notebookInDB = model.record(0);

  int parentID = -1;
  if ( notebook->parent() != nullptr)
    parentID = notebook->parent()->id();

  notebookInDB.setValue("title", notebook->title());
  notebookInDB.setValue("parent", parentID);

  model.setRecord(0, notebookInDB);

  return logSqlError(model.lastError());
}

bool SQLManager::updateNotebookFromDB(Notebook* notebook) {
  QSqlQuery query;
  QString queryString
    = QString("select %1 from notebooks where id = :id").arg(notebookColumns().join(", "));
  query.prepare( queryString );
  query.bindValue(":id", notebook->id());
  query.exec();
  Map notebookRow = row(query, notebookColumns());

  if ( !logSqlError(query.lastError()) )
    return false;

  notebook->setSyncId ( notebookRow["sync_id"].toInt() );
  notebook->setId     ( notebookRow["id"].toInt() );
  notebook->setTitle  ( notebookRow["title"].toString() );
  notebook->requestParentChangeToID( notebookRow["parent"].toInt());

  return true;
}

bool SQLManager::deleteNotebook(Notebook* notebook, bool delete_children) {
  // Delete notebook
  QSqlQuery q;
  q.prepare("DELETE FROM notebooks WHERE id = :id");
  q.bindValue(":id", notebook->id());
  q.exec();

  // Change notes under this notebook to use default notebook
  logSqlError(q.lastError());
  q.prepare("UPDATE notes SET notebook = -1 WHERE notebook = :id");
  q.bindValue(":id", notebook->id());
  q.exec();

  // Delete children
  if ( delete_children )
    for (Notebook *child : m_getNotebooks(notebook) )
      deleteNotebook(child, false);

  return logSqlError(q.lastError());
}

bool SQLManager::addTag(Tag *tag, bool getNewID) {
  QStringList tagCols = tagColumns();
  QStringList columnPlaceholders;

  tagCols.removeOne("sync_id");
  if (getNewID) tagCols.removeOne("id");

  for (QString col : tagCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO tags (%1) "
                                "VALUES (%2)").arg(tagCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  if (!getNewID) q.bindValue(":id" , tag->id());
  q.bindValue(":title"            , tag->title());

  q.exec();

  // Set the ID
  int newID = q.lastInsertId().toInt();
  if ( getNewID )
    tag->setId( newID );
  if ( newID != tag->id() )
    qWarning() << "Tag" << tag->title() << "has an id of" << tag->id() << "however it was just inserted to the database as id" << newID;

  // Print error if there is one. Return true if no error.
  return logSqlError(q.lastError());
}

bool SQLManager::updateTagToDB(Tag* tag) {
  QSqlTableModel model;
  model.setTable("tags");
  model.setFilter( QString("id = %1").arg(tag->id()) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Tag SQLite3 Warning!] Found" << model.rowCount() << "of" << tag->title();

  QSqlRecord tagInDB = model.record(0);
  tagInDB.setValue("title", tag->title());
  model.setRecord(0, tagInDB);

  return logSqlError(model.lastError());
}

bool SQLManager::updateTagFromDB(Tag* tag) {
  QSqlQuery query;
  QString queryString
    = QString("select %1 from tags where id = :id").arg(tagColumns().join(", "));
  query.prepare( queryString );
  query.bindValue(":id", tag->id());
  query.exec();
  Map tagRow = row(query, tagColumns());

  if ( !logSqlError(query.lastError()) )
    return false;

  tag->setSyncId ( tagRow["sync_id"].toInt() );
  tag->setId     ( tagRow["id"].toInt() );
  tag->setTitle  ( tagRow["title"].toString() );

  return true;
}

bool SQLManager::deleteTag(Tag* tag) {
  QSqlQuery q;
  q.prepare("DELETE FROM tags WHERE id = :id");
  q.bindValue(":id", tag->id());
  q.exec();
  logSqlError(q.lastError());
  q.prepare("DELETE FROM notes_tags WHERE tag = :id");
  q.bindValue(":id", tag->id());
  q.exec();
  return logSqlError(q.lastError());
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

void SQLManager::importTutorialNotes() {
  QString welcomeText = HelperIO::fileToQString(":/tutorial/1-welcome.md");
  QDateTime now = QDateTime::currentDateTime();
  Note welcome(-1, -1,
               "Welcome to Vibrato Notes!",
               welcomeText,
               now, now,
               true,
               NOTEBOOK_DEFAULT_NOTEBOOK_ID,
               {});
  addNote(&welcome);
}
