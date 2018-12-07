#include "notebook.h"

Notebook::Notebook(int id, QString title, Notebook *parent, QVector<Notebook*> children) :
  m_id(id),
  m_title(title),
  m_parent(parent),
  m_children(children)
{
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
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
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
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    return;
  m_parent = parent;
  emit notebookParentChanged(this);
  emit notebookChanged(this);
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
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    return;
  m_children.append(child);
  child->setParent(this);
  emit notebookChildrenChanged(this);
  emit notebookChanged(this);
}

void Notebook::removeChild(Notebook *child)
{
  if ( m_id == NOTEBOOK_DEFAULT_NOTEBOOK_ID )
    return;
  int index = m_children.indexOf(child);
  child->setParent(nullptr);
  m_children.removeAt(index);
  emit notebookChildrenChanged(this);
  emit notebookChanged(this);
}
