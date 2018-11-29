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

class NoteListProxyModel : public QSortFilterProxyModel
{
public:
	NoteListProxyModel(QListView *view);

	QVariant data(const QModelIndex &index, int role) const;

	enum SortingMethods {DateCreated, DateModified};
	void setSortingMethod(int sortingMethod);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    void clearFilter();
    void addNotebookToFilter(Notebook *notebook);
    void addTagToFilter(Tag *tag);

	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    NoteListItem *item(int row);


private:
	QListView *m_view;
	int m_sortingMethod=DateModified;

    QVector<Notebook*> m_notebook_filter;
    QVector<Tag*> m_tag_filter;

    QMap<int, NoteListItemWidget*> m_notelistitemwidgets;
    NoteListItemWidget *getWidget(const int row, Note *note);
};

#endif // NOTELISTPROXYMODEL_H
