#ifndef NOTEFILTER_H
#define NOTEFILTER_H
#include <QVector>
#include "../db/database.h"

#define FILTER_NOTEBOOK_ID 1
#define FILTER_TAG_ID      2

typedef QVector<Note*> noteFilterList;

class NoteFilter
{
public:
    NoteFilter(Database *db);

    noteFilterList notebookFilter(Notebook *notebook, noteFilterList additionalFilter={});
    noteFilterList tagFilter(Tag *tag, noteFilterList additionalFilter={});

    // TODO: Advanced Filters
    // filterList filterByDateRange(QDate start, QDate end, noteFilterList additionalFilter={});

private:
    Database     *m_db;
    noteFilterList notesOfChildren(QVector<Notebook*> children);
};

#endif // NOTEFILTER_H
