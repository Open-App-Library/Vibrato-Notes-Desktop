/*
 * "I speak for the trees" - The Lorax
 *
 * The purpose of this file is to control the tree view
 * and provide a flexible API for managing notebooks
 * and tags.
 */

#ifndef TREEMANAGER_H
#define TREEMANAGER_H
#include <QTreeView>
#include "../models/items/basictreeitem.h"
#include "../models/treemodel.h"
#include "../meta/db/notebookdatabase.h"

class TreeManager
{
public:
    TreeManager(QTreeView *treeView);

    // Notebook functions
    QVector<BasicTreeItem*> notebooks();
    BasicTreeItem        *addNotebook(QString label);
    BasicTreeItem        *addNotebook(QString label, BasicTreeItem *parent);
    BasicTreeItem        *addNotebook(BasicTreeItem *item);
    BasicTreeItem        *addNotebook(BasicTreeItem *item, BasicTreeItem *parent);
    void                    removeNotebook(int index);
    void                    removeNotebook(int index, BasicTreeItem *fosterParent);
    void                    clearNotebooks();
    void                    loadNotebookObjectAndChildren(Notebook *notebook, BasicTreeItem *parent=nullptr);
    void                    loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase);

    // Tag functions
    QVector<BasicTreeItem*> tags();
    BasicTreeItem          *addTag(QString label);
    void                    removeTag(int index);
    void                    clearTags();

private:
    TreeModel     *m_tree_model;
    QTreeView     *m_tree_view;

    BasicTreeItem *m_all_notes;
    BasicTreeItem *m_notebooks;
    BasicTreeItem *m_tags;

    QVector<BasicTreeItem*> m_notebook_list;
    QVector<BasicTreeItem*> m_tag_list;

    // TODO: https://forum.qt.io/topic/45262/disable-certain-rows-in-qtreeview
    BasicTreeItem *m_no_notebooks_placedholder = nullptr;
    BasicTreeItem *m_no_tags_placedholder = nullptr;

    void add_no_notebooks_placeholder();
    void add_no_tags_placeholder();
    void remove_no_notebooks_placeholder();
    void remove_no_tags_placeholder();

};

#endif // TREEMANAGER_H
