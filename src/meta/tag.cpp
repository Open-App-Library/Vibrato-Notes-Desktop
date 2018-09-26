#include "tag.h"

Tag::Tag()
{
    m_id = -1;
    m_title = "Untitled Tag";
}

int Tag::id() const
{
    return m_id;
}

void Tag::setId(int id)
{
    m_id = id;
}

QString Tag::title() const
{
    return m_title;
}

void Tag::setTitle(const QString &title)
{
    m_title = title;
}
