#include "notebook.h"
#include <QDebug>
#include "../sql/sqlmanager.h"

Notebook::Notebook(VibratoObjectMap fields) :
    VibratoObject (fields)
{
    assignFieldsExplicitly(fields);
}

Notebook *Notebook::parent() const
{
  return m_parent;
}

void Notebook::setParent(Notebook *parent)
{
  if ( isDefaultNotebook() ) return;

  if (m_parent != nullptr)
    m_parent->removeChildExplicitly(this);
  setParentExplicitly(parent);
  if (m_parent != nullptr)
    m_parent->appendChildExplicitly(this);

  emit childrenChanged(parent);
  emit parentChanged(this);
  emit changed(this);
}

void Notebook::setParentExplicitly(Notebook *parent)
{
  m_parent = parent;
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
  if ( isDefaultNotebook() )
    return;
  m_children = children;
  emit childrenChanged(this);
  emit changed(this);
}

void Notebook::appendChild(Notebook *child)
{
  if ( isDefaultNotebook() ) return;

  child->parent()->removeChildExplicitly(child);
  child->setParentExplicitly(this);
  appendChildExplicitly(child);

  emit parentChanged(child);
  emit childrenChanged(this);
  emit changed(this);
}

void Notebook::removeChild(Notebook *child)
{
  if ( isDefaultNotebook() )
    return;

  removeChildExplicitly(child);

  emit childrenChanged(this);
  emit changed(this);
}

void Notebook::appendChildExplicitly(Notebook *child)
{
  m_children.append(child);
}

void Notebook::removeChildExplicitly(Notebook *child)
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

  setRowExplicitly(row);
  emit rowChanged(this);
  emit changed(this);
}

void Notebook::setRowExplicitly(int row)
{
    m_row = row;
}

bool Notebook::isDefaultNotebook() const
{
  return uuid() == nullptr;
}

Notebook *Notebook::createBlankNotebook()
{
    return new Notebook(VibratoObjectMap());
}

void Notebook::assignFieldsExplicitly(QMap<QString, QVariant> fields)
{
    VibratoObject::assignFieldsExplicitly(fields);

    this->setRowExplicitly(
                fields.contains("row") ?
                    fields.value("row").toInt() : defaultRow());
    this->setParentExplicitly(
                fields.contains("parent") ?
                    fields.value("parent").value<Notebook*>() : defaultParent());
    this->setRowExplicitly(
                fields.contains("row") ?
                    fields.value("row").toInt() : defaultRow());


}

QMap<QString, QVariant> Notebook::fields() const
{
    VibratoObjectMap fields = VibratoObject::fields();
    fields["row"] = row();
    fields["parent"] = QVariant::fromValue(parent());
    fields["children"] = QVariant::fromValue(children());
    return fields;
}

QVector<QString> Notebook::defaultFieldKeys() const
{
    return NOTEBOOK_DEFAULT_FIELDS;
}

Notebook *Notebook::defaultParent() const
{
    return NOTEBOOK_DEFAULT_PARENT;
}

QVector<Notebook *> Notebook::defaultChildren() const
{
    return NOTEBOOK_DEFAULT_CHILDREN;
}

int Notebook::defaultRow() const
{
    return NOTEBOOK_DEFAULT_ROW;
}

QString Notebook::defaultTitle() const
{
    return NOTEBOOK_DEFAULT_TITLE;
}
