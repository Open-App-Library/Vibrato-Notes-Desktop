#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QVector>
#include "escribamanager.h"
#include "../sortfilter/notelistproxymodel.h"
#include "../models/views/customlistview.h"
#include "../models/notelistmodel.h"
#include "../models/items/notelistitem.h"
#include "../meta/note.h"
#include "../meta/db/database.h"
#include "../meta/filter/notefilter.h"
#include <ui_notelist_addons.h>

namespace Ui {
	class NoteListAddonsWidget;
}

class NoteListManager : public QObject
{
	Q_OBJECT
public:
	NoteListManager(CustomListView *view, QWidget *noteListAddons, EscribaManager *escribaManager, Database *db);
	~NoteListManager();
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

	void showAllNotesView();
	void showFavoritesView();
	void showNotebookView(Notebook *notebook);
	void showTagView(Tag *tag);
	void showTrashView();
  void showSearchView(QString searchQuery);

	void setTitle(QString title);
	void setMetrics(int count, QString objectTypeSingular, QString pluralOverride=QString("")); // objectTypeSingular - ex. "note", "notebook"
  void hideTitle();
	void hideMetrics();
  void hideAddons();

public slots:
	void noteListItemChanged(const QModelIndex &current_proxy, const QModelIndex &previous_proxy);

private slots:
	void rowsInsertedInProxy(const QModelIndex & parent, int start, int end);


signals:
	void selectedNote(Note *note);

private:
	CustomListView *m_view;
	Ui::NoteListAddonsWidget *m_noteListAddonsUi;
	QWidget *m_noteListAddons;

	// m_model is the global list of notes
	// m_proxyModel is the sorted & filtered list of notes, based on m_model.
	NoteListModel *m_model;
	NoteListProxyModel *m_proxyModel;

	EscribaManager *m_escribaManager;

	NoteFilter *m_filter;
	Database *m_db;
};

#endif // NOTELISTMANAGER_H
