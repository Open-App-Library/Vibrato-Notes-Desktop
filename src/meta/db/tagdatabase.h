#ifndef TAGDATABASE_H
#define TAGDATABASE_H
#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include "../tag.h"

class TagDatabase : public QObject
{
	Q_OBJECT
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

private slots:
	void tagChanged_slot(Tag *tag);

signals:
	void tagAdded(Tag *tag);
	void tagChanged(Tag *tag);

private:
	QVector<Tag*> m_list;

};

#endif // TAGDATABASE_H
