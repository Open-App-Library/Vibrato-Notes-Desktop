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
#define NOTE_DEFAULT_FIELDS {"uuid", "title", "date_created", "date_modified", "encrypted", \
                             "mimetype", "encoding", "data", "notebook", "tags"}
#define NOTE_DEFAULT_MIMETYPE "text/markdown"
#define NOTE_DEFAULT_ENCODING "UTF-8"
#define NOTE_DEFAULT_EXCERPT_LENGTH 26

class Note : public VibratoObject
{
  Q_OBJECT

public:
  Note(SQLManager *sql_manager,
       VibratoObjectMap fields = VibratoObjectMap());

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

  /*
   * Default Data
   */
  QVector<QString> defaultFields() const;
  QString          defaultTitle() const;

  /*
   * The content of the note will be fetched
   * from the database. This will save memory
   * for those who have thousands of notes.
   * The excerpt is the only thing that is
   * stored in memory.
   */
  QByteArray data();
  QByteArray dataExplict() const;
  void    setData(const QByteArray data);

  QString excerpt() const;
  void setExcerptExplicitly(QString excerpt);


signals:
  void dataChanged(Note *note);
  void notebookChanged(Note *note);
  void tagsChanged(Note *note);
  void favoritedChanged(Note *note);
  void trashedChanged(Note *note);

private:
  SQLManager    *m_sql_manager;
  QString        m_mime_type;
  QString        m_encoding;
  QString        m_excerpt;
  QUuid          m_notebook;
  QVector<QUuid> m_tags;
  bool           m_favorited;
  bool           m_trashed;
};

#endif // NOTE_H
