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
  QVector<QVariant> column(QString query, int column=0);

  // Returns the rows of a SQL query.
  ArrayOfMaps rows(QString query, QStringList tableLabels);

  bool runScript(QString fileName);
  bool runScript(QFile *file, QSqlQuery *query);

  // Log SQL error to console.
  void logSqlError(QSqlError error, bool fatal=false);

  /*
   * Vibrato-specific SQL functions
   */
  QStringList noteColumns() const; // List of supported note columns

  // Retrieve notes
  QVector<Note*> Notes();
  QVector<Note*> Notebooks();
  QVector<Note*> tags();

  void addNoteToDatabase(Note *note, bool getNewID=false);

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
