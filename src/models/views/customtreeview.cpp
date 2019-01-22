#include "customtreeview.h"
#include <QMouseEvent>

CustomTreeView::CustomTreeView(QWidget *parent) :
  QTreeView(parent)
{
  setEditTriggers(QTreeView::EditKeyPressed);

  setDragEnabled(true);
  setAcceptDrops(true);
  setDragDropMode(QAbstractItemView::InternalMove);
}

void CustomTreeView::mousePressEvent(QMouseEvent *event)
{
  if ( event->button() == Qt::LeftButton )
    QTreeView::mousePressEvent(event);
  else {
    QPoint pos = event->pos();
    QPersistentModelIndex index = indexAt(pos);
    emit pressed(index);
  }
}
