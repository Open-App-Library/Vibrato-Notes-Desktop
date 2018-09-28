#ifndef BASICTREEITEM_H
#define BASICTREEITEM_H
#include <QVariant>
#include "../../meta/notebook.h"
#include "../../meta/tag.h"

typedef union
{
    short     type;
    Notebook *notebook;
    Tag      *tag;
} NotebookOrTag;

class BasicTreeItem
{
public:
    explicit BasicTreeItem(const QString label, BasicTreeItem *parent = nullptr);
    ~BasicTreeItem();

    QString   label()  const;
    void      setLabel(QString label);
    NotebookOrTag object() const;
    void setObjectNotebook(Notebook *notebook);
    void setObjectTag(Tag *tag);

    BasicTreeItem *getChild(int index) const;
    BasicTreeItem *appendChild(BasicTreeItem *child);
    QVector<BasicTreeItem*> recurseChildren() const;
    int childCount() const;

    BasicTreeItem *parentItem();
    void           setParent(BasicTreeItem *parent);

    int row() const;

private:
    QString m_label;
    NotebookOrTag m_object;
    QList<BasicTreeItem*> m_childItems;
    BasicTreeItem *m_parentItem;
};

#endif // BASICTREEITEM_H
