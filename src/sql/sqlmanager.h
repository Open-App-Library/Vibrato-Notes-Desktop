#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QFile>
#include "../meta/note.h"

// A 2d array.
typedef QMap<QString, QVariant> Map;
typedef QVector<Map>            MapVector;
typedef QVector<QVariant>       VariantList;

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
  VariantList column(QString query, int column=0);

  // Returns the row/s of a SQL query.
  Map row(QSqlQuery query, QStringList tableLabels);
  Map row(QString queryString, QStringList tableLabels);
  MapVector rows(QSqlQuery query, QStringList tableLabels);
  MapVector rows(QString queryString, QStringList tableLabels);

  bool runScript(QString fileName);
  bool runScript(QFile *file, QSqlQuery *query);

  // Log SQL error to console. Returns false if error.
  bool logSqlError(QSqlError error, bool fatal=false);

  /*
   * Vibrato-specific SQL functions
   */
  // List of supported note, notebook, and tag columns (in SQL)
  QStringList noteColumns() const;
  QStringList notebookColumns() const;
  QStringList tagColumns() const;

  // Retrieve notes
  QVector<Note*> notes();
  QVector<Notebook*> notebooks();
  QVector<Tag*> tags();

  bool addNote(Note *note, bool getNewID=true);
  bool updateNoteToDB(Note *note);
  bool updateNoteFromDB(Note *note);

  bool addNotebook(Notebook *notebook, bool getNewID=true);
  bool updateNotebookToDB(Notebook *notebook);
  bool updateNotebookFromDB(Notebook *notebook);

  bool addTag(Tag *tag, bool getNewID=true);
  bool updateTagToDB(Tag *tag);
  bool updateTagFromDB(Tag *tag);

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

  QVector<Notebook*> m_getNotebooks(Notebook *parent=nullptr);

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
  QStringList m_notebookColumns =
    {"sync_id",
     "id",
     "title",
     "parent"
    };
  QStringList m_tagColumns =
    {"sync_id",
     "id",
     "title"
    };
};

#endif // SQLMANAGER_H
