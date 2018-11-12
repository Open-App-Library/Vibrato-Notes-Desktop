#include "notelistmanager.h"
#include <QDebug>
#include "../meta/db/notedatabase.h"

NoteListManager::NoteListManager(QListWidget *listWidget, Database *db) :
	m_listWidget(listWidget),
	m_db(db)
{
	m_filter = new NoteFilter( m_db );
	connect(m_listWidget, &QListWidget::currentItemChanged,
					this, &NoteListManager::noteListItemChanged);
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
	loadNotesFromNoteDatabase( m_db->noteDatabase() );
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

void NoteListManager::noteListItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	(void) previous; // Avoid unused argument compiler warning.
	if (current) {
		NoteListItem *item = dynamic_cast<NoteListItem*>(current);
		selectNote( item->note() );
	}
}

void NoteListManager::selectNote(Note *note)
{
	emit noteSelected(note);
}
