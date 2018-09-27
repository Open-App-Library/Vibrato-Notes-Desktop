#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QList>
#include "../notebook.h"

class NotebookDatabase
{
public:
    NotebookDatabase();
    QList<Notebook *> list() const;
    int               size() const;
    QList<Notebook *> list_recursively() const;
    QList<Notebook *> list_recursively(const QList<Notebook*> notebookList) const;

    void addNotebook(Notebook *notebook);
    void addNotebook(Notebook *notebook, Notebook *parent);

    void removeNotebook(int index);
    void clearNotebooks();

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyNotebooks();

    void jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent=nullptr);

private:
    QList<Notebook*> m_list;

};

#endif // NOTEBOOKDATABASE_H
