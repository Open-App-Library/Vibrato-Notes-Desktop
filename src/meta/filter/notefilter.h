#ifndef NOTEFILTER_H
#define NOTEFILTER_H
#include <QVector>
#include <QDate>
#include <QDebug>
#include "../note.h"
#include "../db/notedatabase.h"

#define FILTER_NOTEBOOK_ID 1
#define FILTER_TAG_ID      2

typedef QVector<Note*> noteFilterList;

class NoteFilter
{
public:
    NoteFilter(NoteDatabase *database);

    noteFilterList filter(int filterType, QVariant value, noteFilterList additionalFilter={});

    // TODO: Advanced Filters
    // filterList filterByDateRange(QDate start, QDate end);

private:
    NoteDatabase *m_database;
    noteFilterList notebook_filter(int notebook_id);
};

#endif // NOTEFILTER_H
