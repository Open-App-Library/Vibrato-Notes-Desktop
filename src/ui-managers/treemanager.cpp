#include "treemanager.h"
#include "../../iconutils.h"

TreeManager::TreeManager(CustomTreeView *treeView, Database *db, Manager *manager) :
  m_tree_view(treeView),
  m_manager(manager),
  m_db(db)
{
  m_tree_model = new TreeModel;
  m_tree_view->setModel(m_tree_model);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);

  QFile styleFile( ":/style/DarkSolarized.qss" );
  styleFile.open( QFile::ReadOnly );
  QString style(styleFile.readAll());
  m_tree_view->setStyleSheet( style );

  m_all_notes = new BasicTreeItem( tr("All Notes") );
  m_favorites = new BasicTreeItem( tr("Favorites") );
  m_notebooks = new BasicTreeItem( tr("Notebooks") );
  m_tags      = new BasicTreeItem( tr("Tags") );
  m_trash     = new BasicTreeItem( tr("Trash (6)") );
  BasicTreeItem *m_search = new BasicTreeItem( tr("Search results for \"Test\"") );

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

  ////////////////////////////////////
  // START: CONTEXT MENUS
  ////////////////////////////////////

  // Treeview context requested signal
  connect(m_tree_view, &QTreeView::customContextMenuRequested,
          this, &TreeManager::treeContextMenu);

  // Notebook context menu

  m_notebookContextMenu = new QMenu();

  m_notebookNew = new QAction(tr("&New notebook"));
  m_notebookContextMenu->addAction(m_notebookNew);
  connect(m_notebookNew, &QAction::triggered,
          this, &TreeManager::contextNewNotebook);

  m_notebookContextMenu->addSeparator();

  m_notebookRename = new QAction(tr("&Rename notebook"));
  m_notebookContextMenu->addAction(m_notebookRename);
  connect(m_notebookRename, &QAction::triggered,
          this, &TreeManager::contextRenameNotebook);

  m_notebookEditHierarchy = new QAction(tr("&Edit notebook hierarchy"));
  m_notebookContextMenu->addAction(m_notebookEditHierarchy);
  connect(m_notebookEditHierarchy, &QAction::triggered,
          this, &TreeManager::contextEditNotebookHierarchy);

  m_notebookDelete = new QAction(tr("&Delete notebook"));
  m_notebookContextMenu->addAction(m_notebookDelete);
  connect(m_notebookDelete, &QAction::triggered,
          this, &TreeManager::contextDeleteNotebook);

  // Tag context menu
  m_tagContextMenu = new QMenu();

  m_tagNew = new QAction(tr("&New tag"));
  m_tagContextMenu->addAction(m_tagNew);
  connect(m_tagNew, &QAction::triggered,
          this, &TreeManager::contextNewTag);

  m_tagContextMenu->addSeparator();

  m_tagRename = new QAction(tr("&Rename tag"));
  m_tagContextMenu->addAction(m_tagRename);
  connect(m_tagRename, &QAction::triggered,
          this, &TreeManager::contextRenameTag);

  m_tagDelete = new QAction(tr("&Delete tag"));
  m_tagContextMenu->addAction(m_tagDelete);
  connect(m_tagDelete, &QAction::triggered,
          this, &TreeManager::contextDeleteTag);

  ////////////////////////////////////
  // END: CONTEXT MENUS
  ////////////////////////////////////

  // Signal Connections
  connect(m_tree_view->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &TreeManager::treeItemChanged);
  connect(m_db->tagDatabase(), &TagDatabase::tagAdded,
          this, &TreeManager::tagAdded);
  connect(m_db->tagDatabase(), &TagDatabase::tagRemoved,
          this, &TreeManager::tagRemoved);
  connect(m_db->tagDatabase(), &TagDatabase::tagChanged,
          this, &TreeManager::tagChanged);
  connect(m_db->notebookDatabase(), &NotebookDatabase::notebookAdded,
          this, &TreeManager::notebookAdded);
  connect(m_db->notebookDatabase(), &NotebookDatabase::notebooksRemoved,
          this, &TreeManager::notebooksRemoved);

  // Load databases
  loadNotebooksFromNotebookDatabase( m_db->notebookDatabase() );
  loadTagsFromTagDatabase( m_db->tagDatabase() );
}

