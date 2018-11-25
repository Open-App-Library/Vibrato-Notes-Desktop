#include "escribamanager.h"
#include <QDebug>
#include "../models/items/treeitemwithid.h"

// Parse tags string (saving thing for when I'm ready to implement)
//    QString tagsStr;
//    for (int i = 0; i < note->tags().length(); i++) {
//            int tagID = note->tags().at(i);
//			Tag *tag = m_db->tagDatabase()->findTagWithID(tagID);
//			if (tag != nullptr) {
//                tagsStr.append(tag->title());
//                if (i < note->tags().length()-1)
//                    tagsStr.append(", ");
//			}
//    }
//    m_tagsWidget->setText(tagsStr);

EscribaManager::EscribaManager(Escriba *editor, Database *db) :
    m_editor(editor),
		m_db(db),
    m_addons_ui(new Ui::EscribaAddonsWidget)
{
    m_addons_ui->setupUi( editor->addonsArea() );
    m_titleWidget    = m_addons_ui->f_noteTitle;
    m_tagsInputWidget     = m_addons_ui->f_noteTagsInput;
    m_tagsViewerWidget     = m_addons_ui->f_noteTagsViewer;
    m_notebookWidget = m_addons_ui->f_noteNotebook;
    m_trashWidget    = m_addons_ui->f_noteTrash;
    m_moreWidget     = m_addons_ui->f_noteMore;
    m_dateCreatedWidget = m_addons_ui->f_noteDateCreated;
    m_dateModifiedWidget = m_addons_ui->f_noteDateModified;

    connect(editor, &Escriba::documentChanged,
            this, &EscribaManager::contentChangedFromEditor);
    connect(m_titleWidget, &QLineEdit::textChanged,
            this, &EscribaManager::titleChangedFromEditor);
    connect(m_titleWidget, &QLineEdit::returnPressed,
            this, &EscribaManager::focusEditor);
    connect(m_tagsInputWidget, &QLineEdit::returnPressed,
            this, &EscribaManager::addTag);
    connect(m_notebookWidget, &QToolButton::clicked,
            this, &EscribaManager::openNotebookEditor);
    connect(m_tagsViewerWidget, &QToolButton::clicked,
            this, &EscribaManager::openTagsEditor);
}

void EscribaManager::updateTagsButtonCounter()
{
    m_tagsViewerWidget->setText( QString("Manage Tags (%1)").arg( m_curNote->tags().count() ) );
}

void EscribaManager::setNote( Note *note )
{
    // Save current note and disconnect signal/slot
    if (m_curNote != nullptr) {
        m_curNote->setTitle( m_titleWidget->text() );
        m_curNote->setText( m_editor->toMarkdown() );
        disconnect(m_curNote, &Note::noteChanged,
                this, &EscribaManager::updateNotebookWidget);
    }

    connect(note, &Note::noteChanged,
            this, &EscribaManager::noteChanged);

    // Change to requested note
    m_curNote = note;
    m_titleWidget->setText(note->title());
    m_editor->setMarkdown(note->text());
    updateTagsButtonCounter();

    updateNotebookWidget();
    updateDateWidgets();

    QString created = "<strong>Created:</strong> %1";
    QString modified = "<strong>Modified:</strong> %1";
    //m_dateCreatedWidget->setText( created.arg( m_curNote->date_created().toString() ) );
}

Note *EscribaManager::note()
{
    return m_curNote;
}

void EscribaManager::contentChangedFromEditor(QString markdown)
{
    if (!m_curNote)
        return;
    if (!QString::compare(m_curNote->text(), markdown)) // If titles are same, exit
        return;
    m_curNote->setText(markdown);
}

void EscribaManager::titleChangedFromEditor(QString title)
{
    if (!m_curNote)
        return;
    if (!QString::compare(m_curNote->title(), title)) // If titles are same, exit
        return;
    m_curNote->setTitle( title );
}

void EscribaManager::addTag()
{
    Tag *tag = m_db->tagDatabase()->findTagWithNameOrCreate( m_tagsInputWidget->text() );

    // If tag is not already added, add it.
    if ( m_curNote->tags().indexOf(tag->id()) == -1 ) {
        m_curNote->tags().append( tag->id() );
        m_curNote->setTags( m_curNote->tags() << tag->id());
    }
    updateTagsButtonCounter();
    m_tagsInputWidget->clear();
}

void EscribaManager::openNotebookEditor()
{
    if ( m_curNote == nullptr)
        return;

    // If dialog is not a nullptr and its note ID is not equal to the new note, delete it.
    if (m_editNotebookDialog != nullptr && m_editNotebookDialog->note()->id() != m_curNote->id()) {
        delete m_editNotebookDialog;
        m_editNotebookDialog = nullptr;
    }

    if (m_editNotebookDialog == nullptr) {
        m_editNotebookDialog = new Note_EditNotebook(m_db, m_curNote);
    }

    m_editNotebookDialog->show();
}

void EscribaManager::openTagsEditor()
{
    if ( m_curNote == nullptr)
        return;

    // If dialog is not a nullptr and its note ID is not equal to the new note, delete it.
    if (m_editTagsDialog != nullptr && m_editTagsDialog->note()->id() != m_curNote->id()) {
        delete m_editNotebookDialog;
        m_editTagsDialog = nullptr;
    }

    if (m_editTagsDialog == nullptr) {
        m_editTagsDialog = new Note_EditTags(m_db, m_curNote);
    }

    m_editTagsDialog->show();
}

void EscribaManager::focusEditor()
{
    m_editor->focusEditor();
}

void EscribaManager::noteChanged()
{
    updateNotebookWidget();
    updateDateWidgets();
}

void EscribaManager::updateNotebookWidget()
{
    // Adjust notebook selector widget
    if ( m_curNote != nullptr && m_curNote->notebook() >= 0 ) {
        Notebook *notebook = m_db->notebookDatabase()->findNotebookWithID(m_curNote->notebook());
        if ( notebook != nullptr )
            m_notebookWidget->setText( notebook->title() );
        // If notebook has parents, append a nice little hierarchy indication
        if ( notebook->parent() != nullptr ) {
            Notebook *parent = notebook->parent();
            while (parent != nullptr) {
                m_notebookWidget->setText( QString(parent->title() + " -> ").append( m_notebookWidget->text() ) );
                parent = parent->parent();
            }
        }
    } else {
        m_notebookWidget->setText( "Default Notebook" );
    }
}

void EscribaManager::updateDateWidgets()
{
    if ( m_curNote == nullptr )
        return;
    //QString cur = m_curNote->date_created().toString()
    m_dateCreatedWidget->setText( "<strong>Created:</strong> " + m_curNote->date_created_str() );
    m_dateCreatedWidget->setToolTip( m_curNote->date_created_str_informative() );

    m_dateModifiedWidget->setText( "<strong>Modified:</strong> " + m_curNote->date_modified_str() );
    m_dateModifiedWidget->setToolTip( m_curNote->date_modified_str_informative() );
}
