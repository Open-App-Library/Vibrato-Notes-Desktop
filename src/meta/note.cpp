#include "note.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>
#include <helper-io.hpp>

Note::Note(QUuid sync_hash, QString title, QString text, QDateTime date_created, QDateTime date_modified, QUuid notebook, QVector<QUuid> tags, bool favorited, bool encrypted, bool trashed) :
  m_sync_hash(sync_hash),
  m_title(title),
  m_text(text),
  m_date_created(date_created),
  m_date_modified(date_modified),
  m_notebook(notebook),
  m_tags(tags),
  m_favorited(favorited),
  m_encrypted(encrypted),
  m_trashed(trashed)
{
  connect(this, &Note::changed,
          this, &Note::handleChange);
}

QUuid Note::syncHash() const
{
  return m_sync_hash;
}

void Note::setSyncHash(QUuid sync_hash)
{
  if (m_sync_hash == sync_hash)
    return;
  m_sync_hash = sync_hash;
  emit changed( this, false );
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
  emit changed( this );
  emit titleChanged( this );
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
  emit changed( this );
  emit textChanged( this );
}

QDateTime Note::dateCreated() const
{
  return m_date_created;
}

QString Note::dateCreatedStr() const
{
  return m_date_created.toString("MMMM d, yyyy");
}

QString Note::dateCreatedStrInformative() const
{
  return informativeDate( m_date_created );
}

void Note::setDateCreated(const QDateTime &date_created)
{
  if (!QString::compare(m_date_created.toString(), date_created.toString())) // If dates are same, exit
    return;
  m_date_created = date_created;
  emit changed( this, false );
  emit dateCreatedChanged( this );
}


QDateTime Note::dateModified() const
{
  return m_date_modified;
}

QString Note::dateModifiedStr()
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

QString Note::dateModifiedStrInformative()
{
  return informativeDate( m_date_modified );
}

void Note::setDateModified(const QDateTime &date_modified)
{
  if (!QString::compare(m_date_modified.toString(), date_modified.toString())) // If dates are same, exit
    return;
  m_date_modified = date_modified;
  emit changed( this, false );
  emit dateModifiedChanged( this );
}

QUuid Note::notebook() const
{
  return m_notebook;
}

void Note::setNotebook(QUuid sync_hash, bool updateDateModified)
{
  if (m_notebook == sync_hash)
    return;
  m_notebook = sync_hash;
  emit changed( this, updateDateModified );
  emit notebookChanged( this );
}

QVector<QUuid> Note::tags() const
{
  return m_tags;
}

void Note::setTags(const QVector<QUuid> &tags)
{
  m_tags = tags;
  emit changed( this );
  emit tagsChanged( this );
}

bool Note::isFavorited() const
{
  return m_favorited;
}

void Note::setFavorited(bool favorited)
{
  if (m_favorited == favorited)
    return;
  m_favorited = favorited;
  emit changed( this, false );
  emit favoritedChanged( this );
}

bool Note::isEncrypted() const
{
  return m_encrypted;
}

void Note::setEncrypted(bool encrypted)
{
  if (encrypted == m_encrypted)
    return;
  m_encrypted = encrypted;
  emit changed(this, true);
  emit encryptedChanged(this);
}

bool Note::isTrashed() const {
  return m_trashed;
}

void Note::setTrashed(bool _trashed) {
  if (_trashed == m_trashed)
    return;
  m_trashed = _trashed;
  emit changed(this, false);
  emit trashedOrRestored(this, _trashed);
  if ( _trashed ) emit trashed(this);
  else           emit restored(this);
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
  return compareTwoDateTimes( n1->dateCreated(), n2->dateCreated(), '<' );
}

bool Note::byDateCreatedDesc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->dateCreated(), n2->dateCreated(), '>' );
}

bool Note::byDateModifiedAsc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->dateModified(), n2->dateModified(), '<' );
}

bool Note::byDateModifiedDesc(const Note *n1, const Note *n2)
{
  return compareTwoDateTimes( n1->dateModified(), n2->dateModified(), '>' );
}

void Note::handleChange(Note *note, bool updateDateModified)
{
  if ( updateDateModified )
    note->setDateModified( QDateTime::currentDateTime() );
}
