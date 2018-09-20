#include <QSettings>
#include <QFile>
#include "appinfo.h"
#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QtDebug>

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
    for (int column = 0; column < m_notebooks->columnCount(); ++column)
        ui->notebookTreeView->resizeColumnToContents(column);
}

MainWindow::~MainWindow()
{
    QSettings meta_config(COMPANY, CONFIG_META);
    meta_config.setValue("last_opened_window_size", this->saveGeometry());

    delete m_notebooks;
    delete ui;
}
