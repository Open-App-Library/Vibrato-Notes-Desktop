#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QList>
#include "../notebook.h"

class NotebookDatabase
{
public:
    NotebookDatabase();
    QList<Notebook *> list() const;
    QList<Notebook *> list_recursively(const Notebook *notebook=nullptr) const;

    void addNotebook(Notebook *notebook);
    void addNotebook(Notebook *notebook, Notebook *parent);

    void removeNotebook(int index);
    void clearNotebooks();

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyNotebooks();

private:
    QList<Notebook*> m_list;

};

#endif // NOTEBOOKDATABASE_H
