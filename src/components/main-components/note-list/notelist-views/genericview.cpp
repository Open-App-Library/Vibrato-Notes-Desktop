#include "genericview.h"
#include <QDebug>
#include "../notelistmanager.h"

GenericView::GenericView(Database *db, Manager *manager, QObject *parent):
  QObject(parent),
  m_db(db),
  m_manager(manager)
{
}

Database *GenericView::db() const
{
  return m_db;
}

Manager *GenericView::manager() const
{
  return m_manager;
}

NoteListProxyModel *GenericView::proxyModel() const
{
  return m_manager->noteListManager()->proxyModel();
}

QListView *GenericView::listView() const
{
  return m_manager->noteListManager()->view();
}

NoteListAddonsWidget *GenericView::addonsWidgetUi() const
{
  return m_manager->noteListManager()->addonsWidgetUi();
}

void GenericView::activateView() {
  qDebug("[GenericView] activateView() function has not yet been overrided.");
}

void GenericView::deactivateView() {
  qDebug("[GenericView] deactivateView() function has not yet been overrided.");
}
