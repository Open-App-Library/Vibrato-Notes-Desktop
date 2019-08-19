#ifndef NOTEBOOK_EDITPARENT_H
#define NOTEBOOK_EDITPARENT_H

#include <QDialog>
#include <QTreeWidget>
#include <QDialogButtonBox>
#include "../src/meta/db/notebookdatabase.h"
#include "../src/models/items/treeitemwithid.h"

namespace Ui {
class Notebook_EditParent;
}

class Notebook_EditParent : public QDialog
{
  Q_OBJECT

public:
  explicit Notebook_EditParent(NotebookDatabase *notebookDatabase, QWidget *parent = nullptr);
  ~Notebook_EditParent();

  Notebook *notebook() const;
  void setNotebook(Notebook *notebook);

  void addNotebooks(Notebook *parentNotebook=nullptr,
                    TreeItemWithID *parentTreeItem=nullptr,
                    bool disable_children=false);

  virtual int exec() override;
  int exec(Notebook *notebook);

public slots:
  void okButtonClicked();

signals:
  void changedParent();

private:
  Ui::Notebook_EditParent *ui;
  Notebook *m_curNotebook=nullptr;
  NotebookDatabase *m_notebookDatabase=nullptr;

  QTreeWidget *treeWidget;
  QDialogButtonBox *buttonBox;
};

#endif // NOTEBOOK_EDITPARENT_H
