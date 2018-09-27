#include <QDebug>
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

int NotebookDatabase::size() const
{
    return m_list.size();
}


QList<Notebook *> NotebookDatabase::list_recursively() const
{
    return list_recursively(m_list);
}

QList<Notebook*> NotebookDatabase::list_recursively(const QList<Notebook*> notebookList) const
{
    QList<Notebook*> the_list;
    for (int i = 0; i < notebookList.size(); i++) {
        the_list.append(notebookList[i]);
        QList<Notebook*> the_children = list_recursively(notebookList[i]->children());
        for (int j = 0; j < the_children.size(); j++) {
            the_list.append(the_children[j]);
        }
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

void NotebookDatabase::jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent)
{
    int     notebook_id    = notebookObj.value("id").toInt();
    QString notebook_title = notebookObj.value("title").toString();
    QJsonArray children = notebookObj["children"].toArray();
    Notebook *newNotebook;
    newNotebook = new Notebook;
    newNotebook->setId(notebook_id);
    newNotebook->setTitle(notebook_title);

    if (parent == nullptr) {           // No parent? Add to main database list
        m_list.append(newNotebook);
    } else {                           // else, Add to the children list of parent
        parent->addChild(newNotebook); // Do note that addChild also modifies newNotebook's parent object automatically.
    }
    for (int i = 0; i < children.size(); i++) {
        jsonObjectToNotebookList(children[i].toObject(), newNotebook);
    }
}

void NotebookDatabase::loadJSON(QJsonDocument jsonDocument)
{
    QJsonArray notebookArray = jsonDocument.array();
    for (int i = 0; i < notebookArray.size(); i++) {
        jsonObjectToNotebookList( notebookArray[i].toObject() );
    }
}

void NotebookDatabase::loadDummyNotebooks()
{
    QJsonDocument notebooks = fileToQJsonDocument(":/dummy/notebooks.json");
    loadJSON(notebooks);
}
