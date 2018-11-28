#include "notelistproxymodel.h"
#include "../items/notelistitem.h"
#include "../../meta/db/notedatabase.h"
#include "../notelistmodel.h"
#include <QStandardItemModel>

#include <QDebug>

NoteListProxyModel::NoteListProxyModel(QListView *view) :
    QSortFilterProxyModel(),
    m_view(view)
{
    setDynamicSortFilter(true);
}

QVariant NoteListProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    QModelIndex realIndex = mapToSource(index);
    NoteListItem *item = static_cast<NoteListItem*>( realIndex.internalPointer() );

    if (item->widget() != nullptr) {
        m_view->setIndexWidget( index, item->widget() );
    }

    return QVariant();
}

void NoteListProxyModel::setSortingMethod(int sortingMethod)
{
    m_sortingMethod = sortingMethod;
}


bool NoteListProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if ( !left.isValid() ) return true;
    if ( !right.isValid() ) return false;
    NoteListItem *item1 = static_cast<NoteListItem*>(left.internalPointer());
    NoteListItem *item2 = static_cast<NoteListItem*>(right.internalPointer());

    switch (m_sortingMethod) {
    case DateCreated:
        return Note::byDateCreatedAsc(item1->note(), item2->note());
    case DateModified:
        return Note::byDateModifiedAsc(item1->note(), item2->note());
    }

    return false;
}

bool NoteListProxyModel::greaterThan(const QModelIndex &left, const QModelIndex &right) const
{
    if ( !left.isValid() ) return false;
    if ( !right.isValid() ) return true;
    NoteListItem *item1 = static_cast<NoteListItem*>(left.internalPointer());
    NoteListItem *item2 = static_cast<NoteListItem*>(right.internalPointer());

    switch (m_sortingMethod) {
    case DateCreated:
        return Note::byDateCreatedDesc(item1->note(), item2->note());
    case DateModified:
        return Note::byDateModifiedDesc(item1->note(), item2->note());
    }

    return false;
}
