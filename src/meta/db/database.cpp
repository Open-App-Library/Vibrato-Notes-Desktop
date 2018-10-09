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
