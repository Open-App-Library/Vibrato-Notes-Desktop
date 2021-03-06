#include "notelistproxymodel.h"
#include "../../meta/db/notedatabase.h"
#include "../notelistmodel.h"
#include <QStandardItemModel>

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <fts_fuzzy_match.hpp>
#include <helper-io.hpp>
#include <QDebug>

NoteListProxyModel::NoteListProxyModel(QListView *view, Database *db) :
  QSortFilterProxyModel(),
  m_view(view),
  m_db(db)
{
  setDynamicSortFilter(true);

  m_delegate = new NoteItemDelegate(m_view, this);
  m_view->setItemDelegate(m_delegate);

  connect(m_db->noteDatabase(), &NoteDatabase::noteChanged,
          this, &NoteListProxyModel::noteChanged);
}

QVariant NoteListProxyModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

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

  QUuid curNotebookSyncHash = note->notebook();
  QVector<QUuid> tagSyncHashes = note->tags();

  bool passed_favorite_check = false;
  bool passed_trashed_check  = false;
  bool passed_notebook_check = false;
  bool passed_tag_check      = false;
  bool passed_search_check   = false;

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
    QVector<QUuid> notebookSyncHasehs = {n->syncHash()};
    for (Notebook *child : childNotebooks)
      notebookSyncHasehs.append( child->syncHash() );
    if ( notebookSyncHasehs.contains(curNotebookSyncHash) )
      passed_notebook_check = true;
  }
  if ( !passed_notebook_check )
    return false;

  //////////////////
  /// TAG FILTER ///
  //////////////////
  for ( Tag *t : m_tag_filter )
    if ( tagSyncHashes.contains(t->syncHash()) )
      passed_tag_check = true;

  if ( !passed_tag_check )
    return false;

  /////////////////////
  /// Search FILTER ///
  /////////////////////
  if (m_search_filter == SearchOn) {
    char *query = HelperIO::QString2CString(m_searchQuery);
    char *title = HelperIO::QString2CString(note->title());
    passed_search_check = fts::fuzzy_match_simple(query, title);
    delete query;
    delete title;
  }
  else
    passed_search_check = true;
  if (!passed_search_check)
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
  m_search_filter = SearchOff;
  m_searchQuery = "";
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

void NoteListProxyModel::setSearchQuery(QString searchQuery, int searchFilterMode) {
  m_searchQuery = searchQuery;
  m_search_filter = searchFilterMode;
  //invalidateFilter();
  invalidate();
}

bool NoteListProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
  if ( !left.isValid() ) return true;
  if ( !right.isValid() ) return false;
  NoteListItem *item1 = static_cast<NoteListItem*>(left.internalPointer());
  NoteListItem *item2 = static_cast<NoteListItem*>(right.internalPointer());

  if ( m_search_filter == SearchOn ) {
    int item1_score = 0;
    int item2_score = 0;

    char *query = HelperIO::QString2CString(m_searchQuery);
    char *item1_title = HelperIO::QString2CString(item1->note()->title());
    char *item2_title = HelperIO::QString2CString(item2->note()->title());

    // Get the ranking score of item1 & item2 and store them in *_score variables.
    fts::fuzzy_match(query, item1_title, item1_score);
    fts::fuzzy_match(query, item2_title, item2_score);

    delete query;
    delete item1_title;
    delete item2_title;

    return item1_score < item2_score;
  }

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

void NoteListProxyModel::noteChanged(Note* note) {
  QModelIndex theIndex = QModelIndex();

  for (int i=0; i<rowCount(); i++) {
    QModelIndex curIndex = index(i, 0);
    QModelIndex realCurIndex = mapToSource(curIndex);
    NoteListItem *item = static_cast<NoteListItem*>( realCurIndex.internalPointer() );
    if (item->note() == note)
      theIndex = curIndex;
  }

  // if (theIndex.isValid())
  //   m_view->viewport()->repaint();
  emit dataChanged(theIndex, theIndex);
}
