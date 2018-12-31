#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QObject>
#include <QSqlDatabase>

class SQLManager : public QObject
{
  Q_OBJECT
public:
  explicit SQLManager(QObject *parent = nullptr);
  void close();

  QString location() const;

signals:

public slots:

private:
  QString m_location;
  QSqlDatabase m_sqldb;
};

#endif // SQLMANAGER_H
