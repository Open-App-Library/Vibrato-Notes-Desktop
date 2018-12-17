#include "notelistitemwidget.h"
#include <ui_notelistitem.h>
#include <QDebug>

NoteListItemWidget::NoteListItemWidget( Note *note ) : m_note(note)
{
  m_ui_class = new Ui::NoteListItem;
  m_ui_class->setupUi( this );

  m_title_label        = m_ui_class->titleLabel;
  m_excerpt_label      = m_ui_class->excerptLabel;
  m_date_created_label = m_ui_class->dateCreatedLabel;
  m_favoriteButton     = m_ui_class->favoriteButton;

  updateLabels();

  connect(note, &Note::noteDateCreatedChanged,
          this, &NoteListItemWidget::noteDateChanged);
  connect(note, &Note::noteDateModifiedChanged,
          this, &NoteListItemWidget::noteDateChanged);
  connect(m_favoriteButton, &QToolButton::clicked,
          this, &NoteListItemWidget::toggleFavorited);
}

NoteListItemWidget::~NoteListItemWidget()
{
  delete m_ui_class;
}

Note *NoteListItemWidget::note() const
{
  return m_note;
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
  updateTitleLabel();
  updateExcerptWidget();
  updateDateLabel();
  updateFavoriteButton();
}

void NoteListItemWidget::updateTitleLabel() {
  m_title_label->setText( m_note->title() );
}

void NoteListItemWidget::updateExcerptWidget() {
  QString excerpt = m_note->text();
  if (excerpt.length() > 50) {
    excerpt = m_note->text().mid(0, 50) + "...";
  }
  excerpt.replace("\n", " ");
  m_excerpt_label->setText(excerpt);
}

void NoteListItemWidget::updateDateLabel() {
  m_date_created_label->setText(m_note->date_created_str());
}

void NoteListItemWidget::noteDateChanged(Note *note)
{
  (void)note;
  updateLabels();
}

void NoteListItemWidget::updateFavoriteButton() {
  if ( m_note->favorited() ) {
    m_favoriteButton->setIcon( QIcon::fromTheme("vibrato-draw-star-solid") );
  } else {
    m_favoriteButton->setIcon( QIcon::fromTheme("vibrato-draw-star") );
  }
  m_favoriteButton->setChecked( m_note->favorited() );
}

void NoteListItemWidget::toggleFavorited() {
  m_note->setFavorited( !m_note->favorited() );
  updateFavoriteButton();
}
