#ifndef TRASHITEM_H
#define TRASHITEM_H
#include <QListWidgetItem>
#include <QCheckBox>
#include "../src/meta/note.h"

namespace Ui {
class TrashItem;
}

class TrashItem : public QObject, public QListWidgetItem
{
  Q_OBJECT

public:
  explicit TrashItem(Note *note, QListWidget *parent = nullptr);
  ~TrashItem();

  void updateLabels();

  QCheckBox *checkbox() const;

signals:
  void noteDeleted(TrashItem *item, int noteID);
  void stateChanged(int state);

private:
  Ui::TrashItem *ui;
  QWidget *widget=nullptr;
  Note *m_note=nullptr;
};

#endif // TRASHITEM_H
