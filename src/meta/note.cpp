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

Note::Note(VibratoObjectMap fields) :
    VibratoObject(fields)
{
    assignFieldsExplicitly(fields);
}

QString Note::mimeType() const
{
    return m_mimetype;
}

void Note::setMIMEType(QString mimetype)
{
    if ( m_mimetype == mimetype)
        return;

    setMIMETypeExplicitly(mimetype);

    emit mimeTypeChanged(this);
    emit changed(this);
}

void Note::setMIMETypeExplicitly(QString mimetype)
{
    m_mimetype = mimetype;
}

QString Note::encoding() const
{
    return m_encoding;
}

void Note::setEncoding(QString encoding)
{
    if ( m_encoding == encoding )
        return;

    setEncodingExplicitly(encoding);

    emit encodingChanged(this);
    emit changed(this);
}

void Note::setEncodingExplicitly(QString encoding)
{
    m_encoding = encoding;
}

QString Note::excerpt() const
{
    return m_excerpt;
}

void Note::setExcerptExplicitly(QString excerpt)
{
    m_excerpt = excerpt;
}

void Note::setSQLManager(SQLManager *sql_manager)
{
    m_sql_manager = sql_manager;
}

SQLManager *Note::sqlManager() const
{
    return m_sql_manager;
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

void Note::assignFieldsExplicitly(QMap<QString, QVariant> fields)
{
    VibratoObject::assignFieldsExplicitly(fields);

    this->setMIMETypeExplicitly(
                fields.contains("mimetype") ?
                    fields.value("mimetype").toString() : defaultMIMEType());

    this->setEncodingExplicitly(
                fields.contains("encoding") ?
                    fields.value("encoding").toString() : defaultEncoding());

    this->setExcerptExplicitly(
                fields.contains("excerpt") ?
                    fields.value("excerpt").toString() : defaultExcerpt());

    this->setNotebookExplicitly(
                fields.contains("notebook") ?
                    fields.value("notebook").toUuid() : defaultNotebook());

    this->setTagsExplicitly(
                fields.contains("tags") ?
                    fields.value("tags").value<QVector<QUuid>>() : defaultTags());
}

VibratoObjectMap Note::fields() const
{
    VibratoObjectMap fields = VibratoObject::fields();
    fields["mimetype"] = mimeType();
    fields["encoding"] = encoding();
    fields["excerpt"] = excerpt();
    fields["notebook"] = notebook();
    fields["tags"] = QVariant::fromValue(tags());
    return fields;
}

QVector<QString> Note::defaultFieldKeys() const
{
    return NOTE_DEFAULT_FIELDS;
}

QString Note::defaultTitle() const
{
    return NOTE_DEFAULT_TITLE;
}

QString Note::defaultMIMEType() const
{
    return NOTE_DEFAULT_MIMETYPE;
}

QString Note::defaultEncoding() const
{
    return NOTE_DEFAULT_ENCODING;
}

QString Note::defaultExcerpt() const
{
    return NOTE_DEFAULT_EXCERPT;
}

QUuid Note::defaultNotebook() const
{
    return NOTE_DEFAULT_NOTEBOOK;
}

QVector<QUuid> Note::defaultTags() const
{
    return NOTE_DEFAULT_TAGS;
}

QByteArray Note::data() const
{
    if ( m_sql_manager == nullptr )
        return "Vibrato SQL Error: Note object is not connected to a local SQLite3 database.";
    return m_sql_manager->fetchNoteData(this->uuid());
}

void Note::setData(const QByteArray data)
{
    if ( m_sql_manager == nullptr ) {
        qWarning() << NOTE_ERROR_NOT_CONNECTED_TO_SQLITE;
        return;
    }
    // TODO: Set note data
}
