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
  bool checked() const;
  Note *note() const;
  QUuid noteSyncHash() const;

  void emitDeleteNoteSignal();
  void emitRestoreNoteSignal();

signals:
  void deleteNote(Note *note);
  void restoreNote(Note *note);
  void itemCheckedOrUnchecked(TrashItem *item);
  void noteDeleted(TrashItem *item, QUuid noteSyncHash);
  void pleaseDeleteTrashItem(TrashItem *item);

private slots:
  void handleItemCheckedOrUnchecked(void);
  void handlePleaseDeleteTrashItem(void);

private:
  Ui::TrashItem *ui;
  QWidget *widget=nullptr;
  Note *m_note=nullptr;
  QUuid m_noteSyncHash=nullptr;
};

#endif // TRASHITEM_H
