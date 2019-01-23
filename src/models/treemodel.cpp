#include "items/basictreeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>
#include <QIcon>
#include <QMimeData>

TreeModel::TreeModel(QObject *parent) :
  QAbstractItemModel(parent),
  m_rootItem( new BasicTreeItem("Root") )
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
  flags.setFlag(Qt::ItemIsSelectable, item->selectable());
  flags.setFlag(Qt::ItemIsEditable, item->isNotebook() || item->isTag());
  flags.setFlag(Qt::ItemIsDropEnabled,
                (item->isNotebook() && item->id() != -1) ||
                (item->isNotebooksLabel() || item->isTagsLabel()));
  flags.setFlag(Qt::ItemIsDragEnabled, item->isNotebook() || item->isTag());

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

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions TreeModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
    return QStringList() << "application/vibrato-basictreeitem" << "image/*";
}

QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QString data;
            QModelIndex curIndex = index;
            int invalidCount = 0;
            while (curIndex.isValid() && invalidCount < 2) {
                data = data.append(QString("%1").arg(curIndex.row()));
                curIndex = curIndex.parent();
                if (curIndex.isValid() && invalidCount == 0)
                    data +=",";
                if (!curIndex.isValid())
                    invalidCount++;
            }
            stream << data;
        }
    }


    mimeData->setData("application/vibrato-basictreeitem", encodedData);
    return mimeData;
}

bool TreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vibrato-basictreeitem"))
        return false;

    if (column > 0)
        return false;

    QByteArray encodedData = data->data("application/vibrato-basictreeitem");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QString mimeString;
    stream >> mimeString;

    QStringList rowParents = mimeString.split(",");
    QModelIndex theIndex = QModelIndex();
    for ( int i = rowParents.length()-1; i >= 0; i-- ) {
        int rowNum = rowParents.at(i).toInt();
        theIndex = index(rowNum, 0, theIndex);
    }

    if (theIndex.isValid()) {
        BasicTreeItem *sourceItem = static_cast<BasicTreeItem*>(theIndex.internalPointer());
        BasicTreeItem *destParentItem = static_cast<BasicTreeItem*>(parent.internalPointer());

        enum contentTypes {NotebookType, TagType};
        int contentType;

        if (sourceItem->isNotebook() && (destParentItem->isNotebook() ||
                                         destParentItem->isNotebooksLabel())) {
            contentType = NotebookType;
        }
        else if (sourceItem->isTag() && (destParentItem->isTag() ||
                                         destParentItem->isTagsLabel())) {
            contentType = TagType;
        }
        else {
            return false;
        }


        // Do not allow users to make a child of the sourceItem the sourceItem's new parent.
        QVector<BasicTreeItem*> destBlacklist;
        destBlacklist += sourceItem->recurseChildren();
        if ( destBlacklist.contains(destParentItem) ) {
            return false;
        }

        // First task is to make the items under the correct parent/hierarchy
        if ( sourceItem->parentItem() != destParentItem ) {
            sourceItem->parentItem()->removeChild(sourceItem); // Remove from old parent
            destParentItem->appendChild(sourceItem);
        }

        // The 'row' includes itself. This fixes it.
        if (theIndex.row() < row)
            row -= 1;

        // Don't allow the user to put their notebook above the default
        if (destParentItem->isNotebooksLabel())
            row = row < 1 ? 1 : row;

        // Next we adjust the row order
        destParentItem->moveChild(sourceItem, row);

        emit layoutChanged();

        return true;
    }

    return false;
}
