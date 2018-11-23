#include "note.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>

Note::Note(int id, QString title, QString text, QDateTime date_created, QDateTime date_modified, int notebook, QVector<int> tags)
{
	m_id = id;
	m_title = title;
	m_text  = text;
	m_date_created = date_created;
	m_date_modified = date_modified;
	m_notebook = notebook;
	m_tags = tags;
	connect(this, &Note::noteChanged,
					this, &Note::handleNoteChange);
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
	if (!QString::compare(m_text.trimmed(), text.trimmed())) // If m_text and text are the same, exit
		return;
	m_text = text;
	emit noteChanged( this );
}

QDateTime Note::date_created() const
{
	return m_date_created;
}

QString Note::date_created_str()
{
	return m_date_created.toString("MMMM d, yyyy");
}

QString Note::date_created_str_informative()
{
	return informativeDate( m_date_created );
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

QString Note::date_modified_str()
{

#ifdef UNIT_TEST
	QDateTime currentDateTime = QDateTime::fromString("2000-12-25T09:38:59Z", Qt::ISODate);
#else
	QDateTime currentDateTime = QDateTime::currentDateTime();
#endif

	// Time difference in seconds
	int td_sec = static_cast<int>(currentDateTime.toSecsSinceEpoch() - m_date_modified.toSecsSinceEpoch());
	int secs_in_minute = 60;
	int secs_in_hour   = 3600;
	int secs_in_year   = 31557600;

	int amount = 0;
	QString unit = ""; // years, months, hours, minutes,
	int diviser = 1;

	if (td_sec < secs_in_minute)
		return "Just now";
	else if ( td_sec >= secs_in_year ) {
		unit = "year";
		diviser = secs_in_year;
	}
	else if ( currentDateTime.date().month() != m_date_modified.date().month() ) {
		int m = m_date_modified.date().month();
		int months_since = 0;
		while ( m != currentDateTime.date().month() ) {
			months_since++;
			if ( m == 12 )
				m = 1;
			else
				m++;
		}
		unit = "month";
		if ( months_since > 1 )
			unit.append('s');
		return QString("%1 %2 ago").arg(numberToString(months_since, true), unit);
	}
	else if ( td_sec >= secs_in_hour ) {
		unit = "hour";
		diviser = secs_in_hour;
	}
	else if ( td_sec >= secs_in_minute ) {
		unit = "minute";
		diviser = secs_in_minute;
	}
	amount = td_sec / diviser;
	if ( amount > 1 )
		unit.append('s');
	return QString("%1 %2 ago").arg(numberToString(amount, true), unit);
}

QString Note::date_modified_str_informative()
{
	return informativeDate( m_date_modified );
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

QVector<int> Note::tags() const
{
	return m_tags;
}

void Note::setTags(const QVector<int> &tags)
{
	m_tags = tags;
	emit noteChanged( this );
}

void Note::handleNoteChange(Note *note)
{
	(void) note; // Ignore compiler warning of unused variable
	setDate_modified( QDateTime::currentDateTime() );
}

QString Note::informativeDate(QDateTime date)
{
	QString dateStr = date.toString("MMMM d, yyyy");
	QString timeStr = date.toString("h:mA t");
	return QString("%1 at %2").arg( dateStr, timeStr );
}

QString Note::numberToString(int number, bool capitalize)
{
	QString str;
	switch (number) {
	case 1:
		str = "one";
		break;
	case 2:
		str = "two";
		break;
	case 3:
		str = "three";
		break;
	case 4:
		str = "four";
		break;
	case 5:
		str = "five";
		break;
	case 6:
		str = "six";
		break;
	case 7:
		str = "seven";
		break;
	case 8:
		str = "eight";
		break;
	case 9:
		str = "nine";
		break;
	case 10:
		str = "ten";
		break;
	default:
		return QString("%1").arg(number);
	}
	if ( capitalize )
		str[0] = str[0].toUpper();
	return str;
}
