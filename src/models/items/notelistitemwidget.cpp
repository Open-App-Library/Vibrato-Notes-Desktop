#include "notelistitemwidget.h"
#include <ui_notelistitem.h>

NoteListItemWidget::NoteListItemWidget( Note *note ) : m_note(note)
{
    m_ui_class = new Ui::NoteListItem;
    m_ui_class->setupUi( this );

    m_title_label        = m_ui_class->titleLabel;
    m_excerpt_label      = m_ui_class->excerptLabel;
    m_date_created_label = m_ui_class->dateCreatedLabel;

    updateLabels();

    connect(note, &Note::noteDateCreatedChanged,
            this, &NoteListItemWidget::noteDateChanged);
    connect(note, &Note::noteDateModifiedChanged,
            this, &NoteListItemWidget::noteDateChanged);
}

NoteListItemWidget::~NoteListItemWidget()
{
    delete m_ui_class;
}

void NoteListItemWidget::setNote(Note *note)
{
    disconnect(m_note, &Note::noteDateCreatedChanged,
            this, &NoteListItemWidget::noteDateChanged);
    disconnect(m_note, &Note::noteDateModifiedChanged,
            this, &NoteListItemWidget::noteDateChanged);

    m_note = note;

    connect(m_note, &Note::noteDateCreatedChanged,
            this, &NoteListItemWidget::noteDateChanged);
    connect(m_note, &Note::noteDateModifiedChanged,
            this, &NoteListItemWidget::noteDateChanged);

    updateLabels();
}

void NoteListItemWidget::updateLabels()
{
    // Set title and excerpt
    m_title_label->setText( m_note->title() );
    QString excerpt = m_note->text();
    if (excerpt.length() > 50) {
        excerpt = m_note->text().mid(0, 50) + "...";
    }
    excerpt.replace("\n", " ");
    m_excerpt_label->setText(excerpt);

    // Set date created label
    m_date_created_label->setText(m_note->date_created_str());
}

void NoteListItemWidget::noteDateChanged(Note *note)
{
    (void)note;
    updateLabels();
}
