#include "escribamanager.h"
#include <QDebug>

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
    connect(m_tagsInputWidget, &QLineEdit::returnPressed,
            this, &EscribaManager::addTag);

}

void EscribaManager::updateTagsButtonCounter()
{
    m_tagsViewerWidget->setText( QString("Manage Tags (%1)").arg( m_curNote->tags().count() ) );
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
    updateTagsButtonCounter();

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
