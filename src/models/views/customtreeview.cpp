#include "customtreeview.h"
#include <QMouseEvent>

CustomTreeView::CustomTreeView(QWidget *parent) :
  QTreeView(parent)
{
  this->setEditTriggers(QTreeView::EditKeyPressed);
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
