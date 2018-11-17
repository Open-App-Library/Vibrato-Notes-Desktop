#include "escribamanager.h"
#include <QDebug>

////////////////////stuff


EscribaManager::EscribaManager(Escriba *editor) :
    m_editor(editor),
    m_addons_ui(new Ui::EscribaAddonsWidget)
{
    m_addons_ui->setupUi( editor->addonsArea() );
    m_titleWidget    = m_addons_ui->f_noteTitle;
    m_tagsWidget     = m_addons_ui->f_noteTags;
    m_notebookWidget = m_addons_ui->f_noteNotebook;
    m_trashWidget    = m_addons_ui->f_noteTrash;
    m_moreWidget     = m_addons_ui->f_noteMore;
    m_dateCreatedWidget = m_addons_ui->f_noteDateCreated;
    m_dateModifiedWidget = m_addons_ui->f_noteDateModified;

    connect(editor, &Escriba::documentChanged,
            this, &EscribaManager::contentChangedFromEditor);
    connect(m_titleWidget, &QLineEdit::textChanged,
            this, &EscribaManager::titleChangedFromEditor);

}

void EscribaManager::setNote( Note *note )
{
    // Save current note
    if (m_curNote != nullptr) {
        m_curNote->setTitle( m_titleWidget->text() );
        m_curNote->setText( m_editor->toMarkdown() );
    }

    // Change to requested note
    m_curNote = note;
    m_titleWidget->setText(note->title());
    m_editor->setMarkdown(note->text());
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
