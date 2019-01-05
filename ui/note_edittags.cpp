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

  setModal(true);

  // Tree Widget
  m_tagList = ui->tagList;
  m_tagList->setSelectionMode( QAbstractItemView::ExtendedSelection ); // allow multiple item selection

  // Button box
  m_removeTagButton = new QPushButton("Remove selected tag from note");
  ui->buttonBox->addButton(m_removeTagButton, QDialogButtonBox::ActionRole);

  // Tag input
  m_tagInput = ui->tagInput;

  updateTitle();

  loadNotesTags();

  connect(note, &Note::noteChanged,
          this, &Note_EditTags::noteChanged);
  connect(m_removeTagButton, &QPushButton::clicked,
          this, &Note_EditTags::removeTagsFromNote);
  connect(m_tagInput, &QLineEdit::returnPressed,
          this, &Note_EditTags::addTag);
  connect(ui->addTagButton, &QPushButton::clicked,
          this, &Note_EditTags::addTag);
  connect(m_tagList, &QListWidget::currentItemChanged,
          this, &Note_EditTags::currentItemChanged);

  connect(m_db->tagDatabase(), &TagDatabase::tagRemoved,
          this, &Note_EditTags::updateTagsCompletionList);
  connect(m_db->tagDatabase(), &TagDatabase::tagAdded,
          this, &Note_EditTags::updateTagsCompletionList);
  connect(m_db->tagDatabase(), &TagDatabase::tagChanged,
          this, &Note_EditTags::updateTagsCompletionList);

  // Create a qcompleter
  m_completer = new QCompleter({""}, this);
  m_completer->setCaseSensitivity( Qt::CaseInsensitive );
  m_tagInput->setCompleter(m_completer);
  m_completionList = qobject_cast<QStringListModel*>(m_completer->model());
  updateTagsCompletionList();
}

Note_EditTags::~Note_EditTags()
{
  delete m_removeTagButton;
  delete m_tagInput;
  delete ui;
}

Note *Note_EditTags::note()
{
  return m_note;
}

void Note_EditTags::keyPressEvent(QKeyEvent *event)
{
  // Ignore enter key. We must ignore it because we are using it in our tagInput.
  if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    return;
  QDialog::keyPressEvent(event);
}

void Note_EditTags::noteChanged(void)
{
  updateTitle();
  loadNotesTags();
}

void Note_EditTags::updateTagsCompletionList(void)
{
  // Create a string list of all of the tags
  QStringList tags;
  for (Tag *tag : m_db->tagDatabase()->list() )
    tags << tag->title();

  m_completionList->setStringList(tags);
}

void Note_EditTags::removeTagsFromNote()
{
  QList<QListWidgetItem*> selItemsArray = m_tagList->selectedItems();
  if ( selItemsArray.length() == 0)
    return;

  QVector<int> tags = m_note->tags();
  QVector<int> remove_list;
  for (int i=0; i < selItemsArray.length(); i++) {
    ListItemWithID *sel = static_cast<ListItemWithID*>( selItemsArray.at(i) );

    int id = sel->id();
    Tag *t = m_db->tagDatabase()->findTagWithID(id);
    if (t == nullptr)
      continue;

    if ( tags.contains(id) )
      remove_list.append(tags.indexOf(id));
  }

  // sort remove list (ascending)
  std::sort(remove_list.begin(), remove_list.end());

  if ( remove_list.size() > 0 ) {
    for (int i=remove_list.length()-1; i>=0; i--)
      tags.remove(remove_list.at(i));
    m_note->setTags( tags );
    emit tagChanged();
  }
}

void Note_EditTags::addTag()
{
  m_db->addTagToNote(m_note, m_tagInput->text());
  loadNotesTags();
  m_tagInput->clear();
  m_tagInput->setFocus();
}

void Note_EditTags::updateTitle()
{
  if ( m_note == nullptr)
    return;

  setWindowTitle( QString("Editing tags for \"%1\"").arg(m_note->title()) );
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
  if ( m_listItems.length() > 0 && m_tagList->selectedItems().length() == 0 )
    m_tagList->setCurrentRow(0);
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
