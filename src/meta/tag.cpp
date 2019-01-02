#include "tag.h"

Tag::Tag(int syncId, int id, QString title) :
  m_syncId(syncId),
  m_id(id),
  m_title(title)
{
}

int Tag::syncId() const
{
  return m_id;
}

void Tag::setSyncId(int syncId)
{
  m_syncId = syncId;
  emit tagSyncIDChanged(this);
}

int Tag::id() const
{
  return m_id;
}

void Tag::setId(int id)
{
  m_id = id;
  emit tagIDChanged(this);
  emit tagChanged(this);
}

QString Tag::title() const
{
  return m_title;
}

void Tag::setTitle(const QString &title)
{
  if ( title.trimmed().isEmpty() )
    return;
  m_title = title;
  emit tagTitleChanged(this);
  emit tagChanged(this);
}
