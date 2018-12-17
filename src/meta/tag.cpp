#include "tag.h"

Tag::Tag(int id, QString title) :
  m_id(id),
  m_title(title)
{
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
  m_title = title;
  emit tagTitleChanged(this);
  emit tagChanged(this);
}
