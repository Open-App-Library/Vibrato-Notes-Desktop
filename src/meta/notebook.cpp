#include "notebook.h"

Notebook::Notebook()
{
    m_id = -1;
    m_title = "Untitled Notebook";
    m_parent = nullptr;
}

int Notebook::id() const
{
    return m_id;
}

void Notebook::setId(int id)
{
    m_id = id;
}

QString Notebook::title() const
{
    return m_title;
}

void Notebook::setTitle(const QString &title)
{
    m_title = title;
}

Notebook *Notebook::parent() const
{
    return m_parent;
}

void Notebook::setParent(Notebook *parent)
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
    m_children = children;
}

void Notebook::addChild(Notebook *child)
{
    m_children.append(child);
    child->setParent(this);
}

void Notebook::removeChild(Notebook *child)
{
    int index = m_children.indexOf(child);
    child->setParent(nullptr);
    m_children.removeAt(index);
}
