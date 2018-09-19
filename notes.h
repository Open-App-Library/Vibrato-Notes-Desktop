#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>

namespace Ui {
class Notes;
}

class Notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr);
    ~Notes();

private:
    Ui::Notes *ui;
};

#endif // NOTES_H
