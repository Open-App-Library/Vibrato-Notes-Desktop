#include "listitemwithid.h"

ListItemWithID::ListItemWithID(QString label, int id) :
    QListWidgetItem(),
    m_id(id)
{
    setText(label);
}

int ListItemWithID::id() const
{
    return m_id;
}

void ListItemWithID::setID(int id)
{
    m_id = id;
}
