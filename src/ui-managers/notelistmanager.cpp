#include "notelistmanager.h"
#include <QDebug>
#include <QAbstractItemView>
#include "../meta/db/notedatabase.h"

NoteListManager::NoteListManager(CustomListView *view, QWidget *noteListAddons, EscribaManager *escribaManager, Database *db) :
	m_view(view),
	m_noteListAddons(noteListAddons),
	m_escribaManager(escribaManager),
	m_db(db)
{
	m_noteListAddonsUi = new Ui::NoteListAddonsWidget;
	m_noteListAddonsUi->setupUi( m_noteListAddons );

	// Hide description area for now. In the
	// future I would love to add notebook
	// description support.
	m_noteListAddonsUi->description->hide();

	m_filter = new NoteFilter( m_db );
	m_model = new NoteListModel(view);
	m_proxyModel = new NoteListProxyModel(view);
	m_proxyModel->setSourceModel(m_model);
	m_proxyModel->sort(0, Qt::DescendingOrder);
	view->setModel(m_proxyModel);

	connect(m_view, &CustomListView::selectedItemChanged,
					this, &NoteListManager::noteListItemChanged);
}

NoteListManager::~NoteListManager()
{
	delete m_filter;
	delete m_noteListAddonsUi;
}

NoteListItem *NoteListManager::add_note(Note *note)
{
	NoteListItem *i = m_model->prependItem(note);
	return i;
}

void NoteListManager::remove_note(int index)
{
	m_model->removeRows(index, 1);
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

void NoteListManager::openIndexInEditor(int index)
{
	if ( m_model->noteItems().length() > 0 && index < m_model->noteItems().length() ) {
		m_view->setCurrentIndex( m_proxyModel->index(index,0) );
		m_escribaManager->setNote( m_proxyModel->item(index)->note() );
    }
}

void NoteListManager::clearFilter()
{
    m_proxyModel->clearFilter();
}

void NoteListManager::addNotebookToFilter(Notebook *notebook)
{
    m_proxyModel->addNotebookToFilter(notebook);
}

void NoteListManager::addTagToFilter(Tag *tag)
{
    m_proxyModel->addTagToFilter(tag);
}

void NoteListManager::noteListItemChanged(const QModelIndex &current_proxy, const QModelIndex &previous_proxy)
{
	QModelIndex current = m_proxyModel->mapToSource(current_proxy);
	QModelIndex previous = m_proxyModel->mapToSource(previous_proxy);
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
