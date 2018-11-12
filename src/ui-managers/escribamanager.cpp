#include "escribamanager.h"
#include <QDebug>

EscribaManager::EscribaManager(Escriba *editor) :
	m_editor(editor)
{
	connect(editor, &Escriba::documentTitleChanged,
					this, &EscribaManager::titleChangedFromEditor);
}

void EscribaManager::setNote( Note *note )
{
	// Save current note
	if (m_curNote != nullptr) {
		m_curNote->setTitle( m_editor->title() );
		m_curNote->setText( m_editor->toMarkdown() );
	}

	// Change to requested note
	m_curNote = note;
	m_editor->setMarkdown(note->title(), note->text());
}

Note *EscribaManager::note()
{
	return m_curNote;
}

void EscribaManager::titleChangedFromEditor(QString title)
{
	if (!m_curNote)
		return;
	if (!QString::compare(m_curNote->title(), title)) // If titles are same, exit
		return;
	m_curNote->setTitle( title );
}
