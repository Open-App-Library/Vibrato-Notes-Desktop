#include "notelistmanager.h"

NoteListManager::NoteListManager(QListWidget *listWidget, NoteDatabase *database) :
    m_listWidget(listWidget),
    m_database(database)
{
    m_filter = new NoteFilter( database );
}

NoteListManager::~NoteListManager()
{
    delete m_filter;
}

NoteListItem *NoteListManager::add_note(Note *note)
{
    NoteListItem *mynote = new NoteListItem(note, m_listWidget);
    m_noteList.append(mynote);
    return mynote;
}

void NoteListManager::remove_note(int index)
{
    // TODO: Implement remove_note
}

void NoteListManager::clear()
{
    for (int i = 0; i < m_noteList.length(); i++) {
        m_noteList[i]->trash();
    }
    m_noteList.clear();
}

void NoteListManager::loadNotesFromNoteDatabase()
{
    loadNotesFromNoteDatabase( m_database );
}

void NoteListManager::loadNotesFromNoteDatabase(NoteDatabase *noteDatabase)
{
    clear();
    for (int i = 0; i < noteDatabase->size(); i++) {
        add_note(noteDatabase->list()[i]);
    }
}

void NoteListManager::loadNotesFromNoteFilter(noteFilterList noteList)
{
    clear();
    for (int i = 0; i < noteList.size(); i++) {
        add_note( noteList[i] );
    }
}

NoteFilter *NoteListManager::filter()
{
    return m_filter;
}
