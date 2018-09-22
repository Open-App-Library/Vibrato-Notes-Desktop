#include <QSettings>
#include <QFile>
#include <QtDebug>
#include "appinfo.h"
#include "appconfig.h"
#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file(":/dummy-content.txt");
    file.open(QIODevice::ReadOnly);
    QStringList notebook_column_headers;
    notebook_column_headers << tr("Notebooks");
    m_notebooks = new TreeModel(notebook_column_headers, file.readAll());
    file.close();

    ui->notebookTreeView->setModel(m_notebooks);

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
}

MainWindow::~MainWindow()
{

    set_meta_config_value( LAST_OPENED_WINDOW_SIZE, this->saveGeometry() );
    set_meta_config_value( MAIN_SCREEN_LAYOUT, ui->mainSplitter->saveState() );

    delete m_notebooks;
    delete ui;
}
