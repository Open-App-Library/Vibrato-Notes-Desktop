#include "database.h"



Database::Database(NoteDatabase *noteDatabase, NotebookDatabase *notebookDatabase, TagDatabase *tagDatabase) :
    m_note_database(noteDatabase),
    m_notebook_database(notebookDatabase),
    m_tag_database(tagDatabase)
{

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
    tagString = tagString.trimmed();
    if ( tagString.isEmpty() )
        return;
    Tag *tag = m_tag_database->findTagWithNameOrCreate( tagString );
    // If tag is not already added, add it.
    if ( note->tags().indexOf(tag->id()) == -1 )
        note->setTags( note->tags() << tag->id());
}
