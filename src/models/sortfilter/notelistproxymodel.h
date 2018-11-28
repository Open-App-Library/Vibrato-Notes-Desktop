/*
 * NoteListProxyModel
 * Intended for out NoteList.
 * It allows us to sort by date, ascending or descending
 */

#ifndef NOTELISTPROXYMODEL_H
#define NOTELISTPROXYMODEL_H
#include <QSortFilterProxyModel>
#include <QListView>

class NoteListProxyModel : public QSortFilterProxyModel
{
public:
    NoteListProxyModel(QListView *view);

    QVariant data(const QModelIndex &index, int role) const;

    enum SortingMethods {DateCreated, DateModified};
    void setSortingMethod(int sortingMethod);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool greaterThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QListView *m_view;
    int m_sortingMethod=DateModified;
};

#endif // NOTELISTPROXYMODEL_H
