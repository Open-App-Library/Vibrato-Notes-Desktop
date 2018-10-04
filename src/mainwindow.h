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
#include "ui-managers/notelistmanager.h"
#include "ui-managers/treemanager.h"
#include "userwindow.h"

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

    void loadDummyData();

private:
    Ui::MainWindow *ui;
    UserWindow m_user_window;
    NoteListManager *m_note_list_manager;
    TreeManager     *m_tree_manager;

    // Where we store user data
    Database         *m_db; // Contains all three databases below
    NoteDatabase     *m_notes;
    NotebookDatabase *m_notebooks;
    TagDatabase      *m_tags;

public slots:
    void userButtonClicked();
};

#endif // NOTES_H
