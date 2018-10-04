#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QVector>
#include "../notebook.h"

class NotebookDatabase
{
public:
    NotebookDatabase();
    QVector<Notebook *> list() const;
    int               size() const;
    QVector<Notebook *> list_recursively() const;
    QVector<Notebook *> list_recursively(const QVector<Notebook*> notebookList) const;

    void addNotebook(Notebook *notebook);
    void addNotebook(Notebook *notebook, Notebook *parent);

    void removeNotebook(int index);
    void clearNotebooks();

    void loadJSON(QJsonDocument jsonDocument);

    void loadDummyNotebooks();

    void jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent=nullptr);

private:
    QVector<Notebook*> m_list;

};

#endif // NOTEBOOKDATABASE_H
