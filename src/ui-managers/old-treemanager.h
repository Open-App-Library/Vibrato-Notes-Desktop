/*
 * "I speak for the trees" - The Lorax
 *
 * The purpose of this file is to control the tree view
 * and provide a flexible API for managing notebooks
 * and tags.
 */

#ifndef TREEMANAGER_H
#define TREEMANAGER_H
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "../meta/db/notebookdatabase.h"

class OldTreeManager
{
public:
    OldTreeManager(QTreeWidget *treeWidget);

    // Notebook functions
    QList<QTreeWidgetItem*> notebooks();
    QTreeWidgetItem        *addNotebook(QString label);
    QTreeWidgetItem        *addNotebook(QString label, QTreeWidgetItem *parent);
    QTreeWidgetItem        *addNotebook(QTreeWidgetItem *item);
    QTreeWidgetItem        *addNotebook(QTreeWidgetItem *item, QTreeWidgetItem *parent);
    void                    removeNotebook(int index);
    void                    removeNotebook(int index, QTreeWidgetItem *fosterParent);
    void                    clearNotebooks();
    void                    loadNotebookObjectAndChildren(Notebook *notebook, QTreeWidgetItem *parent=nullptr);
    void                    loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase);

    // Tag functions
    QList<QTreeWidgetItem*> tags();
    QTreeWidgetItem        *addTag(QString label);
    void                    removeTag(int index);
    void                    clearTags();

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
