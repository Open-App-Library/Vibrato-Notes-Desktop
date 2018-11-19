#ifndef MANAGENOTENOTEBOOKSDIALOG_H
#define MANAGENOTENOTEBOOKSDIALOG_H
#include <QDialog>
#include <QPushButton>
#include "../../meta/note.h"
#include "../../meta/db/database.h"

namespace Ui {
class Note_EditNotebook;
}

class Note_EditNotebook : public QDialog
{
    Q_OBJECT

public:
    explicit Note_EditNotebook(Database *db, Note *note, QWidget *parent = nullptr);
    ~Note_EditNotebook();

    Note *note();

private slots:
    void noteChanged(void);

private:
    Ui::Note_EditNotebook *ui;

    void updateTitle();

    QPushButton *m_selectNotebook;

    Database *m_db=nullptr;
    Note *m_note=nullptr;

};

#endif // MANAGENOTENOTEBOOKSDIALOG_H
