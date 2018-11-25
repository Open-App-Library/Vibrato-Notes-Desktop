#include "note_edittags.h"
#include "ui_note_edittags.h"
#include "../../meta/db/notebookdatabase.h"

#include <QDebug>

Note_EditTags::Note_EditTags(Database *db, Note *note, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Note_EditTags),
    m_db(db),
    m_note(note)
{
    ui->setupUi(this);

    // Tree Widget
    m_tagList = ui->tagList;
    m_tagList->setSelectionMode( QAbstractItemView::ExtendedSelection ); // allow multiple item selection

    // Button box
    m_removeTagButton = new QPushButton( "Remove selected tag from note" );

    ui->buttonBox->addButton(m_removeTagButton, QDialogButtonBox::ActionRole);

    updateTitle();

    loadNotesTags();

    connect(note, &Note::noteChanged,
            this, &Note_EditTags::noteChanged);
    connect(m_removeTagButton, &QPushButton::clicked,
            this, &Note_EditTags::removeTagsFromNote);
    connect(m_tagList, &QListWidget::currentItemChanged,
            this, &Note_EditTags::currentItemChanged);
}

Note_EditTags::~Note_EditTags()
{
    delete m_removeTagButton;
    delete ui;
}

Note *Note_EditTags::note()
{
    return m_note;
}

void Note_EditTags::noteChanged(void)
{
    updateTitle();
    loadNotesTags();
}

void Note_EditTags::removeTagsFromNote()
{
    QList<QListWidgetItem*> selItemsArray = m_tagList->selectedItems();
    if ( selItemsArray.length() == 0)
        return;

    bool tagsChanged = false;
    for (int i=0; i < selItemsArray.length(); i++) {
        ListItemWithID *sel = static_cast<ListItemWithID*>( selItemsArray.at(i) );

        int id = sel->id();
        Tag *t = m_db->tagDatabase()->findTagWithID(id);
        if (t == nullptr)
            continue;

        // Save into note object
        QVector<int> tags = m_note->tags();
        if ( tags.contains(id) ) {
            tags.remove( tags.indexOf(id) );
            m_note->setTags( tags );
            tagsChanged = true;
        }
    }
    emit tagChanged();
}

void Note_EditTags::updateTitle()
{
    if ( m_note == nullptr)
        return;

    setWindowTitle( QString("Editing notebook for \"%1\"").arg(m_note->title()) );
}

void Note_EditTags::clearList()
{
    if (m_listItems.length() == 0)
        return;

    for ( int i=m_listItems.length()-1; i>=0; i-- ) {
        ListItemWithID *item = m_listItems.at(i);
        m_listItems.remove(i);
        delete item;
    }
}

void Note_EditTags::addTagToList(Tag *tag)
{
    ListItemWithID *item = new ListItemWithID(tag->title(), tag->id());
    m_listItems.append(item);

    m_tagList->addItem(item);
}

void Note_EditTags::loadNotesTags()
{
    clearList();
    // Now add everything else
    QVector<int> tags = m_note->tags();
    for ( int i=0; i<tags.length(); i++ ) {
        Tag *t = m_db->tagDatabase()->findTagWithID( tags.at(i) );
        if ( t == nullptr)
            continue;
        addTagToList(t);
    }
}

void Note_EditTags::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QList<QListWidgetItem*> selItemsArray = m_tagList->selectedItems();
    if ( selItemsArray.length() == 0) {
        m_removeTagButton->setEnabled(false);
        m_removeTagButton->setText( "Remove selected tag from note" );
        return;
    } else
        m_removeTagButton->setEnabled(true);

    if ( selItemsArray.length() == 1)
        m_removeTagButton->setText( "Remove selected tag from note" );
    else
        m_removeTagButton->setText( "Remove selected tags from note" );
}
