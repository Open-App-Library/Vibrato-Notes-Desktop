#include "notelistmodel.h"
#include <QDebug>

NoteListModel::NoteListModel(QListView *view) : QAbstractItemModel()
{
	m_view = view;
    QString style = QString("QListView:item { height: %1 }").arg( NOTE_LIST_ITEM_HEIGHT );
	m_view->setStyleSheet(style);
}

void NoteListModel::refresh(int row)
{
	QModelIndex topLeft = createIndex(row,0);
    emit dataChanged(topLeft, topLeft);
}

void NoteListModel::clear()
{
    for (int i = 0; i < m_noteItems.length(); i++) {
        m_noteItems[0]->deleteWidget();
        m_noteItems.remove(0);
        removeRow(0);
    }
}

NoteListItem *NoteListModel::prependItem(Note *note)
{
	NoteListItem *i = new NoteListItem(note);
	m_noteItems.prepend( i );
	refresh(0);
	return i;
}

NoteListItem *NoteListModel::appendItem(Note *note)
{
	NoteListItem *i = new NoteListItem(note);
	m_noteItems.append( i );
	refresh( m_noteItems.length()-1 );
	return i;
}

int NoteListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant NoteListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	NoteListItem *item = static_cast<NoteListItem*>(index.internalPointer());

    if (item->widget() != nullptr) {
        m_view->setIndexWidget(index, item->widget() );
    }

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
