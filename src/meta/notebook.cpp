#include "notebook.h"
#include <QDebug>

Notebook::Notebook(QUuid sync_hash, QString title, QDateTime date_modified, Notebook *parent, int row, bool encrypted) :
  m_sync_hash(sync_hash),
  m_title(title),
  m_date_modified(date_modified),
  m_parent(parent),
  m_row(row),
  m_encrypted(encrypted)
{
  connect(this, &Notebook::changed,
          this, &Notebook::handleChange);
}

Notebook *Notebook::createBlankNotebook()
{
  return new Notebook(QUuid::createUuid(), NOTEBOOK_DEFAULT_TITLE);
}

QUuid Notebook::syncHash() const
{
  return m_sync_hash;
}

void Notebook::setSyncHash(QUuid syncHash)
{
  if ( defaultNotebook() )
    return;
  m_sync_hash = syncHash;
  emit syncHashChanged(this);
}

QString Notebook::title() const
{
  return m_title;
}

void Notebook::setTitle(const QString &title)
{
  QString cleanedTitle = title.trimmed();
  // If Default Notebook or no change or title is empty, return.
  if (defaultNotebook() ||
      cleanedTitle == m_title ||
      cleanedTitle.isEmpty())
    return;
  m_title = cleanedTitle;
  emit titleChanged(this);
  emit changed(this);
}

QDateTime Notebook::dateModified() const
{
  return m_date_modified;
}

void Notebook::setDateModified(QDateTime dateModified)
{
  m_date_modified = dateModified;
}

Notebook *Notebook::parent() const
{
  return m_parent;
}

void Notebook::setParent(Notebook *parent)
{
  if ( defaultNotebook() ) return;

  if (m_parent != nullptr)
    m_parent->removeChild_primitive(this);
  setParent_primitive(parent);
  if (m_parent != nullptr)
    m_parent->addChild_primitive(this);

  emit childrenChanged(parent);
  emit parentChanged(this);
  emit changed(this);
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
  if ( defaultNotebook() )
    return;
  m_children = children;
  emit childrenChanged(this);
  emit changed(this);
}

void Notebook::addChild(Notebook *child)
{
  if ( defaultNotebook() ) return;

  child->parent()->removeChild_primitive(child);
  child->setParent_primitive(this);
  addChild_primitive(child);

  emit parentChanged(child);
  emit childrenChanged(this);
  emit changed(this);
}

void Notebook::removeChild(Notebook *child)
{
  if ( defaultNotebook() )
    return;

  removeChild_primitive(child);

  emit childrenChanged(this);
  emit changed(this);
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

int Notebook::row() const
{
  return m_row;
}

void Notebook::setRow(int row)
{
  if ( row == m_row ) return;

  m_row = row;
  emit rowChanged(this);
  emit changed(this);
}

bool Notebook::encrypted() const
{
  return m_encrypted;
}

void Notebook::setEncrypted(bool encrypted)
{
  if ( encrypted == m_encrypted ) return;

  m_encrypted = encrypted;
  emit encryptedChanged(this);
  emit changed(this);
}

bool Notebook::defaultNotebook() const
{
  // Check if a blank UUID
  return m_sync_hash == nullptr;
}

void Notebook::requestParentWithSyncHash(QUuid parentSyncHash) {
  emit requestedParentWithSyncHash(this, parentSyncHash);
}

void Notebook::handleChange(Notebook *notebook)
{
  notebook->setDateModified( QDateTime::currentDateTime() );
}
