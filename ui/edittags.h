#ifndef EDITTAGS_H
#define EDITTAGS_H
#include <QDialog>
#include <QVector>
#include "../src/meta/db/database.h"
#include "../src/models/items/listitemwithid.h"

namespace Ui {
  class EditTags;
}

class EditTags : public QDialog
{
  Q_OBJECT

public:
  explicit EditTags(Database *db, QWidget *parent = nullptr);
  ~EditTags();
  void addTag(Tag *tag);
  void loadTags();

private slots:
  void tagAdded(Tag *tag);
  void tagChanged(Tag *tag);

private:
  Ui::EditTags *ui;
  Database *m_db;

  typedef struct { Tag *tag; ListItemWithID *item;} t_tagItem;

  QVector<t_tagItem> m_tagItems;
};

#endif // EDITTAGS_H
