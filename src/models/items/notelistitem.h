#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H
#include <QObject>
#include <QListView>
#include <QLabel>
#include "../../meta/note.h"

#define NOTE_LIST_ITEM_HEIGHT 90

namespace Ui {
	class NoteListItem;
}

class NoteListItem : public QObject
{
	Q_OBJECT
public:
	NoteListItem(Note *note);
    ~NoteListItem();
	Note *note();
    void setNote(Note *note);
	QWidget *widget();
    void deleteWidget();

	void updateWidget();

    void setSelectedStyle(bool selected);

private:
	Ui::NoteListItem *m_ui_class;
	QWidget *m_widget;

	Note *m_note;

	QLabel *m_title_label;
	QLabel *m_excerpt_label;

public slots:
	void noteChanged(Note *note);
};

#endif // NOTELISTITEM_H
