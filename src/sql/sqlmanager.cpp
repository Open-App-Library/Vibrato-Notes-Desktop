#include "sqlmanager.h"
#include <QDebug>
#include <QDir>
#include <helper-io.hpp>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVariant>
#include "../meta/note.h"

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

    QString uuid            = noteObject["uuid"].toString();
    QString mimetype        = noteObject["mimetype"].toString();
    QString encoding        = noteObject["encoding"].toString();
    QString title           = noteObject["title"].toString();
    QString text            = noteObject["text"].toString();
    QDateTime date_created  = noteObject["date_created"].toDateTime();
    QDateTime date_modified = noteObject["date_modified"].toDateTime();
    QString notebook        = noteObject["notebook"].toString();
    bool favorited          = noteObject["favorited"].toBool();
    bool encrypted          = noteObject["encrypted"].toBool();
    bool trashed            = noteObject["trashed"].toBool();

    // Parse a tags array
    QVector<QUuid> tags;
    QString tagVariantsQuery = QString("select tag from notes_tags where note = \"%1\"").arg(uuid);
    for ( QVariant tagVariant : column(tagVariantsQuery) )
      tags.append( tagVariant.toString() );

    Note *note = new Note(this, uuid, mimetype, encoding);
    note->setTitle(title);
    note->setDateCreated(date_created);
    note->setDateModified(date_modified);
    note->setNotebook(notebook);
    note->setTags(tags);
    note->setFavorited(favorited);
    note->setEncrypted(encrypted);
    note->setTrashed(trashed);

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

  QString queryString;
  if (parent == nullptr)
    queryString = "SELECT %1 FROM notebooks WHERE parent IS NULL ORDER BY row ASC";
  else
    queryString = "SELECT %1 FROM notebooks WHERE parent = :uuid ORDER BY row ASC";

  queryString = queryString.arg( notebookColumns().join(", ") );

  q.prepare(queryString);
  if (parent != nullptr)
    q.bindValue(":uuid", parent->uuid().toString(QUuid::WithoutBraces));
  q.exec();
  MapVector notebookResults = rows(q, notebookColumns());

  for (Map n : notebookResults) {      
    Notebook *notebook = new Notebook(this,
                                      n["uuid"].toString(),
                                      n["title"].toString(),
                                      n["date_created"].toDateTime(),
                                      n["date_modified"].toDateTime(),
                                      parent,
                                      n["row"].toInt(),
                                      n["encrypted"].toBool());
    notebook->setChildren(m_getNotebooks(notebook));
    notebooks.append(notebook);
  }

  return notebooks;
}

QVector<Tag*> SQLManager::tags() {
  QVector<Tag*> tags;
  QString queryString =
    QString("select %1 from tags ORDER BY row ASC").arg( tagColumns().join(", ") );
  MapVector tagResults = rows(queryString, tagColumns());

  for ( Map tagMap : tagResults) {
    Tag *t = new Tag(tagMap["uuid"].toString(),
                     tagMap["title"].toString(),
                     tagMap["date_created"].toDateTime(),
                     tagMap["date_modified"].toDateTime(),
                     tagMap["row"].toInt(),
                     tagMap["encrypted"].toBool());
    tags.append(t);
  }

  return tags;
}

Note *SQLManager::fetchNote(QUuid uuid)
{
    Note *note = new Note(this);

    QSqlQuery query;
    QString queryString
      = QString("select %1 from notes where uuid = :uuid").arg(noteColumns().join(", "));
    query.prepare( queryString );
    query.bindValue(":uuid", uuid.toString(QUuid::WithoutBraces));
    query.exec();

    if (!logSqlError(query.lastError()) )
      return nullptr;

    Map noteRow = row(query, noteColumns());

    note->setUUID          ( noteRow["uuid"].toString() );
    note->setMIMEType      ( noteRow["mimetype"].toString() );
    note->setEncoding      ( noteRow["encoding"].toString() );
    note->setTitle         ( noteRow["title"].toString() );
    note->setExcerpt       ( noteRow["text"].toString().left(NOTE_EXCERPT_LENGTH) );
    note->setDateCreated   ( noteRow["date_created"].toDateTime() );
    note->setDateModified  ( noteRow["date_modified"].toDateTime() );
    note->setNotebook      ( noteRow["notebook"].toString() );
    note->setFavorited     ( noteRow["favorited"].toBool() );
    note->setEncrypted     ( noteRow["encrypted"].toBool() );
    note->setTrashed       ( noteRow["trashed"].toBool() );

    QSqlTableModel tags;
    tags.setTable("notes_tags");
    tags.setFilter( QString("note = \"%1\"").arg(uuid.toString(QUuid::WithoutBraces)) );
    tags.select();

    if (!logSqlError(tags.lastError()) )
      return nullptr;

    QVector<QUuid> tagList;

    for (int i=0; i<tags.rowCount(); i++)
      tagList.append( tags.record(i).value("tag").toString() );

    note->setTags(tagList);

    return note;
}

