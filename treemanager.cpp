#include "treemanager.h"
#include <QDebug>

TreeManager::TreeManager(QTreeWidget *treeWidget)
{
    m_tree_widget = treeWidget;

    m_all_notes = new QTreeWidgetItem(m_tree_widget);
    m_notebooks = new QTreeWidgetItem(m_tree_widget);
    m_tags      = new QTreeWidgetItem(m_tree_widget);

    m_all_notes->setText(0, QObject::tr("All Notes"));
    m_notebooks->setText(0, QObject::tr("Notebooks"));
    m_tags     ->setText(0, QObject::tr("Tags"));

    // Expand the tree by default
    for ( int i = 0; i < m_tree_widget->topLevelItemCount(); i++ ) {
        m_tree_widget->topLevelItem( i )->setExpanded(true);
    }
}

QList<QTreeWidgetItem *> TreeManager::notebooks()
{
    return m_notebook_list;
}

QTreeWidgetItem *TreeManager::addNotebook(QString label)
{
    return addNotebook(label, m_notebooks); // Uses the function below
}

QTreeWidgetItem *TreeManager::addNotebook(QString label, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, label);
    m_notebook_list.append(item);
    return item;
}

void TreeManager::removeNotebook(int index)
{
    QTreeWidgetItem *item = m_notebook_list[index];
    int childCount = item->childCount();
    int deleteCount = 1; // Amount of items we are deleting (Including children)

    QList<int> indexDeleteList = {index};
    QList<QTreeWidgetItem*> itemDeleteList = {item};

    for (int i = 0; i < childCount; i++) {
        QTreeWidgetItem *child = item->child(i);
        int childIndex = m_notebook_list.indexOf(child);
        indexDeleteList.append(childIndex);
        itemDeleteList.append(child);
        deleteCount++; // We are deleting a child. Increment the delete count.
    }

    for (int i = deleteCount-1; i >= 0; i--) { // See? We are using the deleteCount here to do the deletions.
        m_notebook_list.removeAt(indexDeleteList[i]);
        delete itemDeleteList[i];
    }
}

void TreeManager::removeNotebook(int index, QTreeWidgetItem *fosterParent)
{
    QTreeWidgetItem *item = m_notebook_list[index];
    int childCount = item->childCount();

    for (int i = 0; i < childCount; i++) {
        QTreeWidgetItem *child = item->child(i);
        QTreeWidgetItem *newChild = new QTreeWidgetItem(fosterParent);
        newChild->setText(0, child->text(0));
    }

    removeNotebook(index);
}

void TreeManager::clearNotebooks()
{
    for (int i = m_notebook_list.length()-1; i >= 0; i--) {
        delete m_notebook_list[i];
        m_notebook_list.removeAt(i);
    }
}
