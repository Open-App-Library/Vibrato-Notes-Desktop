#include "notebook.h"
#include <QDebug>

Notebook::Notebook(int syncId, int id, QString title, Notebook *parent, QVector<Notebook*> children) :
  m_syncId(syncId),
  m_id(id),
  m_title(title),
  m_parent(parent),
  m_children(children)
{
}

int Notebook::syncId() const
{
  return m_syncId;
}

void Notebook::setSyncId(int syncId)
{
  if ( m_syncId == NOTEBOOK_DEFAULT_NOTEBOOK_ID ||
       m_syncId   == NOTEBOOK_DEFAULT_NOTEBOOK_ID)
    return;
  m_syncId = syncId;
  emit notebookSyncIDChanged(this);
}

int Notebook::id() const
{
  return m_id;
}

void Notebook::setId(int id)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID ||
       id   == NOTEBOOK_DEFAULT_NOTEBOOK_ID)
    return;
  m_id = id;
  emit notebookIDChanged(this);
  emit notebookChanged(this);
}

QString Notebook::title() const
{
  return m_title;
}

void Notebook::setTitle(const QString &title)
{
  // If Default Notebook or title is empty, return.
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID ||
       title.trimmed().isEmpty() )
    return;
  m_title = title;
  emit notebookTitleChanged(this);
  emit notebookChanged(this);
}

Notebook *Notebook::parent() const
{
  return m_parent;
}

void Notebook::setParent(Notebook *parent)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID ) return;

  if (m_parent != nullptr)
    m_parent->removeChild_primitive(this);
  setParent_primitive(parent);
  if (m_parent != nullptr)
    m_parent->addChild_primitive(this);

  emit notebookChildrenChanged(parent);
  emit notebookParentChanged(this);
  emit notebookChanged(this);
}

void Notebook::requestParentChangeToID(int parentID) {
  emit requestedParentWithID(this, parentID);
}

QVector<Notebook *> Notebook::children() const
{
  return m_children;
}

QVector<Notebook *> Notebook::recurseChildren(Notebook* parent) const
{
  Notebook *curNotebook = parent;
  QVector<Notebook*> notebooks;
  if (curNotebook == nullptr)
    curNotebook = const_cast<Notebook*>(this);

  notebooks.append( curNotebook->children() );
  for ( int i = 0; i < curNotebook->children().size(); i++ ) {
    notebooks.append( recurseChildren( curNotebook->children()[i] ) );
  }
  return notebooks;
}

void Notebook::setChildren(const QVector<Notebook *> &children)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    return;
  m_children = children;
  emit notebookChildrenChanged(this);
  emit notebookChanged(this);
}

void Notebook::addChild(Notebook *child)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID ) return;

  child->parent()->removeChild_primitive(child);
  child->setParent_primitive(this);
  addChild_primitive(child);

  emit notebookParentChanged(child);
  emit notebookChildrenChanged(this);
  emit notebookChanged(this);
}

void Notebook::removeChild(Notebook *child)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    return;

  removeChild_primitive(child);
  child->setParent_primitive(nullptr);

  emit notebookParentChanged(child);
  emit notebookChildrenChanged(this);
  emit notebookChanged(this);
}

void Notebook::setParent_primitive(Notebook *parent)
{
  m_parent = parent;
}

void Notebook::addChild_primitive(Notebook *child)
{
  m_children.append(child);
}

void Notebook::removeChild_primitive(Notebook *child)
{
  int index = m_children.indexOf(child);
  if (index > -1) m_children.removeAt(index);
}
