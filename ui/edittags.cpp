#include "edittags.h"
#include "ui_edittags.h"

EditTags::EditTags(Database *db, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditTags),
  m_db(db)
{
  ui->setupUi(this);

  connect(m_db->tagDatabase(), &TagDatabase::tagAdded,
          this, &EditTags::tagAdded);
  connect(m_db->tagDatabase(), &TagDatabase::tagChanged,
          this, &EditTags::tagChanged);
}

EditTags::~EditTags()
{
  delete ui;
}

void EditTags::addTag(Tag *tag)
{
  ListItemWithID *item = new ListItemWithID(tag->title(), tag->id());
  ui->tagList->addItem(item);
  t_tagItem tagItem;
  tagItem.tag = tag;
  tagItem.item = item;
  m_tagItems.append(tagItem);
}

void EditTags::loadTags()
{
  ui->tagList->clear();
  for ( Tag *t : m_db->tagDatabase()->list() ) {
    addTag(t);
  }
}

void EditTags::tagAdded(Tag *tag)
{
  addTag(tag);
}

void EditTags::tagChanged(Tag *tag)
{
  for (int i=0; i<m_tagItems.length(); i++) {
    Tag *curTag = m_tagItems.at(i).tag;
    ListItemWithID *item = m_tagItems.at(i).item;
    if ( tag == curTag ) {
      item->setText( tag->title() );
      break;
    }
  }
}
