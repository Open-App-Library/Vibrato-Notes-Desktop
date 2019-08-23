#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include <QUuid>
#include "vibratoobject.h"
#include "notebook.h"
#include "tag.h"

class SQLManager;

#define NOTE_DEFAULT_TITLE "Untitled Note"

#define NOTE_DEFAULT_FIELDS VIBRATOOBJECT_DEFAULT_FIELDS + \
    QVector<QString>({"mimetype", "encoding", "excerpt", "notebook", "tags"})

#define NOTE_DEFAULT_MIMETYPE "text/markdown"
#define NOTE_DEFAULT_ENCODING "UTF-8"
#define NOTE_DEFAULT_EXCERPT "..."
#define NOTE_DEFAULT_EXCERPT_LENGTH 26
#define NOTE_DEFAULT_NOTEBOOK nullptr
#define NOTE_DEFAULT_TAGS {}
#define NOTE_ERROR_NOT_CONNECTED_TO_SQLITE "Vibrato SQL Error: Note object is not connected to a local SQLite3 database."

class Note : public VibratoObject
{
  Q_OBJECT

public:
  Note(VibratoObjectMap fields = VibratoObjectMap());

  QString mimeType() const;
  void setMIMEType(QString mimetype);
  void setMIMETypeExplicitly(QString mimetype);

  QString encoding() const;
  void setEncoding(QString encoding);
  void setEncodingExplicitly(QString encoding);

  QUuid notebook() const;
  void setNotebook(QUuid uuid);
  void setNotebookExplicitly(QUuid uuid);

  QVector<QUuid> tags() const;
  QStringList tagsStringList() const;
  void setTags(const QVector<QUuid> tags);
  void setTagsExplicitly(const QVector<QUuid> tags);

  bool favorited() const;
  void setFavorited(bool favorited);
  void setFavoritedExplicitly(bool favorited);

  bool trashed() const;
  void setTrashed(bool trashed);
  void setTrashedExplicitly(bool trashed);

  void assignFieldsExplicitly(QMap<QString, QVariant> fields) override;
  VibratoObjectMap fields() const override;

  QVector<QString> defaultFieldKeys() const override;
  QString          defaultTitle() const override;
  QString          defaultMIMEType() const;
  QString          defaultEncoding() const;
  QString          defaultExcerpt() const;
  QUuid            defaultNotebook() const;
  QVector<QUuid>   defaultTags() const;

  /*
   * The content of the note will be fetched
   * from the database. This will save memory
   * for those who have thousands of notes.
   * The excerpt is the only thing that is
   * stored in memory.
   */
  QByteArray data() const;
  void    setData(const QByteArray data);

  QString excerpt() const;
  void setExcerptExplicitly(QString excerpt);

  void setSQLManager(SQLManager *sql_manager);
  SQLManager *sqlManager() const;


signals:
  void mimeTypeChanged(Note *note);
  void encodingChanged(Note *note);
  void excerptChanged(Note *note);
  void dataChanged(Note *note);
  void notebookChanged(Note *note);
  void tagsChanged(Note *note);
  void favoritedChanged(Note *note);
  void trashedChanged(Note *note);

private:
  QString        m_mimetype;
  QString        m_encoding;
  QString        m_excerpt;
  QUuid          m_notebook;
  QVector<QUuid> m_tags;
  bool           m_favorited;
  bool           m_trashed;

  SQLManager    *m_sql_manager = nullptr;
};

#endif // NOTE_H
