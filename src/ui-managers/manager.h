#ifndef MANAGER_H
#define MANAGER_H
#include <QObject>

class TreeManager;
class NoteListManager;
class EscribaManager;

class Manager: public QObject {
  Q_OBJECT
public:
  TreeManager *treeManager() const;
  NoteListManager *noteListManager() const;
  EscribaManager *escribaManager() const;

  void setManagers(TreeManager *treeManager, NoteListManager *noteListManager, EscribaManager *escribaManager);

private:
  TreeManager *m_treeManager;
  NoteListManager *m_noteListManager;
  EscribaManager *m_escribaManager;
};

#endif
