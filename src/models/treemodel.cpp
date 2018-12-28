#include "items/basictreeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>
#include <QIcon>

TreeModel::TreeModel(QObject *parent) :
  QAbstractItemModel(parent),
  m_rootItem( new BasicTreeItem("Vibrato Tree") )
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
  if ( !index.isValid() ||
       (role != Qt::DisplayRole &&
        role != Qt::DecorationRole &&
        role != Qt::EditRole) )
    return QVariant();

  // Get the BasicTreeItem object from the index
  BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

  // If requesting icon, return icon if exists
  if ( role == Qt::DecorationRole ) // Set Icon
    return item->icon();

  return item->label();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role != Qt::EditRole)
    return false;

  BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

  if ( item->isNotebook() )
    item->object().notebook->setTitle(value.toString());
  else if ( item->isTag() )
    item->object().tag->setTitle(value.toString());
  else return false;

  emit dataChanged(index, index);
  return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return nullptr;

  BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  if ( item->isNotebook() || item->isTag() )
    flags |= Qt::ItemIsEditable;

  return flags;
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

QVector<QModelIndex> TreeModel::recurseChildren() {
  QVector<QModelIndex> items;
  for (int i=0; i<rowCount(); i++) {
    items.append(index(i, 0));
    items.append(m_recurseChildren(index(i, 0)));
  }
  return items;
}

QVector<QModelIndex> TreeModel::m_recurseChildren(QModelIndex parent) {
  QVector<QModelIndex> items;
  if ( !parent.isValid() )
    return items;

  for (int i=0; i<rowCount(parent); i++) {
    items.append(index(i, 0, parent));
    items.append(m_recurseChildren(index(i, 0, parent)));
  }

  return items;
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

  if (parentItem != nullptr)
    return parentItem->childCount();
  return 0;
}

QModelIndex TreeModel::getItem(BasicTreeItem *item, QModelIndex parent) {
  for (int i=0; i<rowCount(parent); i++) {
    QModelIndex curIndex = index(i,0, parent);
    if ( !curIndex.isValid() )
      continue;
    BasicTreeItem *curItem = static_cast<BasicTreeItem*>(curIndex.internalPointer());
    if ( curItem == item )
      return curIndex;
    QModelIndex childrenCheck = getItem(item, curIndex);
    if ( childrenCheck != QModelIndex() )
      return childrenCheck;
  }
  return QModelIndex();
}
