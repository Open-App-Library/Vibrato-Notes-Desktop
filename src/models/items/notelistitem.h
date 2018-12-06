#ifndef NOTELISTITEM_H
#define NOTELISTITEM_H
#include <QObject>
#include <QListView>
#include <QLabel>
#include "../../meta/note.h"

#define NOTE_LIST_ITEM_HEIGHT 90

namespace Ui {
  class NoteListItem;
}

class NoteListItem : public QObject
{
  Q_OBJECT
public:
  NoteListItem(Note *note);
  ~NoteListItem();
  Note *note();
  void setNote(Note *note);

  QWidget *widget();
  void setWidget(QWidget *widget);

  void setSelectedStyle(bool selected);

  int id();

private:
  Ui::NoteListItem *m_ui_class;
  QWidget *m_widget=nullptr;

  Note *m_note;
  int m_id;

  QLabel *m_title_label;
  QLabel *m_excerpt_label;
  QLabel *m_date_created_label;

signals:
  void noteDateChanged(NoteListItem *item);

private slots:
  void noteIDChanged(Note *note);
public slots:
  void noteDateChanged_slot(Note *note);

};

#endif // NOTELISTITEM_H
