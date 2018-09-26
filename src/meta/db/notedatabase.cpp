#include <QDebug>
#include "notedatabase.h"

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
