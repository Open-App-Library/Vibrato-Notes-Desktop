#include "notelistproxymodel.h"
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

    NoteListItemWidget *w = getWidget(3, item->note());
//    if ( !m_view->indexWidget(index) )
//        m_view->setIndexWidget( index,  );

	return QVariant();
}

void NoteListProxyModel::setSortingMethod(int sortingMethod)
{
    m_sortingMethod = sortingMethod;
}

bool NoteListProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    NoteListItem *item = static_cast<NoteListItem*>(index.internalPointer());

    int notebookId = item->note()->notebook();
    QVector<int> tagIds = item->note()->tags();

    bool passed_notebook_check = false;
    bool passed_tag_check      = false;

    if ( m_notebook_filter.length() == 0 )
        passed_notebook_check = true;

    if ( m_tag_filter.length() == 0 )
        passed_tag_check = true;

    for ( Notebook *n : m_notebook_filter )
        if ( notebookId == n->id() )
            passed_notebook_check = true;

    for ( Tag *t : m_tag_filter )
        if ( tagIds.contains(t->id()) )
            passed_tag_check = true;

    return passed_notebook_check && passed_tag_check;
}

void NoteListProxyModel::clearFilter()
{
    m_notebook_filter.clear();
    m_tag_filter.clear();
    setFilterRegExp(QRegExp(""));
}

void NoteListProxyModel::addNotebookToFilter(Notebook *notebook)
{
    m_notebook_filter.append(notebook);
    setFilterRegExp(QRegExp(""));
}

void NoteListProxyModel::addTagToFilter(Tag *tag)
{
    m_tag_filter.append(tag);
    setFilterRegExp(QRegExp(""));
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

NoteListItem *NoteListProxyModel::item(int row)
{
	QModelIndex i = index(row,0);
	if ( !i.isValid() ) return nullptr;

    return static_cast<NoteListItem*>( mapToSource(i).internalPointer() );
}

NoteListItemWidget *NoteListProxyModel::getWidget(const int row, Note *note)
{
    NoteListItemWidget *item;
    if ( m_notelistitemwidgets.contains(row) ) {
        item = m_notelistitemwidgets.value(row);
        item->setNote(note);
    } else {
        item = new NoteListItemWidget(note);
        m_notelistitemwidgets.insert(row, item);
    }
    qDebug() << "Reporting...size is" << m_notelistitemwidgets.size();
    return item;
}
