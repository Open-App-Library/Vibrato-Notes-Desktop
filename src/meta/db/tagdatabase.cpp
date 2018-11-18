#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "tagdatabase.h"
#include <helper-io.hpp>

TagDatabase::TagDatabase()
{

}

QVector<Tag *> TagDatabase::list() const
{
    return m_list;
}

int TagDatabase::size() const
{
    return m_list.size();
}

void TagDatabase::addTag(Tag *Tag)
{
    m_list.append(Tag);
}

Tag *TagDatabase::addTag(int id, QString title)
{
    Tag *tag = new Tag();
    tag->setId(id);
    tag->setTitle(title);
    addTag(tag);
    return tag;
}

void TagDatabase::removeTag(int index)
{
    delete m_list[index];
    m_list.removeAt(index);
}

void TagDatabase::clearTags()
{
    for (int i = m_list.size()-1; i >= 0; i--) {
        removeTag(i);
    }
}

Tag *TagDatabase::findTagWithID(int id)
{
    for (Tag *tag : m_list) {
        if (tag->id() == id)
            return tag;
    }
    return nullptr;
}

Tag *TagDatabase::findTagWithName(QString name)
{
    for (Tag *tag : m_list) {
        if ( QString::compare(tag->title(), name, Qt::CaseInsensitive) == 0 )
            return tag;
    }
    return nullptr;
}

Tag *TagDatabase::findTagWithNameOrCreate(QString name)
{
    Tag *tag = findTagWithName( name );
    if (tag != nullptr)
        return tag;
    else
        return addTag(m_list.count()+1, name);
}

void TagDatabase::loadJSON(QJsonDocument jsonDocument)
{
    QJsonArray tagArray = jsonDocument.array();
    for (int i = 0; i < tagArray.size(); i++) {
        Tag *t = new Tag();
        QJsonObject curTag = tagArray[i].toObject();
        int tag_id = curTag.value("id").toInt();
        QString tag_title = curTag.value("title").toString();
        t->setId(tag_id);
        t->setTitle(tag_title);
        addTag(t);
    }
}

void TagDatabase::loadDummyTags()
{
    clearTags();
    QJsonDocument Tags = fileToQJsonDocument(":/dummy/tags.json");
    loadJSON(Tags);
}
