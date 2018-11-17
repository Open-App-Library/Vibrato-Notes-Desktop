#include "notelistmanager.h"
#include <QDebug>
#include <QAbstractItemView>
#include "../meta/db/notedatabase.h"

NoteListManager::NoteListManager(CustomListView *view, Database *db) :
	m_view(view),
	m_db(db)
{
	m_filter = new NoteFilter( m_db );
	m_model = new NoteListModel(view);
	view->setModel(m_model);

    connect(m_view, &CustomListView::selectedItemChanged,
            this, &NoteListManager::noteListItemChanged);

	// connect(m_listWidget, &QListWidget::currentItemChanged,
	// 				this, &NoteListManager::noteListItemChanged);
}

NoteListManager::~NoteListManager()
{
	delete m_filter;
}

NoteListItem *NoteListManager::add_note(Note *note)
{
	NoteListItem *i = m_model->prependItem(note);
	return i;
}

void NoteListManager::remove_note(int index)
{
	// TODO: Implement remove_note
}

void NoteListManager::clear()
{
    m_model->clear();
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

void NoteListManager::noteListItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
   if (previous.isValid()) {
       NoteListItem *prevItem = m_model->noteItems()[ previous.row() ];
       prevItem->setSelectedStyle(false);
   }

   if (current.isValid()) {
       NoteListItem *curItem = m_model->noteItems()[ current.row() ];
       curItem->setSelectedStyle(true);
       emit selectedNote( curItem->note() );
   }
}
