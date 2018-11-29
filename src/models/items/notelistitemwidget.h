#ifndef NOTELISTITEMWIDGET_H
#define NOTELISTITEMWIDGET_H
#include "../../meta/note.h"
#include <QLabel>

namespace Ui {
    class NoteListItem;
}

class NoteListItemWidget : public QWidget
{
    Q_OBJECT
public:
    NoteListItemWidget(Note *note);
    ~NoteListItemWidget();

    void setNote(Note *note);

    void updateLabels();
    void setSelectedStyle(bool selected);

    Note *note() const;
private:
    Ui::NoteListItem *m_ui_class;

    Note *m_note;

    QLabel *m_title_label;
    QLabel *m_excerpt_label;
    QLabel *m_date_created_label;
private slots:
    void noteDateChanged(Note *note);
};

#endif // NOTELISTITEMWIDGET_H
