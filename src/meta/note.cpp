#include "note.h"
#include <QDateTime>
#include <QTimeZone>

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
	if (m_id == id)
		return;
	m_id = id;
	emit noteChanged( this );
}

QString Note::title() const
{
	return m_title;
}

void Note::setTitle(const QString title)
{
	if (!QString::compare(m_title, title)) // if m_table and title are same, exit
		return;
	m_title = title;
	emit noteChanged( this );
}

QString Note::text() const
{
	return m_text;
}

void Note::setText(const QString text)
{
	if (!QString::compare(m_text, text)) // If m_text and text are the same, exit
		return;
	m_text = text;
	emit noteChanged( this );
}

QDateTime Note::date_created() const
{
	return m_date_created;
}

void Note::setDate_created(const QDateTime &date_created)
{
	if (!QString::compare(m_date_created.toString(), date_created.toString())) // If dates are same, exit
		return;
	m_date_created = date_created;
	emit noteChanged( this );
}


QDateTime Note::date_modified() const
{
	return m_date_modified;
}

void Note::setDate_modified(const QDateTime &date_modified)
{
	if (!QString::compare(m_date_modified.toString(), date_modified.toString())) // If dates are same, exit
		return;
	m_date_modified = date_modified;
	emit noteChanged( this );
}

int Note::notebook() const
{
	return m_notebook;
}

void Note::setNotebook(int id)
{
	if (m_notebook == id)
		return;
	m_notebook = id;
	emit noteChanged( this );
}

QList<int> Note::tags() const
{
	return m_tags;
}

void Note::setTags(const QList<int> &tags)
{
	m_tags = tags;
	emit noteChanged( this );
}
