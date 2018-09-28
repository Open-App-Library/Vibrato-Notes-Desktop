#include "items/basictreeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>

TreeModel::TreeModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_rootItem( new BasicTreeItem("User Data") )
{
    // TreeModel Constructor
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

BasicTreeItem *TreeModel::root() const
{
    return m_rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    (void) parent; // This line does nothing. Just silences compiler warning for unused parameter
    return 1; // One column only
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    // Safety check
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

    return item->label();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    (void) section; (void) orientation; (void) role; // This line stops 'unused variable' compiler warnings
    return QVariant(tr("User Data"));
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BasicTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<BasicTreeItem*>(parent.internalPointer());

    BasicTreeItem *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    BasicTreeItem *childItem = static_cast<BasicTreeItem*>(index.internalPointer());
    BasicTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    BasicTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<BasicTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
