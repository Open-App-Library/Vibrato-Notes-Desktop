#include "ui_notelistitem.h"
#include "notelistmanager.h"

NoteListManager::NoteListManager(QListWidget *listWidget) :
    m_listWidget(listWidget)
{

}

NoteListManager::~NoteListManager()
{

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

void NoteListManager::loadNotesFromNoteDatabase(NoteDatabase *noteDatabase)
{
    clear();
    for (int i = 0; i < noteDatabase->size(); i++) {
        add_note(noteDatabase->list()[i]);
    }
}
