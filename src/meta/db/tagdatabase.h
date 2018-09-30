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

    void removeTag(int index);
    void clearTags();

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyTags();

private:
    QVector<Tag*> m_list;

};

#endif // TAGDATABASE_H
