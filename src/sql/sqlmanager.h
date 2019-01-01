#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QFile>

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
  QVector<QVariant> column(QString query);

  // Returns the rows of a SQL query.
  QVector<QVector<QVariant>> rows(QString query, QStringList tableLabels);

  bool runScript(QString fileName);
  bool runScript(QFile *file, QSqlQuery *query);

  // Log SQL error to console.
  void logSqlError(QSqlError error, bool fatal=false);

signals:

public slots:

private:
  QString m_location;
  QSqlDatabase m_sqldb;
};

#endif // SQLMANAGER_H
