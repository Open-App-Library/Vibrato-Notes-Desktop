#include "note.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>
#include <helper-io.hpp>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "../sql/sqlmanager.h"

Note::Note(SQLManager *sql_manager,
           VibratoObjectMap fields) :
    VibratoObject(fields),
    m_sql_manager(sql_manager)
{

}

QByteArray Note::data()
{
    QByteArray data = dataExplict();
    // TODO: Handle based on encoding.
    setExcerptExplicitly( QString::fromUtf8(data).left(NOTE_DEFAULT_EXCERPT_LENGTH) );
    return data;
}

QByteArray Note::dataExplict() const
{
    return m_sql_manager->fetchNoteData(this->uuid());
}

void Note::setData(const QByteArray data)
{

}

QString Note::excerpt() const
{
    return m_excerpt;
}

void Note::setExcerptExplicitly(QString excerpt)
{
    m_excerpt = excerpt;
}

QUuid Note::notebook() const
{
  return m_notebook;
}

void Note::setNotebook(QUuid uuid)
{
  if (m_notebook == uuid)
    return;
  setNotebookExplicitly(uuid);
  emit changed( this );
  emit notebookChanged( this );
}

void Note::setNotebookExplicitly(QUuid uuid)
{
    m_notebook = uuid;
}

QVector<QUuid> Note::tags() const
{
    return m_tags;
}

QStringList Note::tagsStringList() const
{
    QStringList list;
    for ( int i =0; i < m_tags.length(); i++ )
        list.append( m_tags.at(i).toString(QUuid::WithoutBraces) );
    return list;
}

void Note::setTags(const QVector<QUuid> tags)
{
    setTagsExplicitly(tags);
    emit changed( this );
    emit tagsChanged( this );
}

void Note::setTagsExplicitly(const QVector<QUuid> tags)
{
    m_tags = tags;
}

bool Note::favorited() const
{
  return m_favorited;
}

void Note::setFavorited(bool favorited)
{
  if (m_favorited == favorited)
    return;
  setFavoritedExplicitly(favorited);
  emit favoritedChanged( this );
}

void Note::setFavoritedExplicitly(bool favorited)
{
    m_favorited = favorited;
}

bool Note::trashed() const {
  return m_trashed;
}

void Note::setTrashed(bool trashed) {
  if (trashed == m_trashed)
    return;
  setTrashedExplicitly(trashed);
  emit changed(this);
  emit trashedChanged(this);
}

void Note::setTrashedExplicitly(bool trashed)
{
    m_trashed = trashed;
}

QVector<QString> Note::defaultFieldKeys() const
{
    return NOTE_DEFAULT_FIELDS;
}

QString Note::defaultTitle() const
{
    return NOTE_DEFAULT_TITLE;
}
