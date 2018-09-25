/*
 * "I speak for the trees" - The Lorax
 *
 * The purpose
 */

#ifndef TREEMANAGER_H
#define TREEMANAGER_H
#include <QTreeWidget>
#include <QTreeWidgetItem>

class TreeManager
{
public:
    TreeManager(QTreeWidget *treeWidget);

    // Notebook functions
    QList<QTreeWidgetItem*> notebooks();
    QTreeWidgetItem *addNotebook(QString label);
    QTreeWidgetItem *addNotebook(QString label, QTreeWidgetItem *parent);
    void removeNotebook(int index);
    // This function will delete a notebook. If there are children, it will assign them a new parent
    void removeNotebook(int index, QTreeWidgetItem *fosterParent);
    void clearNotebooks();

    // Tag functions
    QList<QTreeWidgetItem*> tags();
    void add_tag(QString label);
    void remove_tag(int index);
    void clear_tags();

private:
    QTreeWidget     *m_tree_widget;
    QTreeWidgetItem *m_all_notes;
    QTreeWidgetItem *m_notebooks;
    QTreeWidgetItem *m_tags;
    QList<QTreeWidgetItem*> m_notebook_list;
    QList<QTreeWidgetItem*> m_tag_list;

    QTreeWidgetItem *m_no_notebooks_placedholder = nullptr;
    QTreeWidgetItem *m_no_tags_placedholder = nullptr;

    void add_no_notebooks_placeholder();
    void add_no_tags_placeholder();
    void remove_no_notebooks_placeholder();
    void remove_no_tags_placeholder();

};

#endif // TREEMANAGER_H
