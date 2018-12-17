#include "database.h"

Database::Database(NoteDatabase *noteDatabase, NotebookDatabase *notebookDatabase, TagDatabase *tagDatabase) :
    m_note_database(noteDatabase),
    m_notebook_database(notebookDatabase),
    m_tag_database(tagDatabase)
{
  connect(m_tag_database, &TagDatabase::tagRemoved,
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
  if ( note->tags().indexOf(tag->id()) == -1 )
    note->setTags( note->tags() << tag->id());
}
