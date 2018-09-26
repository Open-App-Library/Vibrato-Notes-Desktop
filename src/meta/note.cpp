#include <QDateTime>
#include <QTimeZone>
#include "note.h"

Note::Note()
{
    m_id = -1;
    m_title = "";
    m_text  = "";
    m_notebook = -1;
    m_tags = {};
    // TODO: Implement custom timezones other than UTC
    m_date_created = QDateTime(QDate::currentDate(), QTime::currentTime(), QTimeZone::utc());
    m_date_modified = QDateTime(QDate::currentDate(), QTime::currentTime(), QTimeZone::utc());
}

int Note::id() const
{
    return m_id;
}

void Note::setId(int id)
{
    m_id = id;
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

QDateTime Note::date_created() const
{
    return m_date_created;
}

void Note::setDate_created(const QDateTime &date_created)
{
    m_date_created = date_created;
}


QDateTime Note::date_modified() const
{
    return m_date_modified;
}

void Note::setDate_modified(const QDateTime &date_modified)
{
    m_date_modified = date_modified;
}

int Note::notebook() const
{
    return m_notebook;
}

void Note::setNotebook(int id)
{
    m_notebook = id;
}

QList<int> Note::tags() const
{
    return m_tags;
}

void Note::setTags(const QList<int> &tags)
{
    m_tags = tags;
}
