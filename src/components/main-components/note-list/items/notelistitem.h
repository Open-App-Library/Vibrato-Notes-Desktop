#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H
#include <QObject>
#include <QListView>
#include <QLabel>
#include "../../meta/note.h"

#define NOTE_LIST_ITEM_HEIGHT 90

namespace Ui {
  class NoteListItem;
}

class NoteListItem : public QObject
{
  Q_OBJECT
public:
  NoteListItem(Note *note);
  ~NoteListItem();

  Note *note() const;
  void setNote(Note *note);

  QUuid syncHash() const;

private:
  Note *m_note;
  QUuid m_sync_hash;
};

#endif // NOTELISTITEM_H
