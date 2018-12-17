#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notedatabase.h"

#include <helper-io.hpp>

NoteDatabase::NoteDatabase()
{

}

QList<Note *> NoteDatabase::list() const
{
  return m_list;
}

int NoteDatabase::size() const
{
  return m_list.size();
}

Note *NoteDatabase::addNote(Note *note)
{
  m_list.prepend(note);
  return note;
}

Note *NoteDatabase::addDefaultNote()
{
  Note *note = new Note(-1, "Untitled Note", "", QDateTime::currentDateTime(), QDateTime::currentDateTime(), false, -1, {});
  // Todo add note to cloud and get ID
  return addNote(note);
}

// This causes use of deleted function error. Probably because I started extending QObject
//Note *NoteDatabase::addNote(Note note)
//{
//  Note *newNote = new Note(note);
//  return addNote(newNote);
//}

void NoteDatabase::removeNote(int index)
{
  int id = m_list[index]->id();
  delete m_list[index];
  m_list.removeAt(index);
  emit noteRemoved(id);
}

void NoteDatabase::removeNote(Note *note)
{
  int index = m_list.indexOf( note );
  removeNote( index );
}

void NoteDatabase::removeNotes(QVector<Note*> notes)
{
  for (Note *note : notes)
    removeNote(note);
}

void NoteDatabase::clearNotes()
{
  for (int i = m_list.size()-1; i >= 0; i--) {
    removeNote(i);
  }
}

void NoteDatabase::loadJSON(QJsonDocument jsonDocument)
{
  clearNotes(); // Clear note database for safety
  QJsonArray noteArray = jsonDocument.array();
  for (int i = 0; i < noteArray.size(); i++) {
    QJsonObject val = noteArray[i].toObject();

    int id = get(val, "id").toInt();
    QString title = get(val, "title").toString();
    QString text = get(val, "text").toString();
    QDateTime date_created = QDateTime::fromString(get(val, "date_created").toString(), Qt::ISODate);
    QDateTime date_modified = QDateTime::fromString(get(val, "date_modified").toString(), Qt::ISODate);
    bool favorited = get(val, "favorited").toBool();
    int notebook = get(val, "notebook").toInt();
    // Set invalid notebooks to default notebook ID (-1)
    if (notebook <= 0)
      notebook = NOTEBOOK_DEFAULT_NOTEBOOK_ID;
    QVector<int> tags = {};

    // Setting Tags
    QJsonArray raw_tag_array = val["tags"].toArray();
    for (int i = 0; i < raw_tag_array.size(); i++) {
      int tag_id = raw_tag_array[i].toInt();
      tags.append(tag_id);
    }

    Note *note = new Note(id, title, text, date_created, date_modified, favorited, notebook, tags);

    m_list.append(note);
  }
}

void NoteDatabase::loadDummyNotes()
{
  QJsonDocument dummy_notes = HelperIO::fileToQJsonDocument(":/dummy/notes.json");
  loadJSON(dummy_notes);
}

QJsonValue NoteDatabase::get(QJsonObject obj, QString key)
{
  if ( obj.value(key).isNull() )
    return QJsonValue();
  return obj.value(key);
}

void NoteDatabase::removeNotesWithNotebookID(int notebookID) {
  for (Note *note : m_list)
    if (note->notebook() == notebookID)
      removeNote(note);
}

void NoteDatabase::removeNotesWithNotebookIDs(QVector<int> notebookIDs) {
  for (Note *note : m_list)
    if ( notebookIDs.contains(note->notebook()) )
      removeNote(note);
}

void NoteDatabase::removeTagFromNotes(int tagID) {
  // Loop through each note's list of tags.
  // If note contains the deleted tag,
  // Remove the tag from the note.
  for (Note *note : m_list) {
    if ( note->tags().contains(tagID) ) {
      QVector<int> newTagList = note->tags();
      newTagList.removeAll(tagID);
      note->setTags( newTagList );
    }
  }
}

QVector<Note*> NoteDatabase::findNotesWithNotebookIDs(QVector<int> notebookIDs)
{
  QVector<Note*> notes;
  for ( Note *note : m_list )
    if ( notebookIDs.contains(note->notebook()) )
      notes.append(note);
  return notes;
}

bool NoteDatabase::noteWithIDExists(int noteID) const
{
  for (Note *note : m_list)
    if (note->id() == noteID)
      return true;
  return false;
}
