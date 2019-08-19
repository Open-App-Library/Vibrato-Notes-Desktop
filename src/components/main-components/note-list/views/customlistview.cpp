#include "customlistview.h"

CustomListView::CustomListView(QWidget *parent) : QListView(parent)
{

}

void CustomListView::mousePressEvent(QMouseEvent *event)
{
    (void) event;
}

void CustomListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);
    emit selectedItemChanged(current, previous);
}
