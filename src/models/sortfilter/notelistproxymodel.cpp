#include "notelistproxymodel.h"
#include "../../meta/db/notedatabase.h"
#include "../notelistmodel.h"
#include <QStandardItemModel>

#include <QDebug>

NoteListProxyModel::NoteListProxyModel(QListView *view, Database *db) :
  QSortFilterProxyModel(),
  m_view(view),
  m_db(db)
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

  if ( !m_view->indexWidget(index) ) {
    QWidget *widget = new NoteListItemWidget(item->note());
    m_view->setIndexWidget( index, widget );
    item->setWidget(widget);
    if ( m_view->currentIndex() == index )
      item->setSelectedStyle(true);
  }

  return QVariant();
}

void NoteListProxyModel::setSortingMethod(int sortingMethod)
{
  m_sortingMethod = sortingMethod;
}

void NoteListProxyModel::invalidateFilter()
{
  QSortFilterProxyModel::invalidateFilter();
  emit invalidatedFilter();
}

bool NoteListProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent) const
{
  if ( m_filter_out_everything )
    return false;

  QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
  NoteListItem *item = static_cast<NoteListItem*>(index.internalPointer());
  Note *note = item->note();

  int curNotebookId = note->notebook();
  QVector<int> tagIds = note->tags();

  bool passed_favorite_check = false;
  bool passed_trashed_check  = false;
  bool passed_notebook_check = false;
  bool passed_tag_check      = false;

  ////////////////////////
  /// FAVORITES FILTER ///
  ////////////////////////
  if ( m_favorites_filter == FavoritesFilterDisabled )
    passed_favorite_check = true;
  else if ( m_favorites_filter == FavoritesOnly )
    passed_favorite_check = note->favorited() ? true : false;
  else if ( m_favorites_filter == FavoritesExclude )
    passed_favorite_check = note->favorited() ? false : true;
  if ( !passed_favorite_check )
    return false;

  //////////////////////
  /// TRASHED FILTER ///
  //////////////////////
  if ( m_trashed_filter == TrashBoth )
    passed_trashed_check = true;
  else if ( m_trashed_filter == TrashHidden )
    passed_trashed_check = note->trashed() ? false : true;
  else if ( m_trashed_filter == TrashOnly )
    passed_trashed_check = note->trashed() ? true : false;
  if ( !passed_trashed_check )
    return false;

  ///////////////////////
  /// NOTEBOOK FILTER ///
  ///////////////////////
  if ( m_notebook_filter.length() == 0 )
    passed_notebook_check = true;

  if ( m_tag_filter.length() == 0 )
    passed_tag_check = true;

  for ( Notebook *n : m_notebook_filter ) {
    // If notebook is not in notebookDatabase (probably deleted), skip this index.
    if (!m_db->notebookDatabase()->listRecursively().contains(n))
      continue;
    QVector<Notebook*> childNotebooks = n->recurseChildren();
    QVector<int> notebookIds = {n->id()};
    for (Notebook *child : childNotebooks)
      notebookIds.append( child->id() );
    if ( notebookIds.contains(curNotebookId) )
      passed_notebook_check = true;
  }
  if ( !passed_notebook_check )
    return false;

  //////////////////
  /// TAG FILTER ///
  //////////////////
  for ( Tag *t : m_tag_filter )
    if ( tagIds.contains(t->id()) )
      passed_tag_check = true;

  if ( !passed_tag_check )
    return false;

  return true;
}

void NoteListProxyModel::clearFilter(bool invalidate)
{
  m_favorites_filter = FavoritesFilterDisabled;
  m_trashed_filter = TrashHidden;
  m_notebook_filter.clear();
  m_tag_filter.clear();
  m_filter_out_everything = false;
  if ( invalidate )
    invalidateFilter();
}

QVector<Notebook*> NoteListProxyModel::notebookFilter() const {
  return m_notebook_filter;
}

QVector<Tag*> NoteListProxyModel::tagFilter() const {
  return m_tag_filter;
}

int NoteListProxyModel::favoritesFilter() const {
  return m_favorites_filter;
}

int NoteListProxyModel::trashedFilter() const {
  return m_trashed_filter;
}

void NoteListProxyModel::filterOutEverything(bool shouldFilterOutEverything)
{
  m_filter_out_everything = shouldFilterOutEverything;
  invalidateFilter();
}

void NoteListProxyModel::addNotebookToFilter(Notebook *notebook)
{
  m_notebook_filter.append(notebook);
  invalidateFilter();
}

void NoteListProxyModel::addTagToFilter(Tag *tag)
{
  m_tag_filter.append(tag);
  invalidateFilter();
}

void NoteListProxyModel::setFavoritesFilterMode(int filterMode) {
  m_favorites_filter = filterMode;
  invalidateFilter();
}

void NoteListProxyModel::setTrashedFilter(int trashedFilter) {
  m_trashed_filter = trashedFilter;
  invalidateFilter();
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
