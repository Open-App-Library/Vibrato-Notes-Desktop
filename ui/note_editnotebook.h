#ifndef NOTE_EDITNOTEBOOKS_H
#define NOTE_EDITNOTEBOOKS_H
#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>
#include "../../ui-managers/manager.h"
#include "../../meta/note.h"
#include "../../meta/db/database.h"
#include "../../models/items/treeitemwithid.h"

namespace Ui {
  class Note_EditNotebook;
}

class Note_EditNotebook : public QDialog
{
  Q_OBJECT

public:
  explicit Note_EditNotebook(Database *db, Manager *manager, Note *note, QWidget *parent = nullptr);
  ~Note_EditNotebook();

  Note *note();

  int selectedNotebook() const;

private slots:
  void noteChanged(void);
  void selectNotebook();

  void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

  void notebookAddedOrRemoved(void);

signals:
  void notebookChanged();

private:
  Ui::Note_EditNotebook *ui;
  Manager *m_manager;

  void updateTitle();

  QTreeWidget *m_notebookTree;
  QPushButton *m_selectNotebook;

  Database *m_db=nullptr;
  Note *m_note=nullptr;

  TreeItemWithID *m_defaultNotebookItem=nullptr;
  QVector<TreeItemWithID*> m_treeItems;

  // private functions
  void clearTree();
  void addNotebookToTree(Notebook *notebook, TreeItemWithID *parent=nullptr);
  void loadNotesNotebooks();
  void updateCheckMarks();
  void checkItem(TreeItemWithID *item, bool isChecked);

  bool isResettingTree=false;
};

#endif // NOTE_EDITNOTEBOOKS_H
