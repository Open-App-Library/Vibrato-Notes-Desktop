#ifndef TRASHVIEW_H
#define TRASHVIEW_H
#include <QObject>
#include <QListWidget>
#include <QVector>
#include <QCheckBox>
#include <QToolButton>
#include "genericview.h"
#include "../../../ui/trashitem.h"
#include "../../meta/note.h"

class TrashView : public GenericView
{
public:
  explicit TrashView(Database *db, Manager *manager, QObject *parent=nullptr);

  void setTrashNotes(QVector<Note*> trashNotes);
  void addTrashNote(Note *trashNote);

  void activateView() override;
  void deactivateView() override;

  TrashItem *addItem(Note *note);

  void select(TrashItem *item);
  void deselect(TrashItem *item);
  void selectAll();
  void deselectAll();

private slots:
  void determineMassActionVisibility(void);
  void selectionChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
  QListWidget *m_trashListWidget=nullptr;
  QWidget *m_massActions=nullptr;

  QVector<Note*> m_trashNotes;
  QVector<TrashItem*> m_trashItems;
  QVector<TrashItem*> m_selectedTrashItems;

  QCheckBox *m_checkbox=nullptr;
  QToolButton *m_massRestore=nullptr;
  QToolButton *m_massDelete=nullptr;

  // Private functions
  void toggleMassCheckmark(void);
  void itemCheckedOrUnchecked(TrashItem *item);
};

#endif // TRASHVIEW_H
