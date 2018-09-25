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
};

#endif // TREEMANAGER_H
