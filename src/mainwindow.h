#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include "userwindow.h"
#include "meta/db/notedatabase.h"
#include "notelistmanager.h"
#include "treemanager.h"

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

    void loadNotebookAndChildren(QJsonObject notebookObj, QTreeWidgetItem *parent=nullptr);
    void loadDummyData();

private:
    Ui::MainWindow *ui;
    UserWindow m_user_window;
    NoteListManager *m_note_list_manager;
    TreeManager     *m_tree_manager;
    // List of user's notes
    NoteDatabase m_notes;

public slots:
    void userButtonClicked();
};

#endif // NOTES_H
