#ifndef NOTEITEMDELEGATE_H
#define NOTEITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QListView>
#include <QSortFilterProxyModel>

class NoteItemDelegate : public QStyledItemDelegate
{
public:
    NoteItemDelegate(QListView *view, QSortFilterProxyModel *proxyModel);

    QRect getStarRect(const QStyleOptionViewItem &option) const;

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    bool checked = false;
    QListView *m_view;
    QSortFilterProxyModel *m_proxyModel;
};

#endif // MYDELEGATE_H
