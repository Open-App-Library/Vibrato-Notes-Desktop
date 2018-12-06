#include "notelistitem.h"
#include "ui_notelistitem.h"
#include <QDebug>
#include <QPushButton>

NoteListItem::NoteListItem(Note *note) :
  m_note(note)
{
  if (note != nullptr) {
    connect(note, &Note::noteDateCreatedChanged,
            this, &NoteListItem::noteDateChanged_slot);
    connect(note, &Note::noteDateModifiedChanged,
            this, &NoteListItem::noteDateChanged_slot);
    connect(note, &Note::noteIdChanged,
            this, &NoteListItem::noteIDChanged);
  }
}

NoteListItem::~NoteListItem()
{
  //delete m_ui_class;
  m_note = nullptr;
}

Note *NoteListItem::note()
{
  return m_note;
}

void NoteListItem::setNote(Note *note)
{
  m_note = note;
  m_id = note->id();
  connect(note, &Note::noteDateCreatedChanged,
          this, &NoteListItem::noteDateChanged_slot);
  connect(note, &Note::noteDateModifiedChanged,
          this, &NoteListItem::noteDateChanged_slot);
  connect(note, &Note::noteIdChanged,
          this, &NoteListItem::noteIDChanged);
}

QWidget *NoteListItem::widget()
{
  return m_widget;
}

void NoteListItem::setWidget(QWidget *widget)
{
  m_widget = widget;
}

void NoteListItem::setSelectedStyle(bool selected)
{
  if ( m_widget == nullptr )
    return;
  if (selected)
    m_widget->setStyleSheet("color: palette(Light)");
  else
    m_widget->setStyleSheet("color: initial");
}

int NoteListItem::id() {
  return m_id;
}

void NoteListItem::noteDateChanged_slot(Note *note)
{
  (void)note; // Ignore compiler warning
  emit noteDateChanged(this);
}

void NoteListItem::noteIDChanged(Note* note) {
  m_id = note->id();
}
