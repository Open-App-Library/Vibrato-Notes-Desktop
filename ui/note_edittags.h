#ifndef NOTE_EDITTAGS_H
#define NOTE_EDITTAGS_H
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <QCompleter>
#include <QStringListModel>
#include "../../meta/note.h"
#include "../../meta/db/database.h"
#include "../../models/items/listitemwithid.h"

namespace Ui {
  class Note_EditTags;
}

class Note_EditTags : public QDialog
{
  Q_OBJECT

public:
  explicit Note_EditTags(Database *db, Note *note, QWidget *parent = nullptr);
  ~Note_EditTags();

  Note *note();

  virtual void keyPressEvent(QKeyEvent *event) override;

private slots:
  void noteChanged(void);
  void updateTagsCompletionList(void);
  void removeTagsFromNote();
  void addTag();

  void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

signals:
  void tagChanged();

private:
  Ui::Note_EditTags *ui;

  void updateTitle();

  QListWidget *m_tagList;
  QPushButton *m_removeTagButton;
  QLineEdit   *m_tagInput;

  QCompleter *m_completer;
  QStringListModel *m_completionList;

  Database *m_db=nullptr;
  Note *m_note=nullptr;

  QVector<ListItemWithID*> m_listItems;

  // private functions
  void clearList();
  void addTagToList(Tag *tag);
  void loadNotesTags();

};

#endif // MANAGENOTENOTEBOOKSDIALOG_H
