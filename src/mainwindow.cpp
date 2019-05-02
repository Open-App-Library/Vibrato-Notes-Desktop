#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>
#include <QShortcut>
#include "ui-managers/treemanager.h"
#include "ui-managers/notelistmanager.h"
#include "ui-managers/escribamanager.h"

#include "scripting-api/scriptingengine.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_sqlManager        = new SQLManager();
  m_notes     = new NoteDatabase(m_sqlManager);
  m_notebooks = new NotebookDatabase(m_sqlManager, m_notes);
  m_tags      = new TagDatabase(m_sqlManager);
  m_db        = new Database(m_notes, m_notebooks, m_tags);

  ScriptingEngine *engine = new ScriptingEngine();

  m_manager           = new Manager;
  m_tree_manager      = new TreeManager(ui->TheTree, m_db, m_manager);
  m_note_list_manager = new NoteListManager(ui->noteList, ui->noteListAddons, m_db, m_manager);
  m_escriba_manager   = new EscribaManager(ui->noteEditingArea, m_db, m_manager);
  m_manager->setManagers(m_tree_manager, m_note_list_manager, m_escriba_manager);

  m_tree_manager->gotoAllNotesTab();

  m_note_list_manager->loadNotesFromNoteDatabase(m_notes);

  // Open first item in list in editor
  m_note_list_manager->openIndexInEditor(0);

  // ------------------------------------------------------
  // Restoring config variables if exist. Else set default.
  // ------------------------------------------------------
  if ( meta_config_key_exists(LAST_OPENED_WINDOW_SIZE) ) {
    this->restoreGeometry( meta_config_value(LAST_OPENED_WINDOW_SIZE).toByteArray() );
  }
  if ( meta_config_key_exists(MAIN_SCREEN_LAYOUT) ) {
    ui->mainSplitter->restoreState( meta_config_value(MAIN_SCREEN_LAYOUT).toByteArray() );
  } else {
    view_default();
  }

  // Remove margin on toolbar on Mac OS X
#ifdef Q_OS_MAC
  ui->customToolbar->layout()->setContentsMargins(0,0,0,0);
#endif
  // Hide the header of the notebook tree widget

  // Hide the header of the tree widget
  ui->TheTree->header()->hide(); // TODO: Add this to treemanager

  ui->mainSplitter->setHandleWidth(1);

  // -------
  // SIGNALS
  // -------
  //* User Button clicked - the button to sign into account or access account/sync info.
  connect(ui->newNoteButton, &QPushButton::clicked,
          this, &MainWindow::addNewNote);

  connect(ui->userButton, &QPushButton::clicked,
          this, &MainWindow::userButtonClicked);

  connect(m_note_list_manager, &NoteListManager::selectedNote,
          this, &MainWindow::selectedNoteChanged);

  // Menu items
  connect(ui->action_ViewDefault, &QAction::triggered,
          this, &MainWindow::view_default);
  connect(ui->action_ViewMinimal, &QAction::triggered,
          this, &MainWindow::view_minimal);
  connect(ui->action_viewFocus, &QAction::triggered,
          this, &MainWindow::view_focus);

  // Search bar
  connect(ui->searchBar, &QLineEdit::returnPressed,
          this, &MainWindow::search);

  // File Menu
  connect(ui->actionNote, &QAction::triggered,     // New Note
          this, &MainWindow::addNewNote);
  connect(ui->actionNotebook, &QAction::triggered, // New Notebook
          this, &MainWindow::addNewNotebook);
  connect(ui->actionTag, &QAction::triggered,      // New Tag
          this, &MainWindow::addNewTag);

  // -------------
  // SHORTCUT KEYS
  // -------------
  QShortcut *shortcut_searchbar = new QShortcut(QKeySequence(tr("Ctrl+l")), this);
  connect(shortcut_searchbar, &QShortcut::activated,
          this, &MainWindow::focusSearchbar);
}

MainWindow::~MainWindow()
{

  set_meta_config_value( LAST_OPENED_WINDOW_SIZE, this->saveGeometry() );
  set_meta_config_value( MAIN_SCREEN_LAYOUT, ui->mainSplitter->saveState() );


  delete m_note_list_manager;
  delete m_tree_manager;
  delete m_escriba_manager;
  delete m_notes;
  delete m_notebooks;
  delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
  m_user_window.close();
  event->accept();
}

void MainWindow::loadDummyData()
{
  //   QJsonDocument tags = fileToQJsonDocument(":/dummy/tags.json");
  //   QJsonArray tagArray = tags.array();
  //   for (int i = 0; i < tagArray.size(); i++) {
  //       QJsonObject val = tagArray[i].toObject();
  //       m_tree_manager->addTag(val["title"].toString());
  //   }
}

void MainWindow::selectedNoteChanged(Note *n)
{
  m_escriba_manager->setNote(n);
}

void MainWindow::userButtonClicked()
{
  m_user_window.show();
}

void MainWindow::addNewNote()
{
  Note *newNote = m_notes->addDefaultNote();
  if (m_note_list_manager->curViewType() == NoteListManager::View_Notebook) {
    Notebook *notebook = m_note_list_manager->curViewType_Notebook();
    newNote->setNotebook(notebook->syncHash(), false);
  }
  else if (m_note_list_manager->curViewType() == NoteListManager::View_Tag) {
    Tag *tag = m_note_list_manager->curViewType_Tag();
    newNote->setTags({tag->syncHash()});
  }
  m_note_list_manager->openIndexInEditor(0); // Select the first note (Newest note)
}

void MainWindow::addNewNotebook()
{
  m_notebooks->addNotebook(Notebook::createBlankNotebook(), m_note_list_manager->curViewType_Notebook());
}

void MainWindow::addNewTag()
{
  m_tags->addTag(TAG_DEFAULT_TITLE);
}

void MainWindow::view_default()
{
  QList<int> splitterSizes = {0,0,0};
  int window_width = this->width();
  splitterSizes[0] = window_width / 6;     // First split is 1/6 of the screen
  splitterSizes[1] = window_width / 6 + 40;     // Second split is 1/6 of the screen
  splitterSizes[2] = window_width / 6 * 4 - 40; // Last split occupies 4/6 of the screen
  ui->mainSplitter->setSizes(splitterSizes);
}

void MainWindow::view_minimal()
{
  QList<int> splitterSizes = {0,0,0};
  int window_width = this->width();
  splitterSizes[0] = 0;                    // First split is 0/6 of the screen
  splitterSizes[1] = window_width / 6;     // Second split is 1/6 of the screen
  splitterSizes[2] = window_width / 6 * 5; // Last split occupies 5/6 of the screen
  ui->mainSplitter->setSizes(splitterSizes);
}

void MainWindow::view_focus()
{
  QList<int> splitterSizes = {0,0,0};
  int window_width = this->width();
  splitterSizes[0] = 0;     // First split is 1/6 of the screen
  splitterSizes[1] = 0;     // Second split is 1/6 of the screen
  splitterSizes[2] = window_width; // Last split occupies 4/6 of the screen
  ui->mainSplitter->setSizes(splitterSizes);
}

void MainWindow::search() {
  QString searchQuery = ui->searchBar->text().trimmed();
  ui->searchBar->clear();
  if ( searchQuery.isEmpty() )
    return;
  BasicTreeItem *item = m_manager->treeManager()->addSearchQuery(searchQuery);
  m_manager->treeManager()->selectItem(item);
}

void MainWindow::focusSearchbar() {
  ui->searchBar->setFocus();
  ui->searchBar->selectAll();
}
