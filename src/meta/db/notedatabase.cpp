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

Note *NoteDatabase::addDefaultNote(Note *note)
{
    note->setTitle("Untitleffd Note");
	note->setText("");
    note->setDate_created( QDateTime::currentDateTime() );
    note->setDate_modified( QDateTime::currentDateTime() );
    note->setId( m_list.size()+1 );
    note->setNotebook(-1); // Default Notebook
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
		Note *note = new Note();

        note->setId( get(val, "id").toInt() );
        note->setNotebook( get(val, "notebook").toInt() );
        note->setTitle( get(val, "title").toString() );
        note->setText( get(val, "text").toString() );
        note->setDate_created( QDateTime::fromString(get(val, "date_created").toString(), Qt::ISODate) );
        note->setDate_modified( QDateTime::fromString(get(val, "date_modified").toString(), Qt::ISODate) );

        // Setting Tags
		QJsonArray raw_tag_array = val["tags"].toArray();
        QList<int>     tag_array = {};
		for (int i = 0; i < raw_tag_array.size(); i++) {
			int tag_id = raw_tag_array[i].toInt();
			tag_array.append(tag_id);
		}
		note->setTags(tag_array);

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
