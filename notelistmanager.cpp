#include "notelistmanager.h"
#include "ui_notelistitem.h"

NoteListManager::NoteListManager() :
    noteListItemUi(new Ui::NoteListItem)
{
    // note list
    QListWidget *list = ui->noteList;
    QListWidgetItem *myitem = new QListWidgetItem(list);
    myitem->setSizeHint(QSize(myitem->sizeHint().width(), 100));
    Ui::NoteListItem *item = new Ui::NoteListItem;
    QWidget *coolnote = new QWidget;
    item->setupUi(coolnote);
    list->setItemWidget(myitem, coolnote);

}

NoteListManager::NoteListManager(QListWidget *listWidget) :
    m_listWidget(listWidget),
    noteListItemUi(new Ui::NoteListItem)
{

}

NoteListManager::~NoteListManager()
{

}

void NoteListManager::add_note_at_start()
{

}

void NoteListManager::add_note_at_end()
{

}

void NoteListManager::remove_note(int index)
{

}

QList<QWidget> *NoteListManager::noteList()
{
    return m_noteList;
}
