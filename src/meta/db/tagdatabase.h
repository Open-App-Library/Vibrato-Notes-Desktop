#ifndef TAGDATABASE_H
#define TAGDATABASE_H
#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include "../tag.h"
#include "../../sql/sqlmanager.h"

class TagDatabase : public QObject
{
  Q_OBJECT
public:
  TagDatabase(SQLManager *sqlManager);
  QVector<Tag*> list() const;
  int getUniqueTagID(int start, QVector<Tag*> tagList, Tag *tagToSync=nullptr);
  int getUniqueTagID(Tag *tagToSync=nullptr);
  int           size() const;

  void addTag(Tag *tag);
  // These addTag functions with a 'title' argument will first check if there is already
  // an existing tag object availible with the same string as 'title'. (case-insensitive,
  // leading and trailing whitespace removed.)
  Tag *addTag(QString title);
  Tag *addTag(int id, QString title);

  void removeTag(int index);
  void removeTag(Tag *tag);
  void clearTags();

  Tag *findTagWithID(int id);
  Tag *findTagWithName(QString name);
  // *DEPRECATED*
  // Tag *findTagWithNameOrCreate(QString name);

  void loadSQL();

  void loadJSON(QJsonDocument jsonDocument);
  void loadDummyTags();

private slots:
  void tagChanged_slot(Tag *tag);

signals:
  void tagAdded(Tag *tag);
  void tagRemoved(int tagID);
  void tagChanged(Tag *tag);

private:
  SQLManager *m_sqlManager;
  QVector<Tag*> m_list;
};

#endif // TAGDATABASE_H
