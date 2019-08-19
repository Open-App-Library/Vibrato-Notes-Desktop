/*
 * This is a custom QLineEdit that disables the typical
 * tab behavior and adds a signal - tabPressed - that
 * the develop could take advantage of. Enjoy! :)
 */

#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H
#include <QLineEdit>

class CustomLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  CustomLineEdit(QWidget *parent=nullptr);

  void setDisableTypicalTabKeyBehavior(bool disableTypicalTabKeyBehavior=true);

  bool event(QEvent *event) override;

signals:
  void tabPressed();

private:
  bool m_disableTypicalTabKeyBehavior = true;
};

#endif // CUSTOMLINEEDIT_H
