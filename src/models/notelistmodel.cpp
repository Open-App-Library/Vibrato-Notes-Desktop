#include "notelistmodel.h"
#include <QDebug>

NoteListModel::NoteListModel(QListView *view, NoteDatabase *noteDatabase) : QAbstractItemModel()
{
  m_view = view;
  QString style = QString("QListView:item::active  { color: white; } QListView:item { height: %1 }").arg( NOTE_LIST_ITEM_HEIGHT );
  m_view->setStyleSheet(style);
  m_noteDatabase = noteDatabase;

  connect(m_noteDatabase, &NoteDatabase::noteDeleted,
          this, &NoteListModel::removeNoteItemWithSyncHash);
}

QVector<NoteListItem *> NoteListModel::noteItems() const
{
  return m_noteItems;
}

void NoteListModel::refresh(int row)
{
  QModelIndex topLeft = createIndex(row,0);
  emit dataChanged(topLeft, topLeft);
}

void NoteListModel::clear()
{
  if (m_noteItems.length() > 0)
    removeRows(0, m_noteItems.length());
}

NoteListItem *NoteListModel::prependItem(Note *note)
{
  int newIndex = 0;
  beginInsertRows(QModelIndex(), newIndex, newIndex);

  m_noteItems.prepend( new NoteListItem(nullptr));
  NoteListItem *i = m_noteItems.at(newIndex);
  i->setNote(note);

  endInsertRows();
  return i;
}

NoteListItem *NoteListModel::appendItem(Note *note)
{
  int newIndex = m_noteItems.length();
  beginInsertRows(QModelIndex(), newIndex, newIndex);

  m_noteItems.append( new NoteListItem(nullptr));
  NoteListItem *i = m_noteItems.at(newIndex);
  i->setNote(note);

  endInsertRows();
  return i;
}

void NoteListModel::noteDateChanged(NoteListItem *item)
{
  for ( int i=0; i < m_noteItems.length(); i++ ) {
    if ( m_noteItems.at(i) == item ) {
      refresh( i );
      break;
    }
  }
}

bool NoteListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  beginInsertRows(parent, position, position + rows - 1);

  for (int i = 0; i < rows; i++)
    m_noteItems.insert(position+i, new NoteListItem(nullptr));

  endInsertRows();

  return true;
}

bool NoteListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  beginRemoveRows(parent, position, position + rows - 1);

  for (int i = 0; i < rows; i++) {
    delete m_noteItems[position];
    m_noteItems[position] = nullptr;
    m_noteItems.remove(position);
  }

  endRemoveRows();

  return true;
}

int NoteListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

Note* NoteListModel::noteFromIndex(QModelIndex index) {
  if (!index.isValid())
    return nullptr;
  NoteListItem *item = static_cast<NoteListItem*>(index.internalPointer());
  return item->note();
}

QVariant NoteListModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  return QVariant();
}

Qt::ItemFlags NoteListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return nullptr;

  return QAbstractItemModel::flags(index);
}

QVariant NoteListModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
  return QVariant();
}

QModelIndex NoteListModel::index(int row, int column, const QModelIndex &parent)
  const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (row < m_noteItems.length()) {
    QModelIndex index = createIndex(row, column, m_noteItems.at(row));
    return index;
  }
  else
    return QModelIndex();
}

QModelIndex NoteListModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

int NoteListModel::rowCount(const QModelIndex &parent) const
{
  return m_noteItems.length();
}

void NoteListModel::removeNoteItemWithSyncHash(QUuid syncHash)
{
  for (int i=0; i<m_noteItems.length(); i++) {
    if ( m_noteItems.at(i)->syncHash() == syncHash)
      removeRow(i);
  }
}
