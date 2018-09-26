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

    add_no_notebooks_placeholder();
    add_no_tags_placeholder();

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
    remove_no_notebooks_placeholder();
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

    if (m_notebook_list.length() <= 0) {
        add_no_notebooks_placeholder();
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
    add_no_notebooks_placeholder();
}

QList<QTreeWidgetItem *> TreeManager::tags()
{
    return m_tag_list;
}

QTreeWidgetItem *TreeManager::addTag(QString label)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(m_tags);
    item->setText(0, label);
    m_tag_list.append(item);
    remove_no_tags_placeholder();
    return item;
}

void TreeManager::removeTag(int index)
{
    QTreeWidgetItem *item = m_tag_list[index];

    m_tag_list.removeAt(index);
    delete item;

    if (m_tag_list.length() <= 0) {
        add_no_tags_placeholder();
    }
}

void TreeManager::clearTags()
{
    for (int i = m_tag_list.length()-1; i >= 0; i--) {
        delete m_tag_list[i];
        m_tag_list.removeAt(i);
    }
    add_no_tags_placeholder();
}

void TreeManager::add_no_notebooks_placeholder()
{
    if (m_no_notebooks_placedholder == nullptr) {
        m_no_notebooks_placedholder = new QTreeWidgetItem(m_notebooks);
        m_no_notebooks_placedholder->setText(0, "No Notebooks");
        m_no_notebooks_placedholder->setDisabled(true);
    }
}

void TreeManager::add_no_tags_placeholder()
{
    if (m_no_tags_placedholder == nullptr) {
        m_no_tags_placedholder = new QTreeWidgetItem(m_tags);
        m_no_tags_placedholder->setText(0, "No Tags");
        m_no_tags_placedholder->setDisabled(true);
    }
}

void TreeManager::remove_no_notebooks_placeholder()
{
    if (m_no_notebooks_placedholder != nullptr) {
        delete m_no_notebooks_placedholder;
        m_no_notebooks_placedholder = nullptr;
    }
}

void TreeManager::remove_no_tags_placeholder()
{
    if (m_no_tags_placedholder != nullptr) {
        delete m_no_tags_placedholder;
        m_no_tags_placedholder = nullptr;
    }
}

