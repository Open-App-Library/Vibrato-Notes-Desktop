#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H
#include <QListWidgetItem>
#include "note.h"
#include "ui_notelistitem.h"

#define NOTE_LIST_ITEM_HEIGHT 50

namespace Ui {
class NoteListItem;
}

class NoteListItem
{
public:
    NoteListItem(Note *note);
    ~NoteListItem();
private:
    Note *m_note;
    Ui::NoteListItem *m_ui_class;
    QListWidgetItem  *m_list_item;
    QWidget          *m_list_item_widget; // A child of m_list_item
};

#endif // NOTELISTITEM_H
