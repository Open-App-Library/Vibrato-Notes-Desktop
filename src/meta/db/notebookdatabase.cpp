#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notebookdatabase.h"
#include <helper-io.hpp>
#include <QMessageBox>

NotebookDatabase::NotebookDatabase(NoteDatabase *noteDatabase) :
  m_noteDatabase(noteDatabase)
{
  addNotebook(new Notebook(NOTEBOOK_DEFAULT_NOTEBOOK_ID,
                           "Default Notebook"));
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

int NotebookDatabase::getUniqueNotebookID(int start, QVector<Notebook*> notebookList, Notebook *notebookToSync)
 {
  // If notebookList is empty AND we have at least one notebook availible.
  if (notebookList.length() == 0 && list().length() > 0)
    notebookList = listRecursively();

  // Loop through notebook list. If ID == start (The current index we're testing)
  // that means that the ID is NOT unique and we must find a new one. Increment
  // by one!
  for ( Notebook* n : notebookList )
    if ( n->id() == start ) // Can't use this ID; it's taken.
      return getUniqueNotebookID(start+1, notebookList);

  // If we made it this far in the function, it means that the ID we are testing
  // is indeed unique.
  return start;
}

int NotebookDatabase::getUniqueNotebookID(Notebook *notebookToSync)
{
  // TODO: Create a graphic called 'The Sync Hash' that
  //       explains how cloud-syncing works.

  // Search for a unique notebook ID.
  // If you have notebook ID #1 and create a new notebook, the next ID will
  // most likely be two. For this reason, we set the start index to search
  // for a new ID to the list length + 1 as it is likely unused.
  //   Tip: If the *notebookToSync parameter is passed, and the user has
  //        connected the cloud.
  QVector<Notebook*> notebookList = listRecursively();
  return getUniqueNotebookID(notebookList.length()+1, notebookList, notebookToSync);
}

Notebook *NotebookDatabase::addNotebook(QString title, Notebook *parent, QVector<Notebook*> children)
{
  Notebook *notebook = new Notebook(getUniqueNotebookID(), title, parent, children);
  if (parent != nullptr && parent->id() == NOTEBOOK_DEFAULT_NOTEBOOK_ID)
    return nullptr;
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
  // If the user is trying to delete the default notebook, open a warning message and return.
  if (notebook->id() == NOTEBOOK_DEFAULT_NOTEBOOK_ID) {
    QMessageBox::warning
      (nullptr,
       "Cannot delete 'Default Notebook'",
       "You just tried to delete the default notebook. You may not delete this notebook as it acts as a 'fallback' notebook for notes without a notebook."
       );
    return;
  }

  QVector<int> the_ids = {notebook->id()};
  QVector<Note*> effected_notes;
  QString title= "Delete notes too?";
  QString msg  = "You have requested to delete your '"+notebook->title()+"' notebook. Would you like to delete all of its notes?";
  QString childNotebookMsg = "<strong>WARNING!</strong> This notebook also contains sub-notebooks. Those will be deleted as well.";
  QMessageBox::StandardButton prompt;
  int notebookChildCount = notebook->children().length();

  // If notebook has children, attach a warning message that the children notebooks will also be deleted.
  if ( notebookChildCount > 0 ) {
    for (Notebook* child : notebook->recurseChildren() )
      the_ids.append(child->id());
  }

  effected_notes = m_noteDatabase->findNotesWithNotebookIDs(the_ids);

  // This block of code determines if we need to display a warning prompt to the user
  // and if so, what data to prompt the user for.
  // * If no notes have to be deleted and the notebook has no children, we DON'T prompt the
  //   user and delete the notebook without warning.
  // * If no notes have to be deleted and the ntoebook has children, we preset a warning
  //   that children notebooks will be deleted.
  // * If notes have to be deleted and the notebook has children, we will prompt the user
  //   both about whether they want the notes deleted and that children will be deleted.
  // * If notes have to be deleted and the notebook does not have children, we will only
  //   prompt about notes having to be deleted.
  if ( effected_notes.length() == 0 ) { // No notes in deleted notebook.
    if ( notebookChildCount == 0) {
      prompt = QMessageBox::Yes;
    } else {
      prompt = QMessageBox::question(nullptr, "Sub-Notebook deletion warning", childNotebookMsg + " Are you okay with that?",
                                     QMessageBox::Yes|QMessageBox::Cancel);
    }
  } else {
    if ( notebookChildCount > 0 )
      msg += QString("<br><br>%1").arg( childNotebookMsg );
    prompt = QMessageBox::question(nullptr, title, msg,
                                   QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
  }

  // If user clicks cancel or presses 'x' button on Window, cancel the whole operation.
  if ( prompt == QMessageBox::Cancel )
    return;

  // If prompt is 'Yes', delete notes. Otherwise, just set their notebook ID to the 'Default Notebook'.
  if ( prompt == QMessageBox::Yes ) {
    m_noteDatabase->removeNotes(effected_notes);
  }
  else // Assign notes to 'Default Notebook' instead.
    for ( Note *note : effected_notes )
      note->setNotebook(NOTEBOOK_DEFAULT_NOTEBOOK_ID, false);

  // If notebook does not have parent, delete from main m_list.
  if ( !notebook->parent() ) {
    for (int i=0; i < m_list.length(); i++)
      if ( m_list.at(i) == notebook )
        m_list.removeAt(i);
  }
  // Otherwise, remove the notebook from its parent notebook.
  else
    notebook->parent()->removeChild(notebook);

  // Free memory and emit a notebooksRemoved event.
  delete notebook;
  emit notebooksRemoved( the_ids );
}

void NotebookDatabase::clearNotebooks()
{
  for (int i = m_list.size()-1; i >= 0; i--) {
    Notebook *notebook = m_list[i];
    if (notebook->id() == -1)
      continue;
    QVector<int> the_ids = {notebook->id()};
    for ( Notebook *child : notebook->recurseChildren() )
      the_ids.append( child->id() );
    emit notebooksRemoved( the_ids );
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
