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
public:
  NoteListProxyModel(QListView *view, Database *db);

  QVariant data(const QModelIndex &index, int role) const override;

  enum SortingMethods {DateCreated, DateModified};
  void setSortingMethod(int sortingMethod);

  // Filtering functions
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
  bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
  void clearFilter(bool invalidate=true);

  void filterOutEverything(bool shouldFilterOutEverything=true);
  void addNotebookToFilter(Notebook *notebook);
  void addTagToFilter(Tag *tag);

  NoteListItem *item(int row);


private:
  QListView *m_view;
  int m_sortingMethod=DateModified;
  Database *m_db;

  bool m_filter_out_everything=false;
  QVector<Notebook*> m_notebook_filter;
  QVector<Tag*> m_tag_filter;

};

#endif // NOTELISTPROXYMODEL_H