bool SQLManager::addNote(Note *note)
{
  QStringList noteCols = noteColumns();
  QStringList columnPlaceholders;

  for (QString col : noteCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notes (%1) "
                                "VALUES (%2)").arg(noteCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  q.bindValue(":uuid"          , note->uuid().toString(QUuid::WithoutBraces));
  q.bindValue(":mimetype"      , note->mimeType());
  q.bindValue(":encoding"      , note->encoding());
  q.bindValue(":title"         , note->title());
  q.bindValue(":date_created"  , note->dateCreated());
  q.bindValue(":date_modified" , note->dateModified());
  q.bindValue(":notebook"      , note->notebook());
  q.bindValue(":favorited"     , note->favorited());
  q.bindValue(":encrypted"     , note->encrypted());
  q.bindValue(":trashed"       , note->trashed());

  q.exec();

  // Set the tags
  QSqlQuery tagQ;
  for ( QUuid tagUUID : note->tags() ) {
    tagQ.prepare("insert into notes_tags (note, tag) values "
              "(:noteUuid, :tagUuid)");
    tagQ.bindValue(":noteUuid", note->uuid().toString(QUuid::WithoutBraces));
    tagQ.bindValue(":tagUuid", tagUUID);
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
  model.setFilter( QString("uuid = \"%1\"").arg(note->uuid().toString(QUuid::WithoutBraces)) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Note SQLite3 Warning!] Found" << model.rowCount() << "of" << note->title();

  QSqlRecord noteInDB = model.record(0);

  noteInDB.setValue("title", note->title());
  noteInDB.setValue("text", note->data());
  noteInDB.setValue("date_created", note->dateCreated());
  noteInDB.setValue("date_modified", note->dateModified());
  noteInDB.setValue("favorited", note->favorited());
  noteInDB.setValue("notebook", note->notebook());
  noteInDB.setValue("trashed", note->trashed());

  model.setRecord(0, noteInDB);

  ///
  // Update the note's tags
  ///
  QString noteTagsQuery
    = QString("select note, tag from notes_tags where note = \"%1\"").arg(note->uuid().toString(QUuid::WithoutBraces));
  VariantList curTagObjects = column(noteTagsQuery, 1);
  QVector<QUuid> curTagSyncIDs;
  for (QVariant obj : curTagObjects)
    curTagSyncIDs.append(obj.toString());

  // First, loop through note's tags and add to db if needded
  for (QUuid uuid : note->tags()) {
    if (!curTagSyncIDs.contains(uuid))
      addTagToNote(note->uuid(), uuid);
  }

  // Then, loop through db tags and remove ones that are not needed
  for (QUuid uuid : curTagSyncIDs) {
    if (!note->tags().contains(uuid))
      removeTagFromNote(note->uuid(), uuid);
  }
  return logSqlError(model.lastError());
}

bool SQLManager::updateNoteFromDB(Note* note) {

  Note *fetchedNote = fetchNote(note->uuid());

  if (fetchedNote == nullptr)
    return false;

  note->setUUID          ( fetchedNote->uuid() );
  note->setMIMEType      ( fetchedNote->mimeType() );
  note->setEncoding      ( fetchedNote->encoding() );
  note->setTitle         ( fetchedNote->title() );
  note->setExcerpt       ( fetchedNote->excerpt() );
  note->setDateCreated   ( fetchedNote->dateCreated() );
  note->setDateModified  ( fetchedNote->dateModified() );
  note->setNotebook      ( fetchedNote->notebook() );
  note->setFavorited     ( fetchedNote->favorited() );
  note->setEncrypted     ( fetchedNote->encrypted() );
  note->setTrashed       ( fetchedNote->trashed() );
  note->setTags          ( fetchedNote->tags() );

  delete fetchedNote;

  return true;
}

bool SQLManager::deleteNote(Note* note) {
  QSqlQuery q;
  q.prepare("DELETE FROM notes WHERE uuid = :uuid");
  q.bindValue(":uuid", note->uuid().toString(QUuid::WithoutBraces));
  q.exec();
  return logSqlError(q.lastError());
}

Notebook *SQLManager::fetchNotebook(QUuid uuid)
{
    Notebook *notebook(this);
    QSqlQuery query;
    QString queryString
      = QString("select %1 from notebooks where uuid = :uuid").arg(notebookColumns().join(", "));
    query.prepare( queryString );
    query.bindValue(":uuid", uuid.toString(QUuid::WithoutBraces));
    query.exec();
    Map notebookRow = row(query, notebookColumns());

    if ( !logSqlError(query.lastError()) )
      return nullptr;

}

bool SQLManager::addNotebook(Notebook* notebook) {
  QStringList notebookCols = notebookColumns();
  QStringList columnPlaceholders;

  for (QString col : notebookCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO notebooks (%1) "
                                "VALUES (%2)").arg(notebookCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  QString parentuuid;
  if ( notebook->parent() != nullptr )
    parentuuid = notebook->parent()->uuid().toString(QUuid::WithoutBraces);

  q.bindValue(":uuid"        , notebook->uuid().toString(QUuid::WithoutBraces));
  q.bindValue(":title"            , notebook->title());
  q.bindValue(":date_modified"    , notebook->dateModified());
  q.bindValue(":parent"           , parentuuid);
  q.bindValue(":row"              , notebook->row());
  q.bindValue(":encrypted"        , notebook->encrypted());

  q.exec();

  // Print error if there is one. Return true if no error.
  return logSqlError(q.lastError());
}

bool SQLManager::updateNotebookToDB(Notebook* notebook) {
  QSqlTableModel model;
  model.setTable("notebooks");
  model.setFilter( QString("uuid = \"%1\"").arg(notebook->uuid().toString(QUuid::WithoutBraces)) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Notebook SQLite3 Warning!] Found" << model.rowCount() << "of" << notebook->title();

  QSqlRecord notebookInDB = model.record(0);

  QString parentuuid;
  if ( notebook->parent() != nullptr)
    parentuuid = notebook->parent()->uuid().toString(QUuid::WithoutBraces);

  notebookInDB.setValue("title", notebook->title());
  notebookInDB.setValue("date_modified", notebook->dateModified());
  notebookInDB.setValue("parent", parentuuid);
  notebookInDB.setValue("row", notebook->row());
  notebookInDB.setValue("encrypted", notebook->encrypted());

  model.setRecord(0, notebookInDB);

  return logSqlError(model.lastError());
}

bool SQLManager::updateNotebookFromDB(Notebook* notebook) {

  notebook->setTitle( notebookRow["title"].toString() );
  notebook->setDateModified( notebookRow["date_modified"].toDateTime() );
  notebook->requestParentWithUUID( notebookRow["parent"].toString());
  notebook->setRow( notebookRow["row"].toInt() );
  notebook->setEncrypted( notebookRow["encrypted"].toBool() );

  return true;
}

bool SQLManager::deleteNotebook(Notebook* notebook, bool delete_children) {
  // Delete notebook
  QSqlQuery q;
  q.prepare("DELETE FROM notebooks WHERE uuid = :uuid");
  q.bindValue(":uuid", notebook->uuid().toString(QUuid::WithoutBraces));
  q.exec();

  // Change notes under this notebook to use default notebook
  logSqlError(q.lastError());
  q.prepare("UPDATE notes SET notebook = NULL WHERE notebook = :uuid");
  q.bindValue(":uuid", notebook->uuid().toString(QUuid::WithoutBraces));
  q.exec();

  // Delete children
  if ( delete_children )
    for (Notebook *child : m_getNotebooks(notebook) )
      deleteNotebook(child, false);

  return logSqlError(q.lastError());
}

Tag *SQLManager::fetchTag(QUuid uuid)
{

}

bool SQLManager::addTag(Tag *tag) {
  QStringList tagCols = tagColumns();
  QStringList columnPlaceholders;

  for (QString col : tagCols)
    columnPlaceholders.append( QString(":%1").arg(col) );

  QString queryString = QString("INSERT INTO tags (%1) "
                                "VALUES (%2)").arg(tagCols.join(", "),
                                                   columnPlaceholders.join(", "));

  QSqlQuery q;
  q.prepare(queryString);

  q.bindValue(":uuid", tag->uuid().toString(QUuid::WithoutBraces));
  q.bindValue(":title", tag->title());
  q.bindValue(":date_modified", tag->title());
  q.bindValue(":row", tag->row());
  q.bindValue(":encrypted", tag->encrypted());

  q.exec();

  // Print error if there is one. Return true if no error.
  return logSqlError(q.lastError());
}

bool SQLManager::updateTagToDB(Tag* tag) {
  QSqlTableModel model;
  model.setTable("tags");
  model.setFilter( QString("uuid = \"%1\"").arg(tag->uuid().toString(QUuid::WithoutBraces)) );
  model.select();

  if ( model.rowCount() > 1 )
    qWarning() << "[Duplicate Tag SQLite3 Warning!] Found" << model.rowCount() << "of" << tag->title();

  QSqlRecord tagInDB = model.record(0);
  tagInDB.setValue("title", tag->title());
  tagInDB.setValue("date_modified", tag->dateModified());
  tagInDB.setValue("row", tag->row());
  tagInDB.setValue("encrypted", tag->encrypted());
  model.setRecord(0, tagInDB);

  return logSqlError(model.lastError());
}

bool SQLManager::updateTagFromDB(Tag* tag) {
  QSqlQuery query;
  QString queryString
    = QString("select %1 from tags where uuid = :uuid").arg(tagColumns().join(", "));
  query.prepare( queryString );
  query.bindValue(":uuid", tag->uuid().toString(QUuid::WithoutBraces));
  query.exec();
  Map tagRow = row(query, tagColumns());

  if ( !logSqlError(query.lastError()) )
    return false;

  tag->setTitle( tagRow["title"].toString() );
  tag->setDateModified( tagRow["date_modified"].toDateTime() );
  tag->setRow( tagRow["row"].toInt() );
  tag->setRow( tagRow["encrypted"].toBool() );

  return true;
}

bool SQLManager::deleteTag(Tag* tag) {
  QSqlQuery q;
  q.prepare("DELETE FROM tags WHERE uuid = :uuid");
  q.bindValue(":uuid", tag->uuid().toString(QUuid::WithoutBraces));
  q.exec();
  logSqlError(q.lastError());
  q.prepare("DELETE FROM notes_tags WHERE tag = :uuid");
  q.bindValue(":uuid", tag->uuid().toString(QUuid::WithoutBraces));
  q.exec();
  return logSqlError(q.lastError());
}

bool SQLManager::tagExists(QUuid noteuuid, QUuid taguuid) {
  QSqlTableModel model;
  model.setTable("notes_tags");
  model.setFilter( QString("note = \"%1\" and tag = \"%2\"").arg(noteuuid.toString(QUuid::WithoutBraces), taguuid.toString(QUuid::WithoutBraces)) );
  model.select();
  return model.rowCount() > 0;
}

bool SQLManager::addTagToNote(QUuid noteuuid, QUuid taguuid, bool skip_duplicate_check) {
  if ( !skip_duplicate_check && tagExists(noteuuid, taguuid) )
    return true;
  QSqlQuery q;
  q.prepare("INSERT INTO notes_tags (note, tag) VALUES "
            "(:noteuuid, :taguuid)");
  q.bindValue(":noteuuid", noteuuid.toString(QUuid::WithoutBraces));
  q.bindValue(":taguuid", taguuid.toString(QUuid::WithoutBraces));
  q.exec();
  return logSqlError(q.lastError());
}

bool SQLManager::removeTagFromNote(QUuid noteuuid, QUuid taguuid) {
  QSqlQuery q;
  q.prepare("DELETE FROM notes_tags WHERE "
            "note = :noteuuid and tag = :taguuid");
  q.bindValue(":noteuuid", noteuuid);
  q.bindValue(":taguuid", taguuid);
  q.exec();
  return logSqlError(q.lastError());
}

void SQLManager::importTutorialNotes() {
  QByteArray welcomeText = HelperIO::fileToQString(":/tutorial/1-welcome.md").toUtf8();
  Note welcome;
  addNote(&welcome);
  welcome.setTitle("Welcome to Vibrato Notes!");
  welcome.setData(welcomeText);
}
