#include "note_editnotebook.h"
#include "ui_note_editnotebook.h"
#include "../../meta/db/notebookdatabase.h"

#include <QDebug>

Note_EditNotebook::Note_EditNotebook(Database *db, Note *note, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Note_EditNotebook),
    m_db(db),
    m_note(note)
{
    ui->setupUi(this);

    // Tree Widget
    m_notebookTree = ui->notebookTree;

    // Button box
    m_selectNotebook = new QPushButton("Select Notebook");

    ui->buttonBox->addButton(m_selectNotebook, QDialogButtonBox::ActionRole);

    updateTitle();

    loadNotesNotebooks();
    m_notebookTree->expandAll();

    m_selectNotebook->setEnabled(false);

    connect(note, &Note::noteChanged,
            this, &Note_EditNotebook::noteChanged);
    connect(m_selectNotebook, &QPushButton::clicked,
            this, &Note_EditNotebook::selectNotebook);
    connect(m_notebookTree, &QTreeWidget::currentItemChanged,
            this, &Note_EditNotebook::currentItemChanged);
}

Note_EditNotebook::~Note_EditNotebook()
{
    delete m_selectNotebook;
    delete ui;
}

Note *Note_EditNotebook::note()
{
    return m_note;
}

void Note_EditNotebook::noteChanged(void)
{
    updateTitle();
}

void Note_EditNotebook::selectNotebook()
{
    QList<QTreeWidgetItem*> selItemsArray = m_notebookTree->selectedItems();
    if ( selItemsArray.length() == 0)
        return;
    TreeItemWithID *sel = static_cast<TreeItemWithID*>( selItemsArray.at(0) );

    // Uncheck all
    for ( int i = 0; i < m_treeItems.length(); i++ )
        checkItem(m_treeItems.at(i), false);
    // Check selected icon
    checkItem(sel, true);

    // Save into note object
    int curId = m_note->notebook();
    int id = sel->id();
    if ( curId != id ) {
        m_note->setNotebook(id);
        emit notebookChanged();
    }
}

void Note_EditNotebook::updateTitle()
{
    if ( m_note == nullptr)
        return;

    setWindowTitle( QString("Editing notebook for \"%1\"").arg(m_note->title()) );
}

void Note_EditNotebook::clearTree()
{
    delete m_defaultNotebookItem;

    for ( int i=m_treeItems.length()-1; i>=0; i-- )
        delete m_treeItems.at(i);
}

void Note_EditNotebook::addNotebookToTree(Notebook *notebook, TreeItemWithID *parent)
{
    TreeItemWithID *item = new TreeItemWithID(notebook->title(), notebook->id());
    m_treeItems.append(item);

    checkItem( item, m_note->notebook() == notebook->id() );

    if ( parent == nullptr )
        m_notebookTree->addTopLevelItem(item);
    else
        parent->addChild( item );

    for ( int i=0; i<notebook->children().length(); i++ ) {
        Notebook *child = notebook->children().at(i);
        addNotebookToTree(child, item);
    }
}

void Note_EditNotebook::loadNotesNotebooks()
{
    clearTree();
    // First add the 'Default Notebook' selection
    m_defaultNotebookItem = new TreeItemWithID("Default Notebook", -1);
    m_notebookTree->addTopLevelItem(m_defaultNotebookItem);
    m_treeItems.append(m_defaultNotebookItem);

    // Now add everything else
    QVector<Notebook*> notebooks = m_db->notebookDatabase()->list();
    for ( int i=0; i<notebooks.length(); i++ ) {
        Notebook *n = notebooks.at(i);
        addNotebookToTree(n);
    }

    for ( int i = 0; i < m_treeItems.length(); i++ ) {
        TreeItemWithID *item = m_treeItems.at(i);
        bool condition = item->id() == m_note->notebook();
        checkItem( item, condition);
        if ( condition )
            break;
    }
}

void Note_EditNotebook::checkItem(TreeItemWithID *item, bool isChecked)
{
    if ( isChecked ) {
        item->setIcon(0, QIcon::fromTheme("dialog-ok-apply"));
        item->setSelected(true);
    }
    else
        item->setIcon(0, QIcon());
}

void Note_EditNotebook::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    (void) previous; // Ignore unused parameter compiler warning
    TreeItemWithID *cur = static_cast<TreeItemWithID*>(current);
    if ( cur->id() == m_note->notebook()) {
        m_selectNotebook->setEnabled(false);
    } else {
        m_selectNotebook->setEnabled(true);
    }
}
