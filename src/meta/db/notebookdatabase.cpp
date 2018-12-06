#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notebookdatabase.h"
#include <helper-io.hpp>
#include <QMessageBox>

NotebookDatabase::NotebookDatabase()
{

}

QVector<Notebook *> NotebookDatabase::list() const
{
  return m_list;
}

int NotebookDatabase::size() const
{
  return m_list.size();
}


QVector<Notebook *> NotebookDatabase::listRecursively() const
{
  return listRecursively(m_list);
}

QVector<Notebook*> NotebookDatabase::listRecursively(const QVector<Notebook*> notebookList) const
{
  QVector<Notebook*> the_list;
  for (int i = 0; i < notebookList.size(); i++) {
    the_list.append(notebookList[i]);
    QVector<Notebook*> the_children = listRecursively(notebookList[i]->children());
    for (int j = 0; j < the_children.size(); j++) {
      the_list.append(the_children[j]);
    }
  }
  return the_list;
}

int NotebookDatabase::getNewNotebookId()
{
  // TODO: An actually effective way to safely get a new notebook value
  // In the future I will add an effective. It will get an id by following
  // these steps:
  // 1.) If user is connected to cloud, try to get fresh ID from cloud.
  // 2.) If not, use id of the list length + 1.
  // 3.) Test if that id doesn't actually exist. If it does, increment by
  //     one and try again.
  return m_list.length()+1;
}

Notebook *NotebookDatabase::addNotebook(QString title, Notebook *parent, QVector<Notebook*> children)
{
  Notebook *notebook = new Notebook(getNewNotebookId(), title, parent, children);
  if (parent != nullptr)
    parent->addChild(notebook);
  addNotebook(notebook);
  return notebook;
}

void NotebookDatabase::addNotebook(Notebook *notebook)
{
  if (notebook->parent() == nullptr)
    m_list.append(notebook);
  connect(notebook, &Notebook::notebookChanged,
          this, &NotebookDatabase::notebookChanged_slot);
  connect(notebook, &Notebook::notebookIDChanged,
          this, &NotebookDatabase::notebookIDChanged_slot);
  connect(notebook, &Notebook::notebookTitleChanged,
          this, &NotebookDatabase::notebookTitleChanged_slot);
  connect(notebook, &Notebook::notebookParentChanged,
          this, &NotebookDatabase::notebookParentChanged_slot);
  connect(notebook, &Notebook::notebookChildrenChanged,
          this, &NotebookDatabase::notebookChildrenChanged_slot);
  emit notebookAdded(notebook);
}

void NotebookDatabase::removeNotebook(int notebookID)
{
  removeNotebook( findNotebookWithID(notebookID) );
}

void NotebookDatabase::removeNotebook(Notebook *notebook)
{
  if (notebook->id() == -1) {
    QMessageBox::warning(nullptr,
                         "Cannot delete 'Default Notebook'",
                         "You just tried to delete the default notebook. You may not delete this notebook as it acts as a 'fallback' notebook for notes without a notebook.");
    return;
  }

  QString title= "Delete notes too?";
  QString msg  = "You have requested to delete your '"+notebook->title()+"' notebook. Would you like to delete all of its notes?";

  if ( notebook->children().length() > 0 )
    msg+="<br><br><strong>WARNING!</strong> This notebook also contains child notebooks. Those will be deleted as well.";

  QMessageBox::StandardButton prompt;
  prompt = QMessageBox::question(nullptr, title, msg,
                                 QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
  if ( prompt == QMessageBox::Cancel )
    return;

  if ( prompt == QMessageBox::Yes ) {
    // Delete notes too.
    qDebug("Deleting notes too...");
  } else {
    // Assign notes to 'Default Notebook'
    qDebug("Assigning notes to default notebook");
  }

  if ( !notebook->parent() ) {
    for (int i=0; i < m_list.length(); i++)
      if ( m_list.at(i) == notebook )
        m_list.removeAt(i);
  }
  else
    notebook->parent()->removeChild(notebook);

  delete notebook;
  emit notebookRemoved( notebook->id() );
}

void NotebookDatabase::clearNotebooks()
{
  for (int i = m_list.size()-1; i >= 0; i--) {
    Notebook *notebook = m_list[i];
    emit notebookRemoved( notebook->id() );
    delete notebook;
    m_list.removeAt(i);
  }
}

Notebook *NotebookDatabase::findNotebookWithID(int id)
{
  for (Notebook *notebook : listRecursively()) {
    if (notebook->id() == id)
      return notebook;
  }
  return nullptr;
}

void NotebookDatabase::jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent)
{
  int     notebook_id    = notebookObj.value("id").toInt();
  QString notebook_title = notebookObj.value("title").toString();
  QJsonArray children = notebookObj["children"].toArray();
  Notebook *newNotebook;
  newNotebook = new Notebook(notebook_id, notebook_title);

  if (parent == nullptr) {           // No parent? Add to main database list
    addNotebook(newNotebook);
  } else {                           // else, Add to the children list of parent
    parent->addChild(newNotebook); // Do note that addChild also modifies newNotebook's parent object automatically.
  }
  for (int i = 0; i < children.size(); i++) {
    jsonObjectToNotebookList(children[i].toObject(), newNotebook);
  }
}

void NotebookDatabase::notebookChanged_slot(Notebook *notebook)
{
  emit notebookChanged(notebook);
}

void NotebookDatabase::notebookIDChanged_slot(Notebook *notebook)
{
  emit notebookIDChanged(notebook);
}

void NotebookDatabase::notebookTitleChanged_slot(Notebook *notebook)
{
  emit notebookTitleChanged(notebook);
}

void NotebookDatabase::notebookParentChanged_slot(Notebook *notebook)
{
  emit notebookParentChanged(notebook);
}

void NotebookDatabase::notebookChildrenChanged_slot(Notebook *notebook)
{
  emit notebookChildrenChanged(notebook);
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
  clearNotebooks();
  QJsonDocument notebooks = HelperIO::fileToQJsonDocument(":/dummy/notebooks.json");
  loadJSON(notebooks);
}
