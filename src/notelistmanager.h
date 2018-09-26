#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QList>
#include <QListWidget>
#include "notelistitem.h"
#include "meta/note.h"

class NoteListManager
{
public:
    NoteListManager(QListWidget *listWidget);
    ~NoteListManager();
    QList<QWidget*> *noteList();
    NoteListItem *add_note(Note *note);
    void remove_note(int index);
    void clear();
private:
    QListWidget *m_listWidget;
    QList<NoteListItem*> m_noteList;
};

#endif // NOTELISTMANAGER_H
