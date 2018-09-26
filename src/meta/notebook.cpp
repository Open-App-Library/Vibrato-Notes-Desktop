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

QList<Notebook *> Notebook::children() const
{
    return m_children;
}

void Notebook::setChildren(const QList<Notebook *> &children)
{
    m_children = children;
}
