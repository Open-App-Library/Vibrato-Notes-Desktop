#include "treemanager.h"

TreeManager::TreeManager(QTreeView *treeView)
{
    m_tree_model = new TreeModel;
    treeView->setModel(m_tree_model);

    m_all_notes = new BasicTreeItem(QObject::tr("All Notes"));
    m_notebooks = new BasicTreeItem(QObject::tr("Notebooks"));
    m_tags      = new BasicTreeItem(QObject::tr("Tags"));

    m_tree_model->root()->appendChild(m_all_notes);
    m_tree_model->root()->appendChild(m_notebooks);
    m_tree_model->root()->appendChild(m_tags);
}

BasicTreeItem *TreeManager::addNotebook(QString label)
{
    return addNotebook(label, m_notebooks);
}

BasicTreeItem *TreeManager::addNotebook(QString label, BasicTreeItem *parent)
{
    BasicTreeItem *newItem = new BasicTreeItem(label, parent);
    return addNotebook(newItem, parent);
}

BasicTreeItem *TreeManager::addNotebook(BasicTreeItem *item)
{
    return addNotebook(item, m_notebooks);
}

BasicTreeItem *TreeManager::addNotebook(BasicTreeItem *item, BasicTreeItem *parent)
{
    parent->appendChild(item);
    return item;
}
