/*
 * This 'database' is for holding the notes in memory.
 * Do not confuse this with something that would connect
 * to an actual real-world database.
 */

#ifndef NOTELIST_H
#define NOTELIST_H
#include <QList>
#include "../note.h"
#include "../../sql/sqlmanager.h"

#define NULL_INT -1

class NoteDatabase : public QObject
{
  Q_OBJECT
public:
  NoteDatabase(SQLManager *sqlManager);

  // Lists out the notes in the in-memory database
  QList<Note*> list() const;
  int          size() const;

  Note *addNote(Note *note, bool addToSQL=true);
  Note *addDefaultNote(); // Takes note, sets certain fields to default values.
  //p Note *addNote(Note note);

  void removeNote(int index);
  void removeNote(Note *note);
  void removeNotes(QVector<Note*> notes);
  void clearNotes();

  void loadSQL();

  void removeNotesWithNotebookSyncHash(QUuid notebookSyncHash);
  void removeNotesWithNotebookSyncHashes(QVector<QUuid> notebookSyncHashes);

  void removeTagFromNotes(QUuid tagSyncHash);

  QVector<Note*> findNotesWithNotebookIDs(QVector<QUuid> notebookIDs);

  bool noteWithSyncHashExists(QUuid syncHash) const;

signals:
  // Important: 'Trashed' means the *Note is set as trashed=true.
  //            'Deleted' means the *Note was deleted and removed from database. (Permanent)
  void noteAdded(Note *note);
  void noteChanged(Note *note);
  void noteTrashedOrRestored(Note *note, bool trashed);
  void noteDeleted(QUuid noteSyncHash);
  void noteFavoritedChanged(Note *note);

private slots:
  void slot_noteChanged(Note *note);
  void handleNoteFavoritedChanged(Note *note);

private:
  SQLManager *m_sqlManager;
  QList<Note*> m_list;

};

#endif // NOTELIST_H
