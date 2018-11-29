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

    m_title_label        = m_ui_class->titleLabel;
    m_excerpt_label      = m_ui_class->excerptLabel;
    m_date_created_label = m_ui_class->dateCreatedLabel;

    if (note != nullptr) {
        connect(note, &Note::noteDateCreatedChanged,
                this, &NoteListItem::noteDateChanged_slot);
        connect(note, &Note::noteDateModifiedChanged,
                this, &NoteListItem::noteDateChanged_slot);
    }
}

NoteListItem::~NoteListItem()
{
    delete m_ui_class;
    delete m_widget;
    m_widget = nullptr;
    m_note = nullptr;
}

Note *NoteListItem::note()
{
    return m_note;
}

void NoteListItem::setNote(Note *note)
{
    m_note = note;
    connect(note, &Note::noteDateCreatedChanged,
            this, &NoteListItem::noteDateChanged_slot);
    connect(note, &Note::noteDateModifiedChanged,
            this, &NoteListItem::noteDateChanged_slot);
}

void NoteListItem::setSelectedStyle(bool selected)
{
    if (selected)
        m_widget->setStyleSheet("color: palette(Light)");
    else
        m_widget->setStyleSheet("color: initial");
}

void NoteListItem::noteDateChanged_slot(Note *note)
{
    (void)note; // Ignore compiler warning
    emit noteDateChanged(this);
}
