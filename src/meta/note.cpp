#include "note.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>
#include <helper-io.hpp>

Note::Note(int id, QString title, QString text, QDateTime date_created, QDateTime date_modified, bool favorited,int notebook, QVector<int> tags)
{
  m_id = id;
  m_title = title;
  m_text  = text;
  m_date_created = date_created;
  m_date_modified = date_modified;
  m_favorited = favorited;
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
  emit noteChanged( this, false );
  emit noteIdChanged( this );
}

QString Note::title() const
{
  return m_title;
}

void Note::setTitle(const QString title)
{
  QString titleCleaned = title.trimmed();
  if (!QString::compare(m_title, titleCleaned)) // if m_table and title are same, exit
    return;
  m_title = titleCleaned;
  emit noteChanged( this );
  emit noteTitleChanged( this );
}

QString Note::text() const
{
  return m_text;
}

void Note::setText(const QString text)
{
  QString textCleaned = text.trimmed();
  if (!QString::compare(m_text, textCleaned)) // If m_text and text are the same, exit
    return;
  m_text = textCleaned;
  emit noteChanged( this );
  emit noteTextChanged( this );
}

QDateTime Note::date_created() const
{
  return m_date_created;
}

QString Note::date_created_str() const
{
  return m_date_created.toString("MMMM d, yyyy");
}

QString Note::date_created_str_informative() const
{
  return informativeDate( m_date_created );
}

void Note::setDate_created(const QDateTime &date_created)
{
  if (!QString::compare(m_date_created.toString(), date_created.toString())) // If dates are same, exit
    return;
  m_date_created = date_created;
  emit noteChanged( this, false );
  emit noteDateCreatedChanged( this );
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
    return QString("%1 %2 ago").arg(HelperIO::numberToString(months_since, true), unit);
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
  return QString("%1 %2 ago").arg(HelperIO::numberToString(amount, true), unit);
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
  emit noteChanged( this, false );
  emit noteDateModifiedChanged( this );
}

bool Note::favorited() const
{
  return m_favorited;
}

void Note::setFavorited(bool favorited)
{
  if (m_favorited == favorited)
    return;
  m_favorited = favorited;
  emit noteChanged( this, false );
  emit noteFavoritedChanged( this );
}

int Note::notebook() const
{
  return m_notebook;
}

void Note::setNotebook(int id, bool updateDateModified)
{
  if (m_notebook == id)
    return;
  m_notebook = id;
  emit noteChanged( this, updateDateModified );
  emit noteNotebookChanged( this );
}

QVector<int> Note::tags() const
{
  return m_tags;
}

void Note::setTags(const QVector<int> &tags)
{
  m_tags = tags;
  emit noteChanged( this );
  emit noteTagsChanged( this );
}

void Note::handleNoteChange(Note *note, bool updateDateModified)
{
  if ( updateDateModified )
    note->setDate_modified( QDateTime::currentDateTime() );
}

QString Note::informativeDate(QDateTime date) const
{
  QString dateStr = date.toString("MMMM d, yyyy");
  QString timeStr = date.toString("h:mA t");
  return QString("%1 at %2").arg( dateStr, timeStr );
}

bool compareTwoDateTimes(QDateTime t1, QDateTime t2, char comparisonSymbol) {
  qint64 d1 = t1.toMSecsSinceEpoch();
  qint64 d2 = t2.toMSecsSinceEpoch();
  if ( comparisonSymbol == '>' && d1 > d2 ) {
    return true;
  } else if ( comparisonSymbol == '<' && d1 < d2 )
    return true;
  return false;
}

bool Note::byDateCreatedAsc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->date_created(), n2->date_created(), '<' );
}

bool Note::byDateCreatedDesc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->date_created(), n2->date_created(), '>' );
}

bool Note::byDateModifiedAsc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->date_modified(), n2->date_modified(), '<' );
}

bool Note::byDateModifiedDesc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->date_modified(), n2->date_modified(), '>' );
}
