#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notedatabase.h"

#include "../../helper-io.h"

NoteDatabase::NoteDatabase()
{

}

QList<Note *> NoteDatabase::list() const
{
    return m_list;
}

Note *NoteDatabase::addNote(Note *note)
{
    m_list.append(note);
    return note;
}

Note *NoteDatabase::addNote(Note note)
{
    // TODO: Test if this even works
    Note *newNote = new Note(note);
    qDebug() << "The newNote title is" << newNote->title();
    return addNote(newNote);
}

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
        note->setId(val["id"].toInt());
        note->setTitle(val["title"].toString());
        note->setText(val["text"].toString());
        note->setNotebook(val["id"].toInt());
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
