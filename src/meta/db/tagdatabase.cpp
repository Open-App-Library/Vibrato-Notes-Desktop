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

void TagDatabase::addTag(Tag *tag)
{
	m_list.append(tag);
	connect(tag, &Tag::tagChanged,
					this, &TagDatabase::tagChanged_slot);
	emit tagAdded(tag);
}

Tag *TagDatabase::addTag(int id, QString title)
{
	Tag *tag = new Tag(id, title);
	addTag(tag);
	return tag;
}

void TagDatabase::removeTag(int index)
{
	Tag *tag = m_list[index];
	disconnect(tag, &Tag::tagChanged,
					this, &TagDatabase::tagChanged_slot);
	delete tag;
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
