#ifndef TREEITEMWITHID_H
#define TREEITEMWITHID_H
#include <QTreeWidgetItem>

#define TREE_ITEM_UNKNOWN -1

class TreeItemWithID : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    TreeItemWithID(QString label, int id);
    int id() const;

public slots:
    void setID(int id);
private:
    int m_id = TREE_ITEM_UNKNOWN;
};

#endif // TREEITEMWITHID_H
