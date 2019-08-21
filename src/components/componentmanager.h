#ifndef MANAGER_H
#define MANAGER_H
#include <QObject>

class TreeManager;
class ListManager;
class EscribaManager;

class ComponentManager: public QObject {
  Q_OBJECT
public:
  TreeManager *treeManager() const;
  ListManager *listManager() const;
  EscribaManager *escribaManager() const;

  void setComponent(TreeManager *component);
  void setComponent(ListManager *component);
  void setComponent(EscribaManager *component);

signals:
  void ready();

private:
  TreeManager *m_treeManager;
  ListManager *m_listManager;
  EscribaManager *m_escribaManager;
};

#endif
