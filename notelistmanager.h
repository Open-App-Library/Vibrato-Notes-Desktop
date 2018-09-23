#ifndef NOTELISTMANAGER_H
#define NOTELISTMANAGER_H
#include <QList>
#include <QListWidget>

class NoteListManager
{
public:
    NoteListManager(QListWidget *listWidget);
    ~NoteListManager();
    QList<QWidget*> *noteList();
    void add_note_at_start();
    void add_note_at_end();
    void remove_note(int index);
private:
    QListWidget *m_listWidget;
    Ui::NoteListItem *noteListItemUi;
    QList<QWidget*> m_noteList;
};

#endif // NOTELISTMANAGER_H
