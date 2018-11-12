#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H
#include <QListWidgetItem>
#include "../meta/note.h"
#include "ui_notelistitem.h"

#define NOTE_LIST_ITEM_HEIGHT 75

namespace Ui {
	class NoteListItem;
}

class NoteListItem : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	NoteListItem(Note *note, QListWidget *listWidget);
	~NoteListItem();
	void updateUI();
	void trash();
	Note *note();

public slots:
	void noteChanged(Note *note);

private:
	Note *m_note;
	Ui::NoteListItem *m_ui_class;
	QListWidget      *m_list_widget;
	QWidget          *m_list_item_widget; // A child of m_list_item
	QLabel           *m_title_label;
	QLabel           *m_excerpt_label;
};

#endif // NOTELISTITEM_H
