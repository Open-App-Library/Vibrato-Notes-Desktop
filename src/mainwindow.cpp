#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_notes     = new NoteDatabase;
  m_notebooks = new NotebookDatabase(m_notes);
  m_tags      = new TagDatabase;
  m_db        = new Database(m_notes, m_notebooks, m_tags);

  m_notes->loadDummyNotes();
  m_notebooks->loadDummyNotebooks();
  m_tags->loadDummyTags();

  m_escriba_manager   = new EscribaManager(ui->noteEditingArea, m_db);
  m_note_list_manager = new NoteListManager(ui->noteList, ui->noteListAddons, m_escriba_manager, m_db);
  m_tree_manager      = new TreeManager(ui->TheTree, m_note_list_manager, m_db);

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
  //  QVariant vark(QVector<int>, list);
  //  ui->noteEditingArea->setMarkdown(n->title(), n->text(), const_cast<n);
  m_escriba_manager->setNote(n);
}

void MainWindow::userButtonClicked()
{
  m_user_window.show();
}

void MainWindow::addNewNote()
{
  m_note_list_manager->add_note( m_notes->addDefaultNote() );
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
