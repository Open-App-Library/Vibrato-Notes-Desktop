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
          this, &TrashItem::stateChanged);

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

void TrashItem::updateLabels() {
  ui->checkbox->setText( m_note->title() );
}
