#include "notebook_editparent.h"
#include "ui_notebook_editparent.h"

Notebook_EditParent::Notebook_EditParent(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Notebook_EditParent)
{
  ui->setupUi(this);
}

Notebook_EditParent::~Notebook_EditParent()
{
  delete ui;
}