TreeManager::~TreeManager()
{
  delete m_notebookContextMenu;
  delete m_notebookNew;
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
  int count = m_notebooks->childCount();
  for (int i = 0; i < count; i++)
    m_notebooks->removeChild(0);
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

void TreeManager::loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase, bool expandAll)
{
  clearNotebooks();
  for (int i = 0; i < notebookDatabase->size(); i++) {
    loadNotebookObjectAndChildren(notebookDatabase->list()[i]);
  }
  if (expandAll)
    m_tree_view->expandAll();
}

void TreeManager::gotoAllNotesTab()
{
  // Set the current selection to the first item in the treeview..which is "All Notes"
  // Explanation: https://stackoverflow.com/questions/15817429/how-to-get-list-of-visible-qmodelindex-in-qabstractitemview
  QModelIndex all_notes_index = m_tree_model->index(0, 0);
  m_tree_view->setCurrentIndex( all_notes_index );
  treeItemChanged(all_notes_index, QModelIndex());

}

QVector<BasicTreeItem*> TreeManager::recurseNotebooks(BasicTreeItem *parent)
{
  QVector<BasicTreeItem*> children;
  if ( parent != nullptr )
    children.append(parent);
  else
    parent = m_notebooks;

  for ( BasicTreeItem *child : parent->children() )
    children.append( recurseNotebooks(child) );

  return children;
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
  int count = m_tags->childCount();
  for (int i = 0; i < count; i++)
    m_tags->removeChild(0);
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
        m_manager->noteListManager()->showAllNotesView();
      }

    else if (item == m_favorites)
      {   // Selected all notes label
        m_manager->noteListManager()->showFavoritesView();
      }

    else if ( item == m_notebooks )
      {   // Selected notebooks label
        m_manager->noteListManager()->filterOutEverything();
      }

    else if ( item->isNotebook() )
      {   // Selected a notebook
        Notebook *notebook = item->object().notebook;
        m_manager->noteListManager()->showNotebookView(notebook);
      }

    else if ( item == m_tags)
      {   // Selected tags label
        m_manager->noteListManager()->filterOutEverything();
      }

    else if ( item->isTag() )
      {   // Selected a tag
        Tag *tag = item->object().tag;
        m_manager->noteListManager()->showTagView(tag);
      }

    else if ( item == m_trash )
      {
        m_manager->noteListManager()->showTrashView();
      }

    else
      {   // Selected something else. Clear for safety.
        m_manager->noteListManager()->filterOutEverything();
        qWarning("Selected something weird in the tree list :S");
      }
  } else {
    qWarning("Hmmm..Current tree selection is same as previous. This should NOT happen. [treemanager.cpp]");
  }
}

void TreeManager::tagAdded(Tag *tag)
{
  BasicTreeItem *item = addTag(tag);

  // If user wants to open a notebook for editing
  if ( m_openNewTagForEditing ) {
    // Get the model index of the item and open it for editing
    QModelIndex index = m_tree_model->getItem(item);
    m_tree_view->edit(index);

    // Reset openNewNotebookForEditing to false.
    m_openNewTagForEditing = false;
  }
}

void TreeManager::tagRemoved(int tagID) {
  bool wasViewingDeletedTag = false;
  if ( m_curItem->isTag() &&
       m_curItem->id() == tagID )
    {
      wasViewingDeletedTag = true;
    }

  loadTagsFromTagDatabase(m_db->tagDatabase());

  if ( wasViewingDeletedTag ) {
    QModelIndex all_notes_index = m_tree_model->index(0, 0);
    m_tree_view->setCurrentIndex( all_notes_index );
  }
}

void TreeManager::tagChanged(Tag *tag)
{

}

void TreeManager::notebookAdded(Notebook *notebook)
{
  // Inefficient! But it works for now
  loadNotebooksFromNotebookDatabase(m_db->notebookDatabase());

  // If user wants to open a notebook for editing
  if ( m_openNewNotebookForEditing ) {
    // Get BasicTreeItems of all notebooks
    QVector<BasicTreeItem*> notebooks = m_notebooks->recurseChildren();
    // Loop through them
    for (BasicTreeItem *item : notebooks) {
      // If the notebook object of the item IS the newly added notebook then...
      if (item->isNotebook() &&
          item->object().notebook == notebook) {
        // ...find the qmodelindex and open for editing!
        QModelIndex index = m_tree_model->getItem(item);
        m_tree_view->edit(index);
        break;
      }
    }
    // Reset openNewNotebookForEditing to false.
    m_openNewNotebookForEditing = false;
  }
}

