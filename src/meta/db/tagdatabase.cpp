#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "tagdatabase.h"
#include <helper-io.hpp>

TagDatabase::TagDatabase(SQLManager *sqlManager) :
  m_sqlManager(sqlManager)
{
  loadSQL();
}

QVector<Tag *> TagDatabase::list() const
{
  return m_list;
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
  connect(tag, &Tag::changed,
          this, &TagDatabase::changed_slot);

  emit added(tag);
}

Tag *TagDatabase::addTag(QString title)
{
  // Clean tag & make sure not empty string
  title = title.trimmed();
  if ( title.isEmpty() ) return nullptr;

  // Check if tag exists. Return if so.
  Tag *existingTag = findTagWithName(title);
  if ( existingTag ) return existingTag;

  Tag *tag = new Tag();
  tag->setTitle(title);
  m_sqlManager->addTag(tag);
  addTag(tag);

  return tag;
}

void TagDatabase::removeTag(int index)
{
  Tag *tag = m_list[index];
  QUuid sync_hash = tag->syncHash();

  m_sqlManager->deleteTag(tag);
  delete tag;

  m_list.removeAt(index);
  emit removed(sync_hash);
}

void TagDatabase::removeTag(Tag *tag)
{
  for ( int i=m_list.length()-1; i>=0; i--)
    if ( m_list.at(i) == tag ) {
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

Tag *TagDatabase::findTagWithSyncHash(QUuid syncHash)
{
  for (Tag *tag : m_list) {
    if (tag->syncHash() == syncHash)
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

void TagDatabase::loadSQL()
{
  QVector<Tag*> tags = m_sqlManager->tags();
  for (Tag *tag : tags)
    addTag(tag);
}

void TagDatabase::changed_slot(Tag *tag)
{
  m_sqlManager->updateTagToDB(tag);
  emit changed(tag);
}
