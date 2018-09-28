#include <QStringList>

#include "BasicTreeItem.h"

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

BasicTreeItem *BasicTreeItem::getChild(int row)
{
    return m_childItems.value(row);
}

void BasicTreeItem::appendChild(BasicTreeItem *item)
{
    m_childItems.append(item);
}

int BasicTreeItem::childCount() const
{
    return m_childItems.count();
}

BasicTreeItem *BasicTreeItem::parentItem()
{
    return m_parentItem;
}

int BasicTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<BasicTreeItem*>(this));

    return 0;
}
