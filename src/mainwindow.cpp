#include <QSettings>
#include <QFile>
#include <QtDebug>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "helper-io.h"
#include "appinfo.h"
#include "appconfig.h"
#include "mainwindow.h"
#include "notelistmanager.h"
#include "treemanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_note_list_manager = new NoteListManager(ui->noteList);
    m_tree_manager      = new TreeManager(ui->TheTree);

    if ( meta_config_key_exists(LAST_OPENED_WINDOW_SIZE) ) {
        this->restoreGeometry( meta_config_value(LAST_OPENED_WINDOW_SIZE).toByteArray() );
    }

    if ( meta_config_key_exists(MAIN_SCREEN_LAYOUT) ) {
        ui->mainSplitter->restoreState( meta_config_value(MAIN_SCREEN_LAYOUT).toByteArray() );
    } else {
        // Set the default window size
        QList<int> splitterSizes = {0,0,0};
        int window_width = this->width();
        splitterSizes[0] = window_width / 6;     // First split is 1/6 of the screen
        splitterSizes[1] = window_width / 6;     // Second split is 1/6 of the screen
        splitterSizes[2] = window_width / 6 * 4; // Last split occupies 4/6 of the screen
        ui->mainSplitter->setSizes(splitterSizes);
    }

    m_notes.loadDummyNotes();

    for (int i = 0; i < m_notes.list().length(); i++) {
        m_note_list_manager->add_note(m_notes.list()[i]);
    }

    // Remove margin on toolbar on Mac OS X
#ifdef Q_OS_MAC
    ui->customToolbar->layout()->setContentsMargins(0,0,0,0);
#endif
    // Hide the header of the notebook tree widget

    ui->TheTree->header()->hide();

    connect(ui->userButton, &QPushButton::clicked,
            this, &MainWindow::userButtonClicked);
}

MainWindow::~MainWindow()
{

    set_meta_config_value( LAST_OPENED_WINDOW_SIZE, this->saveGeometry() );
    set_meta_config_value( MAIN_SCREEN_LAYOUT, ui->mainSplitter->saveState() );

    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    m_user_window.close();
    event->accept();
}

void MainWindow::loadNotebookAndChildren(QJsonObject notebookObj, QTreeWidgetItem *parent)
{
    QString notebook_title = notebookObj.value("title").toString();
    QJsonArray children = notebookObj["children"].toArray();
    QTreeWidgetItem *newitem;
    if (parent == nullptr) {
        newitem = m_tree_manager->addNotebook(notebook_title);
    } else {
        newitem = m_tree_manager->addNotebook(notebook_title, parent);
    }
    for (int i = 0; i < children.size(); i++) {
        loadNotebookAndChildren(children[i].toObject(), newitem);
    }
}

void MainWindow::loadDummyData()
{
    QJsonDocument notebooks = fileToQJsonDocument(":/dummy/notebooks.json");
    QJsonArray notebookArray = notebooks.array();
    for (int i = 0; i < notebookArray.size(); i++) {
        QJsonObject val = notebookArray[i].toObject();
        loadNotebookAndChildren(val);
    }

   QJsonDocument tags = fileToQJsonDocument(":/dummy/tags.json");
   QJsonArray tagArray = tags.array();
   for (int i = 0; i < tagArray.size(); i++) {
       QJsonObject val = tagArray[i].toObject();
       m_tree_manager->addTag(val["title"].toString());
   }
}

void MainWindow::userButtonClicked()
{
    m_user_window.show();
}
