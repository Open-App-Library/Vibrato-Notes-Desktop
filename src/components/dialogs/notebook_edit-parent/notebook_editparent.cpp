#include "notebook_editparent.h"
#include "ui_notebook_editparent.h"
#include <QDebug>
Notebook_EditParent::Notebook_EditParent(NotebookDatabase *notebookDatabase, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Notebook_EditParent),
  m_notebookDatabase(notebookDatabase)
{
  ui->setupUi(this);
  setModal(true);

  treeWidget = ui->treeWidget;
  buttonBox  = ui->buttonBox;

  connect(buttonBox, &QDialogButtonBox::accepted,
          this, &Notebook_EditParent::okButtonClicked);
}

Notebook_EditParent::~Notebook_EditParent()
{
  delete ui;
}

Notebook *Notebook_EditParent::notebook() const {
  return m_curNotebook;
}

void Notebook_EditParent::setNotebook(Notebook *notebook)
{
  m_curNotebook = notebook;
  treeWidget->clear();
  addNotebooks();
  treeWidget->expandAll();
}

void Notebook_EditParent::addNotebooks(Notebook* parentNotebook,
                                       TreeItemWithID* parentTreeItem,
                                       bool disable_children)
{
  QVector<Notebook*> notebooks;
  if (parentNotebook == nullptr)
    notebooks = m_notebookDatabase->list();
  else
    notebooks = parentNotebook->children();

  for ( Notebook *notebook : notebooks ) {
    QString title = notebook->syncHash() == nullptr ? "(No parent)" : notebook->title();
    TreeItemWithID *newItem = new TreeItemWithID(title, notebook->syncHash());

    // Disable the tree item iff disable_children or the notebook is
    // the current notebook (You can't be a parent of yourself!)
    bool is_disabled =
      disable_children ||
      notebook == m_curNotebook;

    qDebug() << notebook->title() << disable_children << (notebook == m_curNotebook) << (notebook == m_curNotebook->parent());

    // Disable if is_disabled or if is current notebook's parent
    newItem->setDisabled( is_disabled );

    if (parentTreeItem == nullptr)
      treeWidget->addTopLevelItem(newItem);
    else
      parentTreeItem->addChild(newItem);

    if (notebook->children().count() > 0)
      addNotebooks(notebook, newItem, is_disabled);
  }
}

int Notebook_EditParent::exec()
{
  return QDialog::exec();
}

int Notebook_EditParent::exec(Notebook *notebook)
{
  setNotebook(notebook);
  return exec();
}

void Notebook_EditParent::okButtonClicked()
{
  // If no items are selected, exit.
  if (treeWidget->selectedItems().length() == 0)  return;

  // Get the single selected item.
  TreeItemWithID *selectedItem =
    static_cast<TreeItemWithID*>(treeWidget->selectedItems()[0]);

  QUuid newSyncHash = selectedItem->syncHash();

  // If the parent ID hasn't changed, exit.
  QUuid curParentSyncHash = nullptr;
  if ( m_curNotebook->parent() != nullptr )
    curParentSyncHash = m_curNotebook->parent()->syncHash();
  if ( curParentSyncHash == newSyncHash )
    return;
  Notebook *newParent = nullptr;
  if (newSyncHash != nullptr) newParent = m_notebookDatabase->findNotebookWithSyncHash(newSyncHash);
  m_curNotebook->setParent(newParent);

  emit changedParent();
}
