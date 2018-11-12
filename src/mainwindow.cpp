#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_notes     = new NoteDatabase;
    m_notebooks = new NotebookDatabase;
    m_tags      = new TagDatabase;
    m_db        = new Database(m_notes, m_notebooks, m_tags);

    m_notes->loadDummyNotes();
    m_notebooks->loadDummyNotebooks();
    m_tags->loadDummyTags();

    m_note_list_manager = new NoteListManager(ui->noteList, m_db, ui->noteEditingArea);
    m_tree_manager      = new TreeManager(ui->TheTree, m_note_list_manager);

    m_tree_manager->loadNotebooksFromNotebookDatabase(m_notebooks);
    m_tree_manager->loadTagsFromTagDatabase(m_tags);
    m_note_list_manager->loadNotesFromNoteDatabase(m_notes);

		ui->noteEditingArea->setMarkdown("My Incredible Document", "# Welcome!\n\n**TEST**");

    // ------------------------------------------------------
    // Restoring config variables if exist. Else set default.
    // ------------------------------------------------------
    if ( meta_config_key_exists(LAST_OPENED_WINDOW_SIZE) ) {
        this->restoreGeometry( meta_config_value(LAST_OPENED_WINDOW_SIZE).toByteArray() );
    }
    if ( meta_config_key_exists(MAIN_SCREEN_LAYOUT) ) {
        ui->mainSplitter->restoreState( meta_config_value(MAIN_SCREEN_LAYOUT).toByteArray() );
    } else {
        QList<int> splitterSizes = {0,0,0};
        int window_width = this->width();
        splitterSizes[0] = window_width / 6;     // First split is 1/6 of the screen
        splitterSizes[1] = window_width / 6;     // Second split is 1/6 of the screen
        splitterSizes[2] = window_width / 6 * 4; // Last split occupies 4/6 of the screen
        ui->mainSplitter->setSizes(splitterSizes);
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
    connect(ui->userButton, &QPushButton::clicked,
            this, &MainWindow::userButtonClicked);
}

MainWindow::~MainWindow()
{

    set_meta_config_value( LAST_OPENED_WINDOW_SIZE, this->saveGeometry() );
    set_meta_config_value( MAIN_SCREEN_LAYOUT, ui->mainSplitter->saveState() );


    delete m_note_list_manager;
    delete m_tree_manager;
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

void MainWindow::userButtonClicked()
{
    m_user_window.show();
}
