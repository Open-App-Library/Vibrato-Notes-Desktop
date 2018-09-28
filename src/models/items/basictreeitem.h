#ifndef BASICTREEITEM_H
#define BASICTREEITEM_H
#include <QVariant>

class BasicTreeItem
{
public:
    explicit BasicTreeItem(const QString label, BasicTreeItem *parentItem = nullptr);
    ~BasicTreeItem();

    QString label() const;

    BasicTreeItem *getChild(int index);
    void appendChild(BasicTreeItem *child);
    int childCount() const;

    BasicTreeItem *parentItem();

    int row() const;

private:
    QString m_label;
    QList<BasicTreeItem*> m_childItems;
    BasicTreeItem *m_parentItem;
};

#endif // BASICTREEITEM_H
