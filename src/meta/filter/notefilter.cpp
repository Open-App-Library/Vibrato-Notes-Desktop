#include "notefilter.h"

NoteFilter::NoteFilter(Database *db) :
    m_db(db)
{
    // Implementation
}

noteFilterList NoteFilter::notebookFilter(Notebook *notebook, noteFilterList additionalFilter)
{
    noteFilterList list;
    for (int i = 0; i < m_db->noteDatabase()->list().size(); i++) {
        Note *curNote = m_db->noteDatabase()->list()[i];
        if (curNote->notebook() == notebook->id()) {
            list << curNote;
            list << notesOfChildren( notebook->recurseChildren() );
        }
    }
    list.append(additionalFilter);
    return list;
}

noteFilterList NoteFilter::tagFilter(Tag *tag, noteFilterList additionalFilter)
{
    noteFilterList list;
    for (int i = 0; i < m_db->noteDatabase()->list().size(); i++) {
        Note *curNote = m_db->noteDatabase()->list()[i];
        if (curNote->tags().contains( tag->id() )) {
            list << curNote;
        }
    }
    list.append(additionalFilter);
    return list;
}

noteFilterList NoteFilter::notesOfChildren(QVector<Notebook *> children)
{
    noteFilterList list;
    for (int i = 0; i < m_db->noteDatabase()->list().size(); i++) {
        Note *curNote = m_db->noteDatabase()->list()[i];
        for (int j = 0; j < children.size(); j++) {
            int notebook_id = children[j]->id();
            if (curNote->notebook() == notebook_id) {
                list << curNote;
            }
        }
    }
    return list;
}
