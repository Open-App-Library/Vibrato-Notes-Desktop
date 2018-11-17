#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QVector>
#include "escribamanager.h"
#include "../models/views/customlistview.h"
#include "ui_notelistitem.h"
#include "../models/notelistmodel.h"
#include "../models/items/notelistitem.h"
#include "../meta/note.h"
#include "../meta/db/database.h"
#include "../meta/filter/notefilter.h"

class NoteListManager : public QObject
{
	Q_OBJECT
public:
    NoteListManager(CustomListView *view, EscribaManager *escribaManager, Database *db);
	~NoteListManager();
	NoteListItem *add_note(Note *note);
	void remove_note(int index);
	void clear();
	void loadNotesFromNoteDatabase();
	void loadNotesFromNoteDatabase(NoteDatabase *noteDatabase);
	void loadNotesFromNoteFilter(noteFilterList noteList);
	NoteFilter *filter();

    void openIndexInEditor(int index);

public slots:
    void noteListItemChanged(const QModelIndex & current, const QModelIndex & previous);


signals:
    void selectedNote(Note *note);

private:
    CustomListView *m_view;
	NoteListModel *m_model;
    EscribaManager *m_escribaManager;

	NoteFilter *m_filter;
	Database *m_db;
};

#endif // NOTELISTMANAGER_H
