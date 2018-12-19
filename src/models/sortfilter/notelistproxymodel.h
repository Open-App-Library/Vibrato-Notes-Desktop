/*
 * NoteListProxyModel
 * Intended for out NoteList.
 * It allows us to sort by date, ascending or descending
 */

#ifndef NOTELISTPROXYMODEL_H
#define NOTELISTPROXYMODEL_H
#include <QSortFilterProxyModel>
#include <QListView>
#include "../items/notelistitem.h"
#include "../items/notelistitemwidget.h"
#include "../../meta/db/database.h"

class NoteListProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  NoteListProxyModel(QListView *view, Database *db);

  QVariant data(const QModelIndex &index, int role) const override;

  enum SortingMethods {DateCreated, DateModified};
  // With this enum, you can choose whether you want the favorites filter disable,
  // to display favorites only or exclude favorites.
  enum FavoritesMethods {FavoritesFilterDisabled, FavoritesOnly, FavoritesExclude};
  // Either hide trash (default), show only trash, or show both trash and not-trash.
  enum TrashMethods {TrashHidden, TrashOnly, TrashBoth};

  void setSortingMethod(int sortingMethod);

  void invalidateFilter();

  // Filtering functions
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
  bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
  void clearFilter(bool invalidate=true);

  QVector<Notebook*> notebookFilter() const;
  QVector<Tag*> tagFilter() const;
  int favoritesFilter() const;
  int trashedFilter() const;

  void filterOutEverything(bool shouldFilterOutEverything=true);
  void addNotebookToFilter(Notebook *notebook);
  void addTagToFilter(Tag *tag);
  void setFavoritesFilterMode(int filterMode);
  void setTrashedFilter(int trashedFilter);

  NoteListItem *item(int row);

signals:
  void invalidatedFilter();

private:
  QListView *m_view;
  int m_sortingMethod=DateModified;
  Database *m_db;

  bool m_filter_out_everything=false;
  QVector<Notebook*> m_notebook_filter;
  QVector<Tag*> m_tag_filter;
  int m_favorites_filter=FavoritesFilterDisabled;
  int m_trashed_filter=TrashHidden;

};

#endif // NOTELISTPROXYMODEL_H
