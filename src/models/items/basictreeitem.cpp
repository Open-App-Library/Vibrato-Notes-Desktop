#include <QStringList>
#include <QDebug>
#include "basictreeitem.h"
#include "../../meta/notebook.h"
#include "../../meta/info/appinfo.h"

BasicTreeItem::BasicTreeItem(const QString label, BasicTreeItem *parent)
{
    m_label = label;
    m_parentItem = parent;
}

BasicTreeItem::~BasicTreeItem()
{
    qDeleteAll(m_childItems);
}

QString BasicTreeItem::label() const
{
    return m_label;
}

void BasicTreeItem::setLabel(QString label)
{
    m_label = label;
}

NotebookOrTag BasicTreeItem::object() const
{
    return m_object;
}

void BasicTreeItem::setObjectNotebook(Notebook *notebook)
{
    m_object.type = TYPE_NOTEBOOK;
    m_object.notebook = notebook;
}

void BasicTreeItem::setObjectTag(Tag *tag)
{
    m_object.type = TYPE_TAG;
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
    qDebug() << "hello";
    qDebug() << m_label;
    if (m_parentItem) {
        qDebug() << "Made it this far";
        return m_parentItem->m_childItems.indexOf(const_cast<BasicTreeItem*>(this));
    }
    qDebug() << "Made it even this far";
    return 0;
}
