#include "note.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>
#include <helper-io.hpp>
#include <QSqlQuery>
#include <QSqlError>
#include "../sql/sqlmanager.h"

Note::Note(SQLManager *sql_manager,
           QUuid uuid,
           QString mimetype,
           QString encoding) :
    m_sql_manager(sql_manager),
    m_uuid(uuid),
    m_mime_type(mimetype),
    m_encoding(encoding)
{
    connect(this, &Note::changed,
            this, &Note::handleChange);
}

SQLManager *Note::sqlManager()
{
    return m_sql_manager;
}

void Note::setSQLManager(SQLManager *sql_manager)
{
    m_sql_manager = sql_manager;
}

QUuid Note::uuid() const
{
  return m_uuid;
}

void Note::setUUID(QUuid uuid)
{
  if (m_uuid == uuid)
    return;
  m_uuid = uuid;
  emit changed( this, false );
}

QString Note::title() const
{
  return m_title;
}

void Note::setTitle(const QString title)
{
  QString titleCleaned = title.trimmed();
  // If no changed to title or it is empty, return.
  if (m_title == titleCleaned ||
      title.isEmpty())
    return;
  m_title = titleCleaned;
  emit changed( this );
  emit titleChanged( this );
}

QByteArray Note::data() const
{
    Note foundNote(m_sql_manager);
}

void Note::setData(const QByteArray data)
{
  if (!QString::compare(m_data, data))
    return;
  m_data = data;
  emit changed( this );
  emit dataChanged( this );
}

bool Note::sql_addToDB()
{
    QStringList noteCols = m_sql_manager->noteColumns();
    QStringList columnPlaceholders;

    for (QString col : noteCols)
      columnPlaceholders.append( QString(":%1").arg(col) );

    QString queryString = QString("INSERT INTO notes (%1) "
                                  "VALUES (%2)").arg(noteCols.join(", "),
                                                     columnPlaceholders.join(", "));

    QSqlQuery q;
    q.prepare(queryString);

    q.bindValue(":uuid"          , this->uuid().toString(QUuid::WithoutBraces));
    q.bindValue(":mimetype"      , this->mimeType());
    q.bindValue(":encoding"      , this->encoding());
    q.bindValue(":title"         , this->title());
    q.bindValue(":date_created"  , this->dateCreated());
    q.bindValue(":date_modified" , this->dateModified());
    q.bindValue(":notebook"      , this->notebook());
    q.bindValue(":favorited"     , this->favorited());
    q.bindValue(":encrypted"     , this->encrypted());
    q.bindValue(":trashed"       , this->trashed());

    q.exec();

    // Set the tags
    QSqlQuery tagQ;
    for ( QUuid tagUUID : this->tags() ) {
      tagQ.prepare("insert into notes_tags (note, tag) values "
                "(:noteUuid, :tagUuid)");
      tagQ.bindValue(":noteUuid", this->uuid().toString(QUuid::WithoutBraces));
      tagQ.bindValue(":tagUuid", tagUUID);
      tagQ.exec();
      m_sql_manager->logSqlError(tagQ.lastError());
    }

    // Print error if there is one. Return true if no error.
    return m_sql_manager->logSqlError(q.lastError());

}

bool Note::sql_updateToDB()
{

}

bool Note::sql_updateFromDB()
{

}

bool Note::sql_delete()
{

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

void Note::setNotebook(QUuid uuid, bool updateDateModified)
{
  if (m_notebook == uuid)
    return;
  m_notebook = uuid;
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

bool Note::favorited() const
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

bool Note::encrypted() const
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

bool Note::trashed() const {
  return m_trashed;
}

void Note::setTrashed(bool _trashed) {
  if (_trashed == m_trashed)
    return;
  m_trashed = _trashed;
  emit changed(this, false);
  emit trashedChanged(this);
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
