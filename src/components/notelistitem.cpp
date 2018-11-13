#include "notelistitem.h"
#include <QDebug>

NoteListItem::NoteListItem(Note *note, QListWidget *listWidget) :
	QListWidgetItem(listWidget),
	m_note(note),
	m_ui_class(new Ui::NoteListItem),
	m_list_widget(listWidget)
{
	//	listWidget->addItem( this );
	this->setSizeHint( QSize(this->sizeHint().width(), NOTE_LIST_ITEM_HEIGHT) );

	m_list_item_widget = new QWidget;
	m_ui_class->setupUi(m_list_item_widget);

	m_list_widget->setItemWidget(this, m_list_item_widget);
	m_title_label   = m_ui_class->titleLabel;
	m_excerpt_label = m_ui_class->excerptLabel;

	connect(note, &Note::noteChanged,
					this, &NoteListItem::noteChanged);

	updateUI();
}

NoteListItem::~NoteListItem()
{

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
	m_list_widget->removeItemWidget(this);
	delete m_ui_class;
	// delete m_list_item_widget;
	delete this;
}

Note *NoteListItem::note()
{
	return m_note;
}

void NoteListItem::noteChanged(Note *note)
{
	(void) note; // Avoid unused argument compiled warning
	updateUI();
}
