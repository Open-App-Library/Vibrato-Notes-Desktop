#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

#include "notedatabase.h"

#include <helper-io.hpp>

NoteDatabase::NoteDatabase(SQLManager *sqlManager) :
  m_sqlManager(sqlManager)
{
  loadSQL();
}

QList<Note *> NoteDatabase::list() const
{
  return m_list;
}

int NoteDatabase::size() const
{
  return m_list.size();
}

Note *NoteDatabase::addNote(Note *note, bool addToSQL)
{
  m_list.prepend(note);

  if (addToSQL) m_sqlManager->addNote(note);

  connect(note, &Note::changed,
          this, &NoteDatabase::slot_noteChanged);
  connect(note, &Note::favoritedChanged,
          this, &NoteDatabase::handleNoteFavoritedChanged);
  connect(note, &Note::trashedOrRestored,
          this, &NoteDatabase::noteTrashedOrRestored);
  return note;
}

Note *NoteDatabase::addDefaultNote()
{
  Note *note = new Note();
  return addNote(note);
}

void NoteDatabase::removeNote(int index)
{
  Note *note = m_list[index];
  QUuid syncHash = note->syncHash();
  m_list.removeAt(index);
  m_sqlManager->deleteNote(note);
  delete note;
  emit noteDeleted(syncHash);
}

void NoteDatabase::removeNote(Note *note)
{
  int index = m_list.indexOf( note );
  if ( index >= 0)
    removeNote( index );
  else
    qDebug() << "Tried to remove a note with an ID of -1 :" << note->title();
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

void NoteDatabase::loadSQL()
{
  QVector<Note*> notes = m_sqlManager->notes();
  for (Note *note : notes)
    addNote(note, false);
}

void NoteDatabase::removeNotesWithNotebookSyncHash(QUuid notebookSyncHash)
{
  for (Note *note : m_list)
    if (note->notebook() == notebookSyncHash)
      removeNote(note);
}

void NoteDatabase::removeNotesWithNotebookSyncHashes(QVector<QUuid> notebookSyncHashes)
{
  for (Note *note : m_list)
    if ( notebookSyncHashes.contains(note->notebook()) )
      removeNote(note);
}

void NoteDatabase::removeTagFromNotes(QUuid tagSyncHash) {
  // Loop through each note's list of tags.
  // If note contains the deleted tag,
  // Remove the tag from the note.
  for (Note *note : m_list) {
    if ( note->tags().contains(tagSyncHash) ) {
      QVector<QUuid> newTagList = note->tags();
      newTagList.removeAll(tagSyncHash);
      note->setTags( newTagList );
    }
  }
}

QVector<Note*> NoteDatabase::findNotesWithNotebookIDs(QVector<QUuid> notebookUUIDs)
{
  QVector<Note*> notes;
  for ( Note *note : m_list )
    if ( notebookUUIDs.contains(note->notebook()) )
      notes.append(note);
  return notes;
}

bool NoteDatabase::noteWithSyncHashExists(QUuid syncHash) const
{
  for (Note *note : m_list)
    if (note->syncHash() == syncHash)
      return true;
  return false;
}

void NoteDatabase::slot_noteChanged(Note* note) {
  m_sqlManager->updateNoteToDB(note);
  emit noteChanged(note);
}

void NoteDatabase::handleNoteFavoritedChanged(Note* note) {
  emit noteFavoritedChanged(note);
}
