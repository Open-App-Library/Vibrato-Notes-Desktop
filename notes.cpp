#include <QSettings>
#include "appinfo.h"
#include "notes.h"
#include "ui_notes.h"

#include <iostream>

Notes::Notes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notes)
{
    ui->setupUi(this);
}

Notes::~Notes()
{
    QSettings meta_config(COMPANY, CONFIG_META);
    meta_config.setValue("last_opened_window_size", this->saveGeometry());

    delete ui;
}
