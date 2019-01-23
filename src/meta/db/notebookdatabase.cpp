#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "notebookdatabase.h"
#include <helper-io.hpp>
#include <QMessageBox>

NotebookDatabase::NotebookDatabase(SQLManager *sqlManager, NoteDatabase *noteDatabase) :
  m_sqlManager(sqlManager),
  m_noteDatabase(noteDatabase)
{
  addNotebook(new Notebook(nullptr, "Default Notebook"));
  loadSQL();
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

void NotebookDatabase::addNotebook(Notebook *notebook)
{
  if (notebook->parent() == nullptr)
    m_list.append(notebook);
  connectNotebook(notebook);
  emit added(notebook);
}

void NotebookDatabase::removeNotebook(QUuid syncHash)
{
  removeNotebook( findNotebookWithSyncHash(syncHash) );
}

void NotebookDatabase::removeNotebook(Notebook *notebook)
{
  // If the user is trying to delete the default notebook, open a warning message and return.
  if (notebook->defaultNotebook() ) {
    QMessageBox::warning
      (nullptr,
       "Cannot delete 'Default Notebook'",
       "You just tried to delete the default notebook. You may not delete this notebook as it acts as a 'fallback' notebook for notes without a notebook."
       );
    return;
  }

  QVector<QUuid> the_sync_hashes = {notebook->syncHash()};
  QVector<Note*> effected_notes;
  QString title= "Delete notes too?";
  QString msg  = "You have requested to delete your '"+notebook->title()+"' notebook. Would you like to delete all of its notes?";
  QString childNotebookMsg = "<strong>WARNING!</strong> This notebook also contains sub-notebooks. Those will be deleted as well.";
  QMessageBox::StandardButton prompt;
  int notebookChildCount = notebook->children().length();

  // If notebook has children, attach a warning message that the children notebooks will also be deleted.
  if ( notebookChildCount > 0 ) {
    for (Notebook* child : notebook->recurseChildren() )
      the_sync_hashes.append(child->syncHash());
  }

  effected_notes = m_noteDatabase->findNotesWithNotebookIDs(the_sync_hashes);

  // This block of code determines if we need to display a warning prompt to the user
  // and if so, what data to prompt the user for.
  // * If no notes have to be deleted and the notebook has no children, we DON'T prompt the
  //   user and delete the notebook without warning.
  // * If no notes have to be deleted and the notebook has children, we present a warning
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
      note->setNotebook(nullptr, false);

  // If notebook does not have parent, delete from main m_list.
  if ( !notebook->parent() ) {
    for (int i=0; i < m_list.length(); i++)
      if ( m_list.at(i) == notebook )
        m_list.removeAt(i);
  }
  // Otherwise, remove the notebook from its parent notebook.
  else
    notebook->parent()->removeChild(notebook);

  m_sqlManager->deleteNotebook(notebook);

  // Free memory and emit a notebooksRemoved event.
  delete notebook;
  emit removed( the_sync_hashes );
}

void NotebookDatabase::clearNotebooks()
{
  for (int i = m_list.size()-1; i >= 0; i--) {
    Notebook *notebook = m_list[i];
    if (notebook->defaultNotebook())
      continue;
    QVector<QUuid> the_ids = {notebook->syncHash()};
    for ( Notebook *child : notebook->recurseChildren() )
      the_ids.append( child->syncHash() );
    emit removed( the_ids );
    delete notebook;
    m_list.removeAt(i);
  }
}

Notebook *NotebookDatabase::findNotebookWithSyncHash(QUuid syncHash)
{
  for (Notebook *notebook : listRecursively()) {
    if (notebook->syncHash() == syncHash)
      return notebook;
  }
  return nullptr;
}

void NotebookDatabase::loadSQL()
{
  QVector<Notebook*> notebooks = m_sqlManager->notebooks();
  QVector<Notebook*> notebooks_to_connect;
  for (Notebook *notebook : notebooks) {
    addNotebook(notebook);
    notebooks_to_connect.append( listRecursively(notebook->children()) );
  }

  for (Notebook *notebook : notebooks_to_connect)
    connectNotebook(notebook);
}

void NotebookDatabase::changed_slot(Notebook *notebook)
{
  m_sqlManager->updateNotebookToDB(notebook);
  emit changed(notebook);
}

void NotebookDatabase::syncHashChanged_slot(Notebook *notebook)
{
  emit syncHashChanged(notebook);
}

void NotebookDatabase::titleChanged_slot(Notebook *notebook)
{
  emit titleChanged(notebook);
}

void NotebookDatabase::parentChanged_slot(Notebook *notebook)
{
  // If the notebook does not have a parent and it is not in the root list
  // add it to the root list.
  if ( notebook->parent() == nullptr &&
       !m_list.contains(notebook) ) {
    m_list.append(notebook);
  }
  emit parentChanged(notebook);
}

void NotebookDatabase::childrenChanged_slot(Notebook *notebook)
{
  emit childrenChanged(notebook);
}

void NotebookDatabase::connectNotebook(Notebook *notebook)
{
  connect(notebook, &Notebook::changed,
          this, &NotebookDatabase::changed_slot);
  connect(notebook, &Notebook::syncHashChanged,
          this, &NotebookDatabase::syncHashChanged_slot);
  connect(notebook, &Notebook::titleChanged,
          this, &NotebookDatabase::titleChanged_slot);
  connect(notebook, &Notebook::parentChanged,
          this, &NotebookDatabase::parentChanged_slot);
  connect(notebook, &Notebook::childrenChanged,
          this, &NotebookDatabase::childrenChanged_slot);
}

void NotebookDatabase::disconnectNotebook(Notebook *notebook)
{
  disconnect(notebook, &Notebook::changed,
             this, &NotebookDatabase::changed_slot);
  disconnect(notebook, &Notebook::syncHashChanged,
             this, &NotebookDatabase::syncHashChanged_slot);
  disconnect(notebook, &Notebook::titleChanged,
             this, &NotebookDatabase::titleChanged_slot);
  disconnect(notebook, &Notebook::parentChanged,
             this, &NotebookDatabase::parentChanged_slot);
  disconnect(notebook, &Notebook::childrenChanged,
             this, &NotebookDatabase::childrenChanged_slot);
}