void TreeManager::notebooksRemoved(QVector<int> notebookIDs)
{
  // Lazy way. Re-draws notebook tree. The proper way would be to simply remove a single BasicTreeItem.
  // TODO: Implement TreeManager::notebooksRemoved the proper way!
  bool wasViewingDeletedNotebook = false;
  if ( m_curItem->isNotebook() &&
       notebookIDs.contains( m_curItem->id() ) )
    {
      wasViewingDeletedNotebook = true;
    }

  loadNotebooksFromNotebookDatabase(m_db->notebookDatabase());

  if ( wasViewingDeletedNotebook ) {
    QModelIndex all_notes_index = m_tree_model->index(0, 0);
    m_tree_view->setCurrentIndex( all_notes_index );
  }
}

void TreeManager::notebookChanged(Notebook *notebook)
{

}

void TreeManager::contextNewNotebook()
{
  // Make sure the next notebook is opened for editing (Renaming)
  m_openNewNotebookForEditing = true;

  // Add notebook to notebook database
  if ( m_currentContextIndex->isNotebook() && m_currentContextIndex->object().notebook->id() != NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    m_db->notebookDatabase()->addNotebook(NOTEBOOK_DEFAULT_TITLE, m_currentContextIndex->object().notebook);
  else
    m_db->notebookDatabase()->addNotebook(NOTEBOOK_DEFAULT_TITLE, nullptr);
}

void TreeManager::contextDeleteNotebook()
{
  if ( m_currentContextIndex->isNotebook() )
    m_db->notebookDatabase()->removeNotebook(m_currentContextIndex->object().notebook);
}

void TreeManager::contextRenameNotebook()
{
  if ( m_currentContextModelIndex.isValid() )
    m_tree_view->edit(m_currentContextModelIndex);
}

void TreeManager::contextEditNotebookHierarchy()
{

}

void TreeManager::contextNewTag()
{
  // Make sure the next tag is opened for editing (Renaming)
  m_openNewTagForEditing = true;

  // Add tag to tag database
  m_db->tagDatabase()->addTag(TAG_DEFAULT_TITLE);
}

void TreeManager::contextDeleteTag()
{
  if ( m_currentContextIndex->isTag() )
    m_db->tagDatabase()->removeTag(m_currentContextIndex->object().tag);
}

void TreeManager::contextRenameTag()
{
  if ( m_currentContextModelIndex.isValid() )
    m_tree_view->edit(m_currentContextModelIndex);
}

void TreeManager::treeContextMenu(const QPoint &point)
{
  QModelIndex index = m_tree_view->indexAt(point);
  if ( !index.isValid() )
    return;
  BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());
  m_currentContextModelIndex = index;
  m_currentContextIndex = item;

  QPoint p = m_tree_view->viewport()->mapToGlobal(point); // Pass this as an arg when executing context menu

  if ( item == m_notebooks || item->isNotebook() ) {
    bool showEditingControls;
    if ( item == m_notebooks )
      showEditingControls = false;
    else
      showEditingControls = true;

    m_notebookRename->setVisible(showEditingControls);
    m_notebookEditHierarchy->setVisible(showEditingControls);
    m_notebookDelete->setVisible(showEditingControls);

    // Don't allow editing of default notebook
    if ( item->isNotebook() && item->object().notebook->id() == NOTEBOOK_DEFAULT_NOTEBOOK_ID ) {
      m_notebookRename->setVisible(false);
      m_notebookEditHierarchy->setVisible(false);
    }

    m_notebookContextMenu->exec(p);
  }
  else if ( item == m_tags || item->isTag() ) {
    bool showEditingControls;
    if ( item == m_tags )
      showEditingControls = false;
    else
      showEditingControls = true;

    m_tagRename->setVisible(showEditingControls);
    m_tagDelete->setVisible(showEditingControls);

    m_tagContextMenu->exec(p);
  }
}

void TreeManager::openNotebookWithID(int notebookID)
{
  for (BasicTreeItem *item : recurseNotebooks() ) {
    if ( item->object().notebook->id() == notebookID ) {
      QModelIndex index = m_tree_model->getItem(item);
      if (index.isValid())
        m_tree_view->setCurrentIndex(index);
      else
        qWarning() << "Unable to set current index of tree to notebook id" << notebookID;
      return;
    }
  }
}
