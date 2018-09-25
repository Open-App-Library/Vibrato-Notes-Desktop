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
    QTreeWidgetItem *item = new QTreeWidgetItem(m_notebooks);
    item->setText(0, label);
    return item;
}

QTreeWidgetItem *TreeManager::addNotebook(QString label, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, label);
    return item;
}

void TreeManager::removeNotebook(int index)
{
    qDebug() << "The length of the notebook tree is" << m_notebook_list.length();
    QTreeWidgetItem *item = m_notebook_list[index];
    delete item;
    qDebug() << "The length of the notebook tree is NOW" << m_notebook_list.length();
    //m_notebook_list.removeAt(index);
}

void TreeManager::clearNotebooks()
{
    qDebug() << "CLEARING: The length of the notebook tree is" << m_notebook_list.length();
    for (int i = m_notebook_list.length()-1; i >= 0; i--) {
        delete m_notebook_list[i];
    }
    qDebug() << "CLEARING: The length of the notebook tree is NOW" << m_notebook_list.length();
}
