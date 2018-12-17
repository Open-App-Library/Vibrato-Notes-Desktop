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
  Note *addDefaultNote(); // Takes note, sets certain fields to default values.
  //p Note *addNote(Note note);

  void removeNote(int index);
  void removeNote(Note *note);
  void removeNotes(QVector<Note*> notes);
  void clearNotes();

  void loadJSON(QJsonDocument jsonDocument);

  void loadDummyNotes();

  void removeNotesWithNotebookID(int notebookID);
  void removeNotesWithNotebookIDs(QVector<int> notebookIDs);

  void removeTagFromNotes(int tagID);

  QVector<Note*> findNotesWithNotebookIDs(QVector<int> notebookIDs);

  bool noteWithIDExists(int noteID) const;

signals:
  void noteRemoved(int noteID);

private:
  QList<Note*> m_list;

  QJsonValue get(QJsonObject obj, QString key);
};

#endif // NOTELIST_H
