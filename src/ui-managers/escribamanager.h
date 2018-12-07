#ifndef ESCRIBAMANAGER_H
#define ESCRIBAMANAGER_H

#include <QObject>
#include <escriba.h>
#include "../meta/note.h"
#include "../meta/db/database.h"
#include "../../ui/note_editnotebook.h"
#include "../../ui/note_edittags.h"
#include <ui_escribaaddons.h>

namespace Ui {
  class EscribaAddonsWidget;
}

class EscribaManager : public QObject
{
  Q_OBJECT

public:
  EscribaManager(Escriba *editor, Database *db);

  void updateTagsButtonCounter();

  void setNote( Note *note );

  // Clear the note selection. Disable editor.
  void deselect();

  Note *note();

public slots:
  void contentChangedFromEditor(QString markdown);
  void titleChangedFromEditor(QString title);
  void addTag();

  void openNotebookEditor();
  void openTagsEditor();

  void focusEditor();

  void noteChanged();

  void updateNotebookWidget(void);
  void updateDateWidgets(void);

private slots:
  void aNoteWasRemoved(int noteID);
  void notebooksRemoved(QVector<int> notebookIDs);
  void noteIDChanged(Note *note);
  void notebookChanged(Notebook *notebook);

private:
  Escriba  *m_editor;
  Database *m_db;
  Ui::EscribaAddonsWidget *m_addons_ui;
  Note *m_curNote=nullptr;
  Notebook *m_curNotebook=nullptr;
  int m_id=-1;
  int m_notebook_id=NOTEBOOK_DEFAULT_NOTEBOOK_ID;

  QLineEdit *m_titleWidget;
  QLineEdit *m_tagsInputWidget;
  QToolButton *m_tagsViewerWidget;
  QToolButton *m_notebookWidget;
  QToolButton *m_moreWidget;
  QToolButton *m_trashWidget;
  QLabel *m_dateCreatedWidget;
  QLabel *m_dateModifiedWidget;

  Note_EditNotebook *m_editNotebookDialog = nullptr;
  Note_EditTags     *m_editTagsDialog = nullptr;
};

#endif
