#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QFile>
#include <QUuid>

class Note;
class Notebook;
class Tag;

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

  Note *fetchNote(QUuid uuid);

  Notebook *fetchNotebook(QUuid uuid);
  bool addNotebook(Notebook *notebook);
  bool updateNotebookToDB(Notebook *notebook);
  bool updateNotebookFromDB(Notebook *notebook);
  bool deleteNotebook(Notebook *notebook, bool delete_children=true);

  Tag *fetchTag(QUuid uuid);
  bool addTag(Tag *tag);
  bool updateTagToDB(Tag *tag);
  bool updateTagFromDB(Tag *tag);
  bool deleteTag(Tag *tag);

  bool tagExists(QUuid noteSyncHash, QUuid tagSyncHash);
  // If skip_duplicate_check is set to true, it will not check for a duplicate entry
  // before adding the tag to note. This will save you from an extra database call.
  bool addTagToNote(QUuid noteSyncHash, QUuid tagSyncHash, bool skip_duplicate_check=false);
  bool removeTagFromNote(QUuid noteSyncHash, QUuid tagSyncHash);

  void importTutorialNotes();

signals:

public slots:

private:
  QString m_location;
  QSqlDatabase m_sqldb;

  bool m_shouldImportTutorialNotes = false;

  QVector<Notebook*> m_getNotebooks(Notebook *parent=nullptr);

  QStringList m_noteColumns =
    {"uuid",
     "mimetype",
     "encoding",
     "title",
     "text",
     "date_created",
     "date_modified",
     "notebook",
     "favorited",
     "encrypted",
     "trashed"
    };
  QStringList m_notebookColumns =
    {"uuid",
     "title",
     "date_modified",
     "parent",
     "row",
     "encrypted"
    };
  QStringList m_tagColumns =
    {"uuid",
     "title",
     "date_modified",
     "row",
     "encrypted"
    };
};

#endif // SQLMANAGER_H
