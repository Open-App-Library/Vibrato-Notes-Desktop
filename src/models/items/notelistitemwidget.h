#ifndef NOTELISTITEMWIDGET_H
#define NOTELISTITEMWIDGET_H
#include "../../meta/note.h"
#include <QLabel>
#include <QToolButton>

namespace Ui {
  class NoteListItem;
}

class NoteListItemWidget : public QWidget
{
  Q_OBJECT
public:
  NoteListItemWidget(Note *note);
  ~NoteListItemWidget();

  Note *note() const;
  void setNote(Note *note);

  void updateLabels();
  void updateTitleLabel();
  void updateExcerptWidget();
  void updateDateLabel();
  void updateFavoriteButton(void);

  void setSelectedStyle(bool selected);

  void toggleFavorited();

  void connectSignals();
  void disconnectSignals();

private:
  Ui::NoteListItem *m_ui_class;

  Note *m_note;

  QLabel *m_title_label;
  QLabel *m_excerpt_label;
  QLabel *m_date_created_label;
  QToolButton *m_favoriteButton;

private slots:
  void noteDateChanged(Note *note);
  void noteTitleChanged(Note *note);
  void noteTextChanged(Note *note);
};

#endif // NOTELISTITEMWIDGET_H
