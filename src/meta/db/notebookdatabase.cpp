#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notebookdatabase.h"
#include "../../helper-io.h"

NotebookDatabase::NotebookDatabase()
{

}

QList<Notebook *> NotebookDatabase::list() const
{
    return m_list;
}

QList<Notebook*> NotebookDatabase::list_recursively(const Notebook *notebook) const
{
    QList<Notebook*> the_list;
    if (notebook == nullptr) {
        for (int i = 0; i < m_list.size(); i ++) {
            the_list.append(m_list[i]);
        }

    } else {

    }
    return the_list;
}

void NotebookDatabase::addNotebook(Notebook *notebook)
{
    m_list.append(notebook);
}

void NotebookDatabase::removeNotebook(int index)
{
    delete m_list[index];
    m_list.removeAt(index);
}

void NotebookDatabase::clearNotebooks()
{
    for (int i = m_list.size()-1; i >= 0; i--) {
        removeNotebook(i);
    }
}

void NotebookDatabase::loadJSON(QJsonDocument jsonDocument)
{
    QJsonArray notebookArray = jsonDocument.array();
    for (int i = 0; i < notebookArray.size(); i++) {
        //QJsonObject val = notebookArray[i].toObject();
        //loadNotebookAndChildren(val);
    }
}

void NotebookDatabase::loadDummyNotebooks()
{
    //QJsonDocument notebooks = fileToQJsonDocument(":/dummy/notebooks.json");
}
