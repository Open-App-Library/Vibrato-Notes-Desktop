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
    m_title_label   = m_ui_class->titleLabel;
    m_excerpt_label = m_ui_class->excerptLabel;

    updateUI();
}

NoteListItem::~NoteListItem()
{
    delete m_ui_class;
    delete m_list_item;
    delete m_list_item_widget;
    delete m_title_label;
    delete m_excerpt_label;
}

void NoteListItem::updateUI()
{
    // Set title and excerpt
    m_title_label->setText(m_note->title());
    QString excerpt = m_note->text();
    if (excerpt.length() > 50) {
        excerpt = m_note->text().mid(0, 50) + "...";
    }
    m_excerpt_label->setText(excerpt);
}

void NoteListItem::trash()
{
    m_list_widget->removeItemWidget(m_list_item);
    delete m_list_item;
    delete m_ui_class;
    delete m_list_item_widget;
}
