#include <QDebug>
#include "treemanager.h"

TreeManager::TreeManager(QTreeView *treeView)
{
    m_tree_view = treeView;
    m_tree_model = new TreeModel;
    m_tree_view->setModel(m_tree_model);

    m_all_notes = new BasicTreeItem(QObject::tr("All Notes"));
    m_notebooks = new BasicTreeItem(QObject::tr("Notebooks"));
    m_tags      = new BasicTreeItem(QObject::tr("Tags"));

    m_all_notes->setIcon(QIcon::fromTheme("document-new"));
    m_notebooks->setIcon(QIcon::fromTheme("folder"));
    m_tags->setIcon(QIcon::fromTheme("tag"));


    m_tree_model->root()->appendChild(m_all_notes);
    m_tree_model->root()->appendChild(m_notebooks);
    m_tree_model->root()->appendChild(m_tags);
}

void TreeManager::update()
{
    emit m_tree_model->layoutChanged();
}

QVector<BasicTreeItem *> TreeManager::notebooks() const
{
    return m_notebooks->recurseChildren();
}

BasicTreeItem *TreeManager::addNotebook(Notebook *notebook)
{
    return addNotebook(notebook, m_notebooks);
}

BasicTreeItem *TreeManager::addNotebook(Notebook *notebook, BasicTreeItem *parent)
{
    BasicTreeItem *item = parent->appendChild( new BasicTreeItem(notebook) );
    update();
    return item;
}

void TreeManager::removeNotebook(BasicTreeItem *item)
{
    clearChildren(item);
    delete item;
    update();
}

void TreeManager::removeNotebook(BasicTreeItem *item, BasicTreeItem *fosterParent)
{
    QVector<BasicTreeItem*> children = item->recurseChildren();
    for (int i = 0; i < children.size(); i++) {
        fosterParent->appendChild( children[i] );
    }
    delete item;
    update();
}

void TreeManager::clearChildren(BasicTreeItem *item)
{
    QVector<BasicTreeItem*> children = item->recurseChildren();
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
    update();
}

void TreeManager::clearNotebooks()
{
    for (int i = m_notebooks->childCount()-1; i >= 0; i--) {
        clearChildren(m_notebooks->getChild(i));
        delete m_notebooks->getChild(i);
    }
    update();
}

void TreeManager::loadNotebookObjectAndChildren(Notebook *notebook, BasicTreeItem *parent)
{
    BasicTreeItem *item;
    if (parent == nullptr) {
        item = addNotebook(notebook);
    } else {
        item = addNotebook(notebook, parent);
    }
    for (int i = 0; i < notebook->children().size(); i++) {
        loadNotebookObjectAndChildren(notebook->children()[i], item);
    }
}

void TreeManager::loadNotebooksFromNotebookDatabase(NotebookDatabase *notebookDatabase)
{
    clearNotebooks();
    for (int i = 0; i < notebookDatabase->size(); i++) {
        loadNotebookObjectAndChildren(notebookDatabase->list()[i]);
    }
    m_tree_view->expandAll();
}

QVector<BasicTreeItem *> TreeManager::tags() const
{
    return m_tags->children();
}

BasicTreeItem *TreeManager::addTag(Tag *tag)
{
    BasicTreeItem *item =  m_tags->appendChild( new BasicTreeItem(tag) );
    update();
    return item;
}

void TreeManager::removeTag(BasicTreeItem *item)
{
    if ( item->childCount() > 0 )
        qWarning() << "Tag \"" << item->label() << "\" has a child count greater than zero. This should never happen.";
    delete item;
    update();
}

void TreeManager::clearTags()
{
    for (int i = m_tags->childCount()-1; i >= 0; i--) {
        clearChildren(m_notebooks->getChild(i));
        delete m_notebooks->getChild(i);
    }
    update();
}

void TreeManager::loadTagsFromTagDatabase(TagDatabase *tagDatabase)
{
    clearTags();
    for (int i = 0; i < tagDatabase->size(); i++) {
        addTag(tagDatabase->list()[i]);
    }
    update();
}
