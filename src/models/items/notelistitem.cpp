#include "notelistitem.h"
#include "ui_notelistitem.h"
#include <QDebug>
#include <QPushButton>

NoteListItem::NoteListItem(Note *note) :
  m_note(note)
{
}

NoteListItem::~NoteListItem()
{
  m_note = nullptr;
}

Note *NoteListItem::note() const
{
  return m_note;
}

void NoteListItem::setNote(Note *note)
{
  m_note = note;
  m_sync_hash = note->syncHash();
}

QUuid NoteListItem::syncHash() const
{
  return m_sync_hash;
}
