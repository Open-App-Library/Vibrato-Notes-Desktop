#include "manager.h"

TreeManager *Manager::treeManager() const
{
  return m_treeManager;
}

NoteListManager *Manager::noteListManager() const
{
  return m_noteListManager;
}

EscribaManager *Manager::escribaManager() const
{
  return m_escribaManager;
}

void Manager::setManagers(TreeManager *treeManager, NoteListManager *noteListManager, EscribaManager *escribaManager)
{
  m_treeManager = treeManager;
  m_noteListManager = noteListManager;
  m_escribaManager = escribaManager;
  emit ready();
}
