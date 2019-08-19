#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QVector>
#include "../sortfilter/notelistproxymodel.h"
#include "../models/views/customlistview.h"
#include "../models/notelistmodel.h"
#include "../models/items/notelistitem.h"
#include "../meta/note.h"
#include "../meta/db/database.h"
#include "manager.h"
#include "notelist-views/trashview.h"
#include <ui_notelist_addons.h>

namespace Ui {
  class NoteListAddonsWidget;
}

typedef Ui::NoteListAddonsWidget NoteListAddonsWidget;

class NoteListManager : public QObject
{
  Q_OBJECT
public:
  NoteListManager(CustomListView *view, QWidget *noteListAddons, Database *db, Manager *manager);
  ~NoteListManager();

  enum viewingModes {View_AllNotes, View_Favorites, View_Notebook, View_Tag, View_Trash, View_Search};

  QListView *view() const;
  NoteListModel *model() const;
  NoteListProxyModel *proxyModel() const;
  NoteListAddonsWidget *addonsWidgetUi() const;

  NoteListItem *add_note(Note *note);
  void remove_note(int index);
  void clear();
  void filterOutEverything(bool shouldFilterOutEverything=true);
  void loadNotesFromNoteDatabase();
  void loadNotesFromNoteDatabase(NoteDatabase *noteDatabase);

  void openIndexInEditor(int index);

  // Filter controls
  void clearFilter(bool invalidate=true);
  void addNotebookToFilter(Notebook *notebook);
  void addTagToFilter(Tag *tag);

  int curViewType() const;
  QUuid curViewType_ItemSyncHash() const;
  Notebook *curViewType_Notebook() const;
  Tag *curViewType_Tag() const;

  void disconnectCurrentView();
  void showAllNotesView();
  void showFavoritesView();
  void showNotebookView(Notebook *notebook);
  void showTagView(Tag *tag);
  void showTrashView();
  void showSearchQueryView(QString searchQuery);

  void setTitle(QString title);
  void setMetrics(int count, QString objectTypeSingular, QString pluralOverride=QString("")); // objectTypeSingular - ex. "note", "notebook"
  void hideTitle();
  void hideMetrics();
  void hideAddons();

  void deselect();

public slots:
  void noteListItemChanged(const QModelIndex &current_proxy, const QModelIndex &previous_proxy);
  void ensureCurrentNoteIsSelected();

private slots:
  void managerIsReady();
  void notebooksDeleted(QVector<QUuid> notebookSyncHashes);
  void tagDeleted(QUuid tagSyncHash);
  void favoritedChanged(void);
  void trashedOrRestored(void);
  void escribaDeselected();

  // When a notebook or tag changes in the notebook/tag database
  void aNoteChanged(Note *note);
  void aNotebookChanged(Notebook *notebook);
  void aTagChanged(Tag *tag);

  void removeSearchQuery();

signals:
  void selectedNote(Note *note);

private:
  CustomListView *m_view;
  NoteListAddonsWidget *m_noteListAddonsUi;
  QWidget *m_noteListAddons;
  Manager *m_manager;

  // m_model is the global list of notes
  // m_proxyModel is the sorted & filtered list of notes, based on m_model.
  NoteListModel *m_model;
  NoteListProxyModel *m_proxyModel;

  int m_curViewType=View_AllNotes;
  QUuid m_curViewType_ItemSyncHash;
  Notebook *m_curViewType_Notebook=nullptr;
  Tag *m_curViewType_Tag=nullptr;
  TrashView *m_trashView=nullptr;

  Database *m_db;
};

#endif // NOTELISTMANAGER_H
