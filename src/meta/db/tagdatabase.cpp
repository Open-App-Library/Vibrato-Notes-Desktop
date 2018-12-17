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

int TagDatabase::getUniqueTagID(int start, QVector<Tag*> tagList, Tag *tagToSync)
 {
  // Loop through tag list. If ID == start (The current index we're testing)
  // that means that the ID is NOT unique and we must find a new one. Increment
  // by one!
  for ( Tag* t : tagList )
    if ( t->id() == start ) // Can't use this ID; it's taken.
      return getUniqueTagID(start+1, tagList);

  // If we made it this far in the function, it means that the ID we are testing
  // is indeed unique. Return that bad boy.
  return start;
}

int TagDatabase::getUniqueTagID(Tag *tagToSync)
{
  // Search for a unique tag ID.
  // If you have tag ID #1 and create a new tag, the next ID will
  // most likely be two. For this reason, we set the start index to search
  // for a new ID to the list length + 1 as it is likely unused.
  //   Tip: If the *tagToSync parameter is passed, and the user has
  //        connected the cloud. It will attempt to sync to the cloud first.
  QVector<Tag*> tagList = list();
  return getUniqueTagID(tagList.length()+1, tagList, tagToSync);
}

int TagDatabase::size() const
{
  return m_list.size();
}

void TagDatabase::addTag(Tag *tag)
{
  // Don't allow duplicate tags in our tag database
  for ( Tag *t : m_list ) {
    if ( t->title().toLower() == tag->title().toLower() )
      return;
  }

  m_list.append(tag);
  connect(tag, &Tag::tagChanged,
          this, &TagDatabase::tagChanged_slot);
  emit tagAdded(tag);
}

Tag *TagDatabase::addTag(QString title)
{
  return addTag(getUniqueTagID(), title);
}

Tag *TagDatabase::addTag(int id, QString title)
{
  QString titleCleaned = title.trimmed();
  if ( title.isEmpty() )
    return nullptr;

  Tag *existingTag = findTagWithName(titleCleaned);

  if ( existingTag )
    return existingTag;
  Tag *tag = existingTag ? existingTag : new Tag(id, title);

  addTag(tag);
  return tag;
}

void TagDatabase::removeTag(int index)
{
  Tag *tag = m_list[index];
  int id = tag->id();
  disconnect(tag, &Tag::tagChanged,
             this, &TagDatabase::tagChanged_slot);
  delete tag;
  m_list.removeAt(index);
  emit tagRemoved(id);
}

void TagDatabase::removeTag(Tag *tag)
{
  qDebug() << "Attempting to remove tag" << tag->title();
  for ( int i=m_list.length()-1; i>=0; i--)
    if ( m_list.at(i) == tag ) {
      qDebug() << "Found tag at" << i;
      removeTag(i);
      break;
    }
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
  for (Tag *tag : m_list)
    if ( QString::compare(tag->title(), name, Qt::CaseInsensitive) == 0 )
      return tag;
  return nullptr;
}

// Tag *TagDatabase::findTagWithNameOrCreate(QString name)
// {
//   Tag *tag = findTagWithName( name );
//   if (tag != nullptr)
//     return tag;
//   else
//     return addTag(name);
// }

void TagDatabase::loadJSON(QJsonDocument jsonDocument)
{
  QJsonArray tagArray = jsonDocument.array();
  for (int i = 0; i < tagArray.size(); i++) {
    QJsonObject curTag = tagArray[i].toObject();
    int tag_id = curTag.value("id").toInt();
    QString tag_title = curTag.value("title").toString();
    Tag *t = new Tag(tag_id, tag_title);
    addTag(t);
  }
}

void TagDatabase::loadDummyTags()
{
  clearTags();
  QJsonDocument Tags = HelperIO::fileToQJsonDocument(":/dummy/tags.json");
  loadJSON(Tags);
}

void TagDatabase::tagChanged_slot(Tag *tag)
{
  emit tagChanged(tag);
}
