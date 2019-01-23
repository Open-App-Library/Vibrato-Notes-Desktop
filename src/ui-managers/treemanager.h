/*
 * "I speak for the trees" - The Lorax
 *
 * The purpose of this file is to control the tree view
 * and provide a flexible API for managing notebooks
 * and tags.
 */

#ifndef TREEMANAGER_H
#define TREEMANAGER_H
#include <QObject>
#include <QDebug>
#include <QMenu>
#include "../models/views/customtreeview.h"
#include "../models/items/basictreeitem.h"
#include "../models/treemodel.h"
#include "../meta/db/notebookdatabase.h"
#include "../meta/db/database.h"
#include "manager.h"
#include "notelistmanager.h"
#include "../../ui/notebook_editparent.h"

class TreeManager : public QObject
{
  Q_OBJECT
public:
  explicit TreeManager(CustomTreeView *treeView, Database *db, Manager *manager);
  ~TreeManager();

  enum TreeItemTypes {TreeType_AllNotes, TreeType_Notebooks, TreeType_Notebook, TreeType_Tags, TreeType_Tag, TreeType_Other};

  void update(); // Refreshes GUI

  // Notebook functions
  QVector<BasicTreeItem*> notebooks() const;

  BasicTreeItem *curItem();

  BasicTreeItem *addNotebook(Notebook *notebook);
  BasicTreeItem *addNotebook(Notebook *notebook, BasicTreeItem *parent);

  void removeNotebook(BasicTreeItem *item);
  void removeNotebook(BasicTreeItem *item, BasicTreeItem *fosterParent);

  void clearChildren(BasicTreeItem *item);
  void clearNotebooks();

  void loadNotebookObjectAndChildren(Notebook *notebook, BasicTreeItem *parent=nullptr);
  void loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase, bool expandAll=true);

  QModelIndex findIndexFromTreeItem(BasicTreeItem *item);
  void selectItem(BasicTreeItem *item, bool triggerItemChangedEvent=true);
  void selectItem(QModelIndex index, bool triggerItemChangedEvent=true);
  void gotoAllNotesTab();

  QVector<BasicTreeItem*> recurseNotebooks(BasicTreeItem *parent=nullptr);

  // Tag functions
  QVector<BasicTreeItem*> tags() const;
  BasicTreeItem          *addTag(Tag *tag);
  void                    removeTag(BasicTreeItem *item);
  void                    clearTags();

  void loadTagsFromTagDatabase(TagDatabase *tagDatabase);

  // SearchQuery functions
  BasicTreeItem *addSearchQuery(QString searchQuery);

  // Signal Callbacks
  void treeItemChanged(const QModelIndex &current, const QModelIndex &previous);

  void treeContextMenu(const QPoint &point);

  // Navigation
  void openNotebookWithID(int notebookID);

  // Removes a treeitem if isSearchQuery().
  // If item == nullptr (default) it will try to delete the selected item.
  void removeSearchQuery(BasicTreeItem *item=nullptr);

private slots:
  void tagAdded(Tag *tag);
  void tagRemoved(int tagID);
  void tagChanged(Tag *tag);

  void notebookAdded(Notebook *notebook);
  void notebooksRemoved(QVector<int> notebookIDs);
  void notebookChanged(Notebook *notebook);

  void contextNewNotebook();
  void contextDeleteNotebook();
  void contextRenameNotebook();

  void contextNewTag();
  void contextDeleteTag();
  void contextRenameTag();

  void contextRemoveSearchQuery();

private:
  TreeModel       *m_tree_model;
  CustomTreeView  *m_tree_view;
  Manager *m_manager;
  Database *m_db;

  BasicTreeItem *m_curItem;

  BasicTreeItem *m_all_notes;
  BasicTreeItem *m_favorites;
  BasicTreeItem *m_notebooks;
  BasicTreeItem *m_tags;
  BasicTreeItem *m_trash;

  // Will open newly-created notebook or tag for editing (Renaming) after notebookAdded or tagAdded slot called.
  bool m_openNewNotebookForEditing=false;
  bool m_openNewTagForEditing=false;

  QMenu *m_notebookContextMenu;
  QAction *m_notebookNew;
  QAction *m_notebookRename;
  QAction *m_notebookDelete;

  QMenu *m_tagContextMenu;
  QAction *m_tagNew;
  QAction *m_tagRename;
  QAction *m_tagDelete;

  QMenu *m_searchQueryContextMenu;
  QAction *m_searchQueryRemove;

  // TODO: https://forum.qt.io/topic/45262/disable-certain-rows-in-qtreeview
  BasicTreeItem *m_no_notebooks_placedholder = nullptr;
  BasicTreeItem *m_no_tags_placedholder = nullptr;

  BasicTreeItem *m_currentContextIndex = nullptr;
  QModelIndex   m_currentContextModelIndex;

  void add_no_notebooks_placeholder();
  void add_no_tags_placeholder();
  void remove_no_notebooks_placeholder();
  void remove_no_tags_placeholder();
};

#endif // TREEMANAGER_H
