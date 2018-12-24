#ifndef NOTEBOOK_EDITPARENT_H
#define NOTEBOOK_EDITPARENT_H

#include <QDialog>

namespace Ui {
class Notebook_EditParent;
}

class Notebook_EditParent : public QDialog
{
  Q_OBJECT

public:
  explicit Notebook_EditParent(QWidget *parent = nullptr);
  ~Notebook_EditParent();

private:
  Ui::Notebook_EditParent *ui;
};

#endif // NOTEBOOK_EDITPARENT_H
