#ifndef GENERICVIEW_H
#define GENERICVIEW_H
#include <QObject>
#include <QListView>
#include "../../models/sortfilter/notelistproxymodel.h"
#include "../../meta/db/database.h"
#include "../manager.h"
#include <ui_notelist_addons.h>

namespace Ui {
  class NoteListAddonsWidget;
}

typedef Ui::NoteListAddonsWidget NoteListAddonsWidget;

class GenericView : public QObject
{
  Q_OBJECT
public:
  explicit GenericView(Database *db, Manager *manager, QObject *parent=nullptr);

  Database *db() const;
  Manager *manager() const;
  NoteListProxyModel *proxyModel() const;
  QListView *listView() const;
  NoteListAddonsWidget *addonsWidgetUi() const;

  virtual void activateView();
  virtual void deactivateView();

private:
  Database *m_db=nullptr;
  Manager *m_manager=nullptr;
};

#endif // GENERICVIEW_H
