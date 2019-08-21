#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include <QUuid>
#include "notebook.h"
#include "tag.h"

#define NOTE_DEFAULT_TITLE "Untitled Note"
#define NOTE_DEFAULT_MIMETYPE "text/markdown"
#define NOTE_DEFAULT_ENCODING "UTF-8"

class Note : public QObject
{
  Q_OBJECT

public:
  Note(QUuid id = QUuid::createUuid(),
       QString mimetype = NOTE_DEFAULT_MIMETYPE,
       QString encoding = NOTE_DEFAULT_ENCODING);

  QUuid id() const;
  void setID(QUuid id);

  QString mimeType() const;
  void setMIMEType(QString mimetype);

  QString encoding() const;
  void setEncoding(QString encoding);

  /*
   * Cached Meta Information
   */
  QString title() const;
  void    setTitle(const QString title);

  QString excerpt() const;
  void setExcerpt(QString excerpt);

  // Date Created
  QDateTime dateCreated() const;
  QString dateCreatedStr() const; // ex. January 26, 1965
  QString dateCreatedStrInformative() const; // ex. January 26, 1965 at 12:30pm EST
  void setDateCreated(const QDateTime &dateCreated);

  // Date Modified
  QDateTime dateModified() const;
  QString dateModifiedStr(); // ex. 5 minutes ago
  QString dateModifiedStrInformative(); // ex. January 26, 1965 at 12:30pm EST
  void setDateModified(const QDateTime &dateModified);

  // Notebook
  QUuid notebook() const;
  void setNotebook(QUuid sync_hash, bool updateDateModified=true);

  // Tags
  QVector<QUuid> tags() const;
  void setTags(const QVector<QUuid> &value);

  // Favorited
  bool favorited() const;
  void setFavorited(bool favorited);

  // Encrypted
  bool encrypted() const;
  void setEncrypted(bool encrypted);

  // Trashed
  bool trashed() const;
  void setTrashed(bool trashed);

  /*
   * Dynamic Information, Fetched from SQLite3
   */
  QByteArray data() const;
  void    setData(const QByteArray data);

  /*
   * Methods
   */
  void updateCachedMetaInfo();

  /*
   * Sorting comparison functions for your convenience.
   */
  static bool byDateCreatedAsc(const Note *n1, const Note *n2);
  static bool byDateCreatedDesc(const Note *n1, const Note *n2);
  static bool byDateModifiedAsc(const Note *n1, const Note *n2);
  static bool byDateModifiedDesc(const Note *n1, const Note *n2);

signals:
  void changed(Note *note, bool updateDateModified=true);

  void idChanged(Note *note);
  void titleChanged(Note *note);
  void dataChanged(Note *note);
  void dateCreatedChanged(Note *note);
  void dateModifiedChanged(Note *note);
  void notebookChanged(Note *note);
  void tagsChanged(Note *note);
  void favoritedChanged(Note *note);
  void encryptedChanged(Note *note);
  void trashedChanged(Note *note);

private slots:
  void handleChange(Note *note, bool updateDateModified=true);

private:
  QUuid          m_id;
  QString        m_mime_type;
  QString        m_encoding;
  QString        m_title;
  QByteArray     m_data;
  QDateTime      m_date_created;
  QDateTime      m_date_modified;
  QUuid          m_notebook;
  QVector<QUuid> m_tags;
  bool           m_favorited;
  bool           m_encrypted;
  bool           m_trashed=false;

  QString informativeDate(QDateTime date) const;
};

#endif // NOTE_H
