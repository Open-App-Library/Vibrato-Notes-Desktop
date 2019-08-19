#include <QStringList>
#include <QDebug>
#include <QFile>
#include "basictreeitem.h"
#include "../../meta/notebook.h"
#include "../../meta/info/appinfo.h"

BasicTreeItem::BasicTreeItem(Notebook *notebook, BasicTreeItem *parent)
{
  setObjectNotebook(notebook);
  updateLabel();
  m_parentItem = parent;
  m_sync_hash = notebook->syncHash();
}

BasicTreeItem::BasicTreeItem(Tag *tag, BasicTreeItem *parent)
{
  setObjectTag(tag);
  updateLabel();
  m_parentItem = parent;
  m_sync_hash = tag->syncHash();
}

BasicTreeItem::BasicTreeItem(const QString label, BasicTreeItem *parent)
{
  m_label = label;
  m_parentItem = parent;
  m_type = Type_Other;
}

BasicTreeItem::~BasicTreeItem()
{
  qDeleteAll(m_childItems);
}

bool BasicTreeItem::isNotebook() const
{
  return m_type == Type_Notebook;
}

bool BasicTreeItem::isTag() const
{
  return m_type == Type_Tag;
}

bool BasicTreeItem::isSearchQuery() const
{
    return m_type == Type_SearchQuery;
}

bool BasicTreeItem::isNotebooksLabel() const
{
    return m_type == Type_NotebooksLabel;
}

bool BasicTreeItem::isTagsLabel() const
{
    return m_type == Type_TagsLabel;
}

bool BasicTreeItem::selectable() const
{
  return m_selectable;
}

void BasicTreeItem::setSelectable(bool selectable)
{
  m_selectable = selectable;
}

bool BasicTreeItem::isOther() const
{
  return
    m_type != Type_Notebook       &&
    m_type != Type_Tag            &&
    m_type != Type_NotebooksLabel &&
    m_type != Type_TagsLabel      &&
    m_type != Type_SearchQuery;
}

QUuid BasicTreeItem::syncHash() const
{
  return m_sync_hash;
}

QString BasicTreeItem::label() const
{
  return m_label;
}

void BasicTreeItem::setLabel(QString label)
{
  m_label = label;
}

void BasicTreeItem::updateLabel()
{
  if ( isNotebook() ) {
    m_label = m_object.notebook->title();
  } else if ( isTag() ) {
    m_label = m_object.tag->title();
  }
}

QIcon BasicTreeItem::icon() const
{
  return m_icon;
}

void BasicTreeItem::setIcon(QIcon icon)
{
  m_icon = icon;
}

NotebookOrTag BasicTreeItem::object() const
{
  return m_object;
}

void BasicTreeItem::setObjectNotebook(Notebook *notebook)
{
  NotebookOrTag blankObject;
  m_object = blankObject;
  m_type = Type_Notebook;
  m_object.notebook = notebook;
  connect(notebook, &Notebook::titleChanged,
          this, &BasicTreeItem::notebookTitleChanged);
  connect(notebook, &Notebook::syncHashChanged,
          this, &BasicTreeItem::notebookSyncHashChanged);
}

void BasicTreeItem::setObjectTag(Tag *tag)
{
  NotebookOrTag blankObject;
  m_object = blankObject;
  m_type = Type_Tag;
  m_object.tag = tag;
  connect(tag, &Tag::titleChanged,
          this, &BasicTreeItem::tagTitleChanged);
  connect(tag, &Tag::syncHashChanged,
          this, &BasicTreeItem::tagSyncHashChanged);
}

QString BasicTreeItem::searchQuery() const {
  return m_searchQuery;
}

void BasicTreeItem::setSearchQuery(QString searchQuery) {
  m_type = Type_SearchQuery;
  m_searchQuery = searchQuery.trimmed();
}

void BasicTreeItem::setIsNotebooksLabel()
{
    m_type = Type_NotebooksLabel;
}

void BasicTreeItem::setIsTagsLabel()
{
    m_type = Type_TagsLabel;
}

BasicTreeItem *BasicTreeItem::getChild(int row) const
{
  return m_childItems.value(row);
}

BasicTreeItem *BasicTreeItem::appendChild(BasicTreeItem *child)
{
  m_childItems.append(child);
  child->setParent(this);
  return child;
}

void BasicTreeItem::removeChild(int index)
{
  m_childItems.removeAt(index);
}

void BasicTreeItem::removeChild(BasicTreeItem *item)
{
  // Loop through child items to find the array index of it.
  int index = -1;
  for (int i=0; i < m_childItems.length(); i++)
    if (m_childItems.at(i) == item) {
      index = i;
    }
  // Return if index not found.
  if ( index == -1 ) return;

  // Remove the child
  removeChild(index);
}

void BasicTreeItem::moveChild(int row, int newRow)
{
    // Ensure values are greater than zero
    row = row < 0 ? 0 : row;
    newRow = newRow < 0 ? 0 : newRow;

    // Ensure values are no greater than max size
    int max = children().length()-1;
    row = row > max ? max : row;
    newRow = newRow > max ? max : newRow;

    // QVector::move
    m_childItems.move(row, newRow);
}

void BasicTreeItem::moveChild(BasicTreeItem *item, int newRow)
{
    int counter = 0;
    for (BasicTreeItem *child : children()) {
        if (child == item) {
            moveChild(counter, newRow);
            break;
        }
        counter++;
    }

}

QVector<BasicTreeItem *> BasicTreeItem::children() const
{
  return m_childItems;
}

QVector<BasicTreeItem*> BasicTreeItem::recurseChildren() const
{
  QVector<BasicTreeItem*> childList;
  for (int i = 0; i < this->childCount(); i++) {
    childList.append(this->getChild(i));
    childList.append(this->getChild(i)->recurseChildren());
  }
  return childList;
}

int BasicTreeItem::childCount() const
{
  return m_childItems.count();
}

BasicTreeItem *BasicTreeItem::parentItem()
{
  return m_parentItem;
}

void BasicTreeItem::setParent(BasicTreeItem *parent)
{
  m_parentItem = parent;
}

int BasicTreeItem::row() const
{
  if (m_parentItem) {
    return m_parentItem->m_childItems.indexOf(const_cast<BasicTreeItem*>(this));
  }
  return 0;
}

void BasicTreeItem::notebookTitleChanged(Notebook* notebook) {
  m_label = notebook->title();
}

void BasicTreeItem::notebookSyncHashChanged(Notebook *notebook)
{
  m_sync_hash = notebook->syncHash();
}

void BasicTreeItem::tagTitleChanged(Tag* tag) {
  m_label = tag->title();
}

void BasicTreeItem::tagSyncHashChanged(Tag *tag)
{
  m_sync_hash = tag->syncHash();
}
