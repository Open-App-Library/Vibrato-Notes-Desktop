#include "notefilter.h"

NoteFilter::NoteFilter(NoteDatabase *database) :
    m_database(database)
{
    // Implementation
}

noteFilterList NoteFilter::filter(int filterType, QVariant value, noteFilterList additionalFilter)
{
    noteFilterList list;
    switch (filterType) {
    case FILTER_NOTEBOOK_ID:
        list = notebook_filter( value.toInt() );
        break;
    }
    list.append(additionalFilter);
    return list;
}

noteFilterList NoteFilter::notebook_filter(int notebook_id)
{
    noteFilterList list;
    for (int i = 0; i < m_database->list().size(); i++) {
        Note *curNote = m_database->list()[i];
        if (curNote->notebook() == notebook_id) {
            list << curNote;
        }
    }
    return list;
}

