#include <QStringList>
#include <QDebug>
#include "basictreeitem.h"
#include "../../meta/notebook.h"
#include "../../meta/info/appinfo.h"

BasicTreeItem::BasicTreeItem(Notebook *notebook, BasicTreeItem *parent)
{
    NotebookOrTag nortObject;
    m_type = TYPE_NOTEBOOK;
    nortObject.notebook = notebook;
    m_object = nortObject;
    updateLabel();
    m_parentItem = parent;
}

BasicTreeItem::BasicTreeItem(Tag *tag, BasicTreeItem *parent)
{
    NotebookOrTag nortObject;
    m_type = TYPE_TAG;
    nortObject.tag = tag;
    m_object = nortObject;
    updateLabel();
    m_parentItem = parent;
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
    return m_type == TYPE_NOTEBOOK;
}

bool BasicTreeItem::isTag() const
{
    return m_type == TYPE_TAG;
}

bool BasicTreeItem::isOther() const
{
    return m_type != TYPE_NOTEBOOK && m_type != TYPE_TAG;
}

QString BasicTreeItem::label()
{
    updateLabel();
    return m_label;
}

void BasicTreeItem::setLabel(QString label)
{
    m_label = label;
}

void BasicTreeItem::updateLabel()
{
    if ( isNotebook() )
        m_label = m_object.notebook->title();
    if ( isTag() )
        m_label = m_object.tag->title();
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
    m_type = TYPE_NOTEBOOK;
    m_object.notebook = notebook;
}

void BasicTreeItem::setObjectTag(Tag *tag)
{
    m_type = TYPE_TAG;
    m_object.tag = tag;
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
