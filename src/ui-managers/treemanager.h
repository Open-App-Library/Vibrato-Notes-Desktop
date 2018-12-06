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
#include <QTreeView>
#include <QDebug>
#include <QMenu>
#include "../models/items/basictreeitem.h"
#include "../models/treemodel.h"
#include "../meta/db/notebookdatabase.h"
#include "../meta/db/database.h"
#include "notelistmanager.h"

class TreeManager : public QObject
{
  Q_OBJECT
public:
  explicit TreeManager(QTreeView *treeView, NoteListManager *noteListManager, Database *db, QObject *parent=nullptr);
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

  // Tag functions
  QVector<BasicTreeItem*> tags() const;
  BasicTreeItem          *addTag(Tag *tag);
  void                    removeTag(BasicTreeItem *item);
  void                    clearTags();

  void loadTagsFromTagDatabase(TagDatabase *tagDatabase);

  // Signal Callbacks
  void treeItemChanged(const QModelIndex &current, const QModelIndex &previous);

  void treeContextMenu(const QPoint &point);

public slots:
  void tagAdded(Tag *tag);
  void tagChanged(Tag *tag);

  void notebookAdded(Notebook *notebook);
  void notebooksRemoved(QVector<int> notebookIDs);
  void notebookChanged(Notebook *notebook);

  void contextNewNotebook();
  void contextDeleteNotebook();
  void contextRenameNotebook();
  void contextEditNotebookHierarchy();

private:
  TreeModel       *m_tree_model;
  QTreeView       *m_tree_view;
  NoteListManager *m_note_list_manager;
  Database *m_db;

  BasicTreeItem *m_curItem;

  BasicTreeItem *m_all_notes;
  BasicTreeItem *m_favorites;
  BasicTreeItem *m_notebooks;
  BasicTreeItem *m_tags;
  BasicTreeItem *m_trash;

  void setContextEditingControlVisability(bool visible);
  QMenu *m_notebookContextMenu;
  QAction *m_notebookNew;
  QAction *m_notebookRename;
  QAction *m_notebookEditHierarchy;
  QAction *m_notebookDelete;

  // TODO: https://forum.qt.io/topic/45262/disable-certain-rows-in-qtreeview
  BasicTreeItem *m_no_notebooks_placedholder = nullptr;
  BasicTreeItem *m_no_tags_placedholder = nullptr;

  BasicTreeItem *m_currentContextIndex = nullptr;

  void add_no_notebooks_placeholder();
  void add_no_tags_placeholder();
  void remove_no_notebooks_placeholder();
  void remove_no_tags_placeholder();
};

#endif // TREEMANAGER_H
