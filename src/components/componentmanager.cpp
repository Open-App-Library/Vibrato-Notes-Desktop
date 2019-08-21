#include "componentmanager.h"

TreeManager *ComponentManager::treeManager() const
{ return m_treeManager; }

ListManager *ComponentManager::listManager() const
{ return m_listManager; }

EscribaManager *ComponentManager::escribaManager() const
{ return m_escribaManager; }

void ComponentManager::setComponent(TreeManager *component)
{ m_treeManager = component; }

void ComponentManager::setComponent(ListManager *component)
{ m_listManager = component; }

void ComponentManager::setComponent(EscribaManager *component)
{ m_escribaManager = component; }
