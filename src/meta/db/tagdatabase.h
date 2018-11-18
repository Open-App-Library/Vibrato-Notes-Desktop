#ifndef TAGDATABASE_H
#define TAGDATABASE_H
#include <QVector>
#include <QJsonDocument>
#include "../tag.h"

class TagDatabase
{
public:
    TagDatabase();
    QVector<Tag*> list() const;
    int           size() const;

    void addTag(Tag *tag);
    Tag *addTag(int id, QString title);

    void removeTag(int index);
    void clearTags();

    Tag *findTagWithID(int id);
    Tag *findTagWithName(QString name);
    Tag *findTagWithNameOrCreate(QString name);

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyTags();

private:
    QVector<Tag*> m_list;

};

#endif // TAGDATABASE_H
