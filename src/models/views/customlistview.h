/*
 * CustomListView provides some very minimal tweaks to QListView
 * At the moment, the only tweak it adds is a selectedItemChanged signal.
 */

#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H
#include <QListView>

class CustomListView : public QListView
{
    Q_OBJECT
public:
    CustomListView(QWidget *parent=nullptr);

    // Empty function to prevent mouse press event.
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

signals:
    void selectedItemChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // CUSTOMLISTVIEW_H
