#include "listitemwithid.h"

ListItemWithID::ListItemWithID(QString label, QUuid syncHash) :
    QListWidgetItem(),
    m_sync_hash(syncHash)
{
    setText(label);
}

QUuid ListItemWithID::syncHash() const
{
  return m_sync_hash;
}

void ListItemWithID::setLabel(QString label)
{
  setText(label);
}

void ListItemWithID::setSyncHash(QUuid syncHash)
{
  m_sync_hash = syncHash;
}
