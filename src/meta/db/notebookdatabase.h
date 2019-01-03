#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QVector>
#include "../notebook.h"
#include "notedatabase.h"

class NotebookDatabase : public QObject
{
  Q_OBJECT
public:
  NotebookDatabase(NoteDatabase *noteDatabase);
  QVector<Notebook *> list() const;
  int               size() const;
  QVector<Notebook *> listRecursively() const;
  QVector<Notebook *> listRecursively(const QVector<Notebook*> notebookList) const;

  // Functions that will help return a new ID that is unused.
  int getUniqueNotebookID(int start, QVector<Notebook*> notebookList={}, Notebook *notebookToSync=nullptr);
  int getUniqueNotebookID(Notebook *notebookToSync=nullptr);

  Notebook *addNotebook(QString title="New Notebook", Notebook *parent=nullptr, QVector<Notebook*> children={});
  void addNotebook(Notebook *notebook);
  void addNotebook(Notebook *notebook, Notebook *parent);

  void removeNotebook(int notebookID);
  void removeNotebook(Notebook *notebook);
  void clearNotebooks();

  Notebook *findNotebookWithID(int id);

  void loadJSON(QJsonDocument jsonDocument);

  void loadDummyNotebooks();

  void jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent=nullptr);

private slots:
  void notebookChanged_slot(Notebook *notebook);
  void notebookIDChanged_slot(Notebook *notebook);
  void notebookTitleChanged_slot(Notebook *notebook);
  void notebookParentChanged_slot(Notebook *notebook);
  void notebookChildrenChanged_slot(Notebook *notebook);
  void handleNotebookParentRequest(Notebook *notebook, int requestedParentID);
signals:
  void notebookAdded(Notebook *notebook);
  void notebooksRemoved(QVector<int> notebookIDs);
  void notebookChanged(Notebook *notebook);
  void notebookIDChanged(Notebook *notebook);
  void notebookTitleChanged(Notebook *notebook);
  void notebookParentChanged(Notebook *notebook);
  void notebookChildrenChanged(Notebook *notebook);

  // NotebookDatabase listens to these signals and acts accordingly
  void shouldDeleteNotesInNotebooks(QVector<int> notebookIDs);
  void shouldSetNotesInNotebookToDefaultNotebooks(QVector<int> notebookIDs);

private:
  QVector<Notebook*> m_list;
  NoteDatabase *m_noteDatabase;

};

#endif // NOTEBOOKDATABASE_H
