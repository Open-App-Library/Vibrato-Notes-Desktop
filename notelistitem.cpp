#include "notelistitem.h"

NoteListItem::NoteListItem(Note *note) :
    m_note(note),
    m_ui_class(new Ui::NoteListItem)
{
    m_list_item = new QListWidgetItem;
    m_list_item->setSizeHint( QSize(m_list_item->sizeHint().width(), NOTE_LIST_ITEM_HEIGHT) );

    m_list_item_widget = new QWidget;
    m_ui_class->setupUi(m_list_item_widget);

    m_list_item->
}
