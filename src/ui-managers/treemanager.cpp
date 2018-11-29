#include "treemanager.h"
#include "../../iconutils.h"

TreeManager::TreeManager(QTreeView *treeView, NoteListManager *noteListManager, QObject *parent) :
	QObject(parent),
	m_tree_view(treeView),
	m_note_list_manager(noteListManager)
{
	m_tree_model = new TreeModel;
	m_tree_view->setModel(m_tree_model);

	QFile styleFile( ":/style/DarkSolarized.qss" );
	styleFile.open( QFile::ReadOnly );
	QString style(styleFile.readAll());
	m_tree_view->setStyleSheet( style );

	m_all_notes = new BasicTreeItem( tr("All Notes") );
	m_favorites = new BasicTreeItem( tr("Favorites") );
	m_notebooks = new BasicTreeItem( tr("Notebooks") );
	m_tags      = new BasicTreeItem( tr("Tags") );
	m_trash     = new BasicTreeItem( tr("Trash (6)") );
	m_search    = new BasicTreeItem( tr("Search results for \"Test\"") );

	m_all_notes->setIcon( IconUtils::requestDarkIcon("document-new") );
	m_favorites->setIcon( IconUtils::requestDarkIcon("draw-star") );
	m_notebooks->setIcon( IconUtils::requestDarkIcon("folder") );
	m_tags->setIcon     ( IconUtils::requestDarkIcon("tag") );
	m_trash->setIcon    ( IconUtils::requestDarkIcon("trash-empty") );
	m_search->setIcon   ( IconUtils::requestDarkIcon("edit-find") );


	m_tree_model->root()->appendChild(m_all_notes);
	m_tree_model->root()->appendChild(m_favorites);
	m_tree_model->root()->appendChild(m_notebooks);
	m_tree_model->root()->appendChild(m_tags);
	m_tree_model->root()->appendChild(m_trash);
	m_tree_model->root()->appendChild(m_search);

	// Set the current selection to the first item in the treeview..which is "All Notes"
	// Explanation: https://stackoverflow.com/questions/15817429/how-to-get-list-of-visible-qmodelindex-in-qabstractitemview
	QModelIndex all_notes_index = m_tree_model->index(0, 0);
	m_tree_view->setCurrentIndex( all_notes_index );
	treeItemChanged(all_notes_index, QModelIndex());

	// Signal Connections
	connect(m_tree_view->selectionModel(), &QItemSelectionModel::currentChanged,
					this, &TreeManager::treeItemChanged);
}

TreeManager::~TreeManager()
{
	delete m_tree_model;
}

void TreeManager::update()
{
	emit m_tree_model->layoutChanged();
}

BasicTreeItem *TreeManager::curItem()
{
	return m_curItem;
}

QVector<BasicTreeItem *> TreeManager::notebooks() const
{
	return m_notebooks->recurseChildren();
}

BasicTreeItem *TreeManager::addNotebook(Notebook *notebook)
{
	return addNotebook(notebook, m_notebooks);
}

BasicTreeItem *TreeManager::addNotebook(Notebook *notebook, BasicTreeItem *parent)
{
	BasicTreeItem *item = parent->appendChild( new BasicTreeItem(notebook) );
	update();
	return item;
}

void TreeManager::removeNotebook(BasicTreeItem *item)
{
	clearChildren(item);
	delete item;
	update();
}

void TreeManager::removeNotebook(BasicTreeItem *item, BasicTreeItem *fosterParent)
{
	QVector<BasicTreeItem*> children = item->recurseChildren();
	for (int i = 0; i < children.size(); i++) {
		fosterParent->appendChild( children[i] );
	}
	delete item;
	update();
}

void TreeManager::clearChildren(BasicTreeItem *item)
{
	QVector<BasicTreeItem*> children = item->recurseChildren();
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}
	update();
}

void TreeManager::clearNotebooks()
{
	for (int i = m_notebooks->childCount()-1; i >= 0; i--) {
		clearChildren(m_notebooks->getChild(i));
		delete m_notebooks->getChild(i);
	}
	update();
}

void TreeManager::loadNotebookObjectAndChildren(Notebook *notebook, BasicTreeItem *parent)
{
	BasicTreeItem *item;
	if (parent == nullptr) {
		item = addNotebook(notebook);
	} else {
		item = addNotebook(notebook, parent);
	}
	for (int i = 0; i < notebook->children().size(); i++) {
		loadNotebookObjectAndChildren(notebook->children()[i], item);
	}
}

void TreeManager::loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase)
{
	clearNotebooks();
	for (int i = 0; i < notebookDatabase->size(); i++) {
		loadNotebookObjectAndChildren(notebookDatabase->list()[i]);
	}
	m_tree_view->expandAll();
}

QVector<BasicTreeItem *> TreeManager::tags() const
{
	return m_tags->children();
}

BasicTreeItem *TreeManager::addTag(Tag *tag)
{
	BasicTreeItem *item =  m_tags->appendChild( new BasicTreeItem(tag) );
	update();
	return item;
}

void TreeManager::removeTag(BasicTreeItem *item)
{
	if ( item->childCount() > 0 )
		qWarning() << "Tag \"" << item->label() << "\" has a child count greater than zero. This should never happen.";
	delete item;
	update();
}

void TreeManager::clearTags()
{
	for (int i = m_tags->childCount()-1; i >= 0; i--) {
		clearChildren(m_notebooks->getChild(i));
		delete m_notebooks->getChild(i);
	}
	update();
}

void TreeManager::loadTagsFromTagDatabase(TagDatabase *tagDatabase)
{
	clearTags();
	for (int i = 0; i < tagDatabase->size(); i++) {
		addTag(tagDatabase->list()[i]);
	}
	update();
}

void TreeManager::treeItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
	(void) previous; // Avoid 'unused parameter' compiler warning. Ignore.

	if ( current != previous ) {
		BasicTreeItem *item = static_cast<BasicTreeItem*>(current.internalPointer());
		m_curItem = item;

		if (item == m_all_notes)
			{   // Selected all notes label
                m_note_list_manager->clearFilter();
			}

		else if ( item == m_notebooks )
			{   // Selected notebooks label
                m_note_list_manager->clearFilter();
			}

		else if ( item->isNotebook() )
			{   // Selected a notebook
                Notebook *notebook = item->object().notebook;
                m_note_list_manager->clearFilter(false);
                m_note_list_manager->addNotebookToFilter(notebook);

			}

		else if ( item == m_tags)
			{   // Selected tags label

        }

		else if ( item->isTag() )
			{   // Selected a tag
				Tag *tag = item->object().tag;
                m_note_list_manager->clearFilter();
                m_note_list_manager->addTagToFilter(tag);
			}

		else
			{   // Selected something else. Clear for safety.
				qWarning("Selected something weird in the tree list :S");
            }
	} else {
		qWarning("Hmmm..Current tree selection is same as previous. This should NOT happen. [treemanager.cpp]");
	}
}
