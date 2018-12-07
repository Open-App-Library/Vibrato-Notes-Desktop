#include <QStringList>
#include <QDebug>
#include <QFile>
#include "basictreeitem.h"
#include "../../meta/notebook.h"
#include "../../meta/info/appinfo.h"

BasicTreeItem::BasicTreeItem(Notebook *notebook, BasicTreeItem *parent)
{
  NotebookOrTag nortObject;
  m_type = Type_Notebook;
  nortObject.notebook = notebook;
  m_object = nortObject;
  updateLabel();
  m_parentItem = parent;
  m_id = notebook->id();
}

BasicTreeItem::BasicTreeItem(Tag *tag, BasicTreeItem *parent)
{
  NotebookOrTag nortObject;
  m_type = Type_Tag;
  nortObject.tag = tag;
  m_object = nortObject;
  updateLabel();
  m_parentItem = parent;
  m_id = tag->id();
}

BasicTreeItem::BasicTreeItem(const QString label, BasicTreeItem *parent)
{
  m_label = label;
  m_parentItem = parent;
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

bool BasicTreeItem::isOther() const
{
  return m_type != Type_Notebook && m_type != Type_Tag;
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
  m_type = Type_Notebook;
  m_object.notebook = notebook;
  connect(notebook, &Notebook::notebookIDChanged,
          this, &BasicTreeItem::notebookIDChanged);
}

void BasicTreeItem::setObjectTag(Tag *tag)
{
  m_type = Type_Tag;
  m_object.tag = tag;
  connect(tag, &Tag::tagIDChanged,
          this, &BasicTreeItem::tagIDChanged);
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
  BasicTreeItem *item = m_childItems.at(index);
  m_childItems.removeAt(index);
  //  delete item;
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

void BasicTreeItem::notebookIDChanged(Notebook *notebook)
{
  m_id = notebook->id();
}

void BasicTreeItem::tagIDChanged(Tag *tag)
{
  m_id = tag->id();
}
