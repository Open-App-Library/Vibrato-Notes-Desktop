#include "trashitem.h"
#include "ui_trashitem.h"
#include <QSizePolicy>

TrashItem::TrashItem(Note *note, QListWidget *parent) :
  QListWidgetItem(parent),
  ui(new Ui::TrashItem),
  m_note(note),
  m_noteID(note->id())
{
  widget = new QWidget();
  ui->setupUi(widget);

  listWidget()->setItemWidget(this, widget);
  setSizeHint( QSize(this->sizeHint().width(), 40));

  ui->checkbox->setSizePolicy( QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed) );

  connect(ui->checkbox, &QCheckBox::stateChanged,
          this, &TrashItem::handleItemCheckedOrUnchecked);

  connect(ui->deleteButton, &QToolButton::pressed,
          this, &TrashItem::emitDeleteNoteSignal);

  connect(ui->restoreButton, &QToolButton::pressed,
          this, &TrashItem::emitRestoreNoteSignal);

  connect(note, &Note::noteRestored,
          this, &TrashItem::handlePleaseDeleteTrashItem);

  updateLabels();
}

TrashItem::~TrashItem()
{
  delete ui;
}

void TrashItem::updateLabels() {
  ui->checkbox->setText( m_note->title() );
}

QCheckBox *TrashItem::checkbox() const
{
  return ui->checkbox;
}

bool TrashItem::checked() const
{
  return ui->checkbox->isChecked();
}

Note *TrashItem::note() const
{
  return m_note;
}

int TrashItem::noteID() const
{
  return m_noteID;
}

void TrashItem::emitDeleteNoteSignal() {
  emit deleteNote(m_note);
  emit pleaseDeleteTrashItem(this);
}

void TrashItem::emitRestoreNoteSignal() {
  emit restoreNote(m_note);
  // Don't need pleaseDeleteTrashItem since it is already going to fire off
  // when the note is restored.
}

void TrashItem::handleItemCheckedOrUnchecked(void)
{
  emit itemCheckedOrUnchecked(this);
}

void TrashItem::handlePleaseDeleteTrashItem(void) {
  emit pleaseDeleteTrashItem(this);
}
