#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QSettings>
#include <QFile>
#include <QtDebug>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <helper-io.hpp>
#include "meta/info/appinfo.h"
#include "meta/info/appconfig.h"
#include "meta/db/database.h"
#include "meta/db/notedatabase.h"
#include "meta/db/notebookdatabase.h"
#include "meta/db/tagdatabase.h"
#include "ui-managers/manager.h"
#include "sql/sqlmanager.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void closeEvent (QCloseEvent *event);

public slots:
  void addNewNote();
  void addNewNotebook();
  void addNewTag();

  void activateDefaultView();
  void activateMinimalView();
  void activateFocusView();

  void searchGo(); // Perform search based on what user entered in search bar
  void focusSearchbar();

  void openUserInfoDialog();
  void openNoteInEditor(Note *n);

private:
  Ui::MainWindow *ui;

  SQLManager      *m_sqlManager;
  Manager         *m_manager;
  EscribaManager  *m_escriba_manager;
  NoteListManager *m_note_list_manager;
  TreeManager     *m_tree_manager;

  // Where we store user data
  Database         *m_db; // Contains all three databases below
  NoteDatabase     *m_notes;
  NotebookDatabase *m_notebooks;
  TagDatabase      *m_tags;
};

#endif // NOTES_H
