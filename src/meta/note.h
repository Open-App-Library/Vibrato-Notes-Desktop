#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include <QUuid>
#include "notebook.h"
#include "tag.h"

#define NOTE_DEFAULT_TITLE "Untitled Note"

class Note : public QObject
{
  Q_OBJECT

public:
  Note(QUuid sync_hash = QUuid::createUuid(),
       QString title = "",
       QString text = "",
       QDateTime dateCreated = QDateTime::currentDateTime(),
       QDateTime dateModified = QDateTime::currentDateTime(),
       // TODO: user field
       QUuid notebook = QUuid(),
       QVector<QUuid> tags = {},
       bool favorited = false,
       // TODO: public field
       bool encrypted = false,
       bool trashed = false);

  // Sync Hash
  QUuid syncHash() const;
  void setSyncHash(QUuid sync_hash);

  // Title
  QString title() const;
  void    setTitle(const QString title);

  // Text
  QString text() const;
  void    setText(const QString text);

  // Date created
  QDateTime dateCreated() const;
  QString dateCreatedStr() const; // ex. January 26, 1965
  QString dateCreatedStrInformative() const; // ex. January 26, 1965 at 12:30pm EST
  void setDateCreated(const QDateTime &dateCreated);

  // Date modified
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
  bool isFavorited() const;
  void setFavorited(bool favorited);

  // Encrypted
  bool isEncrypted() const;
  void setEncrypted(bool encrypted);

  // Trashed
  bool isTrashed() const;
  void setTrashed(bool trashed);

  // Sorting comparison functions for your convenience.
  static bool byDateCreatedAsc(const Note *n1, const Note *n2);
  static bool byDateCreatedDesc(const Note *n1, const Note *n2);
  static bool byDateModifiedAsc(const Note *n1, const Note *n2);
  static bool byDateModifiedDesc(const Note *n1, const Note *n2);

signals:
  // The general signal for whenever something changes
  void changed(Note *note, bool updateDateModified=true);

  void syncHashChanged(Note *note);
  void titleChanged(Note *note);
  void textChanged(Note *note);
  void dateCreatedChanged(Note *note);
  void dateModifiedChanged(Note *note);
  void notebookChanged(Note *note);
  void tagsChanged(Note *note);
  void favoritedChanged(Note *note);
  void encryptedChanged(Note *note);
  void trashed(Note *note);
  void restored(Note *note);
  void trashedOrRestored(Note *note, bool _trashed);

private slots:
  void handleChange(Note *note, bool updateDateModified=true);

private:
  QUuid        m_sync_hash;
  QString      m_title;
  QString      m_text;
  QDateTime    m_date_created;
  QDateTime    m_date_modified;
  QUuid          m_notebook;
  QVector<QUuid> m_tags;
  // TODO: Private user variable
  bool         m_favorited;
  // TODO: Private public variable
  bool         m_encrypted;
  bool         m_trashed=false;

  QString informativeDate(QDateTime date) const;
};

#endif // NOTE_H
