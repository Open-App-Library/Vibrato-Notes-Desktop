/*
 * The purpose of this class is to contain all database types
 * in a single struct for convenient access.
 */

#ifndef DATABASE_H
#define DATABASE_H
#include "notedatabase.h"
#include "notebookdatabase.h"
#include "tagdatabase.h"

typedef struct {
    NoteDatabase     *noteDatabase;
    NotebookDatabase *notebookDatabase;
    TagDatabase      *tagDatabase;
} DatabaseObject;

class Database
{
public:
    Database(NoteDatabase *noteDatabase, NotebookDatabase *notebookDatabase, TagDatabase *tagDatabase);
    NoteDatabase     *noteDatabase()     const;
    NotebookDatabase *notebookDatabase() const;
    TagDatabase      *tagDatabase()      const;
private:
    NoteDatabase     *m_note_database;
    NotebookDatabase *m_notebook_database;
    TagDatabase      *m_tag_database;
};

#endif // DATABASE_H
