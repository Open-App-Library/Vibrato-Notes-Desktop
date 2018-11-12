/*
 * This 'database' is for holding the notes in memory.
 * Do not confuse this with something that would connect
 * to an actual real-world database.
 */

#ifndef NOTELIST_H
#define NOTELIST_H
#include <QList>
#include "../note.h"

#define NULL_INT -1

class NoteDatabase : public QObject
{
	Q_OBJECT
public:
    NoteDatabase();

    // Lists out the notes in the in-memory database
    QList<Note*> list() const;
    int          size() const;

    Note *addNote(Note *note);
    Note *addNote(Note note);

    void removeNote(int index);
    void removeNote(Note *note);
    void clearNotes();

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyNotes();

private:
    QList<Note*> m_list;

    int getInt(QJsonObject obj, QString key);
};

#endif // NOTELIST_H
