#include "tag.h"

Tag::Tag(QUuid sync_hash, QString title, QDateTime date_modified, int row, bool encrypted) :
  m_sync_hash(sync_hash),
  m_title(title),
  m_date_modified(date_modified),
  m_row(row),
  m_encrypted(encrypted)
{
  connect(this, &Tag::changed,
          this, &Tag::handleChange);
}

QUuid Tag::syncHash() const
{
  return m_sync_hash;
}

void Tag::setSyncHash(QUuid syncHash)
{
  if ( syncHash == m_sync_hash ) return;

  m_sync_hash = syncHash;
  emit syncHashChanged(this);
  emit changed(this);
}

QString Tag::title() const
{
  return m_title;
}

void Tag::setTitle(const QString title)
{
  QString cleanedTitle = title.trimmed();
  if ( cleanedTitle == m_title ||
       cleanedTitle.isEmpty() )
    return;
  m_title = cleanedTitle;
  emit titleChanged(this);
  emit changed(this);
}

QDateTime Tag::dateModified() const
{
  return m_date_modified;
}

void Tag::setDateModified(QDateTime dateModified, bool emitChangeSignal)
{
  if ( dateModified == m_date_modified ) return;

  m_date_modified = dateModified;
  emit dateModifiedChanged(this);
  if (emitChangeSignal)
    emit changed(this);
}

int Tag::row() const
{
  return m_row;
}

void Tag::setRow(int row)
{
  if ( row == m_row ) return;

  m_row = row;
  emit rowChanged(this);
  emit changed(this);
}

bool Tag::encrypted() const
{
  return m_encrypted;
}

void Tag::setEncrypred(bool encrypted)
{
  if ( encrypted == m_encrypted ) return;

  m_encrypted = encrypted;
  emit encryptedChanged(this);
  emit changed(this);
}

void Tag::handleChange(Tag *tag)
{
  tag->setDateModified( QDateTime::currentDateTime(), false );
}
