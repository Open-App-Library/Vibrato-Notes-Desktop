#include "notelistitem.h"
#include "ui_notelistitem.h"
#include <QDebug>
#include <QPushButton>

NoteListItem::NoteListItem(Note *note) :
    m_ui_class(new Ui::NoteListItem),
	m_note(note)
{
    m_widget = new QWidget;
    m_ui_class->setupUi(m_widget);

    m_title_label   = m_ui_class->titleLabel;
    m_excerpt_label = m_ui_class->excerptLabel;

//    connect(note, &Note::noteChanged,
//            this, &NoteListItem::noteChanged);

    if (note != nullptr)
        updateWidget();
}

NoteListItem::~NoteListItem()
{
    delete m_ui_class;
    delete m_widget;
}

Note *NoteListItem::note()
{
    return m_note;
}

void NoteListItem::setNote(Note *note)
{
    m_note = note;
    updateWidget();
}

QWidget *NoteListItem::widget()
{
    return m_widget;
}

void NoteListItem::deleteWidget()
{
    if ( m_widget != nullptr ) {
        delete m_widget;
        m_widget = nullptr;
    }
}

void NoteListItem::updateWidget()
{
	// Set title and excerpt
    m_title_label->setText(m_note->title());
    QString excerpt = m_note->text();
    if (excerpt.length() > 50) {
        excerpt = m_note->text().mid(0, 50) + "...";
    }
    m_excerpt_label->setText(excerpt);
}

void NoteListItem::noteChanged(Note *note)
{
	updateWidget();
}
