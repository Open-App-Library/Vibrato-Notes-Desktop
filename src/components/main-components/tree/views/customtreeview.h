#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H
#include <QTreeView>

class CustomTreeView : public QTreeView
{
public:
  CustomTreeView(QWidget *parent=nullptr);

  void mousePressEvent(QMouseEvent *event) override;
};

#endif // CUSTOMTREEVIEW_H
