#include "database.h"

Database::Database(NoteDatabase *noteDatabase, NotebookDatabase *notebookDatabase, TagDatabase *tagDatabase) :
    m_note_database(noteDatabase),
    m_notebook_database(notebookDatabase),
    m_tag_database(tagDatabase)
{
  connect(m_tag_database, &TagDatabase::removed,
          m_note_database, &NoteDatabase::removeTagFromNotes);
}

NoteDatabase *Database::noteDatabase() const
{
    return m_note_database;
}

NotebookDatabase *Database::notebookDatabase() const
{
    return m_notebook_database;
}

TagDatabase *Database::tagDatabase() const
{
    return m_tag_database;
}

void Database::addTagToNote(Note *note, QString tagString)
{
  Tag *tag = m_tag_database->addTag(tagString);
  if ( tag == nullptr ) return;
  if ( note->tags().indexOf(tag->syncHash()) == -1 )
    note->setTags( note->tags() << tag->syncHash());
}
