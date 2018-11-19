#include "treeitemwithid.h"

TreeItemWithID::TreeItemWithID(QString label, int id) :
    QTreeWidgetItem(),
    m_id(id)
{
    setText(0, label);
}

int TreeItemWithID::id() const
{
    return m_id;
}

void TreeItemWithID::setID(int id)
{
    m_id = id;
}
