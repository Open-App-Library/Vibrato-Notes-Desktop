#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include "notebook.h"
#include "tag.h"

#define NOTE_DEFAULT_TITLE "Untitled Note"

class Note : public QObject
{
  Q_OBJECT

public:
  Note(int id, QString title, QString text, QDateTime date_created, QDateTime date_modified, bool favorited, int notebook, QVector<int> tags, bool trashed=false);

  int id() const;
  void setId(int value);

  QString title() const;
  void    setTitle(const QString title);

  QString text() const;
  void    setText(const QString text);

  QDateTime date_created() const;
  QString date_created_str() const; // ex. January 26, 1965
  QString date_created_str_informative() const; // ex. January 26, 1965 at 12:30pm EST
  void setDate_created(const QDateTime &date_created);

  QDateTime date_modified() const;
  QString date_modified_str(); // ex. 5 minutes ago
  QString date_modified_str_informative(); // ex. January 26, 1965 at 12:30pm EST
  void setDate_modified(const QDateTime &date_modified);

  bool  favorited() const;
  void setFavorited(bool favorited);

  int  notebook() const;
  void setNotebook(int id, bool updateDateModified=true);

  QVector<int> tags() const;
  void setTags(const QVector<int> &value);

  bool trashed() const;
  void setTrashed(bool trashed);

  // Sorting comparison functions for your convenience.
  static bool byDateCreatedAsc(const Note *n1, const Note *n2);
  static bool byDateCreatedDesc(const Note *n1, const Note *n2);
  static bool byDateModifiedAsc(const Note *n1, const Note *n2);
  static bool byDateModifiedDesc(const Note *n1, const Note *n2);

private slots:
  void handleNoteChange(Note *note, bool updateDateModified=true);

signals:
  void noteChanged(Note *note, bool updateDateModified=true);
  void noteIdChanged(Note *note);
  void noteTitleChanged(Note *note);
  void noteTextChanged(Note *note);
  void noteDateCreatedChanged(Note *note);
  void noteDateModifiedChanged(Note *note);
  void noteFavoritedChanged(Note *note);
  void noteNotebookChanged(Note *note);
  void noteTagsChanged(Note *note);
  void noteTrashedOrRestored(Note *note, bool trashed);
  void noteTrashed(Note *note);
  void noteRestored(Note *note);

private:
  int          m_id;
  QString      m_title;
  QString      m_text;
  QDateTime    m_date_created;
  QDateTime    m_date_modified;
  bool         m_favorited;
  int          m_notebook;
  QVector<int> m_tags;
  bool         m_trashed=false;

  QString informativeDate(QDateTime date) const;

  // TODO: Implement user (shows ID of note owner)
  // TODO: Implement shared_with
  // TODO: Implement is_public
};

#endif // NOTE_H
