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
  m_id = notebook->id();
}

BasicTreeItem::BasicTreeItem(Tag *tag, BasicTreeItem *parent)
{
  setObjectTag(tag);
  updateLabel();
  m_parentItem = parent;
  m_id = tag->id();
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
    m_type != Type_Notebook    &&
    m_type != Type_Tag         &&
    m_type != Type_SearchQuery;
}

int BasicTreeItem::id() const
{
  return m_id;
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
  connect(notebook, &Notebook::notebookTitleChanged,
          this, &BasicTreeItem::notebookTitleChanged);
  connect(notebook, &Notebook::notebookIDChanged,
          this, &BasicTreeItem::notebookIDChanged);
}

void BasicTreeItem::setObjectTag(Tag *tag)
{
  NotebookOrTag blankObject;
  m_object = blankObject;
  m_type = Type_Tag;
  m_object.tag = tag;
  connect(tag, &Tag::tagTitleChanged,
          this, &BasicTreeItem::tagTitleChanged);
  connect(tag, &Tag::tagIDChanged,
          this, &BasicTreeItem::tagIDChanged);
}

QString BasicTreeItem::searchQuery() const {
  return m_searchQuery;
}

void BasicTreeItem::setSearchQuery(QString searchQuery) {
  m_type = Type_SearchQuery;
  m_searchQuery = searchQuery.trimmed();
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

void BasicTreeItem::notebookIDChanged(Notebook *notebook)
{
  m_id = notebook->id();
}

void BasicTreeItem::tagTitleChanged(Tag* tag) {
  m_label = tag->title();
}

void BasicTreeItem::tagIDChanged(Tag *tag)
{
  m_id = tag->id();
}
