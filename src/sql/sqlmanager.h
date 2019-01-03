#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QFile>
#include "../meta/note.h"

// A 2d array.
typedef QVector<QMap<QString, QVariant>> ArrayOfMaps;
typedef QMap<QString, QVariant> Map;
typedef QVector<QVariant> ObjectList;

class SQLManager : public QObject
{
  Q_OBJECT
public:
  explicit SQLManager(QObject *parent = nullptr);
  void close();

  QString location() const;

  /*
   * basicQuery and realBasicQuery are two ways to quickly execute a SQL query.
   * basicQuery executes a query, prints a message if error, and returns a QSqlQuery
   * object.
   * realBasicQuery runs basicQuery but instead of returning an QSqlQuery object, it
   * just returns a boolean indicating whether the query ran without error.
   */
  QSqlQuery basicQuery(QString query);
  bool  realBasicQuery(QString query);

  // Returns a list representing a single-column selection
  ObjectList column(QString query, int column=0);

  // Returns the row/s of a SQL query.
  Map row(QSqlQuery query, QStringList tableLabels);
  Map row(QString queryString, QStringList tableLabels);
  ArrayOfMaps rows(QSqlQuery query, QStringList tableLabels);
  ArrayOfMaps rows(QString queryString, QStringList tableLabels);

  bool runScript(QString fileName);
  bool runScript(QFile *file, QSqlQuery *query);

  // Log SQL error to console. Returns false if error.
  bool logSqlError(QSqlError error, bool fatal=false);

  /*
   * Vibrato-specific SQL functions
   */
  QStringList noteColumns() const; // List of supported note columns

  // Retrieve notes
  QVector<Note*> Notes();
  QVector<Notebook*> Notebooks();
  QVector<Tag*> tags();

  bool addNote(Note *note, bool getNewID=false);
  bool updateNoteToDB(Note *note);
  bool updateNoteFromDB(Note *note);

  bool tagExists(int noteID, int tagID);
  // If skip_duplicate_check is set to true, it will not check for a duplicate entry
  // before adding the tag to note. This will save you from an extra database call.
  bool addTagToNote(int noteID, int tagID, bool skip_duplicate_check=false);
  bool removeTagFromNote(int noteID, int tagID);

signals:

public slots:

private:
  QString m_location;
  QSqlDatabase m_sqldb;

  QStringList m_noteColumns =
    {"sync_id",
     "id",
     "title",
     "text",
     "date_created",
     "date_modified",
     "favorited",
     "notebook",
     "trashed"
    };
};

#endif // SQLMANAGER_H
