#include "customlineedit.h"
#include <QKeyEvent>

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

void CustomLineEdit::setDisableTypicalTabKeyBehavior(bool disableTypicalTabKeyBehavior) {
  m_disableTypicalTabKeyBehavior = disableTypicalTabKeyBehavior;
}

bool CustomLineEdit::event(QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Tab) {
      emit tabPressed();
      if (m_disableTypicalTabKeyBehavior)
        return true;
      else
        return QLineEdit::event(event);
    }
  }
  return QLineEdit::event(event);
}
