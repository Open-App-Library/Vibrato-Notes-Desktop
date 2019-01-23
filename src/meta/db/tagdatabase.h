#ifndef TAGDATABASE_H
#define TAGDATABASE_H
#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QUuid>
#include "../tag.h"
#include "../../sql/sqlmanager.h"

class TagDatabase : public QObject
{
  Q_OBJECT
public:
  TagDatabase(SQLManager *sqlManager);
  QVector<Tag*> list() const;
  int           size() const;

  void addTag(Tag *tag);
  Tag *addTag(QString title);

  void removeTag(int index);
  void removeTag(Tag *tag);
  void clearTags();

  Tag *findTagWithSyncHash(QUuid syncHash);
  Tag *findTagWithName(QString name);

  void loadSQL();

private slots:
  void changed_slot(Tag *tag);

signals:
  void added(Tag *tag);
  void changed(Tag *tag);
  void removed(QUuid tagSyncHash);

private:
  SQLManager *m_sqlManager;
  QVector<Tag*> m_list;
};

#endif // TAGDATABASE_H
