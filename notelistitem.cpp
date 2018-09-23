#include "notelistitem.h"

NoteListItem::NoteListItem(Note *note, QListWidget *listWidget) :
    m_note(note),
    m_ui_class(new Ui::NoteListItem),
    m_list_widget(listWidget)
{
    m_list_item = new QListWidgetItem(listWidget);
    m_list_item->setSizeHint( QSize(m_list_item->sizeHint().width(), NOTE_LIST_ITEM_HEIGHT) );

    m_list_item_widget = new QWidget;
    m_ui_class->setupUi(m_list_item_widget);

    m_list_widget->setItemWidget(m_list_item, m_list_item_widget);
}

NoteListItem::~NoteListItem()
{
    delete m_ui_class;
    delete m_list_item;
    delete m_list_item_widget;
}

void NoteListItem::trash()
{
    m_list_widget->removeItemWidget(m_list_item);
    delete m_list_item;
    delete m_ui_class;
    delete m_list_item_widget;
}
