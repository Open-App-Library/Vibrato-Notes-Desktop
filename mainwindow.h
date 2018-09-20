#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include "treemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TreeModel *m_notebooks;
};

#endif // NOTES_H
