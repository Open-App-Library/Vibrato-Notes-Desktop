#include "trashitem.h"
#include "ui_trashitem.h"

TrashItem::TrashItem(Note *note, QListWidget *parent) :
  QListWidgetItem(parent),
  ui(new Ui::TrashItem),
  m_note(note)
{
  widget = new QWidget();
  ui->setupUi(widget);

  this->listWidget()->setItemWidget(this, widget);
  this->setSizeHint( QSize(this->sizeHint().width(), 40));

  connect(ui->checkbox, &QCheckBox::stateChanged,
          this, &TrashItem::handleItemCheckedOrUnchecked);

  updateLabels();
}

TrashItem::~TrashItem()
{
  delete ui;
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

void TrashItem::updateLabels() {
  ui->checkbox->setText( m_note->title() );
}

void TrashItem::handleItemCheckedOrUnchecked(void)
{
  emit itemCheckedOrUnchecked(this);
}
