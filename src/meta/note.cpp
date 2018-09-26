#include "note.h"

Note::Note()
{
    m_title = "";
    m_text  = "";
}

QString Note::title() const
{
    return m_title;
}

void Note::setTitle(const QString title)
{
    m_title = title;
}

QString Note::text() const
{
    return m_text;
}

void Note::setText(const QString text)
{
    m_text = text;
}

QDate Note::date_created() const
{
    return m_date_created;
}

void Note::setDate_created(const QDate &date_created)
{
    m_date_created = date_created;
}


QDate Note::date_modified() const
{
    return m_date_modified;
}

void Note::setDate_modified(const QDate &date_modified)
{
    m_date_modified = date_modified;
}

Notebook *Note::notebook() const
{
    return m_notebook;
}

void Note::setNotebook(Notebook *value)
{
    m_notebook = value;
}

QList<Tag *> Note::getTags() const
{
    return m_tags;
}

void Note::setTags(const QList<Tag *> &value)
{
    m_tags = value;
}

int Note::getId() const
{
    return id;
}

void Note::setId(int value)
{
    id = value;
}
