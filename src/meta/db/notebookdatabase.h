#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QVector>
#include "../notebook.h"
#include "notedatabase.h"
#include "../../sql/sqlmanager.h"

class NotebookDatabase : public QObject
{
  Q_OBJECT
public:
  NotebookDatabase(SQLManager *sqlManager, NoteDatabase *noteDatabase);

  QVector<Notebook *> list() const;
  int                 size() const;
  QVector<Notebook *> listRecursively() const;
  QVector<Notebook *> listRecursively(const QVector<Notebook*> notebookList) const;

  void addNotebook(Notebook *notebook);
  void addNotebook(Notebook *notebook, Notebook *parent);

  void removeNotebook(QUuid syncHash);
  void removeNotebook(Notebook *notebook);
  void clearNotebooks();

  Notebook *findNotebookWithSyncHash(QUuid syncHash);

  void loadSQL();

  void connectNotebook(Notebook *notebook);
  void disconnectNotebook(Notebook *notebook);

private slots:
  void changed_slot(Notebook *notebook);

  void syncHashChanged_slot(Notebook *notebook);
  void titleChanged_slot(Notebook *notebook);
  void parentChanged_slot(Notebook *notebook);
  void childrenChanged_slot(Notebook *notebook);

  void handleNotebookParentRequest(Notebook *notebook, QUuid parentSyncHash);

signals:
  void changed(Notebook *notebook);

  void syncHashChanged(Notebook *notebook);
  void titleChanged(Notebook *notebook);
  void parentChanged(Notebook *notebook);
  void childrenChanged(Notebook *notebook);

  void added(Notebook *notebook);
  void removed(QVector<QUuid> notebookSyncHashes);

private:
  SQLManager *m_sqlManager;
  QVector<Notebook*> m_list;
  NoteDatabase *m_noteDatabase;

};

#endif // NOTEBOOKDATABASE_H
