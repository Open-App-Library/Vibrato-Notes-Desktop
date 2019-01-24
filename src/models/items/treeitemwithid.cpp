#include "treeitemwithid.h"

TreeItemWithID::TreeItemWithID(QString label, QUuid syncHash) :
    QTreeWidgetItem(),
    m_sync_hash(syncHash)
{
    setText(0, label);
}

QUuid TreeItemWithID::syncHash() const
{
    return m_sync_hash;
}

void TreeItemWithID::setSyncHash(QUuid syncHash)
{
    m_sync_hash = syncHash;
}
