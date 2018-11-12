#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QList>
#include <QListWidget>
#include "ui_notelistitem.h"
#include "../components/notelistitem.h"
#include "../meta/note.h"
#include "../meta/db/database.h"
#include "../meta/filter/notefilter.h"

class NoteListManager : public QObject
{
	Q_OBJECT
public:
	NoteListManager(QListWidget *listWidget, Database *db);
	~NoteListManager();
	QList<QWidget*> *noteList();
	NoteListItem *add_note(Note *note);
	void remove_note(int index);
	void clear();
	void loadNotesFromNoteDatabase();
	void loadNotesFromNoteDatabase(NoteDatabase *noteDatabase);
	void loadNotesFromNoteFilter(noteFilterList noteList);
	NoteFilter *filter();
	void noteListItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

public slots:
	void selectNote(Note *note);
signals:
	void noteSelected(Note *note);

private:
	QListWidget *m_listWidget;
	QList<NoteListItem*> m_noteList;
	NoteFilter *m_filter;
	Database *m_db;
};

#endif // NOTELISTMANAGER_H
