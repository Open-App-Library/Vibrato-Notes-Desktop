#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notedatabase.h"

#include <helper-io.hpp>

NoteDatabase::NoteDatabase()
{

}

QList<Note *> NoteDatabase::list() const
{
	return m_list;
}

int NoteDatabase::size() const
{
	return m_list.size();
}

Note *NoteDatabase::addNote(Note *note)
{
	m_list.prepend(note);
	return note;
}

Note *NoteDatabase::addDefaultNote()
{
    Note *note = new Note(-1, "Untitled Note", "", QDateTime::currentDateTime(), QDateTime::currentDateTime(), -1, {});
    // Todo add note to cloud and get ID
    return addNote(note);
}

// This causes use of deleted function error. Probably because I started extending QObject
//Note *NoteDatabase::addNote(Note note)
//{
//	Note *newNote = new Note(note);
//	return addNote(newNote);
//}

void NoteDatabase::removeNote(int index)
{
	delete m_list[index];
	m_list.removeAt(index);
}

void NoteDatabase::removeNote(Note *note)
{
	int index = m_list.indexOf( note );
	removeNote( index );
}

void NoteDatabase::clearNotes()
{
	for (int i = m_list.size()-1; i >= 0; i--) {
		removeNote(i);
	}
}

void NoteDatabase::loadJSON(QJsonDocument jsonDocument)
{
	clearNotes(); // Clear note database for safety
	QJsonArray noteArray = jsonDocument.array();
	for (int i = 0; i < noteArray.size(); i++) {
		QJsonObject val = noteArray[i].toObject();

		int id = get(val, "id").toInt();
        QString title = get(val, "title").toString();
        QString text = get(val, "text").toString();
        QDateTime date_created = QDateTime::fromString(get(val, "date_created").toString(), Qt::ISODate);
        QDateTime date_modified = QDateTime::fromString(get(val, "date_modified").toString(), Qt::ISODate);
		int notebook = get(val, "notebook").toInt();
        QVector<int> tags = {};

		// Setting Tags
		QJsonArray raw_tag_array = val["tags"].toArray();
		for (int i = 0; i < raw_tag_array.size(); i++) {
			int tag_id = raw_tag_array[i].toInt();
            tags.append(tag_id);
		}

        Note *note = new Note(id, title, text, date_created, date_modified, notebook, tags);

		m_list.append(note);
	}
}

void NoteDatabase::loadDummyNotes()
{
	QJsonDocument dummy_notes = fileToQJsonDocument(":/dummy/notes.json");
    loadJSON(dummy_notes);
}

QJsonValue NoteDatabase::get(QJsonObject obj, QString key)
{
    if ( obj.value(key).isNull() )
        return QJsonValue();
    return obj.value(key);
}
